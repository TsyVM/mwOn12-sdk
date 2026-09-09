// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/AIVehiclePursuit.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_AIVEHICLEPURSUIT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_AIVEHICLEPURSUIT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/AIVehiclePid.h"
#include "mwsdk/game/mw05/Types/IPursuitAI.h"
#include "mwsdk/game/mw05/Types/Timer.h"

namespace mwsdk::mw05::rt {
  struct AIVehiclePursuit : AIVehiclePid, IPursuitAI {
    bool           mInPursuit;
    bool           mBreaker;
    bool           mChicken;
    bool           mDamagedByPerp;
    SirenState     mSirenState;
    bool           mSirenInit;
    Timer          mT_siren[3];
    bool           mInFormation;
    bool           mInPosition;
    bool           mWithinEngagementRadius;
    UMath::Vector3 mPursuitOffset;
    UMath::Vector3 mInPositionOffset;
    UCrc32         mInPositionGoal;
    float          mTimeSinceTargetSeen;
    float          mVisibiltyTestTimer;
    UCrc32         mSupportGoal;

    virtual ~AIVehiclePursuit();
    virtual void UpdateSiren(float deltaTime);
    virtual void                  Update(float deltaTime) override;
    virtual void                  ResetInternals() override;
    virtual void                  StartPatrol() override;
    virtual void                  StartRoadBlock() override;
    virtual void                  StartFlee() override;
    virtual void                  SetInPursuit(bool inPursuit) override;
    virtual bool                  GetInPursuit() override;
    virtual void                  StartPursuit(AITarget* target, ISimable* itargetSimable) override;
    virtual void                  DoInPositionGoal() override;
    virtual void                  EndPursuit() override;
    virtual AITarget*             GetPursuitTarget() override;
    virtual bool                  StartSupportGoal() override;
    virtual AITarget*             PursuitRequest() override;
    virtual void                  SetInFormation(bool inFormation) override;
    virtual bool                  GetInFormation() override;
    virtual void                  SetInPosition(bool inPosition) override;
    virtual bool                  GetInPosition() override;
    virtual void                  SetPursuitOffset(const UMath::Vector3& offset) override;
    virtual const UMath::Vector3& GetPursuitOffset() override;
    virtual void                  SetInPositionGoal(const UCrc32& ipg) override;
    virtual const UCrc32&         GetInPositionGoal() override;
    virtual void                  SetInPositionOffset(const UMath::Vector3& offset) override;
    virtual const UMath::Vector3& GetInPositionOffset() override;
    virtual void                  SetBreaker(bool breaker) override;
    virtual bool                  GetBreaker() override;
    virtual void                  SetChicken(bool chicken) override;
    virtual bool                  GetChicken() override;
    virtual void                  SetDamagedByPerp(bool damaged) override;
    virtual bool                  GetDamagedByPerp() override;
    virtual SirenState            GetSirenState() override;
    virtual float                 GetTimeSinceTargetSeen() override;
    virtual void                  ZeroTimeSinceTargetSeen() override;
    virtual bool                  CanSeeTarget(AITarget* target) override;
    virtual const UCrc32&         GetSupportGoal() override;
    virtual void                  SetSupportGoal(UCrc32 sg) override;
    virtual void                  SetWithinEngagementRadius() override;
    virtual bool                  WasWithinEngagementRadius() override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_AIVEHICLEPURSUIT_H
