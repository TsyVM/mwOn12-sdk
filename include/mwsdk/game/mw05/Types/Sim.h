// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Sim.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_SIM_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_SIM_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/eastl.hpp"

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/IAttachable.h"
#include "mwsdk/game/mw05/Types/UCrc32.h"
#include "mwsdk/game/mw05/Types/Attrib/Gen/simsurface.h"

namespace mwsdk::mw05::rt {
  namespace Sim {
    namespace Collision {
      struct IListener {
        virtual void OnCollision(const Info& cinfo);
      };
      struct Info {
        UMath::Vector3            position;
        const Attrib::Collection* objAsurface;
        UMath::Vector3            normal;
        CollisionType             type : 3;
        std::int32_t              objAImmobile : 1;
        std::int32_t              objADetached : 1;
        std::int32_t              objBImmobile : 1;
        std::int32_t              objBDetached : 1;
        std::int32_t              sliding : 1;
        std::int32_t              unused : 24;
        UMath::Vector3            closingVel;
        float                     force;
        UMath::Vector3            armA;
        HSIMABLE__*               objA;
        UMath::Vector3            armB;
        HSIMABLE__*               objB;
        UMath::Vector3            objAVel;
        float                     impulseA;
        UMath::Vector3            objBVel;
        float                     impulseB;
        UMath::Vector3            slidingVel;
        const Attrib::Collection* objBsurface;
      };
    }  // namespace Collision

    struct IActivity : UTL::COM::IUnknown {
      std::uint32_t _mHandle;  // from UTL::Instanceable<HACTIVITY__*, Sim::IActivity, n>

      virtual ~IActivity();
      virtual void         Release();
      virtual bool         Attach(UTL::COM::IUnknown* object) = 0;
      virtual bool         Detach(UTL::COM::IUnknown* object) = 0;
      virtual Attachments* GetAttachments()                   = 0;
    };
    struct IEntity : UTL::COM::IUnknown {
      virtual ~IEntity();
      virtual void                  AttachPhysics(ISimable*);
      virtual void                  DetachPhysics();
      virtual ISimable*             GetSimable();
      virtual const UMath::Vector3& GetPosition();
      virtual bool                  SetPosition(const UMath::Vector3& newPosition) = 0;
      virtual void                  Kill();
      virtual bool                  Attach(UTL::COM::IUnknown* object);
      virtual bool                  Detach(UTL::COM::IUnknown* object);
      virtual Attachments*          GetAttachments();
    };
    struct IServiceable : UTL::COM::IUnknown {
      virtual ~IServiceable();
      virtual bool OnService(HSIMSERVICE__* hCon, Packet* pkt) = 0;
    };
    struct ITaskable : UTL::COM::IUnknown {
      virtual ~ITaskable();
      virtual bool OnTask(HSIMTASK__* hTask, float deltaTime) = 0;
    };

    struct Object : UTL::COM::Object, IServiceable, ITaskable {
      std::uint8_t  _unk[0x4];
      std::uint32_t mTaskCount;
      std::uint32_t mServiceCount;

      virtual ~Object();
      virtual bool OnService(HSIMSERVICE__* p, Packet*) override;
      virtual bool OnTask(HSIMTASK__* hTask, float deltaTime) override;
    };

    struct Activity : Object, UTL::GarbageNode<Activity, 40>, IActivity, IAttachable {
      // std::uint8_t _unk[0x4];
      Attachments* mAttachments;

      virtual ~Activity();
      virtual void         Release();
      virtual bool         Attach(UTL::COM::IUnknown* object) override;
      virtual bool         Detach(UTL::COM::IUnknown* object) override;
      virtual bool         IsAttached(UTL::COM::IUnknown* object) override;
      virtual void         OnAttached(IAttachable* pOther) override;
      virtual void         OnDetached(IAttachable* pOther) override;
      virtual Attachments* GetAttachments() override;
    };

    struct Entity : Object, UTL::GarbageNode<Entity, 12>, IEntity, IAttachable {
      ISimable*    mSimable;
      Attachments* mAttachments;

      virtual ~Entity();
    };

    struct Packet {
      virtual UCrc32        ConnectionClass() = 0;
      virtual std::uint32_t Compress(Packet*);
      virtual std::uint32_t Decompress(Packet*);
      virtual std::uint32_t Type() = 0;
      virtual std::uint32_t Size() = 0;
      virtual ~Packet();
    };

    struct Param {
      UCrc32       mType;
      UCrc32       mName;
      void*        mData;
      std::uint8_t __pad[0x4];

      UCrc32 GetType() { return mType; }
      UCrc32 GetName() { return mName; }
      void*  GetData() { return &__pad + sizeof(__pad); }

      Param(UCrc32 hash) : mName(hash), mData(this) {}
    };
  }  // namespace Sim

  struct SimSurface : Attrib::Gen::simsurface {};
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_SIM_H
