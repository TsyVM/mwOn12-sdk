// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/DamageCopCar.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_DAMAGECOPCAR_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_DAMAGECOPCAR_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/DamageVehicle.h"

namespace mwsdk::mw05::rt {
  struct DamageCopCar : DamageVehicle {
    float       mFlippedOver;
    HSIMTASK__* mThinkTas;

    virtual ~DamageCopCar();

    virtual bool OnTask(HSIMTASK__* hTask, float deltaTime) override;
    virtual void OnImpact(const UMath::Vector3&, const UMath::Vector3&, float, float, const SimSurface&,
                          ISimable*) override;
    virtual void ResetDamage() override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_DAMAGECOPCAR_H
