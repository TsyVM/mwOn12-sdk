// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/ActionQueue.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ACTIONQUEUE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ACTIONQUEUE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/ActionData.h"
#include "mwsdk/game/mw05/Types/Timer.h"
#include "mwsdk/game/mw05/Types/UCircularQueue.h"

namespace mwsdk::mw05::rt {
  struct ActionQueue {
    enum class eState : std::uint32_t { Disabled, Enabled };

    UCircularQueue<ActionData, 50> fQueue;
    std::int32_t                   mPort;
    eState                         mState;
    InputMapping*                  mMappings;
    std::uint32_t                  mConfig;
    const char*                    mQueueName;
    std::int32_t                   mUniqueID;
    bool                           mConnected;
    bool                           mRequired;
    Timer                          mActionTime;
    Timer                          mActivationTime;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ACTIONQUEUE_H
