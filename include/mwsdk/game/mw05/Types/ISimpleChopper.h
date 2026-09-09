// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/ISimpleChopper.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ISIMPLECHOPPER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ISIMPLECHOPPER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct ISimpleChopper : UTL::COM::IUnknown {
    virtual ~ISimpleChopper();
    virtual void SetDesiredVelocity(const UMath::Vector3& velocity)   = 0;
    virtual void GetDesiredVelocity(UMath::Vector3& out)              = 0;
    virtual void MaxDeceleration(bool limit)                          = 0;
    virtual void SetDesiredFacingVector(const UMath::Vector3& vector) = 0;
    virtual void GetDesiredFacingVector(UMath::Vector3& out)          = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x404050)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ISIMPLECHOPPER_H
