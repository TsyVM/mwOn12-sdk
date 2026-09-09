// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/AIVehicleHuman.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_AIVEHICLEHUMAN_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_AIVEHICLEHUMAN_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/AIVehicleRacecar.h"
#include "mwsdk/game/mw05/Types/IHumanAI.h"

namespace mwsdk::mw05::rt {
  struct AIVehicleHuman : AIVehicleRacecar, IHumanAI {
    bool           bAiControl;
    UMath::Vector3 vMomentPosition;
    float          fMomentRadius;
    bool           mWrongWay;

    virtual ~AIVehicleHuman();
    virtual void                  Update(float deltaTime) override;
    virtual float                 GetSkill() override;
    virtual float                 GetCatchupCheat() override;
    virtual void                  ChangeDragLanes(bool left) override;
    virtual bool                  IsPlayerSteering() override;
    virtual bool                  GetAiControl() override;
    virtual void                  SetAiControl(bool ai_control) override;
    virtual void                  SetWorldMoment(const UMath::Vector3& position, float radius) override;
    virtual const UMath::Vector3& GetWorldMomentPosition() override;
    virtual float                 GetWorldMomentRadius() override;
    virtual void                  ClearWorldMoment() override;
    virtual bool                  IsFacingWrongWay() override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_AIVEHICLEHUMAN_H
