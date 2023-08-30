/***************************************************
 * FILENAME :		player.h
 * 
 * DESCRIPTION :
 * 		Contains player specific offsets and structures for UYA.
 * 
 * NOTES :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_PLAYER_H_
#define _LIBUYA_PLAYER_H_

#include "vehicle.h"
#include "math.h"
#include "pad.h"
#include "math3d.h"
#include "common.h"
#include "guber.h"
#include "moby.h"
#include "weapon.h"
#include "camera.h"

/*
 * Maximum health of player.
 */
#define PLAYER_MAX_HEALTH                   (15)

#ifdef UYA_PAL
#define TNW_PLAYERDATA                      (*(u32*)0x0022f3d4)
#define PLAYER_STRUCT_POINTER               (*(u32*)0x00225cf0)
#else
#define TNW_PLAYERDATA                      (*(u32*)0x0022f554)
#define PLAYER_STRUCT_POINTER               (*(u32*)0x00225e70)
#endif

#define PLAYER_STRUCT                       (PLAYER_STRUCT_POINTER - 0x430C)

/*
 * NAME :		PlayerCameraType
 * 
 * DESCRIPTION :
 * 			Defines the three player camera types.
 *          These are used in the player struct under 'CameraType'
 * 
 * NOTES :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
enum PlayerCameraType
{
    CAMERA_TYPE_THIRD_PERSON,
    CAMERA_TYPE_LOCK_STRAFE,
    CAMERA_TYPE_FIRST_PERSON
};

/*
 * NAME :		PlayerState
 * 
 * DESCRIPTION :
 * 			Defines the player states.
 * 
 * NOTES :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
enum PlayerState {
	PLAYER_STATE_IDLE = 0,
	PLAYER_STATE_LOOK = 1,
	PLAYER_STATE_WALK = 2,
	PLAYER_STATE_SKID = 3,
	PLAYER_STATE_CROUCH = 4,
	PLAYER_STATE_QUICK_TURN = 5,
	PLAYER_STATE_FALL = 6,
	PLAYER_STATE_JUMP = 7,
	PLAYER_STATE_GLIDE = 8,
	PLAYER_STATE_RUN_JUMP = 9,
	PLAYER_STATE_LONG_JUMP = 10,
	PLAYER_STATE_FLIP_JUMP = 11,
	PLAYER_STATE_JINK_JUMP = 12,
	PLAYER_STATE_ROCKET_JUMP = 13,
	PLAYER_STATE_DOUBLE_JUMP = 14,
	PLAYER_STATE_HELI_JUMP = 15,
	PLAYER_STATE_CHARGE_JUMP = 16,
	PLAYER_STATE_WALL_JUMP = 17,
	PLAYER_STATE_WATER_JUMP = 18,
	PLAYER_STATE_COMBO_ATTACK = 19,
	PLAYER_STATE_JUMP_ATTACK = 20,
	PLAYER_STATE_THROW_ATTACK = 21,
	PLAYER_STATE_GET_HIT = 22,
	PLAYER_STATE_LEDGE_GRAB = 23,
	PLAYER_STATE_LEDGE_IDLE = 24,
	PLAYER_STATE_LEDGE_TRAVERSE_LEFT = 25,
	PLAYER_STATE_LEDGE_TRAVERSE_RIGHT = 26,
	PLAYER_STATE_LEDGE_JUMP = 27,
	PLAYER_STATE_VISIBOMB = 28,
	PLAYER_STATE_TARGETING = 29,
	PLAYER_STATE_GUN_WAITING = 30,
	PLAYER_STATE_WALLOPER_ATTACK = 31,
	PLAYER_STATE_ATTACK_BOUNCE = 32,
	PLAYER_STATE_ROCKET_STOMP = 33,
	PLAYER_STATE_GLOVE_ATTACK = 34,
	PLAYER_STATE_GRAPPLE_SHOOT = 35,
	PLAYER_STATE_GRAPPLE_PULL = 36,
	PLAYER_STATE_GRAPPLE_PULL_VEHICLE = 37,
	PLAYER_STATE_SUCK_CANNON = 38,
	PLAYER_STATE_GRIND = 39,
	PLAYER_STATE_GRIND_JUMP = 40,
	PLAYER_STATE_GRIND_SWITCH_JUMP = 41,
	PLAYER_STATE_GRIND_ATTACK = 42,
	PLAYER_STATE_SWING = 43,
	PLAYER_STATE_SWING_FALL = 44,
	PLAYER_STATE_RECOIL = 45,
	PLAYER_STATE_ICE_WALK = 46,
	PLAYER_STATE_DEVASTATOR = 47,
	PLAYER_STATE_SLIDE = 48,
	PLAYER_STATE_VEHICLE = 49,
	PLAYER_STATE_SWIMUNDER = 50,
	PLAYER_STATE_IDLEUNDER = 51,
	PLAYER_STATE_CHARGEUNDER = 52,
	PLAYER_STATE_SWIMSURF = 53,
	PLAYER_STATE_IDLESURF = 54,
	PLAYER_STATE_BOLT_CRANK = 55,
	PLAYER_STATE_LAVA_JUMP = 56,
	PLAYER_STATE_DEATH = 57,
	PLAYER_STATE_BOARD = 58,
	PLAYER_STATE_MAGNE_WALK = 59,
	PLAYER_STATE_GRIND_HIT = 62,
	PLAYER_STATE_GRIND_JUMP_TURN = 63,
	PLAYER_STATE_VENDOR_BOOTH = 98,
	PLAYER_STATE_NPC = 99,
	PLAYER_STATE_WALK_TO_POS = 100,
	PLAYER_STATE_SKID_TO_POS = 101,
	PLAYER_STATE_IDLE_TO_POS = 102,
	PLAYER_STATE_JUMP_TO_POS = 103,
	PLAYER_STATE_QUICKSAND_SINK = 104,
	PLAYER_STATE_QUICKSAND_JUMP = 105,
	PLAYER_STATE_DROWN = 106,
	PLAYER_STATE_MAGNE_ATTACK = 111,
	PLAYER_STATE_MAGNE_JUMP = 112,
	PLAYER_STATE_CUT_SCENE = 113,
	PLAYER_STATE_WADE = 114,
	PLAYER_STATE_ZIP = 115,
	PLAYER_STATE_GET_HIT_SURF = 116,
	PLAYER_STATE_GET_HIT_UNDER = 117,
	PLAYER_STATE_DEATH_FALL = 118,
	PLAYER_STATE_SLOPESLIDE = 120,
	PLAYER_STATE_JUMP_BOUNCE = 121,
	PLAYER_STATE_DEATHSAND_SINK = 122,
	PLAYER_STATE_LAVA_DEATH = 123,
	PLAYER_STATE_CHARGE = 125,
	PLAYER_STATE_ICEWATER_FREEZE = 126,
	PLAYER_STATE_ELECTRIC_DEATH = 127,
	PLAYER_STATE_ROCKET_HOVER = 128,
	PLAYER_STATE_ELECTRIC_DEATH_UNDER = 129,
	PLAYER_STATE_SKATE = 130,
	PLAYER_STATE_MOON_JUMP = 131,
	PLAYER_STATE_JET = 132,
	PLAYER_STATE_THROW_SHURIKEN = 133,
	PLAYER_STATE_RACEBIKE = 134,
	PLAYER_STATE_SPEEDBOAT = 135,
	PLAYER_STATE_HOVERPLANE = 136,
	PLAYER_STATE_LATCH_GRAB = 137,
	PLAYER_STATE_LATCH_IDLE = 138,
	PLAYER_STATE_LATCH_JUMP = 139,
	PLAYER_STATE_PULLSHOT_ATTACH = 140,
	PLAYER_STATE_PULLSHOT_PULL = 141,
	PLAYER_STATE_GET_FLATTENED = 142,
	PLAYER_STATE_SKYDIVE = 143,
	PLAYER_STATE_ELECTRIC_GET_HIT = 144,
	PLAYER_STATE_FLAIL_ATTACK = 145,
	PLAYER_STATE_MAGIC_TELEPORT = 146,
	PLAYER_STATE_TELEPORT_IN = 147,
	PLAYER_STATE_DEATH_NO_FALL = 148,
	PLAYER_STATE_TAUNT_SQUAT = 149,
	PLAYER_STATE_TAUNT_ASSPOINT = 150,
	PLAYER_STATE_TAUNT_ASSRUB = 151,
	PLAYER_STATE_TURRET_DRIVER = 152,
	PLAYER_STATE_WAIT_FOR_RESURRECT = 153,
	PLAYER_STATE_WAIT_FOR_JOIN = 154,
	PLAYER_STATE_DROPPED = 155,
	PLAYER_STATE_CNT = 156
};

/*
 * NAME :		CameraAngle
 * 
 * DESCRIPTION :
 * 			Contains camera angle settings and information.
 * 
 * NOTES :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct CameraAngle
{
    float Value;
    float Velocity;
    float MaxVelocity;
    float UNK0;
    float Acceleration;
    float UNK1[3];
} CameraAngle;

struct tNW_PlayerWarpMessage {
	/*   0 */ char netPlayerIndex;
	/*   1 */ char dontKillMeInBaseHack;
	/*   2 */ char resetPadRing;
	/*   3 */ char playerSeq;
	/*   4 */ float playerPos[3];
	/*  10 */ float playerRot[3];
	/*  1c */ short int state;
	/*  1e */ char isResurrecting;
};

struct tNW_PlayerData {
	/*   0 */ VECTOR vPosition;
	/*  10 */ float vRotation[3];
	/*  1c */ int timeStamp;
	/*  20 */ VECTOR vIdealVec;
	/*  30 */ int idealVecTime;
	/*  34 */ int accountId;
	/*  38 */ unsigned int UID;
	/*  3c */ int playerType;
	/*  40 */ int playerTeam;
	/*  44 */ float hitPoints;
	/*  48 */ int handGadget;
	/*  4c */ int lastKeepAlive;
	/*  50 */ int remoteTarget;
	/*  54 */ int playerIndex;
	/*  58 */ int cameraElevationSettings[3];
	/*  64 */ int cameraAzimuthSettings[3];
	/*  70 */ int cameraRotationSettings[3];
	/*  7c */ int rank[6];
	/*  94 */ char cGadgetLevel[32];
	/*  b4 */ unsigned int updated;
	/*  b8 */ short unsigned int gadgetsWithAmmo;
	/*  ba */ short unsigned int fpsMode;
	/*  bc */ unsigned char flags;
};

struct tNW_PlayerPadInputMessage {
	/*   0 */ int cameraRot[4];
	/*  10 */ short unsigned int playerPos[3];
	/*  16 */ unsigned char sequenceNum;
	union
	{
		/*  17 */ unsigned char stateAndRotFlag : 1;
		/*  17 */ unsigned char playerIndex : 1;
		/*  17 */ unsigned char flags : 2;
		/*  17 */ unsigned char framesWithButtonDiffs : 4;
	};
	/*  18 */ unsigned char pad_data[128];
};

struct tNW_PlayerPadInputMessageListElem {
	/*   0 */ struct tNW_PlayerPadInputMessage msg;
	/*  98 */ struct tNW_PlayerPadInputMessageListElem* pNext;
	/*  9c */ struct tNW_PlayerPadInputMessageListElem* pPrev;
	/*  a0 */ char inUse;
};

/*
 *
 */
struct tNW_Player {
	/*   0 */ int netClientIndex;
	/*   4 */ struct tNW_PlayerData* pNetPlayerData;
	/*   8 */ short int bLocal;
	/*   a */ short int bSpawned;
	/*   c */ short int bGiveMeTheDasBoot;
	/*   e */ short int bCallbackCalled;
	/*  10 */ int latency;
	/*  14 */ unsigned int flags;
	/*  18 */ char accountName[32];
	/*  38 */ struct tNW_PlayerWarpMessage warpMessage;
	/*  58 */ struct tNW_PlayerPadInputMessageListElem padMessageElems[16];
	/* a98 */ char padMessageCurDecodePos;
	/* a99 */ char activePadFrame;
	/* a9c */ int lastActiveSeqNum;
	/* aa0 */ int numBufferedPadMessageElems;
	/* aa4 */ int receivedActivePadMsgFrame;
	/* aa8 */ char pullBack;
	/* aa9 */ signed char jitterThrottleFrames;
	/* aaa */ char numConseqSkips;
	/* aac */ struct tNW_PlayerPadInputMessageListElem* pActivePadMsg;
	/* ab0 */ struct tNW_PlayerPadInputMessageListElem* pPadMsgListTail;
	/* ab4 */ unsigned char padFrame[20];
	/* ac8 */ int homeBaseIndex;
	/* acc */ int homeNodeIndex;
};

typedef struct HeroTimers {
	/*   0 */ int state;
	/*   4 */ int stateType;
	/*   8 */ int subState;
	/*   c */ int animState;
	/*  10 */ int stickOn;
	/*  14 */ int stickOff;
	/*  18 */ short int noLedge;
	/*  1a */ short int allowQuickSelect;
	/*  1c */ int firing;
	/*  20 */ int moveModifierTimer;
	/*  24 */ int boltMultTimer;
	/*  28 */ int wallJumpOk;
	/*  2c */ short int postHitInvinc;
	/*  2e */ short int ignoreHeroColl;
	/*  30 */ short int collOff;
	/*  32 */ short int invisible;
	/*  34 */ short int slide;
	/*  36 */ short int bezerker;
	/*  38 */ short int noWallJump;
	/*  3a */ short int noJumps;
	/*  3c */ short int boxBreaking;
	/*  3e */ short int noMag;
	/*  40 */ short int noChargeJump;
	/*  42 */ short int resurrectWait;
	/*  44 */ int timeSinceStrafe;
	/*  48 */ short int noHackerSwitch;
	/*  4a */ short int noInput;
	/*  4c */ short int noJumpLookBack;
	/*  4e */ short int noShockAbort;
	/*  50 */ short int stuck;
	/*  52 */ short int noSwing;
	/*  54 */ short int noWaterJump;
	/*  56 */ short int noWaterDive;
	/*  58 */ short int facialExpression;
	/*  5a */ short int idle;
	/*  5c */ short int bumpPushing;
	/*  5e */ short int lookButton;
	/*  60 */ short int edgeStop;
	/*  62 */ short int clankRedEye;
	/*  64 */ short int edgePath;
	/*  66 */ short int magSlope;
	/*  68 */ short int ledgeCamAdj;
	/*  6a */ short int screenFlashRed;
	/*  6c */ short int holdDeathPose;
	/*  6e */ short int strafeMove;
	/*  70 */ short int noRaisedGunArm;
	/*  72 */ short int noExternalRot;
	/*  74 */ short int screenFlashOn;
	/*  76 */ short int screenFadeOn;
	/*  78 */ int lastVehicleTimer;
	/*  7c */ float gadgetRefire;
	/*  80 */ int timeAlive;
	/*  84 */ int noFpsCamTimer;
	/*  88 */ int endDeathEarly;
	/*  8c */ short int forceGlide;
	/*  8e */ short int noGrind;
	/*  90 */ short int instaGrind;
	/*  92 */ short int noCamInputTimer;
	/*  94 */ short int postTeleportTimer;
	/*  96 */ short int multiKillTimer;
	/*  98 */ short int armorLevelTimer;
	/*  9a */ short int damageMuliplierTimer;
	/*  9c */ int powerupEffectTimer;
	/*  a0 */ short int juggernautFadeTimer;
	/*  a2 */ short int onFireTimer;
	/*  a4 */ short int acidTimer;
	/*  a6 */ short int freezeTimer;
	/*  a8 */ short int noHelmTimer;
	/*  aa */ short int elecTimer;
	/*  ac */ short int boltDistMulTimer;
	/*  ae */ short int explodeTimer;
	/*  b0 */ short int noDeathTimer;
	/*  b2 */ short int invincibilityTimer;
} HeroTimers;

typedef struct HeroGround {
	/*   0 */ VECTOR normal;
	/*  10 */ VECTOR waterNormal;
	/*  20 */ VECTOR gravity;
	/*  30 */ VECTOR point;
	/*  40 */ VECTOR lastGoodPos;
	/*  50 */ VECTOR externalBootGrav;
	/*  60 */ float feetHeights[1];
	/*  68 */ float pitchSlopes[1];
	/*  70 */ float rollSlopes[1];
	/*  78 */ float height;
	/*  7c */ float dist;
	/*  80 */ float slope;
	/*  84 */ float pitchSlope;
	/*  88 */ float rollSlope;
	/*  8c */ float angz;
	/*  90 */ float waterHeight;
	/*  94 */ float quicksandHeight;
	/*  98 */ int underWater;
	/*  9c */ Moby* pMoby;
	/*  a0 */ int onGood;
	/*  a4 */ float speed;
	/*  a8 */ short int magnetic;
	/*  aa */ short int stickLanding;
	/*  ac */ short int offAny;
	/*  ae */ short int offGood;
	/*  b0 */ int oscillating;
	/*  b4 */ float oscPos1;
	/*  b8 */ float oscPos2;
	/*  bc */ int pad[1];
} HeroGround;

typedef struct HeroPlayerConstants {
	/*   0 */ int mobyNum;
	/*   4 */ float maxWalkSpeed;
	/*   8 */ float kneeHeight;
	/*   c */ float kneeCheckDist;
	/*  10 */ float colRadius;
	/*  14 */ float colTop;
	/*  18 */ float colBot;
	/*  1c */ float colBotFall;
	/*  20 */ int jumpPushOffTime;
	/*  24 */ float jumpPeakFrm;
	/*  28 */ float jumpLandFrm;
	/*  2c */ float jumpGameLandFrm;
	/*  30 */ float jumpMaxHeight;
	/*  34 */ float jumpMinHeight;
	/*  38 */ int jumpMaxUpTime;
	/*  3c */ float jumpGravity;
	/*  40 */ float jumpMaxXySpeed;
	/*  44 */ float fallGravity;
	/*  48 */ float maxFallSpeed;
	/*  4c */ float walkAnimSpeedMul;
	/*  50 */ float walkAnimSpeedLimLower;
	/*  54 */ float walkAnimSpeedLimUpper;
	/*  58 */ float jogAnimSpeedMul;
	/*  5c */ float jogAnimSpeedLimLower;
	/*  60 */ float jogAnimSpeedLimUpper;
	/*  64 */ int pad[3];
} HeroPlayerConstants;

typedef struct HeroItem {
	/*  00 */ Moby * Moby1;
	/*  04 */ Moby * Moby2;
	/*  08 */ int IsActive; // Only changes if shooting
	/*  0c */ char unk0[0xc];
	/*  18 */ int ChangedCount;
	/*  1c */ char unk_4b8[0x4];
	/*  20 */ int TimeSinceChange;
	/*  24 */ int State;
	/*  28 */ int Index;
	/*  2c */ char unk1[0x4];
} HeroItem;


/*
 * NAME :		Player
 * 
 * DESCRIPTION :
 * 			Contains the player data.
 * 
 * NOTES :
 *          This is very large and documentation is incomplete.
 *          Much of the padding is to skip over area's not yet understood.
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct Player
{
    struct Guber Guber;                                             // 0x00
    char unk_18[0x88];
    VECTOR PlayerPosition;                                          // 0xA0
	union {															// 0xB0
		VECTOR PlayerRotation;
		struct {
			float PlayerRoll;
			float PlayerPitch;
			float PlayerYaw;
		};
	};
    char unk_c0[0x60];
    VECTOR Velocity;                                         		// 0x120
    char unk_130[0x1b0];											// 0x130
	int OnGround;													// 0x2E0
	char unk_2e4[0xc];
	int StateTimer;													// 0x2F0
	int StateTypeTimer;												// 0x2F4
	int SubStateTimer;												// 0x2F8
	int animStateTimer;												// 0x2FC
	int stickOnTimer;												// 0x300
	int stickOffTimer;												// 0x304
	short int unk_308;
	short int unk_30a;
	int firingTimer;												// 0x30C
	char unk_310; // Freezes if I change it.
	char unk_311; // Freezes if I change it.
	char unk_312; // Freezes if I change it.
	char unk_313; // Freezes if I change it.
	char unk_314[0x8];
	char RespawnTimer;												// 0x31C
	char NotUsed;													// 0x31D
	short int unkTimer_31e;											// 0x31E
    short int CantMoveTimer;										// 0x320
	short int unkTimer_322; // Not Used
	short int unkTimer_324;
	short int unkTimer_326;
    float gadgetRefireTimer;	                                    // 0x328
	short int unkTimer_32c;
	short int unkTimer_32e;
	short int UnkTimer_330;
	short int unkTimer_332; // Not Used
	short int unk_WrenchTimer;										// 0x334
	short int unkTimer_336;											// 0x336
	short int NoLedgeTimer;											// 0x338
	short int unkTimer_33a;	// Not Used								// 0x33A
	short int unktimer_33c; // Not Used								// 0x33C
	short int MagneticTimer;										// 0x33E
	short int unkTimer_340;
	short int unkTimer_342;
	short int noDeathTimerTimer;									// 0x344
	short int unkTimer_346; // Not Used
	short int unkTimer_348;
	short int unktimer_34a;
	short int unktimer_34c; // Not Used
	short int unktimer_34e; // Not Used
	int timeAliveTimer;												// 0x350
	int unk_354;
	int unk_358;
	int IsChargebooting;											// 0x35C
	int unkTimer_360;
	char unk_364[0xc];
	short int magnetic; // 0 = No, 2 = Yes.							// 0x370
	char unk_372[0xae];
	Moby * StandingOnMoby;											// 0x420
	Moby * SkinMoby2;												// 0x424
	char unk_428[0x28];
	HeroItem Weapon;												// 0x450
	char unk_480[0x20];
	HeroItem Boots;													// 0x4A0
	char unk_4d0[0x20];
	char unk_4f0[0xc90];
    VECTOR CameraPos;                                               // 0x1180
	VECTOR CameraDir;												// 0x1190
	MATRIX CameraMatrix;											// 0x11A0
    CameraAngle CameraYaw;                                          // 0x11E0
    CameraAngle CameraPitch;                                        // 0x1200
    char unk_1220[0xc];
    Moby * LookAtMoby;                                              // 0x122C
    char unk_1230[0x24];
    float CameraPitchMin;                                           // 0x1254
    float CameraPitchMax;                                           // 0x1258
    Moby * SkinMoby;                                                // 0x125C
    VECTOR CameraOffset;                                           	// 0x1260
    char unk_1270[0x10];
	short CameraType2;												// 0x1280
	char unk_1282[0x2];
	// jr ra'ing function messes with camera on metalic surfaces
	int func_1284;													// 0x1284
	void * CameraPtr1;												// 0x1288
	void * CameraPtr2;												// 0x128C
	char unk_1290[0x20];
	VECTOR WeaponShotPosition;										// 0x12B0
	float WeaponLockonDistance;										// 0x12C0
    char unk_12c4[0x8];
	int LocalPlayerIndex;                                           // 0x12CC
	int TopOfPlayerStruct;											// 0x12D0
    char unk_12d4[0x4];
    int PlayerId;                                                   // 0x12D8
	char unk_12dc[0x34];
    // This int is checked when chargebooting, and shooting
	int unk_1310;													// 0x1310
	char unk_1314[0x5b0];
	Moby * WrenchThrown;											// 0x18C4
	char unk_18c8[0x8];
	int WeaponHeldId;												// 0x18D0
	char unk_18d4[0x110];
	char State;														// 0x19E4
	char unk_19e5[0x22];
	union {
        struct {
			char GadgetActive;										// 0x1A07
            char RaisedGunArm;										// 0x1A08
            char InShallowWater;									// 0x1A09
            char Invisible;											// 0x1A0A
            char HideWeapon;										// 0x1A0B
            char GadgetsOff;										// 0x1A0C
            char GadgetNotReady;									// 0x1A0D
            char WrenchOnly;										// 0x1A0E
            char HideWrench;										// 0x1A0F
            char SpawnBoltsToMe;									// 0x1A10
            char AiFollowingMe;										// 0x1A11
            char ForceWrenchSwitch;									// 0x1A12
            char ForceSwingSwitch;									// 0x1A13
            char IsLocal;											// 0x1A14
            char InBaseHack;										// 0x1A15
            char ShieldTrigger;										// 0x1A16
            char CurSeg;											// 0x1A17
            char HandGadgetType;									// 0x1A18
            char ExternalUpdate;									// 0x1A19
            char unk_1a1a[0x14];
		};
    };
	char unk_1a2e[0x4];
	WeaponQuickSelect QuickSelect;									// 0x1A32 to 0x1A34
	char unk_1a35[0x1e];
	WeaponAmmo WeaponAmmo;											// 0x1A53 to 0x1A5E
	char unk_1a5f[0x4];
	WeaponMeter WeaponMeter;										// 0x1A63 to 0x1A6E
	char unk_1a6f[0x11];
	int GadgetBox;													// 0x1A80
	char unk_1a84[0x324];
	int TopOfPlayerStruct2;											// 0x1DA8
	char unk_1dac[0x6ca];
	char Health;													// 0x2476
	char unk_2477[0x5d];
	char DamageTaken;												// 0x24D4
	char unk_24d5[0x28];
	char IsSquating;												// 0x24FD
	char unk_24fe[0x2];
	char Invisible2;												// 0x2500
	char DontSpawnWeaponPack;										// 0x2501
	char unk_2502[0x12];
	int TimerShowHealth;											// 0x2514
	char TimerFadeToBlack;                                          // 0x2518
	// When Start is pressed, this counts down from 0xE, then shows menu when it equals 0
    char TimerStartMenu;                                            // 0x2519
	char unk_251a[0x2];
	Moby * HeldMoby;												// 0x251C
	HeroPlayerConstants * PlayerConstants;							// 0x2520
	Moby * PlayerMoby;												// 0x2524
    Vehicle * Vehicle;                                              // 0x2528
	char unk_252c[0x4];
	GameCamera * Camera;											// 0x2530
	PadButtonStatus * Paddata;                                      // 0x2534
	float PlayerPositionX;											// 0x2538
	float PlayerPositionZ;											// 0x253C
	float PlayerPositionY;											// 0x2540
	int MpIndex;													// 0x2444
	int Team;														// 0x2548
	int InVehicle;													// 0x254C
	// timer gets reset if entering or exiting a vehicle, will always count up.
	int InOutVehicleTimer;											// 0x2550
	char unk_2554[0x4];
	struct tNW_Player * pNetPlayer;									// 0x2558

	char unk_2559[0x26];
	// This area changes if wrench is held.
	char unk_2580[0x78f];
	// All zeros (padding maybe?)
	char unk_2d10[0xd7];
	// Changes if wrench is out and jumping:
	char unk_2de8[0xdb];
	// this last char goes to end of struct.
	char unk_2ec4[0x163b];

	// FULL STRUCT SIZE: 0x44FF
	// Next Player Struct (if Local): 0x4500
} Player;

typedef void (*PlayerUpdate_func)(Player * player);
typedef void (*PlayerUpdateState_func)(Player * player, int stateId, int a2, int a3, int t0);

typedef struct PlayerVTable
{
    void * FUNC_00;
    void * FUNC_04;
    void * FUNC_08;
    PlayerUpdate_func Update;
    void * FUNC_10;
    void * FUNC_14;
    void * FUNC_18;
    void * FUNC_1C;
    void * FUNC_20;
    void * FUNC_24;
    void * FUNC_28;
    void * FUNC_2C;
    void * FUNC_30;
    PlayerUpdateState_func UpdateState;
    void * FUNC_38;
	void * FUNC_3C;
} PlayerVTable;

/*
 * NAME :		playerGetPad
 * DESCRIPTION :
 * 			Returns a pointer to the player's pad data.
 * NOTES :
 * ARGS : 
 *          player:                     Target player.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ PadButtonStatus * playerGetPad(Player * player);

/*
 * NAME :		playerPadGetButton
 * DESCRIPTION :
 * 			Returns 1 when the given player is pressing the given button combination.
 *          Returns negative on failure.
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          buttonMask:                 Buttons to check.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int playerPadGetButton(Player * player, u16 buttonMask);

/*
 * NAME :		playerPadGetButtonDown
 * DESCRIPTION :
 * 			Returns 1 during the frame that the given player starts pressing the given button combination.
 *          Returns negative on failure.
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          buttonMask:                 Buttons to check.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int playerPadGetButtonDown(Player * player, u16 buttonMask);

/*
 * NAME :		playerPadGetButtonUp
 * 
 * DESCRIPTION :
 * 			Returns 1 during the frame that the given player releases the given button combination.
 *          Returns negative on failure.
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          buttonMask:                 Buttons to check.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int playerPadGetButtonUp(Player * player, u16 buttonMask);

/*
 * NAME :		playerSetPlayerState
 * DESCRIPTION :
 * 			Set Player to a certain state.
 * 
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          state:                 		State to set player.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerSetPlayerState(Player * player, u8 state);

/*
 * NAME :		playerDecHealth
 * DESCRIPTION :
 * 			Decrease a players health by a certain amount (1 to 15)
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          health:                 		Amount to hurt player (1 to 15)
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerDecHealth(Player * player, int health);

/*
 * NAME :		playerIncHealth
 * DESCRIPTION :
 * 			Increase a players health by a certain amount (1 to 15)
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          health:                 		Amount to give to player (1 to 15)
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerIncHealth(Player * player, int health);

/*
 * NAME :		playerSetHealth
 * DESCRIPTION :
 * 			Set a players health to a certain amount (1 to 15)
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          health:                 		Value to set health to (1 to 15)
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerSetHealth(Player * player, int health);

/*
 * NAME :		playerGetHealth
 * DESCRIPTION :
 * 				Get a players health
 * NOTES :
 * ARGS : 
 *         		player:         Pointer to player's player object.
 * RETURN :
 * 				Returns players health in float value
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
float playerGetHealth(Player * player);

/*
 * NAME :		playerRespawn
 * 
 * DESCRIPTION :
 * 			Respawns the given player.
 * 
 * NOTES :
 *          Spawn point is determined internally.
 *          This will freeze on CQ if the player hasn't picked a node to respawn to.
 * 
 * ARGS : 
 *      player      :           Target player.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void playerRespawn(Player * player);

/*
 * NAME :		playerSetPosRot
 * 
 * DESCRIPTION :
 * 			Sets a given player's position and rotation.
 * 
 * NOTES :
 * 
 * ARGS : 
 *      player      :           Target player.
 *      position    :           Position.
 *      rotation    :           Rotation.
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
__LIBUYA_SETTER__ void playerSetPosRot(Player * player, u32 * position, u32 * rotation);

/*
 * NAME :		playerIsDead
 * DESCRIPTION :
 * 			Returns non-zero if the given player is dead.
 * NOTES :
 * ARGS : 
 *      player      :           Target player.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int playerIsDead(Player * player);

/*
 * NAME :		playerGiveWeapon
 * DESCRIPTION :
 * 			Give the target player the given weapon ID.
 * NOTES :
 * ARGS : 
 *      player      :           Target player.
 * 		weaponId	:			Weapon
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerGiveWeapon(Player * player, int weaponId);

/*
 * NAME :		playerEquipWeapon
 * DESCRIPTION :
 * 			Equips the given weapon ID.
 * NOTES :
 * ARGS : 
 *      player      :           Target player.
 * 		weaponId	:			Weapon to be equiped
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerEquipWeapon(Player * player, int weaponId);

/*
 * NAME :		playerStripWeapons
 * DESCRIPTION :
 * 			Removes all weapons from player.
 * NOTES :
 * ARGS : 
 *      player      :           Target player.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerStripWeapons(Player * player);

/*
 * NAME :		playerGiveRandomWeapons
 * DESCRIPTION :
 * 			Gives Target player random weapons
 * NOTES :
 * ARGS : 
 *      player      :           Target player.
 * 		amount		:			Amount of weapons to give.  If left empty, will give 3 (default)
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerGiveRandomWeapons(Player * player, int amount);

/*
 * NAME :		playerDeobfuscate
 * DESCRIPTION :
 * 			Gives Target player random weapons
 * NOTES :
 * ARGS : 
 *      src      :           Source pointer from player struct of the data needed to deorbuscate.
 * 							 Ex: &player->Health, &player->State
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
u32 playerDeobfuscate(u32 src, int addr, int method);

#endif // _LIBUYA_PLAYER_H_
