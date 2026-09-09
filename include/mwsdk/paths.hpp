// SPDX-License-Identifier: MIT
//
// mwsdk/paths.hpp
// ---------------
// The two road layers of the world:
//
//   * Track paths  (TrackPathManager container 0x80034150, records 0x00034152)
//       Dense waypoint polylines - the exact line traffic and cops drive down a
//       stretch of road. A *geometry for following*.
//   * CARP road graph  (WorldMapData chunk 0x0003B800)
//       Nodes + segments for routing: the GPS shortest path, junctions, arc
//       costs. A *graph for pathfinding*.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 61 (traffic paths) and
// Chapter 18 (CARP). Track-path record and CARP node/segment layouts verified
// byte-for-byte against retail L2RA.BUN (443 path segments / ~4,136 waypoints;
// 4,385 nodes; 6,538 segments). World space is Z-up. Offline / pure.

#ifndef MWSDK_PATHS_HPP
#define MWSDK_PATHS_HPP

#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

#include "mwsdk/core.hpp"

namespace mwsdk::paths {

// --- chunk ids --------------------------------------------------------------
inline constexpr std::uint32_t kPathManager = 0x80034150u; // TrackPathManager
inline constexpr std::uint32_t kPathIndex   = 0x00034151u; // segment index
inline constexpr std::uint32_t kPathSegments= 0x00034152u; // the segment records
inline constexpr std::uint32_t kPathConnect = 0x00034153u; // connectivity
inline constexpr std::uint32_t kCarpChunk   = 0x0003B800u; // WorldMapData (CARP)

inline constexpr std::size_t kPathRecordHead = 0x24; // fixed head before waypoints

// ===========================================================================
// Track paths (0x34152)
// ===========================================================================
// One path segment: an id, its 2-D bounding box and anchor, and the waypoint
// polyline the AI follows (world X,Y ground-plane points). record size =
// 0x24 + waypoint_count*8.
struct TrackPath {
    std::uint16_t        path_id = 0;         // +0x08
    std::uint16_t        waypoint_count = 0;  // +0x0A
    std::array<float, 4> bbox{};              // +0x0C (minX, minY, maxX, maxY)
    std::array<float, 2> anchor{};            // +0x1C centre/entry point
    std::vector<std::array<float, 2>> waypoints; // +0x24 (X, Y) polyline
};

[[nodiscard]] Result<std::vector<TrackPath>> parse_track_paths(Bytes segs_payload);
[[nodiscard]] Result<std::vector<TrackPath>> read_track_paths(Bytes bun);

// ===========================================================================
// CARP road graph (0x3B800)
// ===========================================================================
// A section-registry row: 4-char tag, record count, arena byte offset.
struct CarpSection {
    std::string_view tag;          // e.g. "RNnd", "RNsg" (un-reversed)
    std::uint32_t    count = 0;
    std::uint32_t    arena_offset = 0;
};

// A road-graph node: a junction/point in world space with its incident segments.
struct CarpNode {
    float         x = 0, y = 0, z = 0; // world (z is the height field, Z-up)
    std::uint16_t node_index = 0;      // == own ordinal
    std::uint16_t road_index = 0;      // into the road-ref table
    std::uint8_t  degree = 0;          // incident segment count
    std::array<std::uint16_t, 7> segments{}; // incident segment ids (0xAAAA pad)
};

// A road-graph segment (edge) between two nodes, with a curve arc-length.
struct CarpSegment {
    std::uint16_t node_a = 0;         // +0x00
    std::uint16_t node_b = 0;         // +0x02
    std::uint16_t arc_len_q106 = 0;   // +0x04 curve length x64 (Q10.6)
    std::uint16_t optional_index = 0; // +0x06 (0xFFFF when none)
    std::uint16_t segment_index = 0;  // +0x08 == own ordinal
    std::uint16_t flags = 0;          // +0x0A lane/config (semantics partial)

    // Decoded curve arc length in world units (Q10.6 fixed point).
    [[nodiscard]] float arc_length() const noexcept {
        return static_cast<float>(arc_len_q106) / 64.0f;
    }
};

struct CarpNetwork {
    bool                     valid_magic = false; // 'CARP' present
    std::uint32_t            version = 0;          // 122 on retail MW
    std::vector<CarpSection> sections;
    std::vector<CarpNode>    nodes;
    std::vector<CarpSegment> segments;
};

// Parse the CARP container payload (header + section registry + node/segment
// lattices). Node/segment arenas are located by their self-index (each record
// carries its own ordinal), so no fragile absolute offsets are baked in.
[[nodiscard]] Result<CarpNetwork> parse_carp(Bytes carp_payload);
[[nodiscard]] Result<CarpNetwork> read_carp(Bytes bun);

} // namespace mwsdk::paths

#endif // MWSDK_PATHS_HPP
