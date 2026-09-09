// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/GTrigger.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_GTRIGGER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_GTRIGGER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/eastl.hpp"

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/GRuntimeInstance.h"
#include "mwsdk/game/mw05/Types/WTrigger.h"

namespace mwsdk::mw05::rt {
  struct GTrigger : GRuntimeInstance {
    WTrigger                 mWorldTrigger;
    UMath::Vector3           mDirection;
    std::uint32_t            mTriggerEnabled;
    eastl::vector<ISimable*> mSimObjInside;
    CARP::EventList          mEventList;
    CARP::EventStaticData    mEventStaticData;
    std::uint8_t             mTriggerEventData[16];
    EmitterGroup*            mParticleEffect[2];
    GIcon*                   mIcon;
    bool                     mEnabled;
    std::int32_t             mActivationReferences;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_GTRIGGER_H
