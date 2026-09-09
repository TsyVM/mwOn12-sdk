// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/CARP.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_CARP_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_CARP_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::CARP {
  struct EventList {
    std::uint32_t fNumEvents;
    std::uint32_t fPad[3];
  };

  struct EventStaticData {
    std::uint32_t fEventID;
    std::uint32_t fEventSize;
    std::uint32_t fDataOffset;
    std::uint32_t fPad;
  };

  struct Trigger {
    UMath::Vector4 fMatRow0Width;
    TriggerTypes   fType : 4;
    std::uint32_t  fShape : 4;
    TriggerFlags   fFlags : 24;
    float          fHeight;
    EventList*     fEvents;
    std::uint16_t  fIterStamp;
    std::uint16_t  fFingerprint;
    UMath::Vector4 fMatRow2Length;
    UMath::Vector4 fPosRadius;
  };
}  // namespace mwsdk::mw05::rt::CARP

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_CARP_H
