// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Gen/pvehicle.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_GEN_PVEHICLE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_GEN_PVEHICLE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib/Instance.h"

namespace mwsdk::mw05::rt::Attrib::Gen {
  struct pvehicle : Instance {
    static pvehicle TryGetInstance(std::uint32_t key) {
      pvehicle instance;
      reinterpret_cast<pvehicle*(__thiscall*)(pvehicle&, std::uint32_t, bool, bool)>(0x4E4EA0)(instance, key, false,
                                                                                               false);

      return instance;
    }
    static pvehicle TryGetInstance(const char* name) { return TryGetInstance(StringToKey(name)); }
  };
}  // namespace mwsdk::mw05::rt::Attrib::Gen

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_GEN_PVEHICLE_H
