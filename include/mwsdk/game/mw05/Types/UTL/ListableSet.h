// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/UTL/ListableSet.h
// ---------------------------------------------------------------------------
// Reconstructed live-object layout for Need for Speed: Most Wanted (2005),
// speed.exe v1.3. Community-reverse-engineered [reference]-confidence data,
// re-expressed in MWSDK form under namespace mwsdk::mw05::rt.
//
// NOT part of MWSDK's verified core tables (mw05_db.inl / mw05_layouts.inl).
// Member offsets are implied by declaration order and depend on target ABI;
// treat addresses/offsets here as leads to confirm, not ground truth. See
// mwsdk/game/mw05/README.md for provenance and the verified/reference split.
// ---------------------------------------------------------------------------

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_UTL_LISTABLESET_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_UTL_LISTABLESET_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL/FixedVector.h"

namespace mwsdk::mw05::rt::UTL {
  template <typename T, std::size_t nT, typename E, std::size_t nE>
  struct ListableSet {
    struct List : FixedVector<T*, nT> {};
    struct _ListSet {
      List _buckets[nE];

      List&       operator[](std::size_t index) noexcept { return _buckets[index]; }
      const List& operator[](std::size_t index) const noexcept { return _buckets[index]; }
      List&       operator[](E index) noexcept { return _buckets[static_cast<std::size_t>(index)]; }
      const List& operator[](E index) const noexcept { return _buckets[static_cast<std::size_t>(index)]; }
    };
  };
}  // namespace mwsdk::mw05::rt::UTL

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_UTL_LISTABLESET_H
