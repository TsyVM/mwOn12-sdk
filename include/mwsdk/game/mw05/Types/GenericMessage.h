// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/GenericMessage.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_GENERICMESSAGE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_GENERICMESSAGE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/HudElement.h"
#include "mwsdk/game/mw05/Types/IGenericMessage.h"

namespace mwsdk::mw05::rt {
  struct GenericMessage : HudElement, IGenericMessage {
    FEObject*     mpMessageFirstLine;
    FEObject*     mpMessageSecondLine;
    FEObject*     mpIcon;
    Priority      mPriority;
    std::uint32_t mNumFramesPlayed;
    char          mStringBuffer[64];
    std::uint32_t mFengHash;
    bool          mPlayOneFrame;

    virtual ~GenericMessage();
    virtual void     RequestGenericMessage(char const* text, bool playOneFrame, Attrib::StringKey fengHash,
                                           std::uint32_t iconHash, Attrib::StringKey iconDisplayKey,
                                           Priority priority) override;
    virtual void     RequestGenericMessageZoomOut(std::uint32_t) override;
    virtual bool     IsGenericMessageShowing() override;
    virtual Priority GetCurrentGenericMessagePriority() override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_GENERICMESSAGE_H
