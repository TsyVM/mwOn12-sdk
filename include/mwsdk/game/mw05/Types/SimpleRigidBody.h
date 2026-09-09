// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/SimpleRigidBody.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_SIMPLERIGIDBODY_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_SIMPLERIGIDBODY_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Behavior.h"
#include "mwsdk/game/mw05/Types/bList.h"
#include "mwsdk/game/mw05/Types/IRigidBody.h"
#include "mwsdk/game/mw05/Types/ISimpleBody.h"

namespace mwsdk::mw05::rt {
  struct SimpleRigidBody : Behavior, IRigidBody, ISimpleBody, bTNode<SimpleRigidBody> {
    struct Volatile {
      UMath::Vector4 orientation;
      UMath::Vector3 position;
      std::uint8_t   __unused;
      std::int8_t    index;
      BodyFlags      flags;
      // [South, East, Vertical]
      UMath::Vector3 linearVelocity;
      float          radius;
      // [South, East, Horizontal]
      UMath::Vector3 angularVelocity;
      float          mass;
      UMath::Matrix4 bodyMatrix;

      bool GetFlag(BodyFlags flag) {
        return (static_cast<std::uint32_t>(this->flags) & static_cast<std::uint32_t>(flag)) ==
               static_cast<std::uint32_t>(flag);
      }
      void SetFlag(BodyFlags flag) {
        this->flags =
            static_cast<BodyFlags>(static_cast<std::uint32_t>(this->flags) | static_cast<std::uint32_t>(flag));
      }
      void RemoveStatus(BodyFlags flag) {
        this->flags =
            static_cast<BodyFlags>(static_cast<std::uint32_t>(this->flags) ^ static_cast<std::uint32_t>(flag));
      }

      static inline Volatile** g_mInstances = reinterpret_cast<Volatile**>(0x9384B0);

      static std::int32_t GetInstancesCount() {
        std::int32_t _amount    = 0;
        auto**       _pInstance = g_mInstances;
        while ((*_pInstance)++) _amount++;

        return _amount;
      }

      static Volatile* GetInstance(std::int32_t idx) {
        auto* _instance = g_mInstances[idx];
        if (_instance) return _instance;

        return nullptr;
      }
    };

    ScratchPtr<Volatile> mData;

    virtual ~SimpleRigidBody();
    virtual void OnDebugDraw();
    virtual bool HasHadCollision();
    virtual ISimable*             GetOwner() override;
    virtual bool                  IsSimple() override;
    virtual std::int32_t          GetIndex() override;
    virtual SimableType           GetSimableType() override;
    virtual float                 GetRadius() override;
    virtual float                 GetMass() override;
    virtual float                 GetOOMass() override;
    virtual const UMath::Vector3& GetPosition() override;
    virtual const UMath::Vector3& GetLinearVelocity() override;
    virtual const UMath::Vector3& GetAngularVelocity() override;
    virtual float                 GetSpeed() override;
    virtual float                 GetSpeedXZ() override;
    virtual void                  GetForwardVector(UMath::Vector3& out) override;
    virtual void                  GetRightVector(UMath::Vector3& out) override;
    virtual void                  GetUpVector(UMath::Vector3& out) override;
    virtual void                  GetMatrix4(UMath::Matrix4& out) override;
    virtual const UMath::Vector4& GetOrientation() override;
    virtual void                  GetDimension(UMath::Vector3& out) const override;
    virtual void                  GetDimension(UMath::Vector3& out) override;
    virtual std::uint32_t         GetTriggerFlags() override;
    virtual WCollider*            GetWCollider() override;
    virtual void                  GetPointVelocity(const UMath::Vector3& vec, UMath::Vector3& out) override;
    virtual void                  SetPosition(const UMath::Vector3& newPosition) override;
    virtual void                  SetLinearVelocity(const UMath::Vector3& newLinearVelocity) override;
    virtual void                  SetAngularVelocity(const UMath::Vector3& newAngularVelocity) override;
    virtual void                  SetRadius(float newRadius) override;
    virtual void                  SetMass(float newMass) override;
    virtual void                  SetOrientation(const UMath::Matrix4& newOrientation) override;
    virtual void                  SetOrientation(const UMath::Vector4& newOrientation) override;
    virtual void                  ModifyXPos(float newPosX) override;
    virtual void                  ModifyYPos(float newPosY) override;
    virtual void                  ModifyZPos(float newPosZ) override;
    virtual void                  Resolve(const UMath::Vector3&, const UMath::Vector3&) override;
    virtual void                  ResolveForce(const UMath::Vector3&, const UMath::Vector3&) override;
    virtual void                  ResolveForce(const UMath::Vector3&) override;
    virtual void                  ResolveTorque(const UMath::Vector3&, const UMath::Vector3&) override;
    virtual void                  ResolveTorque(const UMath::Vector3&) override;
    virtual void                  PlaceObject(const UMath::Matrix4&, const UMath::Vector3&) override;
    virtual void                  Accelerate(const UMath::Vector3& distribution, float amount) override;
    virtual void                  ConvertLocalToWorld(UMath::Vector3& out, bool normalize) override;
    virtual void                  ConvertWorldToLocal(UMath::Vector3& out, bool normalize) override;
    virtual void                  Debug() override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_SIMPLERIGIDBODY_H
