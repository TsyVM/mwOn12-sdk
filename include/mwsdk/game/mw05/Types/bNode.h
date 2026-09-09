// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/bNode.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_BNODE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_BNODE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt {
  class bNode {
   protected:
    bNode* Next;
    bNode* Prev;

   public:
    auto* GetNext() { return Next; }
    auto* GetPrev() { return Prev; }
    auto* Remove() {
      if (this->Prev) this->Prev->Next = this->Next;
      if (this->Next) this->Next->Prev = this->Prev;
      return this;
    }
    auto* AddAfter(bNode* node) {
      node->Next = this->Next;
      node->Prev = this;
      this->Next = node;
      return this;
    }
    auto* AddBefore(bNode* node) {
      node->Prev = this->Prev;
      node->Next = this;
      this->Prev = node;
      return this;
    }

    bNode() = default;
    ~bNode() {
      Remove();
      // free(this);
    }
  };

  template <typename T>
  class bTNode {
   protected:
    bTNode<T>* Next;
    bTNode<T>* Prev;

   public:
    auto* GetNext() { return Next; }
    auto* GetPrev() { return Prev; }
    auto* Remove() {
      if (this->Prev) this->Prev->Next = this->Next;
      if (this->Next) this->Next->Prev = this->Prev;
      return this;
    }
    auto* AddAfter(T* node) {
      node->Next = this->Next;
      node->Prev = this;
      this->Next = node;
      return this;
    }
    auto* AddBefore(T* node) {
      node->Prev = this->Prev;
      node->Next = this;
      this->Prev = node;
      return this;
    }

    bTNode() = default;
    ~bTNode() {
      Remove();
      // free(this);
    }
  };

  struct bPNode : bTNode<bPNode> {
    void* Object;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_BNODE_H
