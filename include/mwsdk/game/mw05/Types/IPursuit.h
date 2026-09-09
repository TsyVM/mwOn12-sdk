// SPDX-License-Identifier: MIT
//
// mwsdk/game/mw05/Types/IPursuit.h
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

#ifndef MWSDK_MW05_RT_GAME_MW05_TYPES_IPURSUIT_H
#define MWSDK_MW05_RT_GAME_MW05_TYPES_IPURSUIT_H
#include "mwsdk/game/mw05/compat/prologue.hpp"
#pragma once

#include "mwsdk/game/mw05/Types.h"
#include "mwsdk/game/mw05/Types/UTL.h"

namespace mwsdk::mw05::rt {
  struct IPursuit : UTL::COM::IUnknown {
    virtual ~IPursuit();

    virtual bool                        IsTarget(AITarget* aitarget)                   = 0;
    virtual AITarget*                   GetTarget()                                    = 0;
    virtual std::int32_t                GetNumCops()                                   = 0;
    virtual std::int32_t                GetNumHeliSpawns()                             = 0;
    virtual std::int32_t                GetNumCopsFullyEngaged()                       = 0;
    virtual float                       GetPursuitDuration()                           = 0;
    virtual float                       GetEvadeLevel()                                = 0;
    virtual float                       GetCoolDownTimeRemaining()                     = 0;
    virtual float                       GetCoolDownTimeRequired()                      = 0;
    virtual bool                        IsPerpInSight()                                = 0;
    virtual bool                        IsPursuitBailed()                              = 0;
    virtual bool                        IsCollapseActive()                             = 0;
    virtual bool                        AttemptingToReAcquire()                        = 0;
    virtual const UMath::Vector3&       GetLastKnownLocation()                         = 0;
    virtual IRoadBlock*                 GetRoadBlock()                                 = 0;
    virtual IVehicle*                   GetNearestCopInRoadblock(float* distance)      = 0;
    virtual GroundSupportRequest*       RequestGroundSupport()                         = 0;
    virtual void                        ClearGroundSupportRequest()                    = 0;
    virtual bool                        IsSupportVehicle(IVehicle* iv)                 = 0;
    virtual const char*                 CopRequest()                                   = 0;
    virtual bool                        PendingRoadBlockRequest()                      = 0;
    virtual bool                        IsFinisherActive()                             = 0;
    virtual float                       TimeToFinisherAttempt()                        = 0;
    virtual float                       TimeUntilBusted()                              = 0;
    virtual bool                        PursuitMeterCanShowBusted()                    = 0;
    virtual FormationType               GetFormationType()                             = 0;
    virtual void                        EndCurrentFormation()                          = 0;
    virtual bool                        ShouldEnd()                                    = 0;
    virtual bool                        IsPerpBusted()                                 = 0;
    virtual bool                        AddVehicle(IVehicle* vehicle)                  = 0;
    virtual bool                        RemoveVehicle(IVehicle* vehicle)               = 0;
    virtual void                        AddRoadBlock(IRoadBlock* roadblock)            = 0;
    virtual std::int32_t                RequestRoadBlock()                             = 0;
    virtual bool                        IsHeliInPursuit()                              = 0;
    virtual bool                        IsPlayerPursuit()                              = 0;
    virtual bool                        ContingentHasActiveCops()                      = 0;
    virtual std::int32_t                GetNumCopsDamaged()                            = 0;
    virtual std::int32_t                GetNumCopsDestroyed()                          = 0;
    virtual void                        IncNumCopsDestroyed(IVehicle* ivehicle)        = 0;
    virtual std::int32_t                GetTotalNumCopsInvolved()                      = 0;
    virtual std::int32_t                GetNumRoadblocksDodged()                       = 0;
    virtual std::int32_t                GetNumRoadblocksDeployed()                     = 0;
    virtual std::int32_t                GetValueOfPropertyDamaged()                    = 0;
    virtual std::int32_t                GetNumPropertyDamaged()                        = 0;
    virtual std::int32_t                GetNumTrafficCarsHit()                         = 0;
    virtual std::int32_t                GetNumSpikeStripsDodged()                      = 0;
    virtual std::int32_t                GetNumSpikeStripsDeployed()                    = 0;
    virtual std::int32_t                GetNumHeliSpikeStripDeployed()                 = 0;
    virtual std::int32_t                GetNumCopCarsDeployed()                        = 0;
    virtual std::int32_t                GetNumSupportVehiclesDeployed()                = 0;
    virtual std::int32_t                GetNumCopsInWave()                             = 0;
    virtual std::int32_t                GetNumCopsRemainingInWave()                    = 0;
    virtual std::int32_t                GetCopDestroyedBonusMultiplier()               = 0;
    virtual std::int32_t                GetMostRecentCopDestroyedRepPoints()           = 0;
    virtual UCrc32                      GetMostRecentCopDestroyedType()                = 0;
    virtual std::uint32_t               CalcTotalCostToState()                         = 0;
    virtual void                        AddVehicleToContingent(IVehicle* ivehicle)     = 0;
    virtual void                        NotifyRoadblockDodged()                        = 0;
    virtual void                        NotifyRoadblockDeployed()                      = 0;
    virtual void                        NotifyCopDamaged(IVehicle* ivehicle)           = 0;
    virtual void                        NotifyPropertyDamaged(std::int32_t cost)       = 0;
    virtual void                        NotifyTrafficCarHit()                          = 0;
    virtual void                        NotifySpikeStripsDodged(std::int32_t num)      = 0;
    virtual void                        NotifySpikeStripDeployed()                     = 0;
    virtual void                        NotifyHeliSpikeStripDeployed(std::int32_t num) = 0;
    virtual void                        NotifyCopCarDeployed()                         = 0;
    virtual void                        NotifySupportVehicleDeployed()                 = 0;
    virtual void                        BailPursuit()                                  = 0;
    virtual ePursuitStatus              GetPursuitStatus()                             = 0;
    virtual float                       GetTimeToBackupSpawned()                       = 0;
    virtual bool                        SkidHitEnabled()                               = 0;
    virtual float                       GetBackupETA()                                 = 0;
    virtual bool                        GetIsAJerk()                                   = 0;
    virtual float                       GetMinDistanceToTarget()                       = 0;
    virtual void                        SpikesHit(IVehicleAI* ivai)                    = 0;
    virtual void                        EndPursuitEnteringSafehouse()                  = 0;
    virtual bool                        GetEnterSafehouseOnDone()                      = 0;
    virtual void                        LockInPursuitAttribs()                         = 0;
    virtual void                        SetBustedTimerToZero()                         = 0;
    virtual Attrib::Gen::pursuitlevels* GetPursuitLevelAttrib()                        = 0;

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x4038C0)(); }
  };
}  // namespace mwsdk::mw05::rt

#endif  // MWSDK_MW05_RT_GAME_MW05_TYPES_IPURSUIT_H
