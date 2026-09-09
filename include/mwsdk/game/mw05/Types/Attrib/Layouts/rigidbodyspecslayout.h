// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Layouts/rigidbodyspecslayout.h
// ---------------------------------------------------------------------------
// Reconstructed live-object layout for Need for Speed: Most Wanted (2005),
// speed.exe v1.3. Community-reverse-engineered [reference]-confidence data,
// re-expressed in MWSDK form under namespace mwsdk::mw05::rt.
//
// NOT part of MWSDK's verified core tables (mw05_db.inl / mw05_layouts.inl).
// Member offsets are implied by declaration order and depend on target ABI;
// treat addresses/offsets here as leads to confirm, not ground truth. See
// mwsdk/game/mw05/README.md for provenance and the verified/reference split.
// ---------------------------------------------------------------------------

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_RIGIDBODYSPECSLAYOUT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_RIGIDBODYSPECSLAYOUT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib/Private.h"

namespace mwsdk::mw05::rt::Attrib::Layouts {
  struct rigidbodyspecslayout {
    Math::Vector4 COLLISION_BOX_PAD;
    Math::Vector4 DRAG;
    Math::Vector4 WORLD_MOMENT_SCALE;
    Math::Vector4 OBJ_MOMENT_SCALE;
    Math::Vector4 GROUND_ELASTICITY;
    Math::Vector4 OBJ_ELASTICITY;
    Math::Vector4 DRAG_ANGULAR;
    Math::Vector4 WALL_ELASTICITY;
    Math::Vector4 GROUND_MOMENT_SCALE;
    Math::Vector4 CG;
    StringKey     BASE_MATERIAL;
    StringKey     DEFAULT_COL_BOX;
    Private       _Array_OBJ_FRICTION;
    Math::Vector2 OBJ_FRICTION;
    Private       _Array_WALL_FRICTION;
    Math::Vector2 WALL_FRICTION;
    Private       _Array_GROUND_FRICTION;
    Math::Vector2 GROUND_FRICTION;
    float         GRAVITY;
    float         NATURAL_ANGULAR_DAMPING;
    float         SLEEP_VELOCITY;
    bool          NO_GROUND_COLLISIONS;
    bool          IMMOBILE_OBJECT_COLLISIONS;
    bool          NO_WORLD_COLLISIONS;
    bool          INSTANCE_COLLISIONS_3D;
    bool          NO_OBJ_COLLISIONS;
  };
}  // namespace mwsdk::mw05::rt::Attrib::Layouts

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_RIGIDBODYSPECSLAYOUT_H
