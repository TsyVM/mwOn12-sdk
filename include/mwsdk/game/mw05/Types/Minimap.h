// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Minimap.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_MINIMAP_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_MINIMAP_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/HudElement.h"
#include "mwsdk/game/mw05/Types/MinimapItem.h"

namespace mwsdk::mw05::rt {
  struct Minimap : HudElement {
    bTList<MiniMapItem> StaticMiniMapItems;
    FEObject*           TrackmapLayout;
    FEMultiImage*       TrackmapArt[4];
    FEVector2           TrackmapArtUVs[4][2];
    FEImage*            TrackmapNorth;
    FEImage*            mPlayerCarIndicator;
    FEImage*            mPlayerCarIndicator2;
    TrackInfo*          CurrentTrack;
    FEVector3           mMapDefaultPos;
    float               mSpeedZoomScale;
    float               mPolyRotation;
    bVector2            mTrackTargetNormalized;
    Math::Vector2       mTrackMapCentre;
    std::int32_t        mCopFlashCounter;
    bool                MinimapRotateWithPlayer;
    FEObject*           mHeliElementArt;
    FEObject*           mHeliLineOfSiteArt;
    FEImage*            mCopElementArt[8];
    FEImage*            mRacerElementArt[8];
    FEImage*            mCheckpointElementArt;
    FEImage*            mGPSSelectionElementArt;
    FEImage*            mGameplayIcons[17][8];
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_MINIMAP_H
