<div align="center">

# Plugin API Reference

<p><em>The callbacks, the structs, and what the renderer guarantees</em></p>

[![Contract C ABI](https://img.shields.io/badge/Contract-C%20ABI-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![ABI version 1](https://img.shields.io/badge/ABI-version%201-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Source of truth mwon12.h](https://img.shields.io/badge/Source%20of%20truth-mwon12.h-D2B48C?style=for-the-badge&labelColor=1C1008)](../include/mwon12/mwon12.h)
[![TeamVanilla](https://img.shields.io/badge/Team-TeamVanilla-D2B48C?style=for-the-badge&labelColor=1C1008)](https://www.teamvanilla.org/)

<br/>

### Contents

[How a plugin is found](#how-a-plugin-is-found) · [The entry point](#the-entry-point) · [Callbacks](#callbacks) · [Host services](#host-services) · [Versioning](#versioning) · [What this is not](#what-this-is-not)

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

The contract is `include/mwon12/mwon12.h`. It is plain C, and it is the file
both MWOn12 and your plugin compile — so if this document and that header ever
disagree, the header is right.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## How a plugin is found

On the first device creation, MWOn12 scans

```
<folder containing speed.exe>\MWOn12\Plugins\*.dll
```

and for each file calls `LoadLibraryEx` with `LOAD_WITH_ALTERED_SEARCH_PATH`
(so your plugin's own dependencies resolve from the plugin folder), then
`GetProcAddress` for `MWOn12PluginMain`.

The scan is deferred to device creation rather than done in `DllMain` because
loading a DLL from inside `DllMain` deadlocks on the loader lock.

A DLL with no `MWOn12PluginMain` is logged as "not an MWOn12 plugin" and
skipped, not treated as an error — dropping an unrelated DLL in that folder is
a mistake, not a failure.

## The entry point

```c
MWON12_EXPORT int MWON12_CALL
MWOn12PluginMain(const MWOn12_Host* host, MWOn12_Plugin* out);
```

Return `MWON12_OK` after filling in `out`, or `MWON12_DECLINE` to be unloaded
quietly. Declining is a normal answer — an overlay has nothing to do under the
passthrough backend — and is logged as information, not as a problem.

There is **no D3D12 device yet** when this runs. Do not create graphics
resources here.

`MWON12_CALL` is `__cdecl`. It is explicit on every function pointer in the ABI
because a plugin project built with `/Gz` would otherwise hand back `__stdcall`
pointers, and on x86 that mismatch corrupts the stack silently until it is
fatal.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Callbacks

All optional; leave any of them null. They arrive on the game's render thread,
never concurrently with each other for the same plugin.

| Callback | When | Notes |
|---|---|---|
| `OnDeviceCreated` | The D3D12 device now exists and is fully constructed. | Create device-lifetime resources here. |
| `OnResize` | The swap chain changed size or mode. | The device is unchanged. Rebuild only what is sized to the back buffer. |
| `OnPresent` | Once per frame, after the game has drawn and before the swap. | See below. |
| `OnDeviceDestroyed` | The device is about to be destroyed. | **Release everything before returning.** |
| `OnShutdown` | The process is going away. | Called even if no device was ever created. |

### What `OnPresent` guarantees

On entry:

- the command list is **open and recording**;
- the back buffer is in `D3D12_RESOURCE_STATE_RENDER_TARGET`;
- it is bound as the **sole** render target, with **no depth buffer**;
- the viewport and scissor cover it.

So the simplest possible plugin — set a pipeline state, draw a quad — works
without knowing anything about what the game left behind.

You may freely change pipeline state, root signature, descriptor heaps, render
targets, viewport and topology. MWOn12 re-establishes all of it before its next
draw and reads nothing back. (This is the same contract its own internal
blitter uses.)

You must **not**:

- close, reset or submit the command list — MWOn12 presents the frame the
  moment you return;
- leave the back buffer in any state other than `RENDER_TARGET`;
- block. You are on the render thread; time spent here is frame time.

`MWOn12_Frame::frameSlot` is the index of the frame in flight, `0` to
`frameCount-1`. **Any buffer you write each frame must be indexed by it.**
Writing one shared buffer every frame corrupts data the GPU is still reading,
and does so intermittently — the worst way for that class of bug to present.
`mwon12::Overlay` already does this; if you roll your own, do the same.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Host services

`MWOn12_Host` is valid for the life of the process, so you may keep the
pointer.

| Member | Purpose |
|---|---|
| `Log(level, fmt, ...)` | Writes to `MWOn12-render.log`, tagged `[plugin]`. Trace and Info need `VerboseLog=1`; Warn and Error always appear. |
| `ActiveBackend()` | `MWON12_BACKEND_DX12`, or `MWON12_BACKEND_DX9_PASSTHROUGH` when MWOn12 found no usable D3D12 adapter. In the passthrough case there is no D3D12 device and `OnDeviceCreated` never fires. |
| `ConfigInt(section, key, default)` | Reads `MWOn12.ini` — the same file the renderer reads. |
| `ConfigString(section, key, default, out, outBytes)` | Always null-terminates when `outBytes >= 1`. Returns the length written. |
| `GameDirectory()` | Absolute, trailing backslash. Where `speed.exe` lives. |
| `PluginDirectory()` | Absolute, trailing backslash. |

Use your **own** `[Section]` in `MWOn12.ini` rather than `[Renderer]`, so a
user has one config file instead of one per plugin.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Versioning

Both structs carry `structSize` and `abiVersion`.

MWOn12 refuses a plugin whose `abiVersion` differs from its own, rather than
reading a struct laid out differently from the one it expects, and says so in
the log. `structSize` is checked as a range: a plugin built against an older
header legitimately reports a smaller struct, and the fields it did not know
about are already zero because MWOn12 zeroes the struct before the call.

New fields are only ever **appended**. `MWON12_ABI_VERSION` is bumped only for
a change that breaks a plugin built against the previous value.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## What this is not

There is no sandbox. A plugin shares the address space and can corrupt the
device as easily as the renderer can. This document says what is safe to touch;
enforcing it is not possible, and pretending otherwise would be worse than
saying so plainly.

A misbehaving plugin costs its own features and a log warning. It never stops
the game from starting: load failures, declines and ABI mismatches are all
non-fatal by design.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

<div align="center">

<sub>Built and maintained by <a href="https://github.com/TsyVM">TsyVM</a> · <a href="https://www.teamvanilla.org/">TeamVanilla</a></sub>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:6B4226,100:1C1008&height=80&section=footer"/>

</div>
