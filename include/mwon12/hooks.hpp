// Function hooking and memory patching for MWOn12 plugins.
//
// The render hook in mwon12.h reaches what the game *draws*. This reaches what
// the game *does*: hook a function inside speed.exe, patch its code or data,
// find an address by byte pattern. Gameplay, physics, AI, handling, HUD logic
// — none of it goes through D3D9, so none of it is reachable any other way.
//
//     #include <mwon12/hooks.hpp>
//
//     // A function TYPE, not a pointer to one -- Install<Fn> takes Fn* as the
//     // detour and Fn** as the original, so a pointer typedef here does not
//     // compile. See the note above Install().
//     using DamageFn = void __fastcall(void* self, void* /*edx*/, float amount);
//     static DamageFn* g_orig = nullptr;
//     static void __fastcall MyDamage(void* self, void* edx, float amount) {
//         g_orig(self, edx, amount * 0.5f);     // half damage
//     }
//
//     mwon12::hooks::Hook h;                    // a member, not a local
//     h.Install<DamageFn>(mwon12::hooks::Rva(0x0067A410), &MyDamage, &g_orig);
//
// Hooking is a thin layer over VanHooks, which does the real work: it decodes
// the target's prologue with a full disassembler, relocates the stolen bytes
// into a trampoline, and suspends other threads while it patches. This layer
// uses only VanHooks' public API (<vh/vh.hpp>); what it adds is MWOn12's
// logging, RAII lifetime, and an address type that says out loud which module a
// number is relative to.
//
// Memory patching and pattern scanning below are self-contained (Win32
// VirtualProtect + an in-image byte scan) and pull in nothing from VanHooks —
// they never needed a hooking engine to begin with.
//
//
// ADDRESSES AND ASLR
//
// Addresses from a disassembler are static: they assume the module loaded at
// its preferred base. speed.exe is a 2005 binary with no ASLR, so its preferred
// base (0x400000) is almost always where it lands, and a raw address usually
// works. Almost always is not always — a loader, a mod manager or a rebased
// build moves it.
//
// Rva() applies the real load address; Abs() takes a number as final. Prefer
// Rva() for anything you read out of a disassembler:
//
//     h.Install(mwon12::hooks::Rva(0x0027A410), &MyDetour, &g_orig);
//
// Better still, find it by pattern and don't depend on an address at all.
//
//
// THREADING AND LIFETIME
//
// Installing and removing are thread-safe by default: VanHooks suspends other
// threads across the patch. Your detour, though, runs on whichever thread the
// game called from, which for most game code is not the render thread — so a
// detour that touches your OnPresent state needs its own synchronisation.
//
// A Hook removes itself when destroyed. Keep it alive as long as the detour
// must stay installed, which for a plugin means a member, not a local.

#ifndef MWON12_HOOKS_HPP
#define MWON12_HOOKS_HPP

#include "mwon12.h"
#include "plugin.hpp"

#include <vh/vh.hpp>

#include <windows.h>

#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace mwon12 {
namespace hooks {

// ── Addresses ────────────────────────────────────────────────────────────────

// Base address speed.exe actually loaded at. Resolved once.
[[nodiscard]] inline uintptr_t ModuleBase(const char* module = nullptr) noexcept
{
    return reinterpret_cast<uintptr_t>(::GetModuleHandleA(module));
}

// The default base a 32-bit PE is linked for. Addresses copied out of a
// disassembler are almost always relative to this.
inline constexpr uintptr_t kPreferredBase = 0x00400000;

// Turn a static address from a disassembler into a live one.
//
// Accepts either form and does the right thing: a value at or above the
// preferred base is treated as a full static address and rebased; a smaller
// value is treated as an offset from the module base. That means both of these
// name the same byte, which is what people actually type:
//
//     Rva(0x0067A410)   // as the disassembler showed it
//     Rva(0x0027A410)   // as an offset
[[nodiscard]] inline uintptr_t Rva(uintptr_t address,
                                   const char* module = nullptr) noexcept
{
    const uintptr_t base = ModuleBase(module);
    if (base == 0) return address;
    return (address >= kPreferredBase)
         ? base + (address - kPreferredBase)
         : base + address;
}

// Take the number as final. Use when the address is already absolute for this
// run — one you got back from Scan(), for instance.
[[nodiscard]] inline uintptr_t Abs(uintptr_t address) noexcept { return address; }

// ── Hook ─────────────────────────────────────────────────────────────────────

// One installed detour. Moveable, not copyable; removes itself on destruction.
//
// Wraps a vh::Hook (VanHooks' public RAII handle). The vh::Hook is what
// actually lifts the detour when this object is destroyed or reassigned.
class Hook {
public:
    Hook() = default;

    Hook(Hook&&) noexcept            = default;
    Hook& operator=(Hook&&) noexcept = default;

    Hook(const Hook&)            = delete;
    Hook& operator=(const Hook&) = delete;

    ~Hook() = default;

    // Installs a detour over `target`.
    //
    // `originalOut` receives a pointer that calls the original function. Call
    // through it from your detour to keep the game's own behaviour; ignore it
    // to replace the function outright. It is written before the patch goes
    // live, so it is safe to use from the first call.
    //
    // Returns false and logs why on failure. Common causes: the address is
    // wrong, the prologue is too short to patch, or something else already
    // hooked it.
    // `Fn` is the function TYPE, never a pointer to one:
    //
    //     using DamageFn = void __fastcall(void*, void*, float);      // YES
    //     using DamageFn = void(__fastcall*)(void*, void*, float);    // NO
    //
    // The second makes `Fn*` a pointer-to-pointer and the call does not
    // compile. Declare the original as `DamageFn* g_orig`.
    template <typename Fn>
    bool Install(uintptr_t target, Fn* detour, Fn** originalOut = nullptr,
                 const char* name = nullptr) noexcept
    {
        Remove();

        auto r = ::vh::inline_hook<Fn>(
            reinterpret_cast<Fn*>(target), detour, originalOut,
            ::vh::config::Trampoline{ /*thread_safe*/ true,
                                      name ? std::string(name) : std::string{} });
        if (!r) {
            const auto msg = ::vh::error_to_string(r.error());
            LogError("hook: %s at 0x%08X failed - %.*s",
                     name ? name : "(unnamed)", unsigned(target),
                     int(msg.size()), msg.data());
            return false;
        }

        m_hook.emplace(std::move(*r));
        LogInfo("hook: %s installed at 0x%08X",
                name ? name : "(unnamed)", unsigned(target));
        return true;
    }

    // Hooks an exported function by module and name, so no address is needed.
    // The natural way to reach anything in a system DLL.
    template <typename Fn>
    bool InstallApi(std::string_view module, std::string_view symbol,
                    Fn* detour, Fn** originalOut = nullptr) noexcept
    {
        Remove();

        auto r = ::vh::api_hook<Fn>(module, symbol, detour, originalOut,
                                    ::vh::config::Trampoline{});
        if (!r) {
            const auto msg = ::vh::error_to_string(r.error());
            LogError("hook: %.*s!%.*s failed - %.*s",
                     int(module.size()), module.data(),
                     int(symbol.size()), symbol.data(),
                     int(msg.size()), msg.data());
            return false;
        }

        m_hook.emplace(std::move(*r));
        LogInfo("hook: %.*s!%.*s installed",
                int(module.size()), module.data(),
                int(symbol.size()), symbol.data());
        return true;
    }

    // Removes the detour. Called by the destructor; safe to call twice.
    void Remove() noexcept { m_hook.reset(); }

    [[nodiscard]] bool Installed() const noexcept
    {
        return m_hook.has_value() && m_hook->valid();
    }

private:
    std::optional<::vh::Hook> m_hook;
};

// ── Memory ───────────────────────────────────────────────────────────────────

namespace detail {

// Overwrites `size` bytes at `address` from `src`, making the page writable for
// the duration and flushing the instruction cache after. Returns false if the
// page could not be made writable.
inline bool WriteProtected(uintptr_t address, const void* src, size_t size) noexcept
{
    if (size == 0) return true;
    DWORD oldProtect = 0;
    if (!::VirtualProtect(reinterpret_cast<LPVOID>(address), size,
                          PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return false;
    }
    std::memcpy(reinterpret_cast<void*>(address), src, size);
    DWORD tmp = 0;
    ::VirtualProtect(reinterpret_cast<LPVOID>(address), size, oldProtect, &tmp);
    ::FlushInstructionCache(::GetCurrentProcess(),
                            reinterpret_cast<LPCVOID>(address), size);
    return true;
}

}  // namespace detail

// Writes bytes over `address`, making the page writable for the duration.
//
//     Patch(Rva(0x0051A340), { 0x90, 0x90 });   // nop out two bytes
//
// Patching code the game is currently executing on another thread is a race
// this cannot protect you from. Patch during OnDeviceCreated, or hook the
// function instead.
inline bool Patch(uintptr_t address, std::initializer_list<uint8_t> bytes,
                  const char* what = nullptr) noexcept
{
    if (!detail::WriteProtected(address, bytes.begin(), bytes.size())) {
        LogError("patch: %s at 0x%08X failed - page not writable",
                 what ? what : "(unnamed)", unsigned(address));
        return false;
    }
    LogInfo("patch: %s at 0x%08X (%u bytes)",
            what ? what : "(unnamed)", unsigned(address), unsigned(bytes.size()));
    return true;
}

// Fills a range with 0x90. The usual way to disable an instruction.
//
// `count` must cover whole instructions -- a partial nop leaves the tail of
// one decoding as garbage. Check the length in a disassembler first.
inline bool Nop(uintptr_t address, size_t count, const char* what = nullptr) noexcept
{
    const std::vector<uint8_t> nops(count, 0x90);
    if (!detail::WriteProtected(address, nops.data(), nops.size())) {
        LogError("nop: %s at 0x%08X failed - page not writable",
                 what ? what : "(unnamed)", unsigned(address));
        return false;
    }
    LogInfo("nop: %s at 0x%08X (%u bytes)",
            what ? what : "(unnamed)", unsigned(address), unsigned(count));
    return true;
}

// Reads `size` bytes. Empty on failure.
//
// Goes through ReadProcessMemory on this process rather than memcpy, which is
// the difference between the documented behaviour and a crash. memcpy from an
// address that is not mapped -- a pattern that matched nothing, an offset into
// the wrong module, a pointer read one frame after the game freed it -- raises
// an access violation inside a function marked noexcept, and the caller never
// gets the empty vector this promises. ReadProcessMemory validates the range
// in the kernel and returns false instead.
[[nodiscard]] inline std::vector<uint8_t> Read(uintptr_t address, size_t size) noexcept
{
    std::vector<uint8_t> out;
    if (size == 0 || address == 0) return out;

    out.resize(size);
    SIZE_T got = 0;
    if (!::ReadProcessMemory(::GetCurrentProcess(),
                             reinterpret_cast<LPCVOID>(address),
                             out.data(), size, &got) || got != size) {
        LogWarn("read: %u bytes at 0x%08X could not be read",
                unsigned(size), unsigned(address));
        out.clear();
    }
    return out;
}

// Writes a value of any trivially-copyable type over `address`.
//
//     Write<float>(Rva(0x008A1234), 2.5f);
template <typename T>
inline bool Write(uintptr_t address, const T& value,
                  const char* what = nullptr) noexcept
{
    static_assert(std::is_trivially_copyable_v<T>,
                  "Write only handles trivially copyable types");
    if (!detail::WriteProtected(address, &value, sizeof(T))) {
        LogError("write: %s at 0x%08X failed - page not writable",
                 what ? what : "(unnamed)", unsigned(address));
        return false;
    }
    return true;
}

// ── Scanning ─────────────────────────────────────────────────────────────────

namespace detail {

// A compiled IDA-style pattern: `bytes[i]` matters only where `mask[i]` is set.
struct Pattern {
    std::vector<uint8_t> bytes;
    std::vector<bool>    mask;   // true = must match, false = wildcard
    bool                 ok = false;
};

// Parses "8B 4C 24 ?? 85 C9" — space-separated hex bytes, `?` or `??` wildcard.
inline Pattern ParsePattern(std::string_view pat) noexcept
{
    auto hex = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        c = static_cast<char>(c | 0x20);
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return -1;
    };

    Pattern p;
    size_t i = 0;
    const size_t n = pat.size();
    while (i < n) {
        while (i < n && (pat[i] == ' ' || pat[i] == '\t')) ++i;
        if (i >= n) break;
        size_t j = i;
        while (j < n && pat[j] != ' ' && pat[j] != '\t') ++j;
        const std::string_view tok = pat.substr(i, j - i);
        i = j;

        if (tok == "?" || tok == "??") {
            p.bytes.push_back(0);
            p.mask.push_back(false);
            continue;
        }
        if (tok.size() != 2) { p.ok = false; return p; }
        const int hi = hex(tok[0]);
        const int lo = hex(tok[1]);
        if (hi < 0 || lo < 0) { p.ok = false; return p; }
        p.bytes.push_back(static_cast<uint8_t>((hi << 4) | lo));
        p.mask.push_back(true);
    }
    p.ok = !p.bytes.empty();
    return p;
}

}  // namespace detail

// Finds a byte pattern in a module. IDA syntax: hex bytes, `??` for a wildcard.
//
//     auto hits = ScanAll("8B 4C 24 ?? 85 C9 74 ??");
//
// Patterns survive a rebase, a recompile and a different game version in a way
// hard-coded addresses do not, so prefer this for anything you intend to share.
// Choose a pattern long enough to be unique -- check the hit count.
//
// `module` is a loaded module name ("speed.exe") or nullptr for the executable.
// Scans every non-discardable, readable section of the module's mapped image.
[[nodiscard]] inline std::vector<uintptr_t>
ScanAll(std::string_view pattern, const char* module = nullptr) noexcept
{
    std::vector<uintptr_t> hits;

    const detail::Pattern pp = detail::ParsePattern(pattern);
    if (!pp.ok) {
        LogWarn("scan: '%.*s' is not a valid pattern",
                int(pattern.size()), pattern.data());
        return hits;
    }

    const auto* base =
        reinterpret_cast<const uint8_t*>(::GetModuleHandleA(module));
    if (!base) {
        LogWarn("scan: module '%s' not loaded", module ? module : "(exe)");
        return hits;
    }

    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return hits;
    const auto* nt =
        reinterpret_cast<const IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return hits;

    const size_t plen      = pp.bytes.size();
    const size_t imageSize = nt->OptionalHeader.SizeOfImage;

    const IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt);
    for (unsigned s = 0; s < nt->FileHeader.NumberOfSections; ++s) {
        const IMAGE_SECTION_HEADER& sh = sec[s];
        if (sh.Characteristics & IMAGE_SCN_MEM_DISCARDABLE) continue;
        if (!(sh.Characteristics & IMAGE_SCN_MEM_READ))     continue;

        size_t vsize = sh.Misc.VirtualSize ? sh.Misc.VirtualSize
                                           : sh.SizeOfRawData;

        // Clamped to the mapped image. A section header is data from a file,
        // and a VirtualAddress or VirtualSize reaching past SizeOfImage -- a
        // packed or hand-edited executable, or simply a malformed one -- walks
        // the loop off the end of the mapping and faults. Nothing here is
        // worth crashing the game for.
        if (sh.VirtualAddress >= imageSize) continue;
        const size_t avail = imageSize - sh.VirtualAddress;
        if (vsize > avail) vsize = avail;

        if (vsize < plen) continue;

        const uint8_t* start = base + sh.VirtualAddress;
        const uint8_t* last  = start + (vsize - plen);
        for (const uint8_t* pc = start; pc <= last; ++pc) {
            size_t k = 0;
            for (; k < plen; ++k) {
                if (pp.mask[k] && pc[k] != pp.bytes[k]) break;
            }
            if (k == plen) hits.push_back(reinterpret_cast<uintptr_t>(pc));
        }
    }
    return hits;
}

// The single match for `pattern`, or nothing.
//
// Empty when there is no match AND when there is more than one: an ambiguous
// pattern that silently returned its first hit is how a mod ends up patching
// the wrong function on someone else's machine. Widen the pattern instead.
[[nodiscard]] inline std::optional<uintptr_t>
Scan(std::string_view pattern, const char* module = nullptr) noexcept
{
    const auto hits = ScanAll(pattern, module);
    if (hits.empty()) {
        LogWarn("scan: '%.*s' matched nothing",
                int(pattern.size()), pattern.data());
        return std::nullopt;
    }
    if (hits.size() > 1) {
        LogWarn("scan: '%.*s' matched %u places - too ambiguous to use, "
                "make the pattern longer",
                int(pattern.size()), pattern.data(), unsigned(hits.size()));
        return std::nullopt;
    }
    return hits.front();
}

}  // namespace hooks
}  // namespace mwon12

#endif  // MWON12_HOOKS_HPP
