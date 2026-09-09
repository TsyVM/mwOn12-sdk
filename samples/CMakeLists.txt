# Sample plugins, release package.
#
# No availability guards and no dependency keywords: the package ships every
# dependency prebuilt, the top-level CMakeLists has already failed with a clear
# message if one is missing, and every plugin links all four.

# The one to build first. Draws nothing; logs the frame rate. If this appears
# in MWOn12-render.log, the install is correct.
mwon12_add_plugin(FrameStats   FrameStats/FrameStats.cpp)

# Real GPU work: a framerate bar drawn over the game via mwon12::Overlay.
mwon12_add_plugin(HelloOverlay HelloOverlay/HelloOverlay.cpp)

# Reaching the game's own code: hooking, patching, pattern scanning.
mwon12_add_plugin(GameHooks    GameHooks/GameHooks.cpp)

# Watching the frame being built: draws, pipeline states, present.
mwon12_add_plugin(VanGfxProbe  VanGfxProbe/VanGfxProbe.cpp)

# The game itself: verified addresses, typed object views, the engine's hash.
mwon12_add_plugin(GameSdk      GameSdk/GameSdk.cpp)

# An ASI that draws a frame-rate bar and reads game state at the same time.
mwon12_add_asi(HelloAsi        HelloAsi/HelloAsi.cpp)

# A real user interface: sliders, live tuning, the VanGUI demo window.
mwon12_add_plugin(HelloGui     HelloGui/HelloGui.cpp)

# The same source file again, as an ASI — one source, two entry points, which
# is what docs/asi.md claims and this is what keeps the claim honest. A separate
# target name because CMake targets are unique; OUTPUT_NAME puts it back.
mwon12_add_asi(HelloGuiAsi     HelloGui/HelloGui.cpp)
target_compile_definitions(HelloGuiAsi PRIVATE HELLOGUI_AS_ASI=1)
set_target_properties(HelloGuiAsi PROPERTIES OUTPUT_NAME "HelloGui")
