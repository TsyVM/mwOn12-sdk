// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/ITransmission.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ITRANSMISSION_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ITRANSMISSION_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct ITransmission : UTL::COM::IUnknown {
    enum class GearID : std::uint32_t { Reverse, Neutral, First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth };
    enum class ShiftPotential : std::uint32_t {
      None,
      Down,
      Up,
      Good,
      Perfect,
      Miss,
    };
    enum class ShiftStatus : std::uint32_t {
      None,
      Normal,
      Good,
      Perfect,
      Missed,
    };

    virtual ~ITransmission();
    virtual GearID         GetGear()                             = 0;
    virtual GearID         GetTopGear()                          = 0;
    virtual bool           Shift(GearID gear)                    = 0;
    virtual bool           IsGearChanging()                      = 0;
    virtual bool           IsReversing()                         = 0;
    virtual float          GetSpeedometer()                      = 0;
    virtual float          GetMaxSpeedometer()                   = 0;
    virtual float          GetDriveTorque()                      = 0;
    virtual float          GetOptimalShiftRange(GearID gear)     = 0;
    virtual float          GetShiftPoint(GearID from, GearID to) = 0;
    virtual ShiftStatus    GetShiftStatus()                      = 0;
    virtual ShiftPotential GetShiftPotential()                   = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x404010)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ITRANSMISSION_H
