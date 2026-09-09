// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IInput.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IINPUT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IINPUT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"
#include "mwsdk/game/mw05/Types/InputControls.h"

namespace mwsdk::mw05::rt {
  struct IInput : UTL::COM::IUnknown {
    virtual ~IInput();
    virtual void           ClearInput()                             = 0;
    virtual InputControls& GetControls()                            = 0;
    virtual float          GetControlHandBrake()                    = 0;
    virtual float          GetControlActionButton()                 = 0;
    virtual void           SetControlSteering(float amount)         = 0;
    virtual void           SetControlGas(float amount)              = 0;
    virtual void           SetControlBrake(float amount)            = 0;
    virtual void           SetControlNOS(bool isPressed)            = 0;
    virtual void           SetControlHandBrake(float amount)        = 0;
    virtual void           SetControlActionButton(bool isPressed)   = 0;
    virtual void           SetControlSteeringVertical(float amount) = 0;
    virtual void           SetControlBanking(float amount)          = 0;
    virtual float          GetControlBanking()                      = 0;
    virtual bool           IsLookBackButtonPressed()                = 0;
    virtual bool           IsPullBackButtonPressed()                = 0;
    virtual bool           IsAutomaticShift()                       = 0;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IINPUT_H
