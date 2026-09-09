// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IPursuitAI.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IPURSUITAI_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IPURSUITAI_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IPursuitAI : UTL::COM::IUnknown {
    virtual ~IPursuitAI();

    virtual void                  StartPatrol()                                            = 0;
    virtual void                  StartRoadBlock()                                         = 0;
    virtual void                  StartFlee()                                              = 0;
    virtual void                  SetInPursuit(bool inPursuit)                             = 0;
    virtual bool                  GetInPursuit()                                           = 0;
    virtual void                  StartPursuit(AITarget* target, ISimable* itargetSimable) = 0;
    virtual void                  DoInPositionGoal()                                       = 0;
    virtual void                  EndPursuit()                                             = 0;
    virtual AITarget*             GetPursuitTarget()                                       = 0;
    virtual bool                  StartSupportGoal()                                       = 0;
    virtual AITarget*             PursuitRequest()                                         = 0;
    virtual void                  SetInFormation(bool inFormation)                         = 0;
    virtual bool                  GetInFormation()                                         = 0;
    virtual void                  SetInPosition(bool inPosition)                           = 0;
    virtual bool                  GetInPosition()                                          = 0;
    virtual void                  SetPursuitOffset(const UMath::Vector3& offset)           = 0;
    virtual const UMath::Vector3& GetPursuitOffset()                                       = 0;
    virtual void                  SetInPositionGoal(const UCrc32& ipg)                     = 0;
    virtual const UCrc32&         GetInPositionGoal()                                      = 0;
    virtual void                  SetInPositionOffset(const UMath::Vector3& offset)        = 0;
    virtual const UMath::Vector3& GetInPositionOffset()                                    = 0;
    virtual void                  SetBreaker(bool breaker)                                 = 0;
    virtual bool                  GetBreaker()                                             = 0;
    virtual void                  SetChicken(bool chicken)                                 = 0;
    virtual bool                  GetChicken()                                             = 0;
    virtual void                  SetDamagedByPerp(bool damaged)                           = 0;
    virtual bool                  GetDamagedByPerp()                                       = 0;
    virtual SirenState            GetSirenState()                                          = 0;
    virtual float                 GetTimeSinceTargetSeen()                                 = 0;
    virtual void                  ZeroTimeSinceTargetSeen()                                = 0;
    virtual bool                  CanSeeTarget(AITarget* target)                           = 0;
    virtual const UCrc32&         GetSupportGoal()                                         = 0;
    virtual void                  SetSupportGoal(UCrc32 sg)                                = 0;
    virtual void                  SetWithinEngagementRadius()                              = 0;
    virtual bool                  WasWithinEngagementRadius()                              = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x4038E0)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IPURSUITAI_H
