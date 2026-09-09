// GameHooks — reaching the game's own code, not just its rendering.
//
// The other samples work through Direct3D. This one does not: it hooks a
// function, patches bytes, and finds an address by byte pattern. That is how
// you change gameplay, physics, AI or handling, none of which goes anywhere
// near D3D9.
//
// What it actually does is deliberately harmless and observable: it hooks
// Sleep() and counts the calls, patches and reads back its own memory, and
// scans its own module for a pattern. Everything is verifiable from the log
// without needing a specific game build or a correct hard-coded address, so
// this sample cannot silently patch the wrong thing on your machine.
//
// The real addresses for NFS:MW are yours to find. The commented block at the
// bottom shows the shape of a real hook once you have one.
//
// Build:  see samples/CMakeLists.txt
// Install: copy GameHooks.dll into <game>\MWOn12\Plugins\, VerboseLog=1

#include <mwon12/plugin.hpp>
#include <mwon12/hooks.hpp>

#include <windows.h>
#include <cstdint>

namespace {

// ── A real hook on a real function ───────────────────────────────────────────
// Sleep is used because every process has it, at a known place, and hooking it
// proves the whole path -- prologue decode, trampoline, thread suspension --
// without depending on a particular speed.exe.

// A function TYPE, not a pointer to one. Install/InstallApi take Fn* and Fn**,
// so handing them a type that is already a pointer asks for a pointer to a
// pointer and does not compile.
using SleepFn = void WINAPI(DWORD);
SleepFn*         g_origSleep  = nullptr;
volatile LONG    g_sleepCalls = 0;

void WINAPI HookedSleep(DWORD ms)
{
    InterlockedIncrement(&g_sleepCalls);
    // Always call through. A detour that forgets this silently deletes the
    // behaviour it was only meant to observe.
    if (g_origSleep) g_origSleep(ms);
    else             ::SleepEx(ms, FALSE);
}

// Deliberately a module-scope variable, not a member of the plugin object.
// Scan() searches the module's image on disk-as-loaded; the plugin instance is
// heap-allocated, and nothing on the heap is inside that image. Putting the
// probe here is what makes the scan below able to find it -- and is the same
// reason a pattern scan finds game code and constants but never runtime state.
uint8_t g_probe[4] = { 0x11, 0x22, 0x33, 0x44 };

}  // namespace

class GameHooks final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "GameHooks"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        return mwon12::ConfigInt("GameHooks", "Enabled", 1) != 0;
    }

    void OnDeviceCreated(const MWOn12_DeviceInfo&) override
    {
        // Hooks go in here rather than OnLoad: by now the game is past its own
        // startup, so the code being patched is settled.
        namespace hooks = mwon12::hooks;

        mwon12::LogInfo("GameHooks: speed.exe is loaded at 0x%08X",
                        unsigned(hooks::ModuleBase()));

        // ── 1. Hook an exported function by name ────────────────────────────
        // No address needed; resolved through the module's export table.
        m_sleepHook.InstallApi<SleepFn>("kernel32.dll", "Sleep",
                                        &HookedSleep, &g_origSleep);

        // ── 2. Patch memory ─────────────────────────────────────────────────
        // Writing into our own buffer, so this is safe to run anywhere and
        // still demonstrates the call that patches game data.
        hooks::Patch(reinterpret_cast<uintptr_t>(g_probe),
                     { 0xAA, 0xBB }, "sample probe");

        const auto back = hooks::Read(reinterpret_cast<uintptr_t>(g_probe),
                                      sizeof(g_probe));
        if (back.size() == sizeof(g_probe)) {
            mwon12::LogInfo("GameHooks: probe now %02X %02X %02X %02X",
                            back[0], back[1], back[2], back[3]);
        }

        // ── 3. Find an address by byte pattern ──────────────────────────────
        // Scan() returns nothing when a pattern matches more than once, which
        // is the case that would otherwise have you patching the wrong site.
        //
        // The module is named explicitly here because the probe lives in this
        // plugin, not in the game. Left out, Scan() searches speed.exe -- which
        // is the right default for finding game code, and the reason a scan for
        // your own data comes back empty if you forget.
        if (auto hit = hooks::Scan("AA BB 33 44", "GameHooks.dll")) {
            mwon12::LogInfo("GameHooks: pattern found at 0x%08X",
                            unsigned(*hit));
        }

        // ── What a real game hook looks like ────────────────────────────────
        //
        // Find the function once, by pattern, and hook whatever you find --
        // no hard-coded address to go stale:
        //
        //     // A function type, not a pointer to one -- see hooking.md.
        //     using DamageFn = void __fastcall(void* self, void*, float);
        //     static DamageFn* g_origDamage = nullptr;
        //
        //     if (auto addr = hooks::Scan("55 8B EC 83 EC ?? 56 8B F1 D9 45 ??")) {
        //         m_damageHook.Install<DamageFn>(*addr, &MyDamage,
        //                                        &g_origDamage, "TakeDamage");
        //     }
        //
        // With an address from a disassembler instead, wrap it in Rva() so it
        // survives the module loading somewhere other than 0x400000:
        //
        //     m_damageHook.Install<DamageFn>(hooks::Rva(0x0067A410), ...);
    }

    void OnPresent(const MWOn12_Frame& f) override
    {
        // Proof the hook is live and firing: twice a second at 60fps.
        if ((f.frameNumber % 30) != 0 || f.frameNumber == 0) return;
        mwon12::LogInfo("GameHooks: Sleep() called %ld times so far",
                        g_sleepCalls);
    }

    void OnDeviceDestroyed() override
    {
        // Always logged, so there is proof the detour ran even on a session too
        // short to reach the periodic report above.
        mwon12::LogInfo("GameHooks: Sleep() was called %ld times in total",
                        g_sleepCalls);

        // Not strictly required -- Hook removes itself when destroyed -- but
        // taking the detour down before the device goes is the tidier order.
        m_sleepHook.Remove();
    }

private:
    mwon12::hooks::Hook m_sleepHook;
    mwon12::hooks::Hook m_damageHook;   // unused; shown in the note above

};

MWON12_PLUGIN(GameHooks)
