// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/AITarget.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_AITARGET_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_AITARGET_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include <new>

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/bNode.h"

namespace mwsdk::mw05::rt {
  struct AITarget : bTNode<AITarget> {
    ISimable*      mOwner;
    UMath::Vector3 mTargetPosition;
    ISimable*      mTargetSimable;
    UMath::Vector3 mTargetDirection;
    bool           mValid;
    UMath::Vector3 mDirTo;
    float          mDistTo;

    virtual ~AITarget();

    inline void Clear() { reinterpret_cast<void(__thiscall*)(AITarget*)>(0x409E60)(this); }
    inline void Acquire(ISimable* target) {
      reinterpret_cast<void(__thiscall*)(AITarget*, ISimable*)>(0x423860)(this, target);
    }
    inline void Register(ISimable* who) {
      reinterpret_cast<void(__thiscall*)(AITarget*, ISimable*)>(0x423750)(this, who);
    }
    inline void TrackInternal(ISimable* who) { reinterpret_cast<void(__thiscall*)(AITarget*)>(0x4180F0)(this); }

    /// <summary>
    /// Creates an AITarget object through game-code.
    /// </summary>
    /// <returns>A volatile pointer to a created <see cref="AITarget"/> object.</returns>
    static AITarget* Construct(ISimable* owner) {
      // A hacky way to create an object that we do not really have a control over
      AITarget* ret = reinterpret_cast<AITarget*>(::operator new(sizeof(AITarget), std::nothrow));
      if (!ret) return nullptr;

      reinterpret_cast<void(__thiscall*)(AITarget*, ISimable*)>(0x418000)(ret, owner);
      return ret;
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_AITARGET_H
