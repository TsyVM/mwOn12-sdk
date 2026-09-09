<div align="center">

# The Game Itself

<p><em>MWSDK — verified addresses, typed object views, live attributes</em></p>

[![Library MWSDK](https://img.shields.io/badge/Library-MWSDK-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Game NFS:MW 2005 v1.3](https://img.shields.io/badge/Game-NFS:MW%202005%20v1.3-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Evidence verified RE](https://img.shields.io/badge/Evidence-verified%20RE-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![Header mw05.hpp](https://img.shields.io/badge/Header-mw05.hpp-D2B48C?style=for-the-badge&labelColor=1C1008)](#)
[![TeamVanilla](https://img.shields.io/badge/Team-TeamVanilla-D2B48C?style=for-the-badge&labelColor=1C1008)](https://www.teamvanilla.org/)

<br/>

### Contents

[Why it is worth using](#why-it-is-worth-using) · [What you get](#what-you-get) · [Rebasing: the one thing to get right](#rebasing-the-one-thing-to-get-right) · [Do not use MWSDK_MOD()](#do-not-use-mwsdkmod) · [Imported research](#imported-research) · [See also](#see-also)

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

Everything else in this SDK is about Direct3D. MWSDK is about **Most Wanted**:
where its functions and globals are, what its objects look like, and how to read
its files.

It is vendored under `external/mwsdk/`, built for x86 alongside the rest, and
part of the SDK — there is no keyword and nothing to enable. Include a header
and it works:

```cpp
#include <mwsdk/mwsdk.hpp>
```

Build `samples/GameSdk` first — it logs the live vehicle list and changes
nothing.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Why it is worth using

Reaching into the game without it looks like this:

```cpp
h.Install<Fn>(hooks::Rva(0x0067A410), &MyDetour, &g_orig, "something");
*reinterpret_cast<float*>(car + 0x1A4) *= 1.1f;
```

Two numbers nobody can check, that mean nothing to the next reader, and that are
wrong on any build but the one they were copied from. With MWSDK:

```cpp
mw05::vehicle(car).top_speed() *= 1.1f;
```

Every offset and address behind that is generated from verified
reverse-engineering data, and traceable back to the evidence for it.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## What you get

```cpp
#include <mwsdk/game/mw05.hpp>        // image, addresses, typed calls
#include <mwsdk/game/mw05_views.hpp>  // VehicleView, PursuitView, PerpView, ...
#include <mwsdk/game/mw05_easy.hpp>   // one-liners defaulting to the live process
#include <mwsdk/hashing.hpp>          // the engine's own lookup2 string hash
```

| | |
|---|---|
| **Verified addresses** | `mw05::fn::*`, `mw05::singleton::*` — byte-anchored, hand-verified functions and confirmed singleton globals. |
| **Typed object views** | `mw05::vehicle(p).top_speed()`, `mw05::perp(p).bustable()` — each a named alias over a verified offset. |
| **Live attributes** | `mw05::attrib(collection)` over the verified `AttribCollection::GetField` ABI, keyed by verified vault keys. Returns `std::expected`; a missing field is an error, not a crash. |
| **Object identity** | `mw05::identify(obj)` matches an object's vtable against the class database instead of assuming what a list holds. |
| **The engine's hash** | `hash::attrib("default") == 0xEEC2271A`, compile-time. Every vault key, class name and car name is this hash. |
| **File formats** | JDLZ, EAGL, TPK, geometry, vaults, scenery, triggers, traffic paths, collision — the offline half, usable from a plugin or a tool. |

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Rebasing: the one thing to get right

MWSDK stores addresses as **preferred-base** values (`Va`) and rebases them
against the running image. `mwon12::hooks::Rva()` rebases too. Do it once:

```cpp
// MWSDK has already rebased, so hooks must take the value as final.
h.Install<Fn>(hooks::Abs(mw05::process().rebase(mw05::fn::SomeFunction)),
              &MyDetour, &g_orig, "SomeFunction");
```

`hooks::Rva()` on an address MWSDK rebased applies the image base twice and
points at nothing. `Abs()` is the pairing.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Do not use `MWSDK_MOD()`

MWSDK's own entry-point macro creates a `DllMain` and starts a background
thread. That is right for a standalone `.asi` and wrong here: an MWOn12 plugin
already has a lifecycle, and `OnPresent` is a per-frame clock on the render
thread. Use `MWON12_PLUGIN` and MWSDK's *library*, never both entry points.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## Imported research

`mw05::db::research::` holds addresses imported from third-party research rather
than verified here — functions, globals, and mid-function patch sites, each in
its own sub-namespace. It is deliberately not mixed into `fn::` and
`singleton::`, so the difference in evidence is visible where you use it:

```cpp
mw05::fn::SomeFunction              // byte-anchored, hand-verified
mw05::db::research::fn::SetGPSDestination   // taken on trust
mw05::db::research::global::PlayerCount
mw05::db::research::patch::NOSTrailJumpSite // INSIDE a function; never call one
```

Names there are labels built from the research's own descriptions, not recovered
symbols.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

## See also

- **[hooking.md](hooking.md)** — installing the hook once MWSDK has told you where.
- **`external/mwsdk/MWSDK-README.md`** — the full SDK, including the file formats.
- **`external/mwsdk/MWSDK-FUNCTIONS.md`** — the function reference.

<img width="100%" src="https://capsule-render.vercel.app/api?type=rect&color=0:1C1008,50:6B4226,100:1C1008&height=3"/>

<div align="center">

<sub>Built and maintained by <a href="https://github.com/TsyVM">TsyVM</a> · <a href="https://www.teamvanilla.org/">TeamVanilla</a></sub>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:6B4226,100:1C1008&height=80&section=footer"/>

</div>
