// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IAIHelicopter.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IAIHELICOPTER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IAIHELICOPTER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IAIHelicopter : UTL::COM::IUnknown {
    virtual ~IAIHelicopter();
    virtual float          GetDesiredHeightOverDest()                                                  = 0;
    virtual void           SetDesiredHeightOverDest(float height)                                      = 0;
    virtual void           SetLookAtPosition(UMath::Vector3 pos)                                       = 0;
    virtual UMath::Vector3 GetLookAtPosition()                                                         = 0;
    virtual void           SetDestinationVelocity(const UMath::Vector3& velocity)                      = 0;
    virtual void           SteerToNav(WRoadNav* road_nav, float height, float speed, bool bStopAtDest) = 0;
    virtual bool           StartPathToPoint(UMath::Vector3& point)                                     = 0;
    virtual bool           StrafeToDestIsSet()                                                         = 0;
    virtual void           SetStrafeToDest(bool strafe)                                                = 0;
    virtual bool           FilterHeliAltitude(UMath::Vector3& point)                                   = 0;
    virtual void           RestrictPointToRoadNet(UMath::Vector3& seekPosition)                        = 0;
    virtual void           SetFuelFull()                                                               = 0;
    virtual float          GetFuelTimeRemaining()                                                      = 0;
    virtual void           SetShadowScale(float scale)                                                 = 0;
    virtual float          GetShadowScale()                                                            = 0;
    virtual void           SetDustStormIntensity(float intensity)                                      = 0;
    virtual float          GetDustStormIntensity()                                                     = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x404060)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IAIHELICOPTER_H
