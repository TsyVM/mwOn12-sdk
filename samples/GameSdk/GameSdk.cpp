// GameSdk — the game's own code and data, through MWSDK.
//
// The other samples are about the picture. This one is about Most Wanted: what
// its objects are, where its functions live, and how to change them without a
// magic number anywhere in the file.
//
// The difference MWSDK makes is provenance. Without it, reaching into the game
// looks like this:
//
//     h.Install<Fn>(hooks::Rva(0x0067A410), &MyDetour, &g_orig, "something");
//     *reinterpret_cast<float*>(car + 0x1A4) *= 1.1f;
//
// Two numbers nobody can check, that mean nothing to the next reader, and that
// are wrong on any build but the one they were copied from. With MWSDK the same
// two lines carry names, and every name is traceable to verified RE data:
//
//     mw05::vehicle(car).top_speed() *= 1.1f;
//
//
// WHAT IT DOES
//
// Once a second it walks the live vehicle list and logs what it finds: how many
// there are, what class each one actually is, and the top speed of the first.
// It changes nothing unless you set Boost to something other than 100.
//
// Install: copy GameSdk.dll into <game>\MWOn12\Plugins\, set VerboseLog=1 in
//          MWOn12.ini, run the game, read MWOn12-render.log.
//
// Settings, in MWOn12.ini:
//
//     [GameSdk]
//     Enabled=1        ; 0 to do nothing at all
//     Boost=100        ; top speed as a percentage; 110 = +10%, 100 = leave alone
//
//
// WHY IT IS DRIVEN FROM OnPresent
//
// MWOn12 calls OnPresent once per frame on the render thread, which is a
// perfectly good clock for something that only wants to look at game state --
// and it means this sample needs no thread of its own. That matters: MWSDK's
// own MWSDK_MOD() macro creates a DllMain and a background thread, which is
// the right shape for a standalone .asi and the wrong one here. A plugin
// already has a lifecycle. Use MWON12_PLUGIN and MWSDK's library, not both
// entry points.

#include <mwon12/plugin.hpp>

#include <mwsdk/game/mw05.hpp>
#include <mwsdk/game/mw05_views.hpp>
#include <mwsdk/game/mw05_easy.hpp>
#include <mwsdk/hashing.hpp>

using namespace mwsdk;

// A compile-time check that the SDK and the game agree. lookup2 is the engine's
// own string hash, and "default" is the attribute key every vault starts from:
// if this is ever not the answer, the SDK is talking about a different game and
// nothing below can be trusted. Costs nothing at runtime -- it is a static
// assertion, so it either compiles or it does not.
static_assert(hash::attrib("default") == 0xEEC2271Au,
              "MWSDK lookup2 hash disagrees with the engine's verified value");

class GameSdk final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "GameSdk"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        if (!mwon12::ConfigInt("GameSdk", "Enabled", 1)) return false;

        // Percent, so the ini stays readable: 110 means +10%.
        m_boost = float(mwon12::ConfigInt("GameSdk", "Boost", 100)) / 100.0f;
        return true;
    }

    void OnDeviceCreated(const MWOn12_DeviceInfo&) override
    {
        // The image base, resolved once. Everything MWSDK does against the live
        // process rebases through this, so a relocated speed.exe needs no
        // special case -- which is the whole reason addresses are stored as
        // preferred-base Va rather than as whatever they were last time.
        const auto& img = mw05::process();
        mwon12::LogInfo("speed.exe image base 0x%08X",
                        unsigned(img.base()));

        if (void* rn = mw05::road_network())
            mwon12::LogInfo("road network singleton at %p", rn);
    }

    void OnPresent(const MWOn12_Frame& f) override
    {
        // Once a second at 60fps. Walking the vehicle list every frame would
        // work and would also be a pointless read of a few hundred pointers.
        if ((f.frameNumber % 60u) != 0u) return;

        const std::uint32_t count = mw05::vehicle_count();
        if (count == 0) return;   // menus, loading -- the list is empty

        void* first = mw05::vehicle_at(0);
        if (!first) return;

        // identify() asks the object what it is by matching its vtable against
        // the verified class database, rather than assuming the list holds what
        // you expect. A null answer means an object the database does not know,
        // which is worth saying out loud instead of casting anyway.
        const char* what = "unknown class";
        if (const auto* cls = mw05::identify(first))
            what = cls->name.data();

        // top_speed() is a reference to a verified offset, so it reads and
        // writes the same field the game does.
        const float top = mw05::vehicle(first).top_speed();

        mwon12::LogInfo("%u vehicle(s); [0] is %s, top speed %.1f m/s",
                        count, what, double(top));

        if (m_boost != 1.0f && !m_applied) {
            m_applied = true;
            for (std::uint32_t i = 0; i < count; ++i) {
                if (void* v = mw05::vehicle_at(i))
                    mw05::vehicle(v).top_speed() *= m_boost;
            }
            mwon12::LogInfo("applied %.0f%% top speed to %u vehicle(s)",
                            double(m_boost * 100.0f), count);
        }
    }

private:
    float m_boost{ 1.0f };
    bool  m_applied{ false };
};

MWON12_PLUGIN(GameSdk)
