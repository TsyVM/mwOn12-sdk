// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Definition.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_DEFINITION_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_DEFINITION_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::Attrib {
  struct Definition {
    enum class Flags : std::uint8_t {
      Array           = 1 << 0,
      InLayout        = 1 << 1,
      IsBound         = 1 << 2,
      IsNotSearchable = 1 << 3,
    };

    std::uint32_t mKey;
    std::uint32_t mType;
    std::uint16_t mOffset;
    std::uint16_t mSize;
    std::uint16_t mMaxCount;
    Flags         mFlags;
    std::uint8_t  mAlignment;
  };

#if 1  // portable flag operators
  DEFINE_ENUM_FLAG_OPERATORS(Definition::Flags)
#endif
}  // namespace mwsdk::mw05::rt::Attrib

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_DEFINITION_H
