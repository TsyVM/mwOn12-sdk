// SPDX-License-Identifier: MIT
//
// mwsdk/eagl.hpp
// --------------
// The EAGL container model — the 8-byte {id,size} chunk tree that every EAGL
// engine file is built from. Learn this one structure and you can walk any
// track bundle, texture pack, solid list, or vault.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 1 (EAGL Container Model),
// Appendix A.6 (chunk walker) and Appendix B.1 (chunk header).
//
//   struct ChunkHeader { uint32_t id; uint32_t size; };  // size excludes the 8 bytes
//   A chunk is a CONTAINER (has child chunks) iff (id & 0x80000000).
//   A leaf chunk's payload is opaque data for its subsystem.
//   0x11 bytes are alignment padding inside many payloads; strip_padding()
//   removes a leading run of them.
//
// All reads are bounds-checked: a malformed or truncated file yields an Error,
// never undefined behaviour.

#ifndef MWSDK_EAGL_HPP
#define MWSDK_EAGL_HPP

#include <cstdint>
#include <functional>
#include <vector>

#include "mwsdk/core.hpp"

namespace mwsdk::eagl {

// Bit 31 of a chunk id marks a container (its payload is more chunks).
inline constexpr std::uint32_t kContainerBit = 0x80000000u;

// Common alignment padding byte found inside payloads (Appendix A.6).
inline constexpr std::uint8_t  kPadByte = 0x11;

struct Chunk {
    std::uint32_t id      = 0;   // FourCC-style chunk identifier
    std::uint32_t size    = 0;   // payload size in bytes (excludes the 8-byte header)
    std::size_t   offset  = 0;   // absolute offset of the header within the root buffer
    Bytes         payload = {};   // view of the `size` payload bytes

    [[nodiscard]] constexpr bool is_container() const noexcept {
        return (id & kContainerBit) != 0;
    }
};

// Parse the direct child chunks of a buffer (one level, non-recursive).
// `base` is the absolute offset of `buf` within the root file, so returned
// Chunk::offset values are file-absolute. Stops cleanly at the first header
// that would overflow the buffer (matching the reference walker's behaviour).
[[nodiscard]] Result<std::vector<Chunk>> children(Bytes buf, std::size_t base = 0);

// Depth-first recursive walk. The visitor receives each chunk and its depth
// (0 = top level). Return false from the visitor to stop the walk early.
using Visitor = std::function<bool(const Chunk&, int depth)>;
Status walk(Bytes buf, const Visitor& visit);

// Find the first chunk with `id` anywhere in the tree (depth-first).
[[nodiscard]] Result<Chunk> find(Bytes buf, std::uint32_t id);

// Collect every chunk with `id` anywhere in the tree (depth-first order).
[[nodiscard]] std::vector<Chunk> find_all(Bytes buf, std::uint32_t id);

// Strip a leading run of 0x11 padding bytes from a payload view.
[[nodiscard]] Bytes strip_padding(Bytes payload) noexcept;

} // namespace mwsdk::eagl

#endif // MWSDK_EAGL_HPP
