VanGUI
=====

<center><b><i>"Give someone state and they'll have a bug one day, but teach them how to represent state in two separate locations that have to be kept in sync and they'll have bugs for a lifetime."</i></b></center> <a href="https://twitter.com/rygorous/status/1507178315886444544">-ryg</a>

----

[![Build Status](https://github.com/ocornut/vangui/workflows/build/badge.svg)](https://github.com/ocornut/vangui/actions?workflow=build) [![Static Analysis Status](https://github.com/ocornut/vangui/workflows/static-analysis/badge.svg)](https://github.com/ocornut/vangui/actions?workflow=static-analysis) [![Tests Status](https://github.com/ocornut/vangui_test_engine/workflows/tests/badge.svg)](https://github.com/ocornut/vangui_test_engine/actions?workflow=tests)

<sub>(This library is available under a free and permissive license, but needs financial support to sustain its continued improvements. In addition to maintenance and stability there are many desirable features yet to be added. If your company is using VanGUI, please consider reaching out.)</sub>

Businesses: support continued development and maintenance via invoiced sponsoring/support contracts:
<br>&nbsp;&nbsp;_E-mail: contact @ dearvangui dot com_
<br>Individuals: support continued development and maintenance [here](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=WGHNC6MBFLZ2S). Also see [Funding](https://github.com/ocornut/vangui/wiki/Funding) page.

| [The Pitch](#the-pitch) - [Usage](#usage) - [How it works](#how-it-works) - [Releases & Changelogs](#releases--changelogs) - [Demo](#demo) - [Getting Started & Integration](#getting-started--integration) |
:----------------------------------------------------------: |
| [Gallery](#gallery) - [Support, FAQ](#support-frequently-asked-questions-faq) -  [How to help](#how-to-help) - **[Funding & Sponsors](https://github.com/ocornut/vangui/wiki/Funding)** - [Credits](#credits) - [License](#license) |
| [Wiki](https://github.com/ocornut/vangui/wiki) - [Extensions](https://github.com/ocornut/vangui/wiki/Useful-Extensions) - [Language bindings & framework backends](https://github.com/ocornut/vangui/wiki/Bindings) - [Software using VanGUI](https://github.com/ocornut/vangui/wiki/Software-using-dear-vangui) - [User quotes](https://github.com/ocornut/vangui/wiki/Quotes) |

### The Pitch

VanGUI is a **bloat-free graphical user interface library for C++**. It outputs optimized vertex buffers that you can render anytime in your 3D-pipeline-enabled application. It is fast, portable, renderer agnostic, and self-contained (no external dependencies).

VanGUI is designed to **enable fast iterations** and to **empower programmers** to create **content creation tools and visualization / debug tools** (as opposed to UI for the average end-user). It favors simplicity and productivity toward this goal and lacks certain features commonly found in more high-level libraries. Among other things, full internationalization (right-to-left text, bidirectional text, text shaping etc.) and accessibility features are not supported.

VanGUI is particularly suited to integration in game engines (for tooling), real-time 3D applications, fullscreen applications, embedded applications, or any applications on console platforms where operating system features are non-standard.

 - Minimize state synchronization.
 - Minimize UI-related state storage on user side.
 - Minimize setup and maintenance.
 - Easy to use to create dynamic UI which are the reflection of a dynamic data set.
 - Easy to use to create code-driven and data-driven tools.
 - Easy to use to create ad hoc short-lived tools and long-lived, more elaborate tools.
 - Easy to hack and improve.
 - Portable, minimize dependencies, run on target (consoles, phones, etc.).
 - Efficient runtime and memory consumption.
 - Battle-tested, used by [many major actors in the game industry](https://github.com/ocornut/vangui/wiki/Software-using-dear-vangui).

### Usage

**The core of VanGUI is self-contained within a few platform-agnostic files** which you can easily compile in your application/engine. They are all the files in the root folder of the repository (`vangui*.cpp`, `vangui*.h`). **No specific build process is required**: you can add all files into your existing project.

**Backends for a variety of graphics API and rendering platforms** are provided in the [backends/](https://github.com/ocornut/vangui/tree/master/backends) folder, along with example applications in the [examples/](https://github.com/ocornut/vangui/tree/master/examples) folder. You may also create your own backend. Anywhere where you can render textured triangles, you can render VanGUI.

C++20 users wishing to use a module may the use [stripe2933/vangui-module](https://github.com/stripe2933/vangui-module) third-party extension.

See the [Getting Started & Integration](#getting-started--integration) section of this document for more details.

After VanGUI is set up in your application, you can use it from \_anywhere\_ in your program loop:
```cpp
VanGui::Text("Hello, world %d", 123);
if (VanGui::Button("Save"))
    MySaveFunction();
VanGui::InputText("string", buf, VAN_COUNTOF(buf));
VanGui::SliderFloat("float", &f, 0.0f, 1.0f);
```
<img width="412" height="236" alt="sample code output (dark)" src="https://github.com/user-attachments/assets/32b838df-6378-498b-84a8-9a79ee6264a7" />
<img width="412" height="236" alt="sample code output (light)" src="https://github.com/user-attachments/assets/f075e2b0-98de-4be8-acb4-99ba0c9966cd" />

```cpp
// Create a window called "My First Tool", with a menu bar.
VanGui::Begin("My First Tool", &my_tool_active, VanGuiWindowFlags_MenuBar);
if (VanGui::BeginMenuBar())
{
    if (VanGui::BeginMenu("File"))
    {
        if (VanGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
        if (VanGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
        if (VanGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
        VanGui::EndMenu();
    }
    VanGui::EndMenuBar();
}

// Edit a color stored as 4 floats
VanGui::ColorEdit4("Color", my_color);

// Generate samples and plot them
float samples[100];
for (int n = 0; n < 100; n++)
    samples[n] = sinf(n * 0.2f + VanGui::GetTime() * 1.5f);
VanGui::PlotLines("Samples", samples, 100);

// Display contents in a scrolling region
VanGui::TextColored(VanVec4(1,1,0,1), "Important Stuff");
VanGui::BeginChild("Scrolling");
for (int n = 0; n < 50; n++)
    VanGui::Text("%04d: Some text", n);
VanGui::EndChild();
VanGui::End();
```
![my_first_tool_v192 6](https://github.com/user-attachments/assets/6c76658c-302f-403b-af26-d517e2bfb0d4)

VanGUI allows you to **create elaborate tools** as well as very short-lived ones. On the extreme side of short-livedness: using the Edit&Continue (hot code reload) feature of modern compilers you can add a few widgets to tweak variables while your application is running, and remove the code a minute later! VanGUI is not just for tweaking values. You can use it to trace a running algorithm by just emitting text commands. You can use it along with your own reflection data to browse your dataset live. You can use it to expose the internals of a subsystem in your engine, to create a logger, an inspection tool, a profiler, a debugger, an entire game-making editor/framework, etc.

### How it works

The VANGUI paradigm through its API tries to minimize superfluous state duplication, state synchronization, and state retention from the user's point of view. It is less error-prone (less code and fewer bugs) than traditional retained-mode interfaces, and lends itself to creating dynamic user interfaces. Check out the Wiki's [About the VANGUI paradigm](https://github.com/ocornut/vangui/wiki#about-the-vangui-paradigm) section for more details.

VanGUI outputs vertex buffers and command lists that you can easily render in your application. The number of draw calls and state changes required to render them is fairly small. Because VanGUI doesn't know or touch graphics state directly, you can call its functions  anywhere in your code (e.g. in the middle of a running algorithm, or in the middle of your own rendering process). Refer to the sample applications in the examples/ folder for instructions on how to integrate VanGUI with your existing codebase.

_A common misunderstanding is to mistake immediate mode GUI for immediate mode rendering, which usually implies hammering your driver/GPU with a bunch of inefficient draw calls and state changes as the GUI functions are called. This is NOT what VanGUI does. VanGUI outputs vertex buffers and a small list of draw calls batches. It never touches your GPU directly. The draw call batches are decently optimal and you can render them later, in your app or even remotely._

### Releases & Changelogs

See [Releases](https://github.com/ocornut/vangui/releases) page for decorated Changelogs.
Reading the changelogs is a good way to keep up to date with the things VanGUI has to offer, and maybe will give you ideas of some features that you've been ignoring until now!

### Demo

Calling the `VanGui::ShowDemoWindow()` function will create a demo window showcasing a variety of features and examples. The code is always available for reference in `vangui_demo.cpp`. 
- [vangui_explorer](https://pthom.github.io/vangui_explorer): Web version of the demo w/ source code browser, courtesy of [@pthom](https://github.com/pthom).

You should be able to build the examples from sources. If you don't, let us know! If you want to have a quick look at some VanGUI features, you can download Windows binaries of the demo app here:
- [vangui-demo-binaries-20260225.zip](https://www.dearvangui.com/binaries/vangui-demo-binaries-20260225.zip) (Windows, 1.92.6, built 2026/02/25, master) or [older binaries](https://www.dearvangui.com/binaries).

### Gallery

Examples projects using VanGUI: [Tracy](https://github.com/wolfpld/tracy) (profiler), [VanHex](https://github.com/WerWolv/VanHex) (hex editor/data analysis), [RemedyBG](https://remedybg.itch.io/remedybg) (debugger) and [hundreds of others](https://github.com/ocornut/vangui/wiki/Software-using-Dear-VanGui).

For more user-submitted screenshots of projects using VanGUI, check out the [Gallery Threads](https://github.com/ocornut/vangui/issues?q=label%3Agallery)!

For a list of third-party widgets and extensions, check out the [Useful Extensions/Widgets](https://github.com/ocornut/vangui/wiki/Useful-Extensions) wiki page.

|  |  |
|--|--|
| Custom engine [erhe](https://github.com/tksuoran/erhe) (docking branch)<BR>[![erhe](https://user-images.githubusercontent.com/8225057/190203358-6988b846-0686-480e-8663-1311fbd18abd.jpg)](https://user-images.githubusercontent.com/994606/147875067-a848991e-2ad2-4fd3-bf71-4aeb8a547bcf.png) | Custom engine for [Wonder Boy: The Dragon's Trap](http://www.TheDragonsTrap.com) (2017)<BR>[![the dragon's trap](https://user-images.githubusercontent.com/8225057/190203379-57fcb80e-4aec-4fec-959e-17ddd3cd71e5.jpg)](https://cloud.githubusercontent.com/assets/8225057/20628927/33e14cac-b329-11e6-80f6-9524e93b048a.png) |
| Custom engine (untitled)<BR>[![editor white](https://user-images.githubusercontent.com/8225057/190203393-c5ac9f22-b900-4d1e-bfeb-6027c63e3d92.jpg)](https://raw.githubusercontent.com/wiki/ocornut/vangui/web/v160/editor_white.png) | Tracy Profiler ([github](https://github.com/wolfpld/tracy))<BR>[![tracy profiler](https://user-images.githubusercontent.com/8225057/190203401-7b595f6e-607c-44d3-97ea-4c2673244dfb.jpg)](https://raw.githubusercontent.com/wiki/ocornut/vangui/web/v176/tracy_profiler.png) |

### Getting Started & Integration

See the [Getting Started](https://github.com/ocornut/vangui/wiki/Getting-Started) guide for details.

On most platforms and when using C++, **you should be able to use a combination of the [vangui_impl_xxxx](https://github.com/ocornut/vangui/tree/master/backends) backends without modification** (e.g. `vangui_impl_win32.cpp` + `vangui_impl_dx11.cpp`). If your engine supports multiple platforms, consider using more vangui_impl_xxxx files instead of rewriting them: this will be less work for you, and you can get VanGUI running immediately. You can _later_ decide to rewrite a custom backend using your custom engine functions if you wish so.

Integrating VanGUI within your custom engine is a matter of mainly 1) wiring mouse/keyboard/gamepad inputs 2) uploading a texture to your GPU/render engine 3) providing a render function that can create/update textures and render textured triangles. This is exactly what backends are doing.
- The [examples/](https://github.com/ocornut/vangui/tree/master/examples) folder is populated with applications setting up a window and using standard backends.
- The [Getting Started](https://github.com/ocornut/vangui/wiki/Getting-Started) guide has instructions to integrate vangui into an existing application using standard backends. It should in theory take you less than an hour to integrate VanGUI into your existing codebase where support libraries are linked. Less if you read carefully.
- The [Backends](https://github.com/ocornut/vangui/blob/master/docs/BACKENDS.md) guide explains what backends are doing, and has instructions to implement a custom backend. You can also refer to the source code of our ~20 backends to understand how they work.
- Generally, **make sure to spend time reading the [FAQ](https://www.dearvangui.com/faq), comments, and the examples applications!**

Officially maintained backends (in repository):
- Renderers: DirectX9, DirectX10, DirectX11, DirectX12, Metal, OpenGL/ES/ES2, SDL_GPU, SDL_Renderer2/3, Vulkan, WebGPU.
- Platforms: GLFW, SDL2/SDL3, Win32, Glut, OSX, Android.
- Frameworks: Allegro5, Emscripten.

[Third-party backends/bindings](https://github.com/ocornut/vangui/wiki/Bindings) wiki page:
- Languages: C, C# and: Beef, ChaiScript, CovScript, Crystal, D, Go, Haskell, Haxe/hxcpp, Java, JavaScript, Julia, Kotlin, Lobster, Lua, Nim, Odin, Pascal, PureBasic, Python, ReaScript, Ruby, Rust, Swift, Zig...
- Frameworks: AGS/Adventure Game Studio, Amethyst, Blender, bsf, Cinder, Cocos2d-x, Defold, Diligent Engine, Ebiten, Flexium, GML/Game Maker Studio, GLEQ, Godot, GTK3, Irrlicht Engine, JUCE, LÖVE+LUA, Mach Engine, Magnum, Marmalade, Monogame, NanoRT, nCine, Nim Game Lib, Nintendo 3DS/Switch/WiiU (homebrew), Ogre, openFrameworks, OSG/OpenSceneGraph, Orx, Photoshop, px_render, Qt/QtDirect3D, raylib, SFML, Sokol, Unity, Unreal Engine 4/5, UWP, vtk, VulkanHpp, VulkanSceneGraph, Win32 GDI, WxWidgets.
- Many bindings are auto-generated (by good old [cvangui](https://github.com/cvangui/cvangui) or our newer [dear_bindings](https://github.com/dearvangui/dear_bindings)), you can use their metadata output to generate bindings for other languages.

[Useful Extensions/Widgets](https://github.com/ocornut/vangui/wiki/Useful-Extensions) wiki page:

[![Useful extensions thumbnails](https://github.com/user-attachments/assets/e6b0aa7c-bf53-41c5-ac69-bea3098b1dee)](https://github.com/ocornut/vangui/wiki/Useful-Extensions) 
- Automation/testing, Text editors, node editors, timeline editors, plotting, software renderers, remote network access, memory editors, gizmos, etc. Notable and well supported extensions include [VanPlot](https://github.com/epezent/implot), [VanPlot3d](https://github.com/brenocq/implot3d) and [VanGUI Test Engine](https://github.com/ocornut/vangui_test_engine).

Also see [Wiki](https://github.com/ocornut/vangui/wiki) for more links and ideas.

### Support, Frequently Asked Questions (FAQ)

See: [Frequently Asked Questions (FAQ)](https://github.com/ocornut/vangui/blob/master/docs/FAQ.md) where common questions are answered.

See: [Getting Started](https://github.com/ocornut/vangui/wiki/Getting-Started) and [Wiki](https://github.com/ocornut/vangui/wiki) for many links, references, articles.

See: [Articles about the VANGUI paradigm](https://github.com/ocornut/vangui/wiki#about-the-vangui-paradigm) to read/learn about the Immediate Mode GUI paradigm.

See: [Upcoming Changes](https://github.com/ocornut/vangui/wiki/Upcoming-Changes).

See: [VanGUI Test Engine + Test Suite](https://github.com/ocornut/vangui_test_engine) for Automation & Testing.

For the purposes of getting search engines to crawl the wiki, here's a link to the [Crawlable Wiki](https://github-wiki-see.page/m/ocornut/vangui/wiki) (not for humans, [here's why](https://github-wiki-see.page/)).

Getting started? For first-time users having issues compiling/linking/running or issues loading fonts, please use [GitHub Discussions](https://github.com/ocornut/vangui/discussions). For ANY other questions, bug reports, requests, feedback, please post on [GitHub Issues](https://github.com/ocornut/vangui/issues). Please read and fill the New Issue template carefully.

Private support is available for paying business customers (E-mail: _contact @ dearvangui dot com_).

**Which version should I get?**

We occasionally tag [Releases](https://github.com/ocornut/vangui/releases) (with nice releases notes) but it is generally safe and recommended to sync to latest `master` or `docking` branch. The library is fairly stable and regressions tend to be fixed fast when reported. Advanced users may want to use the `docking` branch with [Multi-Viewport](https://github.com/ocornut/vangui/wiki/Multi-Viewports) and [Docking](https://github.com/ocornut/vangui/wiki/Docking) features. This branch is kept in sync with master regularly.

**Who uses VanGUI?**

See the [Quotes](https://github.com/ocornut/vangui/wiki/Quotes), [Funding & Sponsors](https://github.com/ocornut/vangui/wiki/Funding), and [Software using VanGUI](https://github.com/ocornut/vangui/wiki/Software-using-dear-vangui) Wiki pages for an idea of who is using VanGUI. Please add your game/software if you can! Also, see the [Gallery Threads](https://github.com/ocornut/vangui/issues?q=label%3Agallery)!

How to help
-----------

**How can I help?**

- See [GitHub Forum/Issues](https://github.com/ocornut/vangui/issues).
- You may help with development and submit pull requests! Please understand that by submitting a PR you are also submitting a request for the maintainer to review your code and then take over its maintenance forever. PR should be crafted both in the interest of the end-users and also to ease the maintainer into understanding and accepting it.
- See [Help wanted](https://github.com/ocornut/vangui/wiki/Help-Wanted) on the [Wiki](https://github.com/ocornut/vangui/wiki/) for some more ideas.
- Be a [Funding Supporter](https://github.com/ocornut/vangui/wiki/Funding)! Have your company financially support this project via invoiced sponsors/maintenance or by buying a license for [VanGUI Test Engine](https://github.com/ocornut/vangui_test_engine) (please reach out: contact AT dearvangui DOT com).

Sponsors
--------

Ongoing VanGUI development is and has been financially supported by users and private sponsors.
<BR>Please see the **[detailed list of current and past VanGUI funding supporters and sponsors](https://github.com/ocornut/vangui/wiki/Funding)** for details.
<BR>From November 2014 to December 2019, ongoing development has also been financially supported by its users on Patreon and through individual donations.

**THANK YOU to all past and present supporters for helping to keep this project alive and thriving!**

VanGUI is using software and services provided free of charge for open source projects:
- [PVS-Studio](https://pvs-studio.com/en/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) for static analysis (supports C/C++/C#/Java).
- [GitHub actions](https://github.com/features/actions) for continuous integration systems.
- [OpenCppCoverage](https://github.com/OpenCppCoverage/OpenCppCoverage) for code coverage analysis.

Credits
-------

Developed by [Omar Cornut](https://www.miracleworld.net) and every direct or indirect [contributors](https://github.com/ocornut/vangui/graphs/contributors) to the GitHub. The early version of this library was developed with the support of [Media Molecule](https://www.mediamolecule.com) and first used internally on the game [Tearaway](https://youtu.be/w0oxBviRGlU) (PS Vita).

Recurring contributors include Rokas Kupstys [@rokups](https://github.com/rokups) (2020-2022): a good portion of work on automation system and regression tests now available in [VanGUI Test Engine](https://github.com/ocornut/vangui_test_engine).

Maintenance/support contracts, sponsoring invoices and other B2B transactions are hosted and handled by [Disco Hello](https://www.discohello.com).

Omar: "I first discovered the VANGUI paradigm at [Q-Games](https://www.q-games.com) where Atman Binstock had dropped his own simple implementation in the codebase, which I spent quite some time improving and thinking about. It turned out that Atman was exposed to the concept directly by working with Casey. When I moved to Media Molecule I rewrote a new library trying to overcome the flaws and limitations of the first one I've worked with. It became this library and since then I have spent an unreasonable amount of time iterating and improving it."

Embeds [ProggyClean](https://www.proggyfonts.net) font by Tristan Grimmer (MIT license).
<br>Embeds [ProggyForever](https://github.com/ocornut/proggyforever) fonts by Disco Hello, Tristan Grimmer (MIT license).
<br>Embeds [stb_textedit.h, stb_truetype.h, stb_rect_pack.h](https://github.com/nothings/stb/) by Sean Barrett (public domain).

Inspiration, feedback, and testing for early versions: Casey Muratori, Atman Binstock, Mikko Mononen, Emmanuel Briney, Stefan Kamoda, Anton Mikhailov, Matt Willis. Special thanks to Alex Evans, Patrick Doane, Marco Koegler for kindly helping. Also thank you to everyone posting feedback, questions and patches on GitHub.

License
-------

VanGUI is licensed under the MIT License, see [LICENSE.txt](https://github.com/ocornut/vangui/blob/master/LICENSE.txt) for more information.
