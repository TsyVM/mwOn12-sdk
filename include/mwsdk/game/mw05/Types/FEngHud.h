// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/FEngHud.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_FENGHUD_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_FENGHUD_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/ActionQueue.h"
#include "mwsdk/game/mw05/Types/IHud.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct FEngHud : UTL::COM::Object, IHud {
    std::uint64_t  CurrentHudFeatures;
    ePlayerHudType mPlayerHudType;
    const char*    pPackageName;
    IPlayer*       pPlayer;
    std::int32_t   PlayerNumber;
    ActionQueue    mActionQ;
    bool           mInPursuit;
    bool           mHasTurbo;
    HudElement*    pSpeedometer;
    HudElement*    pTachometer;
    HudElement*    pTachometerDrag;
    HudElement*    pShiftUpdater;
    HudElement*    pCostToState;
    HudElement*    pReputation;
    HudElement*    pHeatMeter;
    HudElement*    pTurboMeter;
    HudElement*    pEngineTemp;
    HudElement*    pNitrous;
    HudElement*    pSpeedBreakerMeter;
    HudElement*    pRaceOverMessage;
    HudElement*    pGenericMessage;
    HudElement*    pAutoSaveIcon;
    HudElement*    pRaceInformation;
    HudElement*    pLeaderBoard;
    HudElement*    pPursuitBoard;
    HudElement*    pMilestoneBoard;
    HudElement*    pBustedMeter;
    HudElement*    pTimeExtension;
    HudElement*    pWrongWIndi;
    HudElement*    pOnlineSupport;
    HudElement*    p321Go;
    HudElement*    pRadarDetector;
    HudElement*    pMinimap;
    HudElement*    pGetAwayMeter;
    HudElement*    pMenuZoneTrigger;
    HudElement*    pInfractions;
    bool           mCurrentWidescreenSetting;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_FENGHUD_H
