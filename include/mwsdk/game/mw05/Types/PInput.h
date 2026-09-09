// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/PInput.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_PINPUT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_PINPUT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Behavior.h"
#include "mwsdk/game/mw05/Types/IInput.h"

namespace mwsdk::mw05::rt {
  struct PInput : Behavior, IInput {
    InputControls mControls;

    virtual ~PInput();
    virtual void SetControlStrafeVertical(float amount);
    virtual void SetControlStrafeHorizontal(float amount);
    virtual void           Reset() override;
    virtual void           OnTaskSimulate(float deltaTime);
    virtual void           OnBehaviorChange(const UCrc32& mechanic);
    virtual void           ClearInput() override;
    virtual InputControls& GetControls() override;
    virtual float          GetControlHandBrake() override;
    virtual float          GetControlActionButton() override;
    virtual void           SetControlSteering(float amount) override;
    virtual void           SetControlGas(float amount) override;
    virtual void           SetControlBrake(float amount) override;
    virtual void           SetControlNOS(bool isPressed) override;
    virtual void           SetControlHandBrake(float amount) override;
    virtual void           SetControlActionButton(bool isPressed) override;
    virtual void           SetControlSteeringVertical(float amount) override;
    virtual void           SetControlBanking(float amount) override;
    virtual float          GetControlBanking() override;
    virtual bool           IsLookBackButtonPressed() override;
    virtual bool           IsPullBackButtonPressed() override;
    virtual bool           IsAutomaticShift() override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_PINPUT_H
