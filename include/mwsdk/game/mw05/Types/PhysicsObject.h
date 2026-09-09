// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/PhysicsObject.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_PHYSICSOBJECT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_PHYSICSOBJECT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/eastl.hpp"
#include "mwsdk/game/mw05/compat/eastl.hpp"

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attachments.h"
#include "mwsdk/game/mw05/Types/Behavior.h"
#include "mwsdk/game/mw05/Types/IBody.h"
#include "mwsdk/game/mw05/Types/ISimable.h"
#include "mwsdk/game/mw05/Types/Attrib/Instance.h"

namespace mwsdk::mw05::rt {
  struct PhysicsObject : Sim::Object,
                         ISimable,
                         IBody,
                         IAttachable,
                         UTL::GarbageNode<PhysicsObject, 256>,
                         Behavior::Container {
    WWorldPos*              mWPos;
    SimableType             mObjType;
    Attrib::Instance        mPhysicsObjectAttributes;
    IRigidBody*             mRigidBody;
    HSIMTASK__*             mSimulateTask;
    Sim::IEntity*           mEntity;
    IPlayer*                mPlayer;
    HSIMSERVICE__*          mBodyService;
    std::uint32_t           mWorldID;
    eastl::slist<Behavior*> mMechanics;
    eastl::slist<Behavior*> mBehaviors;
    Attachments*            mAttachments;

    virtual ~PhysicsObject();
    virtual void Reset();
    virtual void OnTaskSimulate(float) = 0;
    virtual void OnBehaviorChange(const UCrc32&);
    virtual bool                     OnTask(HSIMTASK__*, float) override;
    virtual SimableType              GetSimableType() override;
    virtual void                     Kill() override;
    virtual bool                     Attach(UTL::COM::IUnknown* object) override;
    virtual bool                     Detach(UTL::COM::IUnknown* object) override;
    virtual Attachments*             GetAttachments() override;
    virtual void                     AttachEntity(Sim::IEntity* entity) override;
    virtual void                     DetachEntity() override;
    virtual IPlayer*                 GetPlayer() override;
    virtual bool                     IsPlayer() override;
    virtual bool                     IsOwnedByPlayer() override;
    virtual Sim::IEntity*            GetEntity() override;
    virtual void                     DebugObject() override;
    virtual HSIMABLE__*              GetOwnerHandle() override;
    virtual ISimable*                GetOwner() override;
    virtual bool                     IsOwnedBy(ISimable* pSimable) override;
    virtual void                     SetOwnerObject(ISimable* pSimable) override;
    virtual const Attrib::Instance&  GetAttributes() override;
    virtual const WWorldPos&         GetWPos() const override;
    virtual WWorldPos&               GetWPos() override;
    virtual const IRigidBody*        GetRigidBody() const override;
    virtual IRigidBody*              GetRigidBody() override;
    virtual bool                     IsRigidBodySimple() override;
    virtual bool                     IsRigidBodyComplex() override;
    virtual const UMath::Vector3&    GetPosition() override;
    virtual void                     GetTransform(UMath::Matrix4& matrix) override;
    virtual void                     GetLinearVelocity(UMath::Vector3& to) override;
    virtual void                     GetAngularVelocity(UMath::Vector3& to) override;
    virtual std::uint32_t            GetWorldID() override;
    virtual EventSequencer::IEngine* GetEventSequencer() override;
    virtual void                     ProcessStimulus(std::uint32_t) override;
    virtual const IModel*            GetModel() const override;
    virtual IModel*                  GetModel() override;
    virtual void                     SetCausality(HCAUSE__* p, float time) override;
    virtual HCAUSE__*                GetCausality() override;
    virtual float                    GetCausalityTime() override;
    virtual void                     GetDimension(UMath::Vector3& to) override;
    virtual bool                     IsAttached(UTL::COM::IUnknown* pOther) override;
    virtual void                     OnAttached(IAttachable* pOther) override;
    virtual void                     OnDetached(IAttachable* pOther) override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_PHYSICSOBJECT_H
