// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05_easy.hpp
// ========================
// The Level-1 API: zero-setup one-liners for the most common live-mod tasks.
//
// The explicit API (mw05::Image, Attrib::bind(game, ...), field<T>(obj, off),
// Patch(game, va, ...)) is always there and gives you full control. This header
// is the "I just want to do the thing" layer on top of it: every function here
// defaults to the live process image, so you never hold an Image yourself.
//
//   #include <mwsdk/mod.hpp>            // pulls this in
//   using namespace mwsdk;
//
//   mw05::vehicle(car).top_speed() *= 1.10f;                 // ergonomic view
//   mw05::attrib(coll).set<float>(k, 1.25f);                 // live attribute
//   auto guard = mw05::nop(mw05::fn::SomeCheck, 6);          // RAII byte patch
//
// Everything that only does pointer arithmetic (the views) works on any
// platform; the parts that touch the live process (attrib, patch, read/write)
// are Windows-only and compiled out elsewhere, exactly like the rest of mw05.

#ifndef MWSDK_GAME_MW05_EASY_HPP
#define MWSDK_GAME_MW05_EASY_HPP

#include <cstdint>
#include <initializer_list>

#include "mwsdk/game/mw05.hpp"
#include "mwsdk/game/mw05_attrib.hpp"
#include "mwsdk/game/mw05_views.hpp"

namespace mwsdk::mw05 {

// ---------------------------------------------------------------------------
// Ergonomic views — pure pointer arithmetic, so these are available everywhere.
// Each returns a value-type view; construct one per use, they own nothing.
//
//   mw05::vehicle(car).drive_speed() = 40.0f;
// ---------------------------------------------------------------------------
[[nodiscard]] inline VehicleView       vehicle(void* obj)        noexcept { return VehicleView{obj}; }
[[nodiscard]] inline TrafficActionView traffic_action(void* obj) noexcept { return TrafficActionView{obj}; }
[[nodiscard]] inline CopManagerView    cop_manager(void* obj)    noexcept { return CopManagerView{obj}; }
[[nodiscard]] inline PursuitView       pursuit(void* obj)        noexcept { return PursuitView{obj}; }
[[nodiscard]] inline PerpView          perp(void* obj)           noexcept { return PerpView{obj}; }
[[nodiscard]] inline PlayerView        player(void* obj)         noexcept { return PlayerView{obj}; }
[[nodiscard]] inline RacerBrainView    racer_brain(void* obj)    noexcept { return RacerBrainView{obj}; }

#if MWSDK_MW05_RUNTIME
// ---------------------------------------------------------------------------
// Live attribute collection, bound to the running process in one call.
//
//   float mul = mw05::attrib(coll).get_or<float>(key, 1.0f);
// ---------------------------------------------------------------------------
[[nodiscard]] inline Attrib attrib(void* collection) noexcept {
    return Attrib::bind(process(), collection);
}

// ---------------------------------------------------------------------------
// Object identity against the live process (process-defaulted overload).
// ---------------------------------------------------------------------------
[[nodiscard]] inline const db::ClassInfo* identify(const void* obj) noexcept {
    return identify(process(), obj);
}

// ---------------------------------------------------------------------------
// One-liner RAII byte patches on the live image. The returned Patch restores
// the original bytes when it goes out of scope — hold it to keep the patch
// live, drop it to revert (exactly like a VanHooks Hook).
//
//   auto guard = mw05::patch(va, {0xB0, 0x01});   // mov al, 1
//   auto guard = mw05::nop(va, 6);                 // 6x 0x90, auto-reverts
// ---------------------------------------------------------------------------
[[nodiscard]] inline Patch patch(Va va, std::initializer_list<std::uint8_t> bytes) noexcept {
    return Patch{process(), va, bytes.begin(), bytes.size()};
}
[[nodiscard]] inline Patch nop(Va va, std::size_t len) noexcept {
    Patch p;
    p.nop(process(), va, len);
    return p;
}

// Permanent (non-reverting) one-shot poke, process-defaulted.
inline bool poke(Va va, std::initializer_list<std::uint8_t> bytes) noexcept {
    return poke(process(), va, bytes.begin(), bytes.size());
}
#endif // MWSDK_MW05_RUNTIME

} // namespace mwsdk::mw05

#endif // MWSDK_GAME_MW05_EASY_HPP
