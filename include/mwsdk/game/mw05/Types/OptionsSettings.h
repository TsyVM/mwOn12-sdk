// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/OptionsSettings.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_OPTIONSSETTINGS_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_OPTIONSSETTINGS_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/AudioSettings.h"
#include "mwsdk/game/mw05/Types/GameplaySettings.h"
#include "mwsdk/game/mw05/Types/PlayerSettings.h"
#include "mwsdk/game/mw05/Types/VideoSettings.h"

namespace mwsdk::mw05::rt {
  struct OptionsSettings {
    eOptionsCategory CurrentCategory;
    VideoSettings    TheVideoSettings;
    GameplaySettings TheGameplaySettings;
    AudioSettings    TheAudioSettings;
    PlayerSettings   ThePlayerSettings[2];

    inline VideoSettings&    GetVideoSettings() { return TheVideoSettings; }
    inline GameplaySettings& GetGameplaySettings() { return TheGameplaySettings; }
    inline AudioSettings&    GetAudioSettings() { return TheAudioSettings; }
    inline PlayerSettings&   GetPlayerSettings(std::size_t index = 0) { return ThePlayerSettings[index]; }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_OPTIONSSETTINGS_H
