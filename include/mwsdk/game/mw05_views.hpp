// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05_views.hpp
// =========================
// Ergonomic typed views over live game objects. These are thin, zero-cost
// wrappers that turn the verified offset form
//
//   float& t = mw05::field<float>(v, mw05::layout::AIVehicle::mTopSpeed);
//
// into the ergonomic one-liner you'd expect:
//
//   mw05::VehicleView(v).top_speed() *= 1.10f;   // +10% top speed
//
// Every accessor returns a REFERENCE into the live object, so it reads and
// writes. Every accessor maps 1:1 to a single `mw05::layout::<Class>::<member>`
// offset — the views add no new addresses and invent nothing; they are pure
// sugar over the already-verified layout table (Encyclopedia Discovery 34). A
// view is a single pointer: copy it freely, it owns nothing.
//
// Only NAMED members are surfaced as named accessors, so provenance stays clean.
// Name-stripped getter slots keep their honest `slot_0xNN` form and are reached
// with the generic `field<T>()` if you need them. Where a member's confidence is
// behavioural rather than static-verified, the accessor says so in its comment.
//
// This header is pure pointer arithmetic — no OS, no live-call — so it compiles
// and unit-tests on any platform against a plain buffer.

#ifndef MWSDK_GAME_MW05_VIEWS_HPP
#define MWSDK_GAME_MW05_VIEWS_HPP

#include <cstdint>

#include "mwsdk/game/mw05.hpp"           // field<T>(), Image, identify()
#include "mwsdk/game/mw05_layouts.inl"   // layout::<Class>::<member>

namespace mwsdk::mw05 {

// Common base: hold a live object pointer, expose it, and give slot access.
class ObjectView {
public:
    ObjectView() noexcept = default;
    explicit ObjectView(void* obj) noexcept : obj_(obj) {}

    [[nodiscard]] void* raw()   const noexcept { return obj_; }
    [[nodiscard]] bool  valid() const noexcept { return obj_ != nullptr; }
    explicit operator bool() const noexcept { return obj_ != nullptr; }

    // Escape hatch: typed reference at any verified offset (e.g. a slot_0xNN).
    template <class T>
    [[nodiscard]] T& at(Off off) const noexcept { return field<T>(obj_, off); }

protected:
    void* obj_ = nullptr;
};

// ---------------------------------------------------------------------------
// AIVehicle — the car's AI-side motion parameters. All offsets [verified].
//   +0x38 mDriveSpeed, +0x84 mTopSpeed, +0x6C4 mAccelerationTableData[10],
//   +0x6EC mAccelerationMaxSpeed.
// ---------------------------------------------------------------------------
class VehicleView : public ObjectView {
public:
    using ObjectView::ObjectView;

    [[nodiscard]] float& drive_speed()     const noexcept { return field<float>(obj_, layout::AIVehicle::mDriveSpeed); }        // m/s [verified]
    [[nodiscard]] float& top_speed()       const noexcept { return field<float>(obj_, layout::AIVehicle::mTopSpeed); }          // m/s [verified]
    [[nodiscard]] float& accel_max_speed() const noexcept { return field<float>(obj_, layout::AIVehicle::mAccelerationMaxSpeed); } // curve X-max [verified]

    // The 10-entry accel-vs-speed curve (f32[10] at +0x6C4). Returned as a
    // pointer to the first element; index 0..9.
    [[nodiscard]] float* accel_table() const noexcept {
        return &field<float>(obj_, layout::AIVehicle::mAccelerationTableData);
    }
    static constexpr int kAccelTableSize = 10;
};

// ---------------------------------------------------------------------------
// AIActionTraffic — a traffic car's governor. All offsets [verified].
// ---------------------------------------------------------------------------
class TrafficActionView : public ObjectView {
public:
    using ObjectView::ObjectView;

    [[nodiscard]] float&         target_speed() const noexcept { return field<float>(obj_, layout::AIActionTraffic::mTargetSpeed); } // m/s [verified]
    [[nodiscard]] float&         speed_limit()  const noexcept { return field<float>(obj_, layout::AIActionTraffic::mSpeedLimit); }  // m/s [verified]
    [[nodiscard]] std::uint32_t& crash_state()  const noexcept { return field<std::uint32_t>(obj_, layout::AIActionTraffic::mCrashState); } // 0/1/2 [verified]
    [[nodiscard]] float&         crash_timer()  const noexcept { return field<float>(obj_, layout::AIActionTraffic::mCrashTimer); }  // s [verified]
    [[nodiscard]] std::uint32_t& recovery()     const noexcept { return field<std::uint32_t>(obj_, layout::AIActionTraffic::mRecovery); }   // 0/1/2 [verified]
};

// ---------------------------------------------------------------------------
// AICopManager — the pursuit fleet manager.
//   Counts/caps [verified]; the list bases live via field<T>() if you need them.
// ---------------------------------------------------------------------------
class CopManagerView : public ObjectView {
public:
    using ObjectView::ObjectView;

    [[nodiscard]] std::uint32_t& spawn_request_count() const noexcept { return field<std::uint32_t>(obj_, layout::AICopManager::mSpawnRequestCount); } // [verified]
    [[nodiscard]] std::uint32_t& breaker_zone_count()  const noexcept { return field<std::uint32_t>(obj_, layout::AICopManager::mBreakerZoneCount); }  // [verified]
    [[nodiscard]] std::uint32_t& active_fleet()        const noexcept { return field<std::uint32_t>(obj_, layout::AICopManager::mActiveFleet); }       // [verified]
    [[nodiscard]] std::uint32_t& fleet_cap()           const noexcept { return field<std::uint32_t>(obj_, layout::AICopManager::mFleetCap); }          // [verified]
};

// ---------------------------------------------------------------------------
// AIPursuit — one active pursuit. Offsets are [behavior]-confidence (RE from
// behaviour, not static symbols): a live tuning knob, but confirm before ship.
// ---------------------------------------------------------------------------
class PursuitView : public ObjectView {
public:
    using ObjectView::ObjectView;

    [[nodiscard]] std::uint32_t& active()       const noexcept { return field<std::uint32_t>(obj_, layout::AIPursuit::mActive); }      // [behavior]
    [[nodiscard]] std::uint32_t& cops_damaged() const noexcept { return field<std::uint32_t>(obj_, layout::AIPursuit::mCopsDamaged); } // [behavior]
};

// ---------------------------------------------------------------------------
// AIPerpVehicle — the busting state machine on the player-as-perp side.
// Offsets [behavior]-confidence. The classic "never get busted" trainer flips
// bustable() to 0 / holds the meter.
// ---------------------------------------------------------------------------
class PerpView : public ObjectView {
public:
    using ObjectView::ObjectView;

    [[nodiscard]] float&         bust_gauge()      const noexcept { return field<float>(obj_, layout::AIPerpVehicle::mBustGauge); }        // [behavior]
    [[nodiscard]] float&         bust_meter()      const noexcept { return field<float>(obj_, layout::AIPerpVehicle::mBustMeter); }        // [behavior]
    [[nodiscard]] float&         bust_hold_timer() const noexcept { return field<float>(obj_, layout::AIPerpVehicle::mBustHoldTimer); }    // [behavior]
    [[nodiscard]] std::uint32_t& bustable()        const noexcept { return field<std::uint32_t>(obj_, layout::AIPerpVehicle::mBustableFlag); } // [behavior]
    [[nodiscard]] std::uint32_t& state()           const noexcept { return field<std::uint32_t>(obj_, layout::AIPerpVehicle::mState); }        // [behavior] ==3 -> busted
};

// ---------------------------------------------------------------------------
// LocalPlayer — the player object. mVehicle (+0x44) is the bound IVehicle*
// (set by OnVehicleAdded/Removed). [behavior]-confidence.
// ---------------------------------------------------------------------------
class PlayerView : public ObjectView {
public:
    using ObjectView::ObjectView;

    // The player's current vehicle pointer (an IVehicle*; may be null between
    // OnVehicleRemoved and the next OnVehicleAdded).
    [[nodiscard]] void*& vehicle() const noexcept { return field<void*>(obj_, layout::LocalPlayer::mVehicle); } // [behavior]
};

// ---------------------------------------------------------------------------
// AIRacerBrain — opponent AI. mSkillScalar (+0x7A8) is the catch-up/rubber-band
// difficulty scalar. [behavior]-confidence.
// ---------------------------------------------------------------------------
class RacerBrainView : public ObjectView {
public:
    using ObjectView::ObjectView;

    [[nodiscard]] float& skill_scalar() const noexcept { return field<float>(obj_, layout::AIRacerBrain::mSkillScalar); } // [behavior]
};

} // namespace mwsdk::mw05

#endif // MWSDK_GAME_MW05_VIEWS_HPP
