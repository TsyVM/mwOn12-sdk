// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Layouts/simsurfacelayout.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_SIMSURFACELAYOUT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_SIMSURFACELAYOUT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib/Private.h"
#include "mwsdk/game/mw05/Types/Attrib/RefSpec.h"

namespace mwsdk::mw05::rt::Attrib::Layouts {
  struct simsurfacelayout {
    struct RoadNoiseRecord {
      float Frequency;
      float Amplitude;
      float MinSpeed;
      float MaxSpeed;
    };
    struct TireEffectRecord {
      RefSpec mEmitter;
      float   mMinSpeed;
      float   mMaxSpeed;
    };

    Private          _Array_TireDriveEffects;
    TireEffectRecord TireDriveEffects[3];
    Private          _Array_TireSlideEffects;
    TireEffectRecord TireSlideEffects[3];
    Private          _Array_TireSlipEffects;
    TireEffectRecord TireSlipEffects[3];
    RoadNoiseRecord  RenderNoise;
    const char*      CollectionName;
    float            GROUND_FRICTION;
    float            ROLLING_RESISTANCE;
    float            WORLD_FRICTION;
    float            DRIVE_GRIP;
    float            LATERAL_GRIP;
    float            STICK;
    std::uint16_t    WheelEffectFrequency;
    std::uint8_t     WheelEffectIntensity;
  };
}  // namespace mwsdk::mw05::rt::Attrib::Layouts

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_SIMSURFACELAYOUT_H
