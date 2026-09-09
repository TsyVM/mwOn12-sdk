// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IPerpetrator.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IPERPETRATOR_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IPERPETRATOR_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IPerpetrator : UTL::COM::IUnknown {
    virtual ~IPerpetrator();

    virtual float                           GetHeat()                                                    = 0;
    virtual void                            SetHeat(float heat)                                          = 0;
    virtual Attrib::Gen::pursuitescalation* GetPursuitEscalationAttrib()                                 = 0;
    virtual Attrib::Gen::pursuitlevels*     GetPursuitLevelAttrib()                                      = 0;
    virtual Attrib::Gen::pursuitsupport*    GetPursuitSupportAttrib()                                    = 0;
    virtual void                            AddCostToState(std::int32_t cost)                            = 0;
    virtual std::int32_t                    GetCostToState()                                             = 0;
    virtual void                            SetCostToState(std::int32_t costToState)                     = 0;
    virtual bool                            IsRacing()                                                   = 0;
    virtual bool                            IsBeingPursued()                                             = 0;
    virtual bool                            IsHiddenFromCars()                                           = 0;
    virtual bool                            IsHiddenFromHelicopters()                                    = 0;
    virtual bool                            IsPartiallyHidden(float& howHidden)                          = 0;
    virtual void                            AddToPendingRepPointsFromCopDestruction(std::int32_t amount) = 0;
    virtual void                            AddToPendingRepPointsNormal(std::int32_t amount)             = 0;
    virtual std::int32_t                    GetPendingRepPointsNormal()                                  = 0;
    virtual std::int32_t                    GetPendingRepPointsFromCopDestruction()                      = 0;
    virtual void                            ClearPendingRepPoints()                                      = 0;
    virtual void                            SetRacerInfo(GRacerInfo* info)                               = 0;
    virtual GRacerInfo*                     GetRacerInfo()                                               = 0;
    virtual float                           GetLastTrafficHitTime()                                      = 0;
    virtual void                            Set911CallTime(float time)                                   = 0;
    virtual float                           Get911CallTime()                                             = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x4037E0)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IPERPETRATOR_H
