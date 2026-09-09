// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/UTL/FixedVector.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_UTL_FIXEDVECTOR_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_UTL_FIXEDVECTOR_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL/Vector.h"

namespace mwsdk::mw05::rt::UTL {
  template <typename T, std::size_t nT, std::size_t VectorCapacity>
  struct FixedVector : Vector<T, VectorCapacity> {
    T mVectorSpace[nT];

    virtual ~FixedVector() {}
    virtual T*          AllocVectorSpace() { return mVectorSpace; }
    virtual void        FreeVectorSpace() { std::memset(mVectorSpace, 0, sizeof(T) * nT); }
    virtual std::size_t GetGrowSize() { return nT; }
    virtual std::size_t GetMaxCapacity() { return nT; }
    virtual void        OnGrowRequest() {}

    FixedVector() : Vector<T, nT>() { this->mBegin = mVectorSpace; }
  };
}  // namespace mwsdk::mw05::rt::UTL

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_UTL_FIXEDVECTOR_H
