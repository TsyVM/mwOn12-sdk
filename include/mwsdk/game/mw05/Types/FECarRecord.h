// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/FECarRecord.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_FECARRECORD_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_FECARRECORD_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt {
  struct FECarRecord {
    std::uint32_t mHandle;
    std::uint32_t mFEKey;
    std::uint32_t mVehicleKey;
    std::uint32_t mFilterBits;
    std::uint8_t  mCustomization;
    std::uint8_t  mCareerHandle;
    std::uint16_t mPadd;

    inline std::int32_t GetCost() { return reinterpret_cast<std::int32_t(__thiscall*)(FECarRecord*)>(0x581730)(this); }
    inline const char*  GetManufacturerName() {
       return reinterpret_cast<const char*(__thiscall*)(FECarRecord*)>(0x581790)(this);
    }
    CarType GetType() { return reinterpret_cast<CarType(__thiscall*)(FECarRecord*)>(0x5816B0)(this); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_FECARRECORD_H
