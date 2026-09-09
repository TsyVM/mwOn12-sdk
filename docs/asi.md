<div align="center">

# ASI Mods

<p><em>Drop a .asi into scripts and it runs — and it can draw, too</em></p>

[![Format .asi](https://img.shields.io/badge/Format-.asi-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Loads before the game](https://img.shields.io/badge/Loads-before%20the%20game-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Can draw yes](https://img.shields.io/badge/Can%20draw-yes-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Header asi.hpp](https://img.shields.io/badge/Header-asi.hpp-D2B48C?style=for-the-badge&labelColor=1C1008)](../include/mwon12/asi.hpp)
[![TeamVanilla](https://img.shields.io/badge/Team-TeamVanilla-D2B48C?style=for-the-badge&labelColor=1C1008)](https://www.teamvanilla.org/)

<br/>

### Contents

[What an ASI is](#what-an-asi-is) · [Where they load from](#where-they-load-from) · [When they load](#when-they-load) · [ASI or plugin?](#asi-or-plugin) · [An ASI that draws](#an-asi-that-draws) · [Does an old ASI still work under MWOn12?](#does-an-old-asi-still-work-under-mwon12) · [See also](#see-also)

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

MWOn12 is also an ASI loader. Drop a `.asi` into `<game>\scripts\` and it runs —
no separate loader, and no conflict with one you already have.

That matters because most Most Wanted mods that already exist are `.asi` files,
and because writing one is the shortest path from an idea to something running
inside the game: no ABI and no exports, just a `DllMain`.

And an ASI is not limited to gameplay here. `<mwon12/asi.hpp>` registers one
with the renderer, so it gets the live D3D12 device once per frame exactly as a
plugin does — see [An ASI that draws](#an-asi-that-draws).

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## What an ASI is

A DLL with a different extension and no required exports. All of it happens in
`DllMain`:

```cpp
#include <windows.h>

static DWORD WINAPI Main(LPVOID)
{
    // install hooks, patch memory, run a loop -- whatever the mod does
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(inst);
        CloseHandle(CreateThread(nullptr, 0, &Main, nullptr, 0, nullptr));
    }
    return TRUE;
}
```

The thread matters: `DllMain` runs under the loader lock, and doing real work
there — anything that loads a library, waits on another thread, or touches the
game before it exists — deadlocks. Start a thread and return.

MWSDK's `MWSDK_MOD(fn)` macro is exactly this, written out:

```cpp
#include <mwsdk/mod.hpp>
using namespace mwsdk;

void my_mod() { mw05::vehicle(mw05::vehicle_at(0)).top_speed() *= 1.1f; }
MWSDK_MOD(my_mod)
```

Build it as a DLL, rename the output to `.asi`, drop it in `scripts\`.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Where they load from

In order:

```
<game>\scripts\          the convention every other ASI loader uses
<game>\MWOn12\ASI\
```

Set `Directory=` in the `[ASI]` section of `MWOn12.ini` to use one folder of
your own instead of both. `Enabled=0` turns loading off.

The same filename in both folders is loaded once — loading a mod twice installs
its hooks twice, and a detour chained onto itself usually means infinite
recursion.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## When they load

On the first `Direct3DCreate9`, which is:

- **after** `speed.exe` and MWOn12 are in memory, so the game's code is there to
  hook;
- **before** the game has drawn anything, so a hook is in place ahead of
  whatever it affects;
- **outside** the loader lock, so an ASI that loads its own dependencies is
  safe.

Nothing is ever unloaded. An ASI leaves detours in the game's code, and freeing
the module would leave them pointing at unmapped memory — a crash later, and
somewhere else.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## ASI or plugin?

| | ASI | MWOn12 plugin |
|---|---|---|
| Extension | `.asi` | `.dll` |
| Where | `scripts\` | `MWOn12\Plugins\` |
| Entry point | `DllMain` | `MWOn12PluginMain` |
| Loads | before the game initialises | at device creation |
| Gets the D3D12 device | **yes** — see below | yes, once per frame |
| Runs without MWOn12 | yes, under any ASI loader | no |

The difference is **when you run**, not what you are allowed to do.

An ASI loads before the game has initialised, which is what you want if you are
hooking something that runs early. A plugin loads at device creation, which is
simpler when drawing is all you want and there is nothing to hook. Both can be
installed at once and neither knows about the other.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## An ASI that draws

`<mwon12/asi.hpp>` registers an ASI with the renderer, and from then on it gets
everything a plugin gets — the device, the command list, the back buffer, once
per frame:

```cpp
#include <mwon12/asi.hpp>
#include <mwon12/overlay.hpp>

class MyMod final : public mwon12::Plugin {      // the same base class
public:
    const char* Name() const override { return "MyMod"; }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
        m_ui.Init(static_cast<ID3D12Device*>(d.device),
                  static_cast<DXGI_FORMAT>(d.backBufferFormat), d.frameCount);
    }
    void OnPresent(const MWOn12_Frame& f) override {
        m_ui.Begin(f);
        m_ui.Rect(20, 20, 120, 10, 0xFF4040FF);
        m_ui.Submit(static_cast<ID3D12GraphicsCommandList*>(f.commandList));
    }
    void OnDeviceDestroyed() override { m_ui.Shutdown(); }

private:
    mwon12::Overlay m_ui;
};

MWON12_ASI(MyMod)
```

That is `mwon12::Plugin` unchanged, so one source file can be built either way —
swap `MWON12_ASI` for `MWON12_PLUGIN` and it is a plugin instead.

Build it with `mwon12_add_asi`, which is `mwon12_add_plugin` with the extension
and output folder changed:

```cmake
mwon12_add_asi(MyMod MyMod/MyMod.cpp)          # -> bin/scripts/MyMod.asi
```

`samples/HelloAsi` is a working one: it draws a frame-rate bar *and* logs the
vehicle count through MWSDK, which is the combination an ASI is uniquely good
at.

### How it works, and the one rule

`<mwon12/asi.hpp>` resolves three exports off the loaded `d3d9.dll`:

```
MWOn12_RegisterPlugin    MWOn12_UnregisterPlugin    MWOn12_GetHost
```

The header is header-only, so an ASI needs no `.lib` from this SDK to register —
only to use `mwon12::Overlay`, which is ordinary SDK code.

Registration is **deferred**. MWOn12 takes the plugin on at the next safe point
in the frame and calls `OnDeviceCreated` there, on the render thread. That
matters: your ASI registers from its own thread, and running `OnDeviceCreated`
on that thread would break the promise that callbacks arrive on the render
thread and never concurrently — which is what makes it safe to touch the
command list at all.

So the rule is the plugin rule, unchanged: **create GPU resources in
`OnDeviceCreated`, release them in `OnDeviceDestroyed`, and nothing in the
constructor.** `Register()` returning true means accepted, not running yet.

### When MWOn12 is not there

`Register()` returns false if `d3d9.dll` is the real runtime or another proxy.
That is not an error. `MWON12_ASI` keeps the mod loaded and running — the
gameplay half never needed the renderer — and it simply gets no frame callbacks.
An ASI written this way still works under any other ASI loader.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Does an old ASI still work under MWOn12?

Gameplay mods: yes. They hook `speed.exe`, which MWOn12 does not touch.

Mods that hook Direct3D 9 itself: usually, but they are hooking a proxy. MWOn12
implements `IDirect3DDevice9` and translates to D3D12, so a mod that wraps the
device or its vtable sees a real D3D9 interface and behaves. A mod that reaches
past the interface for the *system* `d3d9.dll` — its exports, its internals —
will not find what it expects, because that runtime is not in use.

If a mod misbehaves, `VerboseLog=1` in `MWOn12.ini` logs each ASI as it loads,
so you can see which are present and in what order.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## See also

- **[mwsdk.md](mwsdk.md)** — verified addresses and typed views, which is what
  you want inside an ASI rather than raw offsets.
- **[hooking.md](hooking.md)** — the hooking library, usable from an ASI too.
- **[writing-a-plugin.md](writing-a-plugin.md)** — the other route.
- **`samples/HelloAsi`** — an ASI that draws a frame-rate bar and reads the
  vehicle list at the same time. The thing to build first.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

<div align="center">

<sub>Built and maintained by <a href="https://github.com/TsyVM">TsyVM</a> · <a href="https://www.teamvanilla.org/">TeamVanilla</a></sub>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:6B4226,100:1C1008&height=80&section=footer"/>

</div>
