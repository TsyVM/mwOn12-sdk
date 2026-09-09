// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Layouts/pvehiclelayout.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_PVEHICLELAYOUT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_PVEHICLELAYOUT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::Attrib::Layouts {
  struct pvehiclelayout {
    enum class CarType : std::uint32_t {
      McLaren     = 1 << 0,
      Porsche     = 1 << 1,
      FordGT      = 1 << 2,
      Viper       = 1 << 3,
      AstonMartin = 1 << 4,
      Corvette    = 1 << 5,
      BMW         = 1 << 6,
      Mercedes    = 1 << 7,
      Audi        = 1 << 8,
      Mitsubishi  = 1 << 9,
      Mustang     = 1 << 10,
      Lotus       = 1 << 11,
      Subaru      = 1 << 12,
      Camaro      = 1 << 13,
      GTO         = 1 << 14,
      Nissan      = 1 << 15,
      Mazda       = 1 << 16,
      Renault     = 1 << 17,
      Lexus       = 1 << 18,
      Mini        = 1 << 19,
      Volkswagen  = 1 << 20,
      SUV         = 1 << 21,
      Pickup      = 1 << 22,
      Pagani      = 1 << 23,
      Lamborghini = 1 << 24,
      Chrysler    = 1 << 25,
      Opel        = 1 << 26,
      Sedan       = 1 << 27,
      SportsCar   = 1 << 28,
      Supra       = 1 << 29,
      Cadillac    = 1 << 30
    };

    Math::Vector4 TENSOR_SCALE;
    StringKey     MODEL;
    const char*   DefaultPresetRide;
    const char*   CollectionName;
    std::int32_t  engine_upgrades;
    std::int32_t  transmission_upgrades;
    std::int32_t  nos_upgrades;
    std::int32_t  brakes_upgrades;
    float         MASS;
    std::int32_t  tires_upgrades;
    CarType       VerbalType;
    std::int32_t  induction_upgrades;
    std::int32_t  chassis_upgrades;
    std::uint8_t  HornType;
    std::uint8_t  TrafficEngType;
  };

#if 1  // portable flag operators
  DEFINE_ENUM_FLAG_OPERATORS(pvehiclelayout::CarType)
#endif
}  // namespace mwsdk::mw05::rt::Attrib::Layouts

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_PVEHICLELAYOUT_H
