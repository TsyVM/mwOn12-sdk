// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/ISimable.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ISIMABLE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ISIMABLE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/eastl.hpp"

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib.h"
#include "mwsdk/game/mw05/Types/EventSequencer.h"
#include "mwsdk/game/mw05/Types/IAttachable.h"
#include "mwsdk/game/mw05/Types/WWorldPos.h"

namespace mwsdk::mw05::rt {
  struct ISimable : UTL::COM::IUnknown {
    std::uint32_t _mHandle;  // from UTL::Instanceable<HSIMABLE__ *, ISimable, n>

    virtual ~ISimable();
    virtual SimableType              GetSimableType()                       = 0;
    virtual void                     Kill()                                 = 0;
    virtual bool                     Attach(UTL::COM::IUnknown* object)     = 0;
    virtual bool                     Detach(UTL::COM::IUnknown* object)     = 0;
    virtual Attachments*             GetAttachments()                       = 0;
    virtual void                     AttachEntity(Sim::IEntity* entity)     = 0;
    virtual void                     DetachEntity()                         = 0;
    virtual IPlayer*                 GetPlayer()                            = 0;
    virtual bool                     IsPlayer()                             = 0;
    virtual bool                     IsOwnedByPlayer()                      = 0;
    virtual Sim::IEntity*            GetEntity()                            = 0;
    virtual void                     DebugObject()                          = 0;
    virtual HSIMABLE__*              GetOwnerHandle()                       = 0;
    virtual ISimable*                GetOwner()                             = 0;
    virtual bool                     IsOwnedBy(ISimable* pSimable)          = 0;
    virtual void                     SetOwnerObject(ISimable* pSimable)     = 0;
    virtual const Attrib::Instance&  GetAttributes()                        = 0;
    virtual const WWorldPos&         GetWPos() const                        = 0;
    virtual WWorldPos&               GetWPos()                              = 0;
    virtual const IRigidBody*        GetRigidBody() const                   = 0;
    virtual IRigidBody*              GetRigidBody()                         = 0;
    virtual bool                     IsRigidBodySimple()                    = 0;
    virtual bool                     IsRigidBodyComplex()                   = 0;
    virtual const UMath::Vector3&    GetPosition()                          = 0;
    virtual void                     GetTransform(UMath::Matrix4& matrix)   = 0;
    virtual void                     GetLinearVelocity(UMath::Vector3& to)  = 0;
    virtual void                     GetAngularVelocity(UMath::Vector3& to) = 0;
    virtual std::uint32_t            GetWorldID()                           = 0;
    virtual EventSequencer::IEngine* GetEventSequencer()                    = 0;
    virtual void                     ProcessStimulus(std::uint32_t)         = 0;
    virtual const IModel*            GetModel() const                       = 0;
    virtual IModel*                  GetModel()                             = 0;
    virtual void                     SetCausality(HCAUSE__* p, float time)  = 0;
    virtual HCAUSE__*                GetCausality()                         = 0;
    virtual float                    GetCausalityTime()                     = 0;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ISIMABLE_H
