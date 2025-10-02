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

struct tDR_Profile { // 0x28
	/* 0x00 */ float LinearPredictionCutoff;
	/* 0x04 */ float LinearConvergenceThreshold;
	/* 0x08 */ float MaxLinearConvergenceDelta;
	/* 0x0c */ float MinLinearConvergenceDelta;
	/* 0x10 */ float MinSmoothConvergenceDelta;
	/* 0x14 */ float PositionErrorThreshold;
	/* 0x18 */ float MaxTrackSpeed;
	/* 0x1c */ float MaxConvergeSpeed;
	/* 0x20 */ float Accel;
	/* 0x24 */ float Decel;
};

struct tDR_PositionHistory { // 0x30
	/* 0x00 */ VECTOR Pos[2];
	/* 0x20 */ int Time[2];
	/* 0x28 */ int Oldest;
	/* 0x2c */ int pad[1];
};

struct tDR_Vars { // 0x190
	/* 0x000 */ struct tDR_PositionHistory Pos_History;
	/* 0x030 */ VECTOR Velocity_Tracking;
	/* 0x040 */ VECTOR Accel_Tracking;
	/* 0x050 */ VECTOR Velocity_Converge;
	/* 0x060 */ VECTOR Accel_Converge;
	/* 0x070 */ VECTOR Pos_Start;
	/* 0x080 */ VECTOR Pos_Converge;
	/* 0x090 */ VECTOR Pos_Current;
	/* 0x0a0 */ VECTOR Velocity_Current;
	/* 0x0b0 */ VECTOR Accel_Current;
	/* 0x0c0 */ VECTOR Pos_Start_Minus_One;
	/* 0x0d0 */ VECTOR Pos_Converge_Plus_One;
	/* 0x0e0 */ int Time_Start_Minus_One;
	/* 0x0e4 */ int Time_Converge_Plus_One;
	/* 0x0e8 */ int Time_StartStopping;
	/* 0x0ec */ float Dist_Converge;
	/* 0x0f0 */ struct tDR_Profile *pProfile;
	/* 0x0f4 */ int Time_Start;
	/* 0x0f8 */ int Time_Converge;
	/* 0x0fc */ int Time_Current;
	/* 0x100 */ VECTOR Pos_Actual;
	/* 0x110 */ struct tDR_PositionHistory Pos_PendingUpdates;
	/* 0x140 */ int Time_ProcessPendingUpdate;
	/* 0x144 */ int Interval_Latency_Offset;
	/* 0x148 */ unsigned int flags;
	/* 0x14c */ int pad;
	/* 0x150 */ int prevKnownSyncedTime;
	/* 0x154 */ int KnownSyncedTime;
	/* 0x158 */ int PreviousUpdateTime;
	/* 0x15c */ int PendingSyncTime;
	/* 0x160 */ VECTOR PendingSyncedPos;
	/* 0x170 */ VECTOR KnownSyncedPos;
	/* 0x180 */ VECTOR prevKnownSyncedPos;
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
	/* 0x020 */ char unk_020[0x2b0]; 
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
