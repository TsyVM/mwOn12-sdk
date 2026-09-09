// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/GRaceStatus.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_GRACESTATUS_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_GRACESTATUS_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/eastl.hpp"

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/GRacerInfo.h"
#include "mwsdk/game/mw05/Types/IVehicleCache.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct GRaceStatus : UTL::COM::Object, IVehicleCache {
    enum class Context : std::uint32_t { QuickRace, Online, Career };
    enum class PlayMode : std::uint32_t { Roaming, Racing };

    GRacerInfo               mRacerInfo[16];
    std::int32_t             mRacerCount;
    bool                     mIsLoading;
    PlayMode                 mPlayMode;
    Context                  mRaceContext;
    GRaceParameters*         mRaceParms;
    GRaceBin*                mRaceBin;
    GTimer                   mRaceMasterTimer;
    bool                     mPlayerPursuitInCooldown;
    float                    mBonusTime;
    float                    mTaskTime;
    bool                     mSuddenDeathMode;
    bool                     mTimeExpiredMsgSent;
    bool                     mActivelyRacing;
    std::int32_t             mLastSecondTickSent;
    WorldModel*              mCheckpointModel;
    EmitterGroup*            mCheckpointEmitter;
    bool                     mQueueBinChange;
    std::uint32_t            mWarpWhenInFreeRoam;
    std::int32_t             mNumTollbooths;
    bool                     mScriptWaitingForLoad;
    bool                     mRefreshBinAfterRace;
    eastl::vector<GTrigger*> mCheckpoints;
    GTrigger*                mNextCheckpoint;
    float                    mLapTimes[10][16];
    float                    mCheckTimes[10][16][16];
    float                    mSegmentLengths[18];
    float                    fRaceLength;
    float                    fFirstLapLength;
    float                    fSubsequentLapLength;
    bool                     mCaluclatedAdaptiveGain;
    float                    fCatchUpIntegral;
    float                    fCatchUpDerivative;
    float                    fCatchUpAdaptiveBonus;
    float                    fAveragePercentComplete;
    std::int32_t             nCatchUpSkillEntries;
    float                    aCatchUpSkillData[11];
    std::int32_t             nCatchUpSpreadEntries;
    float                    aCatchUpSpreadData[11];
    std::int32_t             nSpeedTraps;
    GTrigger*                aSpeedTraps[16];
    bool                     mVehicleCacheLocked;
    bool                     bRaceRouteError;
    eTrafficDensity          mTrafficDensity;
    std::uint32_t            mTrafficPattern;
    bool                     mHasBeenWon;

    static void DisableBarriers() { reinterpret_cast<void(__cdecl*)()>(0x5DBF00)(); }

    static inline GRaceStatus** g_mThis = reinterpret_cast<GRaceStatus**>(0x91E000);
    static GRaceStatus*         Get() {
              if (!g_mThis) return nullptr;
      return *g_mThis;
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_GRACESTATUS_H
