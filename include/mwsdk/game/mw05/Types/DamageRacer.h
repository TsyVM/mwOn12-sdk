// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/DamageRacer.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_DAMAGERACER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_DAMAGERACER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/DamageVehicle.h"
#include "mwsdk/game/mw05/Types/ISpikeable.h"

namespace mwsdk::mw05::rt {
  struct DamageRacer : DamageVehicle, ISpikeable {
    ISuspension* mSuspension;
    union {
      float mBlowOutTimes[4];
      union {
        float mFrontLeftBlowOutTime;
        float mFrontRightBlowOutTime;
        float mRearRightBlowOutTime;
        float mRearLeftBlowOutTime;
      };
    };
    union {
      eTireDamage mDamage[4];
      union {
        eTireDamage mFrontLeftDamage;
        eTireDamage mFrontRightDamage;
        eTireDamage mRearRightDamage;
        eTireDamage mRearLeftDamage;
      };
    };

    virtual ~DamageRacer();

    virtual bool                  CanDamageVisuals() override;
    virtual void                  OnTaskSimulate(float deltaTime) override;
    virtual void                  OnBehaviorChange(const UCrc32& mechanic) override;
    virtual void                  ResetDamage() override;
    virtual Sim::Collision::Info* GetZoneDamage() override;
    virtual void                  OnCollision(const Sim::Collision::Info& cinfo) override;
    virtual bool                  IsLightDamaged(VehicleFX::LightID idx) override;
    virtual eTireDamage           GetTireDamage(eTireIdx idx) override;
    virtual std::int32_t          GetNumBlowouts() override;
    virtual void                  Puncture(eTireIdx idx) override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_DAMAGERACER_H
