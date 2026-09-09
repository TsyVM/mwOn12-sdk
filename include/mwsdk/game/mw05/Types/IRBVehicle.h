// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IRBVehicle.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IRBVEHICLE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IRBVEHICLE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IRBVehicle : UTL::COM::IUnknown {
    virtual ~IRBVehicle();
    virtual void             SetCollisionMass(float mass)                                         = 0;
    virtual void             SetCollisionCOG(const UMath::Vector3& newCenterOfGravity)            = 0;
    virtual void             EnableObjectCollisions(bool isEnabled)                               = 0;
    virtual void             SetInvulnerability(eInvulnerability type, float durationInSeconds)   = 0;
    virtual eInvulnerability GetInvulnerability()                                                 = 0;
    virtual void             SetPlayerReactions(const Attrib::Gen::collisionreactions& reactions) = 0;
    virtual const Attrib::Gen::collisionreactions& GetPlayerReactions()                           = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x4039E0)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IRBVEHICLE_H
