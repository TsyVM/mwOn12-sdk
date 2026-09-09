// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Dynamics.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_DYNAMICS_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_DYNAMICS_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::Dynamics {
  namespace Collision {
    struct Geometry {
      UMath::Vector4 mPosition;
      UMath::Vector4 mNormal[3];
      UMath::Vector4 mExtent[3];
      UMath::Vector4 mCollision_point;
      UMath::Vector4 mCollision_normal;
      float          mDimension[3];
      std::uint16_t  mShape;
      std::uint16_t  mPenetratesOther;
      UMath::Vector3 mDelta;
      float          mOverlap;
    };
  }  // namespace Collision

  struct IEntity {
    virtual const UMath::Vector3& GetPosition()                                                = 0;
    virtual void                  SetPosition(const UMath::Vector3& newPosition)               = 0;
    virtual const UMath::Vector3& GetAngularVelocity()                                         = 0;
    virtual void                  SetAngularVelocity(const UMath::Vector3& newAngularVelocity) = 0;
    virtual const UMath::Vector3& GetLinearVelocity()                                          = 0;
    virtual void                  SetLinearVelocity(const UMath::Vector3& newLinearVelocity)   = 0;
    virtual const UMath::Matrix4& GetRotation()                                                = 0;
    virtual void                  SetRotation(const UMath::Matrix4& newRotation)               = 0;
    virtual const UMath::Vector4& GetOrientation()                                             = 0;
    virtual void                  SetOrientation(const UMath::Vector4& newOrientation)         = 0;
    virtual const UMath::Vector3& GetPrincipalInertia()                                        = 0;
    virtual float                 GetMass()                                                    = 0;
    virtual const UMath::Vector3& GetCenterOfGravity()                                         = 0;
    virtual bool                  IsImmobile()                                                 = 0;
  };
}  // namespace mwsdk::mw05::rt::Dynamics

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_DYNAMICS_H
