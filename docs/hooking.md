<div align="center">

# Hooking and Patching

<p><em>Reaching the game's own code — gameplay, physics, AI</em></p>

[![Library VanHooks](https://img.shields.io/badge/Library-VanHooks-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Target x86](https://img.shields.io/badge/Target-x86-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Covers hook + patch + scan](https://img.shields.io/badge/Covers-hook%20%2B%20patch%20%2B%20scan-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Header hooks.hpp](https://img.shields.io/badge/Header-hooks.hpp-D2B48C?style=for-the-badge&labelColor=1C1008)](../include/mwon12/hooks.hpp)
[![TeamVanilla](https://img.shields.io/badge/Team-TeamVanilla-D2B48C?style=for-the-badge&labelColor=1C1008)](https://www.teamvanilla.org/)

<br/>

### Contents

[Three things](#three-things) · [Fn is a function type, not a pointer](#fn-is-a-function-type-not-a-pointer) · [Addresses](#addresses) · [Why patterns beat addresses](#why-patterns-beat-addresses) · [Hooking an export by name](#hooking-an-export-by-name) · [Lifetime and threading](#lifetime-and-threading) · [When it fails](#when-it-fails) · [Patching code the game is running](#patching-code-the-game-is-running) · [What is underneath](#what-is-underneath)

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

`<mwon12/hooks.hpp>` reaches the game's own code. The render hook in
[plugin-api.md](plugin-api.md) reaches what the game *draws*; this reaches what
it *does* — gameplay, physics, AI, handling, damage, HUD logic. None of that
goes through Direct3D, so none of it is reachable any other way.

Everything here is part of the SDK and shipped prebuilt — there is nothing to
fetch, build or opt into.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Three things

```cpp
#include <mwon12/hooks.hpp>
namespace hooks = mwon12::hooks;
```

**Hook a function** — replace it, or wrap it and call through:

```cpp
using DamageFn = void __fastcall(void* self, void* /*edx*/, float amount);
static DamageFn* g_orig = nullptr;

void __fastcall MyDamage(void* self, void* edx, float amount) {
    g_orig(self, edx, amount * 0.5f);      // half damage
}

hooks::Hook h;                              // keep this alive!
h.Install<DamageFn>(hooks::Rva(0x0067A410), &MyDamage, &g_orig, "TakeDamage");
```

**Patch memory**:

```cpp
hooks::Nop(hooks::Rva(0x0051A340), 5, "disable speed limiter");
hooks::Patch(hooks::Rva(0x0051A340), { 0xEB, 0x1A }, "force jump");
hooks::Write<float>(hooks::Rva(0x008A1234), 2.5f, "grip multiplier");
```

**Find an address by pattern**:

```cpp
if (auto addr = hooks::Scan("55 8B EC 83 EC ?? 56 8B F1 D9 45 ??")) {
    h.Install<DamageFn>(*addr, &MyDamage, &g_orig, "TakeDamage");
}
```

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## `Fn` is a function type, not a pointer

This is the single most common compile error against this API.

```cpp
using DamageFn = void __fastcall(void*, void*, float);      // YES
using DamageFn = void(__fastcall*)(void*, void*, float);    // NO
```

`Install<Fn>` takes `Fn* detour` and `Fn** originalOut`. Hand it a type that is
already a pointer and you have asked for a pointer to a pointer, and it will not
compile. Declare the original as `DamageFn* g_orig`.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Addresses

An address out of a disassembler is **static**: it assumes the module sits at
its preferred base, `0x400000`. `speed.exe` is a 2005 binary with no ASLR, so
that is nearly always where it lands — but a loader, a mod manager or a rebased
build moves it, and then a raw address points at nothing in particular.

| | |
|---|---|
| `hooks::Rva(0x0067A410)` | Rebases onto the real load address. **Use this.** |
| `hooks::Abs(addr)` | Takes the number as final. For an address you already resolved at runtime. |
| `hooks::ModuleBase()` | Where `speed.exe` actually loaded. |

`Rva()` accepts either spelling — a full static address (`0x0067A410`) or a bare
offset (`0x0027A410`) — and does the right thing with both, because both are
what people actually type.

Better still: don't depend on an address. Find it by pattern.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Why patterns beat addresses

A hard-coded address is correct for exactly one build of one executable. A byte
pattern matches the *code*, so it survives a rebase, a recompile, and often a
different game version. If you intend anyone else to run your mod, scan.

```cpp
auto hits = hooks::ScanAll("8B 4C 24 ?? 85 C9 74 ??");   // every match
auto one  = hooks::Scan   ("8B 4C 24 ?? 85 C9 74 ??");   // exactly one, or nothing
```

`Scan()` returns nothing when the pattern matches **more than once**, not just
when it matches zero times. That is deliberate: an ambiguous pattern that
quietly returned its first hit is how a mod ends up patching the wrong function
on somebody else's machine. Widen the pattern until it is unique — `ScanAll()`
tells you how many places it currently hits.

Both default to searching `speed.exe`. Pass a module name to search elsewhere:

```cpp
hooks::Scan("AA BB 33 44", "MyPlugin.dll");
```

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Hooking an export by name

No address at all, for anything in a DLL's export table:

```cpp
using SleepFn = void WINAPI(DWORD);
static SleepFn* g_origSleep = nullptr;

m_hook.InstallApi<SleepFn>("kernel32.dll", "Sleep", &HookedSleep, &g_origSleep);
```

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Lifetime and threading

**A `Hook` uninstalls itself when destroyed.** Store it as a member of your
plugin, not as a local — a local goes out of scope at the end of the function
and takes your detour with it.

Install in `OnDeviceCreated`, not in the constructor or `OnLoad`: by then the
game is past its own startup and the code you are patching has settled.

**Your detour runs on whichever thread called it** — for most game code, not the
render thread. If it touches state your `OnPresent` also touches, that needs
synchronising. Installing and removing are themselves thread-safe: other
threads are suspended across the patch.

**Always call through the original** unless you genuinely mean to delete the
function's behaviour. Forgetting is the most common way a hook "does nothing"
and breaks something else.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## When it fails

Every failure is logged with a reason, at Warn or Error level, so it appears in
`MWOn12-render.log` even without `VerboseLog=1`.

| Symptom | Usually |
|---|---|
| `hook: ... failed - InvalidAddress` | Wrong address, or you needed `Rva()` |
| `hook: ... failed - HookAlreadyExists` | Something else already hooked it |
| Prologue-related failure | Function too short to patch; hook its caller |
| `scan: ... matched nothing` | Pattern wrong, or wrong module |
| `scan: ... matched N places` | Pattern too short — make it longer |
| Crash on unload | A detour still installed after its `Hook` died |

`DebugLayer=1` does not help here — it only sees D3D12. Attach a debugger; your
plugin is an ordinary DLL.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Patching code the game is running

`Patch()` and `Nop()` write bytes while other threads may be executing them.
Nothing can make that safe. Patch during `OnDeviceCreated`, before the game is
busy, or hook the function instead of rewriting it.

`Nop()`'s `count` must cover **whole instructions**. A partial nop leaves the
tail of one instruction decoding as garbage. Check the instruction length in a
disassembler first.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## What is underneath

The real work — decoding the target's prologue with a full disassembler,
relocating the stolen bytes into a trampoline, suspending threads across the
patch — is done by libraries the SDK ships prebuilt, as x86 and static-CRT to
match MWOn12 and every plugin. Nothing needs fetching or building.

If they are missing from an install, CMake stops at configure time and names
the file it wanted, rather than dropping hooking and letting the build fail
later on symbols nobody recognises.

`mwon12::hooks` covers hooking, patching and scanning, which is what almost
every mod needs. More is available underneath, through VanHooks' own public
header `<vh/vh.hpp>` — already on your include and link lines:

| | |
|---|---|
| `vh::inline_hook` | what `hooks::Hook::Install` is built on |
| `vh::api_hook` | an export by module and name |
| `vh::vtable_hook` | one slot of a COM or C++ vtable |
| `vh::iat_hook` / `vh::plt_hook` | an import, before it is called |
| `vh::mid_hook` | mid-function, with every register in a `MidContext` |
| `vh::group` | install and lift a set of hooks as one unit |

VanHooks ships **prebuilt, binary-only**: a static library plus that curated
public header set. There is no source in this SDK and none is implied — see
[LICENSE.txt](../LICENSE.txt). The public API is the supported surface; anything
below it is not addressable from here and is not documented.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

<div align="center">

<sub>Built and maintained by <a href="https://github.com/TsyVM">TsyVM</a> · <a href="https://www.teamvanilla.org/">TeamVanilla</a></sub>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:6B4226,100:1C1008&height=80&section=footer"/>

</div>
