// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05_reflection.hpp
// ------------------------------
// Runtime entry points into speed.exe's reflection layer - the machinery that
// turns names into keys and keys into live attribute storage. Where
// mwsdk/hashing.hpp reproduces the hash *offline*, these are the *in-process*
// functions and globals, so a live plugin can hash a name exactly as the engine
// does, resolve an attribute, or walk the class registry.
//
// Every address is a verified speed.exe v1.3 VA (preferred base 0x00400000);
// rebase through mw05::Image under ASLR. Curated (not generated) - each carries
// its grounding inline. Companion offline data: mwsdk/reflection.hpp.
//
// Grounding: The Most Wanted Encyclopedia, Chapter 12 (Reflection Schema),
// Chapter 2 (Identifiers & Hashing), Chapter 33 (Class Registry & Factories).

#ifndef MWSDK_GAME_MW05_REFLECTION_HPP
#define MWSDK_GAME_MW05_REFLECTION_HPP

#include <cstdint>

namespace mwsdk::mw05::reflection {

using Va = std::uint32_t;

// The reflection hash seed (Jenkins lookup2). hash("default") == 0xEEC2271A.
inline constexpr std::uint32_t kSeed = 0xABCDEF00u;

// --- code anchors -----------------------------------------------------------
// Reflection::Hash(const char* name) -> u32. cdecl. The one function every
// class/field/attribute name flows through; tail-calls the lookup2 core with
// seed 0xABCDEF00. 686 call-sites. (C2 / C12.1)
inline constexpr Va kReflectionHash = 0x005CC240u;

// lookup2(data, len, seed) core mixer (golden-ratio const 0x9E3779B9). The
// primitive kReflectionHash tail-calls; useful for hashing arbitrary buffers.
inline constexpr Va kLookup2Core = 0x005CC090u;

// AttribCollection::GetField(self, keyHash, index) -> void* storage. thiscall.
// The single primitive every vault getter funnels through - the live
// read/write attribute mechanism (C12.2 / Discovery 35). Also mw05::fn.
inline constexpr Va kAttribGetField = 0x00454810u;

// The attribute resolver on the lookup path: Lookup(collection, key)->attribute*
// (binary search over the per-collection sorted {key,value} array). thiscall.
inline constexpr Va kAttribResolve = 0x005D59F0u;

// --- data anchors -----------------------------------------------------------
// Head of the global class-registration singly-linked list. Each node is
// {u32 nameHash, u32 factoryPtr, u32 next}; every reflection class pushes one
// at static-init. Walk it to enumerate (classNameHash, factory). (C33)
inline constexpr Va kClassRegistryHead = 0x0092C660u;

// The inline class-name string table (28 NUL-padded names): the expected class
// set / coverage denominator. (C33)
inline constexpr Va kClassNameTable = 0x008ADD1Cu;

} // namespace mwsdk::mw05::reflection

#endif // MWSDK_GAME_MW05_REFLECTION_HPP
