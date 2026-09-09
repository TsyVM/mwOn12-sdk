// SPDX-License-Identifier: MIT
//
// mwsdk/geometry.hpp
// ------------------
// Solid geometry — walking a solid list into objects, their shading groups,
// and their vertex/index buffers, and decoding the three known vertex layouts.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 5 (3D Geometry & Solids),
// Appendix B.4 (solid structs) and the master chunk-ID table.
//
//   0x80134000 GeometryContainer
//     0x80134010 GeometryObject
//       0x00134011 GeometryObjectHeader (160 bytes + name, after 0x11 padding)
//       0x80134100 GeometryMesh
//         0x00134B01 MeshVertices
//         0x00134B02 MeshShadingGroups (104 bytes each)
//         0x00134B03 MeshIndices       (u16 triangle list)
//
// Vertex strides (Appendix B.4): 36B and 60B map to verified FVF flags; 24B is
// a known layout not tied here to a specific flag, so it is decoded only when a
// caller passes stride 24 explicitly.

#ifndef MWSDK_GEOMETRY_HPP
#define MWSDK_GEOMETRY_HPP

#include <cstdint>
#include <string_view>
#include <vector>

#include "mwsdk/core.hpp"

namespace mwsdk::geom {

// Chunk identifiers.
inline constexpr std::uint32_t kGeometryContainer = 0x80134000u;
inline constexpr std::uint32_t kGeometryObject    = 0x80134010u;
inline constexpr std::uint32_t kObjectHeader      = 0x00134011u;
inline constexpr std::uint32_t kGeometryMesh      = 0x80134100u;
inline constexpr std::uint32_t kMeshVertices      = 0x00134B01u;
inline constexpr std::uint32_t kMeshShadingGroups = 0x00134B02u;
inline constexpr std::uint32_t kMeshIndices       = 0x00134B03u;

// Verified FVF flag → vertex stride mappings (Appendix B.4).
inline constexpr std::uint32_t kFvf36 = 0x004000u;   // -> 36-byte stride
inline constexpr std::uint32_t kFvf60a= 0x2A4000u;   // -> 60-byte stride
inline constexpr std::uint32_t kFvf60b= 0x224000u;   // -> 60-byte stride

struct ShadingGroup {
    std::uint32_t fvf_flags    = 0;
    std::uint32_t vertex_count = 0;
    std::uint32_t tri_count    = 0;
    std::uint32_t first_index  = 0;
    std::uint32_t index_count  = 0;
    std::uint32_t tex_count    = 0;
    std::uint8_t  shader_idx   = 0;
    std::uint8_t  tex_idx[5]   = {0, 0, 0, 0, 0};
};

struct SolidObject {
    std::string_view name;
    std::uint32_t    name_hash = 0;
    std::uint32_t    num_tris  = 0;
    std::uint32_t    flags     = 0;
    float            bbox_min[4] = {0, 0, 0, 0};
    float            bbox_max[4] = {0, 0, 0, 0};
    float            transform[16] = {0};       // row-major, read straight (no transpose)
    std::vector<ShadingGroup> groups;
    Bytes            vertices = {};   // MeshVertices payload (all shading groups concatenated)
    Bytes            indices  = {};   // MeshIndices payload (u16 triangle list)
};

// A decoded vertex. `has_normal` is false for the 24-byte layout.
struct Vertex {
    float        pos[3]    = {0, 0, 0};
    float        normal[3] = {0, 0, 0};
    std::uint8_t bgra[4]   = {0, 0, 0, 0};
    float        uv[2]     = {0, 0};
    bool         has_normal = false;
};

// Read every solid object in a geometry container (or whole file).
[[nodiscard]] Result<std::vector<SolidObject>> read(Bytes buf);

// Map a verified FVF flag word to its vertex stride, or UnknownVertexStride.
[[nodiscard]] Result<std::uint32_t> stride_for_fvf(std::uint32_t fvf_flags) noexcept;

// Decode one vertex from a `stride`-byte record (stride in {24, 36, 60}).
[[nodiscard]] Result<Vertex> decode_vertex(Bytes record, std::uint32_t stride) noexcept;

} // namespace mwsdk::geom

#endif // MWSDK_GEOMETRY_HPP
