// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/CareerSettings.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_CAREERSETTINGS_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_CAREERSETTINGS_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/SMSMessage.h"

namespace mwsdk::mw05::rt {
  struct CareerSettings {
    std::uint32_t CurrentCar;
    std::uint32_t SpecialFlags;
    std::uint8_t  CurrentBin;
    std::int32_t  CurrentCash;
    std::int16_t  AdaptiveDifficulty;
    SMSMessage    SMSMessages[150];
    std::uint16_t SMSSortOrder;
    char          CaseFileName[16];

    inline const char*   GetCaseFileName() { return CaseFileName; }
    inline void          AwardCash(std::int32_t amount) { CurrentCash += amount; }
    inline std::int32_t  GetCash() { return CurrentCash; }
    inline std::uint8_t  GetCurrentBin() { return CurrentBin; }
    inline void          SetCurrentBin(std::uint8_t bin) { CurrentBin = bin; }
    inline std::uint32_t GetCurrentCar() { return CurrentCar; }
    inline void          SetCurrentCar(std::uint32_t handle) { CurrentCar = handle; }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_CAREERSETTINGS_H
