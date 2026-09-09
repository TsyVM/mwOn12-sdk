// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IPlayer.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IPLAYER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IPLAYER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IPlayer : UTL::COM::IUnknown {
    virtual ISimable*             GetSimable()                                   = 0;
    virtual bool                  IsLocal()                                      = 0;
    virtual const UMath::Vector3& GetPosition()                                  = 0;
    virtual bool                  SetPosition(const UMath::Vector3& newPosition) = 0;
    virtual PlayerSettings*       GetSettings()                                  = 0;
    virtual void                  SetSettings(std::int32_t)                      = 0;
    virtual std::int32_t          GetSettingsIndex()                             = 0;
    virtual IHud*                 GetHud()                                       = 0;
    virtual void                  SetHud(ePlayerHudType newHudType)              = 0;
    virtual void                  SetRenderPort(int32_t)                         = 0;
    virtual std::int32_t          GetRenderPort()                                = 0;
    virtual void                  SetControllerPort(std::int32_t)                = 0;
    virtual std::int32_t          GetControllerPort()                            = 0;
    virtual IFeedback*            GetFFB()                                       = 0;
    virtual ISteeringWheel*       GetSteeringDevice()                            = 0;
    virtual bool                  InGameBreaker()                                = 0;
    virtual bool                  CanRechargeNOS()                               = 0;
    virtual void                  ResetGameBreaker(bool fill)                    = 0;
    virtual void                  ChargeGameBreaker(float amount)                = 0;
    virtual bool                  ToggleGameBreaker()                            = 0;

    static inline UTL::ListableSet<IPlayer, 8, ePlayerList, 3>* g_mLists =
        reinterpret_cast<UTL::ListableSet<IPlayer, 8, ePlayerList, 3>*>(0x92D87C);

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x6200D0)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IPLAYER_H
