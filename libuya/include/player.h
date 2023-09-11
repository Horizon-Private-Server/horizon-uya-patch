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
typedef struct CameraAngleZ
{
    float rotation;												// 0x11E0
    float speed_current;										// 0x11E4
    float speed_max_quick;										// 0x11E8
    float speed_max_aim;										// 0x11EC
    float accel_quick;											// 0x11F0
    float accel_aim;											// 0x11F4
	float target_slowness_factor_quick;							// 0x11F8
	float target_slowness_factor_aim;							// 0x11FC
} CameraAngleZ;

typedef struct CameraAngleY
{
    float rotation;												// 0x1200
    float speed_current;										// 0x1204
    float speed_max;											// 0x1208
    float accel;												// 0x120C
    float target_slowness_factor;								// 0x1210
    float strafe_turn_factor;									// 0x1214
	float strafe_tilt_factor;									// 0x1218
	float max_target_angle;										// 0x121C
} CameraAngleY;

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

typedef struct DL_HeroTimers {
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
} DL_HeroTimers;

typedef struct HeroTimers {
	int state;														// 0x2F0
	int stateType;													// 0x2F4
	int subState;													// 0x2F8
	int animState;													// 0x2FC
	int stickOn;													// 0x300
	int stickOff;													// 0x304
	short int unk_308;
	short int unk_30a;
	int firing;														// 0x30C
	char unk_310; // Freezes if I change it.
	char unk_311; // Freezes if I change it.
	char unk_312; // Freezes if I change it.
	char postHitInvinc; // Freezes if I change it.					// 0x313
	char unk_314[0x8];
	char resurrectWait;												// 0x31C
	char NotUsed;													// 0x31D
	short int unkTimer_31e;											// 0x31E
    short int noInput;												// 0x320
	short int unkTimer_322; // Not Used
	short int unkTimer_324;
	short int unkTimer_326;
    float gadgetRefire;			                                    // 0x328
	short int unkTimer_32c;
	short int unkTimer_32e;
	short int UnkTimer_330;
	short int unkTimer_332; // Not Used
	short int unk_Wrench;											// 0x334
	short int unkTimer_336;											// 0x336
	short int noLedge;												// 0x338
	short int unkTimer_33a;	// Not Used								// 0x33A
	short int unktimer_33c; // Not Used								// 0x33C
	short int magnetic;												// 0x33E
	short int unkTimer_340;
	short int unkTimer_342;
	short int noDeathTimer;											// 0x344
	short int unkTimer_346; // Not Used
	short int unkTimer_348;
	short int unktimer_34a;
	short int unktimer_34c; // Not Used
	short int unktimer_34e; // Not Used
	int timeAlive;													// 0x350
	int unk_354;
	int unk_358;
	int IsChargebooting;											// 0x35C
	int unkTimer_360;
	char unk_364[0xc];
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

typedef struct Gadget {
	/*   0 */ VECTOR jointPos;
	/*  10 */ VECTOR jointRot;
	/*  20 */ Moby* pMoby;
	/*  24 */ Moby* pMoby2;
	/*  28 */ int padButtonDown;
	/*  2c */ int alignPad;
	/*  30 */ int padButton;
	/*  34 */ int gsSpawnFrame;
	/*  38 */ char noAmmoTime;
	/*  39 */ char unEquipTimer;
	/*  3a */ char detached;
	/*  3b */ char unequipTime;
	/*  3c */ char unEquipStatus;
	/*  3d */ char unEquipDelay;
	/*  40 */ int equippedTime;
	/*  44 */ int state;
	/*  48 */ int id;
	/*  4c */ float lightAng;
} Gadget;

typedef struct FpsCam {
	MATRIX CameraMatrix;											// 0x11A0
    CameraAngleZ CameraYaw;                                         // 0x11E0
    CameraAngleY CameraPitch;                                       // 0x1200
    int state;														// 0x1220
	short int quick_turn_input_time;								// 0x1224
	short int snap_hold_time;										// 0x1226
	float snap_input;												// 0x1228
    Moby *Target;                                      	      	    // 0x122C
	VECTOR target_last_pos;											// 0x1230
	VECTOR target_vel;												// 0x1240
	float target_blend_fac;											// 0x1250
    float CameraPitchMin; // aka: float max_y_rot                   // 0x1254
    float CameraPitchMax; // aka: float min_y_rot                   // 0x1258
    Moby * pExcludeMoby;                                            // 0x125C
    VECTOR CameraPositionOffset;                                  	// 0x1260
    VECTOR CameraRotationOffset;									// 0x1270
	short int flags;												// 0x1280
	short int allegiance;											// 0x1282
	void *special_target_func;										// 0x1284
	MATRIX *pWorldMtx;												// 0x1288
	MATRIX *pWorldInvMtx;											// 0x128C
	VECTOR facing_dir;												// 0x1290
	VECTOR internal_facing_dir;										// 0x12A0
	VECTOR aim_pos;													// 0x12B0
	float range;													// 0x12C0
    float ext_extension;											// 0x12C4
	float ext_extension_speed;										// 0x12C8
	int cam_slot; // aka: LocalPlayerIndex    		   	            // 0x12CC
	void *pHero;													// 0x12D0
    float camRadius;												// 0x12D4
    int camSettingsIndex; // aka: PlayerId		                    // 0x12D8
	int karma_pad;													// 0x12DC
	VECTOR prevCamPos;												// 0x12E0
	int karma_pad2[4];												// 0x12F0
	float camYSpeed;												// 0x1300
	float camZSpeed;												// 0x1304
	float gunInterp;												// 0x1308
	float gunInterpSpeed;											// 0x130C
} FpsCam;

typedef struct HeroCamera { // 0x30
	VECTOR CameraPos;												// 0x1180
	VECTOR CameraDir;												// 0x1190
	struct FpsCam Vars; 											// 0x11A0 - 0x130C
	int active;														// 0x1310
	float gunWithdrawSpeed;											// 0x1314
	float gunWithdrawDist;											// 0x1318
	float gunWithdrawIdeal;											// 0x131C
	float bobRot;													// 0x1320
	float bobRotSpeed;												// 0x1324
	float bobAmp;													// 0x1328
	float camHeroOfs;												// 0x132C
	float camHeroOfsSpeed;											// 0x1330
	int ignoreGroundHeight;											// 0x1334
	float reticlePulseAng;											// 0x1338
	int reticleFadeInTimer;											// 0x133C
} HeroCamera;

typedef struct HeroWalkToPos {
	VECTOR idealPos;												// 0x13B0
	VECTOR idealRot;												// 0x13C0
	int abortOnArrival;												// 0x13D0
	int walkToTeleport;												// 0x13D4
	int teleportWaitTime;											// 0x13D8
	Moby *pTeleTarget;												// 0x13DC
} HeroWalkToPos;

typedef struct MotionBlur { // 0x150
	/* 0x000 */ VECTOR posRing[8];
	/* 0x080 */ VECTOR rotRing[8];
	/* 0x100 */ int blurAlphas[4];
	/* 0x110 */ int blurSteps[4];
	/* 0x120 */ Moby *blurMobys[4];
	/* 0x130 */ float gapReduction[4];
	/* 0x140 */ short int ringIndex;
	/* 0x142 */ short int ringValidSize;
	/* 0x144 */ Moby *pTrackedMoby;
	/* 0x148 */ int blurCnt;
	/* 0x14c */ int active;
} MotionBlur;

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
	MATRIX Water;													// 0x20
	MATRIX Terrain;													// 0x60
	VECTOR PlayerPosition;                                          // 0xA0
	union {															// 0xB0
		VECTOR PlayerRotation;
		struct {
			float PlayerRoll;										// 0xB0
			float PlayerPitch;										// 0xB4
			float PlayerYaw;										// 0xB8
			float unk_cc;
		};
	};
	char unk_c0[0x20];
	VECTOR unk_e0;
	char unk_f0[0x30];
    VECTOR Velocity;                                         		// 0x120
    char unk_130[0x1b0];											// 0x130
	int OnGround;													// 0x2E0
	char unk_2e4[0xc];
	HeroTimers timers;												// 0x2F0
	short int magnetic; // 0 = No, 2 = Yes.							// 0x370
	char unk_372[0xae];
	Moby *StandingOnMoby;											// 0x420
	Moby *SkinMoby2;												// 0x424
	char unk_428[0x4];
	Gadget Weapon;													// 0x430
	Gadget Boots;													// 0x480
	char unk_4d0[0x20];
	char unk_4f0[0xc90];
    HeroCamera fps;													// 0x1180 - 0x133C
	char unk_1340[0x70];
	HeroWalkToPos walkToPos;										// 0x13B0 - 0x13DC
	char unk_13e0[0x3a0];
	MotionBlur wrenchMotionBlur;									// 0x1780 - 0x18D0
	int WeaponHeldId;												// 0x18D0
	char unk_18d4[0x110];
	char State;														// 0x19E4
	char SubState;													// 0x19E5
	char StateType;													// 0x19E6
	char PreviousState;												// 0x19E7
	char PreviousType;												// 0x19E8
	char PrePreviousState;											// 0x19E9
	char PrePreviousType;											// 0x19EA
	char StateHistory[8];											// 0x19EB
	char StateTypeHistory[8];										// 0x19F3
	char unk_19fb[0xc];
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
	char unk_1dac[0x688];
	Moby * SkinMoby3;												// 0x2434
	char unk_2438[0x10];
	int firingAnim;													// 0x2448
	int firingGadget;												// 0x244C
	int desiredCam;													// 0x2450
	char unk_2454[0x14];
	float stickStrength;											// 0x2468
	float stickRawAngle;											// 0x246C
	char unk_2470[0x6];
	char Health;													// 0x2476
	char unk_2477[0xd];
	float SkidDeceleration;											// 0x2484
	int mtxFxActive;												// 0x2488
	float analogStickStrength;										// 0x248C
	char unk_2490[0x14];
	short int unkGadgetTimer_24a4;									// 0x24A4
	short int GadgetPADDelayTimer;									// 0x24A6
	short int unkGadgetTimer_24a8;									// 0x24A8
	char unk_24aa[0x22];
	u32 LastDamagedMeOwnerUID;										// 0x24CC
	int LastDamagedMeGadgetId;										// 0x24D0
	char DamageTaken;												// 0x24D4
	char unk_24d5[0x28];
	char LookAndCrouch;												// 0x24FD
	char LookAndThrowWrench;										// 0x24FE
	char unk_24ff;
	char Invisible2;												// 0x2500
	// lastDeathWasSuicide: Doesn't Spawn Pack if true
	char LastDeathWasSuicide;										// 0x2501
	char unk_2502[0x12];
	int HudHealthTimer;												// 0x2514
	char PauseOn;                                 		 	        // 0x2518
    char PauseTimer;   		                                        // 0x2519
	char unk_251a;
	char PlayerType;												// 0x251B
	Moby *FlagMoby;													// 0x251C
	HeroPlayerConstants *PlayerConstants;							// 0x2520
	Moby *PlayerMoby;												// 0x2524
    Vehicle *Vehicle;                                               // 0x2528
	Vehicle *VehiclePending;										// 0x252C
	GameCamera *Camera;												// 0x2530
	PadButtonStatus *Paddata;                                       // 0x2534
	float PlayerPositionX;											// 0x2538
	float PlayerPositionZ;											// 0x253C
	float PlayerPositionY;											// 0x2540
	int mpIndex;													// 0x2444
	int mpTeam;														// 0x2548
	int vehicleState;												// 0x254C
	int vehicleStateTimer;											// 0x2550
	int pointsLastKill;												// 0x2554
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
 * 			Deobfuscates given player struct item.
 * NOTES :
 * ARGS : 
 *      src     :           Source pointer from player struct of the data needed to deorbuscate.
 * 							Ex: &player->Health, &player->State
 * 		addr	:			0: vaPlayerObfuscateAddr
 * 							1: vaPlayerObfuscateWeaponAddr
 * 		mode	:			0: Used for: Health, Player State, other.
 * 							1: Used for: Weapon IDs, other.
 * 							2: Used for: Respawn Timer, other.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
u32 playerDeobfuscate(u32 src, int addr, int mode);

#endif // _LIBUYA_PLAYER_H_
