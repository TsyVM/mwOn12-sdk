// ScreenFX — a graphical mod with an in-game menu, as an .asi.
//
// This is the "hello world" of doing something to the picture. It composites a
// full-screen colour grade on top of the finished frame — night, heat, sepia,
// or any colour you pick — and gives you a menu, opened with F12, to drive it
// live while you play.
//
// It is the thing MWOn12 makes possible: Most Wanted is a 2005 Direct3D 9 game
// with no way in. MWOn12 renders it through Direct3D 12, hands a plugin the live
// D3D12 command list once per frame after the game has finished drawing, and
// from there compositing over the image and standing up a real UI on the GPU is
// a few lines. None of it needs the game's cooperation.
//
// It is an .asi rather than a plugin, which means it loads before the game does.
// For pure drawing that makes no difference (nothing is drawn until there is a
// device), but keeping every tutorial an .asi means one install folder and one
// mental model. See docs/asi.md for the plugin-vs-asi distinction.
//
//
// INSTALL
//
//   Build (see Tutorials/README.md), then copy ScreenFX.asi into
//     <game>\scripts\            (or <game>\MWOn12\ASI\)
//   Run the game and press F12.
//
// Settings, in MWOn12.ini:
//
//   [ScreenFX]
//   Enabled=1
//   ToggleKey=123      ; virtual-key code. 123 = VK_F12, 112 = VK_F1
//
//
// HOW IT WORKS
//
//   mwon12::Gui      — the menu. It owns the descriptor heap, the Win32 input
//                      hook and both backend lifecycles; you get widgets.
//   mwon12::Overlay  — the effect. A textured-quad renderer that records into
//                      the frame's command list. Here it draws one rectangle
//                      the size of the screen, blended over the game.
//
// The colour grade is the crudest possible "post-process": a translucent quad.
// A real one — bloom, tonemap, a LUT — is a pixel shader, and MWOn12 supports
// that too (mwon12::gfx::ReplacePixelShader, and the 04_DrawFilter tutorial).
// This is the version that fits on a postcard and never fails to build.

#include <mwon12/asi.hpp>
#include <mwon12/gui.hpp>
#include <mwon12/overlay.hpp>

// A preset is just a base colour; the menu's intensity slider becomes its
// alpha. 0xRRGGBB — the alpha is added at draw time.
namespace {

struct Preset { const char* name; uint32_t rgb; };

constexpr Preset kPresets[] = {
    { "None",          0x000000 },
    { "Night",         0x0A1A40 },   // deep blue
    { "Heat",          0xC81E0A },   // red, for a Most Wanted pursuit
    { "Sepia",         0x8A6A2A },   // warm amber
    { "Cold",          0x1E5AA0 },   // cyan-blue
    { "Blackout",      0x000000 },   // pure darken; crank the intensity
};
constexpr int kPresetCount = int(sizeof(kPresets) / sizeof(kPresets[0]));

}  // namespace

class ScreenFX final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "ScreenFX"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        if (!mwon12::ConfigInt("ScreenFX", "Enabled", 1)) return false;
        m_toggleKey = mwon12::ConfigInt("ScreenFX", "ToggleKey", VK_F12);
        return true;
    }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override
    {
        // The effect renderer. Everything it needs is in the device info.
        m_fx.Init(static_cast<ID3D12Device*>(d.device),
                  static_cast<DXGI_FORMAT>(d.backBufferFormat), d.frameCount);

        // The menu. Init can fail on the DX9 passthrough backend (no D3D12
        // device); if it does, the effect below still works, you just cannot
        // open the panel to change it.
        if (m_gui.Init(d)) {
            m_gui.SetToggleKey(m_toggleKey);
            m_gui.SetVisible(false);                // the game is the point
            mwon12::LogInfo("press key %d to open the Screen FX panel",
                            m_toggleKey);
        }

        // Seed the live colour from the "Night" preset so the first toggle
        // shows something.
        ApplyPreset(1);
    }

    void OnPresent(const MWOn12_Frame& f) override
    {
        // ── 1. The effect, drawn under the menu ─────────────────────────────
        // One screen-sized quad. The alpha comes from the intensity slider, so
        // intensity 0 draws nothing and the game is untouched.
        const uint8_t alpha = static_cast<uint8_t>(m_intensity * 255.0f + 0.5f);
        if (m_enabled && alpha > 0) {
            const uint32_t r = uint8_t(m_color[0] * 255.0f + 0.5f);
            const uint32_t g = uint8_t(m_color[1] * 255.0f + 0.5f);
            const uint32_t b = uint8_t(m_color[2] * 255.0f + 0.5f);
            const uint32_t rgba = (r << 24) | (g << 16) | (b << 8) | alpha;

            m_fx.Begin(f);
            m_fx.Rect(0.0f, 0.0f, float(f.width), float(f.height), rgba);
            m_fx.Submit(static_cast<ID3D12GraphicsCommandList*>(f.commandList));
        }

        // ── 2. The menu, on top ─────────────────────────────────────────────
        if (!m_gui.Begin(f)) return;               // hidden: build nothing

        VanGui::SetNextWindowSize(VanVec2(320, 0), VanGuiCond_FirstUseEver);
        VanGui::SetNextWindowPos(VanVec2(40, 40), VanGuiCond_FirstUseEver);

        if (VanGui::Begin("Screen FX")) {
            VanGui::Text("%.0f fps  -  %ux%u   (DX12)",
                         double(VanGui::GetIO().Framerate), f.width, f.height);
            VanGui::Separator();

            VanGui::Checkbox("Effect on", &m_enabled);
            VanGui::SliderFloat("Intensity", &m_intensity, 0.0f, 1.0f);
            VanGui::ColorEdit3("Tint", m_color);

            VanGui::Separator();
            VanGui::TextDisabled("Presets");
            for (int i = 0; i < kPresetCount; ++i) {
                if (VanGui::Button(kPresets[i].name)) ApplyPreset(i);
                if (i % 3 != 2 && i != kPresetCount - 1) VanGui::SameLine();
            }

            VanGui::Separator();
            VanGui::TextDisabled("A translucent full-screen quad, composited");
            VanGui::TextDisabled("over the game on MWOn12's D3D12 command list.");
        }
        VanGui::End();

        m_gui.End(f);
    }

    void OnDeviceDestroyed() override
    {
        m_gui.Shutdown();
        m_fx.Shutdown();
    }

private:
    void ApplyPreset(int i) noexcept
    {
        if (i < 0 || i >= kPresetCount) return;
        const uint32_t rgb = kPresets[i].rgb;
        m_color[0] = float((rgb >> 16) & 0xFF) / 255.0f;
        m_color[1] = float((rgb >>  8) & 0xFF) / 255.0f;
        m_color[2] = float((rgb      ) & 0xFF) / 255.0f;
        m_enabled  = (i != 0);                     // "None" turns it off
        if (m_intensity <= 0.0f) m_intensity = 0.35f;
    }

    mwon12::Gui     m_gui;
    mwon12::Overlay m_fx;

    int   m_toggleKey{ VK_F12 };
    bool  m_enabled{ true };
    float m_intensity{ 0.35f };
    float m_color[3]{ 0.04f, 0.10f, 0.25f };       // night blue, 0..1 per channel
};

MWON12_ASI(ScreenFX)
