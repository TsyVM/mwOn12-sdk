// SPDX-License-Identifier: MIT
//
// mwsdk/jdlz.hpp
// --------------
// JDLZ compression — the LZ+bitstream codec wrapping most of the game's
// compressed streams (`.lzc`, minimap tiles, per-texture blobs, etc.).
//
// Grounding: The Most Wanted Encyclopedia, Chapter 3 (Compression / JDLZ),
// Appendix A.3 (decompressor) and Appendix B.2 (16-byte header).
//
//   Header (16 bytes, little-endian):
//     char     magic[4]         = 'JDLZ'
//     uint8_t  version          = 0x02
//     uint8_t  headerSize       = 0x10
//     uint16_t reserved         = 0
//     uint32_t decompressedSize
//     uint32_t compressedSize   (== whole file size)
//
// decompress() reproduces the verified dual-flag LZ algorithm exactly.
//
// encode_store() emits a VALID JDLZ stream in literal/store mode — every
// control bit set to "literal", so no back-references are produced. This is a
// faithful, minimal use of the documented bitstream (not a reconstruction of
// EA's original match-finder), and exists so callers can round-trip data the
// game will accept. It never invents format details; it only exercises the
// literal path the decompressor already defines. Ratio is ~1:1 (slightly
// larger than input); use it for correctness, not size.

#ifndef MWSDK_JDLZ_HPP
#define MWSDK_JDLZ_HPP

#include <cstdint>
#include <vector>

#include "mwsdk/core.hpp"

namespace mwsdk::jdlz {

inline constexpr std::size_t kHeaderSize = 16;
inline constexpr std::uint8_t kVersion   = 0x02;
// 'JDLZ' as it appears at file offset 0 (bytes 'J','D','L','Z').
inline constexpr std::uint8_t kMagic[4]  = {0x4A, 0x44, 0x4C, 0x5A};

struct Header {
    std::uint8_t  version;
    std::uint8_t  header_size;
    std::uint32_t decompressed_size;
    std::uint32_t compressed_size;
};

// True if `data` begins with the 'JDLZ' magic.
[[nodiscard]] bool is_jdlz(Bytes data) noexcept;

// Parse and validate the 16-byte header.
[[nodiscard]] Result<Header> read_header(Bytes data) noexcept;

// Decompress a complete JDLZ stream (header + payload) to its original bytes.
// Verifies the output length against the header's decompressedSize.
[[nodiscard]] Result<std::vector<std::uint8_t>> decompress(Bytes data);

// Encode `raw` as a valid JDLZ stream in literal/store mode (see file header).
// decompress(encode_store(x)) == x for all x.
[[nodiscard]] std::vector<std::uint8_t> encode_store(Bytes raw);

} // namespace mwsdk::jdlz

#endif // MWSDK_JDLZ_HPP
