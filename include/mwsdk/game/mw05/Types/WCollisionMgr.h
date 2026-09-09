// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/WCollisionMgr.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_WCOLLISIONMGR_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_WCOLLISIONMGR_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::WCollisionMgr {
  struct WorldCollisionInfo {
    UMath::Vector4                 fCollidePt;
    UMath::Vector4                 fNormal;
    float                          fDist;
    bool                           fAnimated;
    bool                           fType;
    std::uint16_t                  fPad;
    void*                          fCInst;
    Dynamics::Collision::Geometry* pWorldGeom;
    Dynamics::Collision::Geometry* pUserGeom;
    UTL::COM::IUnknown*            pUnk;
    unsigned char                  __PADDING__[8];
  };

  struct ICollisionHandler {
    virtual bool OnWCollide(const WCollisionMgr::WorldCollisionInfo&, const UMath::Vector3&, void*) = 0;
  };
}  // namespace mwsdk::mw05::rt::WCollisionMgr

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_WCOLLISIONMGR_H
