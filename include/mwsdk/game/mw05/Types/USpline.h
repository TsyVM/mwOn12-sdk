// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/USpline.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_USPLINE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_USPLINE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/eastl.hpp"

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt {
  struct USpline {
    enum class SplineType : std::uint32_t { Bezier, CatMulRom, Invalid };

    UMath::Matrix4              fSplineMat;
    UMath::Vector4              fLookAt[2];
    SplineType                  fSplineType;
    eastl::list<UMath::Vector4> fSplinePtList;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_USPLINE_H
