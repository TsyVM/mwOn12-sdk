// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/UTL.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_UTL_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_UTL_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/eastl.hpp"

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL/FixedVector.h"

namespace mwsdk::mw05::rt::UTL {
  template <typename T, std::size_t nT>
  struct GarbageNode {
    struct Collector {
      struct _Node {
        T*           myptr;
        std::int32_t refcount;
      };
      UTL::FixedVector<GarbageNode<T, nT>::Collector::_Node, nT> _mDirty;
      UTL::FixedVector<GarbageNode<T, nT>::Collector::_Node, nT> _mClean;
      std::uint32_t                                              _mCount;
    };

    bool mDirty;
  };

  template <typename HandleType, typename T, std::size_t nMaxInstances>
  struct Instanceable {
    HandleType _mHandle;
  };

  namespace COM {
    struct Object {
      struct _IPair {
        struct _Finder {
          const IUnknown* ref;
        };

        void*     handle;
        IUnknown* ref;
      };
      struct _IList : eastl::vector<UTL::COM::Object::_IPair> {
        template <typename T>
        T* Find() {
          return reinterpret_cast<T*(__thiscall*)(_IList*, IHandle*)>(0x5D59F0)(this, T::GetIHandle());
        }
      };

      _IList _mInterfaces;
    };

    struct IUnknown {
      Object* _mCOMObject;

      virtual ~IUnknown();
    };
  }  // namespace COM
}  // namespace mwsdk::mw05::rt::UTL

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_UTL_H
