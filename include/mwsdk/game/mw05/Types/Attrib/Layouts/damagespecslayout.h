// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Layouts/damagespecslayout.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_DAMAGESPECSLAYOUT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_DAMAGESPECSLAYOUT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::Attrib::Layouts {
  struct damagespecslayout {
    struct DamageScaleRecord {
      float VisualScale;
      float HitPointScale;
    };

    DamageScaleRecord DZ_LFRONT;
    DamageScaleRecord DZ_FRONT;
    DamageScaleRecord DZ_BOTTOM;
    DamageScaleRecord DZ_LEFT;
    DamageScaleRecord DZ_RREAR;
    DamageScaleRecord DZ_LREAR;
    DamageScaleRecord DZ_RIGHT;
    DamageScaleRecord DZ_REAR;
    DamageScaleRecord DZ_TOP;
    DamageScaleRecord DZ_RFRONT;
    float             SHOCK_TIME;
    float             HP_THRESHOLD;
    float             SUPPRESS_DIST;
    float             SHOCK_FORCE;
    float             FORCE;
    float             HIT_POINTS;
  };
}  // namespace mwsdk::mw05::rt::Attrib::Layouts

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_DAMAGESPECSLAYOUT_H
