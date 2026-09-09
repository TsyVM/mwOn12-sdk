// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Layouts/camerainfolayout.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_CAMERAINFOLAYOUT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_CAMERAINFOLAYOUT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include <iterator>

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib/Private.h"

namespace mwsdk::mw05::rt::Attrib::Layouts {
  struct camerainfolayout {
    Private     _Array_STIFFNESS;
    float       STIFFNESS[2];
    Private     _Array_ANGLE;
    float       ANGLE[2];
    Private     _Array_LAG;
    float       LAG[2];
    Private     _Array_FOV;
    float       FOV[2];
    Private     _Array_HEIGHT;
    float       HEIGHT[2];
    Private     _Array_LATEOFFSET;
    float       LATEOFFSET[2];
    const char* CollectionName;
    Private     _Array_TILTING;
    bool        TILTING[2];
    Private     _Array_SELECTABLE;
    bool        SELECTABLE[2];

    void operator=(const camerainfolayout& rhs) {
      std::copy(std::cbegin(rhs.STIFFNESS), std::cend(rhs.STIFFNESS), std::begin(this->STIFFNESS));
      std::copy(std::cbegin(rhs.ANGLE), std::cend(rhs.ANGLE), std::begin(this->ANGLE));
      std::copy(std::cbegin(rhs.LAG), std::cend(rhs.LAG), std::begin(this->LAG));
      std::copy(std::cbegin(rhs.FOV), std::cend(rhs.FOV), std::begin(this->FOV));
      std::copy(std::cbegin(rhs.HEIGHT), std::cend(rhs.HEIGHT), std::begin(this->HEIGHT));
      std::copy(std::cbegin(rhs.LATEOFFSET), std::cend(rhs.LATEOFFSET), std::begin(this->LATEOFFSET));
      std::copy(std::cbegin(rhs.TILTING), std::cend(rhs.TILTING), std::begin(this->TILTING));
      std::copy(std::cbegin(rhs.SELECTABLE), std::cend(rhs.SELECTABLE), std::begin(this->SELECTABLE));
    }
  };
}  // namespace mwsdk::mw05::rt::Attrib::Layouts

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_CAMERAINFOLAYOUT_H
