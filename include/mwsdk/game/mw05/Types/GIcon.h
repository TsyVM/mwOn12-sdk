// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/GIcon.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_GICON_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_GICON_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt {
  struct GIcon {
    struct EffectInfo {
      std::uint32_t mType;
      std::uint32_t mModelHash;
      std::uint32_t mParticleHash;
    };

    enum class Flags : std::uint16_t {
      ShowInWorld     = 1 << 0,
      ShowOnMap       = 1 << 1,
      Spawned         = 1 << 2,
      Enabled         = 1 << 3,
      Disposable      = 1 << 4,
      SnappedToGround = 1 << 5,
      ShowOnSpawn     = 1 << 6,
      GPSing          = 1 << 7
    };
    enum class Type : std::uint16_t {
      Invalid,
      RaceSprint,
      RaceCircuit,
      RaceDrag,
      RaceKnockout,
      RaceCheckPoint,
      RaceSpeedtrap,
      RaceRival,
      GateSafehouse,
      GateCarLot,
      GateCustomShop,
      HidingSpot,
      PursuitBreaker,
      SpeedTrapInRace,
      Checkpoint,
      GoalMarker
    };

    Type          mType;
    Flags         mFlags;
    std::int16_t  mSectionId;
    std::int16_t  mCombSectionId;
    WorldModel*   mModel;
    EmitterGroup* mEmitter;
    Math::Vector3 mPosition;
    std::uint16_t mRotation;
    std::uint16_t mPad;

   public:
    GIcon(Type type, const UMath::Vector3& initialPosition, float _unk = 0.0f) {
      reinterpret_cast<void(__thiscall*)(GIcon*, Type, const UMath::Vector3&, float)>(0x5E5970)(this, type,
                                                                                                initialPosition, _unk);
    }
    ~GIcon() { reinterpret_cast<void(__thiscall*)(GIcon*, bool)>(0x5EBED0)(this, true); }

    inline std::uint32_t FindSection() { return reinterpret_cast<std::uint32_t(__thiscall*)(GIcon*)>(0x5DE590)(this); }
    inline WorldModel*   CreateGeometry(std::uint32_t _unk) {
        return reinterpret_cast<WorldModel*(__thiscall*)(GIcon*, std::uint32_t)>(0x5DE690)(this, _unk);
    }
    inline void SetPosition() { reinterpret_cast<void(__thiscall*)(GIcon*)>(0x5E5A90)(this); }
    inline void Spawn() { reinterpret_cast<void(__thiscall*)(GIcon*)>(0x5EC270)(this); }
    inline void Unspawn() { reinterpret_cast<void(__thiscall*)(GIcon*)>(0x5E5A00)(this); }
    inline void SnapToGround() { reinterpret_cast<void(__thiscall*)(GIcon*)>(0x5EA0F0)(this); }

    inline Type         GetType() { return mType; }
    inline std::int16_t GetSectionID() { return mSectionId; }
    inline std::int16_t GetCombinedSectionID() { return mCombSectionId; }
    inline bool         GetVisibleInWorld() {
              return static_cast<std::uint16_t>(mFlags) & static_cast<std::uint16_t>(Flags::ShowInWorld);
    }
    inline bool GetVisibleOnMap() {
      return static_cast<std::uint16_t>(mFlags) & static_cast<std::uint16_t>(Flags::ShowOnMap);
    }
    inline bool GetIsDisposable() {
      return static_cast<std::uint16_t>(mFlags) & static_cast<std::uint16_t>(Flags::Disposable);
    }
    inline bool GetIsEnabled() {
      return static_cast<std::uint16_t>(mFlags) & static_cast<std::uint16_t>(Flags::Enabled);
    }
    inline bool GetIsGPSing() { return static_cast<std::uint16_t>(mFlags) & static_cast<std::uint16_t>(Flags::GPSing); }
    inline bool GetIsSnapped() {
      return static_cast<std::uint16_t>(mFlags) & static_cast<std::uint16_t>(Flags::SnappedToGround);
    }

    inline void MarkDisposable() {
      mFlags = static_cast<Flags>(static_cast<std::uint16_t>(mFlags) | static_cast<std::uint16_t>(Flags::Disposable));
    }
    inline void Show() {
      mFlags = static_cast<Flags>(static_cast<std::uint16_t>(mFlags) | static_cast<std::uint16_t>(Flags::ShowInWorld));
    }
    inline void Hide() {
      mFlags = static_cast<Flags>(static_cast<std::uint16_t>(mFlags) ^ static_cast<std::uint16_t>(Flags::ShowInWorld));
    }
    inline void HideUntilRespawn() {
      Hide();
      mFlags = static_cast<Flags>(static_cast<std::uint16_t>(mFlags) | static_cast<std::uint16_t>(Flags::ShowOnSpawn));
    }
    inline void ShowOnMap() {
      mFlags = static_cast<Flags>(static_cast<std::uint16_t>(mFlags) | static_cast<std::uint16_t>(Flags::ShowOnMap));
    }
    inline void HideOnMap() {
      mFlags = static_cast<Flags>(static_cast<std::uint16_t>(mFlags) ^ static_cast<std::uint16_t>(Flags::ShowOnMap));
    }
  };

#if 1  // portable flag operators
  DEFINE_ENUM_FLAG_OPERATORS(GIcon::Flags)
  DEFINE_ENUM_FLAG_OPERATORS(GIcon::Type)
#endif
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_GICON_H
