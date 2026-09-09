// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/RaceParameters.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_RACEPARAMETERS_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_RACEPARAMETERS_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Timer.h"

namespace mwsdk::mw05::rt {
  struct RaceParameters {
    enum class RaceTypes : std::uint8_t {
      None,
      SingleRace,
      TimeTrial,
      LapKnockout,
      RaceRnockout,
      Tournament,
      CarShow,
      GetAway
    };

    std::int32_t          TrackNumber;
    eTrackDirection       TrackDirection;
    eTrafficDensity       TrafficDensity;
    float                 TrafficOncoming;
    bool                  AIDemoMode;
    bool                  ReplayDemoMode;
    RaceTypes             RaceType;
    std::int32_t          Point2Point;
    float                 RollingStartSpeed;
    std::int32_t          NumLapsInRace;
    std::int32_t          NumPlayerCars;
    std::int32_t          NumAICars;
    std::int32_t          NumOnlinePlayerCars;
    std::int32_t          NumOnlineAICars;
    std::int8_t           PlayerStartPosition[2];
    bool                  DamageEnabled;
    eHandlingMode         HandlingMode;
    std::int32_t          FinishLineNumber;
    bool                  bDragRaceFlag;
    bool                  bDriftRaceFlag;
    bool                  bBurnoutFlag;
    bool                  bShortRaceFlag;
    bool                  bOnlineRace;
    bool                  bCarShowFlag;
    bool                  bGamebreakerOn;
    std::int32_t          PlayerJoyports[2];
    std::int32_t          nMaxCops;
    eOpponentStrength     CopStrength;
    float                 DriftOpponentScoreMultiplier;
    eOpponentStrength     OpponentStrength;
    std::int32_t          Boost;
    float                 BoostScale[2];
    eAIDifficultyModifier AIDifficultyModifier;
    std::int32_t          PlayerDriverNumber[2];
    std::int32_t          NumDriverInfo;
    Timer                 TimeTrialTime;
    bool                  bCareerEventRace;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_RACEPARAMETERS_H
