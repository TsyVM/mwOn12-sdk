// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/HighScoresDatabase.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_HIGHSCORESDATABASE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_HIGHSCORESDATABASE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/CareerPursuitScores.h"
#include "mwsdk/game/mw05/Types/CostToStateScores.h"
#include "mwsdk/game/mw05/Types/PursuitScore.h"
#include "mwsdk/game/mw05/Types/RaceTypeHighScores.h"
#include "mwsdk/game/mw05/Types/TopEvadedPursuitDetail.h"
#include "mwsdk/game/mw05/Types/TrackHighScore.h"

namespace mwsdk::mw05::rt {
  struct HighScoresDatabase {
    TrackHighScore         TrackHighScoreTable[320];
    float                  TotalOdometer;
    std::int32_t           TotalStarts;
    std::int32_t           TotalWins;
    std::int32_t           TotalLosses;
    RaceTypeHighScores     RaceTypeScores[2];
    RaceTypeHighScores     SplitScreenScores;
    std::int32_t           TotalDragTotalled;
    TopEvadedPursuitDetail TopEvadedPursuitScores[5];
    CareerPursuitScores    CareerPursuitDetails;
    PursuitScore           BestPursuitRankings[10];
    CostToStateScores      CostToStateDetails;
    std::uint32_t          PreviouslyPursuedCarFEKey;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_HIGHSCORESDATABASE_H
