// C++ convenience layer over the C ABI in mwon12.h.
//
// The ABI is plain C on purpose, but writing a plugin against it directly
// means a static struct, a set of free functions and a cast of `user` in every
// one of them. This header hides that: derive from mwon12::Plugin, override
// what you care about, and name the class once.
//
//     #include <mwon12/plugin.hpp>
//
//     class Overlay final : public mwon12::Plugin {
//     public:
//         const char* Name()    const override { return "Overlay"; }
//         const char* Version() const override { return "1.0"; }
//
//         void OnDeviceCreated(const MWOn12_DeviceInfo& d) override { ... }
//         void OnPresent(const MWOn12_Frame& f)            override { ... }
//         void OnDeviceDestroyed()                         override { ... }
//     };
//
//     MWON12_PLUGIN(Overlay)
//
// Nothing in here crosses the DLL boundary as C++: the macro emits a plain C
// entry point, and the base class lives entirely inside the plugin's own
// binary. Using this header is a choice the plugin makes alone -- the renderer
// neither knows nor cares.

#ifndef MWON12_PLUGIN_HPP
#define MWON12_PLUGIN_HPP

#include "mwon12.h"

#include <cstdarg>
#include <string>
#include <cstdio>
#include <new>

namespace mwon12 {

// The host table, available from the moment MWOn12PluginMain is entered.
//
// Defined in MWOn12SDK.lib rather than by the MWON12_PLUGIN macro, so that a
// plugin which links the library but writes its own entry point still links --
// it only has to assign this itself. SetHost() is that assignment.
extern const MWOn12_Host* g_host;

inline void SetHost(const MWOn12_Host* h) noexcept { g_host = h; }

inline const MWOn12_Host* Host() noexcept { return g_host; }

// Formatted logging into MWOn12-render.log. Safe before the host is set (it
// does nothing), which matters because a static initialiser in the plugin runs
// before the entry point does.
inline void Log(MWOn12_LogLevel level, const char* fmt, ...) noexcept
{
    if (!g_host || !g_host->Log) return;
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    const int n = std::vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n < 0) return;
    g_host->Log(level, "%s", buf);
}

// The same thing at a fixed level, which is what almost every call site wants.
#define MWON12_DEFINE_LOG_SHIM(fn, lvl)                                        \
    inline void fn(const char* fmt, ...) noexcept                              \
    {                                                                          \
        if (!g_host || !g_host->Log) return;                                   \
        char buf[1024];                                                        \
        va_list ap;                                                            \
        va_start(ap, fmt);                                                     \
        const int n = std::vsnprintf(buf, sizeof(buf), fmt, ap);               \
        va_end(ap);                                                            \
        if (n < 0) return;                                                     \
        g_host->Log(lvl, "%s", buf);                                           \
    }

MWON12_DEFINE_LOG_SHIM(LogTrace, MWON12_LOG_TRACE)
MWON12_DEFINE_LOG_SHIM(LogInfo,  MWON12_LOG_INFO)
MWON12_DEFINE_LOG_SHIM(LogWarn,  MWON12_LOG_WARN)
MWON12_DEFINE_LOG_SHIM(LogError, MWON12_LOG_ERROR)

#undef MWON12_DEFINE_LOG_SHIM

// Reads MWOn12.ini. Use your own section name rather than [Renderer].
inline int ConfigInt(const char* section, const char* key, int fallback) noexcept
{
    return (g_host && g_host->ConfigInt)
         ? g_host->ConfigInt(section, key, fallback) : fallback;
}

// The string form. Returns the value, or `fallback` when the key is absent or
// the host is not available -- so the result is always usable and there is no
// buffer for the caller to size.
inline std::string ConfigString(const char* section, const char* key,
                                const char* fallback = "") noexcept
{
    if (!g_host || !g_host->ConfigString) return fallback ? fallback : "";

    char buf[1024];
    const int n = g_host->ConfigString(section, key, fallback,
                                       buf, static_cast<int>(sizeof(buf)));
    return (n > 0) ? std::string(buf, static_cast<size_t>(n)) : std::string();
}

// Absolute, with a trailing backslash. GameDirectory is where speed.exe lives;
// PluginDirectory is <game>\MWOn12\Plugins\.
//
// Use these rather than a relative path: a plugin's working directory is the
// game's, which is not where the plugin is, and "it works on my machine" for
// that reason is the most common way a mod fails to find its own files.
inline std::string GameDirectory() noexcept
{
    const char* d = (g_host && g_host->GameDirectory) ? g_host->GameDirectory() : nullptr;
    return d ? d : "";
}

inline std::string PluginDirectory() noexcept
{
    const char* d = (g_host && g_host->PluginDirectory) ? g_host->PluginDirectory() : nullptr;
    return d ? d : "";
}

// What the renderer actually is. MWON12_BACKEND_DX9_PASSTHROUGH means no D3D12
// device exists and OnDeviceCreated will never fire -- a plugin that only draws
// should decline from OnLoad rather than sit there doing nothing.
inline MWOn12_Backend ActiveBackend() noexcept
{
    return (g_host && g_host->ActiveBackend) ? g_host->ActiveBackend()
                                             : MWON12_BACKEND_DX9_PASSTHROUGH;
}

// The renderer's version string, e.g. "1.0.0".
inline const char* RendererVersion() noexcept
{
    return (g_host && g_host->version) ? g_host->version : "unknown";
}

// Base class for a plugin.
//
// Every hook has a do-nothing default, so a plugin overrides only what it
// needs. Name() is the one thing worth always providing: it is what identifies
// the plugin in the log, and "unnamed plugin" helps nobody reading a bug
// report.
class Plugin {
public:
    virtual ~Plugin() = default;

    virtual const char* Name()    const { return "unnamed plugin"; }
    virtual const char* Version() const { return "0"; }

    // Return false to be unloaded without being reported as broken. The D3D12
    // device does not exist yet; check Host()->ActiveBackend() here if the
    // plugin is useless without one.
    virtual bool OnLoad() { return true; }

    virtual void OnDeviceCreated(const MWOn12_DeviceInfo&) {}
    virtual void OnResize(const MWOn12_DeviceInfo&)        {}
    virtual void OnPresent(const MWOn12_Frame&)            {}
    virtual void OnDeviceDestroyed()                       {}
    virtual void OnShutdown()                              {}
};

namespace detail {

inline void MWON12_CALL ThunkDeviceCreated(void* u, const MWOn12_DeviceInfo* i)
{ static_cast<Plugin*>(u)->OnDeviceCreated(*i); }

inline void MWON12_CALL ThunkResize(void* u, const MWOn12_DeviceInfo* i)
{ static_cast<Plugin*>(u)->OnResize(*i); }

inline void MWON12_CALL ThunkPresent(void* u, const MWOn12_Frame* f)
{ static_cast<Plugin*>(u)->OnPresent(*f); }

inline void MWON12_CALL ThunkDeviceDestroyed(void* u)
{ static_cast<Plugin*>(u)->OnDeviceDestroyed(); }

// Also where the object is freed. The renderer calls OnShutdown exactly once,
// last, whether or not a device was ever created, so it is the only hook that
// can own the lifetime.
inline void MWON12_CALL ThunkShutdown(void* u)
{
    auto* p = static_cast<Plugin*>(u);
    p->OnShutdown();
    delete p;
}

// Fills the ABI struct from a constructed plugin. Separated from the macro so
// the interesting part is ordinary code that a debugger can step through.
inline int Bind(const MWOn12_Host* host, MWOn12_Plugin* out, Plugin* self)
{
    g_host = host;

    if (!self) return MWON12_DECLINE;
    if (!self->OnLoad()) { delete self; return MWON12_DECLINE; }

    out->structSize        = sizeof(MWOn12_Plugin);
    out->abiVersion        = MWON12_ABI_VERSION;
    out->name              = self->Name();
    out->version           = self->Version();
    out->user              = self;
    out->OnDeviceCreated   = &ThunkDeviceCreated;
    out->OnResize          = &ThunkResize;
    out->OnPresent         = &ThunkPresent;
    out->OnDeviceDestroyed = &ThunkDeviceDestroyed;
    out->OnShutdown        = &ThunkShutdown;
    return MWON12_OK;
}

}  // namespace detail
}  // namespace mwon12

// Emits the exported entry point. Put it once, at file scope, in exactly one
// translation unit of the plugin.
#define MWON12_PLUGIN(ClassName)                                               \
    extern "C" MWON12_EXPORT int MWON12_CALL                                   \
    MWOn12PluginMain(const MWOn12_Host* host, MWOn12_Plugin* out)              \
    {                                                                          \
        if (!host || !out) return MWON12_DECLINE;                              \
        if (host->abiVersion != MWON12_ABI_VERSION) return MWON12_DECLINE;     \
        return ::mwon12::detail::Bind(host, out,                               \
                                      new (std::nothrow) ClassName());         \
    }

#endif  // MWON12_PLUGIN_HPP
