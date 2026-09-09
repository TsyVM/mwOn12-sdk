// DrawFilter — reach into the game's rendering, one draw at a time, with a menu.
//
// ScreenFX drew *over* the finished frame. This gets inside the frame while it
// is being built. MWOn12 renders Most Wanted's Direct3D 9 through Direct3D 12,
// and VanGFX lets a plugin watch every D3D12 draw as it is recorded — and drop
// the ones it does not want. That is a thing the 2005 game has no notion of and
// no way to allow; it exists only because the game now runs on a modern,
// interceptable pipeline.
//
// The demo is deliberately blunt: a slider that culls every draw with more than
// N indices. Slide it down and the heavy geometry — the world, the crowds —
// vanishes a layer at a time, which is also the crudest, fastest way to find
// out *which* draws are responsible for what. It is the first move of any real
// graphics mod: before you can reshade the cars you have to know which of the
// few thousand draws in a frame the cars are.
//
// From here the same on_pso_create hook that VanGfxProbe prints is what
// mwon12::gfx::ReplacePixelShader targets — swap a matched pipeline state's
// pixel shader for HLSL of your own and you have reshaded the game from C++.
// See docs/graphics.md and the VanGfxProbe sample.
//
//
// INSTALL
//
//   Build (see Tutorials/README.md), then copy DrawFilter.asi into
//     <game>\scripts\            (or <game>\MWOn12\ASI\)
//   Run the game and press F12. Drag "Max index count" down.
//
// Settings, in MWOn12.ini:
//
//   [DrawFilter]
//   Enabled=1
//   ToggleKey=123      ; 123 = VK_F12
//
//
// THREADING
//
// The draw callback and OnPresent are different calls on the render thread and
// VanGFX does not serialise them for you, so the menu and the callback talk
// through std::atomic. See the threading note in graphics.hpp.

#include <mwon12/asi.hpp>
#include <mwon12/gui.hpp>
#include <mwon12/graphics.hpp>

#include <atomic>

class DrawFilter final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "DrawFilter"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        if (!mwon12::ConfigInt("DrawFilter", "Enabled", 1)) return false;
        m_toggleKey = mwon12::ConfigInt("DrawFilter", "ToggleKey", VK_F12);
        return true;
    }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override
    {
        // Attach fails cleanly on the DX9 passthrough backend (no D3D12
        // device); the mod simply does nothing there.
        if (m_gfx.Attach(d)) {
            m_gfx->on_draw([this](vangfx::FrameContext&, vangfx::DrawEvent& ev) {
                m_seen.fetch_add(1, std::memory_order_relaxed);

                if (!m_cull.load(std::memory_order_relaxed)) return;

                // Never touch compute — culling a dispatch tends to break the
                // frame rather than declutter it.
                if (ev.type == vangfx::DrawType::Dispatch ||
                    ev.type == vangfx::DrawType::DispatchIndirect) return;

                if (ev.index_count > m_threshold.load(std::memory_order_relaxed)) {
                    ev.intercept = true;           // drop this draw
                    m_culled.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }

        if (m_gui.Init(d)) {
            m_gui.SetToggleKey(m_toggleKey);
            m_gui.SetVisible(false);
            mwon12::LogInfo("press key %d to open the Draw Filter panel",
                            m_toggleKey);
        }
    }

    void OnPresent(const MWOn12_Frame& f) override
    {
        // Snapshot the per-frame counters once a second for a readable display.
        if ((f.frameNumber % 60u) == 0u) {
            m_seenShown   = m_seen.exchange(0, std::memory_order_relaxed);
            m_culledShown = m_culled.exchange(0, std::memory_order_relaxed);
        }

        if (!m_gui.Begin(f)) return;

        VanGui::SetNextWindowSize(VanVec2(340, 0), VanGuiCond_FirstUseEver);
        VanGui::SetNextWindowPos(VanVec2(40, 40), VanGuiCond_FirstUseEver);

        if (VanGui::Begin("Draw Filter")) {
            VanGui::Text("%.0f fps  -  %u draws/frame, %u culled",
                         double(VanGui::GetIO().Framerate),
                         m_seenShown, m_culledShown);
            VanGui::Separator();

            bool cull = m_cull.load(std::memory_order_relaxed);
            if (VanGui::Checkbox("Cull large draws", &cull))
                m_cull.store(cull, std::memory_order_relaxed);

            int thr = int(m_threshold.load(std::memory_order_relaxed));
            if (VanGui::SliderInt("Max index count", &thr, 0, 30000))
                m_threshold.store(uint32_t(thr < 0 ? 0 : thr),
                                  std::memory_order_relaxed);
            VanGui::SetItemTooltip("Draws with more indices than this are "
                                   "dropped. Slide down to peel the world away.");

            VanGui::Separator();
            VanGui::TextDisabled("Every draw MWOn12 records is offered here");
            VanGui::TextDisabled("before the GPU sees it. This is the pipeline");
            VanGui::TextDisabled("a shader replacement plugs into.");
        }
        VanGui::End();

        m_gui.End(f);
    }

    void OnDeviceDestroyed() override
    {
        m_gfx.Detach();                            // before the device goes
        m_gui.Shutdown();
    }

private:
    mwon12::Gui          m_gui;
    mwon12::gfx::Context m_gfx;

    std::atomic<bool>     m_cull{ false };
    std::atomic<uint32_t> m_threshold{ 3000 };
    std::atomic<uint32_t> m_seen{ 0 };
    std::atomic<uint32_t> m_culled{ 0 };

    uint32_t m_seenShown{ 0 };
    uint32_t m_culledShown{ 0 };
    int      m_toggleKey{ VK_F12 };
};

MWON12_ASI(DrawFilter)
