// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/UserProfile.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_USERPROFILE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_USERPROFILE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/CareerSettings.h"
#include "mwsdk/game/mw05/Types/FEPlayerCarDB.h"
#include "mwsdk/game/mw05/Types/HighScoresDatabase.h"
#include "mwsdk/game/mw05/Types/JukeboxEntry.h"
#include "mwsdk/game/mw05/Types/OptionsSettings.h"

namespace mwsdk::mw05::rt {
  struct UserProfile {
    char               m_aProfileName[32];
    bool               m_bNamed;
    OptionsSettings    mTheOptionsSettings;
    CareerSettings     mTheCareerSettings;
    JukeboxEntry       mPlaylist[30];
    FEPlayerCarDB      mPlayersCarStable;
    bool               mCareerModeHasBeenCompletedAtLeastOnce;
    HighScoresDatabase mHighScores;

    inline OptionsSettings&    GetOptions() { return mTheOptionsSettings; }
    inline CareerSettings&     GetCareer() { return mTheCareerSettings; }
    inline HighScoresDatabase& GetHighScores() { return mHighScores; }
    inline void                SetProfileNamed(bool isNamed) { m_bNamed = isNamed; }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_USERPROFILE_H
