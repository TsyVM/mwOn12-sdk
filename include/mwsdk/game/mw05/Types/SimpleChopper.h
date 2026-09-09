// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/SimpleChopper.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_SIMPLECHOPPER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_SIMPLECHOPPER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib/Gen/chopperspecs.h"
#include "mwsdk/game/mw05/Types/Attrib/Gen/pvehicle.h"
#include "mwsdk/game/mw05/Types/ISimpleChopper.h"
#include "mwsdk/game/mw05/Types/ITransmission.h"
#include "mwsdk/game/mw05/Types/VehicleBehavior.h"

namespace mwsdk::mw05::rt {
  struct SimpleChopper : VehicleBehavior, ISimpleChopper {
    UMath::Vector3                              mLastBodyOffset;
    UMath::Vector3                              mLastAngVelocity;
    UMath::Vector3                              mLastAccelVector;
    UMath::Vector3                              mDesiredVelocity;
    UMath::Vector3                              mPreviousVelocity;
    UMath::Vector3                              mDesiredFacingVector;
    BehaviorSpecsPtr<Attrib::Gen::chopperspecs> mChopperSpecs;
    BehaviorSpecsPtr<Attrib::Gen::pvehicle>     mVehicleSpecs;
    bool                                        mMaxDecelFlag;
    IRigidBody*                                 mIRigidBody;
    ICollisionBody*                             mIRBComplex;
    IDamageable*                                mIDamage;

    virtual ~SimpleChopper();
    virtual float GetPower();
    virtual float GetRPM();
    virtual float GetMaxRPM();
    virtual float GetRedline();
    virtual float GetMinRPM();
    virtual float GetMinGearRPM(ITransmission::GearID gearId);
    virtual void  MatchSpeed(float speed);
    virtual float GetNOSCapacity();
    virtual bool  IsNOSEngaged();
    virtual bool  HasNOS();
    virtual void Reset() override;
    virtual void OnTaskSimulate(float deltaTime) override;
    virtual void OnBehaviorChange(const UCrc32& mechanic) override;
    virtual void SetDesiredVelocity(const UMath::Vector3& velocity) override;
    virtual void GetDesiredVelocity(UMath::Vector3& out) override;
    virtual void MaxDeceleration(bool limit) override;
    virtual void SetDesiredFacingVector(const UMath::Vector3& vector) override;
    virtual void GetDesiredFacingVector(UMath::Vector3& out) override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_SIMPLECHOPPER_H
