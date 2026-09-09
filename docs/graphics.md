# Graphics and shaders

Three different things are called "a graphics mod", and they want three
different tools. Pick before you start — the wrong one costs a weekend.

| You want to | Use | Needs |
|---|---|---|
| Replace the game's shaders with your own HLSL | **[shader kit](../shaderkit/README.md)** | a text editor |
| Draw on top of the finished frame | **`mwon12::Overlay`** | ~10 lines of C++ |
| React to draws as they happen, inject effects, capture the G-buffer | **`<mwon12/graphics.hpp>`** | this page |

Most ideas are the first row. `ShaderDump=1`, `ShaderMods=1`, edit the dumped
`.hlsl`, restart. No compiler and no C++, and the result is a folder of text
files anyone can install. If that does what you want, stop reading here.

This page is the third row.

---

## What it is

`<mwon12/graphics.hpp>` attaches [VanGFX](#vangfx) to the D3D12 device MWOn12
is rendering the game with. From there a plugin sees the frame being built:
every draw as it is recorded, every pipeline state as it is created, every
resource as it appears — and can change or drop any of them.

```cpp
#include <mwon12/graphics.hpp>

class MyPlugin final : public mwon12::Plugin {
    mwon12::gfx::Context m_gfx;             // a member, not a local

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
        if (!m_gfx.Attach(d)) return;

        m_gfx->on_draw([](vangfx::FrameContext&, vangfx::DrawEvent& ev) {
            if (ev.index_count > 5000) ev.intercept = true;    // drop it
        });
    }

    void OnDeviceDestroyed() override { m_gfx.Detach(); }
};
```

`Attach` takes everything it needs from `MWOn12_DeviceInfo`. `m_gfx->` is the
`vangfx::Context` itself, so the whole VanGFX API is reached through it and this
header adds only a lifetime and a log route.

---

## The draws are MWOn12's, and that is the point

The game is a Direct3D 9 program. It never issues a D3D12 command — MWOn12
translates what it asks for and issues that. So what arrives in `on_draw` is
MWOn12's D3D12 traffic: one or more draws per D3D9 draw the game made, against
shaders MWOn12 produced by translating the game's D3D9 bytecode.

This is what makes any of it possible. A 2005 D3D9 renderer has no pipeline
state objects, no root signatures and no DXIL, and nothing modern can be
attached to it. MWOn12's translation is what turns the game's rendering into
something with those things in it.

The consequence to plan around is that the vocabulary is MWOn12's, not the
game's. A `DrawEvent` carries index counts and a PSO handle, not "this is a
car". Working out which draws are which is the real work, and the fastest route
is the crude one:

```cpp
m_gfx->on_draw([](vangfx::FrameContext&, vangfx::DrawEvent& ev) {
    if (ev.index_count > 5000) ev.intercept = true;
});
```

Drop a group, look at what vanished, narrow the predicate, repeat. The shader
kit's magenta trick is the same idea from the other end and the two work well
together: dump the shaders, colour one, and you have both the shader hash and
the draw shape for the same object.

---

## Lifetime

**Attach in `OnDeviceCreated`. Detach in `OnDeviceDestroyed`.** Both matter, and
the second one more.

Attaching patches vtable slots on MWOn12's device, command queue and swap chain.
Those patches point into your DLL. If they are still installed when the device
is destroyed and the plugin unloads, the next call through one of them lands in
freed memory — an access violation on exit with no stack that names you.

`gfx::Context` detaches in its destructor, so a plugin that keeps one as a
member is already safe. Call `Detach()` anyway: it happens at the moment the
renderer expects rather than whenever the plugin object is freed.

The same rule as everywhere else in this SDK, for the same reason.

---

## Threading

Callbacks arrive on the thread that made the D3D12 call. Under this game that is
effectively always the render thread, so callbacks do not race each other.

They are **not** the same call as `OnPresent`, and VanGFX does not serialise them
against it. State shared between a draw callback and `OnPresent` needs its own
lock, or an atomic — `samples/VanGfxProbe` uses atomics for exactly this.

A callback runs inside the game's frame. Whatever it does is frame time.

---

## Compiling shaders

```cpp
auto ps = mwon12::gfx::CompilePS(m_gfx, R"(
    float4 main(float4 pos : SV_Position) : SV_Target {
        return float4(1, 0, 1, 1);
    })");
```

`CompilePS` and `CompileVS` wrap `vangfx::ShaderBuilder` with MWOn12's logging,
defaulting to shader model 6.0. The builder itself is available for anything
more involved — `#define`s, include directories, a specific profile, a
precompiled blob:

```cpp
auto shader = vangfx::ShaderBuilder{}
                  .source_file("MyEffect.hlsl")
                  .entry("PSMain").profile("ps_6_2")
                  .define("QUALITY", "2")
                  .build(*m_gfx);
```

**Compile at attach time, never inside a callback.** Compilation spawns a
compiler and takes milliseconds; doing it during a draw is a visible hitch.

### `dxcompiler.dll`

Shader model 6.x compiles through DXC, which VanGFX loads with `LoadLibrary` at
the moment you first compile something. It is not linked, so a plugin that never
compiles a shader does not need it — but if it is absent when you do compile,
the call fails with:

```
dxcompiler.dll not found in process — required for SM 6.x shaders.
```

It ships with the Windows SDK and with the DXC releases, and it is not present on
a stock Windows install. **A shader mod built this way has to ship
`dxcompiler.dll` (and `dxil.dll`, for validation) beside the game.** A shader mod
built with the shader kit does not, because MWOn12 compiles those itself. That
alone is a good reason to check the shader kit first.

---

## Replacing a shader

This is how you change what the game looks like from C++.

D3D12 has no "bind this shader" call. A shader exists only inside a pipeline
state object, and a PSO is immutable once built — so a shader is replaced at the
moment MWOn12 builds the PSO, by substituting the bytecode before the driver
sees it.

**Step 1 — find the shader.** Log what the renderer builds:

```cpp
m_gfx->on_pso_create([](auto&, vangfx::d3d12::PSOCreateEvent& ev) {
    if (ev.kind != vangfx::d3d12::PSOKind::Graphics) return;
    mwon12::LogInfo("pso: ps %016llX, %u RTs, %u inputs",
                    mwon12::gfx::BytecodeHash(ev.graphics->ps),
                    ev.graphics->num_render_targets,
                    ev.graphics->input_element_count);
});
```

**Step 2 — replace it.**

```cpp
auto ps = mwon12::gfx::CompilePS(m_gfx, hlsl);
mwon12::gfx::ReplacePixelShader(m_gfx, ps,
                                mwon12::gfx::WhenPixelShaderIs(0xA1B2C3D4E5F60718));
```

`WhenPixelShaderIs` is one predicate; `PSOMatch` is any function of the
`GraphicsPSODesc`, so you can match on render-target count and format, input
layout, blend or depth state instead. Prefer matching on shape for anything you
intend to distribute: a hash is over MWOn12's *translated* bytecode and does not
have to survive an MWOn12 update. The shader kit's hashes are over the game's own
D3D9 bytecode and do, which is one more reason to check it first.

Install replacements in `OnDeviceCreated`, immediately after `Attach`. A PSO is
built the first time the renderer needs it, and anything installed later misses
every pipeline state built before it.

### Root signature compatibility

A replacement must use the same constant buffers, textures and samplers, in the
same registers, as the shader it replaces. MWOn12 built the root signature to
match the shader it translated, and replacing the bytecode does not change it.

Get it wrong and D3D12 refuses to create the pipeline state. MWOn12 logs the
refusal and the draws that needed that PSO do not render — so the symptom is
things disappearing, not the game crashing, and the log line names the shaders:

```
[dx12] CreateGraphicsPipelineState failed | vs=... ps=... il=... numRT=1 ...
```

Take the register declarations from the shader you are replacing — dump it with
`ShaderDump=1` and read the top of the file.

### `vangfx::replace_ps` and the per-draw route

`vangfx::replace_ps`, `vangfx::inject_ps_post` and the `ScopedShaderReplace` /
`ScopedInjection` guards work on D3D12, but not the way they work on D3D9 and
D3D11, and the difference is worth understanding before you rely on one.

On those APIs an effect is installed by binding it per draw — `SetPixelShader`,
`PSSetShader` — so a `ReplaceFilter` can decide draw by draw. D3D12 has no call
that binds a shader. A shader exists only inside a pipeline state object, a PSO
is immutable once created, and the one on a command list was chosen long before
a draw callback runs.

So on D3D12 they register a **substitution at pipeline-state creation**: the
next time the game builds a PSO, the bytecode is swapped before the driver
compiles it. Two consequences:

- **It is not per-draw.** A substitution applies to every pipeline state built
  while it is registered, and to every draw using one. A `ReplaceFilter` that
  selects on vertex count or target size cannot be honoured — none of that is
  known when a PSO is made.
- **It is not retroactive.** A PSO built before the bind keeps the shader it was
  built with. Bind during `OnDeviceCreated` and you catch everything; bind after
  a race has loaded and you catch only what has yet to be seen.

`gfx::ReplacePixelShader` is the same mechanism with the shape D3D12 actually
has, which is why it is still the one to reach for: it matches on the
`GraphicsPSODesc`, which is what a substitution can genuinely select on.

`vangfx::Uniforms`, `CaptureSession` and the G-buffer capture do not go through
that path and behave the same on every backend.

### Reading the frame's buffers

`FrameContext::readback` gets the bytes of anything the game has on the GPU:

`ResourceEvent` is where the handles come from, so collect them as the game
creates its buffers and read them later — at creation the buffer has been
allocated but not filled, and reading it then gets you whatever the allocator
left behind:

```cpp
std::vector<vangfx::IndexBufferHandle> m_ibs;   // filled below, read in OnPresent

m_gfx->on_resource([this](vangfx::FrameContext&, vangfx::ResourceEvent& ev) {
    if (ev.action == vangfx::ResourceAction::Create &&
        vangfx::is_valid(ev.index_buffer))
        m_ibs.push_back(ev.index_buffer);
});

m_gfx->on_present([this](vangfx::FrameContext& fc, vangfx::PresentEvent&) {
    if (m_ibs.empty() || m_dumped) return;
    m_dumped = true;

    // First 64 bytes, not the whole thing -- a readback stalls the GPU.
    if (auto r = fc.readback(m_ibs.front(), 0, 64)) {
        const auto* idx = reinterpret_cast<const uint16_t*>(r->bytes.data());
        mwon12::LogInfo("index buffer %llu: %llu bytes, first tri %u/%u/%u",
                        m_ibs.front().id, r->buffer_size,
                        idx[0], idx[1], idx[2]);
    } else {
        mwon12::LogWarn("readback failed: %s", r.error().message.c_str());
    }
});
```

There is an overload for each of `VertexBufferHandle`, `IndexBufferHandle`,
`ConstantBufferHandle`, `StructuredBufferHandle`, `RawBufferHandle` and
`UAVHandle` — the handles `ResourceEvent` hands out — plus the existing
`TextureHandle`, `RenderTargetHandle` and `DepthBufferHandle` overloads, which
return `ReadbackData` (pixels, width, height, format) rather than `BufferData`
(bytes).

Reading the index buffer behind a draw is how you find out what that draw *is*,
which is the first question any rendering mod has to answer: a frame is
thousands of draws and nothing labels which one is the car. Reading a constant
buffer is how you get the matrices.

`offset` and `size` are bytes; `size` of 0 reads to the end, and both are
clamped to the buffer, so asking for the first 64 bytes of everything you see
needs no special case for the short ones. **Ask for a window.** Each call blocks
until the GPU has finished the copy, and copying a 4 MB vertex buffer to look at
64 bytes of it costs the same stall as copying the 64 bytes.

### From a draw to its geometry

Collecting handles is fine when any buffer will do. Usually it will not: the
question is *which of these four thousand draws is the car*, and that means
starting from a draw.

`DrawEvent` carries the vertex and index buffer **views** the game set, and a
view is a GPU virtual address rather than a handle. `buffer_at` resolves one:

```cpp
m_gfx->on_draw([](vangfx::FrameContext& fc, vangfx::DrawEvent& ev) {
    if (!ev.d3d12_ib.bound || ev.index_count < 300) return;

    auto ref = fc.buffer_at(ev.d3d12_ib.view.gpu_virtual_address);
    if (!ref) return;                    // not a buffer VanGFX saw created

    // ref.offset is where this draw's indices sit inside that buffer -- rarely
    // zero, because a game packs many meshes into one large buffer.
    if (auto r = fc.readback(ref.as_index_buffer(), ref.offset, 96)) {
        mwon12::LogInfo("draw of %u indices -> buffer %llu +%llu of %llu",
                        ev.index_count, ref.id, ref.offset, ref.size);
    }
});
```

`ref.id` and `ref.offset` are the pair worth caching: the same mesh gives the
same two every frame, so once you have identified the car you can recognise it
again without reading anything back. `readback_at(address, size)` does the
resolve and the read in one call when you only want the bytes.

A readback stalls the GPU. This is a tool for identifying geometry once, not
something to do per draw per frame.

### Backend support

| | D3D12 | D3D11 | D3D9 |
|---|---|---|---|
| Texture / render target / depth readback | yes | yes | yes |
| Buffer readback by handle | yes | yes | vertex + index only |
| `buffer_at` / `readback_at` | yes | — | — |

D3D11 and D3D9 bind buffers as interface pointers, so there is no address for a
draw to carry and nothing for `buffer_at` to resolve; it returns an empty ref
rather than pretending. On D3D9 a buffer created `D3DUSAGE_WRITEONLY` — which is
most static geometry in a real game — cannot be read at all, and the error says
so rather than returning bytes that are not its contents.

None of that limits MWOn12, which is D3D12 throughout. It matters only if you
use VanGFX elsewhere.

See `external/vangfx/VanGFX-README.md` for the rest of the API.

---

## When Attach returns false

```cpp
if (!m_gfx.Attach(d)) return;
```

The usual reason is not a failure. MWOn12 falls back to a DirectX 9 passthrough
when it finds no usable D3D12 adapter, and there is no D3D12 device to attach to
in that case — `MWOn12_DeviceInfo::device` is null and this returns false with a
line in the log saying so. A plugin should carry on doing whatever it can
without graphics interception rather than treating it as fatal.

The other reasons are logged with their cause: a `minShaderModel` the device
cannot meet, or a vtable slot that could not be patched.

---

## VanGFX

Vendored under `external/vangfx/` and built for x86 alongside the SDK. Opt a
plugin in with the `VANGFX` keyword — it is a large static library and most
plugins have no use for it:

```cmake
mwon12_add_plugin(MyPlugin VANGFX MyPlugin/MyPlugin.cpp)
```

`<mwon12/graphics.hpp>` needs that keyword. `<mwon12/plugin.hpp>` and
`<mwon12/overlay.hpp>` do not.

---

## See also

- **[shaderkit/README.md](../shaderkit/README.md)** — shader replacement with no
  code, and the `.fx` sources that say what each of the game's shaders was for.
- **[plugin-api.md](plugin-api.md)** — the callbacks, `mwon12::Overlay`, and what
  is bound during `OnPresent`.
- **[hooking.md](hooking.md)** — reaching the game's own code, for the half of a
  mod that is not rendering.
- **`samples/VanGfxProbe`** — attaches, counts draws and PSOs, changes nothing.
  The thing to build first.
