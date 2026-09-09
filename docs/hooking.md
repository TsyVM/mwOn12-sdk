# Hooking and memory patching

`<mwon12/hooks.hpp>` reaches the game's own code. The render hook in
[plugin-api.md](plugin-api.md) reaches what the game *draws*; this reaches what
it *does* — gameplay, physics, AI, handling, damage, HUD logic. None of that
goes through Direct3D, so none of it is reachable any other way.

Built on [VanHooks](#vanhooks), which is shipped prebuilt with the SDK.

---

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

---

## `Fn` is a function type, not a pointer

This is the single most common compile error against this API.

```cpp
using DamageFn = void __fastcall(void*, void*, float);      // YES
using DamageFn = void(__fastcall*)(void*, void*, float);    // NO
```

`Install<Fn>` takes `Fn* detour` and `Fn** originalOut`. Hand it a type that is
already a pointer and you have asked for a pointer to a pointer, and it will not
compile. Declare the original as `DamageFn* g_orig`.

---

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

---

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

---

## Hooking an export by name

No address at all, for anything in a DLL's export table:

```cpp
using SleepFn = void WINAPI(DWORD);
static SleepFn* g_origSleep = nullptr;

m_hook.InstallApi<SleepFn>("kernel32.dll", "Sleep", &HookedSleep, &g_origSleep);
```

---

## Lifetime and threading

**A `Hook` uninstalls itself when destroyed.** Store it as a member of your
plugin, not as a local — a local goes out of scope at the end of the function
and takes your detour with it.

Install in `OnDeviceCreated`, not in the constructor or `OnLoad`: by then the
game is past its own startup and the code you are patching has settled.

**Your detour runs on whichever thread called it** — for most game code, not the
render thread. If it touches state your `OnPresent` also touches, that needs
synchronising. Installing and removing are themselves thread-safe: VanHooks
suspends other threads across the patch.

**Always call through the original** unless you genuinely mean to delete the
function's behaviour. Forgetting is the most common way a hook "does nothing"
and breaks something else.

---

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

---

## Patching code the game is running

`Patch()` and `Nop()` write bytes while other threads may be executing them.
Nothing can make that safe. Patch during `OnDeviceCreated`, before the game is
busy, or hook the function instead of rewriting it.

`Nop()`'s `count` must cover **whole instructions**. A partial nop leaves the
tail of one instruction decoding as garbage. Check the instruction length in a
disassembler first.

---

## VanHooks

The real work — decoding the target's prologue with a full disassembler,
relocating the stolen bytes into a trampoline, suspending threads across the
patch — is [VanHooks](https://www.teamvanilla.org/). The SDK ships it prebuilt:

```
include/vanhooks/*.hpp      the library
include/vh/*.hpp            the thin C++ wrappers over it
lib/vanhooks.lib
lib/Zydis.lib               ← the disassembler VanHooks decodes with
lib/Zycore.lib
```

All three libs are x86 and static-CRT, matching MWOn12 and every plugin.
**Zydis and Zycore are not optional**: `vanhooks.lib` references them and does
not contain them, so linking it alone fails on five unresolved Zydis symbols.
CMake checks for all three and stops at configure time if one is missing,
rather than letting the build fail later on symbols nobody recognises.

`mwon12::hooks` covers hooking, patching and scanning. VanHooks itself does more
— IAT and PLT hooks, vtable hooks, mid-function hooks, callstack tools. Include
`<vanhooks/vanhooks.hpp>` directly to reach it; the SDK already puts it on your
include and link lines.
