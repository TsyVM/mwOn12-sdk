// VanGfxProbe — watching the game's rendering as MWOn12 issues it.
//
// The starting point for a graphics mod. It attaches VanGFX to MWOn12's D3D12
// device and reports what it sees: how many draws a frame is made of, how many
// pipeline states the renderer built, and what the device is capable of. It
// changes nothing.
//
// That report is the thing you need before any of the rest is possible. "Make
// the cars shinier" begins with knowing which of the four thousand draws in a
// frame are cars, and this is what tells you how many there are to sort through.
//
// Install: copy VanGfxProbe.dll into <game>\MWOn12\Plugins\, set VerboseLog=1
//          in MWOn12.ini, run the game, read MWOn12-render.log.
//
// Settings, in MWOn12.ini:
//
//     [VanGfxProbe]
//     Enabled=1        ; 0 to do nothing at all
//     ListPSOs=0       ; 1 to name every pipeline state by its shader hashes
//
//
// WHAT IT PRINTS
//
//     [VanGfxProbe] gfx: attached to MWOn12's D3D12 device (SM 6.6)
//     [VanGfxProbe] device: raytracing tier 11, mesh shader tier 1
//     [VanGfxProbe] frame 60: 3841 draws, 12 dispatches, 214 PSOs so far
//     [VanGfxProbe] frame 120: 3902 draws, 12 dispatches, 214 PSOs so far
//
// The PSO count climbing and then flattening is the shader cache warming up:
// MWOn12 translates and compiles each of the game's shaders the first time it
// is used, so a number that stops rising means you have seen everything that
// area draws with.
//
//
// WHERE TO GO FROM HERE
//
// The draw callback below has an `ev` you can write to. Setting
// `ev.intercept = true` drops that draw, which is the crudest and fastest way
// to find out what a group of draws is responsible for -- drop everything with
// more than N indices and see what disappears.
//
// From there, ListPSOs=1 names every pipeline state by the hash of its shaders,
// and mwon12::gfx::ReplacePixelShader swaps one of them for HLSL of your own.
// See docs/graphics.md.
//
// And if what you actually want is to edit the game's shaders as text, none of
// this is needed: shaderkit/README.md does that with no code at all.

#include <mwon12/plugin.hpp>
#include <mwon12/graphics.hpp>

#include <atomic>

class VanGfxProbe final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "VanGfxProbe"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        return mwon12::ConfigInt("VanGfxProbe", "Enabled", 1) != 0;
    }

    void OnDeviceCreated(const MWOn12_DeviceInfo& d) override
    {
        // Attach fails cleanly on the DX9 passthrough backend, where there is
        // no D3D12 device. Nothing else in this plugin runs in that case.
        if (!m_gfx.Attach(d)) return;

        mwon12::LogInfo("device: raytracing tier %u, mesh shader tier %u",
                        m_gfx->d3d12_raytracing_tier(),
                        m_gfx->d3d12_mesh_shader_tier());

        // Counting is all this does. The counters are atomic because a draw
        // callback and OnPresent are not the same call and VanGFX does not
        // serialise them for you -- see the threading note in graphics.hpp.
        m_gfx->on_draw([this](vangfx::FrameContext&, vangfx::DrawEvent& ev) {
            if (ev.type == vangfx::DrawType::Dispatch ||
                ev.type == vangfx::DrawType::DispatchIndirect) {
                m_dispatches.fetch_add(1, std::memory_order_relaxed);
            } else {
                m_draws.fetch_add(1, std::memory_order_relaxed);
            }
        });

        // Every pipeline state MWOn12 builds, which is one per distinct shader
        // and render-state combination the game asks for.
        //
        // The line this prints is step one of replacing a shader: it names each
        // pipeline state by the hash of its pixel shader, which is what
        // gfx::WhenPixelShaderIs() matches on. Drive to the thing you want to
        // change, find the hash that appeared, and target it. See
        // docs/graphics.md.
        const bool listPsos = mwon12::ConfigInt("VanGfxProbe", "ListPSOs", 0) != 0;

        m_gfx->on_pso_create(
            [this, listPsos](vangfx::FrameContext&,
                             vangfx::d3d12::PSOCreateEvent& ev) {
                m_psos.fetch_add(1, std::memory_order_relaxed);

                if (!listPsos || ev.kind != vangfx::d3d12::PSOKind::Graphics ||
                    !ev.graphics) {
                    return;
                }

                // Off by default: there are hundreds of these, and they all
                // arrive in the first seconds of a scene.
                mwon12::LogInfo("pso: vs %016llX ps %016llX | %u RT, %u inputs",
                    static_cast<unsigned long long>(
                        mwon12::gfx::BytecodeHash(ev.graphics->vs)),
                    static_cast<unsigned long long>(
                        mwon12::gfx::BytecodeHash(ev.graphics->ps)),
                    ev.graphics->num_render_targets,
                    ev.graphics->input_element_count);
            });

        // MWOn12's own Present, which is the end of the frame -- after the
        // plugin OnPresent below, not before it.
        m_gfx->on_present([this](vangfx::FrameContext&, vangfx::PresentEvent&) {
            const uint64_t frame = m_frames.fetch_add(1,
                std::memory_order_relaxed) + 1;
            if (frame % 60 != 0) return;

            // Draws are per frame; PSOs are cumulative, because the interesting
            // thing about them is when the number stops growing.
            mwon12::LogInfo("frame %llu: %u draws, %u dispatches, %u PSOs so far",
                            static_cast<unsigned long long>(frame),
                            m_draws.exchange(0, std::memory_order_relaxed),
                            m_dispatches.exchange(0, std::memory_order_relaxed),
                            m_psos.load(std::memory_order_relaxed));
        });
    }

    void OnDeviceDestroyed() override
    {
        // Before the device goes. Leaving the vtable patches in place past this
        // point is how a plugin crashes the process on exit.
        m_gfx.Detach();
    }

private:
    mwon12::gfx::Context  m_gfx;

    std::atomic<uint32_t> m_draws{ 0 };
    std::atomic<uint32_t> m_dispatches{ 0 };
    std::atomic<uint32_t> m_psos{ 0 };
    std::atomic<uint64_t> m_frames{ 0 };
};

MWON12_PLUGIN(VanGfxProbe)
