# MWOn12 Tutorials

Four small mods, each a single `.asi`, each about one thing you can do to
*Need for Speed: Most Wanted (2005)* now that MWOn12 runs it on Direct3D 12.

Most Wanted is a Direct3D 9 game with no plugin interface. MWOn12 is a `d3d9.dll`
that translates the game's D3D9 into D3D12 and, along the way, hands mods three
things the original game never could:

- **the finished frame**, on a live D3D12 command list, once per frame;
- **the render pipeline**, every draw and pipeline state as it is recorded;
- **the game itself**, through MWSDK — verified function addresses and typed
  object layouts, so a mod reads and writes the game's own data by name.

These tutorials take one of those each. They are `.asi` mods rather than plugins
on purpose: one folder to install into, one file to read, and the graphical ones
prove that an `.asi` gets the D3D12 device exactly as a plugin does.

| # | Tutorial | Shows | Uses |
|---|----------|-------|------|
| 01 | **ScreenFX** | A full-screen colour grade with an **F12 menu** | `mwon12::Gui` + `mwon12::Overlay` |
| 02 | **NoCops** | Stop the police spawning — a gameplay hook | `mwon12::hooks` + MWSDK |
| 03 | **SuperCars** | Boost top speed on a key press | MWSDK typed views |
| 04 | **DrawFilter** | Cull draws live from an **F12 menu** — inside the pipeline | `mwon12::Gui` + `mwon12::gfx` (VanGFX) |

Read them in order: 01 draws *over* the game, 04 reaches *inside* its rendering,
and 02–03 leave the picture alone and change the game.

---

## Building

The tutorials build with the SDK. From the SDK root:

```bat
Build.bat
```

Each tutorial is emitted as a `.asi` into `build_win32\bin\scripts\`. To build
them without the samples, or on their own:

```bat
cmake -S . -B build_win32 -G "Visual Studio 17 2022" -A Win32
cmake --build build_win32 --config Release --target ScreenFX NoCops SuperCars DrawFilter
```

Turn the whole set off with `-DMWON12SDK_BUILD_TUTORIALS=OFF`.

## Installing

Copy the `.asi` you want from `build_win32\bin\scripts\` into either of:

```
<game>\scripts\
<game>\MWOn12\ASI\
```

Run the game. The two with a menu (ScreenFX, DrawFilter) open on **F12**; the
gameplay ones (NoCops, SuperCars) read `MWOn12.ini` and, for SuperCars, a hotkey.
Set `VerboseLog=1` in `MWOn12.ini` to see each mod's log lines in
`MWOn12-render.log`.

Every mod has its own `[Section]` in `MWOn12.ini` — see the header comment at
the top of each `.cpp` for its keys.

---

## The four, in a sentence each

**01 · ScreenFX** — `mwon12::Overlay` composites a translucent, tinted quad the
size of the screen over the finished frame, and `mwon12::Gui` gives you a panel
(colour picker, intensity, presets) to drive it live. The whole "post-process"
is one blended rectangle; the point is how little stands between you and drawing
on the game.

**02 · NoCops** — a detour on `AICopManager::UpdateSpawnRequests` (a *verified*
address from MWSDK) zeroes the pending cop-spawn count on the way in and lets the
original run, so the pursuit system stays consistent but never creates a car.
The safe shape of a gameplay mod: starve a behaviour of its input rather than
amputate the function.

**03 · SuperCars** — a hotkey walks the live vehicle list and multiplies
`top_speed()` and `accel_max_speed()` — two verified offsets with names on them
instead of `*(float*)(car + 0x1A4)`. No hook, no drawing, no thread of its own:
it rides OnPresent as a once-per-frame clock.

**04 · DrawFilter** — `mwon12::gfx` (VanGFX) offers every D3D12 draw to a
callback before the GPU sees it; a menu slider drops the ones above an index
count, peeling the world away a layer at a time. This is the pipeline a real
shader mod plugs into.

---

## Going further

- **Replace a shader, not just cull draws.** DrawFilter watches draws; the
  `on_pso_create` hook the `VanGfxProbe` sample prints is what
  `mwon12::gfx::ReplacePixelShader` targets — match a pipeline state by its
  pixel-shader hash and substitute HLSL of your own. See `docs/graphics.md`.
- **Edit the game's shaders as text, with no code at all.** Set `ShaderDump=1`
  and `ShaderMods=1` in `MWOn12.ini`, edit the dumped `.hlsl`, restart. See
  `shaderkit/README.md`. Most "graphics mod" ideas are this.
- **Reach more of the game.** MWSDK's typed views cover vehicles, the cop
  manager, pursuits, the player and more; `MWOn12_Functions_Guide.md` and the
  `mwsdk` docs list what is verified and what is still `[behavior]`-confidence.
- **A menu for a gameplay mod.** Nothing stops NoCops or SuperCars from carrying
  an `mwon12::Gui` panel of their own — ScreenFX and DrawFilter show the two
  lines it takes.

Each tutorial's `.cpp` header comment is the long-form version of its row above;
start there.
