// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/UMath.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_UMATH_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_UMATH_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Shared/UMath.h"

namespace mwsdk::mw05::rt::UMath {
  using Vector2 = mwsdk::mw05::rt::UVector2;
  using Vector3 = mwsdk::mw05::rt::UVector3;

  struct Vector4 : mwsdk::mw05::rt::UVector4 {
    // RightVector [Roll]
    inline void ExtractXAxis(Vector3* to) const {
      reinterpret_cast<void(__cdecl*)(const Vector4*, Vector3*)>(0x5CA6F0)(this, to);
    }
    // UpVector [Pitch]
    inline void ExtractYAxis(Vector3* to) const {
      reinterpret_cast<void(__cdecl*)(const Vector4*, Vector3*)>(0x5CA7B0)(this, to);
    }
    // ForwardVector [Yaw]
    inline void ExtractZAxis(Vector3* to) const {
      reinterpret_cast<void(__cdecl*)(const Vector4*, Vector3*)>(0x5CA870)(this, to);
    }

    inline void ToMatrix4(Matrix4* to) const {
      reinterpret_cast<void(__cdecl*)(const Vector4*, Matrix4*)>(0x5CA910)(this, to);
    }

    inline Vector4& operator=(const UVector4& r) {
      x = r.x;
      y = r.y;
      z = r.z;
      w = r.w;

      return *this;
    }
  };
  struct Matrix4 : mwsdk::mw05::rt::UMatrix4 {
    Vector4 v0, v1, v2, v3;

    inline void ToQuaternion(Vector4* to) const {
      reinterpret_cast<void(__cdecl*)(const Matrix4*, Vector4*)>(0x5D06A0)(this, to);
    }

    inline Matrix4& operator=(const UMatrix4& r) {
      v0 = r.v0;
      v1 = r.v1;
      v2 = r.v2;
      v3 = r.v3;

      return *this;
    }
  };
}  // namespace mwsdk::mw05::rt::UMath

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_UMATH_H
