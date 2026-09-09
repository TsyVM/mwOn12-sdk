// SPDX-License-Identifier: MIT
//
// mwsdk/hashing.hpp
// -----------------
// The hash functions the game uses to turn names into the 32-bit keys that
// index vaults, texture packs, solids and bins.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 2 (Identifiers & Hashing),
// Appendix A.1/A.2 (Code Library) and RE-Data engine_hash.json. Every
// algorithm is reproduced bit-for-bit from the verified reference and checked
// against known game constants in the test suite.
//
//   * lookup2 ("AttribStringKey", Bob Jenkins lookup2 1996, seed 0xABCDEF00) -
//     THE engine hash. Every attribute-vault key, class name, collection key
//     (car name) and field name is this hash. Verified: attrib("default")
//     == 0xEEC2271A. This is what you want for car tuning. It is NOT Joaat.
//   * Joaat  ("one-at-a-time", Jenkins) - used for asset/texture/solid names.
//     Names are stored lower-case, so joaat_ci mirrors the game's own
//     case-insensitive behaviour.
//   * Bin    (sum hash, seed 0xFFFFFFFF, multiplier 33) - used for some
//     bin/label keys and scenery-group names.
//
// Header-only and constexpr: hashes of string literals fold at compile time,
// which the Constitution's "prefer compile-time validation" rule encourages
// (you can build switch statements on attrib("...") constants).

#ifndef MWSDK_HASHING_HPP
#define MWSDK_HASHING_HPP

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace mwsdk::hash {

// Joaat over an explicit byte range. Faithful to Appendix A.1.
[[nodiscard]] constexpr std::uint32_t joaat(std::span<const std::uint8_t> s) noexcept {
    std::uint32_t h = 0;
    for (std::uint8_t c : s) {
        h += c;
        h += h << 10;
        h ^= h >> 6;
    }
    h += h << 3;
    h ^= h >> 11;
    h += h << 15;
    return h;
}

namespace detail {
constexpr std::uint8_t to_lower(std::uint8_t c) noexcept {
    return (c >= 'A' && c <= 'Z') ? static_cast<std::uint8_t>(c - 'A' + 'a') : c;
}
} // namespace detail

// Joaat over a string, byte-exact (no case folding).
[[nodiscard]] constexpr std::uint32_t joaat(std::string_view s) noexcept {
    std::uint32_t h = 0;
    for (char ch : s) {
        h += static_cast<std::uint8_t>(ch);
        h += h << 10;
        h ^= h >> 6;
    }
    h += h << 3;
    h ^= h >> 11;
    h += h << 15;
    return h;
}

// Case-insensitive Joaat - lower-cases each ASCII byte first. This is the form
// the engine uses when resolving names, because stored names are lower-case.
[[nodiscard]] constexpr std::uint32_t joaat_ci(std::string_view s) noexcept {
    std::uint32_t h = 0;
    for (char ch : s) {
        h += detail::to_lower(static_cast<std::uint8_t>(ch));
        h += h << 10;
        h ^= h >> 6;
    }
    h += h << 3;
    h ^= h >> 11;
    h += h << 15;
    return h;
}

// Bin (sum) hash. Faithful to Appendix A.2: seed 0xFFFFFFFF, h = c + 33*h.
[[nodiscard]] constexpr std::uint32_t bin(std::string_view s) noexcept {
    std::uint32_t h = 0xFFFFFFFFu;
    for (char ch : s)
        h = static_cast<std::uint8_t>(ch) + 33u * h;
    return h;
}

[[nodiscard]] constexpr std::uint32_t bin(std::span<const std::uint8_t> s) noexcept {
    std::uint32_t h = 0xFFFFFFFFu;
    for (std::uint8_t c : s)
        h = c + 33u * h;
    return h;
}

// ---------------------------------------------------------------------------
// lookup2 - the engine's AttribStringKey hash (Bob Jenkins lookup2, 1996).
//
// This is the single most important hash for modding: every attribute-vault
// key, engine class name, car/collection key and field name is this hash with
// the reflection seed 0xABCDEF00. Verified bit-exact against the retail
// speed.exe inline constants (Encyclopedia Ch.2 / engine_hash.json):
//   attrib("default")            == 0xEEC2271A
//   attrib("TopSpeedMultiplier") == 0xEC57E16B
//   attrib("SpawnTime")          == 0xBF2FDB5C
// ---------------------------------------------------------------------------
namespace detail {

// The lookup2 mix step. Operates on three accumulators in place.
constexpr void lookup2_mix(std::uint32_t& a, std::uint32_t& b,
                           std::uint32_t& c) noexcept {
    a -= b; a -= c; a ^= (c >> 13);
    b -= c; b -= a; b ^= (a << 8);
    c -= a; c -= b; c ^= (b >> 13);
    a -= b; a -= c; a ^= (c >> 12);
    b -= c; b -= a; b ^= (a << 16);
    c -= a; c -= b; c ^= (b >> 5);
    a -= b; a -= c; a ^= (c >> 3);
    b -= c; b -= a; b ^= (a << 10);
    c -= a; c -= b; c ^= (b >> 15);
}

// One implementation over any indexable byte source (string_view or byte span),
// so the two public overloads share a single body (DRY).
template <class K>
constexpr std::uint32_t lookup2_impl(const K& k, std::size_t n,
                                     std::uint32_t seed) noexcept {
    auto byte = [&](std::size_t idx) -> std::uint32_t {
        return static_cast<std::uint32_t>(static_cast<std::uint8_t>(k[idx]));
    };
    std::uint32_t a = 0x9E3779B9u; // golden ratio
    std::uint32_t b = 0x9E3779B9u;
    std::uint32_t c = seed;
    std::size_t i = 0;
    std::size_t len = n;

    while (len >= 12) {
        a += byte(i)      | (byte(i + 1)  << 8) | (byte(i + 2)  << 16) | (byte(i + 3)  << 24);
        b += byte(i + 4)  | (byte(i + 5)  << 8) | (byte(i + 6)  << 16) | (byte(i + 7)  << 24);
        c += byte(i + 8)  | (byte(i + 9)  << 8) | (byte(i + 10) << 16) | (byte(i + 11) << 24);
        lookup2_mix(a, b, c);
        i += 12;
        len -= 12;
    }

    c += static_cast<std::uint32_t>(n);
    switch (len) {                                        // remainder, fall-through
        case 11: c += byte(i + 10) << 24; [[fallthrough]];
        case 10: c += byte(i + 9)  << 16; [[fallthrough]];
        case 9:  c += byte(i + 8)  << 8;  [[fallthrough]];
        case 8:  b += byte(i + 7)  << 24; [[fallthrough]];
        case 7:  b += byte(i + 6)  << 16; [[fallthrough]];
        case 6:  b += byte(i + 5)  << 8;  [[fallthrough]];
        case 5:  b += byte(i + 4);        [[fallthrough]];
        case 4:  a += byte(i + 3)  << 24; [[fallthrough]];
        case 3:  a += byte(i + 2)  << 16; [[fallthrough]];
        case 2:  a += byte(i + 1)  << 8;  [[fallthrough]];
        case 1:  a += byte(i);            break;
        default: break;                                   // len == 0
    }
    lookup2_mix(a, b, c);
    return c;
}

} // namespace detail

// The engine reflection seed. Every AttribStringKey uses this.
inline constexpr std::uint32_t kAttribSeed = 0xABCDEF00u;

// lookup2 over a byte range with an explicit seed.
[[nodiscard]] constexpr std::uint32_t
lookup2(std::span<const std::uint8_t> key, std::uint32_t seed = kAttribSeed) noexcept {
    return detail::lookup2_impl(key, key.size(), seed);
}

// lookup2 over a string with an explicit seed.
[[nodiscard]] constexpr std::uint32_t
lookup2(std::string_view s, std::uint32_t seed = kAttribSeed) noexcept {
    return detail::lookup2_impl(s, s.size(), seed);
}

// attrib(): the friendly name for the engine hash - lookup2 with the reflection
// seed. Use this for car names, class names, vault keys and field names.
[[nodiscard]] constexpr std::uint32_t attrib(std::string_view s) noexcept {
    return lookup2(s, kAttribSeed);
}

// User-defined literals for terse compile-time constants:
//   if (id == "wheel"_joaat) ...
//   if (key == "TopSpeedMultiplier"_attrib) ...
inline namespace literals {
[[nodiscard]] consteval std::uint32_t operator""_attrib(const char* s, std::size_t n) {
    return lookup2(std::string_view{s, n}, kAttribSeed);
}
[[nodiscard]] consteval std::uint32_t operator""_joaat(const char* s, std::size_t n) {
    return joaat(std::string_view{s, n});
}
[[nodiscard]] consteval std::uint32_t operator""_joaat_ci(const char* s, std::size_t n) {
    return joaat_ci(std::string_view{s, n});
}
[[nodiscard]] consteval std::uint32_t operator""_bin(const char* s, std::size_t n) {
    return bin(std::string_view{s, n});
}
} // namespace literals

} // namespace mwsdk::hash

#endif // MWSDK_HASHING_HPP
