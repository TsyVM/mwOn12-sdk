// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/GRacerInfo.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_GRACERINFO_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_GRACERINFO_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/GTimer.h"

namespace mwsdk::mw05::rt {
  struct GRacerInfo {
    HSIMABLE__*    mhSimable;
    GCharacter*    mGameCharacter;
    const char*    mName;
    std::int32_t   mIndex;
    std::int32_t   mRanking;
    std::int32_t   mAiRanking;
    float          mPctRaceComplete;
    bool           mKnockedOut;
    bool           mTotalled;
    bool           mEngineBlown;
    bool           mBusted;
    bool           mChallengeComplete;
    bool           mFinishedRacing;
    bool           mCameraDetached;
    float          mPctLapComplete;
    std::int32_t   mLapsCompleted;
    std::int32_t   mCheckpointsHitThisLap;
    std::int32_t   mTollboothsCrossed;
    float          mTimeRemainingToBooth[16];
    std::int32_t   mSpeedTrapsCrossed;
    float          mSpeedTrapSpeed[16];
    std::int32_t   mSpeedTrapPosition[16];
    float          mDistToNextCheckpoint;
    float          mDistanceDriven;
    float          mTopSpeed;
    float          mFinishingSpeed;
    float          mPoundsNOSUsed;
    float          mTimeCrossedLastCheck;
    float          mTotalUpdateTime;
    std::int32_t   mNumPerfectShifts;
    std::int32_t   mNumTrafficCarsHit;
    float          mSpeedBreakerTime;
    float          mPointTotal;
    float          mZeroToSixtyTime;
    float          mQuarterMileTime;
    float          mSplitTimes[4];
    std::int32_t   mSplitRankings[4];
    GTimer         mRaceTimer;
    GTimer         mLapTimer;
    GTimer         mCheckTimer;
    UMath::Vector3 mSavedPosition;
    float          mSavedHeatLevel;
    UMath::Vector3 mSavedDirection;
    float          mSavedSpeed;
    bool           mDNF;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_GRACERINFO_H
