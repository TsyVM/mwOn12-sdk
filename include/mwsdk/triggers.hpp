// SPDX-License-Identifier: MIT
//
// mwsdk/triggers.hpp
// ------------------
// The world's gameplay geometry: trigger regions (the polygon zones that fire
// gates, speed-traps, checkpoints and events) and position markers (named 3-D
// anchor points). Both ride in the track BUN alongside the streaming data.
//
//   Trigger regions  container 0x80034147, records in sub-chunk 0x0003414A
//   Position markers  chunk 0x00034146 (48-byte records)
//
// A trigger region is a *simple polygon* in the world XZ plane (even-odd
// containment; convexity is NOT required), guarded by a coarse AABB and a
// center/radius quick-reject. This module decodes the records and reimplements
// the engine's verified point-in-region test (PointInAABB2D + even-odd
// PointInPolygon2D) so tools can answer "is this point inside trigger N?"
// exactly as the game does.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 17 (Triggers & Barriers) and
// C17.7 (position markers); trigger record + the even-odd test verified against
// retail L2RA.BUN (705 records; literal reimplementation agrees 4230/4230).
// World space is Z-up; triggers use the ground XZ plane. Offline / pure.

#ifndef MWSDK_TRIGGERS_HPP
#define MWSDK_TRIGGERS_HPP

#include <array>
#include <cstdint>
#include <vector>

#include "mwsdk/core.hpp"

namespace mwsdk::triggers {

// --- chunk ids (Chapter 17) -------------------------------------------------
inline constexpr std::uint32_t kTriggerContainer = 0x80034147u; // TriggerRegionChunk
inline constexpr std::uint32_t kTriggerRecords   = 0x0003414Au; // record sub-chunk
inline constexpr std::uint32_t kTriggerAltId     = 0x0003414Du; // alt container id
inline constexpr std::uint32_t kMarkerChunk      = 0x00034146u; // TrackPositionMarkers

inline constexpr std::size_t kTriggerHeadBytes = 0x44; // fixed head before polygon
inline constexpr std::size_t kMarkerStride     = 48;   // bytes per position marker

// --- TriggerRegion (variable length) ----------------------------------------
// A polygon trigger volume. `polygon` is the ordered list of absolute world
// (X, Z) vertices; `aabb_*` is the broadphase box (== the vertex hull); the
// center/radius pair is a separate coarse gate (radius does NOT bound the
// polygon). `type` selects the gameplay role (gate/speedtrap/checkpoint/...).
struct TriggerRegion {
    std::uint32_t type = 0;       // +0x00 trigger type (observed 1..14)
    float         center_x = 0;   // +0x04 volume center X (world)
    float         center_z = 0;   // +0x08 volume center Z
    float         radius = 0;     // +0x14 quick-reject sphere radius
    std::uint32_t flags = 0;      // +0x18 0/1 (active?)
    float         aabb_min_x = 0; // +0x20 broadphase box
    float         aabb_min_z = 0; // +0x24
    float         aabb_max_x = 0; // +0x28
    float         aabb_max_z = 0; // +0x2C
    std::vector<std::array<float, 2>> polygon; // +0x44 vertex_count x (X, Z)
};

// --- PositionMarker (0x34146, 48 bytes) -------------------------------------
// A named 3-D anchor point: spawn point, camera node, gameplay reference. The
// name is stored hashed (recover via a name dictionary); position is full 3-D.
struct PositionMarker {
    std::uint32_t        name_hash = 0; // +0x08 marker name (hashed)
    std::uint32_t        index = 0;     // +0x0C
    std::array<float, 3> position{};    // +0x10 (x, y, z) world, Z-up
    std::uint32_t        param_a = 0;   // +0x20
    std::uint32_t        param_b = 0;   // +0x24
};

// --- geometry tests (the engine's verified containment math) ----------------
// 2-D AABB test on the broadphase box (eps 0), matching PointInAABB2D_Eps.
[[nodiscard]] bool point_in_aabb(const TriggerRegion& t, float x, float z) noexcept;

// Even-odd ray-crossing point-in-polygon in world XZ, matching the engine's
// PointInPolygon2D_EvenOdd. Verified: agrees 4230/4230 with a reference across
// all 705 retail records.
[[nodiscard]] bool point_in_polygon(const TriggerRegion& t, float x, float z) noexcept;

// Full containment: broadphase AABB then even-odd polygon (the game's order).
[[nodiscard]] bool contains(const TriggerRegion& t, float x, float z) noexcept;

// --- parsers ----------------------------------------------------------------
// Parse the trigger records out of a record-array payload (walks by the record
// length field at +0x42). Tolerant of a small leading pad/header.
[[nodiscard]] Result<std::vector<TriggerRegion>> parse_regions(Bytes records_payload);

// Parse 0x34146 position-marker records from a leaf payload (optional leading
// 0x11111111 pad word).
[[nodiscard]] Result<std::vector<PositionMarker>> parse_markers(Bytes leaf_payload);

// Find + decode every trigger region in a track BUN (locates the record chunk).
[[nodiscard]] Result<std::vector<TriggerRegion>> read_regions(Bytes bun);

// Find + decode every position marker in a track BUN.
[[nodiscard]] Result<std::vector<PositionMarker>> read_markers(Bytes bun);

} // namespace mwsdk::triggers

#endif // MWSDK_TRIGGERS_HPP
