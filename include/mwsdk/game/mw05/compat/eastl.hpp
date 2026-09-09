// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/compat/eastl.hpp
// ---------------------------------------------------------------------------
// Minimal MWSDK-authored stand-ins for the handful of EA-STL containers the
// reconstructed mw05::rt types name in their layouts (list, vector, map, slist).
// They alias to the C++ standard-library equivalents so the reference headers
// are self-contained and compile with no external dependency.
//
// NOTE: standard containers do not guarantee the exact byte size/layout of the
// game's original EA-STL containers, so any struct that embeds one BY VALUE is
// size-approximate. Pointer/reference members are unaffected. This layer is a
// knowledge/reference aid; use the verified mw05_layouts.inl offsets when exact
// ABI matters.
// ---------------------------------------------------------------------------
#ifndef MWSDK_MW05_RT_COMPAT_EASTL_HPP
#define MWSDK_MW05_RT_COMPAT_EASTL_HPP

#include <list>
#include <vector>
#include <map>
#include <forward_list>

namespace eastl {
template <class... Ts> using list       = std::list<Ts...>;
template <class... Ts> using vector     = std::vector<Ts...>;
template <class... Ts> using map        = std::map<Ts...>;
template <class... Ts> using slist      = std::forward_list<Ts...>;
}  // namespace eastl

#endif  // MWSDK_MW05_RT_COMPAT_EASTL_HPP
