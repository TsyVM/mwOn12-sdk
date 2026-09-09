<div align="center">

# A User Interface

<p><em>Checkboxes, sliders, tables and a demo window, over a running game</em></p>

[![Library VanGUI](https://img.shields.io/badge/Library-VanGUI-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![API Dear ImGui style](https://img.shields.io/badge/API-Dear%20ImGui%20style-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Backends DX12 + Win32](https://img.shields.io/badge/Backends-DX12%20%2B%20Win32-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Header gui.hpp](https://img.shields.io/badge/Header-gui.hpp-D2B48C?style=for-the-badge&labelColor=1C1008)](../include/mwon12/gui.hpp)
[![TeamVanilla](https://img.shields.io/badge/Team-TeamVanilla-D2B48C?style=for-the-badge&labelColor=1C1008)](https://www.teamvanilla.org/)

<br/>

### Contents

[The part that usually stops people](#the-part-that-usually-stops-people) · [Input](#input) · [Finding out what a widget is called](#finding-out-what-a-widget-is-called) · [From an ASI](#from-an-asi) · [DirectX 9](#directx-9) · [What is vendored](#what-is-vendored) · [See also](#see-also)

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

`mwon12::Overlay` draws rectangles. The moment a mod wants a checkbox, a slider,
a colour picker or a list the user can scroll, it wants a UI toolkit.

VanGUI is one, shipped as `lib/vangui.lib` and **part of the SDK** — there is
no keyword and nothing to enable. Include the header and it works:

```cpp
#include <mwon12/gui.hpp>
```

Its API is Dear ImGui's under different names, so anything written for ImGui
translates by replacing `ImGui::` with `VanGui::`.

Build `samples/HelloGui` first. Press **INSERT** in game.

It costs nothing to a plugin that does not use it. The linker pulls object files
out of a static library only when something references them, so a plugin that
never names `VanGui::` links none of it — `FrameStats.dll` is byte-for-byte the
same size with this in the SDK as it was without.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## The part that usually stops people

Standing ImGui up on D3D12 by hand is a shader-visible descriptor heap for the
font atlas, a Win32 message hook so the mouse works, two backend lifecycles
started and stopped in the right order, and a NewFrame/Render pair that has to
straddle your own callbacks. None of it is specific to the mod being written.

`mwon12::Gui` is that code, once:

```cpp
#include <mwon12/gui.hpp>

class Tuner final : public mwon12::Plugin {
    mwon12::Gui m_gui;

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
        m_gui.Init(d);                       // heap, backends, window hook
        m_gui.SetToggleKey(VK_INSERT);
        m_gui.SetVisible(false);
    }

    void OnPresent(const MWOn12_Frame& f) override {
        if (!m_gui.Begin(f)) return;         // hidden: build nothing

        VanGui::Begin("Tuner");
        VanGui::SliderFloat("Top speed", &m_speed, 1.0f, 2.0f);
        if (VanGui::Button("Apply")) Apply();
        VanGui::End();

        m_gui.End(f);                        // records the draw calls
    }

    void OnDeviceDestroyed() override { m_gui.Shutdown(); }
};
```

`Init` takes `MWOn12_DeviceInfo` whole — device, window, back buffer format and
frame count all come out of it, so there is no size to keep in step by hand.

**`Begin()` returning false means build nothing and do not call `End()`.** There
is no frame to end, and VanGUI asserts if you end one it did not start.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Input

A UI nobody can click is not a UI, so `Init` subclasses the game's window. That
means the game and the UI are reading the same mouse, and a click on a button
would also steer the car.

`SetBlockGameInput(true)` — on by default — stops mouse and keyboard messages
reaching the game while the UI wants them. `WantsInput()` reports the same
condition if you would rather decide yourself.

It cannot be perfect, and the reason is worth knowing: a game that reads the
keyboard with `GetAsyncKeyState` or through DirectInput never looks at the
message queue, and nothing done to that queue will stop it. **Most Wanted drives
this way.** What this reliably fixes is the mouse and anything menu-driven.

**Only one `Gui` per process.** The first to `Init` takes it; a second is
refused outright and says so in the log.

That is stronger than the window-subclassing problem it started as, and the
reason is VanGUI rather than Win32. The widget API, both backends and the `IO`
struct all address a single *current* context held in a global, and neither
backend can be initialised twice — each stashes its state in that context's
`BackendPlatformUserData` / `BackendRendererUserData`.
`VanGui::CreateContext` restores the previously current context before
returning, so a second `Gui`'s backends would initialise into the **first**
`Gui`'s context, overwrite the two pointers there, and leave whichever shuts
down second freeing memory the first already freed. It is a use-after-free that
only shows up on exit.

So the second `Init` returns false. Two panels in one process is a normal thing
to want, and the way to have it is one `Gui` and two `VanGui::Begin`/`End`
blocks — which is how a UI toolkit expects to be used anyway.

The window subclass is a separate, weaker rule that still applies: `Shutdown`
puts the game's procedure back only if the window still has ours. Something
loaded after us may have subclassed the same window since, and restoring over
it would cut it out of the chain — and leave it holding a pointer into a module
that is about to unload.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Finding out what a widget is called

```cpp
VanGui::ShowDemoWindow(&open);
```

Every control the library has, on screen, each next to the line of source that
draws it. It is the fastest documentation there is, and `HelloGui` has a
checkbox that opens it.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## From an ASI

Everything above works in an `.asi` too — `<mwon12/asi.hpp>` registers it with
the renderer, and it gets the same device once per frame:

```cmake
mwon12_add_asi(MyMod MyMod/MyMod.cpp)             # -> bin/scripts/MyMod.asi
```

`samples/HelloGui` is built **both** ways from one source file, with nothing
different but the entry-point macro at the bottom. `HelloGui.dll` goes in
`MWOn12\Plugins\` and toggles on INSERT; `HelloGui.asi` goes in `scripts\` and
toggles on HOME. Install both at once if you want to see the difference — the
ASI loads before the game initialises.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## DirectX 9

MWOn12 falls back to a D3D9 passthrough on a machine with no usable D3D12
adapter. There is no D3D12 device then and no `OnPresent`, so `mwon12::Gui`
cannot help: `Init` returns false and logs why. Treat that as "no UI", not as
fatal.

The **D3D9 backend is still built and shipped** for a mod that wants to cover
that case. It is not wired into `mwon12::Gui` because the renderer never hands a
plugin a D3D9 device to draw with — you would be hooking
`IDirect3DDevice9::Present` yourself:

```cpp
#include <vangui_impl_dx9.h>

VanGui_ImplDX9_Init(device);                              // once
VanGui_ImplDX9_NewFrame();                                // each frame
VanGui_ImplDX9_RenderDrawData(VanGui::GetDrawData());     // after Render()
```

That is real work, and it is the honest amount of work, rather than a helper
pretending to a device it was never given.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## What is vendored

Nine files: the VanGUI core, the demo, the Win32 platform backend, and both the
D3D12 and D3D9 renderer backends. Upstream lives at `Projects/VanGUI` and covers
fourteen backends, vcpkg and a C++23 module target — none of which a game plugin
needs, and any of which could fail at configure time for reasons that have
nothing to do with the plugin being built.

Sources are copied verbatim. Anything that needs changing is changed upstream
and re-copied.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## See also

- **[plugin-api.md](plugin-api.md)** — `mwon12::Overlay`, for when rectangles are
  genuinely all you need.
- **[asi.md](asi.md)** — the ASI route, including the one-source-two-builds
  arrangement `HelloGui` uses.
- **[mwsdk.md](mwsdk.md)** — reading and writing the game state a panel shows.
- **[VanGUI-README.md](VanGUI-README.md)** — the upstream README, for the widget
  set behind `VanGui::`.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

<div align="center">

<sub>Built and maintained by <a href="https://github.com/TsyVM">TsyVM</a> · <a href="https://www.teamvanilla.org/">TeamVanilla</a></sub>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:6B4226,100:1C1008&height=80&section=footer"/>

</div>
