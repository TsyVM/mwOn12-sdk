// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/Game.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_GAME_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_GAME_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"

namespace mwsdk::mw05::rt::Game {
  static inline void BeginGameFlowUnloadTrack() { reinterpret_cast<void(__cdecl*)()>(0x667340)(); }
  // ECameraShake
  static inline void ShakeCamera() { reinterpret_cast<void(__cdecl*)()>(0x62B110)(); }
  // EForcePursuitBail
  static inline void ForcePursuitBail() { reinterpret_cast<void(__cdecl*)()>(0x62ACE0)(); }
  // EForcePursuitStart, also sets the heat level to max
  static inline void ForcePursuitStart() { reinterpret_cast<void(__cdecl*)()>(0x60AAC0)(); }
  // Game_AbandonRace
  static inline void AbandonRace() { reinterpret_cast<void(__cdecl*)()>(0x60DEB0)(); }
  // Game_AwardPlayerBounty
  static inline void AwardPlayerBounty(std::int32_t amount) {
    reinterpret_cast<void(__cdecl*)(std::int32_t)>(0x612220)(amount);
  }
  // Game_BlowEngine
  static inline void BlowEngine(ISimable* pTarget) { reinterpret_cast<void(__cdecl*)(ISimable*)>(0x60AB30)(pTarget); }
  // Game_ClearAIControl
  static inline void ClearAIControl(bool _unk = true) { reinterpret_cast<void(__cdecl*)(bool)>(0x612420)(_unk); }
  // GPS_Disengage
  static inline void DisengageGPS() { reinterpret_cast<void(__cdecl*)()>(0x41ACE0)(); }
  // GPS_Engage
  static inline void EngageGPS(UMath::Vector3* to, float _unk = 0.0f) {
    reinterpret_cast<void(__cdecl*)(UMath::Vector3*, float)>(0x42C830)(to, _unk);
  }
  // Game_ForceAIControl
  static inline void ForceAIControl(bool _unk = true) { reinterpret_cast<void(__cdecl*)(bool)>(0x6123B0)(_unk); }
  // Game_JumpToCarLot
  static inline void JumpToCarLot() { reinterpret_cast<void(__cdecl*)()>(0x605250)(); }
  // Game_JumpToSafehouse
  static inline void JumpToSafehouse() { reinterpret_cast<void(__cdecl*)()>(0x6052B0)(); }
  // Game_NavigatePlayerTo
  static inline void NavigatePlayerTo(GRuntimeInstance* to, GTrigger* trigger, float _unk = 0.0f, bool setGPS = true) {
    reinterpret_cast<void(__cdecl*)(GRuntimeInstance*, GTrigger*, float, bool)>(0x605360)(to, trigger, _unk, setGPS);
  }
  // Game_SabotageEngine
  static inline void SabotageEngine(ISimable* pTarget, float durationInSeconds) {
    reinterpret_cast<void(__cdecl*)(ISimable*, float)>(0x60AB60)(pTarget, durationInSeconds);
  }
  // Game_SetCopsEnabled
  static inline void SetCopsEnabled(bool copsEnabled) { reinterpret_cast<void(__cdecl*)(bool)>(0x604F40)(copsEnabled); }
  // Game_SetWorldHeat
  static inline void SetWorldHeat(float newHeatLevel) {
    reinterpret_cast<void(__cdecl*)(float)>(0x612660)(newHeatLevel);
  }
  // Game_ShowPauseMenu
  static inline void ShowPauseMenu() { reinterpret_cast<void(__cdecl*)()>(0x6050F0)(); }

  // custom hashing
  static inline std::uint32_t bStringHash(const char* cstring) {
    return reinterpret_cast<std::uint32_t(__cdecl*)(const char*)>(0x460BF0)(cstring);
  }

  // uses 0xABCDEF00 magic
  static inline std::uint32_t stringhash32(const char* cstring) {
    return reinterpret_cast<std::uint32_t(__cdecl*)(const char*)>(0x5CC240)(cstring);
  }
}  // namespace mwsdk::mw05::rt::Game

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_GAME_H
