// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IGenericMessage.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IGENERICMESSAGE_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IGENERICMESSAGE_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IGenericMessage : UTL::COM::IUnknown {
    enum class FEngTypes : std::uint32_t {
      None,
      Type1 = 0x8AB83EDB,  // [White Text] Fades out towards bottom of the screen
      Type2 = 0x9D73BC15,  // [White Text] Fades out towards player camera
      Type3 = 0x5230FAF6,  // [Red Text] Fades out towards top of the screen
      Type4 = 0xA19BB14C,  // [Green Text] Fades out towards bottom of the screen
      Type5 = 0x821E6378   // [Green/White Text] Flashes and is kept alive
    };
    enum class IconDisplayTypes : std::uint32_t {
      HideIcon,
      ShowIcon = 0x609F6B15,  // [Flashing background] Fades out towards bottom of the screen
    };
    enum class Priority : std::uint32_t { Priority5, Priority4, Priority3, Priority2, Priority1 };

    virtual ~IGenericMessage();
    virtual void     RequestGenericMessage(char const* text, bool playOneFrame, Attrib::StringKey fengHash,
                                           std::uint32_t iconHash, Attrib::StringKey iconDisplayKey, Priority priority) = 0;
    virtual void     RequestGenericMessageZoomOut(std::uint32_t) = 0;
    virtual bool     IsGenericMessageShowing()                   = 0;
    virtual Priority GetCurrentGenericMessagePriority()          = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x5650B0)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IGENERICMESSAGE_H
