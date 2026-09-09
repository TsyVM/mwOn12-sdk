cmake_minimum_required(VERSION 3.20)
project(MWOn12SDK CXX)

# ─────────────────────────────────────────────────────────────────────────────
#  MWOn12SDK — release package
#
#  Headers and prebuilt libraries. Nothing here is compiled from source except
#  your own plugins, so a build is a few seconds rather than a few minutes.
#
#  Everything the SDK offers is in include/ and lib/. There is no external/
#  folder to populate, no submodule to initialise and no dependency to fetch:
#  VanHooks, VanGFX, MWSDK and VanGUI are all part of the package and all four
#  are linked into every plugin.
#
#  Needs Visual Studio 2022 with the C++ workload. No DirectX SDK, no network,
#  no package manager.
# ─────────────────────────────────────────────────────────────────────────────

set(MWON12SDK_VERSION "1.0.0")

# C++23, because VanHooks and VanGFX are both built on std::expected. The
# shipped libraries were compiled at this standard; a plugin that uses neither
# still has to match, because MWOn12SDK.lib itself was built this way.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── x86 only ─────────────────────────────────────────────────────────────────
# speed.exe is 32-bit, so a plugin must be too. Every library here is x86; a
# 64-bit build would fail at link, or worse, produce a DLL that silently never
# loads. Stop here rather than let either happen.
if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64" OR
   CMAKE_GENERATOR_PLATFORM STREQUAL "AMD64" OR
   (NOT CMAKE_GENERATOR_PLATFORM AND CMAKE_SIZEOF_VOID_P EQUAL 8))
    message(FATAL_ERROR
        "MWOn12 plugins must be built for Win32 (x86) — speed.exe is 32-bit.\n"
        "Use Build.bat, or pass -A Win32 to CMake.")
endif()

message(STATUS "MWOn12SDK ${MWON12SDK_VERSION} (release package) — Win32 / x86")

option(MWON12SDK_BUILD_SAMPLES "Build the sample plugins" ON)

set(MWON12SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(MWON12SDK_LIB  "${MWON12SDK_ROOT}/lib")

foreach(required
        MWOn12SDK.lib vangfx.lib vanhooks.lib Zydis.lib Zycore.lib
        mwsdk_data.lib vangui.lib)
    if(NOT EXISTS "${MWON12SDK_LIB}/${required}")
        message(FATAL_ERROR "Missing ${required} in ${MWON12SDK_LIB}. "
                            "The release package is incomplete.")
    endif()
endforeach()

# ── The SDK, as one thing to link ────────────────────────────────────────────
#
# An INTERFACE target rather than an IMPORTED one: it carries the include path,
# the whole set of libraries, and the compile settings a plugin must match, so
# a plugin links MWOn12SDK and inherits all of it.
#
# All four dependencies are in here, and there is no second target to opt into.
# VanGFX and MWSDK used to be separate, on the theory that a plugin should not
# pay for a library it does not use. It does not pay for one: the linker pulls
# object files out of a static library only when something references them, so
# a plugin that never names vangfx::, mw05:: or VanGui:: links none of that
# code. FrameStats.dll is the same size with all four on the line as it was
# with two. What opt-in actually cost was a person following graphics.md and
# getting a page of unresolved externals because the keyword is documented
# somewhere else.
#
# Zydis and Zycore are listed explicitly. vanhooks.lib references them and does
# not contain them — VanHooks decodes a target function's prologue to relocate
# it, and that decoder is Zydis. Linking vanhooks.lib alone fails on five
# unresolved Zydis symbols.
#
# The VanGUI backend headers sit in their own include directory because gui.hpp
# asks for them unqualified, the way VanGUI's own sources do.
#
# The system libraries after them are the ones VanGFX's backends and codecs
# reference; every one ships in the Windows SDK, dxcompiler.lib included, so
# none of this adds a prerequisite. An import library contributes nothing to a
# DLL that references no symbol from it — a plugin that never compiles a shader
# model 6 shader gets no dxcompiler.dll import and no new runtime dependency.
add_library(MWOn12SDK INTERFACE)

target_include_directories(MWOn12SDK INTERFACE
    "${MWON12SDK_ROOT}/include"
    "${MWON12SDK_ROOT}/include/vangui_backends")

target_link_libraries(MWOn12SDK INTERFACE
    "${MWON12SDK_LIB}/MWOn12SDK.lib"
    "${MWON12SDK_LIB}/vangui.lib"
    "${MWON12SDK_LIB}/vangfx.lib"
    "${MWON12SDK_LIB}/mwsdk_data.lib"
    "${MWON12SDK_LIB}/vanhooks.lib"
    "${MWON12SDK_LIB}/Zydis.lib"
    "${MWON12SDK_LIB}/Zycore.lib"
    d3d12.lib
    d3d11.lib
    d3d9.lib
    dxgi.lib
    dxguid.lib
    d3dcompiler.lib
    dxcompiler.lib
    xaudio2.lib
    dsound.lib
    windowscodecs.lib
)

# All 1, and they stay defined. Nothing in the SDK's own headers reads them any
# more — there is no configuration in which one is 0 — but a plugin written
# against an older SDK may still test them, and that code should keep compiling
# and keep taking the branch that works.
target_compile_definitions(MWOn12SDK INTERFACE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    MWON12_HAVE_VANHOOKS=1
    MWON12_HAVE_VANGFX=1
    MWON12_HAVE_MWSDK=1
    MWON12_HAVE_VANGUI=1
)

# ── mwon12_add_plugin(name source...) ────────────────────────────────────────
#
# Builds one plugin DLL with everything a plugin needs: x86, the static CRT,
# the SDK linked, and the output dropped in bin/Plugins ready to copy into the
# game.
#
# There are no dependency keywords. Every source file can include any SDK
# header and every plugin links every library:
#
#   mwon12_add_plugin(MyPlugin MyPlugin.cpp)
#
# The static CRT is not a preference. The shipped libraries were built /MT, and
# a plugin on the shared CRT both fails to link against them and needs that CRT
# present on the player's machine.
function(mwon12_add_plugin name)
    set(sources ${ARGN})

    # VANGFX, MWSDK and VANGUI used to opt a plugin into one of the libraries.
    # All three are part of the SDK now, so all three mean nothing -- accepted
    # and dropped rather than rejected, so a CMakeLists written against an
    # older SDK keeps working.
    foreach(legacy VANGFX MWSDK VANGUI)
        if("${legacy}" IN_LIST sources)
            list(REMOVE_ITEM sources "${legacy}")
        endif()
    endforeach()

    add_library(${name} SHARED ${sources})
    target_link_libraries(${name} PRIVATE MWOn12SDK)

    if(MSVC)
        set_property(TARGET ${name} PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        target_compile_options(${name} PRIVATE /W3)
    endif()

    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_BINARY_DIR}/bin/Plugins"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Plugins"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/Plugins"
    )
endfunction()

# ── mwon12_add_asi(name source...) ───────────────────────────────────────────
# An ASI is a plugin DLL with a different extension and a different entry point
# (<mwon12/asi.hpp> rather than MWON12_PLUGIN). Everything else about the build
# is identical, so this is mwon12_add_plugin with the suffix and the output
# directory changed — ASIs go where an ASI loader looks, which is `scripts\`.
function(mwon12_add_asi name)
    mwon12_add_plugin(${name} ${ARGN})
    set_target_properties(${name} PROPERTIES
        SUFFIX ".asi"
        RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_BINARY_DIR}/bin/scripts"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/scripts"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin/scripts"
    )
endfunction()

if(MWON12SDK_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

# Worked examples, each a standalone .asi. See Tutorials/README.md.
option(MWON12SDK_BUILD_TUTORIALS "Build the tutorial .asi mods" ON)
if(MWON12SDK_BUILD_TUTORIALS)
    add_subdirectory(Tutorials)
endif()
