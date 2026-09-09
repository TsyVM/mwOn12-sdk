// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/MW05.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_MW05_H
#define MWSDK_MW05_RT_GAME_MW05_MW05_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/compat/memory.hpp"
#include "mwsdk/game/mw05/all_types.hpp"

namespace mwsdk::mw05::rt::Variables {
  inline MemoryFieldWrapper<float>                  AnimationSpeed{0x904AEC, 45.0f, FLT_MIN, FLT_MAX};
  inline MemoryFieldWrapper<Math::Matrix4>          CarScaleMatrix{0x9B34B0, Math::Matrix4()};
  inline MemoryFieldWrapper<bool>                   DrawCars{0x903320, true};
  inline MemoryFieldWrapper<bool>                   DrawCarsReflections{0x903324, true};
  inline MemoryFieldWrapper<bool>                   DrawCarShadow{0x903328, true};
  inline MemoryFieldWrapper<bool>                   DrawHUD{0x57CAA8, true};
  inline MemoryFieldWrapper<bool>                   DrawLightFlares{0x8F2918, true};
  inline MemoryFieldWrapper<CARPART_LOD>            ForceCarLOD{0x903384, CARPART_LOD::A};
  inline MemoryFieldWrapper<CARPART_LOD>            ForceTireLOD{0x903388, CARPART_LOD::A};
  inline MemoryFieldWrapper<bool>                   IsFadeScreenOn{0x91CAE4, false};
  inline MemoryFieldWrapper<bool>                   IsInNIS{0x91606C, false};
  inline MemoryFieldWrapper<std::uint16_t>          NOSFOVWidening{0x91112C, 0x666, 0x001, 0x3500};
  inline MemoryFieldWrapper<ePrecullerMode>         PrecullerMode{0x8FAE44, ePrecullerMode::On};
  inline MemoryFieldWrapper<bool>                   SkipFE{0x926064, false};
  inline MemoryFieldWrapper<bool>                   SkipFEDisableCops{0x8F86C0, true};
  inline MemoryFieldWrapper<bool>                   SkipFEDisableTraffic{0x926094, true};
  inline MemoryFieldWrapper<const char*>            SkipFEPlayerCar{0x8F86A8, "bmwm3gtre46"};
  inline MemoryFieldWrapper<ePlayerSettingsCameras> SkipFEPOVType{0x8F86C4, ePlayerSettingsCameras::Close};
  inline MemoryFieldWrapper<float>                  SkipFETrafficDensity{0x926090, 0.0f};
  inline MemoryFieldWrapper<bool>                   StopUpdatingCamera{0x911020, false};
  inline MemoryFieldWrapper<AICopManager*>          TheOneCopManager{0x90D5F4, nullptr};
  inline MemoryFieldWrapper<GameFlowState>          TheGameFlowManager{0x925E90, GameFlowState::InFrontEnd};
  inline MemoryFieldWrapper<float>                  Tweak_GameBreakerCollisionMass{0x901AEC, 2.0f, FLT_MIN, FLT_MAX};
  inline MemoryFieldWrapper<float>                  Tweak_GameSpeed{0x901B1C, 1.0f, FLT_MIN, FLT_MAX};
  inline MemoryFieldWrapper<bool>                   Tweak_InfiniteNOS{0x937804, false};
  inline MemoryFieldWrapper<bool>                   Tweak_InfiniteRaceBreaker{0x988E1C, false};
  inline MemoryFieldWrapper<bool>                   Tweak_PauseCameraLock{0x92584C, false};
  inline MemoryFieldWrapper<bool>                   WindowHasLostFocus{0x982C50, false};
}  // namespace mwsdk::mw05::rt::Variables

#endif  // MWSDK_MW05_RT_GAME_MW05_MW05_H
