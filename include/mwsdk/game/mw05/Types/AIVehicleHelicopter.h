// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/AIVehicleHelicopter.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_AIVEHICLEHELICOPTER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_AIVEHICLEHELICOPTER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/AIVehiclePursuit.h"
#include "mwsdk/game/mw05/Types/IAIHelicopter.h"
#include "mwsdk/game/mw05/Types/HeliSheetCoordinate.h"

namespace mwsdk::mw05::rt {
  struct AIVehicleHelicopter : AIVehiclePursuit, IAIHelicopter {
    UMath::Vector3      mDestinationVelocity;
    UMath::Vector3      mLookAtPosition;
    UMath::Vector3      mLastPlaceHeliSawPerp;
    float               mHeight;
    bool                mStrafeToDest;
    bool                mPerpHiddenFromMe;
    float               mHeliFuelTimeRemaining;
    float               mShadowScale;
    float               mDustStormIntensity;
    HeliSheetCoordinate mHeliSheetCoord;
    HeliSheetCoordinate mSecondaryHeliSheetCoord;
    HeliSheetCoordinate mThirdHeliSheetCoord;
    ISimpleChopper*     mISimpleChopper;

    virtual ~AIVehicleHelicopter();
    virtual const HeliSheetCoordinate& GetHeliSheetCoord();
    virtual void           Update(float deltaTime) override;
    virtual void           OnDriving(float deltaTime) override;
    virtual bool           CanSeeTarget(AITarget* target) override;
    virtual float          GetDesiredHeightOverDest() override;
    virtual void           SetDesiredHeightOverDest(float height) override;
    virtual void           SetLookAtPosition(UMath::Vector3 pos) override;
    virtual UMath::Vector3 GetLookAtPosition() override;
    virtual void           SetDestinationVelocity(const UMath::Vector3& velocity) override;
    virtual void           SteerToNav(WRoadNav* road_nav, float height, float speed, bool bStopAtDest) override;
    virtual bool           StartPathToPoint(UMath::Vector3& point) override;
    virtual bool           StrafeToDestIsSet() override;
    virtual void           SetStrafeToDest(bool strafe) override;
    virtual bool           FilterHeliAltitude(UMath::Vector3& point) override;
    virtual void           RestrictPointToRoadNet(UMath::Vector3& seekPosition) override;
    virtual void           SetFuelFull() override;
    virtual float          GetFuelTimeRemaining() override;
    virtual void           SetShadowScale(float scale) override;
    virtual float          GetShadowScale() override;
    virtual void           SetDustStormIntensity(float intensity) override;
    virtual float          GetDustStormIntensity() override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_AIVEHICLEHELICOPTER_H
