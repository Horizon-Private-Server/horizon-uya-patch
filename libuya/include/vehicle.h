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

#define TURBOSLIDER_TURN_RADIUS (*(float*)0x003a124c)

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
	/* 0x000 */ struct Guber guber;
	/* 0x018 */ char unk_018[0x2b8]; 
	/* 0x2d0 */ struct Moby* pMoby;
	/* 0x2d4 */ struct Moby* pTarget;
	/* 0x2d8 */ struct Player* pDriver;
	/* 0x2dc */ struct Player* pPassenger;
	/* 0x2e0 */ int netUpdatedTimeStamp;
	/* 0x2e4 */ int netUpdatedPassengerTimeStamp;
	/* 0x2e8 */ int explodeTime;
	/* 0x2ec */ int noRiderTimer;
	/* 0x2f0 */ int resurrectTimer;
	/* 0x2f4 */ int resurrectTimeTotal;
	/* 0x2f8 */ unsigned int flags;
	/* 0x2fc */ short int vehicleType;
	/* 0x2fe */ char whichGun;
	/* 0x2ff */ char whichGun2;
	/* 0x300 */ char inAir;
	/* 0x301 */ char masterInAir;
	/* 0x302 */ short int inAirTimer;
	/* 0x304 */ short int vehicleMobyUID;
	/* 0x3e6 */ short int exitToggle;
	/* 0x308 */ float hitPoints;
	/* 0x30c */ float maxHP;
	/* 0x310 */ float speed;
	/* 0x314 */ int comeToRestTime;
	/* 0x318 */ int waitDriverTime;
	/* 0x0x31c */ char bDriverInValidState;
	/* 0x31d */ char bPassengerInValidState;
	/* 0x31e */ char bDoPostSinkingCameraRaise;
	/* 0x31f */ char cPad[1];
	/* 0x320 */ int waitPassengerTime;
	/* 0x324 */ int explodeRequestSent;
	/* 0x328 */ int justExited;
	/* 0x32c */ int teamNum;
	/* 0x330 */ int iLastExplodedTime;
	/* 0x334 */ int iLastDamageTime;
	/* 0x338 */ float fDriverAttackDamage;
	/* 0x33c */ float fPassengerAttackDamage;
} Vehicle;

#endif // _LIBUYA_VEHICLE_H_
