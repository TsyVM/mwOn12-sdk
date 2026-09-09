// SPDX-License-Identifier: MIT
//
// mwsdk/mod.hpp
// =============
// The one-include "make my DLL a mod" helper. Removes the DllMain + worker-
// thread boilerplate every plugin needs, in the spirit of VanHooks' one-line
// ergonomics. Windows-only (a mod is a DLL injected into speed.exe).
//
//   #include <mwsdk/mod.hpp>
//
//   void my_mod() {                       // runs on its own thread, AFTER the
//       mwsdk::mod::Log log{"my_mod.log"};// loader lock is released - safe.
//       log("base = %p", (void*)mwsdk::mw05::process().base());
//   }
//   MWSDK_MOD(my_mod)                      // <- that's the entire entry point
//
// The macro wires up DllMain, disables per-thread notifications, and spins your
// function up on a background thread so you never run heavy work under the
// loader lock (a classic injection footgun).

#ifndef MWSDK_MOD_HPP
#define MWSDK_MOD_HPP

#include "mwsdk/game/mw05.hpp"
#include "mwsdk/game/mw05_attrib.hpp"
#include "mwsdk/game/mw05_views.hpp"
#include "mwsdk/game/mw05_easy.hpp"
#include "mwsdk/game/mw05_group.hpp"

#if !MWSDK_MW05_RUNTIME
#  error "mwsdk/mod.hpp targets a Windows plugin DLL; include it only in mod builds."
#endif

#include <cstdio>
#include <cstdarg>

namespace mwsdk::mod {

// A dead-simple line logger. A DLL has no stdout, so mods log to a file next to
// wherever the game runs. Opens on first write; flushes every line.
class Log {
public:
    explicit Log(const char* path) noexcept {
#if defined(_MSC_VER)
        if (fopen_s(&f_, path, "w") != 0) f_ = nullptr;
#else
        f_ = std::fopen(path, "w");
#endif
    }
    ~Log() { if (f_) std::fclose(f_); }
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    void operator()(const char* fmt, ...) noexcept {
        if (!f_) return;
        std::va_list ap;
        va_start(ap, fmt);
        std::vfprintf(f_, fmt, ap);
        va_end(ap);
        std::fputc('\n', f_);
        std::fflush(f_);
    }

private:
    std::FILE* f_ = nullptr;
};

// Optional: pop up a console for printf-style debugging during development.
inline void alloc_console() noexcept {
    if (::AllocConsole()) {
        std::FILE* f = nullptr;
#if defined(_MSC_VER)
        freopen_s(&f, "CONOUT$", "w", stdout);
#else
        f = std::freopen("CONOUT$", "w", stdout);
#endif
        (void)f;
    }
}

namespace detail {
using ModFn = void (*)();
inline ModFn g_mod_fn = nullptr;
inline unsigned long __stdcall thread_thunk(void*) noexcept {
    if (g_mod_fn) g_mod_fn();
    return 0;
}
} // namespace detail

} // namespace mwsdk::mod

// Define the DLL entry point that launches FN on a background thread.
#define MWSDK_MOD(FN)                                                          \
    extern "C" int __stdcall DllMain(void* inst, unsigned long reason, void*) {\
        if (reason == /*DLL_PROCESS_ATTACH*/ 1) {                              \
            ::mwsdk::mod::detail::g_mod_fn = (FN);                             \
            ::DisableThreadLibraryCalls(reinterpret_cast<HMODULE>(inst));      \
            void* h = ::CreateThread(nullptr, 0,                              \
                &::mwsdk::mod::detail::thread_thunk, nullptr, 0, nullptr);     \
            if (h) ::CloseHandle(h);                                           \
        }                                                                      \
        return 1; /* TRUE */                                                   \
    }

#endif // MWSDK_MOD_HPP
