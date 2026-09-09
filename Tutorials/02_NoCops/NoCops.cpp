// NoCops — stop the police ever spawning, as an .asi.
//
// The graphical tutorials reach the *picture*. This one reaches the *game*:
// gameplay, AI and physics never go through Direct3D, so nothing MWOn12 does to
// the frame can touch them. What reaches them is a function hook — a detour
// planted on one of speed.exe's own functions — and MWSDK, which knows where
// those functions are and what their objects look like without a magic number
// in the file.
//
// The pursuit fleet is filled by AICopManager. Every tick it calls
// UpdateSpawnRequests(), which turns however many spawn requests have piled up
// into that many new cop cars. We hook that function, zero the request count
// on the way in, then let the original run: it finds nothing to do, so no cop
// is ever created — and because the real function still runs, none of the
// pursuit system's own bookkeeping is left half-finished. That is the safe way
// to suppress a behaviour: starve it of input, don't amputate the function.
//
// This half of an .asi works with or without MWOn12's renderer: it never asks
// for a D3D12 device. Installed from OnLoad, the hook is in place before the
// first pursuit. (A mod that wanted to run even when MWOn12 is not the active
// d3d9.dll would hook straight from DllMain instead; here OnLoad is the tidy
// place, and it is where an .asi first has the host's log.)
//
//
// INSTALL
//
//   Build (see Tutorials/README.md), then copy NoCops.asi into
//     <game>\scripts\            (or <game>\MWOn12\ASI\)
//   Run the game and go looking for trouble; none arrives.
//
// Settings, in MWOn12.ini:
//
//   [NoCops]
//   Enabled=1
//
//
// A NOTE ON CONFIDENCE
//
// AICopManager::UpdateSpawnRequests and the mSpawnRequestCount offset are both
// [verified] in MWSDK's database — reverse-engineered from the retail binary
// and checked, not guessed. hooks::Abs(process().rebase(va)) turns the stored
// preferred-base address into the live one, so this survives a relocated
// speed.exe. See MWOn12_Functions_Guide.md and mwsdk docs for the provenance.

#include <mwon12/asi.hpp>
#include <mwon12/hooks.hpp>

#include <mwsdk/game/mw05.hpp>
#include <mwsdk/game/mw05_easy.hpp>       // mw05::cop_manager()
#include <mwsdk/game/mw05_views.hpp>

using namespace mwsdk;

namespace {

// The hooked function's real type. AICopManager::UpdateSpawnRequests is a
// __thiscall member with no arguments; on 32-bit MSVC that is reached as a
// __fastcall taking `this` in the first slot and an ignored edx in the second.
// It is a function TYPE, not a pointer to one — see the note in hooks.hpp.
using UpdateSpawnRequestsFn = void __fastcall(void* self, void* edx);

UpdateSpawnRequestsFn* g_origUpdate = nullptr;

void __fastcall Detour_UpdateSpawnRequests(void* self, void* edx)
{
    // `self` is the AICopManager. Clear its pending spawn requests, then let
    // the untouched original run over an empty queue.
    mw05::cop_manager(self).spawn_request_count() = 0;
    g_origUpdate(self, edx);
}

}  // namespace

class NoCops final : public mwon12::Plugin {
public:
    const char* Name()    const override { return "NoCops"; }
    const char* Version() const override { return "1.0"; }

    bool OnLoad() override
    {
        if (!mwon12::ConfigInt("NoCops", "Enabled", 1)) return false;

        // Stored address (preferred base) → live address for this run.
        const uintptr_t target = mwon12::hooks::Abs(
            mw05::process().rebase(mw05::fn::AICopManager_UpdateSpawnRequests));

        if (m_hook.Install<UpdateSpawnRequestsFn>(
                target, &Detour_UpdateSpawnRequests, &g_origUpdate,
                "AICopManager::UpdateSpawnRequests")) {
            mwon12::LogInfo("no-cops: cop spawns suppressed");
        } else {
            mwon12::LogWarn("no-cops: hook failed; cops will spawn normally");
        }

        // Stay loaded either way: a failed hook is harmless, and unloading an
        // .asi is never the right move.
        return true;
    }

private:
    mwon12::hooks::Hook m_hook;
};

MWON12_ASI(NoCops)
