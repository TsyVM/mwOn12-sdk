// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Physics.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_PHYSICS_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_PHYSICS_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::Physics {
  namespace Info {
    struct Performance {
      float mTopSpeed;
      float mHandling;
      float mAcceleration;

      explicit Performance() : mTopSpeed(0.0f), mHandling(0.0f), mAcceleration(0.0f) {}
    };
  }  // namespace Info

  namespace Upgrades {
    struct Package {
      eCareerUpgradeLevels mTires;
      eCareerUpgradeLevels mBrakes;
      eCareerUpgradeLevels mChassis;
      eCareerUpgradeLevels mTransmission;
      eCareerUpgradeLevels mEngine;
      eCareerUpgradeLevels mInduction;
      eCareerUpgradeLevels mNOS;
      JunkmanParts         mJunkman;

      explicit Package() :
          mTires(eCareerUpgradeLevels::LevelStock),
          mBrakes(eCareerUpgradeLevels::LevelStock),
          mChassis(eCareerUpgradeLevels::LevelStock),
          mTransmission(eCareerUpgradeLevels::LevelStock),
          mEngine(eCareerUpgradeLevels::LevelStock),
          mInduction(eCareerUpgradeLevels::LevelStock),
          mJunkman(JunkmanParts::None) {}

      static void ApplyPreset(Attrib::Gen::pvehicle& to, const Attrib::Gen::presetride& preset) {
        reinterpret_cast<void(__cdecl*)(Attrib::Gen::pvehicle&, const Attrib::Gen::presetride&)>(0x67C080)(to, preset);
      }

      static bool CanInstallJunkman(const Attrib::Gen::pvehicle& instance, Type type) {
        return reinterpret_cast<bool(__cdecl*)(const Attrib::Gen::pvehicle&, Type)>(0x677FE0)(instance, type);
      }

      static eCareerUpgradeLevels GetLevel(const Attrib::Gen::pvehicle& instance, Type type) {
        return reinterpret_cast<eCareerUpgradeLevels(__cdecl*)(const Attrib::Gen::pvehicle&, Type)>(0x672D30)(instance,
                                                                                                              type);
      }
      static eCareerUpgradeLevels GetMaxLevel(const Attrib::Gen::pvehicle& instance, Type type) {
        return reinterpret_cast<eCareerUpgradeLevels(__cdecl*)(const Attrib::Gen::pvehicle&, Type)>(0x672E80)(instance,
                                                                                                              type);
      }
      static float GetPercent(const Attrib::Gen::pvehicle& instance, Type type) {
        return reinterpret_cast<float(__cdecl*)(const Attrib::Gen::pvehicle&, Type)>(0x677F80)(instance, type);
      }

      static void MatchPerformance(const Attrib::Gen::pvehicle& instance, const Info::Performance& performance) {
        reinterpret_cast<void(__cdecl*)(const Attrib::Gen::pvehicle&, const Info::Performance&)>(0x67C310)(instance,
                                                                                                           performance);
      }

      static void RemoveJunkman(const Attrib::Gen::pvehicle& instance, Type type) {
        reinterpret_cast<void(__cdecl*)(const Attrib::Gen::pvehicle&, Type)>(0x67C220)(instance, type);
      }
      static void RemovePart(const Attrib::Gen::pvehicle& instance, Type type) {
        reinterpret_cast<void(__cdecl*)(const Attrib::Gen::pvehicle&, Type)>(0x678530)(instance, type);
      }

      static void SetJunkman(const Attrib::Gen::pvehicle& instance, Type type) {
        reinterpret_cast<void(__cdecl*)(const Attrib::Gen::pvehicle&, Type)>(0x678150)(instance, type);
      }
      static void SetLevel(const Attrib::Gen::pvehicle& instance, Type type) {
        reinterpret_cast<void(__cdecl*)(const Attrib::Gen::pvehicle&, Type)>(0x678AD0)(instance, type);
      }
      static void SetMaximum(Attrib::Gen::pvehicle& to) {
        reinterpret_cast<void(__cdecl*)(Attrib::Gen::pvehicle&)>(0x67F950)(to);
      }
      static void SetPackage(const Attrib::Gen::pvehicle& instance, const Physics::Upgrades::Package& package) {
        reinterpret_cast<void(__cdecl*)(const Attrib::Gen::pvehicle&, const Physics::Upgrades::Package&)>(0x67F800)(
            instance, package);
      }
    };
  };  // namespace Upgrades

  struct Tunings {
    enum class Path { Steering, Handling, Brakes, RideHeight, Aerodynamics, NOS, Induction };

    float mSteering;
    float mHandling;
    float mBrakes;
    float mRideHeight;
    float mAerodynamics;
    float mNOS;
    float mInduction;

    explicit Tunings() :
        mSteering(0.0f),
        mHandling(0.0f),
        mBrakes(0.0f),
        mRideHeight(0.0f),
        mAerodynamics(0.0f),
        mNOS(0.0f),
        mInduction(0.0f) {}
    explicit Tunings(float steering, float handling, float brakes, float rideHeight, float aerodynamics, float nos,
                     float induction) :
        mSteering(steering),
        mHandling(handling),
        mBrakes(brakes),
        mRideHeight(rideHeight),
        mAerodynamics(aerodynamics),
        mNOS(nos),
        mInduction(induction) {}

    static inline const float g_GameDisplayMultiplier = 5.0f;
  };
}  // namespace mwsdk::mw05::rt::Physics

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_PHYSICS_H
