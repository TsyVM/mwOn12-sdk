// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IRigidBody.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IRIGIDBODY_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IRIGIDBODY_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IRigidBody : UTL::COM::IUnknown {
    virtual ~IRigidBody();
    virtual ISimable*             GetOwner()                                                      = 0;
    virtual bool                  IsSimple()                                                      = 0;
    virtual std::int32_t          GetIndex()                                                      = 0;
    virtual SimableType           GetSimableType()                                                = 0;
    virtual float                 GetRadius()                                                     = 0;
    virtual float                 GetMass()                                                       = 0;
    virtual float                 GetOOMass()                                                     = 0;
    virtual const UMath::Vector3& GetPosition()                                                   = 0;
    virtual const UMath::Vector3& GetLinearVelocity()                                             = 0;
    virtual const UMath::Vector3& GetAngularVelocity()                                            = 0;
    virtual float                 GetSpeed()                                                      = 0;
    virtual float                 GetSpeedXZ()                                                    = 0;
    virtual void                  GetForwardVector(UMath::Vector3& to)                            = 0;
    virtual void                  GetRightVector(UMath::Vector3& to)                              = 0;
    virtual void                  GetUpVector(UMath::Vector3& to)                                 = 0;
    virtual void                  GetMatrix4(UMath::Matrix4& matrix)                              = 0;
    virtual const UMath::Vector4& GetOrientation()                                                = 0;
    virtual void                  GetDimension(UMath::Vector3& to) const                          = 0;
    virtual void                  GetDimension(UMath::Vector3& to)                                = 0;
    virtual std::uint32_t         GetTriggerFlags()                                               = 0;
    virtual WCollider*            GetWCollider()                                                  = 0;
    virtual void                  GetPointVelocity(const UMath::Vector3& vec, UMath::Vector3& to) = 0;
    virtual void                  SetPosition(const UMath::Vector3& newPosition)                  = 0;
    virtual void                  SetLinearVelocity(const UMath::Vector3& newLinearVelocity)      = 0;
    virtual void                  SetAngularVelocity(const UMath::Vector3& newAngularVelocity)    = 0;
    virtual void                  SetRadius(float newRadius)                                      = 0;
    virtual void                  SetMass(float newMass)                                          = 0;
    virtual void                  SetOrientation(const UMath::Matrix4& newOrientation)            = 0;
    virtual void                  SetOrientation(const UMath::Vector4& newOrientation)            = 0;
    virtual void                  ModifyXPos(float newPosX)                                       = 0;
    virtual void                  ModifyYPos(float newPosY)                                       = 0;
    virtual void                  ModifyZPos(float newPosZ)                                       = 0;
    virtual void                  Resolve(const UMath::Vector3&, const UMath::Vector3&)           = 0;
    virtual void                  ResolveForce(const UMath::Vector3&, const UMath::Vector3&)      = 0;
    virtual void                  ResolveForce(const UMath::Vector3&)                             = 0;
    virtual void                  ResolveTorque(const UMath::Vector3&, const UMath::Vector3&)     = 0;
    virtual void                  ResolveTorque(const UMath::Vector3&)                            = 0;
    virtual void                  PlaceObject(const UMath::Matrix4&, const UMath::Vector3&)       = 0;
    virtual void                  Accelerate(const UMath::Vector3& distribution, float amount)    = 0;
    virtual void                  ConvertLocalToWorld(UMath::Vector3& out, bool normalize)        = 0;
    virtual void                  ConvertWorldToLocal(UMath::Vector3& out, bool normalize)        = 0;
    virtual void                  Debug()                                                         = 0;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IRIGIDBODY_H
