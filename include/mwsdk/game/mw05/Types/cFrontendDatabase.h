// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/cFrontendDatabase.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_CFRONTENDDATABASE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_CFRONTENDDATABASE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/cFinishedRaceStats.h"
#include "mwsdk/game/mw05/Types/FEKeyboardSettings.h"
#include "mwsdk/game/mw05/Types/OptionsSettings.h"
#include "mwsdk/game/mw05/Types/RaceSettings.h"
#include "mwsdk/game/mw05/Types/UserProfile.h"

namespace mwsdk::mw05::rt {
  struct cFrontEndDatabase {
    // GRace::Type
    enum class RaceType : std::uint32_t {
      Point2Point,  // Sprint
      Circuit,
      Drag,
      Knockout,
      Tollbooth,
      SpeedTrap,
      Checkpoint,
      CashGrab,
      Challenge,
      JumpToSpeedTrap,
      JumpToMilestone,
      None = UINT32_MAX
    };

    std::uint8_t           iNumPlayers;
    bool                   bComingFromBoot;
    bool                   bSavedProfileForMP;
    bool                   bProfileLoaded;
    bool                   bIsOptionsDirty;
    bool                   bAutoSaveOverwriteConfirmed;
    std::uint32_t          iDefaultStableHash;
    std::int8_t            PlayerJoyports[2];
    UserProfile*           CurrentUserProfiles[2];
    RaceType               RaceMode;
    RaceSettings           TheQuickRaceSettings[11];
    char*                  m_pCarStableBackup;
    char*                  m_pDBBackup;
    eFEGameModes           FEGameMode;
    eLoadSaveGame          LoadSaveGame;
    FEKeyboardSettings     mFEKeyboardSettings;
    std::int32_t           iCurPauseSubOptionType;
    std::int32_t           iCurPauseOptionType;
    FECustomizationRecord* SplitScreenCustomization;
    std::int8_t            SplitScreenCarType[256];
    cFinishedRaceStats     FinishedRaceStats;
    ePostRaceOptions       PostRaceOptionChosen;

    inline UserProfile*      GetUserProfile() { return CurrentUserProfiles[0]; }
    inline CareerSettings&   GetCareerSettings() { return this->GetUserProfile()->GetCareer(); }
    inline OptionsSettings&  GetOptionsSettings() { return this->GetUserProfile()->GetOptions(); }
    inline VideoSettings&    GetVideoSettings() { return this->GetOptionsSettings().GetVideoSettings(); }
    inline GameplaySettings& GetGameplaySettings() { return this->GetOptionsSettings().GetGameplaySettings(); }
    inline AudioSettings&    GetAudioSettings() { return this->GetOptionsSettings().GetAudioSettings(); }
    inline PlayerSettings&   GetPlayerSettings() { return this->GetOptionsSettings().GetPlayerSettings(); }

    static inline cFrontEndDatabase** g_mThis = reinterpret_cast<cFrontEndDatabase**>(0x91CF90);
    static cFrontEndDatabase*         Get() {
              if (!g_mThis) return nullptr;
      return *g_mThis;
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_CFRONTENDDATABASE_H
