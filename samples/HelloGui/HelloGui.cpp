// HelloGui — a real user interface over the game, in about forty lines.
//
// Press the toggle key in game to show it. It has a live vehicle list read via
// MWSDK, a slider that changes the player car's top speed while you drag it,
// and a button that opens VanGUI's demo window — which is the fastest way to
// find out what every widget is called, because each control is shown next to
// the line of source that draws it.
//
// The point of the sample is how little of it is setup. There is no descriptor
// heap here, no window subclassing, no backend lifecycle and no font atlas:
// mwon12::Gui does all of that, and what is left is the panel itself.
//
// Install, either or both:
//   HelloGui.dll -> <game>\MWOn12\Plugins\   then press INSERT
//   HelloGui.asi -> <game>\scripts\           then press HOME
//
// Both are built from this file. The ASI loads earlier -- before the game has
// initialised -- and is otherwise identical, which is the point: an ASI is not
// limited to gameplay, and a UI is not limited to plugins.
//
// Settings, in MWOn12.ini:
//
//     [HelloGui]        ; the plugin
//     Enabled=1
//     ToggleKey=45      ; virtual-key code. 45 = VK_INSERT, 112 = VK_F1
//
//     [HelloGuiAsi]     ; the ASI
//     Enabled=1
//     ToggleKey=36      ; 36 = VK_HOME
//
//
// WHY THE SLIDER IS SAFE TO DRAG
//
// OnPresent runs on the render thread, and the game's physics run on its own.
// Writing a float the game reads every frame is the one kind of cross-thread
// write that is fine without a lock: a torn read of a float cannot happen on
// x86, and the worst case is the game using last frame's value for one frame.
// Anything structural -- adding to a list, freeing an object -- would not be,
// and belongs in a hook on the game's own thread instead.

// Built twice, from this one file: once as HelloGui.dll (a plugin) and once as
// HelloGui.asi (an ASI), with nothing different but the entry point at the
// bottom. That is the claim docs/asi.md makes, and building it both ways is
// what keeps the claim true.
#if defined(HELLOGUI_AS_ASI)
#  include <mwon12/asi.hpp>       // pulls in plugin.hpp
#else
#  include <mwon12/plugin.hpp>
#endif
#include <mwon12/gui.hpp>

#include <mwsdk/game/mw05.hpp>
#include <mwsdk/game/mw05_views.hpp>
#include <mwsdk/game/mw05_easy.hpp>

using namespace mwsdk;

#if defined(HELLOGUI_AS_ASI)
static constexpr const char* kSection    = "HelloGuiAsi";
static constexpr const char* kTitle      = "MWOn12 (ASI)";
static constexpr int         kDefaultKey = VK_HOME;
#else
static constexpr const char* kSection    = "HelloGui";
static constexpr const char* kTitle      = "MWOn12";
static constexpr int         kDefaultKey = VK_INSERT;
#endif

class HelloGui final : public mwon12::Plugin {
public:
    const char* Name()    const override { return kSection; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        // Its own section per build, so the two can be installed side by side
        // and toggled independently rather than fighting over one key.
        if (!mwon12::ConfigInt(kSection, "Enabled", 1)) return false;
        m_toggleKey = mwon12::ConfigInt(kSection, "ToggleKey", kDefaultKey);
        return true;
    }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override
    {
        if (!m_gui.Init(d)) return;
        m_gui.SetToggleKey(m_toggleKey);
        m_gui.SetVisible(false);          // start hidden; the game is the point
        mwon12::LogInfo("press key %d to open the panel", m_toggleKey);
    }

    void OnPresent(const MWOn12_Frame& f) override
    {
        if (!m_gui.Begin(f)) return;      // hidden or unavailable: build nothing

        VanGui::SetNextWindowSize(VanVec2(340, 0), VanGuiCond_FirstUseEver);
        VanGui::SetNextWindowPos(VanVec2(40, 40), VanGuiCond_FirstUseEver);

        if (VanGui::Begin(kTitle)) {
            VanGui::Text("%.0f fps  -  %ux%u", double(VanGui::GetIO().Framerate),
                         f.width, f.height);
            VanGui::Separator();

            const uint32_t count = mw05::vehicle_count();
            VanGui::Text("%u vehicle(s) in the world", count);

            void* car = (count > 0) ? mw05::vehicle_at(0) : nullptr;
            if (car) {
                if (const auto* cls = mw05::identify(car))
                    VanGui::Text("vehicle[0]: %s", cls->name.data());

                // A reference to the verified offset, so the slider writes the
                // field the game reads.
                float& top = mw05::vehicle(car).top_speed();
                VanGui::SliderFloat("top speed (m/s)", &top, 10.0f, 150.0f);
                VanGui::SetItemTooltip("Drag it. The change is live.");
            } else {
                VanGui::TextDisabled("no vehicles - you are in a menu");
            }

            VanGui::Separator();
            VanGui::Checkbox("Block game input while hovering", &m_blockInput);
            m_gui.SetBlockGameInput(m_blockInput);

            VanGui::Checkbox("VanGUI demo window", &m_showDemo);
            VanGui::SameLine();
            VanGui::TextDisabled("(every widget, with its source)");
        }
        VanGui::End();

        if (m_showDemo) VanGui::ShowDemoWindow(&m_showDemo);

        m_gui.End(f);
    }

    void OnDeviceDestroyed() override { m_gui.Shutdown(); }

private:
    mwon12::Gui m_gui;
    int  m_toggleKey{ kDefaultKey };
    bool m_showDemo{ false };
    bool m_blockInput{ true };
};

// The only difference between the two builds.
#if defined(HELLOGUI_AS_ASI)
MWON12_ASI(HelloGui)
#else
MWON12_PLUGIN(HelloGui)
#endif
