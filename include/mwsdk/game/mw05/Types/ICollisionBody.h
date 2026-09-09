// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/ICollisionBody.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ICOLLISIONBODY_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ICOLLISIONBODY_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UCrc32.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct ICollisionBody : UTL::COM::IUnknown {
    virtual ~ICollisionBody();
    virtual const UMath::Vector3& GetPosition()                                                = 0;
    virtual const UMath::Vector3& GetLinearVelocity()                                          = 0;
    virtual const UMath::Vector3& GetAngularVelocity()                                         = 0;
    virtual void                  GetDimension(UMath::Vector3& to)                             = 0;
    virtual void                  Damp(float amount)                                           = 0;
    virtual void                  SetAnimating(bool isAnimating)                               = 0;
    virtual bool                  IsModeling()                                                 = 0;
    virtual void                  DisableModeling()                                            = 0;
    virtual void                  EnableModeling()                                             = 0;
    virtual bool                  IsTriggering()                                               = 0;
    virtual void                  DisableTriggering()                                          = 0;
    virtual void                  EnableTriggering()                                           = 0;
    virtual bool                  IsSleeping()                                                 = 0;
    virtual void                  SetCenterOfGravity(const UMath::Vector3& newCenterOfGravity) = 0;
    virtual const UMath::Vector3& GetCenterOfGravity()                                         = 0;
    virtual bool                  HasHadCollision()                                            = 0;
    virtual bool                  HasHadWorldCollision()                                       = 0;
    virtual bool                  HasHadObjectCollision()                                      = 0;
    virtual void                  EnableCollisionGeometries(UCrc32 crc, bool isEnabled)        = 0;
    virtual bool                  DistributeMass()                                             = 0;
    virtual const UMath::Vector3& GetWorldMomentScale()                                        = 0;
    virtual const UMath::Vector3& GetGroundMomentScale()                                       = 0;
    virtual bool                  IsAttachedToWorld()                                          = 0;
    virtual void                  AttachedToWorld(bool, float)                                 = 0;
    virtual bool                  IsAnchored()                                                 = 0;
    virtual void                  SetAnchored(bool isAnchored)                                 = 0;
    virtual void                  SetInertiaTensor(const UMath::Vector3& newInertiaTensor)     = 0;
    virtual const UMath::Vector3& GetInertiaTensor()                                           = 0;
    virtual float                 GetOrientToGround()                                          = 0;
    virtual bool                  IsInGroundContact()                                          = 0;
    virtual std::int32_t          GetNumContactPoints()                                        = 0;
    virtual const UMath::Vector4& GetGroundNormal()                                            = 0;
    virtual void                  SetForce(const UMath::Vector3& newForce)                     = 0;
    virtual void                  SetTorque(const UMath::Vector3& newTorque)                   = 0;
    virtual const UMath::Vector3& GetForce()                                                   = 0;
    virtual const UMath::Vector3& GetTorque()                                                  = 0;
    virtual float                 GetGravity()                                                 = 0;
    virtual const UMath::Vector3& GetForwardVector()                                           = 0;
    virtual const UMath::Vector3& GetRightVector()                                             = 0;
    virtual const UMath::Vector3& GetUpVector()                                                = 0;
    virtual const UMath::Matrix4& GetMatrix4()                                                 = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x4039C0)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ICOLLISIONBODY_H
