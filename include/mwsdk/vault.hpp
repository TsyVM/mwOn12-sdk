// SPDX-License-Identifier: MIT
//
// mwsdk/vault.hpp
// ---------------
// VPAK attribute vaults - the tuning/AI/gameplay databases. This module reads
// the header, enumerates the tagged blocks, splits the typed-record block
// (NtaD) into records at their 0xEFFECADD markers, and decodes each record's
// verified descriptor fields.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 7 (Attribute Vaults),
// Appendix B.6, and Discovery 4 (the decoded record form).
//
//   struct VPAKHeader {
//       char     magic[4];       // 'VPAK'
//       uint32_t version;        // 1
//       uint32_t headerSize;     // 0x40
//       uint32_t sectionCount;   // 3
//       uint32_t sectionWords[]; // @+16
//   };
//   Tagged blocks begin at +0x80: { char tag[4]; uint32_t size; uint8_t data[size]; }
//     ErtS = string table   NpeD = deps   NrtS = symbols   NtaD = typed records
//   Each NtaD record starts with the marker 0xEFFECADD and runs to the next one.

#ifndef MWSDK_VAULT_HPP
#define MWSDK_VAULT_HPP

#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

#include "mwsdk/core.hpp"
#include "mwsdk/vault_schema.inl" // generated field-key schema (offline)
#include "mwsdk/reflection.hpp"       // reflection primitive types (offline)

namespace mwsdk::vault {

inline constexpr std::size_t   kBlocksOffset = 0x80;       // first tagged block
inline constexpr std::uint32_t kRecordMarker = 0xEFFECADDu; // NtaD record marker
inline constexpr std::array<std::uint8_t, 4> kMagic = {'V', 'P', 'A', 'K'};

struct Header {
    std::uint32_t version       = 0;
    std::uint32_t header_size   = 0;
    std::uint32_t section_count = 0;
};

// A tagged block: its 4-char tag ('ErtS'/'NpeD'/'NrtS'/'NtaD'), payload view,
// and absolute file offset of the tag.
struct Block {
    std::string_view tag;
    Bytes            payload;
    std::size_t      offset = 0;
};

[[nodiscard]] bool is_vpak(Bytes buf) noexcept;

[[nodiscard]] Result<Header> read_header(Bytes buf) noexcept;

// Enumerate every tagged block starting at +0x80.
[[nodiscard]] Result<std::vector<Block>> blocks(Bytes buf);

// Find a block by its 4-char tag (e.g. "NtaD"). Returns UnknownVaultSection if
// absent.
[[nodiscard]] Result<Block> find_block(Bytes buf, std::string_view tag);

// Split an NtaD payload into individual records at the 0xEFFECADD markers.
// Each returned view begins at a marker and runs to the next (or to the end).
[[nodiscard]] std::vector<Bytes> records(Bytes ntad_payload);

// Convenience: read the NtaD block and split it in one call.
[[nodiscard]] Result<std::vector<Bytes>> read_records(Bytes buf);

// ---------------------------------------------------------------------------
// Descriptor decode (read-side).
//
// The leading record descriptor is verified consistent across the 40-byte
// "full" form (Encyclopedia D4 sec.3): field, element type, collection key,
// class, and inherited parent key. Shorter records omit the trailing fields;
// `decode` populates each field only when the record is long enough to carry it.
//
// This decoder is DESCRIPTOR-ONLY on purpose. The inline value triples are a
// verified *mechanism* (D4 sec.6) but their exact per-record byte offsets are
// still frontier, so the SDK does not fabricate a value read/write here - it
// hands you the raw record span and the resolved descriptor, and leaves value
// parsing to you (or a future verified pass). Nothing here is guessed.
// ---------------------------------------------------------------------------
struct Record {
    std::uint32_t field_hash     = 0; // @+0x04 attribute-name key (0 if too short)
    std::uint32_t type_id        = 0; // @+0x08 element type id
    std::uint32_t collection_key = 0; // @+0x0C the car / collection (0 if absent)
    std::uint32_t class_hash     = 0; // @+0x10 class / collection family (0 if absent)
    std::uint32_t parent_key     = 0; // @+0x14 inherited-from key (0 if absent)
    std::uint32_t element_count  = 0; // @+0x24 (only on records >= 0x28 bytes)
    std::size_t   offset         = 0; // record offset within the NtaD payload
    Bytes         raw;                // full record view (marker .. next marker)
};

// Decode an NtaD payload into descriptor records (fields populated by size).
[[nodiscard]] std::vector<Record> decode(Bytes ntad_payload);

// Convenience: locate the NtaD block in a whole VPAK buffer and decode it.
[[nodiscard]] Result<std::vector<Record>> read_decoded(Bytes buf);

// Reverse-resolve a lookup2 attrib key to a field name via the verified schema
// (mwsdk::vault::schema::kFields). Returns "" for the residual stripped names
// (frontier) - never a guess.
[[nodiscard]] std::string_view field_name(std::uint32_t key) noexcept;

// Reverse-resolve a class / collection key to its class name via the verified
// schema (a field's class name hashes to the key). "" if not in the schema.
[[nodiscard]] std::string_view class_name(std::uint32_t key) noexcept;

// The reflection primitive type for a decoded record's element type id, or
// nullptr if the id is not a known reflection type. Bridges Record::type_id to
// mwsdk::reflection (name + byte width).
[[nodiscard]] const reflection::Type* type_of(std::uint32_t type_id) noexcept;

} // namespace mwsdk::vault

#endif // MWSDK_VAULT_HPP
