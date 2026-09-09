// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/host.hpp
// ---------------------------------------------------------------------------
// Reconstructed host/window/graphics environment for Need for Speed: Most
// Wanted (2005), speed.exe v1.3. Community-reverse-engineered [reference]-
// confidence data, re-expressed in MWSDK form under namespace mwsdk::mw05::rt.
//
// These are the fixed process slots for the game's identity string, window
// handle, Direct3D9 device and DirectInput8 device — the pieces a trainer/
// overlay needs to hook rendering or input. NOT part of MWSDK's verified core
// tables; confirm before shipping. See README.md for the verified/reference split.
// ---------------------------------------------------------------------------
#ifndef MWSDK_MW05_RT_GAME_MW05_HOST_HPP
#define MWSDK_MW05_RT_GAME_MW05_HOST_HPP
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include <cstdint>
#include <cstring>

namespace mwsdk::mw05::rt::host {

// Fixed process addresses (speed.exe v1.3, preferred image base 0x00400000).
inline constexpr std::uintptr_t kGameNameString   = 0x008AF684u; // "Need For Speed Most Wanted" (identity probe)
inline constexpr std::uintptr_t kWindowNameString = 0x008B0050u; // window-title string
inline constexpr std::uintptr_t kHWnd             = 0x00982BF4u; // -> HWND (window handle slot)
inline constexpr std::uintptr_t kD3D9Device       = 0x00982BDCu; // -> IDirect3DDevice9*  (render device)
inline constexpr std::uintptr_t kDInput8Device    = 0x00982D14u; // -> IDirectInputDevice8* (input device)

// Snapshot of the live host handles resolved from the fixed slots above.
struct Handles {
    const char* window_name;     // window-title text
    void**      hwnd;            // -> HWND
    void**      d3d9_device;     // -> IDirect3DDevice9*
    void**      dinput8_device;  // -> IDirectInputDevice8*
};

// True when this process is retail MW05 (identity string present at its slot).
// Only meaningful when run in-process against a live speed.exe.
inline bool is_mw05() {
    const char* id = reinterpret_cast<const char*>(kGameNameString);
    return id != nullptr && std::strstr(id, "Need For Speed Most Wanted") != nullptr;
}

// Resolve the host handles from their fixed slots (in-process only).
inline Handles handles() {
    return Handles{
        reinterpret_cast<const char*>(kWindowNameString),
        reinterpret_cast<void**>(kHWnd),
        reinterpret_cast<void**>(kD3D9Device),
        reinterpret_cast<void**>(kDInput8Device),
    };
}

}  // namespace mwsdk::mw05::rt::host

#endif  // MWSDK_MW05_RT_GAME_MW05_HOST_HPP
