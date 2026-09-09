// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IEngine.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IENGINE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IENGINE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IEngine : UTL::COM::IUnknown {
    virtual ~IEngine();
    virtual float GetRPM()                = 0;
    virtual float GetRedline()            = 0;
    virtual float GetMaxRPM()             = 0;
    virtual float GetMinRPM()             = 0;
    virtual float GetPeakTorqueRPM()      = 0;
    virtual void  MatchSpeed(float speed) = 0;
    virtual float GetNOSCapacity()        = 0;
    virtual bool  IsNOSEngaged()          = 0;
    virtual float GetNOSFlowRate()        = 0;
    virtual float GetNOSBoost()           = 0;
    virtual bool  HasNOS()                = 0;
    virtual void  ChargeNOS(float amount) = 0;
    virtual float GetMaxHorsePower()      = 0;
    virtual float GetMinHorsePower()      = 0;
    virtual float GetHorsePower()         = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x404020)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IENGINE_H
