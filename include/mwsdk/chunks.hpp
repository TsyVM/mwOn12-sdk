// SPDX-License-Identifier: MIT
//
// mwsdk/chunks.hpp
// ----------------
// Names for EAGL chunk ids. The engine dispatches each chunk id to a handler
// class (LoadChunk/UnloadChunk); this module exposes that verified dispatch
// table so a chunk walker can *name* what it finds instead of printing a bare
// 0x000341xx. Pair it with mwsdk/eagl.hpp: walk the tree, then resolve each
// Chunk::id to a handler.
//
// Grounding: The Most Wanted Encyclopedia, RE-Data-And-Discoveries chunk
// handler registry (recovered from speed.exe's chunk-dispatch site table).
// The table is GENERATED into chunk_registry.inl — never hand-edited.
//
//   const auto* h = chunks::handler(0x00034159);
//   if (h) printf("%.*s\n", (int)h->handler.size(), h->handler.data());
//   // -> "WorldChunk34159"  (terrain collision)
//
// Pure/offline: no OS dependency, constexpr-friendly, unit-testable anywhere.

#ifndef MWSDK_CHUNKS_HPP
#define MWSDK_CHUNKS_HPP

#include <cstdint>
#include <string_view>

#include "mwsdk/chunk_registry.inl" // generated: kChunks[], kChunkCount

namespace mwsdk::chunks {

// Bit 31 marks a container chunk (its payload is more chunks); see eagl.hpp.
inline constexpr std::uint32_t kContainerBit = 0x80000000u;

// The chunk id with the container bit cleared (the "family" id).
[[nodiscard]] constexpr std::uint32_t base_id(std::uint32_t id) noexcept {
    return id & ~kContainerBit;
}

// Find the handler entry for an exact chunk id (binary search over kChunks).
// Returns nullptr if the id is not a known dispatched chunk.
[[nodiscard]] constexpr const ChunkHandler* find_exact(std::uint32_t id) noexcept {
    std::size_t lo = 0, hi = kChunkCount;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (kChunks[mid].id < id) lo = mid + 1;
        else                      hi = mid;
    }
    if (lo < kChunkCount && kChunks[lo].id == id) return &kChunks[lo];
    return nullptr;
}

// Resolve a chunk id to its handler, tolerant of the container bit: tries the
// id as-is, then with the container bit toggled (some ids appear only in one
// form in the dispatch table). Returns nullptr if genuinely unknown.
[[nodiscard]] constexpr const ChunkHandler* handler(std::uint32_t id) noexcept {
    if (const ChunkHandler* h = find_exact(id)) return h;
    return find_exact(id ^ kContainerBit);
}

// True if the id is a known dispatched chunk (either form).
[[nodiscard]] constexpr bool is_known(std::uint32_t id) noexcept {
    return handler(id) != nullptr;
}

// The handler class name for a chunk id, or "" if unknown. Never a guess: the
// name comes straight from the verified dispatch table.
[[nodiscard]] constexpr std::string_view name(std::uint32_t id) noexcept {
    const ChunkHandler* h = handler(id);
    return h ? h->handler : std::string_view{};
}

} // namespace mwsdk::chunks

#endif // MWSDK_CHUNKS_HPP
