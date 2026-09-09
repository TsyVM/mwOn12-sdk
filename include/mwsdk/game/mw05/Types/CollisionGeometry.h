// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/CollisionGeometry.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_COLLISIONGEOMETRY_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_COLLISIONGEOMETRY_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#if defined(__linux__) || defined(_LINUX)
#if defined(__GNUC__) && !defined(_MSC_VER)
#include <byteswap.h>  // bswap_16()
#endif
#elif defined(_WIN32)
#if defined(_MSC_VER)
#include <intrin.h>  // _byteswap_ushort()
#endif
#else
#error This operating system is not supported.
#endif

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UCrc32.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  namespace CollisionGeometry {
    // Compressed Vector3 (UMath::Vector3)
    struct _V3c {
      std::int16_t x, y, z;

      void Compress(const UMath::Vector3& from) {
        x = static_cast<std::int16_t>(from.x * INT16_MAX / 10000.0f);
        y = static_cast<std::int16_t>(from.y * INT16_MAX / 10000.0f);
        z = static_cast<std::int16_t>(from.z * INT16_MAX / 10000.0f);
      }
      void Decompress(UMath::Vector3& to) {
        to.x = static_cast<float>(x * 10000.0f / INT16_MAX);
        to.y = static_cast<float>(y * 10000.0f / INT16_MAX);
        to.z = static_cast<float>(z * 10000.0f / INT16_MAX);
      }
      void EndianSwap() {
#if defined(__linux__) || defined(_LINUX)
        x = static_cast<std::int16_t>(bswap16(x));
        y = static_cast<std::int16_t>(bswap16(y));
        z = static_cast<std::int16_t>(bswap16(z));
#elif defined(_WIN32)
        x = static_cast<std::int16_t>(_byteswap_ushort(x));
        y = static_cast<std::int16_t>(_byteswap_ushort(y));
        z = static_cast<std::int16_t>(_byteswap_ushort(z));
#endif
      }
    };
    // Compressed quaternion (UMath::Vector4)
    struct _Q4c {
      std::int16_t x, y, z, w;

      void Compress(const UMath::Vector4& from) {
        x = static_cast<std::int16_t>(from.x * INT16_MAX / 10000.0f);
        y = static_cast<std::int16_t>(from.y * INT16_MAX / 10000.0f);
        z = static_cast<std::int16_t>(from.z * INT16_MAX / 10000.0f);
        w = static_cast<std::int16_t>(from.w * INT16_MAX / 10000.0f);
      }
      void Decompress(UMath::Vector4& to) {
        to.x = static_cast<float>(x * 10000.0f / INT16_MAX);
        to.y = static_cast<float>(y * 10000.0f / INT16_MAX);
        to.z = static_cast<float>(z * 10000.0f / INT16_MAX);
        to.w = static_cast<float>(w * 10000.0f / INT16_MAX);
      }
      void EndianSwap() {
#if defined(__linux__) || defined(_LINUX)
        x = static_cast<std::int16_t>(bswap16(x));
        y = static_cast<std::int16_t>(bswap16(y));
        z = static_cast<std::int16_t>(bswap16(z));
        w = static_cast<std::int16_t>(bswap16(w));
#elif defined(_WIN32)
        x = static_cast<std::int16_t>(_byteswap_ushort(x));
        y = static_cast<std::int16_t>(_byteswap_ushort(y));
        z = static_cast<std::int16_t>(_byteswap_ushort(z));
        w = static_cast<std::int16_t>(_byteswap_ushort(w));
#endif
      }
    };

    struct BoundsHeader {
      UCrc32 fNameHash;

      std::int32_t fNumBounds;
      std::int32_t fIsResolved;
      std::int32_t fPad;
    };
    struct PCloudHeader {
      std::int32_t fNumPClouds;
      std::int32_t fPad[3];
    };

#pragma pack(push, 1)
    struct Bounds {
      _Q4c          fOrientation;
      _V3c          fPosition;
      std::uint16_t fFlags;
      _V3c          fHalfDimensions;
      std::uint8_t  fNumChildren;
      std::int8_t   fPCloudIndex;
      _V3c          fPivot;
      std::int16_t  fChildIndex;
      UCrc32        fAttributeName;
      UCrc32        fSurface;
      UCrc32        fNameHash;
      Collection*   fCollection;

      void GetOrientation(UMath::Vector4& out) { fOrientation.Decompress(out); }
      void GetPosition(UMath::Vector3& out) { fPosition.Decompress(out); }
      void GetPivot(UMath::Vector3& out) { fPivot.Decompress(out); }
      void GetHalfDimensions(UMath::Vector3& out) { fHalfDimensions.Decompress(out); }

      /* fCollection functions: ->

          const CollisionGeometry::Bounds* GetRoot();
          const CollisionGeometry::Bounds* GetChild(UCrc32);
          const CollisionGeometry::Bounds* GetChild(std::uint32_t);
          const CollisionGeometry::PCloud* GetPointCloud();
          void GetTransform(UMath::Matrix4&);
      */
    };
#pragma pack(pop)

    struct Collection : BoundsHeader {
      const UCrc32& GetObjectName() { return fNameHash; }
    };

    struct IBoundable : UTL::COM::IUnknown {
      virtual ~IBoundable();
      virtual Bounds* GetGeometryNode()                                             = 0;
      virtual bool AddCollisionPrimitive(UCrc32, UMath::Vector3&, float, UMath::Vector3&, SimSurface*, UMath::Vector4*,
                                         mwsdk::mw05::rt::CollisionGeometry::BoundFlags)  = 0;
      virtual bool AddCollisionMesh(UCrc32, UMath::Vector4&, std::uint32_t, SimSurface*,
                                    mwsdk::mw05::rt::CollisionGeometry::BoundFlags, bool) = 0;
    };
  }  // namespace CollisionGeometry
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_COLLISIONGEOMETRY_H
