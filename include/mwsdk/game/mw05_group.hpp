// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05_group.hpp
// =========================
// Level-3 batch management: a PatchGroup that owns many reversible byte patches
// and toggles them together — the MWSDK analogue of a hooking library's "group".
//
// A trainer usually has several switches, each backed by one or more byte
// patches. Managing them individually is fiddly; a PatchGroup lets you register
// them once (with a tag), then enable/disable/remove the whole set in one call.
// Because every entry is an RAII mw05::Patch, the group reverts everything it
// owns when it is destroyed — you cannot leak a live patch.
//
//   auto trainer = mw05::group("Trainer");
//   trainer.nop  (mw05::fn::BustCheck, 6, "NeverBusted")
//          .patch(mw05::fn::HeatWrite, {0x90,0x90}, "FreezeHeat");
//
//   trainer.disable();                 // revert all, keep them registered
//   trainer.enable();                  // re-apply all
//   trainer.at("NeverBusted")->disable();   // toggle just one
//   // trainer's destructor reverts everything still live.
//
// This is Windows-only (it patches the live image), mirroring mw05::Patch. The
// bookkeeping is a plain vector of {Va, bytes, tag, Patch}, so re-enabling after
// a disable re-applies from the stored descriptor.

#ifndef MWSDK_GAME_MW05_GROUP_HPP
#define MWSDK_GAME_MW05_GROUP_HPP

#include <cstdint>
#include <initializer_list>

#include "mwsdk/game/mw05.hpp"   // Image, Patch, process(), Va

#if MWSDK_MW05_RUNTIME

#include <string>
#include <string_view>
#include <vector>

namespace mwsdk::mw05 {

// One registered patch: its descriptor (so it can be re-applied) plus the live
// RAII Patch. A single value type; move-only because Patch is move-only.
struct PatchEntry {
    Va                        va = Va{0};
    std::vector<std::uint8_t> bytes;   // replacement bytes (for re-enable)
    std::string               tag;
    Patch                     patch;   // live handle; reverts on destruction

    [[nodiscard]] bool enabled() const noexcept { return patch.active(); }

    // (Re)apply from the stored descriptor against the given image.
    bool enable(const Image& img) noexcept {
        return patch.apply(img, va, bytes.data(), bytes.size());
    }
    void disable() noexcept { patch.restore(); }
};

// A named collection of reversible patches, toggled together.
class PatchGroup {
public:
    PatchGroup() = default;
    explicit PatchGroup(std::string name) noexcept : name_(std::move(name)) {}

    PatchGroup(const PatchGroup&)            = delete;
    PatchGroup& operator=(const PatchGroup&) = delete;
    PatchGroup(PatchGroup&&)                 = default;
    PatchGroup& operator=(PatchGroup&&)      = default;

    // --- registration (applies immediately) --------------------------------

    // Register + apply an explicit byte patch, tracked for later toggling.
    PatchGroup& patch(Va va, std::initializer_list<std::uint8_t> bytes,
                      std::string tag = {}) noexcept {
        PatchEntry e;
        e.va    = va;
        e.bytes.assign(bytes.begin(), bytes.end());
        e.tag   = std::move(tag);
        e.enable(process());
        entries_.push_back(std::move(e));
        return *this;
    }

    // Register + apply a run of NOPs (0x90).
    PatchGroup& nop(Va va, std::size_t len, std::string tag = {}) noexcept {
        PatchEntry e;
        e.va    = va;
        e.bytes.assign(len, 0x90);
        e.tag   = std::move(tag);
        e.enable(process());
        entries_.push_back(std::move(e));
        return *this;
    }

    // --- lifecycle ----------------------------------------------------------

    // Re-apply every patch that is currently reverted. Returns *this.
    PatchGroup& enable() noexcept {
        const Image& img = process();
        for (auto& e : entries_)
            if (!e.enabled()) e.enable(img);
        return *this;
    }

    // Revert every patch but keep it registered (re-enable brings it back).
    PatchGroup& disable() noexcept {
        for (auto& e : entries_) e.disable();
        return *this;
    }

    // Revert and drop every patch. The group is empty but still usable.
    void remove_all() noexcept { entries_.clear(); }  // ~Patch reverts each
    void clear()      noexcept { remove_all(); }

    // --- lookup / introspection --------------------------------------------

    [[nodiscard]] std::size_t size()  const noexcept { return entries_.size(); }
    [[nodiscard]] bool        empty() const noexcept { return entries_.empty(); }
    [[nodiscard]] const std::string& name() const noexcept { return name_; }

    // True only if every registered patch is currently applied.
    [[nodiscard]] bool enabled() const noexcept {
        for (const auto& e : entries_) if (!e.enabled()) return false;
        return !entries_.empty();
    }

    // Find a registered patch by tag, or nullptr. Use to toggle one switch:
    //   if (auto* e = g.at("NeverBusted")) e->disable();
    [[nodiscard]] PatchEntry* at(std::string_view tag) noexcept {
        for (auto& e : entries_) if (e.tag == tag) return &e;
        return nullptr;
    }
    [[nodiscard]] const PatchEntry* at(std::string_view tag) const noexcept {
        for (const auto& e : entries_) if (e.tag == tag) return &e;
        return nullptr;
    }

    // Iteration (range-for): for (auto& e : group) ...
    [[nodiscard]] PatchEntry*       begin() noexcept { return entries_.data(); }
    [[nodiscard]] PatchEntry*       end()   noexcept { return entries_.data() + entries_.size(); }
    [[nodiscard]] const PatchEntry* begin() const noexcept { return entries_.data(); }
    [[nodiscard]] const PatchEntry* end()   const noexcept { return entries_.data() + entries_.size(); }

private:
    std::string             name_;
    std::vector<PatchEntry> entries_;
};

// Level-1 factory — mirrors the one-liner feel of the rest of the SDK.
[[nodiscard]] inline PatchGroup group(std::string name = {}) {
    return PatchGroup(std::move(name));
}

} // namespace mwsdk::mw05

#endif // MWSDK_MW05_RUNTIME
#endif // MWSDK_GAME_MW05_GROUP_HPP
