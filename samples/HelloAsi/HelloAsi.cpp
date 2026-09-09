// HelloAsi — an ASI that draws.
//
// The point of this sample is that it is an `.asi`, not a plugin, and it still
// gets the D3D12 device once per frame. Nothing about drawing requires the
// plugin folder or the plugin entry point; an ASI can register itself with the
// renderer and from then on it is a plugin in every way that matters.
//
// Which means the choice between the two is about *when you want to run*, not
// about what you are allowed to do:
//
//   an .asi   loads before the game initialises, so its hooks are in place
//             ahead of whatever they affect -- and can also draw, via this
//   a plugin  loads at device creation, which is simpler when drawing is all
//             you want and there is nothing to hook
//
// Install: build, then copy HelloAsi.asi into <game>\scripts\ (or
//          <game>\MWOn12\ASI\), run the game.
//
// It draws a bar in the top-left that fills as the frame rate rises, and logs
// the vehicle count every second so you can see the gameplay half working at
// the same time. Set VerboseLog=1 in MWOn12.ini to see the log lines.
//
// Settings, in MWOn12.ini:
//
//     [HelloAsi]
//     Enabled=1
//
//
// WHAT TO NOTICE
//
// OnDeviceCreated is where the overlay is built, exactly as in a plugin, and
// for the same reason: there is no D3D12 device before it. Registration
// happening on this mod's own thread does not change that -- MWOn12 defers the
// registration to the render thread and calls OnDeviceCreated there, so the
// ordering guarantee an MWOn12 plugin relies on holds here too.

#include <mwon12/asi.hpp>
#include <mwon12/overlay.hpp>

#include <mwsdk/game/mw05.hpp>

class HelloAsi final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "HelloAsi"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        return mwon12::ConfigInt("HelloAsi", "Enabled", 1) != 0;
    }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override
    {
        m_ui.Init(static_cast<ID3D12Device*>(d.device),
                  static_cast<DXGI_FORMAT>(d.backBufferFormat), d.frameCount);

        mwon12::LogInfo("running as an ASI, drawing on a %ux%u back buffer",
                        d.width, d.height);
    }

    void OnPresent(const MWOn12_Frame& f) override
    {
        // A frame-rate bar, from the interval between presents. Smoothed
        // because an unsmoothed per-frame number is unreadable.
        const double now = Seconds();
        if (m_last > 0.0) {
            const double dt = now - m_last;
            if (dt > 0.0) m_fps = m_fps * 0.95 + (1.0 / dt) * 0.05;
        }
        m_last = now;

        // 0..120fps across 240 pixels.
        const float w = float(m_fps) / 120.0f * 240.0f;

        m_ui.Begin(f);
        m_ui.RectOutline(20.0f, 20.0f, 240.0f, 12.0f, 0x000000C0, 1.0f);
        m_ui.Rect(21.0f, 21.0f, (w > 238.0f ? 238.0f : w), 10.0f, 0x40C040FF);
        m_ui.Submit(static_cast<ID3D12GraphicsCommandList*>(f.commandList));

        // The other half of what an ASI is for. This needs no device and would
        // work just the same with no renderer at all.
        if ((f.frameNumber % 60u) == 0u) {
            mwon12::LogInfo("%.0f fps, %u vehicle(s)",
                            m_fps, mwsdk::mw05::vehicle_count());
        }
    }

    void OnDeviceDestroyed() override { m_ui.Shutdown(); }

private:
    static double Seconds()
    {
        LARGE_INTEGER f, c;
        QueryPerformanceFrequency(&f);
        QueryPerformanceCounter(&c);
        return double(c.QuadPart) / double(f.QuadPart);
    }

    mwon12::Overlay m_ui;
    double m_last{ 0.0 };
    double m_fps{ 60.0 };
};

MWON12_ASI(HelloAsi)
