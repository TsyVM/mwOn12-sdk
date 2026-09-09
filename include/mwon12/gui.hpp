// A real user interface, in four lines.
//
// mwon12::Overlay draws rectangles. That is enough for a bar or a marker and
// nothing else: the moment a mod wants a checkbox, a slider, a colour picker or
// a list the user can scroll, it wants a UI toolkit.
//
// VanGUI is one. Standing it up on D3D12 by hand is the part that stops people:
// a shader-visible descriptor heap for the font atlas, a Win32 message hook so
// the mouse works, two backend lifecycles to start and stop in the right order,
// and a NewFrame/Render pair that has to straddle the plugin's own callbacks.
// None of it is specific to the mod being written. This class is that code,
// once.
//
//     class Tuner final : public mwon12::Plugin {
//         mwon12::Gui m_gui;
//
//         void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
//             m_gui.Init(d);
//         }
//
//         void OnPresent(const MWOn12_Frame& f) override {
//             if (!m_gui.Begin(f)) return;         // hidden: nothing was built
//
//             VanGui::Begin("Tuner");
//             VanGui::SliderFloat("Top speed", &m_speed, 1.0f, 2.0f);
//             if (VanGui::Button("Apply")) Apply();
//             VanGui::End();
//
//             m_gui.End(f);                        // records the draw calls
//         }
//
//         void OnDeviceDestroyed() override { m_gui.Shutdown(); }
//     };
//
// Everything between Begin and End is ordinary VanGUI, which is Dear ImGui's
// API under different names -- anything written for ImGui translates by
// replacing `ImGui::` with `VanGui::`.
//
//
// INPUT, AND THE ONE THING TO GET RIGHT
//
// A UI nobody can click is not a UI, so Init() subclasses the game's window to
// feed VanGUI its input. That means the game and the UI are both reading the
// same mouse, and a click on a button would also steer the car.
//
// WantsInput() is how you stop that: it is true when the pointer is over the UI
// or a text field has focus. What to do with it is the mod's decision -- the
// usual answer is to swallow the game's input while it is true, which
// SetBlockGameInput(true) does for you by not passing those messages on.
//
// Toggle the whole thing with SetVisible(). A UI that is always on top of the
// game is a UI the player cannot turn off.
//
//
// DIRECTX 9
//
// MWOn12 falls back to a D3D9 passthrough on a machine with no usable D3D12
// adapter. There is no D3D12 device in that case and no OnPresent, so this
// class cannot help -- Init() returns false and says so in the log.
//
// The D3D9 backend is still built and shipped, because a mod that wants to
// cover that case can use it directly: hook IDirect3DDevice9::Present yourself,
// call VanGui_ImplDX9_Init(device) once and VanGui_ImplDX9_RenderDrawData()
// each frame. That is real work, and it is the honest amount of work, rather
// than this class pretending to a device it was never given.

#ifndef MWON12_GUI_HPP
#define MWON12_GUI_HPP

#include "mwon12.h"

#include <vangui/vangui.h>
#include <vangui_impl_dx12.h>
#include <vangui_impl_win32.h>

#include <d3d12.h>
#include <windows.h>

namespace mwon12 {

class Gui {
public:
    Gui() = default;
    ~Gui();

    Gui(const Gui&)            = delete;
    Gui& operator=(const Gui&) = delete;

    // Creates the context, both backends, the descriptor heap and the window
    // subclass. Everything comes out of MWOn12_DeviceInfo, so there is nothing
    // to get wrong and no size to keep in step by hand.
    //
    // Returns false and logs why on failure -- most often that MWOn12 is on its
    // D3D9 passthrough and there is no D3D12 device to draw with. Treat that as
    // "no UI", not as fatal.
    bool Init(const MWOn12_DeviceInfo& info) noexcept;

    // Releases everything and puts the window procedure back. Must be called
    // from OnDeviceDestroyed: the descriptor heap and the font texture belong to
    // a device that is about to be destroyed.
    void Shutdown() noexcept;

    [[nodiscard]] bool Ready() const noexcept { return m_ready; }

    // Starts a frame. Returns false when the UI is hidden or not initialised,
    // in which case build nothing and do not call End() -- there is no frame to
    // end, and VanGUI asserts if you end one it did not start.
    bool Begin(const MWOn12_Frame& frame) noexcept;

    // Finishes the frame and records its draw calls into the frame's command
    // list. Pairs with a Begin() that returned true.
    void End(const MWOn12_Frame& frame) noexcept;

    // ── Visibility ──────────────────────────────────────────────────────────
    void SetVisible(bool on) noexcept { m_visible = on; }
    [[nodiscard]] bool Visible() const noexcept { return m_visible; }
    void ToggleVisible() noexcept { m_visible = !m_visible; }

    // Shows or hides on a key press, checked once per Begin(). `vk` is a
    // virtual-key code -- VK_F1, VK_INSERT, VK_OEM_3 for the tilde key. Pass 0
    // to turn the shortcut off and drive SetVisible() yourself.
    void SetToggleKey(int vk) noexcept { m_toggleKey = vk; }

    // ── Input ───────────────────────────────────────────────────────────────

    // True when the UI is using the mouse or keyboard: the pointer is over a
    // VanGUI window, or a text field has focus.
    [[nodiscard]] bool WantsInput() const noexcept;

    // Stop mouse and keyboard messages reaching the game while the UI wants
    // them, so clicking a button does not also drive the car. On by default.
    //
    // It cannot be perfect. A game that reads the keyboard with GetAsyncKeyState
    // or through DirectInput never looks at the message queue, and nothing done
    // to that queue will stop it -- Most Wanted does exactly this for driving.
    // What this reliably fixes is the mouse and anything menu-driven.
    void SetBlockGameInput(bool on) noexcept { m_blockInput = on; }

private:
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    ID3D12DescriptorHeap* m_srvHeap{ nullptr };
    HWND      m_hwnd{ nullptr };
    WNDPROC   m_prevWndProc{ nullptr };
    void*     m_context{ nullptr };   // VanGuiContext*, kept opaque here

    bool m_ready{ false };
    bool m_frameStarted{ false };
    bool m_visible{ true };
    bool m_blockInput{ true };
    int  m_toggleKey{ 0 };
    bool m_toggleWasDown{ false };
};

}  // namespace mwon12

#endif  // MWON12_GUI_HPP
