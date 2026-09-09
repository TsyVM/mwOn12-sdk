// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/DebugVehicleSelection.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_DEBUGVEHICLESELECTION_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_DEBUGVEHICLESELECTION_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/IVehicleCache.h"

namespace mwsdk::mw05::rt {
  struct DebugVehicleSelection : IVehicleCache {
    unsigned char _pad1[0x10];
    std::int32_t  mSelectionIndex;
    unsigned char _pad2[0x4];
    const char**  mSelectionList;
    unsigned char _pad3[0x8];
    const char*   mCollisionObject;
    const char*   mCollisionSurface;

    virtual ~DebugVehicleSelection();

    void ChangePlayerVehicle(std::int32_t selectionIndex) {
      mSelectionIndex        = selectionIndex;
      *g_ChangePlayerVehicle = true;
    }

    static inline DebugVehicleSelection** g_mThis               = reinterpret_cast<DebugVehicleSelection**>(0x9B08F8);
    static inline bool*                   g_ChangePlayerVehicle = reinterpret_cast<bool*>(0x9B08FD);

    static DebugVehicleSelection* Get() {
      if (!g_mThis) return nullptr;
      return *g_mThis;
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_DEBUGVEHICLESELECTION_H
