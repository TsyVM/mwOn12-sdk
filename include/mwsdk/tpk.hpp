// SPDX-License-Identifier: MIT
//
// mwsdk/tpk.hpp
// -------------
// TPK texture packs — reading the standard (uncompressed) variant and decoding
// its DXT (BC1/BC2/BC3) surfaces to RGBA8 for preview or conversion.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 4 (Textures & Materials),
// Appendix A.4 (DXT decode) and Appendix B.3 (TPK structs), plus the master
// chunk-ID table (Glossary/chunk-ids.md).
//
//   0xB3300000 TPKContainer
//     0xB3310000 TPKInfo
//       0x33310001 TPKInfoHeader   (version, pack name, path, hash)
//       0x33310004 TPKEntries      (124-byte standard entries)
//     0xB3320000 TPKData
//       0x33320002 TPKDataRaw      (the pixel blob; entry.dataOffset indexes it)
//
// The compressed variant (0x33310003 descriptors + per-texture JDLZ blobs) is
// out of scope for this module; read() reports BadTpkStructure if the standard
// entry/data chunks are absent.

#ifndef MWSDK_TPK_HPP
#define MWSDK_TPK_HPP

#include <cstdint>
#include <string_view>
#include <vector>

#include "mwsdk/core.hpp"

namespace mwsdk::tpk {

// Chunk identifiers (Glossary master table).
inline constexpr std::uint32_t kTPKContainer  = 0xB3300000u;
inline constexpr std::uint32_t kTPKInfo       = 0xB3310000u;
inline constexpr std::uint32_t kTPKInfoHeader = 0x33310001u;
inline constexpr std::uint32_t kTPKHashTable  = 0x33310002u;
inline constexpr std::uint32_t kTPKCompEntries= 0x33310003u;
inline constexpr std::uint32_t kTPKEntries    = 0x33310004u;
inline constexpr std::uint32_t kTPKData       = 0xB3320000u;
inline constexpr std::uint32_t kTPKDataRaw    = 0x33320002u;

// The one verified compression code (Appendix B.3: 0x24 = DXT3). Others are
// surfaced raw via Texture::compr_type rather than guessed.
inline constexpr std::uint8_t kComprDXT3 = 0x24;

// Block-compression formats supported by decode_dxt (Appendix A.4).
enum class DxtFormat { DXT1, DXT3, DXT5 };

struct Texture {
    std::string_view name;         // fixed 24-byte name field, trimmed
    std::uint32_t    name_hash  = 0;  // Joaat of the name
    std::uint32_t    class_hash = 0;
    std::uint16_t    width      = 0;
    std::uint16_t    height     = 0;
    std::uint8_t     mip_count  = 0;
    std::uint8_t     compr_type = 0;  // raw code; kComprDXT3 (0x24) = DXT3
    Bytes            pixels     = {};  // view into TPKDataRaw (dataOffset..+dataSize)

    // True only for the one verified code. Other codes are left to the caller.
    [[nodiscard]] bool is_dxt3() const noexcept { return compr_type == kComprDXT3; }
};

// Read all textures from a TPK container (standard variant). `buf` may be the
// whole file or the TPKContainer chunk; the walker locates the entry/data
// chunks either way.
[[nodiscard]] Result<std::vector<Texture>> read(Bytes buf);

// Decode a DXT/BCn surface to tightly-packed RGBA8 (w*h*4 bytes).
// Faithful to Appendix A.4: 565 colour expansion, DXT1 1-bit alpha, DXT3
// explicit 4-bit alpha, DXT5 interpolated alpha.
[[nodiscard]] Result<std::vector<std::uint8_t>>
decode_dxt(Bytes blocks, std::uint32_t width, std::uint32_t height, DxtFormat fmt);

} // namespace mwsdk::tpk

#endif // MWSDK_TPK_HPP
