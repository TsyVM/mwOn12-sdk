// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IVehicleCache.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IVEHICLECACHE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IVEHICLECACHE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IVehicleCache : UTL::COM::IUnknown {
    virtual ~IVehicleCache();
    virtual const char*         GetCacheName()                                                                   = 0;
    virtual eVehicleCacheResult OnQueryVehicleCache(const IVehicle* removethis, const IVehicleCache* whosasking) = 0;
    virtual void                OnRemovedVehicleCache(IVehicle* ivehicle)                                        = 0;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IVEHICLECACHE_H
