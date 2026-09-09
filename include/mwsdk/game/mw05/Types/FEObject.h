// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/FEObject.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_FEOBJECT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_FEOBJECT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/FEMinList.h"

namespace mwsdk::mw05::rt {
  struct FEObject : FEMinNode {
    std::uint_least32_t GUID;
    std::uint_least32_t NameHash;
    char*               pName;
    FEObjType           Type;
    std::uint_least32_t Flags;
    std::uint_least16_t RenderContext;
    std::uint_least16_t ResourceIndex;
    std::uint_least32_t Handle;
    std::uint_least32_t UserParam;
    void*               pData;
    std::uint_least32_t DataSize;
    FEMinList           Responses;
    FEMinList           Scripts;
    FEScript*           pCurrentScript;
    FERenderObject*     Cached;

    /// VISIBLE

    void SetVisible(bool visible) { reinterpret_cast<void(__cdecl*)(FEObject*)>(visible ? 0x514CC0 : 0x514C70)(this); }

    /// COLOR

    void SetColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
      reinterpret_cast<void(__cdecl*)(FEObject*, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t)>(0x5157E0)(
          this, b, g, r, a);
    }

    /// CENTER

    FEVector2 GetCenter() {
      FEVector2 ret;
      reinterpret_cast<void(__cdecl*)(FEObject*, float*, float*)>(0x524EE0)(this, &ret.x, &ret.y);

      return ret;
    }
    void SetCenter(FEVector2 center) {
      reinterpret_cast<void(__cdecl*)(FEObject*, float, float)>(0x525050)(this, center.x, center.y);
    }

    /// SIZE

    FEVector2 GetSize() {
      FEVector2 ret;
      reinterpret_cast<void(__cdecl*)(FEObject*, float*, float*)>(0x515520)(this, &ret.x, &ret.y);

      return ret;
    }
    void SetSize(FEVector2 size) {
      reinterpret_cast<void(__cdecl*)(FEObject*, float, float)>(0x5155E0)(this, size.x, size.y);
    }

    /// ROTATION

    void SetRotationZ(float rot) { reinterpret_cast<void(__cdecl*)(FEObject*, float)>(0x514E20)(this, rot); }

    /// GET INSTANCE

    static inline FEObject* GetObject(const char* fng, std::uint32_t hash) {
      return reinterpret_cast<FEObject*(__cdecl*)(const char*, std::uint32_t)>(0x524850)(fng, hash);
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_FEOBJECT_H
