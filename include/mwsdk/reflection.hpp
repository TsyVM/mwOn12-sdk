// SPDX-License-Identifier: MIT
//
// mwsdk/reflection.hpp
// --------------------
// The engine's reflection primitive-type table: the 15 leaf value types every
// attribute field ultimately is (Bool, Float, Int32, UInt8, Text, ...), each
// with its verified type hash and byte width. Pair it with the attribute vault
// (mwsdk/vault.hpp): a decoded record's element type resolves through here to a
// concrete width so a value can be read with the right size.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 12 (Reflection Schema &
// Resolved-Value Model); values verified from the engine's reflection type
// registry (reflection_types.json). Offline / pure / constexpr.

#ifndef MWSDK_REFLECTION_HPP
#define MWSDK_REFLECTION_HPP

#include <cstdint>
#include <string_view>

#include "mwsdk/reflection_names.inl" // generated live-intern name dictionary

namespace mwsdk::reflection {

// One reflection primitive type: engine name, its type hash, and byte width
// (0 = variable-length, e.g. Text / RefSpec).
struct Type {
    std::string_view name;
    std::uint32_t    hash;
    std::uint8_t     size; // bytes; 0 for variable-length
};

// The verified reflection primitive types (reflection_types.json).
inline constexpr Type kTypes[] = {
    {"Bool",             0x064BEC37u, 1},
    {"Char",             0x019C17B0u, 1},
    {"Double",           0x9ABBEF67u, 8},
    {"Float",            0x3C16EC5Eu, 4},
    {"Int8",             0x6F27B5BCu, 1},
    {"Int16",            0x391C6E95u, 2},
    {"Int32",            0x5763DA41u, 4},
    {"Int64",            0x935FF2F8u, 8},
    {"UInt8",            0x671ECBE2u, 1},
    {"UInt16",           0xE51A99C1u, 2},
    {"UInt32",           0x939992BBu, 4},
    {"UInt64",           0xADEC51AAu, 8},
    {"Text",             0xA3F0C234u, 0},
    {"Attrib::StringKey",0xA502A824u, 4},
    {"Attrib::RefSpec",  0x2B936EB7u, 0},
};
inline constexpr std::size_t kTypeCount = sizeof(kTypes) / sizeof(kTypes[0]);

// Look a type up by its reflection type hash. Returns nullptr if unknown.
[[nodiscard]] constexpr const Type* by_hash(std::uint32_t hash) noexcept {
    for (const Type& t : kTypes) if (t.hash == hash) return &t;
    return nullptr;
}

// Look a type up by its (short) engine name, e.g. "Float". nullptr if unknown.
[[nodiscard]] constexpr const Type* by_name(std::string_view name) noexcept {
    for (const Type& t : kTypes) if (t.name == name) return &t;
    return nullptr;
}

// Byte width for a type hash (0 if unknown or variable-length).
[[nodiscard]] constexpr std::uint8_t size_of(std::uint32_t hash) noexcept {
    const Type* t = by_hash(hash);
    return t ? t->size : 0;
}


// Reverse-resolve a runtime reflection hash (class / message / enum / collection
// key / model name) to its name via the verified live-intern dictionary. "" if
// unknown. Complements vault::field_name (field keys) and the class table.
[[nodiscard]] inline std::string_view runtime_name(std::uint32_t hash) noexcept {
    std::size_t lo = 0, hi = kRuntimeNameCount;
    while (lo < hi) {
        std::size_t m = lo + (hi - lo) / 2;
        if (kRuntimeNames[m].hash < hash) lo = m + 1; else hi = m;
    }
    if (lo < kRuntimeNameCount && kRuntimeNames[lo].hash == hash) return kRuntimeNames[lo].name;
    return {};
}

} // namespace mwsdk::reflection

#endif // MWSDK_REFLECTION_HPP
