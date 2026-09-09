// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/ICopMgr.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ICOPMGR_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ICOPMGR_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct ICopMgr : UTL::COM::IUnknown {
    virtual ~ICopMgr();

    virtual bool  VehicleSpawningEnabled(bool isDespawn)   = 0;
    virtual void  ResetCopsForRestart(bool release)        = 0;
    virtual void  SetAllBustedTimersToZero()               = 0;
    virtual void  PursuitIsEvaded(IPursuit* ipursuit)      = 0;
    virtual bool  IsCopRequestPending()                    = 0;
    virtual bool  IsCopSpawnPending()                      = 0;
    virtual void  SpawnCop(UMath::Vector3& initialPos, UMath::Vector3& initialVec, const char* vehicleName,
                           bool inPursuit, bool roadBlock) = 0;
    virtual bool  PlayerPursuitHasCop()                    = 0;
    virtual bool  CanPursueRacers()                        = 0;
    virtual bool  IsPlayerPursuitActive()                  = 0;
    virtual void  LockoutCops(bool lockout)                = 0;
    virtual void  NoNewPursuitsOrCops()                    = 0;
    virtual void  PursueAtHeatLevel(int minHeatLevel)      = 0;
    virtual float GetLockoutTimeRemaining()                = 0;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ICOPMGR_H
