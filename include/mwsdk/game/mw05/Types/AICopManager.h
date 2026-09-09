// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/AICopManager.h
// ---------------------------------------------------------------------------
// Reconstructed live-object layout for Need for Speed: Most Wanted (2005),
// speed.exe v1.3. Community-reverse-engineered [reference]-confidence data,
// re-expressed in MWSDK form under namespace mwsdk::mw05::rt.
//
// NOT part of MWSDK's verified core tables (mw05_db.inl / mw05_layouts.inl).
// Member offsets are implied by declaration order and depend on target ABI;
// treat addresses/offsets here as leads to confirm, not ground truth. See
// mwsdk/game/mw05/README.md for provenance and the verified/reference split.
// ---------------------------------------------------------------------------

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_AICOPMANAGER_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_AICOPMANAGER_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/AISpawnManager.h"
#include "mwsdk/game/mw05/Types/ICopMgr.h"
#include "mwsdk/game/mw05/Types/IVehicleCache.h"
#include "mwsdk/game/mw05/Types/Sim.h"
#include "mwsdk/game/mw05/Types/UTL/Listable.h"

namespace mwsdk::mw05::rt {
  struct AICopManager : Sim::Activity, AISpawnManager, ICopMgr, IVehicleCache {
    struct BreakerZone {
      UMath::Vector3 mPosition;
      float          mEndtime;
      float          mRadius;
    };
    struct SpawnCopRequest {
      UMath::Vector3 mInitialPos;
      UMath::Vector3 mInitialVec;
      char           mVehicleName[32];
      bool           mInPursuit;
      bool           mInRoadBlock;
    };

    eastl::list<SpawnCopRequest>       mSpawnRequests;
    eastl::list<BreakerZone>           mBreakerZones;
    HSIMTASK__*                        mSimulateTask;
    std::int32_t                       mMaxCopCars;
    std::int32_t                       mMaxCopHelicopters;
    std::int32_t                       mPlatformBudgetCopCars;
    std::int32_t                       mMaxPatrolCopCars;
    std::int32_t                       mNumActiveCopCars;
    std::int32_t                       mMaxActiveCopCars;
    std::int32_t                       mNumActiveCopHelicopters;
    std::int32_t                       mMaxActiveCopHelicopters;
    std::int32_t                       mPursuitsInARow;
    std::int32_t                       mTotalCopsDestroyed;
    float                              mLockoutTimer;
    float                              mHeavySupportDelayTimer;
    bool                               mNoNewPursuitsOrCops;
    std::int32_t                       mNumCopsForLatchedRoadblockReq;
    IPursuit*                          mIPursuitWithLatchedRoadblockReq;
    IVehicle*                          mPursuitRequestVehicle;
    UTL::Listable<IVehicle*, 10>::List mIVehicleList;
    eastl::list<IPursuit*>             mIPursuitList;
    eastl::list<IRoadBlock*>           mRoadBlockList;
    ActionQueue*                       mActionQ;
    IActivity*                         mSpeech;
    AttributeSet*                      mAttributes;
    std::uint32_t*                     mMessSpawnCop;
    std::uint32_t*                     mMessSetAutoSpawn;
    std::uint32_t*                     mMessSetCopsEnabled;
    std::uint32_t*                     mMessBreakerStopCops;
    std::uint32_t*                     mMessForcePursuitStart;

    virtual ~AICopManager();
    virtual void OnDebugDraw();

    virtual bool        OnTask(HSIMTASK__* p, float) override;
    virtual void        OnAttached(IAttachable* pOther) override;
    virtual void        OnDetached(IAttachable* pOther) override;
    virtual bool        VehicleSpawningEnabled(bool isDespawn) override;
    virtual void        ResetCopsForRestart(bool release) override;
    virtual void        SetAllBustedTimersToZero() override;
    virtual void        PursuitIsEvaded(IPursuit* ipursuit) override;
    virtual bool        IsCopRequestPending() override;
    virtual bool        IsCopSpawnPending() override;
    virtual void        SpawnCop(UMath::Vector3& initialPos, UMath::Vector3& initialVec, const char* vehicleName,
                                 bool inPursuit, bool roadBlock) override;
    virtual bool        PlayerPursuitHasCop() override;
    virtual bool        CanPursueRacers() override;
    virtual bool        IsPlayerPursuitActive() override;
    virtual void        LockoutCops(bool lockout) override;
    virtual void        NoNewPursuitsOrCops() override;
    virtual void        PursueAtHeatLevel(int minHeatLevel) override;
    virtual float       GetLockoutTimeRemaining() override;
    virtual const char* GetCacheName() override;
    virtual eVehicleCacheResult OnQueryVehicleCache(const IVehicle*      removethis,
                                                    const IVehicleCache* whosasking) override;
    virtual void                OnRemovedVehicleCache(IVehicle* ivehicle) override;
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_AICOPMANAGER_H
