// SuperCars — boost every car's top speed on a key press, as an .asi.
//
// No hook this time, and no drawing: just MWSDK's typed views writing the same
// fields the game reads. The walk over the live vehicle list is the same one
// the GameSdk sample does; the difference here is a hotkey that turns the boost
// on and off while you drive.
//
// The whole mod is two verified offsets with names on them:
//
//     mw05::vehicle(car).top_speed()       *= factor;   // AIVehicle::mTopSpeed
//     mw05::vehicle(car).accel_max_speed() *= factor;   // curve X-max
//
// That is the point of MWSDK: the alternative is `*(float*)(car + 0x1A4)`, a
// number that means nothing to the next reader and is wrong on any build but
// the one it was copied from.
//
// It runs from OnPresent, which MWOn12 calls once per frame on the render
// thread — a perfectly good clock for something that only pokes at game state,
// and it means the mod needs no thread of its own. Writing a float the physics
// reads next frame is the one cross-thread write that is safe without a lock:
// a torn read of a float cannot happen on x86.
//
//
// INSTALL
//
//   Build (see Tutorials/README.md), then copy SuperCars.asi into
//     <game>\scripts\            (or <game>\MWOn12\ASI\)
//   Run the game, get in a car, press F7.
//
// Settings, in MWOn12.ini:
//
//   [SuperCars]
//   Enabled=1
//   Boost=150          ; top speed as a percentage. 150 = +50%, 100 = no change
//   ToggleKey=118      ; virtual-key code. 118 = VK_F7
//
//
// WHY IT MULTIPLIES ON AND DIVIDES OFF
//
// The mod does not know a car's original top speed, so it cannot "restore" one
// — it multiplies by the boost when you switch on and by its reciprocal when
// you switch off, over the cars present at that moment. Reload the world for a
// clean slate. Enforcing an absolute target instead would need the base value,
// which is the kind of thing you would read once and cache; left out here to
// keep the tutorial to its one idea.

#include <mwon12/asi.hpp>

#include <mwsdk/game/mw05.hpp>
#include <mwsdk/game/mw05_views.hpp>
#include <mwsdk/game/mw05_easy.hpp>

using namespace mwsdk;

class SuperCars final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "SuperCars"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        if (!mwon12::ConfigInt("SuperCars", "Enabled", 1)) return false;
        m_factor    = float(mwon12::ConfigInt("SuperCars", "Boost", 150)) / 100.0f;
        m_toggleKey = mwon12::ConfigInt("SuperCars", "ToggleKey", VK_F7);
        mwon12::LogInfo("super-cars: press key %d for %.0f%% top speed",
                        m_toggleKey, double(m_factor * 100.0f));
        return true;
    }

    void OnPresent(const MWOn12_Frame&) override
    {
        // Rising-edge detection so one press is one toggle, not one per frame.
        const bool down = (::GetAsyncKeyState(m_toggleKey) & 0x8000) != 0;
        if (down && !m_keyWasDown) Toggle();
        m_keyWasDown = down;
    }

private:
    void Toggle() noexcept
    {
        if (m_factor == 1.0f) return;              // Boost=100: nothing to do

        m_on = !m_on;
        const float f = m_on ? m_factor : (1.0f / m_factor);

        const uint32_t count = mw05::vehicle_count();
        uint32_t touched = 0;
        for (uint32_t i = 0; i < count; ++i) {
            if (void* v = mw05::vehicle_at(i)) {
                mw05::vehicle(v).top_speed()       *= f;
                mw05::vehicle(v).accel_max_speed() *= f;
                ++touched;
            }
        }
        mwon12::LogInfo("super-cars: %s (%u car(s))",
                        m_on ? "ON" : "off", touched);
    }

    float m_factor{ 1.5f };
    int   m_toggleKey{ VK_F7 };
    bool  m_on{ false };
    bool  m_keyWasDown{ false };
};

MWON12_ASI(SuperCars)
