/* MWOn12 plugin ABI.
 *
 * This header is the contract between MWOn12 (which loads plugins) and a
 * plugin (which is loaded). It is the one file both sides compile, and it is
 * deliberately plain C: a plugin may be built with a different compiler, a
 * different C++ standard library or a different runtime from the renderer, and
 * none of that may matter. Nothing here throws, allocates across the boundary,
 * or passes a C++ type.
 *
 * A plugin is a 32-bit DLL in
 *
 *     <game>\MWOn12\Plugins\<anything>.dll
 *
 * exporting one function, MWOn12PluginMain. MWOn12 scans that directory once,
 * on the first device creation, and calls the entry point of everything it
 * finds. The plugin fills in a MWOn12_Plugin describing itself and whichever
 * callbacks it wants; every callback is optional.
 *
 *
 * WHAT THE RENDERER GUARANTEES
 *
 * Callbacks arrive on the game's render thread, never concurrently with each
 * other for the same plugin.
 *
 * OnPresent is called once per frame, with the command list open and
 * recording, the back buffer already transitioned to RENDER_TARGET and bound
 * as the sole render target with no depth buffer, and the viewport and scissor
 * covering it. That is the point in the frame where the game has finished
 * drawing and nothing has been presented yet, so anything recorded there lands
 * on top of the finished image.
 *
 * A plugin may freely change pipeline state, root signature, descriptor heaps,
 * render targets, viewport and topology inside OnPresent. The renderer
 * re-establishes all of it before its next draw and does not read anything
 * back. What a plugin must NOT do is close, reset or submit the command list,
 * or leave the back buffer in a state other than RENDER_TARGET -- those belong
 * to the renderer, and it presents the frame the moment OnPresent returns.
 *
 * Resources a plugin creates must live until OnDeviceDestroyed, and must all
 * be released by the time that callback returns: the D3D12 device is destroyed
 * immediately afterwards, and anything still holding a reference to it takes
 * the process down with a live-object report.
 *
 *
 * VERSIONING
 *
 * Both structs carry structSize and abiVersion. The renderer checks them and
 * refuses a plugin whose abiVersion it does not implement, rather than reading
 * a struct laid out differently from the one it expects. New fields are only
 * ever appended, so a plugin built against an older header stays loadable: the
 * renderer sees the smaller structSize and does not touch what is not there.
 */

#ifndef MWON12_H
#define MWON12_H

#include <stdint.h>

/* Bumped only for a change that breaks a plugin built against the previous
 * value. Appending a field to the end of a struct is not such a change. */
#define MWON12_ABI_VERSION 1u

/* The exported name MWOn12 looks for. */
#define MWON12_PLUGIN_ENTRY_NAME "MWOn12PluginMain"

/* Explicit on every function pointer. The renderer is __cdecl, and a plugin
 * project built with /Gz (stdcall by default) would otherwise hand back
 * pointers that corrupt the stack on the first call -- on x86 the mismatch is
 * silent until it is fatal. */
#if defined(_MSC_VER)
#  define MWON12_CALL __cdecl
#else
#  define MWON12_CALL
#endif

#if defined(_WIN32)
#  define MWON12_EXPORT __declspec(dllexport)
#else
#  define MWON12_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Which renderer is actually in use. DX9_PASSTHROUGH means MWOn12 found no
 * usable D3D12 adapter and handed the game to the system runtime; there is no
 * D3D12 device in that case, OnDeviceCreated never fires, and a plugin that
 * needs one should do nothing. */
typedef enum MWOn12_Backend {
    MWON12_BACKEND_DX9_PASSTHROUGH = 1,
    MWON12_BACKEND_DX12            = 3
} MWOn12_Backend;

typedef enum MWOn12_LogLevel {
    MWON12_LOG_TRACE = 0,
    MWON12_LOG_INFO  = 1,
    MWON12_LOG_WARN  = 2,
    MWON12_LOG_ERROR = 3
} MWOn12_LogLevel;

/* Result of MWOn12PluginMain. */
typedef enum MWOn12_Result {
    MWON12_DECLINE = 0,  /* not interested; unloaded, not an error */
    MWON12_OK      = 1
} MWOn12_Result;

/* The device and swap chain, as they stand right now.
 *
 * Every void* is a COM interface pointer, typed in the comment. They are
 * borrowed, not owned: do not Release them, and do not keep one past
 * OnDeviceDestroyed. AddRef one only if you also Release it before then. */
typedef struct MWOn12_DeviceInfo {
    uint32_t structSize;

    void*    device;             /* ID3D12Device*              */
    void*    commandQueue;       /* ID3D12CommandQueue*        */
    void*    swapChain;          /* IDXGISwapChain4*           */
    void*    d3d9Device;         /* IDirect3DDevice9* -- MWOn12's proxy, which
                                  * is what the game itself holds */
    void*    hwnd;               /* HWND of the render window  */

    uint32_t width;
    uint32_t height;
    uint32_t backBufferFormat;   /* DXGI_FORMAT                */
    uint32_t frameCount;         /* frames in flight; frameSlot is < this */
} MWOn12_DeviceInfo;

/* Handed to OnPresent. Valid only for the duration of that call. */
typedef struct MWOn12_Frame {
    uint32_t structSize;

    void*    device;             /* ID3D12Device*                       */
    void*    commandQueue;       /* ID3D12CommandQueue*                 */
    void*    commandList;        /* ID3D12GraphicsCommandList*, OPEN    */
    void*    swapChain;          /* IDXGISwapChain4*                    */
    void*    backBuffer;         /* ID3D12Resource*, in RENDER_TARGET   */
    void*    d3d9Device;         /* IDirect3DDevice9*                   */

    /* D3D12_CPU_DESCRIPTOR_HANDLE::ptr for the back buffer's RTV. Already
     * bound as the only render target, with no depth buffer -- given here so a
     * plugin that rebinds targets for its own passes can put it back. */
    uint64_t backBufferRtv;

    uint32_t width;
    uint32_t height;
    uint32_t backBufferFormat;   /* DXGI_FORMAT                         */

    /* Index of the frame in flight, 0 .. frameCount-1. A plugin keeping
     * per-frame resources (an upload buffer it writes each frame) must index
     * them by this, or it will overwrite memory the GPU is still reading. */
    uint32_t frameSlot;
    uint32_t frameCount;

    /* Monotonic frame number since the device was created. */
    uint64_t frameNumber;
} MWOn12_Frame;

/* Services the renderer offers a plugin. Filled in by MWOn12 and passed to
 * MWOn12PluginMain. The pointer stays valid for the life of the process, so a
 * plugin may keep it. */
typedef struct MWOn12_Host {
    uint32_t structSize;
    uint32_t abiVersion;         /* MWON12_ABI_VERSION of the renderer */

    /* e.g. "1.0.0". Owned by the renderer. */
    const char* version;

    /* Writes to MWOn12-render.log, the same file the renderer logs to, tagged
     * with the plugin's name. Trace and info are only recorded when
     * VerboseLog=1 in MWOn12.ini; warnings and errors always are. */
    void (MWON12_CALL *Log)(MWOn12_LogLevel level, const char* fmt, ...);

    /* What is actually rendering. See MWOn12_Backend. */
    MWOn12_Backend (MWON12_CALL *ActiveBackend)(void);

    /* Reads MWOn12.ini, the same file the renderer reads. A plugin should use
     * its own [Section] rather than [Renderer], so a user has one config file
     * instead of one per plugin. Returns defaultValue when absent. */
    int (MWON12_CALL *ConfigInt)(const char* section, const char* key,
                                 int defaultValue);

    /* Writes at most outBytes into out, always null-terminated when outBytes
     * is at least 1. Returns the length written, excluding the terminator. */
    int (MWON12_CALL *ConfigString)(const char* section, const char* key,
                                    const char* defaultValue,
                                    char* out, int outBytes);

    /* Absolute, with a trailing backslash. GameDirectory is where speed.exe
     * lives; PluginDirectory is <game>\MWOn12\Plugins\. Owned by the renderer. */
    const char* (MWON12_CALL *GameDirectory)(void);
    const char* (MWON12_CALL *PluginDirectory)(void);
} MWOn12_Host;

/* What the plugin fills in. Zero it first, set structSize and abiVersion, then
 * set the callbacks you want -- a null callback is simply not called.
 *
 * `user` is passed back to every callback and is never touched by the
 * renderer. It is the only per-plugin state the renderer carries, which is
 * what lets one plugin DLL be written without globals. */
typedef struct MWOn12_Plugin {
    uint32_t structSize;
    uint32_t abiVersion;         /* set to MWON12_ABI_VERSION */

    const char* name;            /* shown in the log; keep it short */
    const char* version;         /* free-form, e.g. "0.3.1" */

    void* user;

    /* The D3D12 device now exists. Create device-lifetime resources here. */
    void (MWON12_CALL *OnDeviceCreated)(void* user, const MWOn12_DeviceInfo* info);

    /* The swap chain changed size or mode. Anything sized to the back buffer
     * needs rebuilding; the device itself is unchanged. */
    void (MWON12_CALL *OnResize)(void* user, const MWOn12_DeviceInfo* info);

    /* Once per frame, on top of the finished image. See the notes at the top
     * of this header for what is bound and what may be changed. */
    void (MWON12_CALL *OnPresent)(void* user, const MWOn12_Frame* frame);

    /* The device is about to be destroyed. Release every D3D12 object the
     * plugin created before returning. */
    void (MWON12_CALL *OnDeviceDestroyed)(void* user);

    /* The process is going away. Called even if no device was ever created. */
    void (MWON12_CALL *OnShutdown)(void* user);
} MWOn12_Plugin;

/* ─────────────────────────────────────────────────────────────────────────────
 * RUNTIME REGISTRATION — for ASI mods
 *
 * A plugin is found by MWOn12 scanning a folder. An .asi is not: it is loaded
 * for its DllMain, has no entry point to call, and by the time its thread is
 * running the scan has already happened. So an ASI registers itself instead, by
 * resolving these off the loaded d3d9.dll:
 *
 *     HMODULE m = GetModuleHandleA("d3d9.dll");
 *     auto reg = (MWOn12_RegisterPluginFn)GetProcAddress(m, "MWOn12_RegisterPlugin");
 *     if (reg) reg(&myApi);
 *
 * That gives an ASI everything a plugin has, including the D3D12 device once
 * per frame. <mwon12/asi.hpp> in the SDK wraps this; the C form is here so an
 * ASI that wants no SDK at all can still do it.
 *
 * WHEN THE CALLBACKS START
 *
 * Registration is deferred, not immediate. The renderer takes the plugin on at
 * the next safe point in the frame and calls OnDeviceCreated there, on the
 * render thread, before its first OnPresent. That is what keeps the promise
 * made at the top of this header -- callbacks arrive on the render thread and
 * never concurrently -- which would be broken if registering from an ASI's own
 * thread ran OnDeviceCreated on that thread.
 *
 * So: register whenever you like, from whatever thread. Do not create D3D12
 * resources in the call; wait for OnDeviceCreated as a plugin does. If the
 * device already exists, that will be within a frame or two.
 *
 * The MWOn12_Plugin you pass is copied. The pointers inside it (name, version,
 * user) are not, and must outlive the process or your OnShutdown.
 * ────────────────────────────────────────────────────────────────────────── */

/* Returns MWON12_OK when the plugin was accepted, MWON12_DECLINE when its
 * abiVersion or structSize is one this renderer cannot read. */
typedef int (MWON12_CALL *MWOn12_RegisterPluginFn)(const MWOn12_Plugin* api);

/* Stops callbacks for the plugin registered with this `user` pointer. Takes
 * effect at the same next safe point, so a callback may still be in progress
 * when this returns; it is not a way to make unloading safe, and an ASI should
 * not unload itself anyway. */
typedef void (MWON12_CALL *MWOn12_UnregisterPluginFn)(void* user);

/* The same host table a plugin is handed. Valid for the life of the process. */
typedef const MWOn12_Host* (MWON12_CALL *MWOn12_GetHostFn)(void);

/* The exported names, for GetProcAddress. */
#define MWON12_REGISTER_PLUGIN_NAME   "MWOn12_RegisterPlugin"
#define MWON12_UNREGISTER_PLUGIN_NAME "MWOn12_UnregisterPlugin"
#define MWON12_GET_HOST_NAME          "MWOn12_GetHost"

/* The one symbol a plugin must export.
 *
 *     MWON12_EXPORT int MWON12_CALL MWOn12PluginMain(
 *         const MWOn12_Host* host, MWOn12_Plugin* out);
 *
 * Return MWON12_OK after filling in `out`, or MWON12_DECLINE to be unloaded
 * without being treated as broken -- which is the right answer when the plugin
 * finds it has nothing to do, for example because ActiveBackend() is not
 * MWON12_BACKEND_DX12.
 *
 * This runs before the D3D12 device exists. Do not create graphics resources
 * here; wait for OnDeviceCreated. */
typedef int (MWON12_CALL *MWOn12_PluginMainFn)(const MWOn12_Host* host,
                                               MWOn12_Plugin* out);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* MWON12_H */
