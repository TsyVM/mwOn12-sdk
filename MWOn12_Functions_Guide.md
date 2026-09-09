# MWOn12 Functions Guide

Every function the SDK offers, in one place, with the signature you will
actually type. The prose walkthroughs live in `docs/`; this is the reference you
come back to. For finished, buildable examples of these functions working
together, see `Tutorials/` (four standalone `.asi` mods).

**Everything is x86.** `speed.exe` is 32-bit, so a plugin is too. CMake stops a
64-bit build rather than letting you discover it as a silent load failure.

| | |
|---|---|
| [Plugin lifecycle](#plugin-lifecycle) | `mwon12::Plugin`, `MWON12_PLUGIN` |
| [Host services](#host-services) | logging, config, directories, backend |
| [ASI mods](#asi-mods) | `MWON12_ASI`, registering at runtime |
| [Overlay](#overlay) | rectangles over the finished frame |
| [User interface](#user-interface) | `mwon12::Gui` + VanGUI widgets |
| [Hooking](#hooking) | functions, memory, pattern scanning |
| [Graphics](#graphics) | draws, pipeline states, shader replacement, buffers |
| [The C ABI](#the-c-abi) | what the renderer actually passes |
| [Build reference](#build-reference) | CMake, what needs a keyword |

---

## Plugin lifecycle

```cpp
#include <mwon12/plugin.hpp>

class MyPlugin final : public mwon12::Plugin { /* ... */ };
MWON12_PLUGIN(MyPlugin)
```

`MWON12_PLUGIN` goes at file scope in exactly one translation unit. It emits the
exported entry point, constructs your class, and hands ownership to the
renderer, which deletes it after `OnShutdown`.

### `class mwon12::Plugin`

Every hook has a do-nothing default; override only what you need.

| | called | notes |
|---|---|---|
| `const char* Name() const` | at load | shown in the log. Always provide one. |
| `const char* Version() const` | at load | free-form, e.g. `"0.3.1"` |
| `bool OnLoad()` | once, before any device | return `false` to decline; the DLL is unloaded and that is **not** an error |
| `void OnDeviceCreated(const MWOn12_DeviceInfo&)` | device exists | create GPU resources **here** |
| `void OnResize(const MWOn12_DeviceInfo&)` | swap chain changed | rebuild anything sized to the back buffer |
| `void OnPresent(const MWOn12_Frame&)` | once per frame | the command list is open and the back buffer is bound |
| `void OnDeviceDestroyed()` | device about to go | release **every** GPU object |
| `void OnShutdown()` | process exit | called even if no device ever existed |

**The one rule.** Create GPU resources in `OnDeviceCreated`; release all of them
in `OnDeviceDestroyed`. Not in the constructor — there is no device when your
plugin is loaded. Not after `OnDeviceDestroyed` returns — the device is
destroyed immediately afterwards, and anything still holding a reference takes
the process down with a live-object report: a crash on exit with no stack
pointing at you.

**Threading.** Callbacks arrive on the game's render thread and never
concurrently with each other for the same plugin.

---

## Host services

From `<mwon12/plugin.hpp>`, usable anywhere after load.

```cpp
mwon12::LogTrace(fmt, ...);          // only with VerboseLog=1
mwon12::LogInfo (fmt, ...);          // only with VerboseLog=1
mwon12::LogWarn (fmt, ...);          // always recorded
mwon12::LogError(fmt, ...);          // always recorded
mwon12::Log(MWON12_LOG_INFO, fmt, ...);
```

printf formatting, written to `MWOn12-render.log` tagged with your plugin's
name.

```cpp
int         ConfigInt   (section, key, fallback);
std::string ConfigString(section, key, fallback = "");
```

Reads `MWOn12.ini`, the same file the renderer reads. **Use your own
`[Section]`**, not `[Renderer]`, so the user has one config file rather than one
per mod.

```cpp
std::string GameDirectory();     // where speed.exe lives, trailing backslash
std::string PluginDirectory();   // <game>\MWOn12\Plugins\
```

Use these rather than a relative path. A plugin's working directory is the
game's, which is not where the plugin is — "it works on my machine" for that
reason is the most common way a mod fails to find its own files.

```cpp
MWOn12_Backend ActiveBackend();   // MWON12_BACKEND_DX12 or _DX9_PASSTHROUGH
const char*    RendererVersion(); // e.g. "1.0.0"
const MWOn12_Host* Host();        // the raw table, if you want it
```

`MWON12_BACKEND_DX9_PASSTHROUGH` means no D3D12 device exists and
`OnDeviceCreated` will never fire. A plugin that only draws should decline from
`OnLoad` rather than sit there doing nothing.

---

## ASI mods

An `.asi` is a DLL with a different extension and no required exports, loaded
from `<game>\scripts\` or `<game>\MWOn12\ASI\`. It loads **before the game
initialises**, which a plugin does not.

```cpp
#include <mwon12/asi.hpp>

class MyMod final : public mwon12::Plugin { /* the same class */ };
MWON12_ASI(MyMod)
```

`MWON12_ASI` emits a `DllMain` that starts a thread and registers you with the
renderer — so an ASI gets the D3D12 device once per frame exactly as a plugin
does. Swap the macro for `MWON12_PLUGIN` and the same source file is a plugin;
`samples/HelloGui` is built both ways from one file.

| | |
|---|---|
| `bool asi::Available()` | is the loaded `d3d9.dll` MWOn12? |
| `const MWOn12_Host* asi::Host()` | the host table, or null |
| `bool asi::Register(Plugin*)` | register; takes ownership, deletes on any failure path |
| `void asi::Unregister(Plugin*)` | stop callbacks; rarely wanted |
| `HMODULE asi::Module()` | the renderer's module, or null |

**Registration is deferred.** MWOn12 takes the plugin on at the next safe point
in the frame and calls `OnDeviceCreated` there, on the render thread. `Register`
returning true means *accepted*, not *running*. Create nothing in the call.

`Register` returning false — MWOn12 absent, or a rejected ABI version — is not
fatal. `MWON12_ASI` keeps the mod loaded and running; it just never gets a frame
callback, which is exactly right for the gameplay half of a mod.

### The raw C entry points

For an ASI that wants no SDK at all. Resolve off `d3d9.dll`:

```c
MWOn12_RegisterPlugin      /* int  (const MWOn12_Plugin*) */
MWOn12_UnregisterPlugin    /* void (void* user)           */
MWOn12_GetHost             /* const MWOn12_Host* (void)   */
```

---

## Overlay

`<mwon12/overlay.hpp>`. Filled rectangles over the finished frame. Not a UI
toolkit and not trying to become one — for controls, see the next section.

```cpp
bool Init(ID3D12Device*, DXGI_FORMAT rtvFormat, uint32_t framesInFlight,
          uint32_t maxRectsPerFrame = 2048);
void Shutdown();
bool Ready() const;

void Begin(const MWOn12_Frame&);                       // reads size + frame slot
void Rect(float x, float y, float w, float h, uint32_t rgba);
void RectOutline(float x, float y, float w, float h, uint32_t rgba,
                 float thickness = 1.0f);
void Submit(ID3D12GraphicsCommandList*);

uint32_t RectsThisFrame() const;
bool     Overflowed() const;      // a rect was dropped; raise maxRectsPerFrame
```

Coordinates are **pixels, origin top-left**. Colours are **`0xRRGGBBAA`**; alpha
blends. `rtvFormat` must match `MWOn12_DeviceInfo::backBufferFormat` or the
pipeline state is rejected at creation.

---

## User interface

`<mwon12/gui.hpp>`. VanGUI is **part of the SDK** — no build keyword. Its API is
Dear ImGui's under different names, so anything written for ImGui translates by
replacing `ImGui::` with `VanGui::`.

```cpp
bool Init(const MWOn12_DeviceInfo&);   // heap, both backends, window subclass
void Shutdown();
bool Ready() const;

bool Begin(const MWOn12_Frame&);       // false = hidden; build nothing
void End(const MWOn12_Frame&);         // records the draw calls

void SetVisible(bool);  bool Visible() const;  void ToggleVisible();
void SetToggleKey(int vk);             // 0 disables; VK_INSERT, VK_F1, ...
bool WantsInput() const;               // UI has the mouse or keyboard
void SetBlockGameInput(bool);          // default true
```

```cpp
void OnPresent(const MWOn12_Frame& f) override {
    if (!m_gui.Begin(f)) return;
    VanGui::Begin("Tuner");
    VanGui::SliderFloat("Top speed", &speed, 1.0f, 2.0f);
    VanGui::End();
    m_gui.End(f);
}
```

**`Begin()` returning false means build nothing and do not call `End()`.** There
is no frame to end, and VanGUI asserts if you end one it did not start.

`VanGui::ShowDemoWindow(&open)` puts every widget the library has on screen next
to the source line that draws it — the fastest documentation there is.

Three things to know: only **one plugin per process** can own the window
subclass (the first to `Init`; a second draws but gets no input and logs so);
input blocking cannot stop a game reading the keyboard through `GetAsyncKeyState`
or DirectInput, **which is how Most Wanted drives**; and on the D3D9 passthrough
`Init` returns false because there is no D3D12 device — the D3D9 backend ships,
but you would drive it yourself. See [docs/gui.md](docs/gui.md).

---

## Hooking

`<mwon12/hooks.hpp>`, built on VanHooks. Reaches the game's own code — gameplay,
physics, AI, none of which touches Direct3D.

### Addresses

```cpp
uintptr_t Rva(uintptr_t address, const char* module = nullptr);  // rebased
uintptr_t Abs(uintptr_t address);                                // as final
uintptr_t ModuleBase(const char* module = nullptr);
```

`Rva()` applies the real load address — use it for anything you read out of a
disassembler. `Abs()` takes the number as final, which is what you want for an
address **MWSDK has already rebased**:

```cpp
hooks::Abs(mw05::process().rebase(mw05::fn::SomeFunction))
```

`Rva()` on an MWSDK address applies the image base twice and points at nothing.

### Detours

```cpp
hooks::Hook h;                                   // a member, never a local
template <typename Fn>
bool h.Install(uintptr_t target, Fn* detour, Fn** originalOut = nullptr,
               const char* name = nullptr);
bool h.InstallApi(std::string_view module, std::string_view symbol, ...);
void h.Remove();
bool h.Installed() const;
```

**`Fn` is the function type, never a pointer to one.** `Install` takes `Fn*` as
the detour and `Fn**` as the original, so a pointer typedef makes both one level
too deep and the call does not compile:

```cpp
using DamageFn = void __fastcall(void*, void*, float);      // YES
using DamageFn = void(__fastcall*)(void*, void*, float);    // NO
static DamageFn* g_orig = nullptr;                          // note the *

static void __fastcall MyDamage(void* self, void* edx, float amount) {
    g_orig(self, edx, amount * 0.5f);                       // half damage
}

h.Install<DamageFn>(hooks::Rva(0x0067A410), &MyDamage, &g_orig, "TakeDamage");
```

The `Hook` owns the detour and lifts it on destruction — a local one uninstalls
itself at the closing brace, which looks like "my hook does nothing".

### Memory

```cpp
bool Patch(uintptr_t address, std::initializer_list<uint8_t> bytes,
           const char* what = nullptr);
bool Nop  (uintptr_t address, size_t count, const char* what = nullptr);
std::vector<uint8_t> Read(uintptr_t address, size_t size);
template <typename T> bool Write(uintptr_t address, const T& value, ...);
```

### Scanning

```cpp
std::optional<uintptr_t> Scan   (std::string_view pattern, const char* module = nullptr);
std::vector<uintptr_t>   ScanAll(std::string_view pattern, const char* module = nullptr);
```

`"55 8B EC 83 EC ?? 56 8B F1"` — `??` is a wildcard. A pattern survives a game
update that moves the function; a fixed address does not.

`Scan` returns an **optional**, and empty means either no match *or* more than
one: a pattern that silently returned its first hit is how a mod ends up
patching the wrong function on someone else's machine. Widen the pattern rather
than taking the first of several.

```cpp
if (auto addr = hooks::Scan("55 8B EC 83 EC ?? 56 8B F1"))
    h.Install<Fn>(*addr, &MyDetour, &g_orig, "TakeDamage");
```

See [docs/hooking.md](docs/hooking.md).

---

## Graphics

`<mwon12/graphics.hpp>`. Attaches VanGFX to the D3D12 device MWOn12 renders
with. **Needs the `VANGFX` keyword** — it is a large static library and most
plugins have no use for it.

```cmake
mwon12_add_plugin(MyPlugin VANGFX MyPlugin/MyPlugin.cpp)
```

### Context

```cpp
mwon12::gfx::Context m_gfx;                       // a member, never a local

bool Attach(const MWOn12_DeviceInfo&, uint32_t minShaderModel = 0,
            vangfx::LogLevel = vangfx::LogLevel::Warn);
void Detach();
bool Attached() const;
vangfx::Context* Get();          // also operator-> and operator bool
```

`Attach` returning false is usually **not** a failure: MWOn12 is on its D3D9
passthrough and there is no D3D12 device. Carry on without interception.

### Callbacks

```cpp
m_gfx->on_draw      ([](vangfx::FrameContext&, vangfx::DrawEvent& ev)     { ... });
m_gfx->on_pso_create([](auto&, vangfx::d3d12::PSOCreateEvent& ev)         { ... });
m_gfx->on_present   ([](auto&, vangfx::PresentEvent&)                     { ... });
m_gfx->on_resource  ([](auto&, vangfx::ResourceEvent& ev)                 { ... });
```

`ev.intercept = true` in a draw callback drops that draw — the crudest and
fastest way to find out what a group of draws is responsible for.

### Shader replacement

```cpp
vangfx::Shader CompilePS(Context&, std::string hlsl,
                         std::string entry = "main", std::string profile = "ps_6_0");
vangfx::Shader CompileVS(Context&, std::string hlsl,
                         std::string entry = "main", std::string profile = "vs_6_0");

uint64_t BytecodeHash(const vangfx::d3d12::ShaderBytecodeView&);
PSOMatch WhenPixelShaderIs (uint64_t hash);
PSOMatch WhenVertexShaderIs(uint64_t hash);

void ReplacePixelShader (Context&, vangfx::Shader replacement, PSOMatch);
void ReplaceVertexShader(Context&, vangfx::Shader replacement, PSOMatch);
```

Install replacements in `OnDeviceCreated`, right after `Attach`: a PSO is built
the first time the renderer needs it, and anything installed later misses every
pipeline state built before it.

A replacement must use the same constant buffers, textures and samplers, in the
same registers, as the shader it replaces — MWOn12 built the root signature to
match the original, and replacing the bytecode does not change it. Get it wrong
and the pipeline state is refused, the draws that needed it do not render, and
the log names the shaders. **Check the shader kit first**: if all you want is to
edit the game's shaders as text, `shaderkit/` does it with no C++ at all.

### Reading the frame's buffers

```cpp
Result<BufferData> fc.readback(VertexBufferHandle,     offset = 0, size = 0);
Result<BufferData> fc.readback(IndexBufferHandle,      offset = 0, size = 0);
Result<BufferData> fc.readback(ConstantBufferHandle,   offset = 0, size = 0);
Result<BufferData> fc.readback(StructuredBufferHandle, offset = 0, size = 0);
Result<BufferData> fc.readback(RawBufferHandle,        offset = 0, size = 0);
Result<BufferData> fc.readback(UAVHandle,              offset = 0, size = 0);

BufferRef          fc.buffer_at(uint64_t gpu_virtual_address);
Result<BufferData> fc.readback_at(uint64_t gpu_virtual_address, uint64_t size);
```

`size` of 0 reads to the end; both are clamped to the buffer. Handles come from
`ResourceEvent`.

`buffer_at` is the bridge from a draw to its geometry — `DrawEvent` carries
vertex and index buffer *views*, which are GPU addresses, not handles:

```cpp
auto ref = fc.buffer_at(ev.d3d12_ib.view.gpu_virtual_address);
if (ref) auto r = fc.readback(ref.as_index_buffer(), ref.offset, 96);
```

`ref.id` + `ref.offset` are stable per mesh across frames — cache that pair once
you have identified something. **A readback stalls the GPU**: this identifies
geometry once, it is not a per-frame operation.

Images use `readback(TextureHandle | RenderTargetHandle | DepthBufferHandle)`
and return `ReadbackData` (pixels, width, height, format, row pitch) instead.

See [docs/graphics.md](docs/graphics.md) and
`external/vangfx/VanGFX_Functions_Guide.md`.

---

## The C ABI

`<mwon12/mwon12.h>` is the single source of truth — MWOn12 compiles this exact
file, and there is deliberately no second copy in the renderer's tree.

### `MWOn12_DeviceInfo`

`device` (`ID3D12Device*`), `commandQueue`, `swapChain` (`IDXGISwapChain4*`),
`d3d9Device`, `hwnd`, `width`, `height`, `backBufferFormat` (`DXGI_FORMAT`),
`frameCount`.

### `MWOn12_Frame`

Everything above plus `commandList` (**open and recording**), `backBuffer`
(already in `RENDER_TARGET`, bound as the sole target with no depth buffer),
`backBufferRtv`, `frameSlot` (`0 .. frameCount-1`), `frameNumber` (monotonic).

Every `void*` is a borrowed COM pointer: do not `Release` one, and do not keep
one past `OnDeviceDestroyed`.

**During `OnPresent` you may** freely change pipeline state, root signature,
descriptor heaps, render targets, viewport and topology — the renderer
re-establishes all of it. **You must not** close, reset or submit the command
list, or leave the back buffer in any state but `RENDER_TARGET`.

Index per-frame resources by `frameSlot`, or you will overwrite memory the GPU
is still reading — intermittently, which is the worst way for that bug to
present.

---

## Build reference

```cmake
mwon12_add_plugin(Name [VANGFX] [MWSDK] sources...)   # -> bin/Plugins/Name.dll
mwon12_add_asi   (Name [VANGFX] [MWSDK] sources...)   # -> bin/scripts/Name.asi
```

| header | keyword | why |
|---|---|---|
| `plugin.hpp`, `overlay.hpp`, `asi.hpp`, `gui.hpp` | none | part of the SDK |
| `hooks.hpp` | none | VanHooks is prebuilt and always linked when found |
| `graphics.hpp` | **`VANGFX`** | a large static library most plugins never use |
| `<mwsdk/...>` | **`MWSDK`** | the game's addresses, layouts and file formats |

VanGUI needs no keyword: it is compiled into `MWOn12SDK.lib`, and it costs
nothing to a plugin that does not reference it — the linker takes only what is
used, and `FrameStats.dll` is byte-for-byte the same size either way.

`VANGUI` is still accepted and ignored, so a CMakeLists written against an
earlier SDK keeps working.

---

## See also

- **[Tutorials/README.md](Tutorials/README.md)** — four finished `.asi` mods, one idea each.
- **[docs/writing-a-plugin.md](docs/writing-a-plugin.md)** — the walkthrough.
- **[docs/plugin-api.md](docs/plugin-api.md)** — callbacks and the overlay in prose.
- **[docs/asi.md](docs/asi.md)** · **[docs/gui.md](docs/gui.md)** ·
  **[docs/hooking.md](docs/hooking.md)** · **[docs/graphics.md](docs/graphics.md)** ·
  **[docs/mwsdk.md](docs/mwsdk.md)**
- **[shaderkit/README.md](shaderkit/README.md)** — replacing shaders with no code.
