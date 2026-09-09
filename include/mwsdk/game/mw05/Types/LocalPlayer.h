// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/LocalPlayer.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_LOCALPLAYER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_LOCALPLAYER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/IPlayer.h"
#include "mwsdk/game/mw05/Types/Sim.h"

namespace mwsdk::mw05::rt {
  struct LocalPlayer : Sim::Entity, IPlayer, Sim::Collision::IListener {
    IFeedback*      mFFB;
    ISteeringWheel* mWheelDevice;
    std::int32_t    mRenderPort;
    std::int32_t    mControllerPort;
    std::int32_t    mSettingIndex;
    const char*     mName;
    std::uint32_t   mNeighbourhoodHash;
    IHud*           mHud;
    HSIMTASK__*     mHudTask;
    Sim::IActivity* mSpeech;
    bool            mInGameBreaker;
    float           mGameBreakerCharge;
    HACTIVITY__*    mLastPursuit;

    virtual ~LocalPlayer();
    virtual ISimable*             GetSimable() override;
    virtual bool                  IsLocal() override;
    virtual const UMath::Vector3& GetPosition() override;
    virtual bool                  SetPosition(const UMath::Vector3& newPosition) override;
    virtual PlayerSettings*       GetSettings() override;
    virtual void                  SetSettings(std::int32_t) override;
    virtual std::int32_t          GetSettingsIndex() override;
    virtual IHud*                 GetHud() override;
    virtual void                  SetHud(ePlayerHudType newHudType) override;
    virtual void                  SetRenderPort(int32_t) override;
    virtual std::int32_t          GetRenderPort() override;
    virtual void                  SetControllerPort(std::int32_t) override;
    virtual std::int32_t          GetControllerPort() override;
    virtual IFeedback*            GetFFB() override;
    virtual ISteeringWheel*       GetSteeringDevice() override;
    virtual bool                  InGameBreaker() override;
    virtual bool                  CanRechargeNOS() override;
    virtual void                  ResetGameBreaker(bool fill) override;
    virtual void                  ChargeGameBreaker(float amount) override;
    virtual bool                  ToggleGameBreaker() override;
    virtual void                  OnAttached(IAttachable* pOther) override;
    virtual void                  OnDetached(IAttachable* pOther) override;
    virtual void                  OnCollision(const Sim::Collision::Info& cInfo) override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_LOCALPLAYER_H
