// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/DamageVehicle.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_DAMAGEVEHICLE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_DAMAGEVEHICLE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/eastl.hpp"

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib/Gen/damagespecs.h"
#include "mwsdk/game/mw05/Types/DamageZone.h"
#include "mwsdk/game/mw05/Types/EventSequencer.h"
#include "mwsdk/game/mw05/Types/IDamageable.h"
#include "mwsdk/game/mw05/Types/IDamageableVehicle.h"
#include "mwsdk/game/mw05/Types/VehicleBehavior.h"

namespace mwsdk::mw05::rt {
  struct DamageVehicle : VehicleBehavior,
                         IDamageable,
                         Sim::Collision::IListener,
                         IDamageableVehicle,
                         EventSequencer::IContext {
    float                                      mShockTimer;
    int                                        fTempInvincibilityTimer;
    BehaviorSpecsPtr<Attrib::Gen::damagespecs> mSpecs;
    float                                      mOffScreenTimer;
    float                                      mDamageTotal;
    ICollisionBody*                            mIRBComplex;
    IRigidBody*                                mRB;
    IRenderable*                               mRenderable;
    DamageZone::Info                           mZoneDamage;
    UMath::Vector3                             mLastImpactSpeed;
    eastl::slist<UCrc32>                       mBrokenParts;
    VehicleFX::LightID                         mLightDamage;

    virtual ~DamageVehicle();
    virtual void OnImpact(const UMath::Vector3&, const UMath::Vector3&, float, float, const SimSurface&, ISimable*);
    virtual bool CanDamageVisuals();

    virtual void                  Reset() override;
    virtual void                  OnTaskSimulate(float deltaTime) override;
    virtual void                  OnBehaviorChange(const UCrc32& mechanic) override;
    virtual void                  SetInShock(float) override;
    virtual void                  SetShockForce(float) override;
    virtual float                 InShock() override;
    virtual void                  ResetDamage() override;
    virtual float                 GetHealth() override;
    virtual bool                  IsDestroyed() override;
    virtual void                  Destroy() override;
    virtual Sim::Collision::Info* GetZoneDamage() override;
    virtual void                  OnCollision(const Sim::Collision::Info& cinfo) override;
    virtual bool                  IsLightDamaged(VehicleFX::LightID idx) override;
    virtual void                  DamageLight(VehicleFX::LightID idx, bool damage) override;
    virtual bool                  SetDynamicData(void* System, void* pOutEventDynamicData) override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_DAMAGEVEHICLE_H
