// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/FEPlayerCarDB.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_FEPLAYERCARDB_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_FEPLAYERCARDB_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/FECareerRecord.h"
#include "mwsdk/game/mw05/Types/FECarRecord.h"
#include "mwsdk/game/mw05/Types/FECustomizationRecord.h"
#include "mwsdk/game/mw05/Types/FEInfractionsData.h"

namespace mwsdk::mw05::rt {
  struct FEPlayerCarDB {
    FECarRecord           CarTable[200];
    FECustomizationRecord Customizations[75];
    FECareerRecord        CareerRecords[25];
    std::uint32_t         SoldHistoryBounty;
    std::uint16_t         SoldHistoryNumEvadedPursuits;
    std::uint16_t         SoldHistoryNumBustedPursuits;
    FEInfractionsData     SoldHistoryUnservedInfractions;
    FEInfractionsData     SoldHistoryServedInfractions;

    inline void AwardBonusCars() { reinterpret_cast<void(__thiscall*)(FEPlayerCarDB*)>(0x56F0C0)(this); }
    inline void AwardRivalCar(Attrib::StringKey carKey) {
      reinterpret_cast<void(__thiscall*)(FEPlayerCarDB*, Attrib::StringKey)>(0x5A41E0)(this, carKey);
    }
    inline FECarRecord* GetCarByIndex(std::uint32_t index) {
      return reinterpret_cast<FECarRecord*(__thiscall*)(FEPlayerCarDB*, std::uint32_t)>(0x56ECF0)(this, index);
    }
    inline FECarRecord* GetCarRecordByHandle(std::uint32_t handle) {
      return reinterpret_cast<FECarRecord*(__thiscall*)(FEPlayerCarDB*, std::uint32_t)>(0x56ECC0)(this, handle);
    }
    inline FECareerRecord* GetCareerRecordByHandle(std::uint8_t handle) {
      return reinterpret_cast<FECareerRecord*(__thiscall*)(FEPlayerCarDB*, std::uint8_t)>(0x56F120)(this, handle);
    }
    inline FECustomizationRecord* GetCustomizationRecordByHandle(std::uint8_t handle) {
      return reinterpret_cast<FECustomizationRecord*(__thiscall*)(FEPlayerCarDB*, std::uint8_t)>(0x56F100)(this,
                                                                                                           handle);
    }
    inline std::size_t GetNumCars() {
      return reinterpret_cast<std::size_t(__thiscall*)(FEPlayerCarDB*)>(0x5812C0)(this);
    }
    inline std::size_t GetNumCareerCars() {
      return reinterpret_cast<std::size_t(__thiscall*)(FEPlayerCarDB*)>(0x5810E0)(this);
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_FEPLAYERCARDB_H
