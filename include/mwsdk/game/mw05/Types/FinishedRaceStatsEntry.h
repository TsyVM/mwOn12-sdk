// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/FinishedRaceStatsEntry.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_FINISHEDRACESTATSENTRY_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_FINISHEDRACESTATSENTRY_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Timer.h"

namespace mwsdk::mw05::rt {
  struct FinishedRaceStatsEntry {
    std::int32_t FinishPosition;
    std::int32_t DriverNumber;
    std::int32_t FinishReason;
    Timer        RaceTime;
    Timer        BestLapTime;
    std::int32_t HandlingMode;
    float        TopSpeed;
    float        AverageSpeed;
    Timer        LapTimes[11];
    Timer        LapRunningTimes[11];
    std::int32_t NumLapsCompleted;
    float        NumLapsCompletedExact;
    std::int32_t PositionPerLap[8];
    std::int32_t NumLapsLead;
    float        Odometer;
    Timer        ZeroToSixtyTime;
    Timer        QuarterMileTime;
    float        QuarterMileSpeed;
    float        LongestJump;
    float        LongestPowerSlide;
    std::int32_t WasRecordBreaker;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_FINISHEDRACESTATSENTRY_H
