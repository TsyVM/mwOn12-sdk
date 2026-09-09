// HelloOverlay — draws over the game with real D3D12 work.
//
// A framerate bar in the top-left corner: a dark panel, an outline, and a fill
// whose width and colour track the frame time. It is deliberately something
// you can see immediately and that changes while you drive, because "did the
// plugin load?" and "is it drawing the right thing?" are different questions
// and a static square only answers the first.
//
// Everything GPU-side is mwon12::Overlay, from MWOn12SDK.lib. What is left
// here is the part that is actually this plugin's: when to create it, what to
// draw, and when to let it go.
//
// The lifetime rule is the one thing worth reading twice. Overlay::Init in
// OnDeviceCreated, Overlay::Shutdown in OnDeviceDestroyed, nothing GPU-side in
// the constructor or OnLoad. The D3D12 device does not exist when the plugin
// is loaded, and it is destroyed the moment OnDeviceDestroyed returns.
//
// Build:  see samples/CMakeLists.txt, or the SDK's Build.bat
// Install: copy HelloOverlay.dll into <game>\MWOn12\Plugins\

#include <mwon12/plugin.hpp>
#include <mwon12/overlay.hpp>

#include <windows.h>

namespace {

// 0xRRGGBBAA.
constexpr uint32_t kPanel   = 0x101014C0;   // dark, mostly opaque
constexpr uint32_t kBorder  = 0xFFFFFF40;   // faint white
constexpr uint32_t kGood    = 0x40C060FF;   // green   —  above 50 fps
constexpr uint32_t kFair    = 0xD0B040FF;   // amber   —  30 to 50
constexpr uint32_t kPoor    = 0xD04040FF;   // red     —  below 30

constexpr float kPad    = 12.0f;
constexpr float kWidth  = 220.0f;
constexpr float kHeight = 26.0f;

}  // namespace

class HelloOverlay final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "HelloOverlay"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        // There is no D3D12 device to draw with under the passthrough backend,
        // so there is nothing this plugin can do. Say so once and unload,
        // rather than sitting in memory checking a flag every frame.
        // mwon12::ActiveBackend(), not Host()->ActiveBackend(): the free
        // function checks both the host table and the function pointer inside
        // it, so a plugin that reaches OnLoad through a path where neither is
        // set -- an ASI built from this file, most obviously -- gets an answer
        // instead of a null dereference.
        if (mwon12::ActiveBackend() != MWON12_BACKEND_DX12) {
            mwon12::LogInfo("HelloOverlay: not the DirectX 12 backend, "
                            "nothing to draw on");
            return false;
        }
        return mwon12::ConfigInt("HelloOverlay", "Enabled", 1) != 0;
    }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override
    {
        // The RTV format must be the back buffer's, or the pipeline state is
        // rejected the first time it meets the real render target.
        m_ui.Init(static_cast<ID3D12Device*>(d.device),
                  static_cast<DXGI_FORMAT>(d.backBufferFormat),
                  d.frameCount,
                  /*maxRectsPerFrame*/ 64);

        m_lastTick  = GetTickCount64();
        m_lastFrame = 0;
        m_fps       = 0.0;
    }

    // The device survives a resize, so the pipeline state does too. Only
    // something sized to the back buffer would need rebuilding here, and this
    // plugin positions everything from the frame's own width and height.
    void OnResize(const MWOn12_DeviceInfo&) override {}

    void OnPresent(const MWOn12_Frame& f) override
    {
        if (!m_ui.Ready()) return;

        UpdateFps(f.frameNumber);

        // Clamped to the bar's range; the colour, not the length, is what says
        // "worse than this scale shows".
        const double     ratio = m_fps > 60.0 ? 1.0 : m_fps / 60.0;
        const uint32_t   fill  = m_fps >= 50.0 ? kGood
                               : m_fps >= 30.0 ? kFair : kPoor;

        m_ui.Begin(f);
        m_ui.Rect(kPad, kPad, kWidth, kHeight, kPanel);
        m_ui.RectOutline(kPad, kPad, kWidth, kHeight, kBorder, 1.0f);
        m_ui.Rect(kPad + 3.0f, kPad + 3.0f,
                  float(ratio) * (kWidth - 6.0f), kHeight - 6.0f, fill);
        m_ui.Submit(static_cast<ID3D12GraphicsCommandList*>(f.commandList));
    }

    void OnDeviceDestroyed() override
    {
        // Not optional. Every D3D12 object this plugin owns has to be released
        // before this returns.
        m_ui.Shutdown();
    }

private:
    // Averaged over a fixed window rather than taken per frame: a per-frame
    // figure on a bar is unreadable noise.
    void UpdateFps(uint64_t frameNumber) noexcept
    {
        const ULONGLONG now = GetTickCount64();
        const ULONGLONG dt  = now - m_lastTick;
        if (dt < 250) return;

        const uint64_t frames = frameNumber - m_lastFrame;
        m_fps       = double(frames) * 1000.0 / double(dt);
        m_lastTick  = now;
        m_lastFrame = frameNumber;
    }

    mwon12::Overlay m_ui;
    ULONGLONG       m_lastTick{ 0 };
    uint64_t        m_lastFrame{ 0 };
    double          m_fps{ 0.0 };
};

MWON12_PLUGIN(HelloOverlay)
