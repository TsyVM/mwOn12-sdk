// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Private.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_PRIVATE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_PRIVATE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::Attrib {
  struct Private {
    std::uint16_t mCapacity;
    std::uint16_t mCount;
    std::uint16_t mElemSize;
    std::uint16_t mData;
  };
}  // namespace mwsdk::mw05::rt::Attrib

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_PRIVATE_H
