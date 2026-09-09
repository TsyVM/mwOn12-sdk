// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/AIPursuit.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_AIPURSUIT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_AIPURSUIT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/AIParams.h"
#include "mwsdk/game/mw05/Types/GroundSupportRequest.h"
#include "mwsdk/game/mw05/Types/IPursuit.h"
#include "mwsdk/game/mw05/Types/Sim.h"
#include "mwsdk/game/mw05/Types/Timer.h"
#include "mwsdk/game/mw05/Types/UTL/Listable.h"

namespace mwsdk::mw05::rt {
  struct AIPursuit : Sim::Activity, IPursuit {
    struct FormationTarget {
      UMath::Vector3 mOffset;
      UMath::Vector3 mInPositionOffset;
      UCrc32         mGoal;
    };
    struct CopContingent {
      UCrc32        mType;
      std::uint32_t mCount;
    };

    HSIMTASK__*                             mSimulateTask;
    HSIMTASK__*                             mBustedTimerTask;
    UTL::Listable<IVehicle*, 10>::List      mIVehicleList;
    AITarget*                               mTarget;
    IVehicle*                               mNearestCopInRoadblock;
    float                                   mDistanceToNearestCopInRoadblock;
    PursuitFormation*                       mFormation;
    IRoadBlock*                             mRoadBlock;
    eastl::vector<AIPursuit::CopContingent> mCopContingent;
    std::int32_t                            mCurrentPursuitLevel;
    float                                   mBaseHeat;
    float                                   mMaximumHeat;
    float                                   mHeatScale;
    bool                                    mAllowStatsToAccumulate;
    float                                   mInFormationTimer;
    float                                   mBreakerTimer;
    float                                   mTotalPursuitTime;
    bool                                    mCollapseActive;
    std::int32_t                            mFormationAttemptCount;
    FormationType                           mActiveFormation;
    float                                   mActiveFormationTime;
    float                                   mRoadBlockTimer;
    float                                   mSpawnCopTimer;
    float                                   mSpawnHeliTimer;
    bool                                    mDoTestForHeliSearch;
    bool                                    mForceHeliSpawnNext;
    Timer                                   mTimeSinceSetupSpeech;
    float                                   mBustedTimer;
    float                                   mBustedIncrement;
    float                                   mBustedHUDTime;
    bool                                    mIsPerpBusted;
    bool                                    mIsPursuitBailed;
    float                                   mCopDestroyedBonusTimer;
    std::int32_t                            mCopDestroyedBonusMultiplier;
    std::int32_t                            mMostRecentCopDestroyedRepPoints;
    UCrc32                                  mMostRecentCopDestroyedType;
    float                                   mEvadeLevel;
    float                                   mCoolDownTimeRemaining;
    float                                   mCoolDownTimeRequired;
    float                                   mPercentOfContingentEngaged;
    std::int32_t                            mNumCopsFullyEngaged;
    float                                   mPursuitMeter;
    bool                                    mIsPerpInSight;
    UMath::Vector3                          mLastKnownLocation;
    float                                   mHiddenZoneTime;
    float                                   mTimeSinceAnyCopSawPerp;
    bool                                    mCoolDownMeterDisplayed;
    float                                   mPursuitMeterModeTimer;
    std::int32_t                            mRepPointsPerMinute;
    std::int32_t                            mTotalCopsInvolved;
    std::int32_t                            mCopsDestroyed;
    std::int32_t                            mRepPointsFromCopsDisabled;
    std::int32_t                            mNumCopsRequiredToEvade;
    std::int32_t                            mNumCopsToTriggerBackupTime;
    std::int32_t                            mNumFullyEngagedCopsEvaded;
    std::int32_t                            mNumHeliSpawns;
    std::int32_t                            mNumRoadblocksDodged;
    std::int32_t                            mNumRoadblocksDeployed;
    std::int32_t                            mNumCopsDamaged;
    std::int32_t                            mNumCopsNeeded;
    eCrossState                             mCrossState;
    std::int32_t                            mNumTrafficCarsHit;
    std::int32_t                            mNumSpikeStripsDodged;
    bool                                    mFastSpawnNext;
    std::int32_t                            mPropertyDamageValue;
    std::int32_t                            mPropertyDamageCount;
    std::int32_t                            mNumSpikeStripsDeployed;
    std::int32_t                            mNumHeliSpikeStripsDeployed;
    std::int32_t                            mNumCopCarsDeployed;
    std::int32_t                            mNumSupportVehiclesDeployed;
    std::int32_t                            mNumSupportVehiclesActive;
    bool                                    mNextRoadblockRequest;
    GroundSupportRequest                    mGroundSupportRequest;
    float                                   mSupportCheckTimer;
    bool                                    mSupportPriorityCheckDone;
    ePursuitStatus                          mPursuitStatus;
    float                                   mBackupCountdownTimer;
    float                                   mMinDistanceToTarget;
    UMath::Vector3                          mJerkLagPosition;
    float                                   mJerkLagDistance;
    float                                   mJerkLagSpeed;
    bool                                    mIsAJerk;
    std::int32_t                            mNumRBCopsAdded;
    bool                                    mEnterSafehouseOnDestruct;

    virtual ~AIPursuit();
    virtual void OnDebugDraw();

    virtual bool                        OnTask(HSIMTASK__* hTask, float deltaTime) override;
    virtual void                        OnAttached(IAttachable* pOther) override;
    virtual void                        OnDetached(IAttachable* pOther) override;
    virtual bool                        IsTarget(AITarget* aitarget) override;
    virtual AITarget*                   GetTarget() override;
    virtual std::int32_t                GetNumCops() override;
    virtual std::int32_t                GetNumHeliSpawns() override;
    virtual std::int32_t                GetNumCopsFullyEngaged() override;
    virtual float                       GetPursuitDuration() override;
    virtual float                       GetEvadeLevel() override;
    virtual float                       GetCoolDownTimeRemaining() override;
    virtual float                       GetCoolDownTimeRequired() override;
    virtual bool                        IsPerpInSight() override;
    virtual bool                        IsPursuitBailed() override;
    virtual bool                        IsCollapseActive() override;
    virtual bool                        AttemptingToReAcquire() override;
    virtual const UMath::Vector3&       GetLastKnownLocation() override;
    virtual IRoadBlock*                 GetRoadBlock() override;
    virtual IVehicle*                   GetNearestCopInRoadblock(float* distance) override;
    virtual GroundSupportRequest*       RequestGroundSupport() override;
    virtual void                        ClearGroundSupportRequest() override;
    virtual bool                        IsSupportVehicle(IVehicle* iv) override;
    virtual const char*                 CopRequest() override;
    virtual bool                        PendingRoadBlockRequest() override;
    virtual bool                        IsFinisherActive() override;
    virtual float                       TimeToFinisherAttempt() override;
    virtual float                       TimeUntilBusted() override;
    virtual bool                        PursuitMeterCanShowBusted() override;
    virtual FormationType               GetFormationType() override;
    virtual void                        EndCurrentFormation() override;
    virtual bool                        ShouldEnd() override;
    virtual bool                        IsPerpBusted() override;
    virtual bool                        AddVehicle(IVehicle* vehicle) override;
    virtual bool                        RemoveVehicle(IVehicle* vehicle) override;
    virtual void                        AddRoadBlock(IRoadBlock* roadblock) override;
    virtual std::int32_t                RequestRoadBlock() override;
    virtual bool                        IsHeliInPursuit() override;
    virtual bool                        IsPlayerPursuit() override;
    virtual bool                        ContingentHasActiveCops() override;
    virtual std::int32_t                GetNumCopsDamaged() override;
    virtual std::int32_t                GetNumCopsDestroyed() override;
    virtual void                        IncNumCopsDestroyed(IVehicle* ivehicle) override;
    virtual std::int32_t                GetTotalNumCopsInvolved() override;
    virtual std::int32_t                GetNumRoadblocksDodged() override;
    virtual std::int32_t                GetNumRoadblocksDeployed() override;
    virtual std::int32_t                GetValueOfPropertyDamaged() override;
    virtual std::int32_t                GetNumPropertyDamaged() override;
    virtual std::int32_t                GetNumTrafficCarsHit() override;
    virtual std::int32_t                GetNumSpikeStripsDodged() override;
    virtual std::int32_t                GetNumSpikeStripsDeployed() override;
    virtual std::int32_t                GetNumHeliSpikeStripDeployed() override;
    virtual std::int32_t                GetNumCopCarsDeployed() override;
    virtual std::int32_t                GetNumSupportVehiclesDeployed() override;
    virtual std::int32_t                GetNumCopsInWave() override;
    virtual std::int32_t                GetNumCopsRemainingInWave() override;
    virtual std::int32_t                GetCopDestroyedBonusMultiplier() override;
    virtual std::int32_t                GetMostRecentCopDestroyedRepPoints() override;
    virtual UCrc32                      GetMostRecentCopDestroyedType() override;
    virtual std::uint32_t               CalcTotalCostToState() override;
    virtual void                        AddVehicleToContingent(IVehicle* ivehicle) override;
    virtual void                        NotifyRoadblockDodged() override;
    virtual void                        NotifyRoadblockDeployed() override;
    virtual void                        NotifyCopDamaged(IVehicle* ivehicle) override;
    virtual void                        NotifyPropertyDamaged(std::int32_t cost) override;
    virtual void                        NotifyTrafficCarHit() override;
    virtual void                        NotifySpikeStripsDodged(std::int32_t num) override;
    virtual void                        NotifySpikeStripDeployed() override;
    virtual void                        NotifyHeliSpikeStripDeployed(std::int32_t num) override;
    virtual void                        NotifyCopCarDeployed() override;
    virtual void                        NotifySupportVehicleDeployed() override;
    virtual void                        BailPursuit() override;
    virtual ePursuitStatus              GetPursuitStatus() override;
    virtual float                       GetTimeToBackupSpawned() override;
    virtual bool                        SkidHitEnabled() override;
    virtual float                       GetBackupETA() override;
    virtual bool                        GetIsAJerk() override;
    virtual float                       GetMinDistanceToTarget() override;
    virtual void                        SpikesHit(IVehicleAI* ivai) override;
    virtual void                        EndPursuitEnteringSafehouse() override;
    virtual bool                        GetEnterSafehouseOnDone() override;
    virtual void                        LockInPursuitAttribs() override;
    virtual void                        SetBustedTimerToZero() override;
    virtual Attrib::Gen::pursuitlevels* GetPursuitLevelAttrib() override;

    static AIPursuit* Construct(const AIParams& aiParams) {
      auto* p = reinterpret_cast<Sim::IActivity*(__cdecl*)(AIParams, UCrc32)>(0x443270)(aiParams, aiParams.mName);
      return static_cast<AIPursuit*>(p);
    }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_AIPURSUIT_H
