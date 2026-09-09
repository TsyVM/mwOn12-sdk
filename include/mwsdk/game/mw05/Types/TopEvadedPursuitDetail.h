// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/TopEvadedPursuitDetail.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_TOPEVADEDPURSUITDETAIL_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_TOPEVADEDPURSUITDETAIL_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt {
  struct TopEvadedPursuitDetail {
    char          PursuitName[12];
    std::uint32_t CarFEKey;
    std::int32_t  Bounty;
    std::int32_t  Length;
    std::int32_t  NumCops;
    std::int32_t  NumCopsDamaged;
    std::int32_t  NumCopsDestroyed;
    std::int32_t  NumRoadblocksDodged;
    std::int32_t  NumSpikeStripsDodged;
    std::int32_t  TotalCostToState;
    std::int32_t  NumInfractions;
    std::int32_t  NumHelicopters;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_TOPEVADEDPURSUITDETAIL_H
