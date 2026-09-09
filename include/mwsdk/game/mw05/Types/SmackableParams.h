// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/SmackableParams.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_SMACKABLEPARAMS_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_SMACKABLEPARAMS_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Sim.h"

namespace mwsdk::mw05::rt {
  struct SmackableParams : Sim::Param {
    UMath::Matrix4 fMatrix;
    bool           fVirginSpawn;
    IModel*        fScenery;
    bool           fSimplePhysics;

    explicit SmackableParams(const UMath::Matrix4& matrix, bool isVirginSpawn = true, IModel* scenery = nullptr,
                             bool isSimplePhysics = true) :
        Sim::Param(0x0A6B47FAC),
        fMatrix(matrix),
        fVirginSpawn(isVirginSpawn),
        fScenery(scenery),
        fSimplePhysics(isSimplePhysics) {
      AddTypeName(this);
    }

    static void AddTypeName(SmackableParams* to) { reinterpret_cast<void(__cdecl*)(SmackableParams*)>(0x620FF0)(to); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_SMACKABLEPARAMS_H
