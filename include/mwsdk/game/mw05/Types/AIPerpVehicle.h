// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/AIPerpVehicle.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_AIPERPVEHICLE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_AIPERPVEHICLE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/AIVehiclePid.h"
#include "mwsdk/game/mw05/Types/ICause.h"
#include "mwsdk/game/mw05/Types/ICheater.h"
#include "mwsdk/game/mw05/Types/IPerpetrator.h"

namespace mwsdk::mw05::rt {
  struct AIPerpVehicle : AIVehiclePid, IPerpetrator, ICause, ICheater {
    float                           mHeat;
    std::int32_t                    mCostToState;
    std::int32_t                    mPendingRepPointsNormal;
    std::int32_t                    mPendingRepPointsFromCopDestruction;
    bool                            mHiddenFromCars;
    bool                            mHiddenFromHelicopters;
    bool                            mWasInRaceEventLastHeatUpdate;
    float                           mHiddenZoneTimer;
    float                           mHiddenZoneLatchTime;
    bool                            mWasInZoneLastUpdate;
    std::int32_t                    mPursuitZoneCheck;
    Attrib::Gen::pursuitlevels*     mPursuitLevelAttrib;
    Attrib::Gen::pursuitsupport*    mPursuitSupportAttrib;
    Attrib::Gen::pursuitescalation* mPursuitEscalationAttrib;
    GRacerInfo*                     pRacerInfo;
    float                           fBaseSkill;
    float                           fGlueSkill;
    PidError*                       pGlueError;
    float                           fGlueTimer;
    float                           fGlueOutput;
    float                           LastTrafficHitTime;
    float                           m911CallTimer;

    virtual ~AIPerpVehicle();
    virtual void                            Update(float deltaTime) override;
    virtual void                            OnClearCausality(float startTime) override;
    virtual void                            OnBehaviorChange(const UCrc32& mechanic) override;
    virtual float                           GetSkill() override;
    virtual float                           GetShortcutSkill() override;
    virtual float                           GetPercentRaceComplete() override;
    virtual float                           GetHeat() override;
    virtual void                            SetHeat(float heat) override;
    virtual Attrib::Gen::pursuitescalation* GetPursuitEscalationAttrib() override;
    virtual Attrib::Gen::pursuitlevels*     GetPursuitLevelAttrib() override;
    virtual Attrib::Gen::pursuitsupport*    GetPursuitSupportAttrib() override;
    virtual void                            AddCostToState(std::int32_t cost) override;
    virtual std::int32_t                    GetCostToState() override;
    virtual void                            SetCostToState(std::int32_t costToState) override;
    virtual bool                            IsRacing() override;
    virtual bool                            IsBeingPursued() override;
    virtual bool                            IsHiddenFromCars() override;
    virtual bool                            IsHiddenFromHelicopters() override;
    virtual bool                            IsPartiallyHidden(float& howHidden) override;
    virtual void                            AddToPendingRepPointsFromCopDestruction(std::int32_t amount) override;
    virtual void                            AddToPendingRepPointsNormal(std::int32_t amount) override;
    virtual std::int32_t                    GetPendingRepPointsNormal() override;
    virtual std::int32_t                    GetPendingRepPointsFromCopDestruction() override;
    virtual void                            ClearPendingRepPoints() override;
    virtual void                            SetRacerInfo(GRacerInfo* info) override;
    virtual GRacerInfo*                     GetRacerInfo() override;
    virtual float                           GetLastTrafficHitTime() override;
    virtual void                            Set911CallTime(float time) override;
    virtual float                           Get911CallTime() override;
    virtual void  OnCausedCollision(const Sim::Collision::Info& cInfo, ISimable* from, ISimable* to) override;
    virtual void  OnCausedExplosion(IExplosion* explosion, ISimable* to) override;
    virtual float GetCatchupCheat() override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_AIPERPVEHICLE_H
