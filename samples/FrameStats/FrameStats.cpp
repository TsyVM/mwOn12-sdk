// FrameStats — the smallest useful MWOn12 plugin.
//
// Draws nothing. Every second it writes the frame rate to MWOn12-render.log,
// which makes it the right thing to build first: if this loads and logs, the
// SDK, the build and the install path are all correct, and anything that goes
// wrong afterwards is the next plugin's fault rather than the setup's.
//
// Log output is Info level, so MWOn12.ini needs VerboseLog=1 to see it.
//
// Build:  see samples/CMakeLists.txt, or the SDK's Build.bat
// Install: copy FrameStats.dll into <game>\MWOn12\Plugins\

#include <mwon12/plugin.hpp>

#include <windows.h>

class FrameStats final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "FrameStats"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        // Reads [FrameStats] from MWOn12.ini -- the same file the renderer
        // uses. A plugin with its own config file is one more thing for a user
        // to lose.
        m_intervalMs = mwon12::ConfigInt("FrameStats", "IntervalMs", 1000);
        if (m_intervalMs < 100) m_intervalMs = 100;

        if (!mwon12::ConfigInt("FrameStats", "Enabled", 1)) {
            // Declining is not a failure. The user turned it off.
            mwon12::LogInfo("FrameStats: disabled in MWOn12.ini");
            return false;
        }
        return true;
    }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override
    {
        mwon12::LogInfo("FrameStats: device up, %ux%u, %u frames in flight",
                        d.width, d.height, d.frameCount);
        m_lastTick   = GetTickCount64();
        m_lastFrame  = 0;
    }

    void OnResize(const MWOn12_DeviceInfo& d) override
    {
        mwon12::LogInfo("FrameStats: resized to %ux%u", d.width, d.height);
    }

    void OnPresent(const MWOn12_Frame& f) override
    {
        const ULONGLONG now = GetTickCount64();
        const ULONGLONG dt  = now - m_lastTick;
        if (dt < static_cast<ULONGLONG>(m_intervalMs)) return;

        const uint64_t frames = f.frameNumber - m_lastFrame;
        mwon12::LogInfo("FrameStats: %.1f fps (%llu frames in %llu ms)",
                        double(frames) * 1000.0 / double(dt),
                        static_cast<unsigned long long>(frames),
                        static_cast<unsigned long long>(dt));

        m_lastTick  = now;
        m_lastFrame = f.frameNumber;
    }

    void OnDeviceDestroyed() override
    {
        mwon12::LogInfo("FrameStats: device going away");
    }

private:
    int        m_intervalMs{ 1000 };
    ULONGLONG  m_lastTick{ 0 };
    uint64_t   m_lastFrame{ 0 };
};

MWON12_PLUGIN(FrameStats)
