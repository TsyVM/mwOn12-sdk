// SPDX-License-Identifier: MIT
//
// mwsdk/scenery.hpp
// -----------------
// World scenery: the placed props (streetlights, trees, buildings, signs) and
// the per-section cull tree that decides which are visible. A ScenerySection
// (EAGL container 0x80034100) holds three leaf arrays this module decodes:
//
//   0x00034102  SceneryInfo      72-byte model records (name, LOD solid hashes)
//   0x00034103  SceneryInstance  64-byte placements (AABB, position, orient)
//   0x00034105  SceneryTreeNode  36-byte bounded-fanout AABB cull tree
//
// Grounding: The Most Wanted Encyclopedia, Chapter 16 (Scenery & the Cull Tree),
// verified against retail STREAML2RA.BUN (947 sections, 77,783 instances) and
// the loader at 0x007311F0. Orientation is a row-major 3x3 rotation*scale in
// s3.12 fixed point (8192 == 1.0). World space is Z-up (game native).
//
// Offline / pure: bounds-checked, returns Result<T>, owns no memory.

#ifndef MWSDK_SCENERY_HPP
#define MWSDK_SCENERY_HPP

#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

#include "mwsdk/core.hpp"

namespace mwsdk::scenery {

// --- chunk ids (Chapter 16) -------------------------------------------------
inline constexpr std::uint32_t kSectionContainer = 0x80034100u; // ScenerySection
inline constexpr std::uint32_t kHeaderChunk      = 0x00034101u; // section header
inline constexpr std::uint32_t kInfoChunk        = 0x00034102u; // SceneryInfo[]
inline constexpr std::uint32_t kInstanceChunk    = 0x00034103u; // SceneryInstance[]
inline constexpr std::uint32_t kTreeChunk        = 0x00034105u; // cull tree nodes

inline constexpr std::size_t kInstanceStride = 64; // bytes per instance
inline constexpr std::size_t kInfoStride     = 72; // bytes per info
inline constexpr std::size_t kTreeNodeStride = 36; // bytes per cull node

// Fixed-point scale for the orientation matrix: stored s16 / 8192 == float.
inline constexpr float kOrientFixedScale = 8192.0f;

// --- SceneryInstance (0x34103, 64 bytes) ------------------------------------
// One placed prop. The orientation is a row-major 3x3 rotation*scale matrix;
// det>0 is a normal instance, det<0 a mirrored one, non-unit row norms a scaled
// one. `position` is the pivot; `aabb_*` is the instance's world bound.
struct Instance {
    std::array<float, 3> aabb_min{};    // +0x00 world AABB min (X, Y, Z-up)
    std::array<float, 3> aabb_max{};    // +0x0C world AABB max
    std::uint32_t        flags = 0;     // +0x18 bitfield (per-bit meaning partial)
    std::uint16_t        field_1c = 0;  // +0x1C zero in all retail samples
    std::uint16_t        sentinel = 0;  // +0x1E 0xFFFF in each section's 1st record
    std::array<float, 3> position{};    // +0x20 pivot (world)
    std::array<float, 9> orient{};      // +0x2C row-major 3x3 (fixed-point decoded)
    std::uint16_t        info_index = 0; // +0x3E index into this section's SceneryInfo[]

    // Signed determinant of the 3x3 orientation (sign flags mirrored instances).
    [[nodiscard]] float orient_det() const noexcept {
        const auto& m = orient;
        return m[0] * (m[4] * m[8] - m[5] * m[7])
             - m[1] * (m[3] * m[8] - m[5] * m[6])
             + m[2] * (m[3] * m[7] - m[4] * m[6]);
    }
};

// --- SceneryInfo (0x34102, 72 bytes) ----------------------------------------
// One model description shared by many instances. `solid_lod` is the near/mid/
// far solid name-hash chain (identical x3 when the model has no LOD variants).
struct Info {
    std::string_view      model_name;        // +0x00 char[24] ASCII (trimmed)
    std::array<std::uint32_t, 3> solid_lod{}; // +0x18 near/mid/far solid hashes
    std::uint32_t         solid_hash2 = 0;    // +0x24 extra variant (0 if none)
    float                 bounding_radius = 0;// +0x38 world-unit radius
    std::uint32_t         hash_3c = 0;        // +0x3C per-model hash
    std::uint32_t         on_load_resolve = 0;// +0x40 optional resolve hash

    [[nodiscard]] bool has_lod() const noexcept {
        return solid_lod[0] != solid_lod[2]; // near != far => real LOD chain
    }
};

// --- SceneryCullNode (0x34105, 36 bytes) ------------------------------------
// A node in the per-section bounded-fanout (<=5) AABB hierarchy. An entry >= 0
// is an instance index into this section's 0x34103 array; an entry < 0 is a
// child node index (= -entry). Node 0 is the root.
struct CullNode {
    std::array<float, 3> aabb_min{};   // +0x00 world AABB min
    std::array<float, 3> aabb_max{};   // +0x0C world AABB max
    std::int16_t         entry_count = 0; // +0x18 (1..5)
    std::array<std::int16_t, 5> entries{}; // +0x1A instance idx (>=0) or -child

    [[nodiscard]] bool is_leaf_entry(int i) const noexcept {
        return i >= 0 && i < entry_count && entries[static_cast<std::size_t>(i)] >= 0;
    }
};

// A decoded scenery section: the three arrays that share one container.
struct Section {
    std::vector<Info>     infos;
    std::vector<Instance> instances;
    std::vector<CullNode> tree;
};

// --- leaf-level parsers (take a single leaf chunk payload) ------------------
[[nodiscard]] Result<std::vector<Instance>> parse_instances(Bytes leaf_payload);
[[nodiscard]] Result<std::vector<Info>>     parse_infos(Bytes leaf_payload);
[[nodiscard]] Result<std::vector<CullNode>> parse_tree(Bytes leaf_payload);

// --- container-level: decode every ScenerySection in a track/stream BUN ------
// Walks the EAGL tree; every container carrying a 0x34103 instance leaf is a
// section. Returns one Section per such container, in tree order.
[[nodiscard]] Result<std::vector<Section>> read_sections(Bytes bun);

} // namespace mwsdk::scenery

#endif // MWSDK_SCENERY_HPP
