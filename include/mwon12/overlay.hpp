// A minimal 2D overlay renderer for the OnPresent hook.
//
// Drawing anything at all over the game means a root signature, a pipeline
// state, an input layout, a per-frame upload buffer and a pair of shaders --
// several hundred lines of D3D12 before a single pixel appears, none of which
// is specific to the plugin writing it. This class is that code, once.
//
// Coordinates are pixels, origin top-left, matching where things are on the
// screen rather than NDC. Colours are 0xRRGGBBAA.
//
//     class Bar final : public mwon12::Plugin {
//         mwon12::Overlay m_ui;
//
//         void OnDeviceCreated(const MWOn12_DeviceInfo& d) override {
//             m_ui.Init(static_cast<ID3D12Device*>(d.device),
//                       static_cast<DXGI_FORMAT>(d.backBufferFormat),
//                       d.frameCount);
//         }
//         void OnPresent(const MWOn12_Frame& f) override {
//             m_ui.Begin(f);
//             m_ui.Rect(20.0f, 20.0f, 200.0f, 8.0f, 0xFF3020C0);
//             m_ui.Submit(static_cast<ID3D12GraphicsCommandList*>(f.commandList));
//         }
//         void OnDeviceDestroyed() override { m_ui.Shutdown(); }
//     };
//
// Everything is per-frame-slot where it has to be. Vertices are written into
// an upload buffer indexed by MWOn12_Frame::frameSlot, because the GPU is
// still reading the previous frames' copies -- writing one buffer every frame
// would corrupt work in flight, and does so intermittently, which is the worst
// way for a bug like that to present.
//
// Not a UI toolkit and not trying to become one. Filled rectangles are the
// primitive; text, textures and clipping are deliberately absent. A plugin
// that needs those is past the point where a helper helps, and should own its
// own pipeline.

#ifndef MWON12_OVERLAY_HPP
#define MWON12_OVERLAY_HPP

#include "mwon12.h"

#include <d3d12.h>
#include <dxgiformat.h>

#include <cstdint>
#include <vector>

namespace mwon12 {

class Overlay {
public:
    Overlay() = default;
    ~Overlay();

    // Neither copyable nor movable. Every member below is a raw COM pointer
    // this object releases in Shutdown(), so a copy would release each of them
    // twice; and a move has to be spelled out to exist at all once the copy is
    // deleted, so the deletion below is what stops one being written by
    // accident later. Keep an Overlay as a member, or behind a unique_ptr.
    Overlay(const Overlay&)            = delete;
    Overlay& operator=(const Overlay&) = delete;
    Overlay(Overlay&&)                 = delete;
    Overlay& operator=(Overlay&&)      = delete;

    // `rtvFormat` must match the back buffer, which MWOn12_DeviceInfo reports
    // as backBufferFormat: a pipeline state whose render-target format differs
    // from the target bound at draw time is rejected by the runtime.
    //
    // `maxRectsPerFrame` sizes the per-frame vertex buffer. Rectangles past it
    // in a frame are dropped rather than growing the buffer mid-frame, which
    // would mean allocating while the GPU reads.
    bool Init(ID3D12Device* device,
              DXGI_FORMAT   rtvFormat,
              uint32_t      framesInFlight,
              uint32_t      maxRectsPerFrame = 2048) noexcept;

    // Releases every D3D12 object. Must be called from OnDeviceDestroyed --
    // holding any of these past the device's destruction takes the process
    // down with a live-object report.
    void Shutdown() noexcept;

    [[nodiscard]] bool Ready() const noexcept { return m_pso != nullptr; }

    // Starts a frame. Reads width, height and frameSlot from the frame the
    // renderer handed over, so the caller cannot pass a stale size.
    void Begin(const MWOn12_Frame& frame) noexcept;

    // Pixels, origin top-left. Colour is 0xRRGGBBAA; alpha blends.
    void Rect(float x, float y, float w, float h, uint32_t rgba) noexcept;

    // A one-pixel-thick outline, as four rectangles.
    void RectOutline(float x, float y, float w, float h,
                     uint32_t rgba, float thickness = 1.0f) noexcept;

    // Records everything queued since Begin. The command list must be the one
    // from MWOn12_Frame, with the back buffer already bound -- which is what
    // MWOn12 guarantees on entry to OnPresent.
    //
    // Leaves the pipeline state changed. That is safe, and is why the renderer
    // marks its own command state dirty after calling plugins.
    void Submit(ID3D12GraphicsCommandList* cmd) noexcept;

    [[nodiscard]] uint32_t RectsThisFrame() const noexcept
    { return static_cast<uint32_t>(m_verts.size() / 6); }

    // True when a rectangle was dropped because maxRectsPerFrame was reached.
    [[nodiscard]] bool Overflowed() const noexcept { return m_overflowed; }

private:
    struct Vertex {
        float    x, y;
        uint32_t rgba;   // as R8G8B8A8_UNORM, so byte order is R,G,B,A
    };

    void Emit(float x, float y, float w, float h, uint32_t rgba) noexcept;

    ID3D12Device*        m_device{ nullptr };
    ID3D12RootSignature* m_rootSig{ nullptr };
    ID3D12PipelineState* m_pso{ nullptr };

    // One upload buffer per frame in flight, each persistently mapped.
    std::vector<ID3D12Resource*> m_vb;
    std::vector<uint8_t*>        m_vbCpu;

    std::vector<Vertex> m_verts;

    uint32_t m_frames{ 0 };
    uint32_t m_maxRects{ 0 };
    uint32_t m_slot{ 0 };
    float    m_width{ 0.0f };
    float    m_height{ 0.0f };
    bool     m_overflowed{ false };
};

}  // namespace mwon12

#endif  // MWON12_OVERLAY_HPP
