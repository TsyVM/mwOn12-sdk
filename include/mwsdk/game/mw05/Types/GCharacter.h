// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/GCharacter.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_GCHARACTER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_GCHARACTER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/GRuntimeInstance.h"
#include "mwsdk/game/mw05/Types/IAttachable.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct GCharacter : GRuntimeInstance, UTL::COM::Object, IAttachable {
    enum class Flags : std::uint8_t { UsingStockCar, AttachedToManager };
    enum class State : std::uint8_t {
      Invalid,
      Unspawned,
      SpawningWaitingForModel,
      SpawningWaitingForTrack,
      Spawned,
      UnspawningWaitingUntilOffscreen
    };

    UMath::Vector3 mSpawnPos;
    State          mState;
    Flags          mFlags;
    std::uint16_t  mCreateAttemptsMade;
    UMath::Vector3 mSpawnDir;
    float          mSpawnSpeed;
    UMath::Vector3 mTargetPos;
    IVehicle*      mVehicle;
    UMath::Vector3 mTargetDir;
    Attachments*   mAttachments;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_GCHARACTER_H
