<p align="center">
  <img src="logo.png" alt="MW Tools — Most Wanted SDK" width="612">
</p>

# MWSDK

**A verified, modern C++ modding SDK for Need for Speed: Most Wanted (2005), PC retail v1.3.**

MWSDK gives you two things no other Most Wanted toolkit gives you together: a
**zero-dependency library for reading and writing the game's files**, and a
**header-only runtime layer for writing live plugins/trainers** — with every
single game address traceable to a byte-level, verified reverse-engineering
data set. Nothing in this SDK is guessed. If it's here, it was proven.

```cpp
#include <mwsdk/mod.hpp>
using namespace mwsdk;

void my_mod() {
    auto& game = mw05::process();                       // rebased for ASLR
    void*  rn  = mw05::road_network();                  // confirmed singleton
    auto*  cls = mw05::identify(game, rn);              // what class is this?
    // hash a car/attribute name the way the engine does (compile-time):
    static_assert(hash::attrib("default") == 0xEEC2271A);
}
MWSDK_MOD(my_mod)                                       // <- entire DLL entry point
```

---

## Why another SDK?

The excellent [NFSPluginSDK](https://github.com/berkayylmao/NFSPluginSDK) has
been the go-to for BlackBox-era plugins for years, and it's mature and
battle-tested across MW/Carbon/ProStreet. MWSDK is not a replacement for its
breadth — it's a different bet: **provenance and tooling over hand-crafted
breadth**, plus an offline file layer the plugin SDKs don't have.

| | NFSPluginSDK | **MWSDK** |
|---|---|---|
| Games | MW / Carbon / ProStreet | **MW05 (focused, deep)** |
| Source of truth | Hand-crafted from leaked builds | **Machine-readable verified RE data → generated headers (DRY)** |
| Address provenance | Curated by hand | **Every Va traceable to byte-anchored evidence; 287 hand-verified functions** |
| Offline file formats | — | **JDLZ, EAGL, TPK, geometry, vaults + world data: scenery, triggers, traffic paths, CARP, terrain collision, smackables** |
| Typed layouts | Hand-written structs (MW/Carbon/PS) | **Generated from verified RE data: 62 live member offsets + 738 `lookup2`-exact vault field keys (incl. 14 recovered by our own verified pipeline), provenance-tagged** |
| Live attribute read/write | Hand-authored `Attrib` accessors | **`mw05::Attrib` over the *verified* `AttribCollection::GetField` ABI (`0x00454810`, `thiscall(key,index)`), keyed by the 738 verified `lookup2` keys — `std::expected`, no crashes on a missing field** |
| Ergonomic one-liners | `car->TopSpeed = x` style | **Typed views: `VehicleView(v).top_speed() *= 1.1f`, `PerpView(p).bustable() = 0` — each a 1:1 alias over a verified offset, zero invented addresses** |
| Engine string hash | — | **Correct `lookup2`/AttribStringKey, compile-time, verified `default→0xEEC2271A`** |
| Error handling | Return codes / raw | **`std::expected` (`Result<T>`), no exceptions, no global state** |
| Hooking | Bring your own | **Self-contained RAII byte-patcher + *optional* VanHooks adapter (never forced)** |
| Verification | Manual | **Compile-time `static_assert`s against game constants + unit tests, warning-clean at `-O2 -Werror`** |
| Dependencies | Header-only | **Zero (data lib is pure C++20; runtime is header-only)** |
| C++ | C++20 | **C++20 (C++23 only if you opt into the VanHooks adapter)** |

MWSDK now ships **typed layouts** for the vehicle + pursuit family too
(`mw05::layout::AIVehicle::mTopSpeed`, `mwsdk::vault::schema::pvehicle::…`), and
matches NFSPluginSDK on its two headline features: **live attribute read/write**
(`mw05::Attrib` over the verified `GetField` ABI) and **ergonomic one-liners**
(`mw05::VehicleView`/`PerpView`/…) — but built from the Encyclopedia's verified
RE data rather than hand-written, so every offset, key, and address is traceable,
and name-stripped members are exposed honestly as `slot_0xNN` rather than guessed.
Where NFSPluginSDK is still ahead: it covers three games (MW/Carbon/ProStreet)
and its hand-authored structs name some members MWSDK still lists as `slot_*` —
a data-source gap (leaked symboled builds), not an engineering one. MWSDK's bet
is provenance — you're never building on a guess, and everything regenerates as
the RE data improves.

---

## The two pillars

```
mwsdk/                      one umbrella: #include <mwsdk/mwsdk.hpp>
├── Data SDK  (static lib, offline, zero OS deps)
│   core       byte reader, Result<T>/Error vocabulary
│   hashing    lookup2/attrib (engine hash), joaat, bin  — all constexpr
│   jdlz       JDLZ (de)compression
│   eagl       EAGL chunk container walker
│   chunks     chunk-id -> handler names (verified registry)
│   tpk        texture packs
│   geometry   solids / meshes
│   vault      attribute vaults
│   reflection reflection primitive type table
│   scenery    scenery instances + info + cull tree
│   triggers   trigger regions (+ even-odd test) + markers
│   paths      traffic paths + CARP road graph
│   collision  terrain collision mesh + smackables
└── Runtime SDK (header-only, live speed.exe — Windows)
    game/mw05  verified address database, rebasing, typed calls,
               memory access, object identity, byte patching
    adapters/  optional VanHooks glue (opt-in)
    mod.hpp    one-macro plugin entry point
```

### World data — decoded and verified against retail

Beyond the classic asset formats, the data SDK now reads the **world** itself,
each parser verified byte-for-byte against retail `L2RA.BUN` / `STREAML2RA.BUN`:

| Module | What it decodes | Verified against retail |
|---|---|---|
| `scenery` | placed props (64-byte instances), model info (LOD solid hashes), the per-section cull tree | **947 sections, 28,525 infos, 6,833 LOD models** |
| `triggers` | polygon trigger volumes + the engine's even-odd point-in-region test; 3-D position markers | **705 regions, 328 markers** |
| `paths` | traffic waypoint polylines; the CARP road graph (nodes + segments) | **443 paths / 4,136 waypoints; 4,385 nodes / 6,538 segments** |
| `collision` | terrain collision triangle soup (s16, dequant /4,/4,/16); smackable prop spawners | **51,504 triangles; 13,484 smackables** |
| `chunks` | names every EAGL chunk id from the verified handler registry | 28 handlers |

Try it: `mwsdk_world_dump TRACKS/L2RA.BUN` prints the whole inventory.

The runtime layer is built from `include/mwsdk/game/mw05_db.inl`, a **generated**
file: `tools/gen_mw05_db.py` reads the verified RE tables and emits the address
database, so the JSON stays the single source of truth (the Constitution's DRY
rule). Regenerate whenever the RE data improves — you never hand-edit addresses.

Current database: **1,754 recovered symbols** (296 hand-verified functions +
verified data globals under `mw05::data::*`), **124 reflected classes** (vtable +
object size), **126 Lua-binder natives**, and **3 confirmed singletons**
(`WRoadNetwork`, `CameraAI`, and — new 2026-07-09 — `GManager`).

---

## Ease of use — a three-level API

MWSDK is built to be as easy to reach for as a good hooking library. There are
three levels; start at the top and only go deeper when you need to.

- **Level 1 — one-liners, zero setup.** Everything defaults to the live process,
  so you never hold an `Image` yourself.

  ```cpp
  mw05::vehicle(car).top_speed() *= 1.10f;                       // +10% top speed
  mw05::attrib(coll).set<float>(vault::schema::aivehicle::TopSpeedMultiplier, 1.25f);
  auto guard = mw05::nop(mw05::fn::SomeCheck, 6);                // reversible patch
  ```

- **Level 2 — explicit.** Hold the `Image`, name every step: `mw05::field<T>`,
  `mw05::Attrib::bind`, `mw05::Patch`, `mw05::read/safe_read/write`.

- **Level 3 — raw.** Call engine functions with the ABI you declare
  (`mw05::Fn` / `mw05::ThisFn`), query the symbol database, batch many reversible
  patches with `mw05::group("Trainer")`, or hook via the optional VanHooks adapter.

Full API in **[FUNCTIONS.md](FUNCTIONS.md)**. Building a mod with Claude?
**[SKILL.md](SKILL.md)** is an installable skill that teaches it the whole flow.

---

## Quick start

### Install — drop-in, no build system required

The **runtime SDK is header-only**: copy `include/` into your project, include
`<mwsdk/mod.hpp>`, and build a **32-bit** DLL (retail `speed.exe` is x86). No
library to link, no dependencies.

```cmake
# Runtime-only (header-only): just add the include dir.
target_include_directories(my_mod PRIVATE ${MWSDK_DIR}/include)

# Offline file formats too? Link the tiny data lib (also zero-dependency):
add_subdirectory(${MWSDK_DIR} mwsdk)
target_link_libraries(my_mod PRIVATE MWSDK::game)   # runtime; or MWSDK::data for files
```

Or, in an MSVC project: **Additional Include Directories** → add `include\`,
set the target to **Win32 (x86)**, done.

### Build from source (data lib + tests)

Zero dependencies. Any C++20 compiler.

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
build/mwsdk_selfcheck            # prints the verification summary
```

### Use the data SDK (offline)

```cpp
#include <mwsdk/jdlz.hpp>
#include <mwsdk/eagl.hpp>
using namespace mwsdk;

Result<std::vector<std::uint8_t>> raw = jdlz::decompress(file_bytes);
if (raw)
    eagl::walk(*raw, [](const eagl::Chunk& c, int) { return true; });

// Decode a car's attribute vault and resolve field names by verified key:
for (const vault::Record& r : vault::read_decoded(attributes_bin).value_or({}))
    if (r.class_hash == hash::attrib("chassis") && r.collection_key == hash::attrib("carreragt"))
        printf("%.*s\n", (int)vault::field_name(r.field_hash).size(),
                          vault::field_name(r.field_hash).data());
```

### Write a live mod (runtime SDK)

```cpp
#include <mwsdk/mod.hpp>
using namespace mwsdk;

void my_mod() {
    mod::Log log{"my_mod.log"};
    auto& game = mw05::process();

    // Live attribute tuning over the VERIFIED AttribCollection::GetField ABI.
    // Keys are verified lookup2 hashes; a missing field is a typed error, never
    // a crash:
    mw05::Attrib car = mw05::Attrib::bind(game, collection /* your AttribCollection* */);
    float mul = car.get_or<float>(vault::schema::aivehicle::TopSpeedMultiplier, 1.0f);
    car.set<float>(vault::schema::aivehicle::TopSpeedMultiplier, mul * 1.25f);   // +25%

    // Reversible byte patch (RAII toggle — restores on scope exit):
    std::uint8_t nop[] = {0x90, 0x90};
    mw05::Patch p(game, mw05::fn::SomeFunction, nop, sizeof nop);
}
MWSDK_MOD(my_mod)
```

Build it as a **32-bit** DLL (retail speed.exe is x86) and inject with your
loader of choice. See `examples/hello_mod` for a complete, safe, read-only mod.

### Read/write a verified object member (typed layouts)

```cpp
// Every offset below is generated from the Encyclopedia (Discovery 34).
AIVehicle* v = /* identify() a live vehicle */;

// Ergonomic view — the one-liner NFSPluginSDK users expect, but every accessor
// is a 1:1 alias over a verified offset (no invented addresses):
mw05::VehicleView(v).top_speed() *= 1.10f;                               // +10% top speed
mw05::PerpView(perp).bustable() = 0;                                     // never-busted toggle

// The explicit form is always there too:
float& top = mw05::field<float>(v, mw05::layout::AIVehicle::mTopSpeed);   // +0x84, verified

// Tuning by attrib key (what AttribCollection::GetField wants):
constexpr auto k = mwsdk::vault::schema::pvehicle::HandlingRating;        // == attrib("HandlingRating")
```

### Hook a function (optional VanHooks adapter)

```cpp
#include <mwsdk/adapters/vanhooks.hpp>   // inert unless VanHooks is present
static mw05::LuaCFunction orig = nullptr;
int detour(void* L) { return orig(L); }

auto h = mw05::hook::at(mw05::lua::Game::SetWorldHeat, &detour, &orig);
// h is RAII; the hook lifts when it drops. VanHooks is never required by core.
```

---

## Grounding & verification

Every address resolves back to The Most Wanted Encyclopedia's
`RE-Data-And-Discoveries` set. The build proves the core invariants at compile
time — these `static_assert`s won't compile if the engine hash ever drifts:

```cpp
static_assert(hash::attrib("default")            == 0xEEC2271A);
static_assert(hash::attrib("TopSpeedMultiplier") == 0xEC57E16B);
static_assert(hash::attrib("gameplay")           == 0x5CEA9D46);
```

`mwsdk_selfcheck` and the CTest suite re-verify the hashes, the sorted symbol
table, binary-search lookups, and vtable-based object identity at runtime.

---

## Scope & ethics

MWSDK targets **single-player, offline** modding of the retail PC release:
tools, trainers, visual/tuning mods, and research. It is not affiliated with
EA. Reverse-engineering data is used for interoperability. Don't use it to gain
an unfair advantage in online/multiplayer play.

## License

MIT. See `LICENSE`.
