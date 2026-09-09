// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/ISuspension.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ISUSPENSION_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ISUSPENSION_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct ISuspension : UTL::COM::IUnknown {
    virtual ~ISuspension();
    virtual float                 GetWheelTraction(std::uint32_t idx)                              = 0;
    virtual std::uint32_t         GetNumWheels()                                                   = 0;
    virtual const UMath::Vector3& GetWheelPos(std::uint32_t idx)                                   = 0;
    virtual const UMath::Vector3& GetWheelLocalPos(std::uint32_t idx)                              = 0;
    virtual UMath::Vector3        GetWheelCenterPos(std::uint32_t idx)                             = 0;
    virtual float                 GetWheelLoad(std::uint32_t idx)                                  = 0;
    virtual void                  ApplyVehicleEntryForces(bool, const UMath::Vector3&, bool)       = 0;
    virtual const float           GetWheelRoadHeight(std::uint32_t idx)                            = 0;
    virtual bool                  IsWheelOnGround(std::uint32_t idx)                               = 0;
    virtual float                 GetCompression(std::uint32_t)                                    = 0;
    virtual float                 GuessCompression(std::uint32_t, float)                           = 0;
    virtual float                 GetWheelSlip(std::uint32_t idx)                                  = 0;
    virtual float                 GetToleratedSlip(std::uint32_t)                                  = 0;
    virtual float                 GetWheelSkid(std::uint32_t idx)                                  = 0;
    virtual float                 GetWheelSlipAngle(std::uint32_t idx)                             = 0;
    virtual const UMath::Vector4& GetWheelRoadNormal(std::uint32_t idx)                            = 0;
    virtual const SimSurface&     GetWheelRoadSurface(std::uint32_t idx)                           = 0;
    virtual const UMath::Vector3& GetWheelVelocity(std::uint32_t idx)                              = 0;
    virtual std::uint32_t         GetNumWheelsOnGround()                                           = 0;
    virtual float                 GetWheelAngularVelocity(std::int32_t idx)                        = 0;
    virtual void                  SetWheelAngularVelocity(std::int32_t idx, float angularVelocity) = 0;
    virtual float                 GetWheelSteer(std::uint32_t idx)                                 = 0;
    virtual float                 CalculateUndersteerFactor()                                      = 0;
    virtual float                 CalculateOversteerFactor()                                       = 0;
    virtual float                 GetRideHeight(std::uint32_t)                                     = 0;
    virtual float                 GetWheelRadius(std::uint32_t idx)                                = 0;
    virtual float                 GetMaxSteering()                                                 = 0;
    virtual void                  MatchSpeed(float speed)                                          = 0;
    virtual float                 GetRenderMotion()                                                = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x404040)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ISUSPENSION_H
