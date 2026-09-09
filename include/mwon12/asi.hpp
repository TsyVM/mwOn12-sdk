// Writing an ASI that can also draw.
//
// An .asi normally gets nothing from the renderer: it is loaded for its
// DllMain, and by the time its thread runs, the plugin scan has been and gone.
// This header is the way in. It resolves MWOn12's runtime-registration exports
// off the loaded d3d9.dll and hands the renderer a plugin, so an ASI gets the
// same D3D12 device, once per frame, that a plugin does.
//
// The whole of an ASI that draws:
//
//     #include <mwon12/asi.hpp>
//     #include <mwon12/overlay.hpp>
//
//     class MyMod final : public mwon12::Plugin {
//     public:
//         const char* Name() const override { return "MyMod"; }
//
//         void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
//             m_ui.Init(static_cast<ID3D12Device*>(d.device),
//                       static_cast<DXGI_FORMAT>(d.backBufferFormat),
//                       d.frameCount);
//         }
//         void OnPresent(const MWOn12_Frame& f) override {
//             m_ui.Begin(f);
//             m_ui.Rect(20, 20, 120, 10, 0xFF4040FF);
//             m_ui.Submit(static_cast<ID3D12GraphicsCommandList*>(f.commandList));
//         }
//         void OnDeviceDestroyed() override { m_ui.Shutdown(); }
//
//     private:
//         mwon12::Overlay m_ui;
//     };
//
//     MWON12_ASI(MyMod)
//
// That is the same class a plugin writes -- mwon12::Plugin, unchanged -- so one
// source file can be built as either, and a mod that wants both an early
// DllMain and a render callback needs only this macro instead of MWON12_PLUGIN.
//
//
// HEADER-ONLY, AND ON PURPOSE
//
// Nothing here links against anything. An ASI is often built outside this SDK's
// CMake, by someone who has an existing project and one file they want to add,
// and requiring a .lib to draw a rectangle would be the thing that stops them.
// mwon12::Overlay does need the SDK library; the registration below does not.
//
//
// WHEN CALLBACKS START
//
// Registration is deferred. MWOn12 takes the plugin on at the next safe point
// in the frame and calls OnDeviceCreated there, on the render thread -- so
// callbacks keep the same guarantee a plugin's do, and Register() returning
// true means "accepted", not "running yet". Create nothing in the constructor;
// wait for OnDeviceCreated, exactly as a plugin does.
//
//
// IF MWOn12 IS NOT THERE
//
// Register() returns false when d3d9.dll is not MWOn12 -- the game running on
// the real runtime, or another proxy in front of it. That is not an error and
// not a reason to give up: an ASI's gameplay half works regardless, and only
// the drawing half needs the renderer. MWON12_ASI keeps the mod alive and
// running in that case; it just never gets a frame callback.

#ifndef MWON12_ASI_HPP
#define MWON12_ASI_HPP

#include "mwon12.h"
#include "plugin.hpp"

#include <windows.h>

namespace mwon12 {
namespace asi {

// The renderer's module, or null. d3d9.dll is what MWOn12 is installed as, and
// GetModuleHandle rather than LoadLibrary because if it is not already loaded
// then it is not the renderer this process is using.
[[nodiscard]] inline HMODULE Module() noexcept
{
    return ::GetModuleHandleA("d3d9.dll");
}

// True when the loaded d3d9.dll is MWOn12 rather than the system runtime or
// another proxy. Tested by the export existing, which nothing else has.
[[nodiscard]] inline bool Available() noexcept
{
    HMODULE m = Module();
    return m && ::GetProcAddress(m, MWON12_REGISTER_PLUGIN_NAME) != nullptr;
}

// The host table, or null. Also what makes mwon12::LogInfo and friends work
// from an ASI; Register() installs it for you.
[[nodiscard]] inline const MWOn12_Host* Host() noexcept
{
    HMODULE m = Module();
    if (!m) return nullptr;
    auto fn = reinterpret_cast<MWOn12_GetHostFn>(
        ::GetProcAddress(m, MWON12_GET_HOST_NAME));
    return fn ? fn() : nullptr;
}

// Registers a plugin object with the renderer, which takes ownership: it holds
// the pointer for the life of the process and deletes it after OnShutdown, the
// same way it does for a plugin. So `self` must be a heap allocation, never a
// local or a static.
//
// On any path that returns false the object is deleted here instead, so a
// caller never has to work out which of them leaked.
//
// Returns false when MWOn12 is not present, refused the ABI version, or the
// plugin's own OnLoad declined. All three are survivable; see the note at the
// top of this header.
inline bool Register(Plugin* self) noexcept
{
    if (!self) return false;

    HMODULE m = Module();
    if (!m) { delete self; return false; }

    auto reg = reinterpret_cast<MWOn12_RegisterPluginFn>(
        ::GetProcAddress(m, MWON12_REGISTER_PLUGIN_NAME));
    if (!reg) { delete self; return false; }

    // Before OnLoad, so a plugin that logs from it has somewhere to log to.
    SetHost(Host());

    if (!self->OnLoad()) { delete self; return false; }

    MWOn12_Plugin api{};
    api.structSize        = sizeof(api);
    api.abiVersion        = MWON12_ABI_VERSION;
    api.name              = self->Name();
    api.version           = self->Version();
    api.user              = self;
    api.OnDeviceCreated   = &detail::ThunkDeviceCreated;
    api.OnResize          = &detail::ThunkResize;
    api.OnPresent         = &detail::ThunkPresent;
    api.OnDeviceDestroyed = &detail::ThunkDeviceDestroyed;
    // ThunkShutdown deletes the object after calling OnShutdown, which is why
    // ownership passes to the renderer above.
    api.OnShutdown        = &detail::ThunkShutdown;

    if (reg(&api) == MWON12_OK) return true;

    // Refused: nothing will ever call ThunkShutdown, so the delete is ours.
    delete self;
    return false;
}

// Stops callbacks for a previously registered plugin. Rarely wanted: an ASI
// does not unload, and the renderer calls OnShutdown at process teardown
// anyway.
inline void Unregister(Plugin* self) noexcept
{
    HMODULE m = Module();
    if (!m || !self) return;
    auto fn = reinterpret_cast<MWOn12_UnregisterPluginFn>(
        ::GetProcAddress(m, MWON12_UNREGISTER_PLUGIN_NAME));
    if (fn) fn(self);
}

}  // namespace asi
}  // namespace mwon12

// The entire entry point of an ASI.
//
// DllMain starts a thread and returns immediately. The thread is not
// optional: DllMain runs under the loader lock, and GetProcAddress against
// another module, never mind anything a mod actually wants to do, is not safe
// to do there. Returning fast and working on a thread is the rule every ASI
// follows.
//
// The instance is handed to the renderer, which owns it from then on: it holds
// the pointer for the life of the process and deletes it after OnShutdown at
// teardown. Register() cleans up on every path where that does not happen, so
// nothing here leaks and nothing is freed while the renderer still has it.
#define MWON12_ASI(ClassName)                                                  \
    static DWORD WINAPI MWOn12_AsiThread(LPVOID)                               \
    {                                                                          \
        auto* self = new ClassName();                                          \
        if (!::mwon12::asi::Register(self)) {                                  \
            /* No renderer, or it refused. The mod stays loaded: whatever it   \
               does outside the frame callbacks still works. */                \
            ::OutputDebugStringA(                                              \
                "[" #ClassName "] MWOn12 not present; running without frame "  \
                "callbacks\n");                                                \
        }                                                                      \
        return 0;                                                              \
    }                                                                          \
    extern "C" BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID)       \
    {                                                                          \
        if (reason == DLL_PROCESS_ATTACH) {                                    \
            ::DisableThreadLibraryCalls(inst);                                 \
            HANDLE h = ::CreateThread(nullptr, 0, &MWOn12_AsiThread,           \
                                      nullptr, 0, nullptr);                    \
            if (h) ::CloseHandle(h);                                           \
        }                                                                      \
        return TRUE;                                                           \
    }

#endif  // MWON12_ASI_HPP
