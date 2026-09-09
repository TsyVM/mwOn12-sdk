<div align="center">

# Writing a Plugin

<p><em>From an empty file to a DLL the game loads</em></p>

[![Type walkthrough](https://img.shields.io/badge/Type-walkthrough-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Language C++23](https://img.shields.io/badge/Language-C%2B%2B23-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Target x86](https://img.shields.io/badge/Target-x86-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Reference plugin-api.md](https://img.shields.io/badge/Reference-plugin-api.md-D2B48C?style=for-the-badge&labelColor=1C1008)](plugin-api.md)
[![TeamVanilla](https://img.shields.io/badge/Team-TeamVanilla-D2B48C?style=for-the-badge&labelColor=1C1008)](https://www.teamvanilla.org/)

<br/>

### Contents

[0. Get the samples running first](#0-get-the-samples-running-first) · [1. A new plugin](#1-a-new-plugin) · [2. Drawing something](#2-drawing-something) · [3. Configuration](#3-configuration) · [4. Lifetime](#4-lifetime) · [5. Debugging](#5-debugging)

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

A walkthrough. For the reference, see [plugin-api.md](plugin-api.md).

## 0. Get the samples running first

Do not skip this. Build the SDK, copy `FrameStats.dll` into
`<game>\MWOn12\Plugins\`, set `VerboseLog=1` in `MWOn12.ini`, run the game and
look for this in `MWOn12-render.log`:

```
[plugin] loaded FrameStats v1.0 from FrameStats.dll
[plugin] 1 plugin(s) active, none drawing
[plugin] FrameStats: device up, 1920x1080, 3 frames in flight
[plugin] FrameStats: 59.9 fps (60 frames in 1001 ms)
```

If those lines appear, the DLL is 32-bit, in the right folder, exporting the
right symbol, and the ABI matched. Every one of those is a separate way to fail
silently, and you want them eliminated before your own code is in the picture.

If nothing appears at all, check in this order: `VerboseLog=1`? DLL actually in
`<game>\MWOn12\Plugins\`? Built x86, not x64? MWOn12 itself loaded — is there a
`MWOn12.log` with a backend banner?

## 1. A new plugin

Add `samples/MyPlugin/MyPlugin.cpp`:

```cpp
#include <mwon12/plugin.hpp>

class MyPlugin final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "MyPlugin"; }
    const char* Version() const override { return "0.1"; }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
        mwon12::LogInfo("MyPlugin: %ux%u", d.width, d.height);
    }
};

MWON12_PLUGIN(MyPlugin)
```

and a line to `samples/CMakeLists.txt`:

```cmake
mwon12_add_plugin(MyPlugin MyPlugin/MyPlugin.cpp)
```

`mwon12_add_plugin` sets x86, the static CRT, links `MWOn12SDK.lib`, and writes
the DLL to `build_win32/bin/Plugins/`.

Building outside this tree works too — link `MWOn12SDK.lib`, add
`MWOn12SDK/include` to your include path, target **Win32**, and use the
**static** CRT (`/MT`). A plugin on the shared CRT needs that CRT present on
the player's machine, and the failure mode is a load error with no explanation.

## 2. Drawing something

`mwon12::Overlay` exists so that "draw a rectangle over the game" is not four
hundred lines of D3D12. It owns a root signature, a pipeline state, shaders
compiled at startup, and one persistently-mapped vertex buffer per frame in
flight.

```cpp
#include <mwon12/plugin.hpp>
#include <mwon12/overlay.hpp>

class Bar final : public mwon12::Plugin {
public:
    const char* Name() const override { return "Bar"; }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
        m_ui.Init(static_cast<ID3D12Device*>(d.device),
                  static_cast<DXGI_FORMAT>(d.backBufferFormat),
                  d.frameCount);
    }

    void OnPresent(const MWOn12_Frame& f) override {
        if (!m_ui.Ready()) return;
        m_ui.Begin(f);                                   // reads size + frameSlot
        m_ui.Rect(20, 20, 200, 24, 0x101014C0);          // panel
        m_ui.RectOutline(20, 20, 200, 24, 0xFFFFFF40);   // border
        m_ui.Submit(static_cast<ID3D12GraphicsCommandList*>(f.commandList));
    }

    void OnDeviceDestroyed() override { m_ui.Shutdown(); }

private:
    mwon12::Overlay m_ui;
};

MWON12_PLUGIN(Bar)
```

Coordinates are **pixels, origin top-left**. Colours are **`0xRRGGBBAA`**;
alpha blends.

Pass the back buffer's format to `Init` — `MWOn12_DeviceInfo::backBufferFormat`
— not a format you assume. A pipeline state whose render-target format differs
from the target bound at draw time is rejected by the runtime.

`Overlay` is filled rectangles and nothing else. No text, no textures, no
clipping. A plugin that needs those is past the point where a helper helps and
should own its own pipeline; `overlay.cpp` is a working reference for how to
build one against this hook.

## 3. Configuration

Read from `MWOn12.ini`, in your own section:

```cpp
bool OnLoad() override {
    if (!mwon12::ConfigInt("MyPlugin", "Enabled", 1)) return false;  // unload
    m_scale = mwon12::ConfigInt("MyPlugin", "Scale", 100) / 100.0f;
    return true;
}
```

```ini
[MyPlugin]
Enabled=1
Scale=150
```

Returning `false` from `OnLoad` unloads the plugin quietly. That is the right
answer when it has nothing to do — including when the backend is not DX12:

```cpp
if (mwon12::Host()->ActiveBackend() != MWON12_BACKEND_DX12) return false;
```

## 4. Lifetime

The mistake that costs the most time:

```cpp
class Wrong final : public mwon12::Plugin {
    Wrong() { m_ui.Init(...); }        // NO — there is no device yet
    ~Wrong() { m_ui.Shutdown(); }      // NO — too late, the device is gone
};
```

```
OnLoad              no device.  Read config, decide whether to run.
OnDeviceCreated     device exists.  Create everything here.
OnPresent           per frame.  Record, do not allocate.
OnDeviceDestroyed   release everything.  The device dies when you return.
OnShutdown          process ending.
```

`OnDeviceCreated` and `OnDeviceDestroyed` can happen **more than once** in a
session if the device is recreated. Do not assume one of each.

## 5. Debugging

Set `DebugLayer=1` in `MWOn12.ini` and run. The D3D12 validation layer's
messages are pumped into `MWOn12-render.log` rather than left for an attached
debugger, so mistakes in your own recording show up in the same file as
everything else. It needs the Windows "Graphics Tools" optional feature.

Attaching a debugger works normally: your plugin is an ordinary DLL. Break in
`OnDeviceCreated` to catch startup, or in `OnPresent` for per-frame work.

Per-frame logging is a trap — 60 lines a second buries everything else. Rate
limit it the way `FrameStats` does.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

<div align="center">

<sub>Built and maintained by <a href="https://github.com/TsyVM">TsyVM</a> · <a href="https://www.teamvanilla.org/">TeamVanilla</a></sub>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:6B4226,100:1C1008&height=80&section=footer"/>

</div>
