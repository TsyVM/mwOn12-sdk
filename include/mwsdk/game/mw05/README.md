# mw05 — reconstructed live-object reference layer (`mwsdk::mw05::rt`)

This folder holds **community-reverse-engineered, `[reference]`-confidence**
layouts for the live objects of *Need for Speed: Most Wanted* (2005),
`speed.exe` v1.3 — full C++ struct definitions for ~230 engine types (the
`AIVehicle` family, the `Attrib` vault system incl. `AttribCollection`, physics
/ rigid-body / suspension interfaces, the front-end database, and more), plus
the fixed-address global `Variables` and per-type `Extensions` helpers.

It is deliberately kept **separate** from MWSDK's verified core:

| Layer | Namespace | Confidence | Source of truth |
|-------|-----------|------------|-----------------|
| Verified core | `mwsdk::mw05` (`db`, `layout`, views) | `[verified]` / `[behavior]` / `[slot]` | Generated from the Encyclopedia RE datasets. Every address/offset traceable. |
| Reference layer | `mwsdk::mw05::rt` | `[reference]` | Reconstructed full-struct layouts, offered as leads. |

Why the split: the generated `mw05_db.inl` / `mw05_layouts.inl` tables promise
that *every* value is traceable to verified RE data. The reconstructed structs
here are enormously useful for *knowing the shape* of an object, but their
member offsets are implied by declaration order and by container ABI, so they
are not byte-guaranteed. Keeping them in `rt` means consuming them never dilutes
the "verified" promise of the core tables.

## Using it

```cpp
#include <mwsdk/game/mw05/all.hpp>   // opt-in; not included by <mwsdk/mwsdk.hpp>

// e.g. the field resolver behind live tuning:
mwsdk::mw05::rt::Attrib::Collection* c = /* ... */;
float* mul = c->GetData<float>("TopSpeedMultiplier");
```

For anything you intend to ship against, cross-check offsets/addresses against
the verified core (`mwsdk::mw05::db::fn`, `mwsdk::mw05::layout::*`). The port
report generated alongside this import lists every address that agrees with, or
conflicts with, the verified tables.

## Self-containment

The layer depends only on the C++ standard library via two small MWSDK shims in
`compat/` (`eastl.hpp` maps the four EA-STL containers used to `std::` ones;
`memory.hpp` provides the tiny page-write / global-field surface a few helpers
use). No external SDK is required.
