// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IHumanAI.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IHUMANAI_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IHUMANAI_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IHumanAI : UTL::COM::IUnknown {
    virtual ~IHumanAI();
    virtual void                  ChangeDragLanes(bool left)                                   = 0;
    virtual bool                  IsPlayerSteering()                                           = 0;
    virtual bool                  GetAiControl()                                               = 0;
    virtual void                  SetAiControl(bool ai_control)                                = 0;
    virtual void                  SetWorldMoment(const UMath::Vector3& position, float radius) = 0;
    virtual const UMath::Vector3& GetWorldMomentPosition()                                     = 0;
    virtual float                 GetWorldMomentRadius()                                       = 0;
    virtual void                  ClearWorldMoment()                                           = 0;
    virtual bool                  IsFacingWrongWay()                                           = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x403770)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IHUMANAI_H
