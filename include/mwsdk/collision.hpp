// SPDX-License-Identifier: MIT
//
// mwsdk/collision.hpp
// -------------------
// The physical world the car touches:
//
//   * Terrain collision  (chunk 0x00034159) - the ground-height triangle soup
//       the wheels sample. Quantized s16 vertices, dequantized by fixed /4,/4,
//       /16 (coarse XY, fine Z). 18 bytes per self-contained triangle.
//   * Smackables          (chunk 0x00034027) - the knock-down prop spawners
//       (physics half of a breakable): 64-byte records with a stored-frame
//       position, an asset hash, and a vault parameter key.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 63 (Collision World), C63.7
// (terrain) and C63.9 (smackables). Dequant constants 4.0 / 0.25 / 0.0625
// verified in speed.exe; formats verified against retail track data. World is
// Z-up. Offline / pure.

#ifndef MWSDK_COLLISION_HPP
#define MWSDK_COLLISION_HPP

#include <array>
#include <cstdint>
#include <vector>

#include "mwsdk/core.hpp"

namespace mwsdk::collision {

// --- chunk ids (Chapter 63) -------------------------------------------------
inline constexpr std::uint32_t kTerrainChunk   = 0x00034159u; // terrain collision
inline constexpr std::uint32_t kSmackableChunk = 0x00034027u; // smackable spawners

inline constexpr std::size_t kTerrainHeader  = 24; // bytes before the triangles
inline constexpr std::size_t kTriangleStride = 18; // 9 x s16
inline constexpr std::size_t kSmackHeader    = 16; // bytes before the records
inline constexpr std::size_t kSmackStride    = 64; // bytes per record

// Fixed dequant multipliers: world = s16 * scale. XY coarse (1/4), Z fine (1/16).
inline constexpr float kDequantXY = 0.25f;   // 1/4  (const 4.0 in speed.exe)
inline constexpr float kDequantZ  = 0.0625f; // 1/16 (const 0.0625 in speed.exe)

// --- terrain collision ------------------------------------------------------
// One collision triangle: three world-space vertices (already dequantized).
struct Triangle {
    std::array<std::array<float, 3>, 3> v{}; // v[i] = {x, y, z} world (Z-up)
};

// One 0x34159 chunk's worth of terrain: a self-contained triangle soup (no
// shared vertex/index buffer) for one world section.
struct TerrainMesh {
    std::uint32_t         section_id = 0;
    std::vector<Triangle> triangles;
};

[[nodiscard]] Result<TerrainMesh> parse_terrain(Bytes chunk_payload);

// Every 0x34159 mesh in a track/stream BUN (one per world section).
[[nodiscard]] Result<std::vector<TerrainMesh>> read_terrain(Bytes bun);

// --- smackables -------------------------------------------------------------
// One knock-down prop spawner. `position`/`direction` are in the exporter's
// stored frame (not world space - see C63.9). `asset_hash` names the model
// (bin hash); `param_hash` keys the vault smackable parameters; `local_index`
// is stable within the section.
struct Smackable {
    std::array<float, 3> direction{}; // +0x04 unit vector (stored frame)
    std::array<float, 3> position{};  // +0x10 (stored frame)
    std::uint32_t        asset_hash = 0; // +0x20 (repeated at +0x24)
    std::uint32_t        param_hash = 0; // +0x28 vault smackable key
    std::uint16_t        global_id = 0;  // +0x2C
    std::uint32_t        local_index = 0;// +0x30 firstLocalIndex + n
    float                scale = 0;      // +0x38
};

// One 0x34027 chunk: the smackable records for one world section.
struct SmackableSection {
    std::int16_t           section_id = 0;
    std::int16_t           first_local_index = 0;
    std::vector<Smackable> records;
};

[[nodiscard]] Result<SmackableSection> parse_smackables(Bytes chunk_payload);

// Every 0x34027 section in a track/stream BUN.
[[nodiscard]] Result<std::vector<SmackableSection>> read_smackables(Bytes bun);

} // namespace mwsdk::collision

#endif // MWSDK_COLLISION_HPP
