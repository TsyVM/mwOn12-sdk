# MWSDK — Functions Guide

**Public API Reference · v0.1.0**
C++20 (data) · C++20/23 (runtime) · Windows x86 target · pure logic tests on any platform

---

One include is all you need:

```cpp
#include <mwsdk/mwsdk.hpp>     // everything
// or, for a live plugin/trainer DLL:
#include <mwsdk/mod.hpp>       // runtime layer + the MWSDK_MOD entry macro
```

Everything in this document lives in the `mwsdk::` namespace. The runtime layer
lives in `mwsdk::mw05::`. Bring them in with `using namespace mwsdk;` and write
`mw05::…`.

---

## Table of Contents

1. [Legend](#part-i--legend)
2. [Result type and error handling](#part-ii--result-type-and-error-handling)
3. [Level 1 — zero-setup one-liners](#part-iii--level-1)
4. [Level 2 — the explicit runtime API](#part-iv--level-2)
5. [Level 3 — raw calls, symbols, hooking](#part-v--level-3)
6. [Hashing](#part-vi--hashing)
7. [Offline file formats (data SDK)](#part-vii--offline-file-formats)
8. [Generated tables — addresses, layouts, vault keys](#part-viii--generated-tables)
9. [Error codes](#part-ix--error-codes)
10. [Quick reference card](#part-x--quick-reference-card)

---

## Part I — Legend

Seven words. Everything else is detail.

### IMAGE

The loaded `speed.exe` module, wrapped in `mw05::Image`. The game is linked for
base `0x00400000`; when Windows loads it somewhere else, the Image translates a
**preferred address (Va)** to the real one. You rarely construct an Image —
`mw05::process()` gives you the live one, and every one-liner defaults to it.

### VA

A **preferred virtual address** — an address as it appears in the verified
database, relative to base `0x00400000`. `mw05::Va` is a typed integer. The
Image rebases a Va to a live pointer for you, so a Va keeps working under
ASLR/relocation.

### FIELD / OFFSET

A member inside a live game object, addressed by its byte **offset** from the
object base (`mw05::Off`). `mw05::field<T>(obj, off)` returns a **reference** at
that offset — it reads *and* writes. Offsets come from the verified layout
tables (`mw05::layout::<Class>::<member>`).

### VIEW

An ergonomic wrapper over a live object that names its members:
`VehicleView(car).top_speed()`. Each accessor is a 1:1 alias over one verified
offset. A view is a single pointer; it owns nothing.

### ATTRIB

The engine's runtime tuning interface. An `AttribCollection` holds a car's
attributes; `mw05::Attrib` wraps one and reads/writes fields by their verified
`lookup2` key through the game's own `AttribCollection::GetField`.

### PATCH

A reversible byte edit on the live image. `mw05::Patch` is RAII: it saves the
original bytes on construction and restores them when it goes out of scope —
exactly like a self-removing hook. `mw05::nop(...)` / `mw05::patch(...)` return
one in a single call.

### RESULT

`Result<T>` is `std::expected<T, mwsdk::Error>`. Anything that can fail returns
one. No exceptions, no global error state, zero cost on the success path.

---

## Part II — Result type and error handling

Every fallible call returns `Result<T>` (a value or an `Error`), or `Status`
(success or an `Error`, no value).

```cpp
mwsdk::Result<float> r = car.get<float>(key);

if (!r) {
    std::printf("failed: %.*s\n",
                (int)mwsdk::error_to_string(r.error()).size(),
                mwsdk::error_to_string(r.error()).data());
    return;
}
float value = *r;          // or r.value()
```

Aliases and helpers:

| Name | Meaning |
|---|---|
| `Result<T>` | `std::expected<T, Error>` — a value or an error. |
| `Status` | `expected<void, Error>` — success or an error. |
| `err(Error)` | Shorthand that builds the failure value. |
| `error_to_string(Error)` | Stable human-readable name (`string_view`, static storage — do not free). |

Monadic chaining is available (`.and_then()`, `.transform()`,
`.transform_error()`), matching `std::expected`.

---

## Part III — Level 1

*Beginner — one line, zero setup. Everything defaults to the live process.*
*Header: `<mwsdk/game/mw05_easy.hpp>` (pulled in by `<mwsdk/mod.hpp>`).*

### Ergonomic views (pure — work anywhere)

```cpp
mw05::vehicle(car).top_speed()   *= 1.10f;   // +10% top speed
mw05::vehicle(car).drive_speed()  = 40.0f;
mw05::perp(perp).bustable()       = 0;        // never-busted toggle
mw05::cop_manager(mgr).fleet_cap();           // read the pursuit fleet cap
mw05::player(lp).vehicle();                   // the player's IVehicle*
```

| Function | Returns | Wraps |
|---|---|---|
| `mw05::vehicle(void*)` | `VehicleView` | `AIVehicle` |
| `mw05::traffic_action(void*)` | `TrafficActionView` | `AIActionTraffic` |
| `mw05::cop_manager(void*)` | `CopManagerView` | `AICopManager` |
| `mw05::pursuit(void*)` | `PursuitView` | `AIPursuit` |
| `mw05::perp(void*)` | `PerpView` | `AIPerpVehicle` |
| `mw05::player(void*)` | `PlayerView` | `LocalPlayer` |
| `mw05::racer_brain(void*)` | `RacerBrainView` | `AIRacerBrain` |

### Live attribute tuning (Windows)

```cpp
float mul = mw05::attrib(collection).get_or<float>(
                vault::schema::aivehicle::TopSpeedMultiplier, 1.0f);
mw05::attrib(collection).set<float>(
                vault::schema::aivehicle::TopSpeedMultiplier, mul * 1.25f);
```

`mw05::attrib(void* collection)` → an `Attrib` bound to the live process.

### Reversible byte patches (Windows)

```cpp
auto guard = mw05::nop(mw05::fn::SomeCheck, 6);   // 6× 0x90, reverts at scope end
auto mov   = mw05::patch(va, {0xB0, 0x01});       // mov al,1 — hold to keep live
```

| Function | Returns | Notes |
|---|---|---|
| `mw05::nop(Va, size_t len)` | `Patch` | Fills with `0x90`. Reverts on destruction. |
| `mw05::patch(Va, {bytes…})` | `Patch` | Writes your bytes. Reverts on destruction. |
| `mw05::poke(Va, {bytes…})` | `bool` | **Permanent** one-shot write, no revert. |
| `mw05::identify(const void*)` | `const db::ClassInfo*` | Identify a live object by vtable. |

---

## Part IV — Level 2

*The explicit runtime API — you hold the Image, you name every step.*
*Header: `<mwsdk/game/mw05.hpp>`.*

### The Image — rebasing

```cpp
const mw05::Image& game = mw05::process();       // the live speed.exe
std::uintptr_t live = game.rebase(mw05::fn::AttribCollection_GetField);
mw05::Va       va   = game.unbase(live);          // live pointer → preferred Va
float*         p    = game.ptr<float>(some_va);   // typed rebased pointer
```

| Method | Returns | Description |
|---|---|---|
| `base()` | `uintptr_t` | Actual load base. |
| `valid()` | `bool` | Base is non-zero. |
| `rebase(Va)` | `uintptr_t` | Preferred Va → live linear address. |
| `unbase(uintptr_t)` | `Va` | Live address → preferred Va (identify a pointer). |
| `ptr<T>(Va)` | `T*` | Typed pointer to a rebased Va. |

### Memory access (Windows)

```cpp
std::uint32_t v = mw05::read<std::uint32_t>(some_va);        // fast, unchecked
auto safe = mw05::safe_read<std::uint32_t>(some_va);         // Result<T>, SEH-guarded
mw05::write<float>(writable_va, 1.0f);                        // .data writes
```

| Function | Returns | Notes |
|---|---|---|
| `read<T>(Va)` / `read<T>(Image, Va)` | `T` | Unchecked. Precondition: mapped & readable. |
| `safe_read<T>(Va)` | `Result<T>` | `AddressUnreadable` on fault (SEH on MSVC). |
| `write<T>(Va, const T&)` | `void` | For writable pages; use `Patch` for code. |
| `singleton_ptr(Va)` | `void*` | Reads a singleton instance pointer from its global slot; the `0x0F0F0F0F` debug-fill (slot not yet populated) is reported as `nullptr`. |
| `road_network()` / `camera_ai()` / `g_manager()` | `void*` | The three confirmed singletons (`g_manager` is the D36 icon-system `GManager**` cell — `nullptr` until the icon system is up). |
| `vehicle_count()` / `vehicle_at(i)` | `u32` / `void*` | The game's own live vehicle list (D40, `0x92CD1C`/`0x92CD24`). Elements are **`IVehicle` interface pointers** — never cast to `PVehicle`. |
| `driver_class(iv)` | `DriverClass` | `IVehicle::GetDriverClass` via verified vtable slot 22 (`Human/Traffic/Cop/Racer/None/NIS/Remote`). |
| `player_vehicle()` | `void*` | The unique live vehicle with `DriverClass::Human` — the verified player-car resolution (replaces the older community `GetPlayerInstance` approach, whose `0x9352B0` table is dead on retail v1.3). Re-resolve every use; never cache. |
| `identify(Image, const void*)` | `const db::ClassInfo*` | Object class by vtable. |

### Typed object members

```cpp
float& top = mw05::field<float>(vehicle, mw05::layout::AIVehicle::mTopSpeed);
top *= 1.10f;
mw05::field<std::uint32_t>(perp, mw05::layout::AIPerpVehicle::mBustableFlag) = 0;
```

`field<T>(obj, off)` returns a reference (reads and writes). `obj` must be a live
object of the matching class — `identify()` it if unsure.

### Live attributes — `mw05::Attrib`

```cpp
mw05::Attrib car = mw05::Attrib::bind(game, collection);     // or Attrib::bind(collection)

bool  present = car.has(key);
auto  r       = car.get<float>(key);                 // Result<T>; FieldNotFound if absent
float m       = car.get_or<float>(key, 1.0f);        // fallback, never fails
Status st     = car.set<float>(key, 1.25f);          // FieldNotFound writes nothing
if (float* p  = car.ref<float>(key)) *p *= 1.10f;    // in-place edit; nullptr if absent
void* raw     = car.field_ptr(key, /*index=*/0);     // the underlying GetField primitive
```

| Method | Returns | Description |
|---|---|---|
| `Attrib::bind(Image, void* coll)` | `Attrib` | Bind to a live collection (rebased `GetField`). |
| `Attrib::bind(void* coll)` | `Attrib` | Same, process-defaulted. |
| `Attrib::with_resolver(void* coll, GetFieldFn)` | `Attrib` | Explicit resolver (the test/mock seam). |
| `valid()` | `bool` | Has a collection and a resolver. |
| `has(key, index=0)` | `bool` | Does the collection carry this field? |
| `get<T>(key, index=0)` | `Result<T>` | Typed read; `FieldNotFound` if absent. |
| `get_or<T>(key, fallback, index=0)` | `T` | Typed read with fallback. |
| `set<T>(key, value, index=0)` | `Status` | Typed write; `FieldNotFound` if absent. |
| `ref<T>(key, index=0)` | `T*` | Pointer into storage, or `nullptr`. |
| `field_ptr(key, index=0)` | `void*` | Raw storage pointer (the one live call). |

The key is a `lookup2`/`0xABCDEF00` attrib hash — use the verified constants in
`mwsdk::vault::schema::` or `hash::attrib("Name")`.

### Views (the classes behind the Level-1 helpers)

Every view derives from `ObjectView` and returns references:

```cpp
mw05::VehicleView v{car};
v.top_speed();  v.drive_speed();  v.accel_max_speed();  v.accel_table();  // float[10]
v.at<std::uint32_t>(mw05::layout::AIVehicle::slot_0x20);                   // generic slot
v.raw();  v.valid();  (bool)v;
```

| View | Named accessors |
|---|---|
| `VehicleView` | `top_speed`, `drive_speed`, `accel_max_speed`, `accel_table` |
| `TrafficActionView` | `target_speed`, `speed_limit`, `crash_state`, `crash_timer`, `recovery` |
| `CopManagerView` | `spawn_request_count`, `breaker_zone_count`, `active_fleet`, `fleet_cap` |
| `PursuitView` | `active`, `cops_damaged` |
| `PerpView` | `bust_gauge`, `bust_meter`, `bust_hold_timer`, `bustable`, `state` |
| `PlayerView` | `vehicle` |
| `RacerBrainView` | `skill_scalar` |

`ObjectView::at<T>(Off)` reaches any offset, including name-stripped `slot_0xNN`.

### Reversible patching — `mw05::Patch`

```cpp
{
    mw05::Patch p(game, mw05::fn::SomeCheck, ret, sizeof ret);  // saves originals
    // patch is live in this scope
}   // originals restored here (RAII)
```

| Method | Returns | Description |
|---|---|---|
| `Patch(Image, Va, const uint8_t*, size_t)` | — | Construct + apply, saving originals. |
| `apply(Image, Va, bytes, len)` | `bool` | (Re)apply; `false` if the page can't be made writable. |
| `nop(Image, Va, len)` | `bool` | Replace with `0x90`s. |
| `restore()` | `void` | Put the original bytes back now. |
| `active()` | `bool` | Is a patch currently applied? |

`Patch` is move-only (copy deleted). Max patch length 32 bytes. `mw05::poke(...)`
is the permanent, non-reverting one-shot equivalent.

---

## Part V — Level 3

*Raw calls, the symbol database, and optional hooking. Full control.*

### Calling engine functions — you choose the ABI

The x86 game uses `__thiscall` for methods and `__cdecl` for the Lua binders.
MWSDK never guesses an ABI for you.

```cpp
// __cdecl free function:
mw05::Fn<int(void* lua_state)> native(game, mw05::lua::Game::SetWorldHeat);
int rc = native(L);

// __thiscall method (first arg is the object):
mw05::ThisFn<void*(void* self, unsigned key, int index)>
    get_field(game, mw05::fn::AttribCollection_GetField);
void* storage = get_field(collection, 0xEC57E16B, 0);

// escape hatch — cast a Va to exactly the pointer type you name:
auto fp = mw05::as<int(MWSDK_THISCALL*)(void*)>(game, some_va);
```

| Type | ABI | Use |
|---|---|---|
| `Fn<R(A...)>` | `__cdecl` | Lua binder natives, `__cdecl` functions. |
| `ThisFn<R(Self,A...)>` | `__thiscall` | Engine methods (`Self` is the object). |
| `as<FnPtr>(Image, Va)` | you specify | Any signature, including `MWSDK_STDCALL`/`MWSDK_FASTCALL`. |
| `LuaCFunction` | `__cdecl` | `int(void* lua_State)` — the binder native shape. |

### Symbol / class database (pure — works anywhere)

```cpp
const db::Symbol*    s = mw05::find_symbol("AttribCollection::GetField");
const db::Symbol*    a = mw05::symbol_at(0x00454810);       // binary search by Va
const db::ClassInfo* c = mw05::class_info("AIVehicle");     // by name
const db::ClassInfo* d = mw05::class_by_vtable(0x00891998); // by vtable Va
```

Namespaces of verified constants: `mw05::fn::*` (functions), `mw05::lua::*`
(Lua-binder natives), `mw05::singleton::*` (confirmed global slots).

### Batch-managed patches — `mw05::group` (`mw05_group.hpp`)

A trainer usually has several switches, each backed by one or more byte patches.
`PatchGroup` owns many reversible patches and toggles them together. Every entry
is an RAII `Patch`, so the group reverts everything it owns when destroyed — you
cannot leak a live patch.

```cpp
auto trainer = mw05::group("Trainer");
trainer.nop  (mw05::fn::BustCheck, 6, "NeverBusted")
       .patch(mw05::fn::HeatWrite, {0x90, 0x90}, "FreezeHeat");

trainer.disable();                       // revert all, keep them registered
trainer.enable();                        // re-apply all (from stored descriptors)
if (auto* e = trainer.at("NeverBusted")) e->disable();   // toggle one switch
for (auto& e : trainer) { /* e.tag, e.enabled() */ }
// trainer's destructor reverts everything still live.
```

| Member | Returns | Description |
|---|---|---|
| `mw05::group(name = "")` | `PatchGroup` | Factory. |
| `patch(Va, {bytes…}, tag = "")` | `PatchGroup&` | Register + apply an explicit patch. |
| `nop(Va, len, tag = "")` | `PatchGroup&` | Register + apply a NOP run. |
| `enable()` / `disable()` | `PatchGroup&` | Re-apply / revert all (keeps registration). |
| `remove_all()` / `clear()` | `void` | Revert and drop all (still usable). |
| `at(tag)` | `PatchEntry*` | Find one switch by tag, or `nullptr`. |
| `size()` / `empty()` / `name()` | — | Introspection. |
| `enabled()` | `bool` | True only if every registered patch is applied. |
| `begin()`/`end()` | `PatchEntry*` | Range-for iteration. |

`PatchGroup` is move-only. Windows-only (it patches the live image), guarded by
`MWSDK_MW05_RUNTIME` like `Patch`. A `PatchEntry` exposes `va`, `tag`,
`enabled()`, `enable(Image)`, and `disable()`.

### Optional hooking — `mwsdk/adapters/vanhooks.hpp`

MWSDK never forces a hooking backend. The adapter is inert unless VanHooks is
present. When it is:

```cpp
#include <mwsdk/adapters/vanhooks.hpp>
static mw05::LuaCFunction orig = nullptr;
int detour(void* L) { /* ... */ return orig(L); }

auto h = mw05::hook::at(mw05::lua::Game::SetWorldHeat, &detour, &orig);  // RAII
```

For everything else, `mw05::Patch` gives you reversible edits with no dependency.

---

## Part VI — Hashing

The engine turns names into 32-bit keys. Use the right hash for the job — all
`constexpr`, so literals fold at compile time.

```cpp
using namespace mwsdk::hash::literals;
static_assert("default"_attrib            == 0xEEC2271A);   // verified game constant
static_assert("TopSpeedMultiplier"_attrib == 0xEC57E16B);
```

| Function | Algorithm | Use for |
|---|---|---|
| `hash::attrib(sv)` | Jenkins **lookup2**, seed `0xABCDEF00` | car/class/field/vault names (all attribute tuning) |
| `hash::lookup2(sv, seed)` | lookup2 with your seed | non-default seed |
| `hash::joaat(sv)` / `joaat_ci(sv)` | one-at-a-time | asset/texture/solid names (`_ci` = lower-cased) |
| `hash::bin(sv)` | sum hash, seed `0xFFFFFFFF`, ×33 | some bin/label keys, scenery groups |

`hash::kAttribSeed == 0xABCDEF00`. Literal suffixes: `"x"_attrib`, `"x"_joaat`.

---

## Part VII — Offline file formats

The data SDK reads/writes the game's files with zero OS dependencies. Every
parser returns `Result<T>` and owns nothing (you own the input bytes).

```cpp
#include <mwsdk/jdlz.hpp>
#include <mwsdk/eagl.hpp>
#include <mwsdk/vault.hpp>
using namespace mwsdk;

Result<std::vector<std::uint8_t>> raw = jdlz::decompress(file_bytes);
if (raw)
    eagl::walk(*raw, [](const eagl::Chunk& c, int depth) { return true; });

for (const vault::Record& r : vault::read_decoded(attributes_bin).value_or({}))
    std::string_view name = vault::field_name(r.field_hash);   // "" if stripped
```

| Module | Entry points |
|---|---|
| `jdlz` | `jdlz::decompress(Bytes) → Result<vector<uint8_t>>` |
| `eagl` | `eagl::walk(Bytes, visitor) → Status` (chunk container walk) |
| `tpk` | texture-pack reads (`tpk::…`) |
| `geometry` | solids / meshes |
| `vault` | `vault::read_decoded(Bytes) → Result<vector<Record>>`, `vault::field_name(key) → string_view` |
| `chunks` | `chunks::name(id) → string_view`, `chunks::handler(id) → const ChunkHandler*` (names any EAGL chunk) |
| `reflection` | primitive types (`by_hash`/`by_name`/`size_of`) **+ `runtime_name(hash)`**: reverse-resolve 565 verified class/message/enum/collection names |
| `scenery` | `scenery::read_sections(Bytes) → Result<vector<Section>>` (instances + info + cull tree) |
| `triggers` | `triggers::read_regions`/`read_markers`; `contains(region,x,z)` (even-odd point-in-region) |
| `paths` | `paths::read_track_paths` (traffic polylines); `paths::read_carp` (road graph nodes/segments) |
| `collision` | `collision::read_terrain` (dequantised triangles); `collision::read_smackables` |
| `core` | `ByteReader` (bounds-checked little-endian cursor), `fixed_string(Bytes)` |

---

## Part VIII — Generated tables

These are generated from the verified RE data (single source of truth), so they
never drift from the evidence. They compile on any platform.

| Table | Contents |
|---|---|
| `mw05::fn::*` | verified function Vas (e.g. `AttribCollection_GetField == 0x00454810`) |
| `mw05::lua::*` | Lua-binder native Vas |
| `mw05::singleton::*` | confirmed singleton global slots (`WRoadNetwork`, `CameraAI`) |
| `mw05::layout::<Class>::<member>` | live object member offsets (62 members, 16 classes) |
| `mw05::layout::kMembers` / `kMemberCount` | the full member table `{cls,name,type,offset,confidence}` |
| `mwsdk::vault::schema::<class>::<Field>` | 738 verified `lookup2` vault field keys (incl. recovered) |
| `mwsdk::vault::schema::kFields` / `kFieldCount` | the full vault-key table |
| `mw05::db::kSymbols` / `kClasses` | recovered symbols and reflected classes |
| `chunks::kChunks` | verified EAGL chunk-id -> handler registry (generated `chunk_registry.inl`) |

Confidence tags on layout members: `verified` (static/DE-IDA), `slot` (a getter
proves the member exists, name unknown → `slot_0xNN`), `behavior` (behavioural
RE). Names are never invented.

---

## Part IX — Error codes

All values of the `mwsdk::Error` enum (`uint32_t`). Use `error_to_string(e)`.

### General (1–19)

| Code | Value | Meaning |
|---|---|---|
| `Ok` | 0 | Not an error. |
| `InvalidArgument` | 1 | Null pointer or nonsensical value. |
| `Unsupported` | 2 | Not supported here. |
| `OutOfRange` | 3 | Index/offset outside bounds. |
| `NotImplemented` | 4 | Not implemented. |

### Buffer / IO (100–119)

| Code | Value | Meaning |
|---|---|---|
| `UnexpectedEndOfData` | 100 | Read past the end of the input. |
| `BufferTooSmall` | 101 | Output buffer too small. |
| `BadAlignment` | 102 | Misaligned access. |

### Container / compression / texture / geometry / vault (200–619)

| Code | Meaning |
|---|---|
| `BadChunkHeader`, `ChunkSizeOverflow`, `ChunkNotFound`, `NotAContainer` | EAGL container errors. |
| `BadMagic`, `UnsupportedVersion`, `DecompressedSizeMismatch`, `CorruptStream` | JDLZ errors. |
| `BadTpkStructure`, `UnsupportedPixelFormat`, `TextureNotFound` | TPK errors. |
| `BadSolidStructure`, `UnknownVertexStride` | Geometry errors. |
| `BadVaultHeader`, `UnknownVaultSection` | Vault errors. |

### Runtime / live process (700+)

| Code | Value | Meaning |
|---|---|---|
| `ModuleNotFound` | 700 | `speed.exe` module not found. |
| `AddressUnreadable` | 701 | `safe_read` faulted on the address. |
| `AddressUnwritable` | 702 | Target page could not be written. |
| `ProtectFailed` | 703 | `VirtualProtect` failed. |
| `SymbolNotFound` | 704 | Symbol name not in the database. |
| `NullObject` | 705 | A live-object pointer was null. |
| `FieldNotFound` | 706 | `AttribCollection::GetField` returned no storage for the key. |

---

## Part X — Quick reference card

### Include

```cpp
#include <mwsdk/mod.hpp>          // live plugin: runtime layer + MWSDK_MOD
using namespace mwsdk;
```

### Level 1 — one line

```cpp
mw05::vehicle(car).top_speed() *= 1.10f;                          // view
mw05::attrib(coll).set<float>(key, 1.25f);                        // live attrib
auto guard = mw05::nop(mw05::fn::SomeCheck, 6);                   // RAII patch
```

### Level 2 — explicit

```cpp
auto& game = mw05::process();
float& top = mw05::field<float>(car, mw05::layout::AIVehicle::mTopSpeed);
auto  r    = mw05::attrib(coll).get<float>(key);
mw05::Patch p(game, va, bytes, len);                              // reversible
```

### Level 3 — raw call / symbols / batch patches

```cpp
mw05::ThisFn<void*(void*,unsigned,int)> gf(game, mw05::fn::AttribCollection_GetField);
const db::ClassInfo* c = mw05::class_info("AIVehicle");

auto trainer = mw05::group("Trainer");
trainer.nop(mw05::fn::BustCheck, 6, "NeverBusted").disable();   // revert all on scope exit
```

### Plugin entry point

```cpp
void my_mod() { mod::Log log{"my_mod.log"}; /* ... */ }
MWSDK_MOD(my_mod)                            // the entire DLL entry point
```

### Error handling

```cpp
if (!r) std::printf("%.*s\n", (int)error_to_string(r.error()).size(),
                              error_to_string(r.error()).data());
```

---

*MWSDK Functions Guide · v0.1.0 · every address traceable to verified RE data.*
