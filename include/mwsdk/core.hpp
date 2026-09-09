// SPDX-License-Identifier: MIT
//
// mwsdk/core.hpp
// --------------
// Core vocabulary types shared by every MWSDK module: the Error enum, the
// Result<T>/Status aliases, and a bounds-checked little-endian ByteReader.
//
// Design mirrors VanHooks: one error enum, std::expected everywhere, no
// exceptions in the happy path, no global mutable state. Every parser reports
// failure through Result<T> rather than throwing or returning sentinels.
//
// Grounding: the byte-level conventions (little-endian unless noted, 0x11
// padding, the 8-byte chunk header) come from The Most Wanted Encyclopedia,
// Chapter 1 (EAGL Container Model) and Appendix B (Struct Reference).

#ifndef MWSDK_CORE_HPP
#define MWSDK_CORE_HPP

#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

#include "mwsdk/detail/expected.hpp"

namespace mwsdk {

// ---------------------------------------------------------------------------
// Error taxonomy. Grouped by subsystem so a numeric code alone locates the
// failure. Ranges follow VanHooks' convention (100s = memory/IO, etc.).
// ---------------------------------------------------------------------------
enum class Error : std::uint32_t {
    Ok = 0,

    // General (1-19)
    InvalidArgument = 1,
    Unsupported,
    OutOfRange,
    NotImplemented,

    // Buffer / IO (100-119)
    UnexpectedEndOfData = 100,
    BufferTooSmall,
    BadAlignment,

    // Container / chunk (200-219) — Chapter 1
    BadChunkHeader = 200,
    ChunkSizeOverflow,
    ChunkNotFound,
    NotAContainer,

    // Compression / JDLZ (300-319) — Chapter 3
    BadMagic = 300,
    UnsupportedVersion,
    DecompressedSizeMismatch,
    CorruptStream,

    // Texture / TPK (400-419) — Chapter 4
    BadTpkStructure = 400,
    UnsupportedPixelFormat,
    TextureNotFound,

    // Geometry (500-519) — Chapter 5
    BadSolidStructure = 500,
    UnknownVertexStride,

    // Attribute vault (600-619) — Chapter 7
    BadVaultHeader = 600,
    UnknownVaultSection,

    // Runtime / live process (700-719) - mwsdk/game/mw05.hpp
    ModuleNotFound = 700,
    AddressUnreadable,
    AddressUnwritable,
    ProtectFailed,
    SymbolNotFound,
    NullObject,       // a live-object pointer was null
    FieldNotFound,    // AttribCollection::GetField returned no storage for a key
};

// Human-readable, stable strings. Implemented in src/error.cpp.
std::string_view error_to_string(Error e) noexcept;

// ---------------------------------------------------------------------------
// Result vocabulary. Result<T> carries a value or an Error; Status carries
// only success/failure. err(...) is the shorthand for the failure path.
// ---------------------------------------------------------------------------
template <class T> using Result = expected<T, Error>;
using Status = expected<void, Error>;

[[nodiscard]] inline unexpected<Error> err(Error e) noexcept {
    return unexpected<Error>(e);
}

// Convenience aliases for raw byte views. MWSDK never takes ownership of input
// buffers — callers own the bytes; parsers produce lightweight views into them.
using Bytes     = std::span<const std::uint8_t>;
using MutBytes  = std::span<std::uint8_t>;

// ---------------------------------------------------------------------------
// ByteReader: a cursor over a Bytes view with bounds-checked, little-endian
// reads. Every read returns a Result so truncated or malformed input is a
// recoverable error, never undefined behaviour (Constitution: "avoid UB",
// "document invariants"). Multi-byte values are little-endian per the
// encyclopedia's global convention (Chapter 1 / README "Conventions").
// ---------------------------------------------------------------------------
class ByteReader {
public:
    constexpr explicit ByteReader(Bytes data, std::size_t offset = 0) noexcept
        : data_(data), pos_(offset) {}

    constexpr std::size_t position()  const noexcept { return pos_; }
    constexpr std::size_t size()      const noexcept { return data_.size(); }
    constexpr std::size_t remaining() const noexcept {
        return pos_ <= data_.size() ? data_.size() - pos_ : 0;
    }
    constexpr bool eof() const noexcept { return pos_ >= data_.size(); }
    constexpr Bytes underlying() const noexcept { return data_; }

    Status seek(std::size_t absolute) noexcept {
        if (absolute > data_.size()) return err(Error::OutOfRange);
        pos_ = absolute;
        return {};
    }
    Status skip(std::size_t n) noexcept {
        if (n > remaining()) return err(Error::UnexpectedEndOfData);
        pos_ += n;
        return {};
    }

    Result<std::uint8_t> u8() noexcept {
        if (remaining() < 1) return err(Error::UnexpectedEndOfData);
        return data_[pos_++];
    }
    Result<std::uint16_t> u16() noexcept {
        if (remaining() < 2) return err(Error::UnexpectedEndOfData);
        std::uint16_t v = static_cast<std::uint16_t>(
            data_[pos_] | (data_[pos_ + 1] << 8));
        pos_ += 2;
        return v;
    }
    Result<std::uint32_t> u32() noexcept {
        if (remaining() < 4) return err(Error::UnexpectedEndOfData);
        std::uint32_t v =
            static_cast<std::uint32_t>(data_[pos_]) |
            (static_cast<std::uint32_t>(data_[pos_ + 1]) << 8) |
            (static_cast<std::uint32_t>(data_[pos_ + 2]) << 16) |
            (static_cast<std::uint32_t>(data_[pos_ + 3]) << 24);
        pos_ += 4;
        return v;
    }
    Result<float> f32() noexcept {
        auto r = u32();
        if (!r) return err(r.error());
        // std::bit_cast rather than __builtin_memcpy: the builtin is a GCC and
        // Clang spelling that MSVC does not have, so this header could not be
        // compiled by the compiler most of its users are on. bit_cast is the
        // C++20 way to say the same thing, and it is constexpr besides.
        return std::bit_cast<float>(r.value());
    }

    // A non-owning view of the next `n` bytes; advances the cursor.
    Result<Bytes> take(std::size_t n) noexcept {
        if (n > remaining()) return err(Error::UnexpectedEndOfData);
        Bytes v = data_.subspan(pos_, n);
        pos_ += n;
        return v;
    }

    // A view of `n` bytes at an absolute offset; does NOT move the cursor.
    Result<Bytes> at(std::size_t offset, std::size_t n) const noexcept {
        if (offset > data_.size() || n > data_.size() - offset)
            return err(Error::OutOfRange);
        return data_.subspan(offset, n);
    }

private:
    Bytes       data_;
    std::size_t pos_;
};

// Read a fixed-width, possibly-unterminated ASCII field as a trimmed view.
// Used for the many fixed char[] name fields in TPK/solid/vault headers.
std::string_view fixed_string(Bytes field) noexcept;

} // namespace mwsdk

#endif // MWSDK_CORE_HPP
