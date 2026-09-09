// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05.hpp
// ===================
// Runtime SDK: typed, *verified* access to a live retail speed.exe
// (Need for Speed: Most Wanted, 2005, PC v1.3).
//
// This is the header a plugin/trainer DLL includes. It is header-only and
// pulls in no hooking or UI backend - the Constitution's "low coupling" and
// mwsdk.hpp's "zero forced hooking backend" contract. If you want inline hooks,
// add the optional adapter in mwsdk/adapters/vanhooks.hpp; if you only need
// byte patches, this header already gives you a self-contained, RAII patcher.
//
// Everything here is grounded in the generated address database
// (mwsdk/game/mw05_db.inl), which is produced from The Most Wanted
// Encyclopedia's verified RE tables. No address is invented; every Va is
// traceable back to that data. The preferred image base is 0x00400000 and all
// addresses are rebased at runtime, so the SDK survives ASLR/relocation.
//
// Design (mirrors VanHooks):
//   * std::expected (Result<T>) for anything that can fail; no exceptions,
//     no global error state.
//   * RAII for every owned resource (page protection, applied patches).
//   * You pick the calling convention explicitly for raw calls - the x86 game
//     uses __thiscall for methods and __cdecl for the Lua binders; the SDK
//     never guesses an ABI for you.
//
// The pure parts (address math, the symbol/class database, object identity by
// vtable) compile and are unit-tested on any platform. The live parts (reading
// process memory, calling game functions, patching) are Windows-only and are
// guarded by MWSDK_MW05_RUNTIME.

#ifndef MWSDK_GAME_MW05_HPP
#define MWSDK_GAME_MW05_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <type_traits>

#include "mwsdk/core.hpp"     // Result<T>, Error, err()
#include "mwsdk/hashing.hpp"  // hash::attrib - the engine string hash
#include "mwsdk/game/mw05_db.inl"      // generated: address database
#include "mwsdk/game/mw05_layouts.inl" // generated: live object member offsets
#include "mwsdk/vault_schema.inl"      // generated: vault field attrib keys (offline-safe)

#if defined(_WIN32)
#  define MWSDK_MW05_RUNTIME 1
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#else
#  define MWSDK_MW05_RUNTIME 0
#endif

// Calling-convention keywords only matter for 32-bit MSVC (the real target).
// Everywhere else - x64, GCC, Clang - they collapse to nothing so the header
// still parses and the pure logic can be tested.
#if defined(_MSC_VER) && !defined(_WIN64)
#  define MWSDK_CDECL    __cdecl
#  define MWSDK_STDCALL  __stdcall
#  define MWSDK_FASTCALL __fastcall
#  define MWSDK_THISCALL __thiscall
#else
#  define MWSDK_CDECL
#  define MWSDK_STDCALL
#  define MWSDK_FASTCALL
#  define MWSDK_THISCALL
#endif

namespace mwsdk::mw05 {

using db::Va;

// Convenience re-exports so callers can write mw05::lua::Game::BlowEngine,
// mw05::fn::AttribCollection_GetField, mw05::singleton::WRoadNetwork.
namespace fn        = db::fn;
namespace lua       = db::lua;
namespace singleton = db::singleton;

// The engine string hash, one namespace up for convenience: mw05::attrib(...).
using hash::attrib;

// ===========================================================================
// Image - the rebasing anchor.
//
// speed.exe is linked for base 0x00400000. When it actually loads at some
// other base (ASLR/relocation), a preferred Va V maps to
//   live = actual_base + (V - kPreferredBase).
// Image is a tiny value type that does exactly this. It is constexpr-friendly
// and needs no OS, so all rebasing logic is unit-testable off-target.
// ===========================================================================
class Image {
public:
    constexpr Image() noexcept = default;
    constexpr explicit Image(std::uintptr_t base) noexcept : base_(base) {}

    constexpr std::uintptr_t base()  const noexcept { return base_; }
    constexpr bool           valid() const noexcept { return base_ != 0; }

    // Preferred Va -> live linear address.
    constexpr std::uintptr_t rebase(Va va) const noexcept {
        return base_ + (static_cast<std::uintptr_t>(va) - db::kPreferredBase);
    }
    // Live linear address -> preferred Va (for identifying a pointer).
    constexpr Va unbase(std::uintptr_t live) const noexcept {
        return static_cast<Va>(live - base_ + db::kPreferredBase);
    }
    // Typed pointer to a rebased Va.
    template <class T>
    T* ptr(Va va) const noexcept {
        return reinterpret_cast<T*>(rebase(va));
    }

private:
    std::uintptr_t base_ = 0;
};

// ===========================================================================
// Symbol & class database lookups (pure; usable anywhere).
// ===========================================================================

// Exact-name symbol lookup (linear; the table is small enough and names are
// not sorted). Returns nullptr if unknown.
[[nodiscard]] inline const db::Symbol* find_symbol(std::string_view name) noexcept {
    for (const auto& s : db::kSymbols)
        if (s.name == name) return &s;
    return nullptr;
}

// Symbol covering an exact Va (binary search; kSymbols is sorted by va).
[[nodiscard]] inline const db::Symbol* symbol_at(Va va) noexcept {
    std::size_t lo = 0, hi = db::kSymbolCount;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (db::kSymbols[mid].va < va)       lo = mid + 1;
        else if (db::kSymbols[mid].va > va)  hi = mid;
        else return &db::kSymbols[mid];
    }
    return nullptr;
}

// Reflected class by exact name (e.g. "AIActionAirborne").
[[nodiscard]] inline const db::ClassInfo* class_info(std::string_view name) noexcept {
    for (const auto& c : db::kClasses)
        if (c.name == name) return &c;
    return nullptr;
}

// Reflected class by its vtable Va (preferred address space).
[[nodiscard]] inline const db::ClassInfo* class_by_vtable(Va vtable_va) noexcept {
    for (const auto& c : db::kClasses)
        if (c.vtable_va == vtable_va) return &c;
    return nullptr;
}

// ---------------------------------------------------------------------------
// Typed member access at a verified byte offset within a live object.
// Grounded in mw05::layout::<Class>::<member> (Encyclopedia Discovery 34).
// Returns a reference, so it reads AND writes:
//
//   float& top = mw05::field<float>(vehicle, layout::AIVehicle::mTopSpeed);
//   top *= 1.10f;                       // +10% top speed
//   mw05::field<std::uint32_t>(perp, layout::AIPerpVehicle::mBustableFlag) = 0;
//
// `obj` must be a live object pointer of the matching class (identify() it if
// unsure). This is pure pointer arithmetic; it needs no OS and is testable.
// ---------------------------------------------------------------------------
template <class T>
[[nodiscard]] T& field(void* obj, Off off) noexcept {
    return *reinterpret_cast<T*>(static_cast<std::uint8_t*>(obj) + off);
}
template <class T>
[[nodiscard]] const T& field(const void* obj, Off off) noexcept {
    return *reinterpret_cast<const T*>(static_cast<const std::uint8_t*>(obj) + off);
}

// ===========================================================================
// Raw typed function access. You choose the ABI - the SDK never guesses.
//
//   as<Sig>(img, va)   : cast a Va to any function-pointer type you name
//                        (put __thiscall/__stdcall in your typedef as needed).
//   Fn<R(A...)>        : a bound __cdecl callable - the common case, and the
//                        ABI of the Lua binder natives (int(lua_State*)).
//   ThisFn<R(Self,A)>  : a bound __thiscall callable for engine methods; the
//                        first argument is the object pointer (ecx).
// ===========================================================================

// Escape hatch: reinterpret a Va as exactly the pointer type you specify.
template <class FnPtr>
[[nodiscard]] FnPtr as(const Image& img, Va va) noexcept {
    static_assert(std::is_pointer_v<FnPtr>,
                  "mw05::as<FnPtr> expects a function-pointer type");
    return reinterpret_cast<FnPtr>(img.rebase(va));
}

// A bound __cdecl function.
template <class Sig> class Fn;
template <class R, class... A>
class Fn<R(A...)> {
public:
    using Pointer = R(MWSDK_CDECL*)(A...);
    Fn() noexcept = default;
    explicit Fn(Pointer p) noexcept : p_(p) {}
    Fn(const Image& img, Va va) noexcept
        : p_(reinterpret_cast<Pointer>(img.rebase(va))) {}
    R operator()(A... args) const { return p_(args...); }
    [[nodiscard]] Pointer get() const noexcept { return p_; }
    explicit operator bool() const noexcept { return p_ != nullptr; }
private:
    Pointer p_ = nullptr;
};

// A bound __thiscall member function. Self is the object-pointer type.
template <class Sig> class ThisFn;
template <class R, class Self, class... A>
class ThisFn<R(Self, A...)> {
public:
    using Pointer = R(MWSDK_THISCALL*)(Self, A...);
    ThisFn() noexcept = default;
    explicit ThisFn(Pointer p) noexcept : p_(p) {}
    ThisFn(const Image& img, Va va) noexcept
        : p_(reinterpret_cast<Pointer>(img.rebase(va))) {}
    R operator()(Self self, A... args) const { return p_(self, args...); }
    [[nodiscard]] Pointer get() const noexcept { return p_; }
    explicit operator bool() const noexcept { return p_ != nullptr; }
private:
    Pointer p_ = nullptr;
};

// The ABI of every Lua binder native in db::lua (int(lua_State*)). Treat the
// lua_State as opaque - you only need this type to hook or forward a native.
using LuaCFunction = int(MWSDK_CDECL*)(void* lua_state);

// ===========================================================================
// Image-based typed access (pure pointer math - OS-free, unit-testable).
// ===========================================================================

// Unchecked typed read. Precondition: the Va is mapped and readable. Fast.
template <class T>
[[nodiscard]] T read(const Image& img, Va va) noexcept {
    static_assert(std::is_trivially_copyable_v<T>, "read<T> needs trivially-copyable T");
    T v{};
    std::memcpy(&v, img.template ptr<const void>(va), sizeof(T));
    return v;
}

// Read a singleton instance pointer out of its global slot.
//   ptr = *(void**)rebase(global_va)
// The confirmed slots are singleton::WRoadNetwork, singleton::CameraAI and
// singleton::GManager (D36: a GManager** cell, populated at runtime — before
// the icon system is up it still holds the 0x0F0F0F0F debug fill, which this
// helper reports as nullptr rather than a fake instance).
// The instance layout is not fully reversed, so these come back as void*; cast
// only against a layout you have verified yourself.
[[nodiscard]] inline void* singleton_ptr(const Image& img, Va global_va) noexcept {
    void* p = read<void*>(img, global_va);
    if (reinterpret_cast<std::uintptr_t>(p) == 0x0F0F0F0Fu) return nullptr; // debug-CRT uninit fill
    return p;
}

// --- The live vehicle list (D40, byte-verified retail v1.3) ----------------
// The game's own registry of every live vehicle: PVehicle::PVehicle (0x689020)
// self-registers `this + 0xAC` — the IVehicle interface subobject — into a
// growable array. Elements are therefore IVehicle* directly: an interface with
// a byte-anchored vtable (0x8AA828), NOT a PVehicle you may cast or offset
// into. Returned as void* because MWSDK does not declare foreign interfaces;
// call through verified slots only (see DriverClass / driver_class below).
[[nodiscard]] inline std::uint32_t vehicle_count(const Image& img) noexcept {
    return read<std::uint32_t>(img, db::data::PVehicle_mVehicleListCount);
}
[[nodiscard]] inline void* vehicle_at(const Image& img, std::uint32_t i) noexcept {
    if (i >= vehicle_count(img)) return nullptr;
    auto** data = read<void**>(img, db::data::PVehicle_mVehicleListData);
    if (data == nullptr) return nullptr;
    return data[i];
}

// Who is driving (D40): IVehicle vtable slot 22 (GetDriverClass, 0x6880B0)
// reads PVehicle+0x140. Human == 0 identifies the player's car.
enum class DriverClass : std::uint32_t {
    Human = 0, Traffic, Cop, Racer, None, NIS, Remote
};
inline constexpr std::size_t kIVehicleSlot_GetDriverClass          = 22; // +0x58
inline constexpr std::size_t kIVehicleSlot_SetBehaviorOverride     = 4;  // +0x10
inline constexpr std::size_t kIVehicleSlot_RemoveBehaviorOverride  = 5;  // +0x14
inline constexpr std::size_t kIVehicleSlot_CommitBehaviorOverrides = 6;  // +0x18

#if MWSDK_MW05_RUNTIME
// ===========================================================================
// Live process access (Windows only).
// ===========================================================================

// The main module (speed.exe). Cached after first resolve.
[[nodiscard]] inline const Image& process() noexcept {
    static const Image img{
        reinterpret_cast<std::uintptr_t>(::GetModuleHandleW(nullptr))};
    return img;
}

namespace detail {
// A memory copy that will not crash on a bad pointer: on MSVC it is guarded by
// SEH, elsewhere (e.g. MinGW) it is a best-effort plain copy.
inline bool guarded_copy(void* dst, const void* src, std::size_t n) noexcept {
#if defined(_MSC_VER)
    __try {
        std::memcpy(dst, src, n);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
#else
    std::memcpy(dst, src, n);
    return true;
#endif
}
} // namespace detail

// Unchecked typed read of the live process. See the OS-free read(img, va) above.
template <class T>
[[nodiscard]] T read(Va va) noexcept { return read<T>(process(), va); }

// Crash-safe typed read. Returns Error::AddressUnreadable on a fault.
template <class T>
[[nodiscard]] Result<T> safe_read(const Image& img, Va va) noexcept {
    static_assert(std::is_trivially_copyable_v<T>, "safe_read<T> needs trivially-copyable T");
    T v{};
    if (!detail::guarded_copy(&v, img.template ptr<const void>(va), sizeof(T)))
        return err(Error::AddressUnreadable);
    return v;
}
template <class T>
[[nodiscard]] Result<T> safe_read(Va va) noexcept { return safe_read<T>(process(), va); }

// Typed write into a writable page (e.g. a .data global). For code/read-only
// pages use Patch below, which handles page protection for you.
template <class T>
void write(const Image& img, Va va, const T& v) noexcept {
    static_assert(std::is_trivially_copyable_v<T>, "write<T> needs trivially-copyable T");
    std::memcpy(img.template ptr<void>(va), &v, sizeof(T));
}
template <class T>
void write(Va va, const T& v) noexcept { write<T>(process(), va, v); }

// Live-process singleton access (see the OS-free singleton_ptr(img, va) above).
[[nodiscard]] inline void* singleton_ptr(Va global_va) noexcept {
    return singleton_ptr(process(), global_va);
}
[[nodiscard]] inline void* road_network() noexcept { return singleton_ptr(singleton::WRoadNetwork); }
[[nodiscard]] inline void* camera_ai()    noexcept { return singleton_ptr(singleton::CameraAI); }
[[nodiscard]] inline void* g_manager()    noexcept { return singleton_ptr(singleton::GManager); }

// Live vehicle-list access on the running game (see the OS-free overloads above).
[[nodiscard]] inline std::uint32_t vehicle_count() noexcept { return vehicle_count(process()); }
[[nodiscard]] inline void* vehicle_at(std::uint32_t i) noexcept { return vehicle_at(process(), i); }

// Call IVehicle::GetDriverClass (vtable slot 22, verified 0x6880B0) on a list
// element. 32-bit MSVC only for the actual call (thiscall), like all typed
// calls in this header.
[[nodiscard]] inline DriverClass driver_class(void* ivehicle) noexcept {
    auto* vt = *reinterpret_cast<void* const*const*>(ivehicle);
    using F  = std::uint32_t(MWSDK_THISCALL*)(void*);
    return static_cast<DriverClass>(
        reinterpret_cast<F>(vt[kIVehicleSlot_GetDriverClass])(ivehicle));
}

// The player's car: the unique live vehicle whose driver class is Human (D40).
// Re-resolve on every use — the PVehicle is torn down and rebuilt between
// races, so never cache the pointer.
[[nodiscard]] inline void* player_vehicle() noexcept {
    const auto& img = process();
    const auto n = vehicle_count(img);
    for (std::uint32_t i = 0; i < n; ++i) {
        void* iv = vehicle_at(img, i);
        if (iv != nullptr && driver_class(iv) == DriverClass::Human) return iv;
    }
    return nullptr;
}

// Identify a live object by its vtable pointer. Reads *(Va*)obj, unbases it,
// and finds the matching reflected class. Returns nullptr if unknown.
[[nodiscard]] inline const db::ClassInfo* identify(const Image& img, const void* obj) noexcept {
    if (obj == nullptr) return nullptr;
    auto vt = read<std::uintptr_t>(img, img.unbase(reinterpret_cast<std::uintptr_t>(obj)));
    return class_by_vtable(img.unbase(vt));
}

// ---------------------------------------------------------------------------
// Self-contained byte patching (no hooking library required).
//
// Patch is an RAII toggle: constructing it saves the original bytes and writes
// your replacement (handling VirtualProtect + FlushInstructionCache); its
// destructor puts the original bytes back. Perfect for a trainer switch.
// ---------------------------------------------------------------------------
class Patch {
public:
    Patch() noexcept = default;

    // Overwrite `len` bytes at `va` with `bytes`, saving the originals.
    Patch(const Image& img, Va va, const std::uint8_t* bytes, std::size_t len) noexcept {
        apply(img, va, bytes, len);
    }
    Patch(const Patch&) = delete;
    Patch& operator=(const Patch&) = delete;
    Patch(Patch&& o) noexcept { move_from(o); }
    Patch& operator=(Patch&& o) noexcept {
        if (this != &o) { restore(); move_from(o); }
        return *this;
    }
    ~Patch() { restore(); }

    [[nodiscard]] bool active() const noexcept { return addr_ != nullptr; }

    // (Re)apply a patch. Returns false if the page could not be made writable.
    bool apply(const Image& img, Va va, const std::uint8_t* bytes, std::size_t len) noexcept {
        restore();
        if (len == 0 || len > sizeof(saved_)) return false;
        void* p = img.ptr<void>(va);
        if (!write_raw(p, bytes, len)) return false;
        addr_ = p;
        len_  = len;
        return true;
    }

    // Convenience: replace with NOPs (0x90).
    bool nop(const Image& img, Va va, std::size_t len) noexcept {
        std::uint8_t nops[kMax];
        if (len > kMax) return false;
        std::memset(nops, 0x90, len);
        return apply(img, va, nops, len);
    }

    // Put the original bytes back.
    void restore() noexcept {
        if (addr_) { write_raw(addr_, saved_, len_); addr_ = nullptr; len_ = 0; }
    }

private:
    static constexpr std::size_t kMax = 32;

    bool write_raw(void* p, const std::uint8_t* bytes, std::size_t len) noexcept {
        DWORD old = 0;
        if (!::VirtualProtect(p, len, PAGE_EXECUTE_READWRITE, &old)) return false;
        // On first apply, snapshot the originals before we clobber them.
        if (addr_ == nullptr) std::memcpy(saved_, p, len);
        std::memcpy(p, bytes, len);
        DWORD tmp = 0;
        ::VirtualProtect(p, len, old, &tmp);
        ::FlushInstructionCache(::GetCurrentProcess(), p, len);
        return true;
    }
    void move_from(Patch& o) noexcept {
        addr_ = o.addr_; len_ = o.len_;
        std::memcpy(saved_, o.saved_, sizeof(saved_));
        o.addr_ = nullptr; o.len_ = 0;
    }

    void*         addr_ = nullptr;
    std::size_t   len_  = 0;
    std::uint8_t  saved_[kMax] = {};
};

// One-shot helpers that patch permanently (no restore).
inline bool poke(const Image& img, Va va, const std::uint8_t* bytes, std::size_t len) noexcept {
    DWORD old = 0;
    void* p = img.ptr<void>(va);
    if (!::VirtualProtect(p, len, PAGE_EXECUTE_READWRITE, &old)) return false;
    std::memcpy(p, bytes, len);
    DWORD tmp = 0;
    ::VirtualProtect(p, len, old, &tmp);
    ::FlushInstructionCache(::GetCurrentProcess(), p, len);
    return true;
}

#endif // MWSDK_MW05_RUNTIME

} // namespace mwsdk::mw05

#endif // MWSDK_GAME_MW05_HPP
