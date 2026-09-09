// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/FECustomizationRecord.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_FECUSTOMIZATIONRECORD_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_FECUSTOMIZATIONRECORD_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include <algorithm>

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Physics.h"

namespace mwsdk::mw05::rt {
  struct FECustomizationRecord {
    std::int16_t               mInstalledParts[139];
    Physics::Upgrades::Package mInstalledPhysics;
    Physics::Tunings           mTunings[3];
    eCustomTuningType          mActiveTuning;
    std::int32_t               mPreset;
    std::uint8_t               mHandle;

    std::int16_t& GetInstalledPart(CarSlotId slotId) noexcept {
      return mInstalledParts[static_cast<std::size_t>(slotId)];
    }
    const std::int16_t& GetInstalledPart(CarSlotId slotId) const noexcept {
      return mInstalledParts[static_cast<std::size_t>(slotId)];
    }

    void WriteRideIntoRecord(const RideInfo* rideInfo) {
      reinterpret_cast<void(__thiscall*)(FECustomizationRecord*, const RideInfo*)>(0x56F2F0)(this, rideInfo);
    }

    void operator=(const FECustomizationRecord& rhs) {
      std::copy(std::cbegin(rhs.mInstalledParts), std::cend(rhs.mInstalledParts), std::begin(mInstalledParts));
      mInstalledPhysics = rhs.mInstalledPhysics;
      std::copy(std::cbegin(rhs.mTunings), std::cend(rhs.mTunings), std::begin(mTunings));
      mActiveTuning = rhs.mActiveTuning;
      mPreset       = rhs.mPreset;
      mHandle       = rhs.mHandle;
    }

    explicit FECustomizationRecord() {
      reinterpret_cast<FECustomizationRecord*(__thiscall*)(FECustomizationRecord*)>(0x581C20)(this);
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_FECUSTOMIZATIONRECORD_H
