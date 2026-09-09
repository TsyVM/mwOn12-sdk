// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Collection.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_COLLECTION_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_COLLECTION_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/memory.hpp"

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib/HashMap.h"

namespace mwsdk::mw05::rt::Attrib {
  struct Collection {
    HashMap       mTable;
    Collection*   mParent;
    Class*        mClass;
    void*         mLayout;
    std::uint32_t mRefCount;
    std::uint32_t mKey;
    Vault*        mSource;
    const char**  mNamePtr;

    template <typename T>
    inline T* GetData(std::uint32_t fieldKey, std::int32_t idx = 0) {
      return reinterpret_cast<T*(__thiscall*)(Collection*, std::uint32_t, std::int32_t)>(0x454190)(this, fieldKey, idx);
    }
    template <typename T>
    inline T* GetData(const char* fieldName, std::int32_t idx = 0) {
      return GetData<T>(StringToKey(fieldName), idx);
    }

    template <typename T>
    inline T* GetLayout() {
      return reinterpret_cast<T*>(mLayout);
    }

    template <typename T>
    inline bool TrySetData(std::uint32_t fieldKey, T value, bool safe = true, std::int32_t idx = 0) {
      auto* p = GetData<T>(fieldKey, idx);
      if (p) {
        if (safe)
          MemoryEditor::Get().GetRawMemory(p).SetValue(value);
        else
          *p = value;
      }
      return p != nullptr;
    }
    template <typename T>
    inline bool TrySetData(const char* fieldName, T value, bool safe = true, std::int32_t idx = 0) {
      return TrySetData(StringToKey(fieldName), value, safe, idx);
    }
  };
}  // namespace mwsdk::mw05::rt::Attrib

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_COLLECTION_H
