// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/all.hpp
// ---------------------------------------------------------------------------
// Opt-in umbrella for the reconstructed mw05::rt reference layer: every live
// game object/type layout, the fixed-address global "Variables", and the
// convenience extension helpers. Include this ONLY when you want the full
// community-reconstructed type surface; it is intentionally NOT pulled in by
// <mwsdk/mwsdk.hpp> so the verified, cross-platform core stays lean.
//
// Everything here lives in namespace mwsdk::mw05::rt and is [reference]-tier.
// See README.md in this folder for the verified-vs-reference split.
//
// These headers reconstruct 32-bit MSVC game structs; a handful carry benign
// MSVC-era constructs (implicit-copy, member init order) that strict GCC/Clang
// flag. We locally quiet those *for this reference layer only* so opt-in users
// building with -Werror aren't blocked; the verified core is unaffected.
// ---------------------------------------------------------------------------
#ifndef MWSDK_MW05_RT_ALL_HPP
#define MWSDK_MW05_RT_ALL_HPP

#if defined(__GNUC__) || defined(__clang__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-copy"
#  pragma GCC diagnostic ignored "-Wreorder"
#  pragma GCC diagnostic ignored "-Wconversion-null"
#  pragma GCC diagnostic ignored "-Wuninitialized"
#  pragma GCC diagnostic ignored "-Wignored-qualifiers"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "mwsdk/game/mw05/all_types.hpp"   // every reconstructed type layout
#include "mwsdk/game/mw05/MW05.h"          // fixed-address global Variables
#include "mwsdk/game/mw05/Extensions.h"    // ergonomic per-type helpers
#include "mwsdk/game/mw05/host.hpp"        // fixed host/window/D3D9/DInput8 slots

#if defined(__GNUC__) || defined(__clang__)
#  pragma GCC diagnostic pop
#endif

#endif  // MWSDK_MW05_RT_ALL_HPP
