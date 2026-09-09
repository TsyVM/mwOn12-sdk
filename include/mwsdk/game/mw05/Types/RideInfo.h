// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/RideInfo.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_RIDEINFO_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_RIDEINFO_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt {
  struct RideInfo {
    CarType        Type;
    char           InstanceIndex;
    char           HasDash;
    char           CanBeVertexDamaged;
    char           SkinType;
    CARPART_LOD    mMinLodLevel;
    CARPART_LOD    mMaxLodLevel;
    CARPART_LOD    mMinFELodLevel;
    CARPART_LOD    mMaxFELodLevel;
    CARPART_LOD    mMaxLicenseLodLevel;
    CARPART_LOD    mMinTrafficDiffuseLodLevel;
    CARPART_LOD    mMinShadowLodLevel;
    CARPART_LOD    mMaxShadowLodLevel;
    CARPART_LOD    mMaxTireLodLevel;
    CARPART_LOD    mMaxBrakeLodLevel;
    CARPART_LOD    mMaxSpoilerLodLevel;
    CARPART_LOD    mMaxRoofScoopLodLevel;
    CARPART_LOD    mMinReflectionLodLevel;
    std::uint32_t  mCompositeSkinHash;
    std::uint32_t  mCompositeWheelHash;
    std::uint32_t  mCompositeSpinnerHash;
    CarPart*       mPartsTable[139];
    char           mPartsEnabled[139];
    CarPart*       PreviewPart;
    std::int32_t   mMyCarLoaderHandle;
    CarRenderUsage mMyCarRenderUsage;
    std::uint8_t   mSpecialLODBehavior;

    inline CarPart* GetPart(CarSlotId slotId) {
      return reinterpret_cast<CarPart*(__thiscall*)(RideInfo*, CarSlotId)>(0x739C70)(this, slotId);
    }

    inline void SetRandomPart(CarSlotId slotId, eCareerUpgradeLevels upgradeLevel) {
      reinterpret_cast<void(__thiscall*)(RideInfo*, CarSlotId, eCareerUpgradeLevels)>(0x7596E0)(this, slotId,
                                                                                                upgradeLevel);
    }
    inline void SetUpgradePart(CarSlotId slotId, eCareerUpgradeLevels upgradeLevel) {
      reinterpret_cast<void(__thiscall*)(RideInfo*, CarSlotId, eCareerUpgradeLevels)>(0x759470)(this, slotId,
                                                                                                upgradeLevel);
    }

    inline void SetRandomPaint() { reinterpret_cast<void(__thiscall*)(RideInfo*)>(0x759800)(this); }
    inline void SetRandomParts() { reinterpret_cast<void(__thiscall*)(RideInfo*)>(0x75B220)(this); }
    inline void SetStockParts() { reinterpret_cast<void(__thiscall*)(RideInfo*)>(0x7594A0)(this); }

    inline void Init(CarType type, CarRenderUsage renderUsage, std::int32_t _unk1 = 0, std::int32_t _unk2 = 0) {
      reinterpret_cast<void(__thiscall*)(RideInfo*, CarType, CarRenderUsage, std::int32_t, std::int32_t)>(0x739A70)(
          this, type, renderUsage, _unk1, _unk2);
    }

    explicit RideInfo(CarType type, CarRenderUsage renderUsage) { Init(type, renderUsage); }
    explicit RideInfo(CarType type) : RideInfo(type, CarRenderUsage::Player) {}
    explicit RideInfo() : RideInfo(CarType::NONE) {}
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_RIDEINFO_H
