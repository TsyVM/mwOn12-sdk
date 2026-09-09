// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/GRuntimeInstance.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_GRUNTIMEINSTANCE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_GRUNTIMEINSTANCE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib/Gen/gameplay.h"

namespace mwsdk::mw05::rt {
  struct GRuntimeInstance : Attrib::Gen::gameplay {
    struct ConnectedInstance {
      std::uint32_t     mIndexedKey;
      GRuntimeInstance* mInstance;
    };

    std::uint16_t      mFlags;
    std::uint16_t      mNumConnected;
    ConnectedInstance* mConnected;
    GRuntimeInstance*  mPrev;
    GRuntimeInstance*  mNext;

    inline bool GetDirection(UMath::Vector3& out) {
      return reinterpret_cast<bool(__thiscall*)(GRuntimeInstance*, UMath::Vector3&)>(0x5DACF0)(this, out);
    }
    inline bool GetPosition(UMath::Vector3& out) {
      return reinterpret_cast<bool(__thiscall*)(GRuntimeInstance*, UMath::Vector3&)>(0x5E2970)(this, out);
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_GRUNTIMEINSTANCE_H
