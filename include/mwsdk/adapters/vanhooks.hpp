// SPDX-License-Identifier: MIT
//
// mwsdk/adapters/vanhooks.hpp
// ===========================
// OPTIONAL glue between MWSDK's verified address database and VanHooks
// (https://github.com/TsyVM/VanHooks) - the modern C++23 hooking library this
// SDK's ergonomics are modelled on.
//
// This adapter is strictly opt-in. The core SDK never pulls in a hooking
// backend (see mwsdk.hpp). Including this header does nothing unless VanHooks
// is actually on your include path: it is fenced behind __has_include(<vh/vh.hpp>).
// If you only need byte patches, mwsdk/game/mw05.hpp already gives you a
// self-contained RAII patcher and you don't need this at all.
//
// What you get: hook a *verified game address* (or a whole vtable slot) in one
// line, with the Va rebased for you and the symbol name used as the hook tag.
//
//   // Detour SetWorldHeat and call through to the original.
//   static mw05::LuaCFunction orig = nullptr;
//   int my_SetWorldHeat(void* L) { /* ... */ return orig(L); }
//
//   auto h = mw05::hook::at(mw05::lua::Game::SetWorldHeat, &my_SetWorldHeat, &orig);
//   if (!h) log("hook failed: %s", vh::error_to_string(h.error()).data());
//   // h is RAII: the hook lifts when it goes out of scope.

#ifndef MWSDK_ADAPTERS_VANHOOKS_HPP
#define MWSDK_ADAPTERS_VANHOOKS_HPP

#include "mwsdk/game/mw05.hpp"

#if defined(__has_include)
#  if __has_include(<vh/vh.hpp>)
#    define MWSDK_HAS_VANHOOKS 1
#  endif
#endif

#ifdef MWSDK_HAS_VANHOOKS
#include <vh/vh.hpp>

#if !MWSDK_MW05_RUNTIME
#  error "mwsdk/adapters/vanhooks.hpp requires the live runtime (Windows target)."
#endif

namespace mwsdk::mw05::hook {

// The live, rebased target for a verified Va.
[[nodiscard]] inline void* target(Va va) noexcept {
    return mw05::process().ptr<void>(va);
}

// Inline-hook a verified game function by Va. `orig` receives a trampoline that
// calls through to the untouched original (pass nullptr if you don't need it).
template <class Detour, class Orig>
[[nodiscard]] vh::Result<vh::Hook>
at(Va va, Detour* detour, Orig* orig, const char* tag = "mw05") {
    return vh::inline_hook(target(va),
                           reinterpret_cast<void*>(detour),
                           reinterpret_cast<void**>(orig),
                           vh::config::Trampoline{ .tag = tag });
}

// Inline-hook a verified function by symbol name (looked up in the DB). The
// symbol's own name becomes the hook tag. A missing name yields an invalid
// target, which VanHooks reports as an error - it never silently no-ops.
template <class Detour, class Orig>
[[nodiscard]] vh::Result<vh::Hook>
named(std::string_view symbol, Detour* detour, Orig* orig) {
    const db::Symbol* s = mw05::find_symbol(symbol);
    return at(s ? s->va : Va{0}, detour, orig, s ? s->name.data() : "mw05:unknown");
}

// Hook a single slot of a class vtable (identified by its verified vtable Va).
[[nodiscard]] inline vh::Result<vh::Hook>
vtable_slot(Va vtable_va, std::size_t slot, void* detour, void** orig,
            const char* tag = "mw05:vtable") {
    void** vt = mw05::process().ptr<void*>(vtable_va);
    return vh::vtable_hook(vt, slot, detour, orig, vh::config::VTable{ .tag = tag });
}

} // namespace mwsdk::mw05::hook

#endif // MWSDK_HAS_VANHOOKS
#endif // MWSDK_ADAPTERS_VANHOOKS_HPP
