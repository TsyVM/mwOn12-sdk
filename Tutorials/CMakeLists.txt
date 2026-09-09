# Tutorials — worked examples, each a standalone .asi.
#
# No guards and no dependency keywords, as in the release package's
# samples/CMakeLists: every dependency is prebuilt, the top-level CMakeLists
# has already failed with a clear message if one is missing, and every
# tutorial links all four.
#
# mwon12_add_asi() is defined in the top-level CMakeLists.

# 01 — a graphical mod with an F12 menu (Gui + Overlay).
mwon12_add_asi(ScreenFX   01_ScreenFX/ScreenFX.cpp)

# 02 — a gameplay hook (VanHooks + MWSDK).
mwon12_add_asi(NoCops     02_NoCops/NoCops.cpp)

# 03 — live field edits through MWSDK's typed views.
mwon12_add_asi(SuperCars  03_SuperCars/SuperCars.cpp)

# 04 — reaching into the D3D12 render pipeline (VanGFX + Gui).
mwon12_add_asi(DrawFilter 04_DrawFilter/DrawFilter.cpp)
