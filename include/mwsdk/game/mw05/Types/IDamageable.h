// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IDamageable.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IDAMAGEABLE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IDAMAGEABLE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IDamageable : UTL::COM::IUnknown {
    virtual ~IDamageable();
    virtual void                  SetInShock(float)    = 0;
    virtual void                  SetShockForce(float) = 0;
    virtual float                 InShock()            = 0;
    virtual void                  ResetDamage()        = 0;
    virtual float                 GetHealth()          = 0;
    virtual bool                  IsDestroyed()        = 0;
    virtual void                  Destroy()            = 0;
    virtual Sim::Collision::Info* GetZoneDamage()      = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x405000)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IDAMAGEABLE_H
