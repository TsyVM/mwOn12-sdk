// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/bList.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_BLIST_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_BLIST_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/bNode.h"

namespace mwsdk::mw05::rt {
  struct bList {
    bNode HeadNode;

    bList() = default;
    ~bList() {}
  };

  template <class T>
  struct bTList {
    bTNode<T> HeadNode;

    bTList() = default;
    ~bTList() {}
  };

  template <class T>
  struct bPList : bTList<bPNode> {
    bPNode* AddHead(T*);
    bPNode* AddTail(T*);
    bPNode* AddBefore(bNode*, T*);
    bPNode* AddAfter(bNode*, T*);

    bPList() = default;
    ~bPList() {}
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_BLIST_H
