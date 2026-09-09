// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Attrib/RGBA.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_RGBA_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_RGBA_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::Attrib {
  struct RGBA {
    float r, g, b, a;

    RGBA operator+(const RGBA& rhs) const noexcept {
      RGBA _result = *this;
      _result.r += rhs.r;
      _result.g += rhs.g;
      _result.b += rhs.b;
      _result.a += rhs.a;

      return _result;
    }
    RGBA operator-(const RGBA& rhs) const noexcept {
      RGBA _result = *this;
      _result.r -= rhs.r;
      _result.g -= rhs.g;
      _result.b -= rhs.b;
      _result.a -= rhs.a;

      return _result;
    }
    RGBA operator*(const RGBA& rhs) const noexcept {
      RGBA _result = *this;
      _result.r *= rhs.r;
      _result.g *= rhs.g;
      _result.b *= rhs.b;
      _result.a *= rhs.a;

      return _result;
    }
    RGBA operator/(const RGBA& rhs) const noexcept {
      RGBA _result = *this;
      _result.r /= rhs.r;
      _result.g /= rhs.g;
      _result.b /= rhs.b;
      _result.a /= rhs.a;

      return _result;
    }
    RGBA operator*(float rhs) const noexcept {
      RGBA _result = *this;
      _result.r *= rhs;
      _result.g *= rhs;
      _result.b *= rhs;
      _result.a *= rhs;

      return _result;
    }
    RGBA operator/(float rhs) const noexcept {
      RGBA _result = *this;
      _result.r /= rhs;
      _result.g /= rhs;
      _result.b /= rhs;
      _result.a /= rhs;

      return _result;
    }

    void operator=(const RGBA& rhs) {
      this->r = rhs.r;
      this->g = rhs.g;
      this->b = rhs.b;
      this->a = rhs.a;
    }
    void operator+=(const RGBA& rhs) { *this = *this + rhs; }
    void operator-=(const RGBA& rhs) { *this = *this - rhs; }
    void operator*=(const RGBA& rhs) { *this = *this * rhs; }
    void operator/=(const RGBA& rhs) { *this = *this / rhs; }
    void operator*=(float rhs) { *this = *this * rhs; }
    void operator/=(float rhs) { *this = *this / rhs; }

                 operator float*() noexcept { return reinterpret_cast<float*>(this); }
                 operator const float*() const noexcept { return reinterpret_cast<const float*>(this); }
    float&       operator[](std::size_t index) noexcept { return (reinterpret_cast<float*>(this))[index]; }
    const float& operator[](std::size_t index) const noexcept { return (reinterpret_cast<const float*>(this))[index]; }

    RGBA() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
    ~RGBA() = default;
    RGBA(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {}
  };
}  // namespace mwsdk::mw05::rt::Attrib

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_ATTRIB_RGBA_H
