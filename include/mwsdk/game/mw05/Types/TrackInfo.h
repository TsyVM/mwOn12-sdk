// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/TrackInfo.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_TRACKINFO_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_TRACKINFO_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt {
  struct TrackInfo {
    char             Name[32];
    char             TrackDirectory[32];
    char             RegionName[8];
    char             RegionDirectory[32];
    std::int32_t     LocationNumber;
    char             LocationDirectory[16];
    eLocationName    LocationName;
    eDriftType       DriftType;
    bool             IsValid;
    bool             Point2Point;
    bool             ReverseVersionExists;
    bool             Unused;
    bool             IsPerformanceTuning;
    std::int16_t     TrackNumber;
    std::int16_t     SameAsTrackNumber;
    std::uint32_t    SunInfoNameHash;
    std::uint32_t    UsageFlags;
    std::uint32_t    TrackLength;
    float            TimeToBeatForwards_ms;
    float            TimeToBeatReverse_ms;
    std::int32_t     ScoreToBeatForwards_DriftOnly;
    std::int32_t     ScoreToBeatReverse_DriftOnly;
    bVector2         TrackMapCalibrationUpperLeft;
    float            TrackMapCalibrationMapWidthMetres;
    std::uint16_t    TrackMapCalibrationRotation;
    std::uint16_t    TrackMapStartLineAngle;
    std::uint16_t    TrackMapFinishLineAngle;
    float            TrackMapZoomFactor;
    eTrackDifficulty ForwardDifficulty;
    eTrackDifficulty ReverseDifficulty;
    std::int16_t     OverrideStartingRouteForAI[2][4];
    std::int16_t     NumSecondsBeforeShortcutsAllowed;
    std::int16_t     nDriftSecondsMin;
    std::int16_t     nDriftSecondsMax;
    std::int16_t     pad2[1];
    std::int8_t      MaxTrafficCars[4][2];
    std::int8_t      TrafficAllowedNearStartLine[2];
    char             CarRaceStartConfig;
    char             padbyte;
    float            TrafficMinInitialDistanceFromStartLine[2];
    float            TrafficMinInitialDistanceBetweenCars[2];
    float            TrafficOncomingFraction[4];
    Math::Vector2    TrackMapZoomTopLeft;
    float            TrackMapZoomWidth;
    bool             TrackMapStartZoomed;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_TRACKINFO_H
