// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Behavior.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_BEHAVIOR_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_BEHAVIOR_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/eastl.hpp"

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Sim.h"
#include "mwsdk/game/mw05/Types/UCrc32.h"

namespace mwsdk::mw05::rt {
  struct Behavior : Sim::Object {
    struct Container {
      struct Elements : eastl::list<Behavior*> {};

      Elements _mElements;
    };

    bool           mPaused;
    PhysicsObject* mOwner;
    ISimable*      mIOwner;
    UCrc32         mMechanic;
    UCrc32         mSignature;
    std::int32_t   mPriority;
    HSIMPROFILE__* mProfile;

    virtual ~Behavior();
    virtual void         Reset() = 0;
    virtual std::int32_t GetPriority();
    virtual void         OnOwnerAttached(IAttachable* pOther);
    virtual void         OnOwnerDetached(IAttachable* pOther);
    virtual void         OnTaskSimulate(float deltaTime);
    virtual void         OnBehaviorChange(const UCrc32& mechanic);
    virtual void         OnPause();
    virtual void         OnUnPause();
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_BEHAVIOR_H
