// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/ISimpleBody.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ISIMPLEBODY_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ISIMPLEBODY_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct ISimpleBody : UTL::COM::IUnknown {
    enum class BodyFlags : std::uint16_t {
      None                   = 0,
      SkipSRBCollisions      = 1 << 0,
      SkipRBCollisions       = 1 << 1,
      SkipCollisions         = 3,
      ApplyBasicPhysics      = 1 << 2,
      LinearMotion           = 1 << 3,
      OBBCollideWith_RB      = 1 << 6,
      OBBCollideWith_SRB     = 1 << 7,
      CanHitTriggers         = 1 << 8,
      AngularMotion          = 1 << 9,
      SkipSameTypeCollisions = 1 << 14,
      SOBBCollideWith_RB     = 1 << 15,
    };

    virtual ~ISimpleBody();
    virtual void                   ModifyFlags(BodyFlags remove, BodyFlags add) = 0;
    virtual bool                   CanCollideWithSRB()                          = 0;
    virtual bool                   CanCollideWithRB()                           = 0;
    virtual bool                   CanHitTrigger()                              = 0;
    virtual const SimCollisionMap* GetCollisionMap() const                      = 0;
    virtual SimCollisionMap*       GetCollisionMap()                            = 0;
  };

#if 1  // portable flag operators
  DEFINE_ENUM_FLAG_OPERATORS(ISimpleBody::BodyFlags)
#endif
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ISIMPLEBODY_H
