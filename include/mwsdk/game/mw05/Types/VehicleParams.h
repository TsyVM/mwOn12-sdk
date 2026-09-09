// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/VehicleParams.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_VEHICLEPARAMS_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_VEHICLEPARAMS_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Sim.h"

namespace mwsdk::mw05::rt {
  struct VehicleParams : Sim::Param {
    DriverClass                 mDriverClass;
    std::uint32_t               mVehicleKey;
    const UMath::Vector3*       mDirection;
    const UMath::Vector3*       mPosition;
    FECustomizationRecord*      mCustomization;
    IVehicleCache*              mVehicleCache;
    Physics::Info::Performance* mPerformanceMatch;
    eVehicleParamFlags          mFlags;

    explicit VehicleParams(DriverClass driverClass, std::uint32_t vehicleKey, const UMath::Vector3& direction,
                           const UMath::Vector3& position, FECustomizationRecord* pFECR = nullptr,
                           eVehicleParamFlags flags = eVehicleParamFlags::SnapToGround |
                                                      eVehicleParamFlags::CalcPerformance,
                           IVehicleCache*              pVehicleCache     = nullptr,
                           Physics::Info::Performance* pPerformanceMatch = nullptr) :
        Sim::Param(0x0A6B47FAC),
        mDriverClass(driverClass),
        mVehicleKey(vehicleKey),
        mDirection(&direction),
        mPosition(&position),
        mCustomization(pFECR),
        mVehicleCache(pVehicleCache),
        mPerformanceMatch(pPerformanceMatch),
        mFlags(flags) {
      AddTypeName(this);
    }

    static void AddTypeName(VehicleParams* to) { reinterpret_cast<void(__cdecl*)(VehicleParams*)>(0x4040F0)(to); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_VEHICLEPARAMS_H
