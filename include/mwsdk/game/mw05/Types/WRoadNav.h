// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/WRoadNav.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_WROADNAV_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_WROADNAV_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/USpline.h"

namespace mwsdk::mw05::rt {
  struct WRoadNav {
    template <typename T, std::int32_t N = 32>
    struct CookieTrail {
      std::int32_t       mCount;
      std::int32_t       mLast;
      const std::int32_t mCapacity;
      std::int32_t       pad;
      T                  mData[N];
    };
    struct NavCookie {
      enum class eFlags : std::uint32_t { CutByObject = 1, CutFromBehind = 2 };

      UMath::Vector2 Left;
      UMath::Vector2 Right;
      UMath::Vector2 Forward;
      float          Length;
      float          Curvature;
      float          LeftOffset;
      float          RightOffset;
      eFlags         Flags;
      float          Padding;
      UMath::Vector3 Centre;
      std::int16_t   SegmentParameter;
      std::uint16_t  SegmentNumber : 15;
      std::uint16_t  SegmentNodeInd : 1;
    };

    std::int32_t                nCookieIndex;
    CookieTrail<NavCookie, 32>* pCookieTrail;
    NavCookie                   mCurrentCookie;
    float                       mOutOfBounds;
    bool                        fValid;
    bool                        bRaceFilter;
    bool                        bTrafficFilter;
    bool                        bCopFilter;
    bool                        bDecisionFilter;
    bool                        bCookieTrail;
    std::int32_t                nRoadOcclusion;
    std::int32_t                nAvoidableOcclusion;
    bool                        bOccludedFromBehind;
    float                       fOccludingTrailSpeed;
    Math::Vector2               vCookieTrailBoxMin;
    Math::Vector2               vCookieTrailBoxMax;
    eNavType                    fNavType;
    ePathType                   fPathType;
    eLaneType                   fLaneType;
    AIVehicle*                  pAIVehicle;
    float                       fVehicleHalfWidth;
    std::int8_t                 fNodeInd;
    std::int16_t                fSegmentInd;
    float                       fSegTime;
    float                       fCurvature;
    UMath::Vector3              fPosition;
    UMath::Vector3              fLeftPosition;
    UMath::Vector3              fRightPosition;
    UMath::Vector3              fForwardVector;
    UMath::Vector3              fEndPos;
    UMath::Vector3              fStartPos;
    UMath::Vector3              fEndControl;
    UMath::Vector3              fStartControl;
    UMath::Vector3              fLeftEndPos;
    UMath::Vector3              fLeftStartPos;
    UMath::Vector3              fLeftEndControl;
    UMath::Vector3              fLeftStartControl;
    UMath::Vector3              fRightEndPos;
    UMath::Vector3              fRightStartPos;
    UMath::Vector3              fRightEndControl;
    UMath::Vector3              fRightStartControl;
    UMath::Vector3              fApexPosition;
    UMath::Vector3              fOccludedPosition;
    USpline                     fRoadSpline;
    USpline                     fLeftSpline;
    USpline                     fRightSpline;
    std::int8_t                 fDeadEnd;
    std::int8_t                 fLaneInd;
    std::int8_t                 fFromLaneInd;
    std::int8_t                 fToLaneInd;
    float                       fLaneOffset;
    float                       fFromLaneOffset;
    float                       fToLaneOffset;
    float                       fLaneChangeDist;
    float                       fLaneChangeInc;
    bool                        bCrossedPathGoal;
    std::uint8_t                nPathGoalSegment;
    std::uint16_t               fPathGoalParam;

    virtual ~WRoadNav();
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_WROADNAV_H
