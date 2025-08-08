/***************************************************
 * FILENAME :		player.h
 * DESCRIPTION :
 * 		Contains player specific offsets and structures for UYA.
 * NOTES :
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

typedef float PlayeWalkSpeedTable[2][4];

/*
* NAME :		PlayerCameraType
* DESCRIPTION :
* 			Defines the three player camera types.
*          These are used in the player struct under 'CameraType'
* NOTES :
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
* DESCRIPTION :
* 			Defines the player states.
* NOTES :
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

typedef enum ObfuscateMode {
	OBFUSCATE_MODE_STATE = 0,
	OBFUSCATE_MODE_HEALTH = 1,
	OBFUSCATE_MODE_GADGET = 2,
	OBFUSCATE_MODE_TIMER = 3,
} ObfuscateMode_e;

typedef enum DeobfuscateMode {
	DEOBFUSCATE_MODE_HEALTH = 0,
	DEOBFUSCATE_MODE_STATE = 0,
	DEOBFUSCATE_MODE_GADGET = 1,
	DEOBFUSCATE_MODE_TIMER = 2,
} DeobfuscateMode_e;

typedef struct Obfuscate {
	u8 *randData;
	int max;
	int step;
	int multiplyVal;
	union {
		u64 data;
		struct {
			u32 addr;
			u32 val;
		};
	};
} Obfuscate_t;

typedef struct CameraAngleZ { // 0x20
	/* 0x11e0 */ float rotation;
	/* 0x11e4 */ float speed_current;
	/* 0x11e8 */ float speed_max_quick;
	/* 0x11ec */ float speed_max_aim;
	/* 0x11f0 */ float accel_quick;
	/* 0x11f4 */ float accel_aim;
	/* 0x11f8 */ float target_slowness_factor_quick;
	/* 0x11fc */ float target_slowness_factor_aim;
} CameraAngleZ;

typedef struct CameraAngleY { // 0x20
	/* 0x1200 */ float rotation;
	/* 0x1204 */ float speed_current;
	/* 0x1208 */ float speed_max;
	/* 0x120c */ float accel;
	/* 0x1210 */ float target_slowness_factor;
	/* 0x1214 */ float strafe_turn_factor;
	/* 0x1218 */ float strafe_tilt_factor;
	/* 0x121c */ float max_target_angle;
} CameraAngleY;

typedef struct HeroTimers { // 0x80
	/* 0x00 */ int state;
	/* 0x04 */ int stateType;
	/* 0x08 */ int subState;
	/* 0x0c */ int animState;
	/* 0x10 */ int stickOn;
	/* 0x14 */ int stickOff;
	/* 0x18 */ short int noLedge;
	/* 0x1a */ short int allowQuickSelect;
	/* 0x1c */ int firing;
	/*      */ char unk_310; // Obfuscated
	/*      */ char unk_311; // Obfuscated
	/*      */ char unk_312; // Obfuscated
	/* 0x23 */ char collOff; // Obfuscated
	/* 0x24 */ char unk_314;
	/* 0x25 */ char unk_315;
	/* 0x26 */ char shield;
	/* 0x27 */ char unk_317[0x5];
	/* 0x2c */ char resurrectWait;
	/* 0x2d */ char NotUsed;
	/* 0x2e */ short int unkTimer_31e;
	/* 0x30 */ short int noInput;
	/*      */ short int unkTimer_322; // Not Used
	/*      */ short int unkTimer_324;
	/*      */ short int unkTimer_326;
	/* 0x38 */ float gadgetRefire;
	/*      */ short int unkTimer_32c;
	/* 0x3e */ short int noShockAbort;
	/*      */ short int UnkTimer_330;
	/*      */ short int unkTimer_332; // Not Used
	/* 0x44 */ int wake_timer;
	/* 0x48 */ int drop_timer;
	/*      */ short int unktimer_33c; // Not Used
	/* 0x4e */ short int magnetic;
	/*      */ short int unkTimer_340;
	/*      */ short int unkTimer_342;
	/* 0x54 */ short int noDeathTimer;
	/*      */ short int unkTimer_346; // Not Used
	/*      */ short int unkTimer_348;
	/*      */ short int unktimer_34a;
	/*      */ short int unktimer_34c; // Not Used
	/*      */ short int unktimer_34e; // Not Used
	/* 0x60 */ int timeAlive;
	/*      */ int unk_354;
	/*      */ int unk_358;
	/* 0x6c */ int noFpsCamTimer;
	/*      */ int unkTimer_360;
	/*      */ char unk_364[0xc];
} HeroTimers;

typedef struct HeroSwim { // 0x50
	/* 0x00 */ vec4 padWindUp;
	/* 0x10 */ int diveTime;
	/* 0x14 */ float bankSpeed;
	/* 0x18 */ float pitchSpeed;
	/* 0x1c */ float yawSpeed;
	/* 0x20 */ int bubbleTimer;
	/* 0x24 */ int minSwimTimer;
	/* 0x28 */ short int chargeBubbleTimer;
	/* 0x2a */ short int soundTimer;
	/* 0x2c */ float buoyancySpeed;
	/* 0x30 */ short int padUpTime;
	/* 0x32 */ short int padDownTime;
	/* 0x34 */ short int ring_timer;
	/* 0x36 */ short int drop_timer;
	/* 0x38 */ short int wake_timer;
	/* 0x3a */ short int plunge_bubbles;
	/* 0x3c */ short int solidRiseTimer;
	/* 0x3e */ short int riseTimer;
	/* 0x40 */ int riseTapCnt;
	/* 0x44 */ int timeRiseLastPressed;
	/* 0x48 */ int pad[2];
} HeroSwim;

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
	/* 0x98 */ int pad[2];
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

typedef struct HeroGround { // 0xc0
	/* 0x00 */ VECTOR normal;
	/* 0x10 */ VECTOR waterNormal;
	/* 0x20 */ VECTOR gravity;
	/* 0x30 */ VECTOR point;
	/* 0x40 */ VECTOR lastGoodPos;
	/* 0x50 */ VECTOR externalBootGrav;
	/* 0x60 */ float feetHeights[2];
	/* 0x68 */ float pitchSlopes[2];
	/* 0x70 */ float rollSlopes[2];
	/* 0x78 */ float height;
	/* 0x7c */ float dist;
	/* 0x80 */ float slope;
	/* 0x84 */ float pitchSlope;
	/* 0x88 */ float rollSlope;
	/* 0x8c */ float angz;
	/* 0x90 */ float waterHeight;
	/* 0x94 */ float quicksandHeight;
	/* 0x98 */ int underWater;
	/* 0x9c */ Moby* pMoby;
	/* 0xa0 */ int onGood;
	/* 0xa4 */ float speed;
	/* 0xa8 */ short int magnetic;
	/* 0xaa */ short int stickLanding;
	/* 0xac */ short int offAny;
	/* 0xae */ short int offGood;
	/* 0xb0 */ int oscillating;
	/* 0xb4 */ float oscPos1;
	/* 0xb8 */ float oscPos2;
	/* 0xbc */ int pad;
} HeroGround;

typedef struct HeroCommand { // 0xc
	/* 0x0 */ int state;
	/* 0x4 */ Moby *pCurTarget;
	/* 0x8 */ int timer;
} HeroCommand;

typedef struct HeroPlayerConstants { // 0x70
	/* 0x00 */ int mobyNum;
	/* 0x04 */ float maxWalkSpeed;
	/* 0x08 */ float kneeHeight;
	/* 0x0c */ float kneeCheckDist;
	/* 0x10 */ float colRadius;
	/* 0x14 */ float colTop;
	/* 0x18 */ float colBot;
	/* 0x1c */ float colBotFall;
	/* 0x20 */ int jumpPushOffTime;
	/* 0x24 */ float jumpPeakFrm;
	/* 0x28 */ float jumpLandFrm;
	/* 0x2c */ float jumpGameLandFrm;
	/* 0x30 */ float jumpMaxHeight;
	/* 0x34 */ float jumpMinHeight;
	/* 0x38 */ int jumpMaxUpTime;
	/* 0x3c */ float jumpGravity;
	/* 0x40 */ float jumpMaxXySpeed;
	/* 0x44 */ float fallGravity;
	/* 0x48 */ float maxFallSpeed;
	/* 0x4c */ float walkAnimSpeedMul;
	/* 0x50 */ float walkAnimSpeedLimLower;
	/* 0x54 */ float walkAnimSpeedLimUpper;
	/* 0x58 */ float jogAnimSpeedMul;
	/* 0x5c */ float jogAnimSpeedLimLower;
	/* 0x60 */ float jogAnimSpeedLimUpper;
	/* 0x64 */ int pad[3];
} HeroPlayerConstants;

typedef struct Gadget { // 0x50
	/* 0x00 */ VECTOR jointPos;
	/* 0x10 */ VECTOR jointRot;
	/* 0x20 */ Moby *pMoby;
	/* 0x24 */ Moby *pMoby2;
	/* 0x28 */ int padButtonDown;
	/* 0x2c */ int alignPad;
	/* 0x30 */ int padButton;
	/* 0x34 */ int gsSpawnFrame;
	/* 0x38 */ char noAmmoTime;
	/* 0x39 */ char unEquipTimer;
	/* 0x3a */ char detached;
	/* 0x3b */ char unequipTime;
	/* 0x3c */ char unEquipStatus;
	/* 0x3d */ char unEquipDelay;
	/* 0x3e */ short pad;
	/* 0x40 */ int equippedTime;
	/* 0x44 */ int state;
	/* 0x48 */ int id;
	/* 0x4c */ float lightAng;
} Gadget;

typedef struct HeroGadgets { // 0x1e0
	/* 0x000 */ Gadget weapon;
	/* 0x050 */ Gadget boots;
	/* 0x0a0 */ Gadget gadget3;
	/* 0x0f0 */ Gadget gadget4;
	/* 0x140 */ Gadget gadget5;
	/* 0x190 */ Gadget gadget6;
} HeroGadgets;

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

typedef struct HeroShadow { // 0x20
	/* 0x00 */ float slope;
	/* 0x04 */ float plane;
	/* 0x08 */ float range;
	/* 0x0c */ int sample_id;
	// /* 0x10 */ int pad[1]; // commented due to misalignment for HeroTurn struct
	/* 0x10 */ float sample_pos[4];
} HeroShadow;

typedef struct HeroThrust { // 0x10
	/* 0x0 */ float ideal;
	/* 0x4 */ float actual;
	/* 0x8 */ int pad[2];
} HeroThrust;

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

typedef struct HeroTailIdle { // 0x50
	/* 0x00 */ VECTOR rotOffsets[4];
	/* 0x40 */ int timers[4];
} HeroTailIdle;

typedef struct FpsCamVars {
	/* 0x11a0 */ MATRIX cameraMatrix;
	/* 0x11e0 */ struct CameraAngleZ cameraZ;
	/* 0x1200 */ struct CameraAngleY cameraY;
	/* 0x1220 */ int state;
	/* 0x1224 */ short int quick_turn_input_time;
	/* 0x1226 */ short int snap_hold_time;
	/* 0x1228 */ float snap_input;
	/* 0x122c */ struct Moby *pTarget;
	/* 0x1230 */ VECTOR target_last_pos;
	/* 0x1240 */ VECTOR target_vel;
	/* 0x1250 */ float target_blend_fac;
	/* 0x1254 */ float min_y_rot; // aka: float mix_y_rot
	/* 0x1258 */ float max_y_rot; // aka: float man_y_rot
	/* 0x125c */ struct Moby *pExcludeMoby;
	/* 0x1260 */ VECTOR positionOffset;
	/* 0x1270 */ VECTOR rotationOffset;
	/* 0x1280 */ short int flags;
	/* 0x1282 */ short int allegiance;
	/* 0x1284 */ void *special_target_func;
	/* 0x1288 */ MATRIX *pWorldMtx;
	/* 0x128c */ MATRIX *pWorldInvMtx;
	/* 0x1290 */ VECTOR facing_dir;
	/* 0x12a0 */ VECTOR internal_facing_dir;
	/* 0x12b0 */ VECTOR aim_pos;
	/* 0x12c0 */ float range;
	/* 0x12c4 */ float ext_extension;
	/* 0x12c8 */ float ext_extension_speed;
	/* 0x12cc */ int cam_slot; // aka: LocalPlayerIndex
	/* 0x12d0 */ void *pHero;
	/* 0x12d4 */ float camRadius;
	/* 0x12d8 */ int camSettingsIndex; // aka: PlayerId
	/* 0x12dc */ int karma_pad;
	/* 0x12e0 */ VECTOR prevCamPos;
	/* 0x12f0 */ int karma_pad2[4];
	/* 0x1300 */ float camYSpeed;
	/* 0x1304 */ float camZSpeed;
	/* 0x1308 */ float gunInterp;
	/* 0x130c */ float gunInterpSpeed;
} FpsCamVars;

typedef struct HeroFps { // 0x1c0
	/* 0x1180 */ VECTOR cameraPos;
	/* 0x1190 */ VECTOR cameraDir;
	/* 0x11a0 */ FpsCamVars vars;
	/* 0x1310 */ int active;
	/* 0x1314 */ float gunWithdrawSpeed;
	/* 0x1318 */ float gunWithdrawDist;
	/* 0x131c */ float gunWithdrawIdeal;
	/* 0x1320 */ float bobRot;
	/* 0x1324 */ float bobRotSpeed;
	/* 0x1328 */ float bobAmp;
	/* 0x132c */ float camHeroOfs;
	/* 0x1330 */ float camHeroOfsSpeed;
	/* 0x1334 */ int ignoreGroundHeight;
	/* 0x1338 */ float reticlePulseAng;
	/* 0x133c */ int reticleFadeInTimer;
} HeroFps;

typedef struct HeroWeaponPosRec { // 0x80
	/* 0x00 */ VECTOR fpGunMtx[4];
	/* 0x40 */ VECTOR tpGunMtx[4];
} HeroWeaponPosRec;

typedef struct HeroWalkToPos { // 0x20
	/* 0x13c0 */ VECTOR idealPos;
	/* 0x13d0 */ int abortOnArrival;
	/* 0x13d4 */ int walkToTeleport;
	/* 0x13d8 */ int teleportWaitTime;
	/* 0x13dc */ Moby *pTeleTarget;
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

typedef struct HeroDust { // 0x10
	/* 0x0 */ float vel;
	/* 0x4 */ float velvar;
	/* 0x8 */ int timer;
	/* 0xc */ short int rate;
	/* 0xe */ short int flags;
} HeroDust;

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

typedef struct HeroTurn { // 0x20
	/* 0x00 */ vec4 idealVec;
	/* 0x10 */ float ideal;
	/* 0x14 */ float speed;
	/* 0x18 */ float diff;
	/* 0x1c */ int pad;
} HeroTurn;

struct tNW_PlayerWarpMessage { // 0x20
	/* 0x00 */ char netPlayerIndex;
	/* 0x01 */ char dontKillMeInBaseHack;
	/* 0x02 */ char resetPadRing;
	/* 0x03 */ char playerSeq;
	/* 0x04 */ float playerPos[3];
	/* 0x10 */ float playerRot[3];
	/* 0x1c */ short int state;
	/* 0x1e */ char isResurrecting;
};

struct tNW_PlayerData { // 0xc0
	/* 0x00 */ VECTOR position;
	/* 0x10 */ float rotation[3];
	/* 0x1c */ int timeStamp;
	/* 0x20 */ VECTOR idealVec;
	/* 0x30 */ int idealVecTime;
	/* 0x34 */ int accountId;
	/* 0x38 */ u32 UID;
	/* 0x3c */ int playerType;
	/* 0x40 */ int playerTeam;
	/* 0x44 */ float hitPoints;
	/* 0x48 */ int handGadget;
	/* 0x4c */ int lastKeepAlive;
	/* 0x50 */ int remoteTarget;
	/* 0x54 */ int playerIndex;
	/* 0x58 */ int cameraElevationSettings[3];
	/* 0x64 */ int cameraAzimuthSettings[3];
	/* 0x70 */ int cameraRotationSettings[3];
	/* 0x7c */ int rank[6];
	/* 0x94 */ char cGadgetLevel[32];
	/* 0xb4 */ u32 updated;
	/* 0xb8 */ short int gadgetsWithAmmo;
	/* 0xba */ short int fpsMode;
	/* 0xbc */ u8 flags;
};

struct tNW_PlayerPadInputMessage { // 0x98
	/* 0x00 */ int cameraRot[4];
	/* 0x10 */ short int playerPos[3];
	/* 0x16 */ u8 sequenceNum;
	union {
		/* 0x17 */ u8 stateAndRotFlag : 1;
		/* 0x17 */ u8 playerIndex : 1;
		/* 0x17 */ u8 flags : 2;
		/* 0x17 */ u8 framesWithButtonDiffs : 4;
	};
	/* 0x18 */ u8 pad_data[128];
};
 
struct tNW_PlayerPadInputMessageListElem { // 0xa4
	/* 0x00 */ struct tNW_PlayerPadInputMessage msg;
	/* 0x98 */ struct tNW_PlayerPadInputMessageListElem* pNext;
	/* 0x9c */ struct tNW_PlayerPadInputMessageListElem* pPrev;
	/* 0xa0 */ char inUse;
};


struct tNW_Player { // 0x31c
	// Struct needs heavily updated
	// currently new stuff is commented, but correct.
	// /* 0x000 */ int netClientIndex;
	// /* 0x068 */ char bLocal;
	// /* 0x069 */ short int bGiveMeTheDasBoot;
	// /* 0x1a2 */ short int bSpawned;
	// /* 0x1a4 */ short int bUnk_1a4;
	// /* 0x1a6 */ short int bCallbackCalled;

	/* 0x000 */ int netClientIndex;
	/* 0x004 */ struct tNW_PlayerData* pNetPlayerData;
	/* 0x008 */ short int bLocal;
	/* 0x00a */ short int bSpawned;
	/* 0x00c */ short int bGiveMeTheDasBoot;
	/* 0x00e */ short int bCallbackCalled;
	/* 0x010 */ int latency;
	/* 0x014 */ unsigned int flags;
	/* 0x018 */ char accountName[32];
	/* 0x038 */ struct tNW_PlayerWarpMessage warpMessage;
	/* 0x058 */ struct tNW_PlayerPadInputMessageListElem padMessageElems[16];
	/* 0xa98 */ char padMessageCurDecodePos;
	/* 0xa99 */ char activePadFrame;
	/* 0xa9c */ int lastActiveSeqNum;
	/* 0xaa0 */ int numBufferedPadMessageElems;
	/* 0xaa4 */ int receivedActivePadMsgFrame;
	/* 0xaa8 */ char pullBack;
	/* 0xaa9 */ signed char jitterThrottleFrames;
	/* 0xaaa */ char numConseqSkips;
	/* 0xaac */ struct tNW_PlayerPadInputMessageListElem* pActivePadMsg;
	/* 0xab0 */ struct tNW_PlayerPadInputMessageListElem* pPadMsgListTail;
	/* 0xab4 */ u8 padFrame[20];
	/* 0xac8 */ int homeBaseIndex;
	/* 0xacc */ int homeNodeIndex;
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

typedef struct HeroQueuedSound { // 0x8
	/* 0x0 */ short int active;
	/* 0x2 */ short int sound;
	/* 0x4 */ short int timer;
	/* 0x6 */ short int flags;
} HeroQueuedSound;

typedef struct HeroSpecialIdleDef { // 0x10
	/* 0x0 */ int anim;
	/* 0x4 */ float frequency;
	/* 0x8 */ float minRepeatTime;
	/* 0xc */ int repeatTimer;
} HeroSpecialIdleDef;

typedef struct Player { // 0x4500
	/* 0x0000 */ struct Guber guber;
	/* 0x0020 */ MATRIX mtx;
	/* 0x0060 */ MATRIX invMtx;
	/* 0x00a0 */ VECTOR playerPosition;
			union {
	/* 0x00b0 */ VECTOR playerRotation;
				struct {
	/* 0x00b0 */	float playerRoll;
	/* 0x00b4 */	float playerPitch;
	/* 0x00b8 */	float playerYaw;
	/* 0x00bc */	float pad;
				};
			};
	/* 0x00c0 */ VECTOR rotSpeed;
	/* 0x00d0 */ VECTOR sphereCenter;
	/* 0x00e0 */ VECTOR missileTarget;
	/* 0x00f0 */ VECTOR mtxFxScale;
	/* 0x0100 */ VECTOR lastPosition;
	/* 0x0110 */ VECTOR stickInput;
	/* 0x0120 */ HeroMove move;
	/* 0x01c0 */ HeroColl coll;
	/* 0x0230 */ HeroGround ground;
	/* 0x02f0 */ HeroTimers timers;
	/* 0x0370 */ HeroHotspots hotspots;
	/* 0x0380 */ HeroFireDir fireDir;
	/* 0x03d0 */ HeroLockOn lockOn;
	/* 0x0420 */ HeroMobys mobys;
	/* 0x0430 */ HeroGadgets gadget;
	/* 0x0610 */ HeroAnim anim;
	/*        */ int unk_630; // Always gets written -1
	/* 0x0634 */ int animTimerToRestartWalkingAnim;
	/*        */ char unk_638[0x8];
	/* 0x0640 */ HeroJoints joints;
	/* 0x0740 */ HeroAnimLayers animLayers;
	/* 0x0760 */ HeroTweaker tweaker[12];
	/* 0x0fa0 */ char unk_0fa0[0x10];
	/* 0x0fb0 */ HeroThrust thrust;
	/* 0x0fc0 */ HeroTurn turn;
	/* 0x0fe0 */ HeroAttack attack;
	/* 0x1090 */ HeroHeadIdle head;
	/* 0x10b0 */ HeroTailIdle tailIdle;
	/*        */ char unk_1100[0x80];
	/* 0x1180 */ HeroFps fps;
	/* 0x1340 */ HeroWeaponPosRec weaponPosRec;
	/* 0x13c0 */ HeroWalkToPos walkToPos;
	/* 0x13e0 */ HeroSurf surf;
	/* 0x1410 */ HeroWalk walk;
	/* 0x1430 */ HeroJump jump;
	/* 0x1540 */ HeroLedge ledge;
	/* 0x1580 */ HeroCharge charge;
	/* 0x15a0 */ HeroWind wind;
	/* 0x15c0 */ HeroFall fall;
	/* 0x15e0 */ HeroSwing swing;
	/* 0x1620 */ HeroDust dust; // unsure
	/* 0x1630 */ MotionBlur motionBlur;
	/* 0x1780 */ MotionBlur wrenchMotionBlur;
	/* 0x18d0 */ int weaponHeldId;
	/*        */ char unk_18d4[0xc];
	/* 0x18e0 */ VECTOR prevHandPos;
	/*        */ char unk_18f0[0xd0];
	/*        */ void * pUnk_19c0;
	/*        */ char unk_19c4[0x1c];
	/* 0x19e0 */ char playerFontDrawRegistered;
	/* 0x19e1 */ char gadgetGlowCnt;
	/* 0x19e2 */ char playerPostDrawFxRegistered;
	/* 0x19e3 */ char playerPostPreDrawFxRegistered;
	/* 0x19e4 */ char state;
	/* 0x19e5 */ char subState;
	/* 0x19e6 */ char stateType;
	/* 0x19e7 */ char previousState;
	/* 0x19e8 */ char previousType;
	/* 0x19e9 */ char prePreviousState;
	/* 0x19ea */ char prePreviousType;
	/* 0x19eb */ char stateHistory[8];
	/* 0x19f3 */ char stateTypeHistory[8];
	/* 0x19fb */ char stateTimerHistory[8];
	/* 0x1a03 */ char stateHistoryLen;
	/* 0x1a04 */ char unk_1a04;
	/* 0x1a05 */ char cycleFiring;
	/* 0x1a06 */ char gravityType;
	/* 0x1a07 */ char firing;
	/* 0x1a08 */ char raisedGunArm;
	/* 0x1a09 */ char inShallowWater;
	/* 0x1a0a */ char invisible;
	/* 0x1a0b */ char hideWeapon;
	/* 0x1a0c */ char gadgetsOff;
	/* 0x1a0d */ char gadgetNotReady;
	/* 0x1a0e */ char wrenchOnly;
	/* 0x1a0f */ char hideWrench;
	/* 0x1a10 */ char spawnBoltsToMe;
	/* 0x1a11 */ char aiFollowingMe;
	/* 0x1a12 */ char forceWrenchSwitch;
	/* 0x1a13 */ char forceSwingSwitch;
	/* 0x1a14 */ char isLocal;
	/* 0x1a15 */ char inBaseHack;
	/* 0x1a16 */ char shieldTrigger;
	/* 0x1a17 */ char curSeg;
	/* 0x1a18 */ char handGadgetType;
	/* 0x1a19 */ char externalUpdate;
	/*        */ char unk_1a1a[0x14];
	/*        */ char unk_1a2e[0x4];
	/* 0x1a32 */ WeaponQuickSelect quickSelect;
	/*        */ char unk_1a3a[0x6];
	/* 0x1a40 */ char chargebootsSlot;
	/* 0x1a41 */ char unk_1a41; // obfuscated
	/*        */ char unk_1a42[0x11];
	/* 0x1a53 */ Weapons weaponAmmo;
	/*        */ char unk_1a5f[0x4];
	/* 0x1a63 */ Weapons weaponMeter;
	/*        */ char unk_1a6f[0x11];
	/* 0x1a80 */ GadgetEvent gadgetEventSlots[10];
	/*        */ int unk_1da0;
	/* 0x1da4 */ GadgetEvent *pNextGadgetEvent;
	/* 0x1da8 */ struct Player *GadgetBox; // Doesn't work at all like Deadlocked's
	/*        */ char unk_1dac[0x14];
	/* 0x1dc0 */ VECTOR failsafePosRing[32];
	/* 0x1fc0 */ float rotZRing[32];
	/* 0x2040 */ mtx3 gadgetRotRing[16];
	/* 0x2340 */ int rotZringIndex;
	/* 0x2344 */ int rotZringValidSize;
	/* 0x2348 */ int failsafeRingIndex;
	/* 0x234c */ int failsafeRingValidSize;
	/* 0x2350 */ int gadgetRotRingIndex;
	/* 0x2354 */ int gadgetRotRingValidSize;
	/* 0x2358 */ int cameraPosRing[8];
	/* 0x2378 */ int cameraRotRing[8];
	/* 0x2398 */ int camRingIndex;
	/* 0x239c */ int camRingValidSize;
	/* 0x23a0 */ VECTOR camPos;
	/* 0x23b0 */ VECTOR camRot;
	/* 0x23c0 */ VECTOR camUMtx[3];
	/* 0x23f0 */ HeroQueuedSound queuedSounds[2];
	/* 0x2400 */ int loopingSounds[9];
	/* 0x2424 */ Moby *loopingSoundMobys[9];
	/* 0x2448 */ int firingAnim;
	/* 0x244c */ int firingGadget;
	/* 0x2450 */ int desiredCam;
	/* 0x2454 */ Moby *pHeadTargetMoby;
	/* 0x2458 */ Moby *pSheepMoby;
	/* 0x245c */ Moby *pWhoHitMe;
	/* 0x2460 */ struct Player *pWhoSheepedMe;
	/* 0x2464 */ int sheepMeLongTime;
	/* 0x2468 */ float stickStrength;
	/* 0x246c */ float stickRawAngle;
	/*        */ char unk_2470[0x6];
	/* 0x2476 */ char hitPoints;
	/*        */ char unk_2477[0x9];
	/*        */ short unk_2480;	 // Used if Flag is held
	/* 0x2482 */ short unk_2482;
	/* 0x2484 */ float skidDeceleration;
	/* 0x2488 */ int mtxFxActive;
	/* 0x248c */ float analogStickStrength;
	/* 0x2490 */ char deathFallChannel;
	/* 0x2491 */ char wallJumpAngLimiter;
	/* 0x2492 */ char wallJumpHeightFactor;
	/* 0x2493 */ char wallJumpChainCnt;
	/* 0x2494 */ float wallJumpDist;
	/* 0x2498 */ float moonJumpIdealHeight;
	/* 0x249c */ float moonJumpGravity;
	/* 0x24a0 */ short int chargeDoubleTapTimer;
	/*        */ char unk_24a2;
	/*        */ char unk_24a3;
	/* 0x24a4 */ short int unkGadgetTimer_24a4;
	/* 0x24a6 */ short int GadgetPADDelayTimer;
	/* 0x24a8 */ short int unkGadgetTimer_24a8; // gets set when flag is captured.
	/* 0x24aa */ char unk_24aa; // gets set when flag is captured.
	/*        */ char unk_24ab[0x1d];
	/* 0x24c8 */ char unk_24c8;
	/* 0x24c9 */ char unk_24c9; // hides reticle/hud, does something with CTF Flag and setting to 0x32 shows save filed popup.
	/*        */ char unk_24ca;
	/*        */ char unk_24cb;
	/* 0x24cc */ u32 lastDamagedMeOwnerUID;
	/* 0x24d0 */ int lastDamagedMeGadgetId;
	/* 0x24d4 */ int lastDamagedMeHp;
	/*        */ char unk_24d5[0x4];
	/* 0x24dc */ int rAmb;
	/* 0x24e0 */ int gAmb;
	/* 0x24e4 */ int bAmb;
	/*        */ char unk_24e8[0x14];
	/* 0x24fc */ char isGetHitMsgPending;
	/* 0x24fd */ char lookAndCrouch;
	/* 0x24fe */ char lookAndThrowWrench;
	/* 0x24ff */ char earlyThrowAbort;
	/* 0x2500 */ char lookAndGetHit;
	// lastDeathWasSuicide: Doesn't Spawn Pack if true
	/* 0x2501 */ char lastDeathWasSuicide;
	/* 0x2502 */ char unk_2502; // Gets set to -1 when Ressurecting.
	/*        */ char unk_2503[0x11];
	/* 0x2514 */ int hudHealthTimer;
	/* 0x2518 */ char pauseOn;
	/* 0x2519 */ char pauseTimer;
	/* 0x251a */ char tauntOverrideTimer;
	/* 0x251b */ char playerType;
	/* 0x251c */ Moby *flagMoby;
	/* 0x2520 */ HeroPlayerConstants *playerConstants;
	/* 0x2524 */ Moby *pMoby;
	/* 0x2528 */ Vehicle *vehicle;
	/* 0x252c */ Vehicle *vehiclePending;
	/* 0x2530 */ GameCamera *camera;
	/* 0x2534 */ PAD *pPad;
	/* 0x2538 */ float cheatX;
	/* 0x253c */ float cheatZ;
	/* 0x2540 */ float cheatY;
	/* 0x2544 */ int mpIndex;
	/* 0x2548 */ int mpTeam;
	/* 0x254c */ int vehicleState;
	/* 0x2550 */ int vehicleStateTimer;
	/* 0x2554 */ int pointsLastKill;
	/* 0x2558 */ struct tNW_Player *pNetPlayer;
	/* 0x255c */ tNW_PlayerStateMessage newStateMessage;
	/* 0x2578 */ Moby *lastVehicleMoby;
	/* 0x257c */ int iLastVehicleOffTime;
	/* 0x2580 */ char animation_RightArm[0x3c0];
	/* 0x2940 */ char animation_LeftArm[0x3c0];
	/* 0x2D00 */ char animation_RightArm_Unk1[0x400];
	/* 0x3100 */ char animation_RightArm_Unk2[0x400];
	/* 0x3500 */ char animation_LeftArm_Unk1[0x400];
	/* 0x3900 */ char animation_LeftArm_Unk2[0x400];
	/* 0x3D00 */ char animation_Unk[0x540];
	/*        */ char unk_4240[0xc0];
	union {
		struct {
	/* 0x4300 */ int frameSentStick;
	/* 0x4304 */ float damageDone;
	/* 0x4308 */ short int numKills;
	/* 0x430a */ short slot;
	/* 0x430c */ PadStream padStream;
	/* 0x44ec */ unsigned char startGameButtonOffFrames;
	/* 0x44ed */ unsigned char curPadMsgSequenceNum;
	/* 0x44ee */ char curPadMsgFrame;
	/* 0x44ef */ char framesUntilStateRot;
	/* 0x44F0 */ short mapTimer;
	/* 0x44F2 */ short int lastMineId;
	/* 0x44F4 */ HeroCommand command;
		} LocalHero;
	};
	union {
		struct {
	/* 0x4500 */ char unk_4500[0x270];  //GameCamera dummyCamera;
	/* 0x4770 */ PAD remotePad;
	/* 0x4d30 */ char completedEnoughUpdates;
	/* 0x4d31 */ char rotOutOfSyncLastUpdate;
	/* 0x4d32 */ char padFrameChunksReceived;
	/* 0x4d33 */ char posOutOfSyncLastUpdate;
	/* 0x4d34 */ char stateAtSyncFrame;
	/* 0x4d35 */ char unk_4d35[0xb];
	/* 0x4d40 */ VECTOR receivedSyncPos;
	/* 0x4d50 */ VECTOR posAtSyncFrame;
	/* 0x4d60 */ VECTOR syncPosDifference;
	/* 0x4d70 */ VECTOR receivedSyncRot;
	/* 0x4d80 */ VECTOR rotAtSyncFrame;
	/* 0x4d90 */ float interpVel;
	/* 0x4d94 */ float syncRotDifference; // TODO fix this struct
	/*        */ // short int flags;
	/* 0x4d98 */ int sequenceIdOfSyncData;
	/* 0x4d9C */ enum PlayerState receivedState;
	/*        */ VECTOR remoteCorrectionVel;
	/*        */ float remoteCorrectionRotVel;
	/*        */ char syncFrameOffset;
		} RemoteHero;
	};
} Player;

typedef void (*PlayerUpdate_Func)(Player *player);
typedef int (*ReNewMe_Func)(Player *player);
typedef void (*HandleEvent_Func)(Player *player, GuberEvent *event);
typedef int (*FriendlyToTeam_Func)(Player *player, int team);
typedef void (*ResetHero_Func)(Moby *pHeroMoby, VECTOR *pos, VECTOR *rot, int mpIndex);
typedef void (*DoBehavior_Func)(Player *player);
typedef void (*ZeroMovement_Func)(Player *player);
typedef void (*PlayerUpdateState_Func)(Player *player, PlayerState state, int anim_switch, int force, int playDeathSound);
typedef int (*GetVehicleMoby_Func)(Player *player);
typedef int (*IsMyLookCamDisabled_Func)(Player *player);
typedef int (*AllowRemoteLedgeGrab_Func)(Player *player);
typedef int (*GetSlot_Func)(Player *player);
typedef int (*GetMoby_Func)(Player *player);

typedef struct PlayerVTable
{
/* 0x00 */ void * FUNC_00; // no pointer
/* 0x04 */ void * FUNC_04; // no pointer
/* 0x08 */ void * FUNC_08; // just a return;
/* 0x0c */ PlayerUpdate_Func Update;
/* 0x10 */ ReNewMe_Func ReNewMe; // returns (player->pMoby)
/* 0x14 */ HandleEvent_Func HandleEvent;
/* 0x18 */ void * FUNC_18; // just a return;
/* 0x1c */ FriendlyToTeam_Func FriendlyToTeam; // Retunrs True if friendly.
/* 0x20 */ void * FUNC_20; // Spawns Local?  memset 0x4500 bytes to zero.
/* 0x24 */ ResetHero_Func PlayerReset;
/* 0x28 */ void * FUNC_28; // Updates Left Stick Movement
/* 0x2c */ DoBehavior_Func DoBehavior;
/* 0x30 */ ZeroMovement_Func ZeroMovement;
/* 0x34 */ PlayerUpdateState_Func UpdateState; // aka: InitBodyState()
/* 0x38 */ void * FUNC_38;
/* 0x3c */ void * FUNC_3C;
/* 0x40 */ GetVehicleMoby_Func GetVehicleMoby; // Returns the vehicle moby pointer the player is in.
/* 0x44 */ IsMyLookCamDisabled_Func IsMyLookCamDisabled; // returns true or false.
/* 0x48 */ AllowRemoteLedgeGrab_Func AllowRemoteLedgeGrab; // Returns true or false.
/* 0x4c */ GetSlot_Func GetSlot; // Returns 1 (In DL it returs -1)
/* 0x50 */ void * FUNC_50;
/* 0x54 */ void * FUNC_54;
/* 0x58 */ void * FUNC_58;
/* 0x5c */ void * FUNC_5c;
/* 0x60 */ void * FUNC_60;
/* 0x64 */ void * FUNC_64;
/* 0x68 */ GetMoby_Func GetMoby; // Returns pMoby of player.
/* 0x6c */ HandleEvent_Func HandleEvent2;
/* 0x70 */ void * FUNC_70; // Just a return;
/* 0x74 */ FriendlyToTeam_Func FriendlyToTeam2; // Retunrs True if friendly.
/* 0x78 */ void * FUNC_78; //memset zero 0x4dc0 bytes
/* 0x7c */ ResetHero_Func PlayerReset2;
/* 0x80 */ void * FUNC_80;
/* 0x84 */ void * FUNC_84;
/* 0x88 */ void * FUNC_88;
/* 0x8c */ void * FUNC_8c;
/* 0x90 */ void * FUNC_90;
/* 0x94 */ void * FUNC_94;
/* 0x98 */ void * FUNC_98;
/* 0x9c */ void * FUNC_9c;
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
__LIBUYA_GETTER__ PAD * playerGetPad(Player * player);

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

int playerGetRespawnTimer(Player * player);

/*
* NAME :		playerGetState
* DESCRIPTION :	Returns Players State
* NOTES :		Needed for obfuscated state values
* ARGS : 
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
int playerGetState(Player *player);

/*
* NAME :		playerRespawn
* DESCRIPTION :
* 			Respawns the given player.
* NOTES :
*          Spawn point is determined internally.
*          This will freeze on CQ if the player hasn't picked a node to respawn to.
* ARGS : 
*      player      :           Target player.
* RETURN :
* AUTHOR :			Daniel "Dnawrkshp" Gerendasy
*/
void playerRespawn(Player * player);

/*
* NAME :		playerSetPosRot
* DESCRIPTION :
* 			Sets a given player's position and rotation.
* NOTES :
* ARGS : 
*      player      :           Target player.
*      position    :           Position.
*      rotation    :           Rotation.
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
__LIBUYA_SETTER__ void playerSetPosRot(Player *player, VECTOR position, VECTOR rotation);

/*
* NAME :		playerIsDead
* DESCRIPTION :
* 			Returns non-zero if the given player is dead.
* NOTES :
* ARGS : 
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
int playerIsDead(Player * player);

/*
* NAME :		playerStateIsDead
* DESCRIPTION :
* 			Returns non-zero if the given state is equivalent to a dead state.
* NOTES :
* ARGS : 
* RETURN :
* AUTHOR :			JelloGiant
*/
int playerStateIsDead(int state);

/*
* NAME :		playerGiveWeaponUpgade
* DESCRIPTION :	Upgrades the target players weapon.
* NOTES : 		Only Upgrades if the player has the given weapon.
* ARGS : 
*      player      :           Target player.
* 		weaponId	:			Weapon
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
void playerGiveWeaponUpgrade(Player * player, int weaponId);

/*
* NAME :		playerGiveWeapon
* DESCRIPTION :	Give the target player the given weapon ID.
* NOTES :
* ARGS : 
*      player      :           Target player.
* 		weaponId	:			Weapon
* 		type		:			0: Gadget, 1: Weapon
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
void playerGiveWeapon(Player * player, int weaponId, int type);

/*
* NAME :		playerEquipWeapon
* DESCRIPTION :	Equips the given weapon ID.
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
* DESCRIPTION :	Removes all weapons from player.
* NOTES :
* ARGS : 
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
void playerStripWeapons(Player * player);

/*
* NAME :		playerGiveRandomWeapons
* DESCRIPTION :	Gives Target player random weapons
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
* DESCRIPTION : Deobfuscates given player struct item.
* NOTES :
* ARGS : 
*      src     :           Source pointer from player struct of the data needed to deorbuscate.
* 							Ex: &player->hitPoints, &player->state
* 		mode	:			0: Used for: Health, Player State, other.
* 							1: Used for: Weapon IDs, other.
* 							2: Used for: Player Timers.
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
u32 playerDeobfuscate(u32 src, DeobfuscateMode_e mode);
int playerGetRespawnTimer(Player *player);
int playerGetGadetId(Player *player, int slot);
int playerGetGadgetLevel(Player *player, int slot);
int playerGetGadgetAmmo(Player *player, int slot);

/*
* NAME :		playerDeobfuscate
* DESCRIPTION : Deobfuscates given player struct item.
* NOTES :
* ARGS : 
*      src    	:            Destination source of needed obfuscated data.
*	   value	:			Value/Data to be obfuscated
* 							Ex: &player->hitPoints, &player->state
* 		mode	:			0: Used for: Player State, other.
*							1: Used For: Health/hitPoints
* 							2: Used for: Weapon IDs, other.
* 							3: Used for: Player Timers.
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
void playerObfuscate(u32 src, int value, ObfuscateMode_e mode);

/*
* NAME :		playerHasShield
* DESCRIPTION : Checks to see if player has shield
* NOTES :
* ARGS : 		player: player
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
int playerHasShield(Player * player);
/*
* NAME :		playerGiveShield
* DESCRIPTION : Gives the player a shield
* NOTES :
* ARGS : 		player: player
* RETURN :
* AUTHOR :			Troy "Metroynome" Pruitt
*/
void playerGiveShield(Player * player);
/*
* NAME :		playerGetFromSlot
* DESCRIPTION : Gets the needed players struct pointer
* NOTES :
* ARGS : 		index: index
* RETURN :
* AUTHOR :		Troy "Metroynome" Pruitt
*/
Player * playerGetFromSlot(int index);

#endif // _LIBUYA_PLAYER_H_
