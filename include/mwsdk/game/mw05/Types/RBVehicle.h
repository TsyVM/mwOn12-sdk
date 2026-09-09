// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/RBVehicle.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_RBVEHICLE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_RBVEHICLE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/IRBVehicle.h"
#include "mwsdk/game/mw05/Types/RigidBody.h"
#include "mwsdk/game/mw05/Types/Attrib/Gen/collisionreactions.h"

namespace mwsdk::mw05::rt {
  struct RBVehicle : RigidBody, IRBVehicle {
    IVehicle*                                     mVehicle;
    ISuspension*                                  mSuspension;
    BehaviorSpecsPtr<Attrib::Gen::rigidbodyspecs> mSpecs;
    float                                         mDeadOnWheels;
    CollisionGeometry::Collection*                mGeoms;
    std::uint32_t                                 mFrame;
    UMath::Vector3                                mCollisionCenterOfGravity;
    float                                         mCollisionMass;
    bool                                          mObjectCollisionsEnabled;
    eInvulnerability                              mInvulnerableState;
    float                                         mInvulnerableTimer;
    float                                         mLastPenetration;
    Attrib::Gen::collisionreactions               mReactions;

    virtual ~RBVehicle();
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_RBVEHICLE_H
