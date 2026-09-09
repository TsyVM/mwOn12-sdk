// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IVehicle.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IVEHICLE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IVEHICLE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib.h"
#include "mwsdk/game/mw05/Types/FECustomizationRecord.h"
#include "mwsdk/game/mw05/Types/UCrc32.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IVehicle : UTL::COM::IUnknown {
    enum class ForceStopType : std::uint8_t {
      ForceStop   = 1 << 0,
      InstantStop = 1 << 1,
      ForceCoast  = 1 << 2,
      StopOnline  = 1 << 4
    };

    virtual ~IVehicle();
    virtual ISimable*                    GetSimable()                                                     = 0;
    virtual const ISimable*              GetSimable() const                                               = 0;
    virtual const UMath::Vector3&        GetPosition()                                                    = 0;
    virtual void                         SetBehaviorOverride(UCrc32, UCrc32)                              = 0;
    virtual void                         RemoveBehaviorOverride(UCrc32)                                   = 0;
    virtual void                         CommitBehaviorOverrides()                                        = 0;
    virtual void                         SetStaging(bool isStaging)                                       = 0;
    virtual bool                         IsStaging()                                                      = 0;
    virtual void                         Launch()                                                         = 0;
    virtual float                        GetPerfectLaunch()                                               = 0;
    virtual void                         SetDriverStyle(DriverStyle newDriverStyle)                       = 0;
    virtual DriverStyle                  GetDriverStyle()                                                 = 0;
    virtual void                         SetPhysicsMode(PhysicsMode newPhysicsMode)                       = 0;
    virtual PhysicsMode                  GetPhysicsMode()                                                 = 0;
    virtual std::int32_t                 GetModelType()                                                   = 0;
    virtual bool                         IsSpooled()                                                      = 0;
    virtual const UCrc32&                GetVehicleClass()                                                = 0;
    virtual const Attrib::Gen::pvehicle& GetVehicleAttributes()                                           = 0;
    virtual const char*                  GetVehicleName()                                                 = 0;
    virtual std::uint32_t                GetVehicleKey()                                                  = 0;
    virtual void                         SetDriverClass(DriverClass newDriverClass)                       = 0;
    virtual DriverClass                  GetDriverClass()                                                 = 0;
    virtual bool                         IsLoading()                                                      = 0;
    virtual float                        GetOffScreenTime()                                               = 0;
    virtual float                        GetOnScreenTime()                                                = 0;
    virtual bool                         SetVehicleOnGround(const UMath::Vector3&, const UMath::Vector3&) = 0;
    virtual void                         ForceStopOn(ForceStopType type)                                  = 0;
    virtual void                         ForceStopOff(ForceStopType type)                                 = 0;
    virtual ForceStopType                GetForceStop()                                                   = 0;
    virtual bool                         InShock()                                                        = 0;
    virtual bool                         IsDestroyed()                                                    = 0;
    virtual void                         Activate()                                                       = 0;
    virtual void                         Deactivate()                                                     = 0;
    virtual bool                         IsActive()                                                       = 0;
    virtual float                        GetSpeedometer()                                                 = 0;
    virtual float                        GetSpeed()                                                       = 0;
    virtual void                         SetSpeed(float newSpeed)                                         = 0;
    virtual float                        GetAbsoluteSpeed()                                               = 0;
    virtual bool                         IsGlareOn(VehicleFX::LightID fxId)                               = 0;
    virtual void                         GlareOn(VehicleFX::LightID fxId)                                 = 0;
    virtual void                         GlareOff(VehicleFX::LightID fxId)                                = 0;
    virtual bool                         IsCollidingWithSoftBarrier()                                     = 0;
    virtual IVehicleAI*                  GetAIVehiclePtr()                                                = 0;
    virtual float                        GetSlipAngle()                                                   = 0;
    virtual const UMath::Vector3&        GetLocalVelocity()                                               = 0;
    virtual void                         ComputeHeading(UMath::Vector3* out)                              = 0;
    virtual bool                         IsAnimating()                                                    = 0;
    virtual void                         SetAnimating(bool isAnimating)                                   = 0;
    virtual bool                         IsOffWorld()                                                     = 0;
    virtual FECustomizationRecord*       GetCustomizations()                                              = 0;
    virtual Physics::Tunings*            GetTunings()                                                     = 0;
    virtual void                         SetTunings(const Physics::Tunings& tunings)                      = 0;
    virtual bool                         GetPerformance(Physics::Info::CorrectedPerformance& to)          = 0;
    virtual void*                        _unkFunc()                                                       = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x4040E0)(); }
  };

#if 1  // portable flag operators
  DEFINE_ENUM_FLAG_OPERATORS(IVehicle::ForceStopType)
#endif
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IVEHICLE_H
