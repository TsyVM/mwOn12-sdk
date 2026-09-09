// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/compat/memory.hpp
// ---------------------------------------------------------------------------
// Minimal MWSDK-authored memory-access helpers for the reconstructed mw05::rt
// reference layer. A few reference types expose live-tuning convenience wrappers
// (a raw typed write behind a page-unlock, and a global "memory field" holder).
// This shim provides ONLY the small surface those wrappers use, written fresh
// for MWSDK. For real hooking/patching prefer the RAII patcher in mwsdk/game/
// mw05.hpp; this exists so the reference headers are self-contained.
// ---------------------------------------------------------------------------
#ifndef MWSDK_MW05_RT_COMPAT_MEMORY_HPP
#define MWSDK_MW05_RT_COMPAT_MEMORY_HPP

#include <cstdint>
#include <cstring>
#include <type_traits>

#if defined(_WIN32)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#endif

namespace MemoryEditor {

// A typed read/write cursor over a process address. On Windows the write path
// temporarily flips page protection; elsewhere it is a plain typed store so the
// reference layer still compiles and unit-tests off-target.
class RawMemory {
  std::uintptr_t mAddress;

 public:
  explicit RawMemory(std::uintptr_t address) : mAddress(address) {}

  template <typename T>
  T GetValue() const {
    T ret{};
    std::memcpy(&ret, reinterpret_cast<const void*>(mAddress), sizeof(T));
    return ret;
  }

  template <typename T>
  bool SetValue(const T& value) const {
#if defined(_WIN32)
    DWORD old = 0;
    if (!::VirtualProtect(reinterpret_cast<LPVOID>(mAddress), sizeof(T), PAGE_EXECUTE_READWRITE, &old))
      return false;
    std::memcpy(reinterpret_cast<void*>(mAddress), &value, sizeof(T));
    DWORD tmp = 0;
    ::VirtualProtect(reinterpret_cast<LPVOID>(mAddress), sizeof(T), old, &tmp);
    return true;
#else
    std::memcpy(reinterpret_cast<void*>(mAddress), &value, sizeof(T));
    return true;
#endif
  }
};

class Editor {
 public:
  RawMemory GetRawMemory(std::uintptr_t address) const { return RawMemory(address); }
  RawMemory GetRawMemory(void* ptr) const { return RawMemory(reinterpret_cast<std::uintptr_t>(ptr)); }

  // Lightweight validity probe used by the reference extension helpers. On
  // Windows this consults VirtualQuery; elsewhere it is a null/So-far check so
  // the reference layer stays self-contained and testable off-target.
  bool ValidateMemory(std::uintptr_t address) const {
#if defined(_WIN32)
    MEMORY_BASIC_INFORMATION mbi{};
    if (!::VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi))) return false;
    if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) return false;
    return true;
#else
    return address != 0;
#endif
  }
  template <typename T>
  bool ValidateMemory(T* ptr) const {
    return ptr != nullptr && ValidateMemory(reinterpret_cast<std::uintptr_t>(ptr));
  }

  static const Editor& Get() {
    static Editor instance;
    return instance;
  }
};

inline const Editor& Get() { return Editor::Get(); }

}  // namespace MemoryEditor

// A holder for a fixed-address global "tweakable" field: read via implicit
// conversion, write via assignment (arithmetic values are range-clamped).
template <typename FieldType>
class MemoryFieldWrapper {
  FieldType* const mFieldPtr;
  const FieldType  mFieldDefVal;
  const FieldType  mFieldMinVal;
  const FieldType  mFieldMaxVal;

  MemoryFieldWrapper(const MemoryFieldWrapper&) = delete;
  MemoryFieldWrapper(MemoryFieldWrapper&&)      = delete;

 public:
  MemoryFieldWrapper(FieldType* fieldPtr, const FieldType& def, const FieldType& lo = FieldType(),
                     const FieldType& hi = FieldType())
      : mFieldPtr(fieldPtr), mFieldDefVal(def), mFieldMinVal(lo), mFieldMaxVal(hi) {}
  MemoryFieldWrapper(std::uintptr_t addr, const FieldType& def, const FieldType& lo = FieldType(),
                     const FieldType& hi = FieldType())
      : MemoryFieldWrapper(reinterpret_cast<FieldType*>(addr), def, lo, hi) {}

  FieldType&       GetField() const { return *mFieldPtr; }
  FieldType* const GetFieldPointer() const { return mFieldPtr; }
  const FieldType& GetFieldDefaultValue() const { return mFieldDefVal; }
  const FieldType& GetFieldMinimumValue() const { return mFieldMinVal; }
  const FieldType& GetFieldMaximumValue() const { return mFieldMaxVal; }

  operator FieldType&() const { return *mFieldPtr; }

  FieldType& operator=(const FieldType& newValue) const {
    if constexpr (std::is_arithmetic_v<FieldType> && !std::is_same_v<FieldType, bool>)
      if (newValue < mFieldMinVal || newValue > mFieldMaxVal) return *mFieldPtr;
    MemoryEditor::Get().GetRawMemory(reinterpret_cast<std::uintptr_t>(mFieldPtr)).template SetValue<FieldType>(newValue);
    return *mFieldPtr;
  }

  FieldType* operator&() const { return mFieldPtr; }
  FieldType& operator->() const { return *mFieldPtr; }
};

#endif  // MWSDK_MW05_RT_COMPAT_MEMORY_HPP
