// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/ResetCar.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_RESETCAR_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_RESETCAR_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/BehaviorParams.h"
#include "mwsdk/game/mw05/Types/IRBVehicle.h"
#include "mwsdk/game/mw05/Types/IResetable.h"
#include "mwsdk/game/mw05/Types/VehicleBehavior.h"
#include "mwsdk/game/mw05/Types/WRoadNav.h"

namespace mwsdk::mw05::rt {
  struct ResetCar : VehicleBehavior, IResetable {
    enum class Flags : std::uint32_t { None, FindRoad };
    struct ResetCookie {
      UMath::Vector3 position;
      Flags          flags;
      UMath::Vector3 direction;
      float          time;
    };

    float                                 mFlippedOver;
    WRoadNav::CookieTrail<ResetCookie, 4> mCookies;
    ICollisionBody*                       mCollisionBody;
    ISuspension*                          mSuspension;
    IRBVehicle*                           mVehicleBody;
    HSIMTASK__*                           mCheckTask;

    virtual ~ResetCar();
    virtual void Reset() override;
    virtual void OnBehaviorChange(const UCrc32& mechanic) override;
    virtual bool HasResetPosition() override;
    virtual bool ResetVehicle(bool findRoad) override;
    virtual void SetResetPosition(const UMath::Vector3& position, const UMath::Vector3& direction) override;
    virtual void ClearResetPosition() override;

    static IResetable* Construct(const BehaviorParams& params) {
      IResetable* p = reinterpret_cast<IResetable*(__cdecl*)(BehaviorParams)>(0x689820)(params);
      return static_cast<ResetCar*>(p);
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_RESETCAR_H
