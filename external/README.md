# external/

The release package ships these dependencies **prebuilt**, as libraries in
`lib/` and headers in `include/`. There are no sources here — this folder
carries their documentation, so that every `external/...` link in `docs/`
resolves in the package exactly as it does in the source tree.

| | what it is | its docs |
|---|---|---|
| **VanGFX** | graphics interception: draws, pipeline states, buffer readback | `vangfx/VanGFX-README.md`, `vangfx/VanGFX_Functions_Guide.md` |
| **MWSDK** | the game itself: verified addresses, typed views, file formats | `mwsdk/MWSDK-README.md`, `mwsdk/MWSDK-FUNCTIONS.md` |
| **VanGUI** | the widget toolkit behind `mwon12::Gui` | `vangui/VanGUI-README.md` |
| **VanHooks** | function hooking and memory patching | headers in `include/vanhooks/` |

To build any of them from source, or to change one, work in its own repository
and rebuild the package from there. Editing a header here changes only what your
plugin compiles against, not the library it links.
