// SPDX-License-Identifier: MIT
//
// mwsdk/mwsdk.hpp
// ===============
// MWSDK — the Need for Speed: Most Wanted (2005) Modding SDK Framework.
//
// One umbrella header for the whole toolkit. Include this and you get every
// module. Each module is also includable on its own if you want a smaller
// surface.
//
//   #include <mwsdk/mwsdk.hpp>
//   using namespace mwsdk;
//
// MWSDK is grounded, module for module, in The Most Wanted Encyclopedia — a
// verified, byte-level reference for the retail PC (v1.3) data set. Nothing
// here is invented: every struct layout, chunk id, hash, and (in the runtime
// module) virtual address is traceable to a chapter or appendix.
//
// Two pillars:
//   * Data SDK (offline)  — read/write the game's files. Zero dependencies.
//       core, hashing, jdlz, eagl, chunks, tpk, geometry, vault,
//       reflection, scenery, triggers, paths, collision
//   * Runtime SDK (live)  — typed access to the running speed.exe for
//       plugins/trainers. Header-only, zero forced hooking backend.
//       game (see mwsdk/game/mw05.hpp)
//
// The core framework never pulls in a third-party hooking or UI library. Any
// such integration lives under mwsdk/adapters/ and is strictly opt-in.

#ifndef MWSDK_MWSDK_HPP
#define MWSDK_MWSDK_HPP

// --- Data SDK (offline file formats) ---------------------------------------
#include "mwsdk/core.hpp"
#include "mwsdk/hashing.hpp"
#include "mwsdk/jdlz.hpp"
#include "mwsdk/eagl.hpp"
#include "mwsdk/tpk.hpp"
#include "mwsdk/geometry.hpp"
#include "mwsdk/vault.hpp"
#include "mwsdk/chunks.hpp"      // EAGL chunk-id -> handler names
#include "mwsdk/reflection.hpp"  // reflection primitive type table
#include "mwsdk/scenery.hpp"     // scenery instances + info + cull tree
#include "mwsdk/triggers.hpp"    // trigger regions + position markers
#include "mwsdk/paths.hpp"       // traffic paths + CARP road graph
#include "mwsdk/collision.hpp"   // terrain collision + smackables

// --- Runtime SDK (live game types) -----------------------------------------
// Header-only; safe to include from a plugin DLL. Windows-only internally.
#include "mwsdk/game/mw05.hpp"
#include "mwsdk/game/mw05_attrib.hpp"   // live AttribCollection read/write
#include "mwsdk/game/mw05_views.hpp"    // ergonomic typed object views
#include "mwsdk/game/mw05_easy.hpp"     // Level-1 zero-setup one-liners
#include "mwsdk/game/mw05_group.hpp"    // batch-managed reversible patches
#include "mwsdk/game/mw05_reflection.hpp" // runtime reflection-layer anchors

// MWSDK semantic version.
#define MWSDK_VERSION_MAJOR 0
#define MWSDK_VERSION_MINOR 1
#define MWSDK_VERSION_PATCH 0

namespace mwsdk {
inline constexpr int version_major = MWSDK_VERSION_MAJOR;
inline constexpr int version_minor = MWSDK_VERSION_MINOR;
inline constexpr int version_patch = MWSDK_VERSION_PATCH;
} // namespace mwsdk

#endif // MWSDK_MWSDK_HPP
