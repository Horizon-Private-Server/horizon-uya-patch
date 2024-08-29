/***************************************************
 * FILENAME :		vehicle.h
 * DESCRIPTION :
 * 		Contains vehicle specific offsets and structures for Deadlocked.
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_VEHICLE_H_
#define _LIBUYA_VEHICLE_H_

#include "math.h"
#include "weapon.h"
#include "moby.h"
#include "pad.h"
#include "guber.h"
#include "team.h"
#include "math3d.h"
#include "player.h"

struct tDR_Profile {
	/*   0 */ float LinearPredictionCutoff;
	/*   4 */ float LinearConvergenceThreshold;
	/*   8 */ float MaxLinearConvergenceDelta;
	/*   c */ float MinLinearConvergenceDelta;
	/*  10 */ float MinSmoothConvergenceDelta;
	/*  14 */ float PositionErrorThreshold;
	/*  18 */ float MaxTrackSpeed;
	/*  1c */ float MaxConvergeSpeed;
	/*  20 */ float Accel;
	/*  24 */ float Decel;
};

struct tDR_PositionHistory {
	/*   0 */ VECTOR Pos[1];
	/*  20 */ int Time[1];
	/*  28 */ int Oldest;
	/*  2c */ int pad;
};

struct tDR_Vars {
	/*   0 */ struct tDR_PositionHistory Pos_History;
	/*  30 */ VECTOR Velocity_Tracking;
	/*  40 */ VECTOR Accel_Tracking;
	/*  50 */ VECTOR Velocity_Converge;
	/*  60 */ VECTOR Accel_Converge;
	/*  70 */ VECTOR Pos_Start;
	/*  80 */ VECTOR Pos_Converge;
	/*  90 */ VECTOR Pos_Current;
	/*  a0 */ VECTOR Velocity_Current;
	/*  b0 */ VECTOR Accel_Current;
	/*  c0 */ VECTOR Pos_Start_Minus_One;
	/*  d0 */ VECTOR Pos_Converge_Plus_One;
	/*  e0 */ int Time_Start_Minus_One;
	/*  e4 */ int Time_Converge_Plus_One;
	/*  e8 */ int Time_StartStopping;
	/*  ec */ float Dist_Converge;
	/*  f0 */ struct tDR_Profile* pProfile;
	/*  f4 */ int Time_Start;
	/*  f8 */ int Time_Converge;
	/*  fc */ int Time_Current;
	/* 100 */ VECTOR Pos_Actual;
	/* 110 */ struct tDR_PositionHistory Pos_PendingUpdates;
	/* 140 */ int Time_ProcessPendingUpdate;
	/* 144 */ int Interval_Latency_Offset;
	/* 148 */ unsigned int flags;
	/* 14c */ int pad;
	/* 150 */ int prevKnownSyncedTime;
	/* 154 */ int KnownSyncedTime;
	/* 158 */ int PreviousUpdateTime;
	/* 15c */ int PendingSyncTime;
	/* 160 */ VECTOR PendingSyncedPos;
	/* 170 */ VECTOR KnownSyncedPos;
	/* 180 */ VECTOR prevKnownSyncedPos;
};

struct VehicleInterface {
	/*   0 */ int iVehicleType;
	/*   4 */ int bArialVehicle;
	/*   8 */ float fVehicleRadius;
	/*   c */ unsigned int iVehicleInitFlags;
	/*  10 */ int iRespawnTime;
	/*  14 */ float fPlayerRunOverMinSpeed;
	/*  18 */ float fPlayerRunOverKillSpeed;
	/*  1c */ int iShotPrimaryType;
	/*  20 */ void* SpawnShotPrimary;
	/*  24 */ int iShotSecondaryType;
	/*  28 */ void* SpawnShotSecondary;
	/*  2c */ float fVehicleTopHeight;
	/*  30 */ float fPlayerExitRadius;
	/*  34 */ int iSeatSwitchButton;
	/*  38 */ int iMPHelpMessage;
	/*  3c */ int iMPRadarIcon;
	/*  40 */ int iDriverMessage;
	/*  44 */ int iPassengerMessage;
	/*  50 */ VECTOR vDriverCameraOfs;
	/*  60 */ struct tDR_Profile DRProfile;
	/*  88 */ /* function */ void** GetVehicleBase;
	/*  8c */ /* function */ void** SetAsUnspawned;
	/*  90 */ /* function */ void** ReinitPhysics;
	/*  94 */ /* function */ void** UpdatePhysics;
	/*  98 */ /* function */ void** DamageReact;
	/*  9c */ /* function */ void** UpdateLocalDriverAttack;
	/*  a0 */ /* function */ void** UpdatePassenger;
	/*  a4 */ /* function */ void** UpdateRemoteVehicle;
	/*  a8 */ /* function */ void** UpdateVisuals;
	/*  ac */ /* function - float */ void** GetExitGroundHeight;
	/*  b0 */ /* function - bool */ void** CheckSpecialEnterRules;
	/*  b4 */ /* function - bool */ void** CheckSpecialExitRules;
	/*  b8 */ /* function - int  */ void** GetJustExitedPlayer;
	/*  bc */ /* function - void */ void** SetAutoSpawn;
	/*  c0 */ /* function - float */ void** GetCamera29Pitch;
	/*  c4 */ /* function - float */ void** HandleSpecialDamageRules;
	/*  c8 */ /* function - bool */ void** IsPlayerLockedIn;
};

typedef struct VehicleBase {
    /*   0 */ struct Guber Guber;
	/*  20 */ struct tDR_Vars drVars;
	/* 1b0 */ MATRIX invMtx;
	/* 1f0 */ VECTOR netLocalPos;
	/* 200 */ VECTOR netLocalRot;
	/* 210 */ VECTOR netUpdatedPos;
	/* 220 */ VECTOR netUpdatedRot;
	/* 230 */ VECTOR netLocalPassengerRot;
	/* 240 */ VECTOR netUpdatedPassengerRot;
	/* 250 */ VECTOR vel;
	/* 260 */ VECTOR rotSpeed;
	/* 270 */ VECTOR initPos;
	/* 280 */ VECTOR initRot;
	/* 290 */ VECTOR comeToRestStart;
	/* 2a0 */ VECTOR comeToRestTotal;

    // Unsure if Following is correct
	/* 2b0 */ int timeSinceLastMessage;
	/* 2b4 */ struct VehicleInterface* pInterface;
	/* 2b8 */ struct Player* pCutSceneDriver;
	/* 2bc */ struct Player* pCutScenePassenger;
	/* 2c0 */ struct Moby* pSpawningEffect;
	/* 2c4 */ struct Moby* pSpawningPad;
	/* 2c8 */ float fRunOverDamageMultiplier;
	/* 2cc */ int iPad;

    // Unsure if ALL of following is correct.
    // pMoby pointers and hitpoints are for a fact correct.
	/* 2d0 */ struct Moby* pMoby;
	/* 2d4 */ struct Moby* pTarget;
	/* 2d8 */ struct Player* pDriver;
	/* 2dc */ struct Player* pPassenger;
	/* 2e0 */ int netUpdatedTimeStamp;
	/* 2e4 */ int netUpdatedPassengerTimeStamp;
	/* 2e8 */ int explodeTime;
	/* 2ec */ int noRiderTimer;
	/* 2f0 */ int resurrectTimer;
	/* 2f4 */ int resurrectTimeTotal;
	/* 2f8 */ unsigned int flags;
	/* 2fc */ short int vehicleType;
	/* 2fe */ char whichGun;
	/* 2ff */ char whichGun2;
	/* 300 */ char inAir;
	/* 301 */ char masterInAir;
	/* 302 */ short int inAirTimer;
	/* 304 */ short int vehicleMobyUID;
	/* 3e6 */ short int exitToggle;
	/* 308 */ float hitPoints;
	/* 30c */ float maxHP;
	/* 310 */ float speed;
	/* 314 */ int comeToRestTime;
	/* 318 */ int waitDriverTime;
	/* 31c */ char bDriverInValidState;
	/* 31d */ char bPassengerInValidState;
	/* 31e */ char bDoPostSinkingCameraRaise;
	/* 31f */ char cPad[1];
	/* 320 */ int waitPassengerTime;
	/* 324 */ int explodeRequestSent;
	/* 328 */ int justExited;
	/* 32c */ int teamNum;
	/* 330 */ int iLastExplodedTime;
	/* 334 */ int iLastDamageTime;
	/* 338 */ float fDriverAttackDamage;
	/* 33c */ float fPassengerAttackDamage;
} Vehicle;

#endif // _LIBUYA_VEHICLE_H_
