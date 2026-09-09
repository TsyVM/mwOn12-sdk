# MWOn12SDK

Tools for modifying **Need for Speed: Most Wanted (2005)** running under
[MWOn12](../MWOn12), the DirectX 9 → DirectX 12 renderer.

These live here, and you can use any of them without the others:

| | What it is | Where |
|---|---|---|
| **Plugin SDK** | Write a DLL that MWOn12 loads and hands the live D3D12 device to, once per frame. C ABI, C++ helpers, a static library, working samples. | `include/`, `src/`, `samples/` |
| **Hooking** | Reach the game's own code: hook functions, patch memory, scan for byte patterns. Gameplay, physics, AI — everything that never touches Direct3D. | `include/mwon12/hooks.hpp` |
| **Shader kit** | Replace the game's shaders with your own HLSL. No code, no compiler — edit a text file and restart. | `shaderkit/` |
| **Graphics** | See the frame being built: every draw, every pipeline state. Drop draws, swap shaders at runtime, read the buffers behind a draw. | `include/mwon12/graphics.hpp` |
| **MWSDK** | The game itself rather than its rendering: verified addresses, typed object views, live attributes, and the file formats. | `external/mwsdk/` |
| **ASI mods** | MWOn12 loads `.asi` files too — the format most existing MW mods ship in. They load before the game initialises, and can still get the D3D12 device once per frame. | `include/mwon12/asi.hpp` |
| **User interface** | Checkboxes, sliders, tables, a demo window. VanGUI is built into the SDK — no keyword — with a wrapper that does the descriptor heap, the window hook and the backend lifecycles for you. | `include/mwon12/gui.hpp` |

The shader kit needs nothing from this folder but the instructions. Everything
else needs Visual Studio.

**[MWOn12_Functions_Guide.md](MWOn12_Functions_Guide.md)** is the one-page
reference for all of it: every function, with signatures.

---

## Build

```bat
Build.bat
```

Visual Studio 2022 with the C++ workload. No DirectX SDK, no network, no
package manager. Output:

```
build_win32\Release\MWOn12SDK.lib
build_win32\bin\Plugins\FrameStats.dll        logs the frame rate; draws nothing
build_win32\bin\Plugins\HelloOverlay.dll      draws a framerate bar
build_win32\bin\Plugins\GameHooks.dll         hooks, patches, scans
build_win32\bin\Plugins\VanGfxProbe.dll       counts draws and pipeline states
build_win32\bin\Plugins\GameSdk.dll           reads the live vehicle list via MWSDK
```

C++23, because VanHooks and VanGFX are both built on `std::expected`.

Everything is **x86**, because `speed.exe` is 32-bit. A 64-bit plugin loads
into nothing; CMake stops the build rather than letting you find that out from
a silent failure at runtime.

---

## Try the samples

1. Build MWOn12 and install `d3d9.dll` + `MWOn12.ini` next to `speed.exe`.
2. Copy the sample DLLs into `<game>\MWOn12\Plugins\`, and
   `HelloAsi.asi` into `<game>\scripts\`.
3. Set `VerboseLog=1` in `MWOn12.ini` — plugin log output is Info level.
4. Run the game and read `MWOn12-render.log`.

`FrameStats` writes the frame rate to the log and draws nothing. Build it
first: if its lines appear, your install is correct, and anything that goes
wrong afterwards belongs to the next plugin rather than to the setup.

`HelloOverlay` draws a framerate bar over the game using `mwon12::Overlay`.

---

## Tutorials

Where the samples exercise one API each, `Tutorials/` builds four small,
finished mods — each a standalone **`.asi`** — around one thing MWOn12 lets you
do to the game. They land in `bin/scripts/`; copy the ones you want into
`<game>\scripts\`.

```
Tutorials/
    01_ScreenFX/    a full-screen colour grade with an F12 menu   (Gui + Overlay)
    02_NoCops/      stop the police spawning — a gameplay hook     (hooks + MWSDK)
    03_SuperCars/   boost top speed on a key press                 (MWSDK views)
    04_DrawFilter/  cull draws live from an F12 menu — the pipeline (Gui + VanGFX)
```

Two open a menu on **F12** (ScreenFX, DrawFilter); the other two read
`MWOn12.ini` and a hotkey. Each `.cpp` starts with a full header comment, and
**[Tutorials/README.md](Tutorials/README.md)** is the guided tour. They build
with the SDK (`Build.bat`), or turn them off with `-DMWON12SDK_BUILD_TUTORIALS=OFF`.

---

## Writing a plugin

A plugin is a 32-bit DLL in `<game>\MWOn12\Plugins\` exporting one function.
With the C++ helpers that is:

```cpp
#include <mwon12/plugin.hpp>
#include <mwon12/overlay.hpp>

class MyPlugin final : public mwon12::Plugin {
public:
    const char* Name() const override { return "MyPlugin"; }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
        m_ui.Init(static_cast<ID3D12Device*>(d.device),
                  static_cast<DXGI_FORMAT>(d.backBufferFormat), d.frameCount);
    }

    void OnPresent(const MWOn12_Frame& f) override {
        m_ui.Begin(f);
        m_ui.Rect(20, 20, 120, 10, 0xFF4040FF);   // x, y, w, h, 0xRRGGBBAA
        m_ui.Submit(static_cast<ID3D12GraphicsCommandList*>(f.commandList));
    }

    void OnDeviceDestroyed() override { m_ui.Shutdown(); }

private:
    mwon12::Overlay m_ui;
};

MWON12_PLUGIN(MyPlugin)
```

Add it to `samples/CMakeLists.txt` with `mwon12_add_plugin(MyPlugin
MyPlugin/MyPlugin.cpp)`, or link `MWOn12SDK.lib` from your own project.

See **[docs/writing-a-plugin.md](docs/writing-a-plugin.md)** for the walkthrough,
**[docs/plugin-api.md](docs/plugin-api.md)** for the callbacks in prose, and
**[MWOn12_Functions_Guide.md](MWOn12_Functions_Guide.md)** for every function the
SDK offers with the signature you will actually type.

### Changing the game, not just the picture

Rendering is only half of it. `<mwon12/hooks.hpp>` reaches the game's own code:

```cpp
#include <mwon12/hooks.hpp>
namespace hooks = mwon12::hooks;

using DamageFn = void __fastcall(void*, void*, float);   // a TYPE, not a pointer
static DamageFn* g_orig = nullptr;

hooks::Hook h;                                        // a member, not a local
h.Install<DamageFn>(hooks::Rva(0x0067A410), &MyDamage, &g_orig, "TakeDamage");

hooks::Nop(hooks::Rva(0x0051A340), 5, "speed limiter");
auto addr = hooks::Scan("55 8B EC 83 EC ?? 56 8B F1");   // beats a fixed address
```

See **[docs/hooking.md](docs/hooking.md)**. Built on VanHooks, shipped prebuilt.

### Changing the picture, not just adding to it

`mwon12::Overlay` draws *on top of* a finished frame. `<mwon12/graphics.hpp>`
opens the frame itself: it attaches VanGFX to the D3D12 device MWOn12 renders
the game with, so a plugin sees every draw as it is recorded and every pipeline
state as it is built — and can change or drop any of them.

```cpp
#include <mwon12/graphics.hpp>

mwon12::gfx::Context m_gfx;                    // a member, not a local

void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
    if (!m_gfx.Attach(d)) return;

    m_gfx->on_draw([](vangfx::FrameContext&, vangfx::DrawEvent& ev) {
        if (ev.index_count > 5000) ev.intercept = true;      // drop it
    });

    auto ps = mwon12::gfx::CompilePS(m_gfx, myHlsl);
    mwon12::gfx::ReplacePixelShader(m_gfx, ps,
                                    mwon12::gfx::WhenPixelShaderIs(0xA1B2...));
}

void OnDeviceDestroyed() override { m_gfx.Detach(); }
```

Needs the `VANGFX` keyword, because VanGFX is a large static library and most
plugins have no use for it:

```cmake
mwon12_add_plugin(MyPlugin VANGFX MyPlugin/MyPlugin.cpp)
```

See **[docs/graphics.md](docs/graphics.md)**, and build `samples/VanGfxProbe`
first — it attaches, counts what a frame is made of, and changes nothing.

**Check the shader kit before reaching for this.** If what you want is to
replace the game's shaders with your own HLSL, `shaderkit/` does it with a text
editor, no C++ and no `dxcompiler.dll` to ship. This is for what that cannot do:
reacting to draws, swapping shaders at runtime, reading the buffers behind a draw.


### Changing the game, not the picture at all

`external/mwsdk/` is a whole SDK for Most Wanted itself: verified function
addresses, typed views over the game's objects, live attribute read/write, the
engine's own string hash, and parsers for its file formats. It turns

```cpp
*reinterpret_cast<float*>(car + 0x1A4) *= 1.1f;    // a number nobody can check
```

into

```cpp
mw05::vehicle(car).top_speed() *= 1.1f;
```

Opt in with the `MWSDK` keyword, and build `samples/GameSdk` first:

```cmake
mwon12_add_plugin(MyPlugin MWSDK MyPlugin/MyPlugin.cpp)
```

See **[docs/mwsdk.md](docs/mwsdk.md)** — in particular the note on rebasing,
which is the one thing to get right when combining it with `<mwon12/hooks.hpp>`.

### Something the player can actually use

`mwon12::Overlay` draws rectangles. For anything with a control in it —
a settings panel, a tuning slider, a debug view — `<mwon12/gui.hpp>` gives you
VanGUI, which is Dear ImGui's API under different names:

```cpp
if (!m_gui.Begin(f)) return;
VanGui::Begin("Tuner");
VanGui::SliderFloat("Top speed", &speed, 1.0f, 2.0f);
VanGui::End();
m_gui.End(f);
```

No descriptor heap, no window subclassing, no backend lifecycle — `Init(d)` does
all of it from the device info. **No keyword either**: VanGUI is part of the SDK,
so `#include <mwon12/gui.hpp>` is the whole setup. It costs nothing to a plugin
that does not use it — the linker takes only what is referenced, and
`FrameStats.dll` is the same size either way.

See **[docs/gui.md](docs/gui.md)**, and build `samples/HelloGui` — it tunes the
player car live and can open VanGUI's demo window, which shows every widget next
to the source line that draws it.

### Not writing a plugin at all

MWOn12 is also an ASI loader: drop a `.asi` into `<game>\scripts\` and it runs.
No ABI and no exports — `DllMain` starts a thread and the mod does whatever it
does. It is the format most existing Most Wanted mods already ship in, and the
shortest path from an idea to something running.

An ASI is not limited to gameplay. `<mwon12/asi.hpp>` registers it with the
renderer, and it then gets the same D3D12 device, once per frame, that a plugin
does — from the same `mwon12::Plugin` class, so one source file can be built as
either:

```cmake
mwon12_add_asi(MyMod MyMod/MyMod.cpp)          # -> bin/scripts/MyMod.asi
```

So the choice is **when you want to run**, not what you may do: an ASI loads
before the game initialises, a plugin at device creation. Build
`samples/HelloAsi` — it draws a frame-rate bar and reads the vehicle list at
once.

See **[docs/asi.md](docs/asi.md)**.

### The one rule worth reading twice

Create GPU resources in `OnDeviceCreated`. Release **all** of them in
`OnDeviceDestroyed`.

Not in the constructor: there is no D3D12 device when your plugin is loaded.
Not after `OnDeviceDestroyed` returns: the device is destroyed immediately
afterwards, and anything still referencing it takes the process down with a
live-object report — a crash on exit with no stack pointing at you.

---

## Layout

```
include/mwon12/
    mwon12.h        the C ABI — the actual contract with the renderer
    plugin.hpp      C++ base class and the MWON12_PLUGIN macro
    overlay.hpp     2D overlay renderer for the present hook
    hooks.hpp       function hooking, memory patching, pattern scanning
    graphics.hpp    draw and pipeline interception, runtime shader replacement
    gui.hpp         VanGUI set up against MWOn12's device, in one call
    asi.hpp         registering an ASI with the renderer, so it can draw too
src/
    host.cpp        storage for the host table pointer
    overlay.cpp     the overlay's D3D12 implementation
samples/
    FrameStats/     logs the frame rate; draws nothing
    HelloOverlay/   draws a framerate bar
    GameHooks/      hooks a function, patches memory, scans for a pattern
    VanGfxProbe/    counts the draws and pipeline states in a frame
    GameSdk/        reads the live vehicle list through MWSDK
    HelloAsi/       an ASI that draws and reads game state
    HelloGui/       a UI panel — built as both a plugin and an ASI
Tutorials/          four finished .asi mods, one idea each (see its README)
external/
    vangfx/         VanGFX source, built for x86 with the SDK
    mwsdk/          MWSDK source — the game's addresses, layouts and formats
    vangui/         VanGUI source — widgets, D3D12 + D3D9 + Win32 backends
shaderkit/          shader replacement — no compiler needed
docs/
```

VanGFX and MWSDK are opt-in per plugin, because neither is small and most
plugins want neither. VanGUI is not: it is linked into the SDK, since a mod that
needs a settings panel should not have to find a build flag first.
`mwon12_add_plugin(MyPlugin VANGFX MWSDK MyPlugin.cpp)`. VanGUI needs no keyword.

VanHooks lives next door in `MWOn12/external/VanHooks/` as a prebuilt x86
static-CRT package (`vanhooks.lib` + `Zydis.lib` + `Zycore.lib`). CMake finds it
there or under this project, and disables hooking with a clear message if it or
its Zydis libs are missing.

`mwon12.h` is the single source of truth for the ABI. MWOn12 compiles this
exact file; there is deliberately no second copy in the renderer's tree,
because two copies drift and a plugin built against the wrong one is a crash
rather than an error.
