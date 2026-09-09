# Shader kit

Replace the game's shaders with your own HLSL. No compiler, no C++, no build —
edit a text file and restart the game.

This works because MWOn12 does not run the game's D3D9 shader bytecode. It
translates each shader to HLSL and compiles that for D3D12. The shader-mod
feature simply lets you supply the HLSL instead.

---

## The workflow

**1. Turn both switches on** in `MWOn12.ini`:

```ini
ShaderDump=1
ShaderMods=1
```

They are meant to be used together and left on. `ShaderDiskCache` is ignored
while either is set, so an edited shader is never masked by a cached copy of
the original.

**2. Play.** Drive around, open the menus, visit the areas you care about.
Shaders are dumped as they are actually used, so anything you never saw is
never written.

Each becomes a file in:

```
<game>\MWOn12\Shaders\Dumped\
```

**3. Copy one up a level** into `<game>\MWOn12\Shaders\` — the parent folder,
not `Dumped`. Dumping only ever writes the generated original, and only into
`Dumped`, so it can never overwrite your work.

**4. Edit it, and restart the game.** Files are read at launch.

If your edit does not compile, MWOn12 logs it once and uses the original. A bad
edit can never stop the game running:

```
[shadermods] ps_2b91f0c4a17d3e05.hlsl failed to compile - the original is used
```

---

## File names

```
vs_<16 hex digits>.hlsl                vertex shader
ps_<16 hex digits>.hlsl                pixel shader
ffpvs_<16 hex>.hlsl                    fixed-function vertex shader
ffpps_<16 hex>.hlsl                    fixed-function pixel shader

ps_<16 hex>_v<8 hex>.hlsl              one variant of a pixel shader
```

The hex is a hash of the game's own D3D9 bytecode — not of anything
backend-specific — so a name is stable across builds and across machines. A
fixed-function shader has no bytecode, so its permutation key is hashed
instead.

**The variant suffix matters.** One game shader becomes several compiled
shaders, differing in alpha test, fog mode and sampler kinds. MWOn12 looks for
the exact variant file first, then falls back to the name without `_v...`:

- `ps_<hash>.hlsl` — affects **every** variant of that shader.
- `ps_<hash>_v<variant>.hlsl` — affects **only** that one.

Start with the generic name. Reach for a variant only when a change is right in
one case and wrong in another.

---

## Finding the shader you want

The dump is a folder of hashes, which tells you nothing about what draws what.
Two ways to narrow it:

**Delete and repopulate.** Empty `Dumped`, restart, and go straight to the one
thing you care about. What appears is what that scene used.

**Change it obviously.** Make the pixel shader return solid magenta and see
what turns magenta:

```hlsl
return float4(1, 0, 1, 1);
```

Crude, and much faster than reading two hundred files.

---

## `game-shader-sources/`

The original **FX shader sources** shipped with the game's renderer —
`world.fx`, `car.fx`, `visualtreatment.fx`, `sky.fx`, and the rest, with their
shared headers.

These are **not** what MWOn12 loads. They are the D3D9-era originals, kept here
as a map: they name things, and they show what a given effect was meant to do.
When a dumped shader is an unreadable wall of `r0.xyzw`, the corresponding `.fx`
here is usually what makes it make sense.

| File | Roughly |
|---|---|
| `world.fx`, `worldbone.fx`, `worldnormalmap.fx`, `worldreflect.fx` | World geometry |
| `car.fx`, `carnormalmap.fx`, `glassreflectshader.fx` | Vehicles |
| `visualtreatment.fx` | The post-process look — heat haze, colour grading |
| `hdr.fx`, `shadowscreenfilter.fx`, `lightscattering.h` | Screen-space passes |
| `sky.fx`, `trees.fx`, `grass.fx`, `waterseashader.fx` | Environment |
| `particles.fx`, `shadowvolume.fx`, `ZPrePass_fx.h` | Effects and depth |
| `global.h`, `shadowmap_fx*.h`, `auxiliarylighting*.h` | Shared includes |

---

## Notes

- Shaders are read **at launch**. There is no hot reload; restart after editing.
- Keep the entry point's signature and semantics. MWOn12 binds constants,
  textures and samplers to the registers the translated shader declared — change
  those and the bindings no longer match.
- Turn `ShaderDump` and `ShaderMods` back to `0` when you are finished. Both
  bypass the disk cache, so every shader is recompiled at each launch.
- Distributing a shader mod means shipping the `.hlsl` files and telling people
  to put them in `<game>\MWOn12\Shaders\` with `ShaderMods=1`.
