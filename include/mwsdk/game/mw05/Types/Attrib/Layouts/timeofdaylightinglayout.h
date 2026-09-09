// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/Layouts/timeofdaylightinglayout.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_TIMEOFDAYLIGHTINGLAYOUT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_TIMEOFDAYLIGHTINGLAYOUT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/Attrib/RGBA.h"

namespace mwsdk::mw05::rt::Attrib::Layouts {
  struct timeofdaylightinglayout {
    RGBA         SpecularColour;
    RGBA         DiffuseColour;
    RGBA         AmbientColour;
    RGBA         FogHazeColour;
    RGBA         FixedFunctionSkyColour;
    float        FogDistanceScale;
    float        FogHazeColourScale;
    float        FogSkyColourScale;
    float        EnvSkyBrightness;
    float        CarSpecScale;
    std::uint8_t _pad[0x4 * 3];
    RGBA         FogSkyColour;

    timeofdaylightinglayout operator+(const timeofdaylightinglayout& rhs) {
      timeofdaylightinglayout ret = *this;
      ret.SpecularColour += rhs.SpecularColour;
      ret.DiffuseColour += rhs.DiffuseColour;
      ret.AmbientColour += rhs.AmbientColour;
      ret.FogHazeColour += rhs.FogHazeColour;
      ret.FixedFunctionSkyColour += rhs.FixedFunctionSkyColour;
      ret.FogDistanceScale += rhs.FogDistanceScale;
      ret.FogSkyColourScale += rhs.FogSkyColourScale;
      ret.FogHazeColourScale += rhs.FogHazeColourScale;
      ret.EnvSkyBrightness += rhs.EnvSkyBrightness;
      ret.CarSpecScale += rhs.CarSpecScale;
      ret.FogSkyColour += rhs.FogSkyColour;

      return ret;
    }
    timeofdaylightinglayout operator-(const timeofdaylightinglayout& rhs) {
      timeofdaylightinglayout ret = *this;
      ret.SpecularColour -= rhs.SpecularColour;
      ret.DiffuseColour -= rhs.DiffuseColour;
      ret.AmbientColour -= rhs.AmbientColour;
      ret.FogHazeColour -= rhs.FogHazeColour;
      ret.FixedFunctionSkyColour -= rhs.FixedFunctionSkyColour;
      ret.FogDistanceScale -= rhs.FogDistanceScale;
      ret.FogSkyColourScale -= rhs.FogSkyColourScale;
      ret.FogHazeColourScale -= rhs.FogHazeColourScale;
      ret.EnvSkyBrightness -= rhs.EnvSkyBrightness;
      ret.CarSpecScale -= rhs.CarSpecScale;
      ret.FogSkyColour -= rhs.FogSkyColour;

      return ret;
    }
    timeofdaylightinglayout operator*(float rhs) {
      timeofdaylightinglayout ret = *this;
      ret.SpecularColour *= rhs;
      ret.DiffuseColour *= rhs;
      ret.AmbientColour *= rhs;
      ret.FogHazeColour *= rhs;
      ret.FixedFunctionSkyColour *= rhs;
      ret.FogDistanceScale *= rhs;
      ret.FogSkyColourScale *= rhs;
      ret.FogHazeColourScale *= rhs;
      ret.EnvSkyBrightness *= rhs;
      ret.CarSpecScale *= rhs;
      ret.FogSkyColour *= rhs;

      return ret;
    }
    timeofdaylightinglayout operator/(float rhs) {
      timeofdaylightinglayout ret = *this;
      ret.SpecularColour /= rhs;
      ret.DiffuseColour /= rhs;
      ret.AmbientColour /= rhs;
      ret.FogHazeColour /= rhs;
      ret.FixedFunctionSkyColour /= rhs;
      ret.FogDistanceScale /= rhs;
      ret.FogSkyColourScale /= rhs;
      ret.FogHazeColourScale /= rhs;
      ret.EnvSkyBrightness /= rhs;
      ret.CarSpecScale /= rhs;
      ret.FogSkyColour /= rhs;

      return ret;
    }

    void operator=(const timeofdaylightinglayout& rhs) {
      SpecularColour         = rhs.SpecularColour;
      DiffuseColour          = rhs.DiffuseColour;
      AmbientColour          = rhs.AmbientColour;
      FogHazeColour          = rhs.FogHazeColour;
      FixedFunctionSkyColour = rhs.FixedFunctionSkyColour;
      FogDistanceScale       = rhs.FogDistanceScale;
      FogSkyColourScale      = rhs.FogSkyColourScale;
      FogHazeColourScale     = rhs.FogHazeColourScale;
      EnvSkyBrightness       = rhs.EnvSkyBrightness;
      CarSpecScale           = rhs.CarSpecScale;
      FogSkyColour           = rhs.FogSkyColour;
    }
    void operator+=(const timeofdaylightinglayout& rhs) { *this = *this + rhs; }
    void operator-=(const timeofdaylightinglayout& rhs) { *this = *this - rhs; }
    void operator*=(float rhs) { *this = *this * rhs; }
    void operator/=(float rhs) { *this = *this / rhs; }
  };
}  // namespace mwsdk::mw05::rt::Attrib::Layouts

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_LAYOUTS_TIMEOFDAYLIGHTINGLAYOUT_H
