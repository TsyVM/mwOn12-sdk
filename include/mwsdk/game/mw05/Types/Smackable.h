// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Smackable.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_SMACKABLE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_SMACKABLE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/IDisposable.h"
#include "mwsdk/game/mw05/Types/IExplodeable.h"
#include "mwsdk/game/mw05/Types/IRenderable.h"
#include "mwsdk/game/mw05/Types/PhysicsObject.h"
#include "mwsdk/game/mw05/Types/SmackableParams.h"
#include "mwsdk/game/mw05/Types/Attrib/Gen/smackable.h"

namespace mwsdk::mw05::rt {
  struct Smackable : PhysicsObject,
                     IDisposable,
                     IRenderable,
                     Sim::Collision::IListener,
                     IExplodeable,
                     EventSequencer::IContext {
    Attrib::Gen::smackable                        mAttributes;
    float                                         mSimplifyWeight;
    float                                         mAge;
    float                                         mLife;
    float                                         mDropTimer;
    const float                                   mDropOutTimerMax;
    float                                         mOffWorldTimer;
    const float                                   mAutoSimplify;
    const bool                                    mVirgin;
    IModel*                                       mModel;
    const CollisionGeometry::Bounds*              mGeometry;
    HSIMTASK__*                                   mManageTask;
    bool                                          mDroppingOut;
    bool                                          mPersistant;
    ICollisionBody*                               mCollisionBody;
    ISimpleBody*                                  mSimpleBody;
    UMath::Vector3                                mLastImpactSpeed;
    BehaviorSpecsPtr<Attrib::Gen::rigidbodyspecs> mRBSpecs;
    UMath::Vector4                                mLastCollisionPosition;

    virtual ~Smackable();
    virtual void OnDebugDraw();
    virtual bool IsRequired() override;

    static Smackable* Construct(const SmackableParams& smackableParams) {
      ISimable* pSimable = reinterpret_cast<ISimable*(__cdecl*)(SmackableParams, UCrc32)>(0x6895A0)(
          smackableParams, smackableParams.mName);
      return static_cast<Smackable*>(pSimable);
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_SMACKABLE_H
