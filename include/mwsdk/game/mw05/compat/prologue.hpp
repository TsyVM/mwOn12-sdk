// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/compat/prologue.hpp
// ---------------------------------------------------------------------------
// Small MWSDK-authored prologue for the reconstructed mw05::rt reference layer.
// The reference types describe a 32-bit MSVC target (speed.exe), so they name
// x86 calling conventions, the Win32 flag-enum operator macro, and byte-swap
// intrinsics. Off-MSVC toolchains don't know those, so we provide portable
// equivalents purely so the headers still PARSE for tooling/tests. None of this
// changes anything on the real MSVC/Windows target.
// ---------------------------------------------------------------------------
#ifndef MWSDK_MW05_RT_COMPAT_PROLOGUE_HPP
#define MWSDK_MW05_RT_COMPAT_PROLOGUE_HPP

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cfloat>
#include <type_traits>

#if !defined(_MSC_VER)
#  ifndef __thiscall
#    define __thiscall
#  endif
#  ifndef __cdecl
#    define __cdecl
#  endif
#  ifndef __stdcall
#    define __stdcall
#  endif
#  ifndef __fastcall
#    define __fastcall
#  endif
   // glibc's <byteswap.h> spells these bswap_16/etc.; the reference sources use
   // the un-underscored form. Map to compiler builtins on non-MSVC.
#  ifndef bswap16
#    define bswap16 __builtin_bswap16
#  endif
#  ifndef bswap32
#    define bswap32 __builtin_bswap32
#  endif
#  ifndef bswap64
#    define bswap64 __builtin_bswap64
#  endif
#endif

// Portable stand-in for the Win32 <winnt.h> macro when building off-Windows.
#if !defined(_WIN32) && !defined(DEFINE_ENUM_FLAG_OPERATORS)
#  define DEFINE_ENUM_FLAG_OPERATORS(E)                                                                       \
    inline constexpr E   operator|(E a, E b) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) |  static_cast<std::underlying_type_t<E>>(b)); } \
    inline constexpr E   operator&(E a, E b) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) &  static_cast<std::underlying_type_t<E>>(b)); } \
    inline constexpr E   operator^(E a, E b) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) ^  static_cast<std::underlying_type_t<E>>(b)); } \
    inline constexpr E   operator~(E a)      { return static_cast<E>(~static_cast<std::underlying_type_t<E>>(a)); }                                             \
    inline E&            operator|=(E& a, E b) { a = a | b; return a; }                                       \
    inline E&            operator&=(E& a, E b) { a = a & b; return a; }                                       \
    inline E&            operator^=(E& a, E b) { a = a ^ b; return a; }
#endif

#endif  // MWSDK_MW05_RT_COMPAT_PROLOGUE_HPP
