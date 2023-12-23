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
typedef enum PlayerCameraType
{
    CAMERA_TYPE_THIRD_PERSON,
    CAMERA_TYPE_LOCK_STRAFE,
    CAMERA_TYPE_FIRST_PERSON
} PlayerCameraType;

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
typedef enum PlayerState {
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
	PLAYER_STATE_WAIT_FOR_RESURRECT = 144,
	PLAYER_STATE_FLAIL_ATTACK = 145,
	PLAYER_STATE_MAGIC_TELEPORT = 146,
	PLAYER_STATE_TELEPORT_IN = 147,
	PLAYER_STATE_DEATH_NO_FALL = 148,
	PLAYER_STATE_TURRET_DRIVER = 152,
	PLAYER_STATE_WAIT_FOR_JOIN = 154,
	PLAYER_STATE_DROPPED = 155,
	PLAYER_STATE_CNT = 156
} PlayerState;

typedef enum PlayerType {
	PLAYER_TYPE_IDLE = 0,
	PLAYER_TYPE_WALK = 1,
	PLAYER_TYPE_FALL = 2,
	PLAYER_TYPE_LEDGE = 3,
	PLAYER_TYPE_JUMP = 4,
	PLAYER_TYPE_GLIDE = 5,
	PLAYER_TYPE_ATTACK = 6,
	PLAYER_TYPE_GET_HIT = 7,
	PLAYER_TYPE_SHOOT = 8,
	PLAYER_TYPE_BUSY = 9,
	PLAYER_TYPE_BOUNCE = 10,
	PLAYER_TYPE_STOMP = 11,
	PLAYER_TYPE_CROUCH = 12,
	PLAYER_TYPE_GRAPPLE = 13,
	PLAYER_TYPE_SWING = 14,
	PLAYER_TYPE_GRIND = 15,
	PLAYER_TYPE_SLIDE = 16,
	PLAYER_TYPE_SWIM = 17,
	PLAYER_TYPE_SURF = 18,
	PLAYER_TYPE_HYDRO = 19,
	PLAYER_TYPE_DEATH = 20,
	PLAYER_TYPE_BOARD = 21,
	PLAYER_TYPE_RACEBOARD = 22,
	PLAYER_TYPE_SPIN = 23,
	PLAYER_TYPE_NPC = 24,
	PLAYER_TYPE_QUICKSAND = 25,
	PLAYER_TYPE_ZIP = 26,
	PLAYER_TYPE_HOLO = 27,
	PLAYER_TYPE_CHARGE = 28,
	PLAYER_TYPE_ROCKET_HOVER = 29,
	PLAYER_TYPE_JET = 30,
	PLAYER_TYPE_RACEBIKE = 31,
	PLAYER_TYPE_SPEEDBOAT = 32,
	PLAYER_TYPE_PULL = 33,
	PLAYER_TYPE_LATCH = 34,
	PLAYER_TYPE_LADDER = 36,
	PLAYER_TYPE_SKYDIVE = 37,
	PLAYER_TYPE_CNT = 38
} PlayerType;

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
	short int noLedge;												// 0x308
	short int allowQuickSelect;										// 0x30A
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
	short int unkTimer_338;											// 0x338
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

typedef struct HeroHotspots { // 0x10
	/* 0x0 */ short int index; // 02: magnetic, 0b: water
	/* 0x2 */ char ice;
	/* 0x3 */ char magictele;
	/* 0x4 */ char deathsand;
	/* 0x5 */ char lava;
	/* 0x6 */ char quicksand;
	/* 0x7 */ char magnetic;
	/* 0x8 */ char noStand;
	/* 0x9 */ char water;
	/* 0xa */ char icewater;
	/* 0xb */ char groundType;
	/* 0xc */ int pad;
} HeroHotspots;

typedef struct HeroLockOn { // 0x50
	/* 0x00 */ VECTOR strafeIdealVec;
	/* 0x10 */ Moby *pMoby;
	/* 0x14 */ short int strafing;
	/* 0x16 */ char strafingBack;
	/* 0x17 */ char strafingDir;
	/* 0x18 */ float strafeRot;
	/* 0x1c */ float strafeRotSpeed;
	/* 0x20 */ float backSpeed;
	/* 0x24 */ int strafeRotTimer;
	/* 0x28 */ short int strafeLeftDampTimer;
	/* 0x2a */ short int strafeRightDampTimer;
	/* 0x2c */ int strafeTurnDiffZero;
	/* 0x30 */ VECTOR curRetPos;
	/* 0x40 */ Moby *curRetMoby;
	/* 0x44 */ float fadeInterp;
	/* 0x48 */ float retRot;
	/* 0x4c */ float strafeTurnDiff;
} HeroLockOn;

typedef struct HeroMobys { // 0x10
	/* 0x0 */ Moby *ground;
	/* 0x4 */ Moby *hero;
	/* 0x8 */ int pad[1];
} HeroMobys;

typedef struct HeroFireDir { // 0x50
	/* 0x00 */ VECTOR m0;
	/* 0x10 */ VECTOR m1;
	/* 0x20 */ VECTOR m2;
	/* 0x30 */ VECTOR v;
	/* 0x40 */ VECTOR rot;
} HeroFireDir;

typedef struct HeroMove { // 0xa0
	/* 0x00 */ VECTOR behavior;
	/* 0x10 */ VECTOR external;
	/* 0x20 */ VECTOR actual;
	/* 0x30 */ VECTOR actualFromBehavior;
	/* 0x40 */ VECTOR actualFromBehaviorGrav;
	/* 0x50 */ VECTOR actualFromBehavior2D;
	/* 0x60 */ VECTOR actualFromExternal;
	/* 0x70 */ VECTOR taper;
	/* 0x80 */ float speed;
	/* 0x84 */ float speed2D;
	/* 0x88 */ float forwardSpeed;
	/* 0x8c */ float ascent;
	/* 0x90 */ float zSpeed;
	/* 0x94 */ float externalSpeed;
	/* 0x98 */ int pad[1];
} HeroMove;

typedef struct HeroColl { // 0x70
	/* 0x00 */ VECTOR normal;
	/* 0x10 */ VECTOR ip;
	/* 0x20 */ float top;
	/* 0x24 */ float bot;
	/* 0x28 */ float ideal_top;
	/* 0x2c */ float ideal_bot;
	/* 0x30 */ float idealRadius;
	/* 0x34 */ float radius;
	/* 0x38 */ float radiusSpeed;
	/* 0x3c */ Moby *pContactMoby;
	/* 0x40 */ Moby *pBumpMoby;
	/* 0x44 */ float bumpPushSpeed;
	/* 0x48 */ float distToWall;
	/* 0x4c */ float wallAng;
	/* 0x50 */ float wallSlope;
	/* 0x54 */ char wallIsCrate;
	/* 0x55 */ char wallIsMoby;
	/* 0x56 */ char contact;
	/* 0x57 */ char cpad;
	/* 0x58 */ float ledgeHeight;
	/* 0x5c */ float ledgeDist;
	/* 0x60 */ int atLedge;
	/* 0x64 */ Moby *pWallJumpMoby;
	/* 0x68 */ int pad[1];
} HeroColl;

typedef struct HeroGround {
	/*   0 */ VECTOR normal;
	/*  10 */ VECTOR waterNormal;
	/*  20 */ VECTOR gravity;
	/*  30 */ VECTOR point;
	/*  40 */ VECTOR lastGoodPos;
	/*  50 */ VECTOR externalBootGrav;
	/*  60 */ float feetHeights[2];
	/*  68 */ float pitchSlopes[2];
	/*  70 */ float rollSlopes[2];
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
	/*  bc */ int pad;
} HeroGround;

typedef struct HeroCommand { // 0xc
	/* 0x0 */ int state;
	/* 0x4 */ Moby *pCurTarget;
	/* 0x8 */ int timer;
} HeroCommand;

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
	/*  20 */ Moby *pMoby;
	/*  24 */ Moby *pMoby2;
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

typedef struct HeroAnim { // 0x20
	/* 0x00 */ float speed;
	/* 0x04 */ int iscale;
	/* 0x08 */ int flags;
	/* 0x0c */ int interping;
	/* 0x10 */ int env_index;
	/* 0x14 */ int env_time;
	/* 0x18 */ float mayaFrm;
	/* 0x1c */ float mayaFrmDelt;
} HeroAnim;

typedef struct HeroJoints { // 0x100
	/* 0x00 */ VECTOR gadgetMtxs0[4];
	/* 0x40 */ VECTOR gadgetMtxs1[4];
	/* 0x80 */ VECTOR gadgetMtxs2[4];
	/* 0xc0 */ VECTOR gadgetMtxs3[4];
} HeroJoints;

typedef struct HeroAnimLayers { // 0x20
	/* 0x00 */ struct MobyAnimLayer *pArmBlenders[2];
	/* 0x08 */ struct MobyAnimLayer *pFiringBlenders[2];
	/* 0x10 */ struct MobyAnimLayer *pHeadBlender;
	/* 0x14 */ int detachingFiring;
	/* 0x18 */ int armBlenderSharesIdle;
	/* 0x1c */ int gadgetAttachedId;
} HeroAnimLayers;

typedef struct HeroTweaker { // 0xb0
	/* 0x00 */ struct Manipulator manip;
	/* 0x40 */ VECTOR rot;
	/* 0x50 */ VECTOR speed;
	/* 0x60 */ VECTOR target;
	/* 0x70 */ VECTOR trans;
	/* 0x80 */ VECTOR transSpeed;
	/* 0x90 */ VECTOR transTarget;
	/* 0xa0 */ short int joint;
	/* 0xa2 */ short int whichMoby;
	/* 0xa4 */ float gain;
	/* 0xa8 */ float damp;
	/* 0xac */ float scale;
} HeroTweaker;

typedef struct HeroShadow { // 0x28
	/* 0x00 */ float slope;
	/* 0x04 */ float plane;
	/* 0x08 */ float range;
	/* 0x0c */ int sample_id;
	/* 0x10 */ int pad[1];
	/* 0x18 */ float sample_pos[4];
} HeroShadow;

typedef struct HeroAttack { // 0xb0
	/* 0x00 */ VECTOR near;
	/* 0x10 */ VECTOR far;
	/* 0x20 */ VECTOR oldNear;
	/* 0x30 */ VECTOR oldFar;
	/* 0x40 */ VECTOR wrenchHandle;
	/* 0x50 */ VECTOR wrenchTip;
	/* 0x60 */ VECTOR idealVec;
	/* 0x70 */ VECTOR bounceVec;
	/* 0x80 */ Moby *pTarget;
	/* 0x84 */ Moby *pMoby;
	/* 0x88 */ int rotSet;
	/* 0x8c */ float rot;
	/* 0x90 */ float bounceAng;
	/* 0x94 */ float speedFactor;
	/* 0x98 */ Moby *pGunPointMoby;
	/* 0x9c */ short int id;
	/* 0x9e */ short int soundPlayed;
	/* 0xa0 */ float descend;
	/* 0xa4 */ float aimAngz;
	/* 0xa8 */ float aimAngy;
	/* 0xac */ int throwAttackDamageID;
} HeroAttack;

typedef struct HeroHeadIdle { // 0x20
	/* 0x00 */ VECTOR rotOffset;
	/* 0x10 */ int timer;
	/* 0x14 */ float gain;
	/* 0x18 */ float damp;
	/* 0x1c */ int pad;
} HeroHeadIdle;

typedef struct FpsCam {
	MATRIX CameraMatrix;											// 0x11A0
    struct CameraAngleZ CameraYaw;									// 0x11E0
    struct CameraAngleY CameraPitch;								// 0x1200
    int state;														// 0x1220
	short int quick_turn_input_time;								// 0x1224
	short int snap_hold_time;										// 0x1226
	float snap_input;												// 0x1228
    struct Moby *Target;											// 0x122C
	VECTOR target_last_pos;											// 0x1230
	VECTOR target_vel;												// 0x1240
	float target_blend_fac;											// 0x1250
    float CameraPitchMin; // aka: float mix_y_rot                   // 0x1254
    float CameraPitchMax; // aka: float man_y_rot                   // 0x1258
    struct Moby *pExcludeMoby;										// 0x125C
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
	FpsCam Vars; 													// 0x11A0 - 0x130C
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

typedef struct HeroWeaponPosRec { // 0x80
	/* 0x00 */ VECTOR fpGunMtx[4];
	/* 0x40 */ VECTOR tpGunMtx[4];
} HeroWeaponPosRec;

typedef struct HeroWalkToPos { // 0x20
	VECTOR idealPos;												// 0x13C0
	int abortOnArrival;												// 0x13D0
	int walkToTeleport;												// 0x13D4
	int teleportWaitTime;											// 0x13D8
	Moby *pTeleTarget;												// 0x13DC
} HeroWalkToPos;

typedef struct HeroSurf { // 0x30
	/* 0x00 */ float sinkDepth;
	/* 0x04 */ float sinkRate;
	/* 0x08 */ int minSwimTimer;
	/* 0x0c */ float surfHeight;
	/* 0x10 */ float surfHeightSpeed;
	/* 0x14 */ Moby *pIceCube;
	/* 0x18 */ float bobt;
	/* 0x1c */ float bobz;
	/* 0x20 */ float swingAng1;
	/* 0x24 */ float swingAng2;
	/* 0x28 */ int pad[2];
} HeroSurf;

typedef struct HeroWalk { // 0x30
	/* 0x00 */ VECTOR idealTurnVec;
	/* 0x10 */ float iceMotionDotProduct;
	/* 0x14 */ float lastGroundSlope;
	/* 0x18 */ float idealTurnAng;
	/* 0x1c */ char ideal_motion;
	/* 0x1d */ char skateThrust;
	/* 0x1e */ char long_trans;
	/* 0x1f */ char qturning;
	/* 0x20 */ int idealAngSet;
	/* 0x24 */ int pad[3];
} HeroWalk;

struct HeroNPJumpThrustStage { // 0x10
	/* 0x0 */ float initThrust;
	/* 0x4 */ float thrustDelta;
	/* 0x8 */ int time;
	/* 0xc */ int pad;
};

struct HeroJumpNonParab { // 0x30
	/* 0x00 */ int startThrustTime;
	/* 0x04 */ int endThrustTime;
	/* 0x08 */ float thrustStartFrm;
	/* 0x0c */ float peakFrm;
	/* 0x10 */ int timeToPeakFrm;
	/* 0x14 */ struct HeroNPJumpThrustStage *thrustTable;
	/* 0x18 */ int thrustStageIndex;
	/* 0x1c */ int thrustStageTimer;
	/* 0x20 */ float thrust;
	/* 0x24 */ float chargeJumpThrust;
	/* 0x28 */ float descendGravity;
	/* 0x2c */ int pad;
};

typedef struct HeroJump { // 0x100
	/* 0x00 */ struct HeroJumpNonParab nonParab;
	/* 0x30 */ VECTOR takeoffPos;
	/* 0x40 */ VECTOR snapJumpThrustVec;
	/* 0x50 */ VECTOR snapJumpForwardVec;
	/* 0x60 */ float camHeight;
	/* 0x64 */ float turnSpeed;
	/* 0x68 */ float wallJumpXySpeed;
	/* 0x6c */ char land_timer;
	/* 0x6d */ char useNonParabAscent;
	/* 0x6e */ char descend;
	/* 0x6f */ char noGlide;
	/* 0x70 */ float maxFallSpeed;
	/* 0x74 */ float maxXySpeed;
	/* 0x78 */ float ideal_height;
	/* 0x7c */ short int pushOffTime;
	/* 0x7e */ short int framesToLand;
	/* 0x80 */ float up_thrust;
	/* 0x84 */ float up_thrust_total;
	/* 0x88 */ float coll_bot;
	/* 0x8c */ float ledgeJumpSpeed;
	/* 0x90 */ float snapJumpThrustAng;
	/* 0x94 */ float snapJumpForwardAng;
	/* 0x98 */ float peakFrm;
	/* 0x9c */ float landFrm;
	/* 0xa0 */ float gameLandFrm;
	/* 0xa4 */ int snapJumpDir;
	/* 0xa8 */ float snapJumpRunThrust;
	/* 0xac */ float snapJumpSpeed;
	/* 0xb0 */ VECTOR wallJumpDir;
	/* 0xc0 */ VECTOR wallJumpDirCur;
	/* 0xd0 */ float accel;
	/* 0xd4 */ float decel;
	/* 0xd8 */ float minHeight;
	/* 0xdc */ float maxHeight;
	/* 0xe0 */ float fallThresh;
	/* 0xe4 */ float animMayaScale;
	/* 0xe8 */ short int maxUpTime;
	/* 0xea */ short int bailoutSafetyTime;
	/* 0xec */ short int particleTimer;
	/* 0xee */ short int particleTimer2;
	/* 0xf0 */ float gravity;
	/* 0xf4 */ short int minForwardThrust;
	/* 0xf6 */ short int minTimeToGlide;
	/* 0xf8 */ short int onIce;
	/* 0xfa */ short int minTimeToFall;
	/* 0xfc */ char gloveAttackOk;
	/* 0xfd */ char strafingFlip;
	/* 0xfe */ char flipJumpOk;
	/* 0xff */ char doubleJumpOk;
} HeroJump;

typedef struct HeroLedge { // 0x40
	/* 0x00 */ VECTOR idealWallPos;
	/* 0x10 */ VECTOR idealGrabPos;
	/* 0x20 */ float groundHeight;
	/* 0x24 */ float wallAngZ;
	/* 0x28 */ int valid;
	/* 0x2c */ float gravity;
	/* 0x30 */ float camHeight;
	/* 0x34 */ int flags;
	/* 0x38 */ Moby *pMoby;
	/* 0x3c */ int pad;
} HeroLedge;

typedef struct HeroCharge { // 0x20
	/* 0x00 */ VECTOR padWindUp;
	/* 0x10 */ float groundSpeed;
	/* 0x14 */ int hitEdge;
	/* 0x18 */ int pad[2];
} HeroCharge;

typedef struct HeroWind { // 0x20
	/* 0x00 */ VECTOR vel;
	/* 0x10 */ float speed;
	/* 0x14 */ float angy;
	/* 0x18 */ float angz;
	/* 0x1c */ int pad;
} HeroWind;

typedef struct HeroFall { // 0x20
	/* 0x00 */ float gravity;
	/* 0x04 */ float xyDecel;
	/* 0x08 */ float xRotSpeed;
	/* 0x0c */ float yRotSpeed;
	/* 0x10 */ float xRotSpeedIdeal;
	/* 0x14 */ float yRotSpeedIdeal;
	/* 0x18 */ float glideTaperSpeed;
	/* 0x1c */ int pad;
} HeroFall;

typedef struct HeroSwing { // 0x40
	/* 0x00 */ Moby *pNextTarget;
	/* 0x04 */ Moby *pTarget;
	/* 0x08 */ float targetScore;
	/* 0x0c */ short int connected;
	/* 0x0e */ char valid;
	/* 0x0f */ char qSwitchMe;
	/* 0x10 */ float idealRadius;
	/* 0x14 */ float curCableLen;
	/* 0x18 */ float radialSpeed;
	/* 0x1c */ float forwardAng;
	/* 0x20 */ float gravity;
	/* 0x24 */ float firstSwingSpeed;
	/* 0x28 */ float alignRotSpeed;
	/* 0x2c */ short int animScaleQueued;
	/* 0x2e */ short int firstSwing;
	/* 0x30 */ float swingElv;
	/* 0x34 */ float radialGain;
	/* 0x38 */ float radialDamp;
	/* 0x3c */ float radialLimit;
} HeroSwing;

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
	/*  38 */ u32 UID;
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
	/*  b4 */ u32 updated;
	/*  b8 */ short int gadgetsWithAmmo;
	/*  ba */ short int fpsMode;
	/*  bc */ u8 flags;
};

struct tNW_PlayerPadInputMessage {
	/*   0 */ int cameraRot[4];
	/*  10 */ short int playerPos[3];
	/*  16 */ u8 sequenceNum;
	union
	{
		/*  17 */ u8 stateAndRotFlag : 1;
		/*  17 */ u8 playerIndex : 1;
		/*  17 */ u8 flags : 2;
		/*  17 */ u8 framesWithButtonDiffs : 4;
	};
	/*  18 */ u8 pad_data[128];
};

struct tNW_PlayerPadInputMessageListElem {
	/*   0 */ struct tNW_PlayerPadInputMessage msg;
	/*  98 */ struct tNW_PlayerPadInputMessageListElem* pNext;
	/*  9c */ struct tNW_PlayerPadInputMessageListElem* pPrev;
	/*  a0 */ char inUse;
};

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
	/* ab4 */ u8 padFrame[20];
	/* ac8 */ int homeBaseIndex;
	/* acc */ int homeNodeIndex;
};

typedef struct tNW_PlayerStateMessage { // 0x1c
	/* 0x00 */ char netPlayerIndex;
	/* 0x01 */ char newState;
	/* 0x02 */ char seqNum;
	/* 0x03 */ char frame;
	/* 0x04 */ float pos[3];
	/* 0x10 */ float rot[3];
} tNW_PlayerStateMessage;

typedef struct tNW_KillDeathMessage { // 0x18
	/* 0x00 */ char killingPlayerIndex;
	/* 0x01 */ u8 deathState;
	/* 0x02 */ char deadPlayerIndex;
	/* 0x03 */ char killingWeapon;
	/* 0x04 */ char killType;
	/* 0x08 */ u32 killerUID;
	/* 0x0c */ u32 deathData;
	/* 0x10 */ short iTag;
	/* 0x12 */ short iTagFlg;
	/* 0x14 */ int netFrameTime;
} tNW_KillDeathMessage;

typedef struct pad_frame { // 0x2
	/* 0x0 */ u8 data[2];
} pad_frame;

typedef struct PadStream { // 0x1e0
	/* 0x000 */ short int button_bits[4];
	/* 0x008 */ short int analog_stick[4];
	/* 0x010 */ short int right_analog_stick[4];
	/* 0x018:0 */ u8 frames_with_btn_diffs : 4;
	/* 0x019 */ u8 btn_bit_diffs[32];
	/* 0x03c */ int cur_btn_bit_offset;
	/* 0x040 */ pad_frame prev_frame;
	/* 0x042 */ unsigned char pad_stream_buf[2][200];
	/* 0x1d4 */ int totalPadStreamBytes;
	/* 0x1d8 */ int curFrame;
	/* 0x1dc */ int padStreamReady;
} PadStream;

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
typedef struct Player {
	struct Guber Guber;										// 0x00
	int padding[1];											// 0x18
	MATRIX Water;											// 0x20
	MATRIX Terrain;											// 0x60
	VECTOR PlayerPosition;                                  // 0xA0
	union {													// 0xB0
		VECTOR PlayerRotation;
		struct {
			float PlayerRoll;								// 0xB0
			float PlayerPitch;								// 0xB4
			float PlayerYaw;								// 0xB8
			float pad;										// 0xBC
		};
	};
	VECTOR rotSpeed;										// 0xC0
	VECTOR sphereCenter;									// 0xD0
	VECTOR missileTarget;									// 0xE0
	VECTOR mtxFxScale;										// 0xF0
	VECTOR lastPosition;									// 0x100
	VECTOR stickInput;										// 0x110
	HeroMove move;											// 0x120
	HeroColl coll;											// 0x1C0
	HeroGround ground;										// 0x230 - 0x2EC
	HeroTimers timers;										// 0x2F0
	HeroHotspots hotspots;									// 0x370
	HeroFireDir fireDir;									// 0x380
	HeroLockOn lockOn;										// 0x3D0
	HeroMobys mobys;										// 0x420
	VECTOR wrenchThrownPos;									// 0x430
	VECTOR wrenchThrownRot;									// 0x440
	Gadget Weapon;											// 0x450
	Gadget Boots;											// 0x4A0
	Gadget gadget2;											// 0x4F0
	Gadget gadget3;											// 0x540
	Gadget gadget4;											// 0x590
	char unk_5e0[0x30];
	HeroAnim anim;											// 0x610
	char unk_630[0x10];
	HeroJoints joints;										// 0x640
	HeroAnimLayers animLayers;								// 0x740
	HeroTweaker tweaker[12];								// 0x760
	HeroShadow shadow;										// 0xFA0
	char unk_fc8[0x18];
	HeroAttack attack;										// 0xFE0
	HeroHeadIdle head;										// 0x1090
	char unk_10b0[0xd0];
	HeroCamera fps;											// 0x1180 - 0x133C
	HeroWeaponPosRec weaponPosRec;							// 0x1340
	HeroWalkToPos walkToPos;								// 0x13C0 - 0x13DC
	HeroSurf surf;											// 0x13E0
	HeroWalk walk;											// 0x1410
	HeroJump jump;											// 0x1430
	HeroLedge ledge;										// 0x1540
	HeroCharge charge;										// 0x1580
	HeroWind wind;											// 0x15A0
	HeroFall fall;											// 0x15C0
	HeroSwing swing;										// 0x15E0
	char unk_1620[0x160];
	MotionBlur wrenchMotionBlur;							// 0x1780 - 0x18CC
	int WeaponHeldId;										// 0x18D0
	char unk_18d4[0xec];
	void * pUnk_19c0;										// 0x19C0										// 0x18D0
	char unk_19c4[0x1c];
	short unk_19e0;
	short unk_19e2;
	char State;												// 0x19E4
	char SubState;											// 0x19E5
	char StateType;											// 0x19E6
	char PreviousState;										// 0x19E7
	char PreviousType;										// 0x19E8
	char PrePreviousState;									// 0x19E9
	char PrePreviousType;									// 0x19EA
	char StateHistory[8];									// 0x19EB
	char StateTypeHistory[8];								// 0x19F3
	char unk_19fb[0xc];
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
	char unk_1a2e[0x4];
	WeaponQuickSelect QuickSelect;							// 0x1A32 to 0x1A34
	char unk_1a35[0x1e];
	WeaponAmmo WeaponAmmo;									// 0x1A53 to 0x1A5E
	char unk_1a5f[0x4];
	WeaponMeter WeaponMeter;								// 0x1A63 to 0x1A6E
	char unk_1a6f[0x11];
	GadgetEvent gadgetEventSlots[10];						// 0x1A80
	int unk_1da0;
	GadgetEvent *pNextGadgetEvent;							// 0x1DA4
	int TopOfPlayerStruct2;									// 0x1DA8
	char unk_1dac[0x5f4];
	VECTOR camPos;											// 0x23A0
	VECTOR camRot;											// 0x23B0
	VECTOR camUMtx[3];										// 0x23C0
	char unk_23f0[0x44];
	Moby *SkinMoby3;										// 0x2434
	char unk_2438[0x10];
	int firingAnim;											// 0x2448
	int firingGadget;										// 0x244C
	int desiredCam;											// 0x2450
	Moby *pHeadTargetMoby;									// 0x2454
	Moby *pSheepMoby;										// 0x2458
	Moby *pWhoHitMe;										// 0x245C
	struct Player *pWhoSheepedMe;							// 0x2460
	int sheepMeLongTime;									// 0x2464
	float stickStrength;									// 0x2468
	float stickRawAngle;									// 0x246C
	char unk_2470[0x6];
	char Health;											// 0x2476
	char unk_2477[0x9];
	short unk_2480;	 // Used if Flag is held
	short unk_2482;											// 0x2482
	float skidDeceleration;									// 0x2484
	int mtxFxActive;										// 0x2488
	float analogStickStrength;								// 0x248C
	char deathFallChannel;									// 0x2490
	char wallJumpAngLimiter;								// 0x2491
	char wallJumpHeightFactor;								// 0x2492
	char wallJumpChainCnt;									// 0x2493
	float wallJumpDist;										// 0x2494
	float moonJumpIdealHeight;								// 0x2498
	float moonJumpGravity;									// 0x249C
	short int chargeDoubleTapTimer;							// 0x24A0
	char unk_24a2;
	char unk_24a3;
	short int unkGadgetTimer_24a4;							// 0x24A4
	short int GadgetPADDelayTimer;							// 0x24A6
	short int unkGadgetTimer_24a8;							// 0x24A8
	char unk_24aa[0x1e];
	char unk_24c8;
	char unk_24c9; // hides reticle/hud, does something with CTF Flag
	char unk_24ca;
	char unk_24cb;
	u32 LastDamagedMeOwnerUID;								// 0x24CC
	int LastDamagedMeGadgetId;								// 0x24D0
	char DamageTaken;										// 0x24D4
	char unk_24d5[0x27];
	char isGetHitMsgPending;								// 0x24FC
	char LookAndCrouch;										// 0x24FD
	char LookAndThrowWrench;								// 0x24FE
	char earlyThrowAbort;									// 0x24FF
	char lookAndGetHit;										// 0x2500
	// lastDeathWasSuicide: Doesn't Spawn Pack if true
	char LastDeathWasSuicide;								// 0x2501
	char unk_2502[0x12];
	int HudHealthTimer;										// 0x2514
	char PauseOn;                                 		 	// 0x2518
	char PauseTimer;										// 0x2519
	char tauntOverrideTimer;								// 0x251A
	char PlayerType;										// 0x251B
	Moby *FlagMoby;											// 0x251C
	HeroPlayerConstants *PlayerConstants;					// 0x2520
	Moby *PlayerMoby;										// 0x2524
	Vehicle *Vehicle;										// 0x2528
	Vehicle *VehiclePending;								// 0x252C
	GameCamera *Camera;										// 0x2530
	PadButtonStatus *Paddata;								// 0x2534
	float PlayerPositionX;									// 0x2538
	float PlayerPositionZ;									// 0x253C
	float PlayerPositionY;									// 0x2540
	int mpIndex;											// 0x2544
	int mpTeam;												// 0x2548
	int vehicleState;										// 0x254C
	int vehicleStateTimer;									// 0x2550
	int pointsLastKill;										// 0x2554
	struct tNW_Player *pNetPlayer;							// 0x2558
	char unk_255c[0x1c];
	Moby *lastVehicleMoby;									// 0x2578
	int unk_257c;
	char Animation_RightArm[0x3c0];							// 0x2580
	char Animation_LeftArm[0x3c0];							// 0x2940
	char Animation_RightArm_Unk1[0x400];					// 0x2D00
	char Animation_RightArm_Unk2[0x400];					// 0x3100
	char Animation_LeftArm_Unk1[0x400];						// 0x3500
	char Animation_LeftArm_Unk2[0x400];						// 0x3900
	char Animation_Unk[0x540];								// 0x3D00
	char unk_4240[0xc0];
	union {
		struct {
			int frameSentStick;								// 0x4300
			float damageDone;								// 0x4304
			short int numKills;								// 0x4308
			short slot;										// 0x430A
			PadStream padStream;							// 0x430C
			unsigned char startGameButtonOffFrames;			// 0x44EC
			unsigned char curPadMsgSequenceNum;				// 0x44ED
			char curPadMsgFrame;							// 0x44EE
			char framesUntilStateRot;						// 0x44EF
			short mapTimer;									// 0x44F0
			short int lastMineId;							// 0x44F2
			HeroCommand command;							// 0x44F4
		} LocalHero;
	};
	// END! :D  (Size: 0x4500)
} Player;

typedef void (*PlayerUpdate_func)(Player * player);
typedef void (*PlayerUpdateState_func)(Player * player, int stateId, int a2, int a3, int t0);

typedef struct PlayerVTable
{
    void * FUNC_00; // no pointer
    void * FUNC_04; // no pointer
    void * FUNC_08; // just a return;
    PlayerUpdate_func Update;
    void * FUNC_10; // returns (player->unk_2524) ???
    void * FUNC_14; // 
    void * FUNC_18; // just a return;
    void * FUNC_1C; // SetTeam(player, team)
    void * FUNC_20; // Spawns Local?
    void * FUNC_24; // 
    void * FUNC_28; // Updates Left Stick Movement
    void * FUNC_2C; // Updates Player Position
    void * FUNC_30; // Runs when player dies or teleports
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
int playerGetHealth(Player * player);

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
 * NAME :		playerGiveWeaponUpgade
 * DESCRIPTION :
 * 			Upgrades the target players weapon.
 * NOTES :  Only Upgrades if the player has the given weapon.
 * ARGS : 
 *      player      :           Target player.
 * 		weaponId	:			Weapon
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerGiveWeaponUpgrade(Player * player, int weaponId);

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
/*
 * NAME :		playerHasShield
 * DESCRIPTION :
 * 				Checks to see if player has shield
 * NOTES :
 * ARGS : 		player: player
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int playerHasShield(Player * player);
/*
 * NAME :		playerGiveShield
 * DESCRIPTION :
 * 				Gives the player a shield
 * NOTES :
 * ARGS : 		player: player
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerGiveShield(Player * player);
/*
 * NAME :		playerGetFromSlot
 * DESCRIPTION :
 * 				Gets the needed players struct pointer
 * NOTES :
 * ARGS : 		index: index
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
Player * playerGetFromSlot(int index);

#endif // _LIBUYA_PLAYER_H_
