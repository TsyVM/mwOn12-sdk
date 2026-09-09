// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05_attrib.hpp
// ==========================
// The live attribute interface: typed read/write of a running car's tuning
// attributes, grounded in ONE verified engine primitive.
//
//   0x00454810  __thiscall void* AttribCollection::GetField(
//                                   AttribCollection* self,
//                                   uint32_t          fieldKeyHash,   // lookup2/0xABCDEF00
//                                   int               index)
//
// Verified by its call sites: every vault getter funnels through it — e.g.
// AIVehicle::GetTopSpeedMultiplier (0x402D30) does
//   push 0xEC57E16B ("TopSpeedMultiplier"); call 0x454810
// and reads the float through the returned pointer. GetField returns a pointer
// straight into the collection's field storage, so reading through it reads the
// live value and writing through it IS the runtime-override mechanism. That is
// the whole live-tuning primitive. (Encyclopedia RE Discovery 35.)
//
// The key is the same lookup2/0xABCDEF00 attrib hash used everywhere else in the
// SDK, so the 724 verified field keys in mwsdk::vault::schema are exactly the
// keys you pass here — MWSDK's Attrib layer is grounded in more verified keys
// than any hand-authored table, and every one is traceable.
//
// Design notes:
//   * The field POINTER RESOLUTION is the only part that must call into the live
//     game (it invokes GetField). The typed load/store on a resolved pointer is
//     pure pointer arithmetic and is unit-tested off-target.
//   * The GetField resolver is injectable. Attrib::bind() fills it from the
//     rebased address database on a live speed.exe; Attrib::with_resolver()
//     takes any conforming function pointer, so a test (or a mock, or a future
//     game revision) can supply its own. No hidden global state.
//   * Nothing throws. Fallible calls return Result<T>; the reference-returning
//     helpers document their precondition (the field must exist).

#ifndef MWSDK_GAME_MW05_ATTRIB_HPP
#define MWSDK_GAME_MW05_ATTRIB_HPP

#include <cstdint>
#include <cstring>
#include <type_traits>

#include "mwsdk/core.hpp"              // Result<T>, Error, err()
#include "mwsdk/game/mw05.hpp"         // Image, fn::AttribCollection_GetField, MWSDK_THISCALL
#include "mwsdk/vault_schema.inl"      // vault::schema::* attrib keys (grounding for callers)

namespace mwsdk::mw05 {

// The verified ABI of AttribCollection::GetField. The MWSDK_THISCALL keyword is
// meaningful only on 32-bit MSVC (the real target); everywhere else it collapses
// to nothing, so this type still parses and a fake resolver is callable in tests.
using GetFieldFn = void*(MWSDK_THISCALL*)(void* self, std::uint32_t key, int index);

namespace detail {
// Typed load/store over a already-resolved field pointer. Pure, testable, and
// endian-correct for the x86 target (a straight memcpy).
template <class T>
[[nodiscard]] inline T load_field(const void* p) noexcept {
    static_assert(std::is_trivially_copyable_v<T>, "Attrib field<T> needs trivially-copyable T");
    T v{};
    std::memcpy(&v, p, sizeof(T));
    return v;
}
template <class T>
inline void store_field(void* p, const T& v) noexcept {
    static_assert(std::is_trivially_copyable_v<T>, "Attrib field<T> needs trivially-copyable T");
    std::memcpy(p, &v, sizeof(T));
}
} // namespace detail

// ===========================================================================
// Attrib — a typed view over a live AttribCollection.
//
// Hold one (bind it to a live collection pointer), then read/write fields by
// their verified attrib key:
//
//   mw05::Attrib car = mw05::Attrib::bind(game, collection);
//   float mul = car.get<float>(vault::schema::aivehicle::TopSpeedMultiplier)
//                  .value_or(1.0f);
//   car.set<float>(vault::schema::aivehicle::TopSpeedMultiplier, 1.25f);   // +25%
//
//   // In-place edit without two calls (precondition: the field exists):
//   if (float* p = car.ref<float>(vault::schema::aivehicle::AccelerationMultiplier))
//       *p *= 1.10f;
//
// Attrib is a cheap value type (two pointers); copy it freely. It owns nothing.
// ===========================================================================
class Attrib {
public:
    Attrib() noexcept = default;

    // Construct against an explicit resolver. This is the testable/portable path
    // and the seam a mock plugs into: `get_field` must obey the verified ABI.
    static Attrib with_resolver(void* collection, GetFieldFn get_field) noexcept {
        Attrib a;
        a.collection_ = collection;
        a.get_field_  = get_field;
        return a;
    }

    // True once a collection and resolver are set and usable.
    [[nodiscard]] bool valid() const noexcept {
        return collection_ != nullptr && get_field_ != nullptr;
    }
    [[nodiscard]] void* collection() const noexcept { return collection_; }

    // Raw pointer into the collection's field storage for `key` (nullptr if the
    // collection lacks that field, or if this Attrib is unbound). This is the
    // single live-call primitive; everything below is built on it.
    [[nodiscard]] void* field_ptr(std::uint32_t key, int index = 0) const noexcept {
        if (!valid()) return nullptr;
        return get_field_(collection_, key, index);
    }

    // Does the collection carry this field?
    [[nodiscard]] bool has(std::uint32_t key, int index = 0) const noexcept {
        return field_ptr(key, index) != nullptr;
    }

    // Typed read. FieldNotFound if the collection doesn't carry `key`.
    template <class T>
    [[nodiscard]] Result<T> get(std::uint32_t key, int index = 0) const noexcept {
        void* p = field_ptr(key, index);
        if (p == nullptr) return err(Error::FieldNotFound);
        return detail::load_field<T>(p);
    }

    // Typed read with a caller-supplied fallback — the ergonomic one-liner.
    template <class T>
    [[nodiscard]] T get_or(std::uint32_t key, T fallback, int index = 0) const noexcept {
        void* p = field_ptr(key, index);
        return p ? detail::load_field<T>(p) : fallback;
    }

    // Typed write. FieldNotFound (nothing written) if the collection lacks `key`.
    template <class T>
    Status set(std::uint32_t key, const T& value, int index = 0) noexcept {
        void* p = field_ptr(key, index);
        if (p == nullptr) return err(Error::FieldNotFound);
        detail::store_field<T>(p, value);
        return {};
    }

    // Typed pointer into storage for in-place edits (reads AND writes through it).
    // Returns nullptr if the field is absent; check before dereferencing.
    template <class T>
    [[nodiscard]] T* ref(std::uint32_t key, int index = 0) const noexcept {
        return static_cast<T*>(field_ptr(key, index));
    }

#if MWSDK_MW05_RUNTIME
    // Bind to a live collection on a running speed.exe: the resolver is the
    // rebased, verified AttribCollection::GetField. Windows-only because it
    // forms a live code pointer; the pure API above works everywhere.
    [[nodiscard]] static Attrib bind(const Image& img, void* collection) noexcept {
        auto fn = reinterpret_cast<GetFieldFn>(
            img.rebase(fn::AttribCollection_GetField));
        return with_resolver(collection, fn);
    }
    [[nodiscard]] static Attrib bind(void* collection) noexcept {
        return bind(process(), collection);
    }
#endif

private:
    void*      collection_ = nullptr;
    GetFieldFn get_field_  = nullptr;
};

} // namespace mwsdk::mw05

#endif // MWSDK_GAME_MW05_ATTRIB_HPP
