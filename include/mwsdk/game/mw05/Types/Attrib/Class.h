// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Class.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_CLASS_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_CLASS_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::Attrib {
  struct Class {
    std::uint32_t mKey;
    std::uint32_t mRefCount;
    ClassPrivate& mPrivates;

    inline Definition* GetDefinition(std::uint32_t key) {
      return reinterpret_cast<Definition*(__thiscall*)(Class*, std::uint32_t)>(0x457380)(this, key);
    }
    inline std::uint32_t GetNumDefinitions() {
      return reinterpret_cast<std::uint32_t(__thiscall*)(Class*)>(0x451660)(this);
    }
    inline std::uint32_t GetFirstDefinition() {
      return reinterpret_cast<std::uint32_t(__thiscall*)(Class*)>(0x451670)(this);
    }
    inline std::uint32_t GetNextDefinition(std::uint32_t key) {
      return reinterpret_cast<std::uint32_t(__thiscall*)(Class*, std::uint32_t)>(0x4573C0)(this, key);
    }

    inline Collection* GetCollection(std::uint32_t key) {
      return reinterpret_cast<Collection*(__thiscall*)(Class*, std::uint32_t)>(0x455960)(this, key);
    }
    inline std::uint32_t GetNumCollections() {
      return reinterpret_cast<std::uint32_t(__thiscall*)(Class*)>(0x453FC0)(this);
    }
    inline std::uint32_t GetFirstCollection() {
      return reinterpret_cast<std::uint32_t(__thiscall*)(Class*)>(0x456B00)(this);
    }
    inline std::uint32_t GetNextCollection(std::uint32_t key) {
      return reinterpret_cast<std::uint32_t(__thiscall*)(Class*, std::uint32_t)>(0x456B20)(this, key);
    }
  };
}  // namespace mwsdk::mw05::rt::Attrib

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_CLASS_H
