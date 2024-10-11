/***************************************************
 * FILENAME :		moby.h
 * DESCRIPTION :
 * 		Contains moby specific offsets and structures for Deadlocked.
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_MOBY_H_
#define _LIBUYA_MOBY_H_

#include <tamtypes.h>
#include "math3d.h"
#include "common.h"
#include "gid.h"

// #include "camera.h"

struct GuberMoby;
struct GuberEvent;
struct Gid;
//--------------------------------------------------------
enum MobyId
{
	MOBY_ID_PLACE_HOLDER = 0x0000,
	MOBY_ID_WEAPON_VENDER = 0x000b,
	MOBY_ID_BOLT_CRATE = 0x01f4,
	MOBY_ID_HEALTH_CRATE_SP = 0x01f5,
	MOBY_ID_BONUS_BOLTS_CRATE = 0x01f7,
	MOBY_ID_EXPLOSIVE_CRATE = 0x01f9,
	MOBY_ID_AMMO_CRATE = 0x01ff,
	MOBY_ID_TITANIUM_BOLT = 0x046e,
	MOBY_ID_PLAYER_ICE_CUBE = 0x052a,
	MOBY_ID_DYNAMO_CONTROL_NODE = 0x0952,
	MOBY_ID_OBANI_JUMP_PAD = 0x09b9,
	MOBY_ID_WATER = 0x0b37,
	MOBY_ID_TELEPORT_PAD_SP = 0x0c60,
	MOBY_ID_LEVEL_MODEL_ANIMATION = 0x0c66,
	MOBY_ID_BASE_LIGHT = 0x0cbe,
	MOBY_ID_LEVEL_MODEL_TEXTURE_ANIMATION = 0x0ce9,
	MOBY_ID_EXPLOSION = 0x0d4e, // Used for Mini-Rocket Tube Rocket Explosion
	MOBY_ID_JUMP_PAD_SP = 0x0f05,
	MOBY_ID_TELEPORT_PAD_RC1 = 0x0f51,
	MOBY_ID_MP_PLAYER_CONFIG = 0x106a,
	MOBY_ID_TURBOSLIDER = 0x107c,
	MOBY_ID_HOVERSHIP = 0x107e,
	MOBY_ID_TANK = 0x107f,
	MOBY_ID_WEAPON_BLITZ_CANNON = 0x1087,
	MOBY_ID_WEAPON_MINI_ROCKET_TUBE = 0x1089,
	MOBY_ID_WEAPON_GRAVITY_BOMB = 0x108a,
	MOBY_ID_SHOT_MINI_ROCKET_TUBE = 0x108c,
	MOBY_ID_SHOT_GRAVITY_BOMB1 = 0x108e,
	MOBY_ID_SHOT_GRAVITY_BOMB2 = 0x108f,
	MOBY_ID_SHOT_GRAVITY_BOMB3 = 0x1090,
	MOBY_ID_WEAPON_N60_STORM = 0x1094,
	MOBY_ID_WEAPON_FLUX_RIFLE = 0x1096,
	MOBY_ID_SHOT_FLUX_RIFLE = 0x1097,
	MOBY_ID_WEAPON_MINE = 0x1098,
	MOBY_ID_SHOT_MINE = 0x1099,
	MOBY_ID_BALL_BOT = 0x109a,
	MOBY_ID_DRONE_BOT = 0x109d,
	MOBY_ID_RANGER_TORSO = 0x109e,
	MOBY_ID_RANGER_FEET = 0x109f,
	MOBY_ID_WEAPON_LAVA_GUN = 0x10a4,
	MOBY_ID_WEAPON_UNKNOWN_1 = 0x10a5,
	MOBY_ID_WEAPON_MORPH_0_RAY = 0x10a7,
	MOBY_ID_WEAPON_HOLOSHIELD = 0x10a9,
	MOBY_ID_MORPH_PLAYER_CONFIG = 0x10ad,
	MOBY_ID_PLAYER_TURRET = 0x10c2,
	MOBY_ID_TELEPORT_PAD = 0x10c3,
	MOBY_ID_JUMP_PAD = 0x10c4,
	MOBY_ID_BLACK_SQUARE = 0x10c6,
	MOBY_ID_OBANI_BEAM_DIRECTOR = 0x11c1,
	MOBY_ID_AMMO_PAD = 0x1388,
	MOBY_ID_SMALL_NODE_TURRET = 0x15ca,
	MOBY_ID_HEALTH_PAD = 0x15ee,
	MOBY_ID_DYNAMO_PLATFORM = 0x1607,
	MOBY_ID_HOVEN_BRIDGE_BREAKABLE1 = 0x1632,
	MOBY_ID_HOVEN_BRIDGE_BREAKABLE2 = 0x1633,
	MOBY_ID_HOVEN_BRIDGE_BREAKABLE3 = 0x1634,
	MOBY_ID_HOVEN_BRIDGE_BREAKABLE4 = 0x1635,
	MOBY_ID_HOVEN_BRIDGE_BREAKABLE5 = 0x1636,
	MOBY_ID_HOVEN_BRIDGE_BREAKABLE6 = 0x1637,
	MOBY_ID_HOVEN_BRIDGE_BREAKABLE7 = 0x1638,
	MOBY_ID_HOVEN_BRIDGE_BREAKABLE8 = 0x1639,
	MOBY_ID_STARSHIP_TERMINAL = 0x163a,
	MOBY_ID_DRONE_BOT_CLUSTER_CONFIG = 0x166F,
	MOBY_ID_HOVEN_GLASS = 0x1672,
	MOBY_ID_SKIN_SNOWMAN = 0x16db,
	MOBY_ID_SKIN_RATCHET = 0x171c,
	MOBY_ID_DRONE_BOT_CLUSTER_UPDATER = 0x173d,
	MOBY_ID_REFRACTOR_LENS = 0x1860,
	MOBY_ID_AMMO_PACK_GRAVITY_BOMB = 0x189c,
	MOBY_ID_AMMO_PACK_BLITZ = 0x189d,
	MOBY_ID_AMMO_PACK_FLUX = 0x189e,
	MOBY_ID_AMMO_PACK_ROCKET_TUBE = 0x189f,
	MOBY_ID_WEAPON_WRENCH = 0x18ab,
	MOBY_ID_VR_GREEN_SHOCK_FIELD = 0x18c0,
	MOBY_ID_VR_GRAY_PLATFORM = 0x18c9,
	MOBY_ID_VR_BLUE_PLATFORM = 0x18ca,
	MOBY_ID_SKIN_ROBO = 0x18cb,
	MOBY_ID_SKIN_THUG = 0x18cc,
	MOBY_ID_SKIN_THYRANNOID = 0x18cf,
	MOBY_ID_GADGET_SWINGSHOT = 0x18df,
	MOBY_ID_GAME_SYSTEM = 0x18e2,
	MOBY_ID_BRIDGE_TRANSPORT = 0x18e3,
	MOBY_ID_VR_SPAWNING_CHAMBER = 0x18ec,
	MOBY_ID_STARSHIP_DOOR = 0x1910,
	MOBY_ID_SHARSHIP_GLASS_PLATFORM_SMALL = 0x195f,
	MOBY_ID_STARSHIP_GLASS_PLATFORM_LARGE = 0x1961,
	MOBY_ID_SASHA = 0x1965,
	MOBY_ID_RANGER = 0x1975,
	MOBY_ID_HEAD_COMMANDER_CHAIR = 0x1976,
	MOBY_ID_VEHICLE_PAD = 0x1981,
	MOBY_ID_MINI_DRONES_PLATFORM = 0x1983,
	MOBY_ID_VR_SIMULATOR = 0x1987,
	MOBY_ID_SPARKS = 0x19a0,
	MOBY_ID_OUTPOST_X12_BRIDGE_NO_PVAR = 0x19a2,
	MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_SIDE = 0x19af,
	MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_CENTER = 0x19b2,
	MOBY_ID_BLACK_SQUARE2 = 0x19ba,
	MOBY_ID_SWINGSHOT_ORB = 0x19c4,
	MOBY_ID_TELEPORTER_SP = 0x19cd,
	MOBY_ID_SIEGE_NODE_2 = 0x19de,
	MOBY_ID_ROLLING_DRONE_SPAWNER = 0x19f6,
	MOBY_ID_SHOCK_DROID = 0x1a1b,
	MOBY_ID_BOLT_CRANK = 0x1a27,
	MOBY_ID_DESTRUCTABLE_BRIDGE_CENTER = 0x1a29,
	MOBY_ID_SIEGE_NODE = 0x1a47,
	MOBY_ID_NODE_TURRET = 0x1a63,
	MOBY_ID_BLUE_FORCE_FIELD = 0x1a77,
	MOBY_ID_SHOCK_DROID_SPAWNER = 0x1a91,
	MOBY_ID_CRATE_GRAVITY_BOMB = 0x1aba,
	MOBY_ID_CRATE_ROCKET_TUBE = 0x1abb,
	MOBY_ID_CRATE_FLUX = 0x1abc,
	MOBY_ID_CRATE_BLITZ = 0x1abd,
	MOBY_ID_CRATE_LAVA_GUN = 0x1abe,
	MOBY_ID_CRATE_HOLOSHIELD = 0x1abf,
	MOBY_ID_CRATE_MORPH_O_RAY = 0x1ac0,
	MOBY_ID_CRATE_MINE = 0x1ac1,
	MOBY_ID_GATLING_TURRET = 0x1ad6,
	MOBY_ID_SKIN_NINJA = 0x1adc,
	MOBY_ID_HACKER_TERMINAL = 0x1ae0,
	MOBY_ID_OBANI_GLASS_CYLINDER = 0x1ae1,
	MOBY_ID_OBANI_ATMOSPHERE_CLOUD = 0x1ae5,
	MOBY_ID_HEALTH_BOX_MP = 0x1af2,
	MOBY_ID_HEALTH_ORB_MP = 0x1af3,
	MOBY_ID_BASE_GATTLING_TURRET_ANTENNA_COLLISION = 0x1af9,
	MOBY_ID_BASE_GATTLING_TURRET_HOLDER_1_COLLISION = 0x1afa,
	MOBY_ID_BASE_CATTLING_TURRET_HOLDER_2_COLLISION = 0x1afb,
	MOBY_ID_WEAPON_PACK = 0x1afc,
	MOBY_ID_AMMO_PACK_MINE = 0x1b02,
	MOBY_ID_AMMO_PACK_LAVA_GUN = 0x1b03,
	MOBY_ID_AMMO_PACK_HOLOSIELD = 0x1b04,
	MOBY_ID_AMMO_PACK_N60 = 0x1b05,
	MOBY_ID_CRATE_CHARGEBOOTS = 0x1b09,
	MOBY_ID_CHARGEBOOTS_PICKUP = 0x1b0c,
	MOBY_ID_KORGON_BRIDGE_PIECE = 0x1b23,
	MOBY_ID_GATLING_TURRET_SHOT = 0x1b35,
	MOBY_ID_OMNI_SHIELD = 0x1b37,
	MOBY_ID_GLASS_CRICLE_PLATFORM = 0x1b3b,
	MOBY_ID_OBANI_BRIDGE_FRAME = 0x1b43,
	MOBY_ID_HELGA = 0x1b66,
	MOBY_ID_SHOCK_DROID_SP = 0x1b6C,
	MOBY_ID_RANGER_AND_SMALL_TURRET_SHOT = 0x1b79,
	MOBY_ID_OBANI_ASTEROID = 0x1b92,
	MOBY_ID_OBANI_SATELITE = 0x1ba8,
	MOBY_ID_ANIT_VEHICLE_TURRET = 0x1bbd,
	MOBY_ID_MORPH_DUCK = 0x1bc3,
	MOBY_ID_BIG_AL = 0x1bd1,
	MOBY_ID_OBANI_METAL_BRIDGE = 0x1bea,
	MOBY_ID_OBANI_SATELITE_BREAKABLE = 0x1bed,
	MOBY_ID_OBANI_TROOPER_ENEMY = 0x1bf6,
	MOBY_ID_OBAMI_GLASS_DOME = 0x1bf8,
	MOBY_ID_MACHINE_GUN_ENEMY = 0x1c08,
	MOBY_ID_TEST = 0x1c0d,
	MOBY_ID_SKID_MCMARX = 0x1c12,
	MOBY_ID_OBANI_SKID_MCMARX = 0x1c29,
	MOBY_ID_CTF_RED_FLAG = 0x1c2f,
	MOBY_ID_CTF_BLUE_FLAG = 0x1c31,
	MOBY_ID_OBANI_GREEN_FORCE_FIELD = 0x1c41,
	MOBY_ID_BASE_COMPUTER = 0x1c57,
	MOBY_ID_BLUE_DECORATIVE_CRATE = 0x1c7f,
	MOBY_ID_FORCE_FIELD = 0x1cf7,
	MOBY_ID_SKIN_GLADIOLA = 0x1cfb,
	MOBY_ID_SKIN_BRUISER = 0x1cfc,
	MOBY_ID_SKIN_ROBO_ROOSTER = 0x1cfd,
	MOBY_ID_SKIN_BUGINOID = 0x1cfe,
	MOBY_ID_RATCHET_TROPHY = 0x1d4d,
	MOBY_ID_ARMOR_VENDOR = 0x1d7b,
	MOBY_ID_SHIP_PLATFORM = 0x1d99,
	MOBY_ID_SKIN_HOTBOT = 0x1e07,
	MOBY_ID_HOVEN_BREAKABLE_WALL_DECORATION = 0x1e41,
	MOBY_ID_BLACKWATER_CITY_DESTRUCTABLE_BRIDGE_CENTER = 0x1e1f,
	MOBY_ID_BLACKWATER_CITY_DESTRUCTABLE_BRIDGE_SIDE = 0x1e20,
	MOBY_ID_SKIN_BRANIUS = 0x1e4c,
	MOBY_ID_SKIN_BONES = 0x1e4d,
	MOBY_ID_SKIN_TROOPER = 0x1e59,
	MOBY_ID_SKIN_SKRUNCH = 0x1e65,
	MOBY_ID_SKIN_GRAY = 0x1e68,
	MOBY_ID_SKIN_NEFARIOUS = 0x1e69,
	MOBY_ID_SKRUNCH_NPC = 0x1e73,
	MOBY_ID_SKIN_EVIL_CLOWN_RATCHET = 0x1e77,
	MOBY_ID_SKIN_BEACH_BUNNY_RATCHET = 0x1e78,
	MOBY_ID_SKIN_CONSTRUCTOBOT = 0x1e7a,
	MOBY_ID_SKIN_DAN = 0x1e83,
	MOBY_ID_FLOOR_LIGHT = 0x1e96,
	MOBY_ID_TANK_PIPE_BREAKABLE = 0x1e97,
	MOBY_ID_TANK_PIPE_BROKEN = 0x1e98,
	MOBY_ID_CONSOLE_COMPUTER_SMALL = 0x1e99,
	MOBY_ID_COMPUTER_MONITOR = 0x1e9c,
	MOBY_ID_TANK_PIPE_DECORATION = 0x1ea3,
	MOBY_ID_COMPUTER_DESK = 0x1ecf,
	MOBY_ID_METAL_SCREW_MUSHROOM = 0x1ed4,
	MOBY_ID_METAL_FLOWER = 0x1ed6,
	MOBY_ID_METAL_LIGHT = 0x1edc,
	MOBY_ID_OBANI_BRIDGE_GLASS = 0x1ee1,
	MOBY_ID_CRATE_RANDOM_PICKUP = 0x1f1e,
	MOBY_ID_OBANI_ENERGY_CLOUD = 0x1f8b,
};

enum MobyModeBit
{
	MOBY_MODE_BIT_NONE =                  (0x0000),
	MOBY_MODE_BIT_DISABLED =              (0x0001),
	MOBY_MODE_BIT_NO_UPDATE =             (0x0002),
	MOBY_MODE_BIT_NO_POST_UPDATE =        (0x0004),
	MOBY_MODE_BIT_TRANSPARENT =           (0x0008),
	MOBY_MODE_BIT_HAS_GLOW =              (0x0010),
	MOBY_MODE_BIT_HAS_SPECIAL_VARS =      (0x0020),
	MOBY_MODE_BIT_UNK_40 =                (0x0040),
	MOBY_MODE_BIT_HIDDEN =                (0x0080),
	MOBY_MODE_BIT_LOCK_ROTATION =         (0x0100),
	MOBY_MODE_BIT_DRAW_TRANSPARENT_WEIRD =(0x0200),
	MOBY_MODE_BIT_DRAW_SHADOW =           (0x0400),
	MOBY_MODE_BIT_DISABLE_Z_WRITE =       (0x0800),
	MOBY_MODE_BIT_CAN_BE_AUTO_TARGETED =  (0x1000),
	MOBY_MODE_BIT_HIDE_BACKFACES =        (0x2000),
	MOBY_MODE_BIT_CAN_BE_DAMAGED =        (0x4000),
	MOBY_MODE_BIT_MIRROR =                (0x8000),
};

struct MobyReactAnim { // 0x20
	/* 0x00 */ char animType;
	/* 0x01 */ char startFrame;
	/* 0x02 */ char endFrame;
	/* 0x03 */ char cpad;
	/* 0x04 */ float drag_mps;
	/* 0x08 */ float xySpeed_mps;
	/* 0x0c */ float zSpeed_mps;
	/* 0x10 */ float upGrav_mps;
	/* 0x14 */ float downGrav_mps;
	/* 0x18 */ float peakFrame;
	/* 0x1c */ float landFrame;
};

struct MoveAnimData { // 0x20
	/* 0x00 */ char elv_state;
	/* 0x01 */ char alert_state;
	/* 0x02 */ char turning;
	/* 0x03 */ char motivation_state;
	/* 0x04 */ char motivation_dir;
	/* 0x05 */ char action_state;
	/* 0x06 */ char reaction_state;
	/* 0x07 */ char animGroup;
	/* 0x08 */ int flags;
	/* 0x0c */ float linear_rate;
	/* 0x10 */ float angular_rate;
	/* 0x14 */ float trigger_frame;
	/* 0x18 */ int pad[2];
};

struct MobyAnimInfo { // 0x8
	/* 0x0 */ struct MobyReactAnim *reactData;
	/* 0x4 */ struct MoveAnimData *auxData;
};

struct MobySeq { // 0x20
	/* 0x00 */ VECTOR bSphere;
	/* 0x10 */ char frameCnt;
	/* 0x11 */ signed char sound;
	/* 0x12 */ char trigsCnt;
	/* 0x13 */ char pad;
	/* 0x14 */ short int *trigs;
	/* 0x18 */ struct MobyAnimInfo *animInfo;
	/* 0x1c */ void *frameData;
};

struct Manipulator { // 0x40
	/* 0x00 */ char animJoint;
	/* 0x01 */ char state;
	/* 0x02 */ char scaleOn;
	/* 0x03 */ char absolute;
	/* 0x04 */ int jointId;
	/* 0x08 */ struct Manipulator *pChain;
	/* 0x0c */ float interp;
	/* 0x10 */ s128 q;
	/* 0x20 */ VECTOR scale;
	/* 0x30 */ VECTOR trans;
};

struct MobyAnimLayer { // 0x20
	/* 0x00 */ struct MobySeq *animSeq;
	/* 0x04 */ float animSeqT;
	/* 0x08 */ float animSpeed;
	/* 0x0c */ short int animIScale;
	/* 0x0e */ short int poseCacheEntryIndex;
	/* 0x10 */ struct MobyAnimLayer *nextLayer;
	/* 0x14 */ char animSeqId;
	/* 0x15 */ char animFlags;
	/* 0x16 */ char animJointIndexA;
	/* 0x17 */ char animJointIndexB;
	/* 0x18 */ float blendWeight;
	/* 0x1c */ float blendFade;
};

typedef struct Moby {
	/* 0x00 */ VECTOR bSphere;
	/* 0x10 */ VECTOR position;
	/* 0x20 */ char state;
	/* 0x21 */ u8 group;
	/* 0x22 */ char mClass;
	/* 0x23 */ u8 opacity;
	/* 0x24 */ void * pClass;
	/* 0x28 */ struct Moby * pChain;
	/* 0x2c */ float scale;
	/* 0x30 */ char updateDist;
    /* 0x31 */ char drawn;
    /* 0x32 */ short drawDist;
	/* 0x34 */ u16 modeBits;
	/* 0x36 */ u16 modeBits2;
	/* 0x38 */ u32 lights;
	/* 0x3c */ u32 primaryColor;
	/*      */ char unk_40[0x24];
	/* 0x64 */ void * pUpdate;
	/* 0x68 */ void * pVar;
	/*      */ char unk_6c[0x2];
	/* 0x6e */ char shadow;
    /* 0x6f */ char shadowIndex;
    /* 0x70 */ float shadowPlane;
    /* 0x74 */ float shadowRange;
	/*      */ char unk_78[0x6];
	/* 0x7e */ short color;
	/* 0x80 */ VECTOR lSphere;
	/* 0x90 */ struct GuberMoby * guberMoby;
	/*      */ char unk_94[0x14];
	/* 0xa8 */ char triggers;
    /* 0xa9 */ char standardDeathCalled;
	/* 0xaa */ short oClass;
	/*      */ char unk_ac[0xc];
	/* 0xb8 */ struct Moby * pParent;
	/* 0xbc */ char unk_bc[0x4];
    /* 0xc0 */ mtx3 rMtx;
    /* 0xf0 */ VECTOR rotation;
} Moby;

typedef struct MobyColDamageIn {
    VECTOR Momentum;
    Moby * Damager;
    int DamageFlags;
    u8 DamageClass;
    u8 DamageStrength;
    u16 DamageIndex;
    float DamageHp;
    int Flags;
    float DamageHeroHp;
    int ShotUID;
    int UNK_2C;
} MobyColDamageIn;

typedef struct MobyColDamage {
    VECTOR Ip;
    VECTOR Momentum;
    Moby * Damager;
    int DamageFlags;
    u8 DamageClass;
    u8 DamageStrength;
    u16 DamageIndex;
    float DamageHp;
    int Flags;
    float DamageHeroHp;
    int ShotUID;
    Moby * Moby;
} MobyColDamage;


enum FlashTypes {
	FT_NONE = 0,
	FT_HIT = 1,
	FT_SELECT = 2,
	FT_PULSE = 3,
	FT_BLINK = 4,
	FT_SOLID = 5,
	FT_SLOWFADE = 6
};

struct FlashVars {
	/*   0 */ short int timer;
	/*   2 */ short int type;
	/*   4 */ int destColor;
	/*   8 */ int srcColor;
	/*   c */ int flags;
};

struct TargetVars {
	/*   0 */ float hitPoints;
	/*   4 */ int maxHitPoints;
	/*   8 */ unsigned char attackDamage[6];
	/*   e */ short int hitCount;
	/*  10 */ int flags;
	/*  14 */ float targetHeight;
	/*  18 */ Moby * mobyThatHurtMeLast;
	/*  1c */ float camPushDist;
	/*  20 */ float camPushHeight;
	/*  24 */ short int damageCounter;
	/*  26 */ short int empTimer;
	/*  28 */ short int infectedTimer;
	/*  2a */ short int invincTimer;
	/*  2c */ short int bogeyType;
	/*  2e */ short int team;
	/*  30 */ char lookAtMeDist;
	/*  31 */ char lookAtMePriority;
	/*  32 */ char lookAtMeZOfsIn8ths;
	/*  33 */ char lookAtMeJoint;
	/*  34 */ char lookAtMeExpression;
	/*  35 */ char lockOnPriority;
	/*  36 */ char soundType;
	/*  37 */ char targetRadiusIn8ths;
	/*  38 */ char noAutoTrack;
	/*  39 */ char trackSpeedInMps;
	/*  3a */ char camModOverride;
	/*  3b */ char destroyMe;
	/*  3c */ char morphoraySpecial;
	/*  3d */ char headJoint;
	/*  3e */ char hitByContinuous;
	/*  3f */ char infected;
	/*  40 */ char empFxTimer;
	/*  41 */ char weaponTargetedOnMe;
	/*  42 */ char isOrganic;
	/*  43 */ signed char bundleIndex;
	/*  44 */ char bundleDamage;
	/*  45 */ char firedAt;
	/*  46 */ char weaponThatHurtMeLast;
	/*  47 */ char invalidTarget;
	/*  48 */ int maxDifficultySlotted;
	/*  4c */ int curDifficultySlotted;
	/*  50 */ Moby * pTargettedByBogeys[8];
	/*  70 */ Moby * mobyThatFiredAtMe;
	/*  74 */ int targetShadowMask;
	/*  78 */ int damageTypes;
	/*  7c */ int padA;
	/*  80 */ float morphDamage;
	/*  84 */ float freezeDamage;
	/*  88 */ float infectDamage;
	/*  8c */ float lastDamage;
};

struct MoveVarsAnimCache {
	/*   0 */ long unsigned int actCache[32];
	/* 100 */ long unsigned int reactCache[7];
	/* 138 */ long unsigned int elvCache[5];
	/* 160 */ long unsigned int motCache[3];
	/* 178 */ long unsigned int dirCache[4];
	/* 198 */ long unsigned int grpCache[33];
	/* 2a0 */ long unsigned int alertCache[4];
};

struct MoveVars_V2 {
	/*   0 */ int flags;
	/*   4 */ int internalFlags;
	/*   8 */ int effectorFlags;
	/*   c */ int dirty;
	/*  10 */ float maxStepUp;
	/*  14 */ float maxStepDown;
	/*  18 */ int avoidHotspots;
	/*  1c */ int passThruHotspots;
	/*  20 */ short int arrestedTimer;
	/*  22 */ short int lostTimer;
	/*  24 */ float gravity;
	/*  28 */ float slopeLimit;
	/*  2c */ float maxFlightAngle;
	/*  30 */ char elv_state;
	/*  31 */ char alert_state;
	/*  32 */ char reaction_state;
	/*  33 */ char action_state;
	/*  34 */ char blend;
	/*  35 */ char lockAnim;
	/*  36 */ short int numColl;
	/*  38 */ struct MoveVarsAnimCache * pAnimCache;
	/*  3c */ struct MoveVarsAnimCache * pAttachAnimCache;
	/*  40 */ Moby ** effectorOverrideList;
	/*  44 */ int effectorOverrideCount;
	/*  48 */ int boundArea;
	/*  4c */ Moby * pIgnoreCollMoby;
	/*  50 */ Moby* pBumpMoby;
	/*  54 */ Moby * pGroundMoby;
	/*  58 */ Moby * pIgnoreEffector;
	/*  5c */ Moby * pAttach;
	/*  60 */ int attachJoint;
	/*  64 */ float attachMaxRot;
	/*  68 */ float actionStartFrame;
	/*  6c */ void* pActionCallback;
	/*  70 */ int lastUpdateFrame;
	/*  74 */ int animGroups;
	/*  78 */ float collRadius;
	/*  7c */ float gravityVel;
	/*  80 */ float swarmOfsAmp;
	/*  84 */ int swarmOfsTimer;
	/*  88 */ int swarmOfsMinTime;
	/*  8c */ int swarmOfsMaxTime;
	/*  90 */ float stopDist;
	/*  94 */ float walkDist;
	/*  98 */ float runDist;
	/*  9c */ float walkSpeed;
	/*  a0 */ float runSpeed;
	/*  a4 */ float strafeSpeed;
	/*  a8 */ float backSpeed;
	/*  ac */ float flySpeed;
	/*  b0 */ float linearAccel;
	/*  b4 */ float linearDecel;
	/*  b8 */ float linearLimit;
	/*  bc */ float linearSpeed;
	/*  c0 */ float angularAccel;
	/*  c4 */ float angularDecel;
	/*  c8 */ float angularLimit;
	/*  cc */ float hitGroundSpeed;
	/*  d0 */ float legFacing;
	/*  d4 */ float bodyFacing;
	/*  d8 */ float legAngularSpeed;
	/*  dc */ float bodyAngularSpeed;
	/*  e0 */ float groundSlope;
	/*  e4 */ float groundZ;
	/*  e8 */ int groundHotspot;
	/*  ec */ int groundCheckFrame;
	/*  f0 */ int onGround;
	/*  f4 */ int offGround;
	/*  f8 */ float passThruSurface;
	/*  fc */ int passThruSurfaceType;
	/* 100 */ float projectedLandingZ;
	/* 104 */ float moveDamper;
	/* 108 */ short int moveDamperTimer;
	/* 10a */ char curNode;
	/* 10b */ char destNode;
	/* 10c */ struct Path * pLastFollowPath;
	/* 110 */ float walkTurnFactor;
	/* 114 */ float desiredFacing;
	/* 120 */ VECTOR vel;
	/* 130 */ VECTOR arrestedPos;
	/* 140 */ VECTOR groundNormal;
	/* 150 */ VECTOR jumpVel;
	/* 160 */ VECTOR target;
	/* 170 */ VECTOR passThruPoint;
	/* 180 */ VECTOR passThruNormal;
	/* 190 */ VECTOR waypoint;
	/* 1a0 */ u64 groupCache;
	/* 1a8 */ u64 attachGroupCache;
};

typedef void (*MobyGetInterface_func)(int mobyId, int arg2, int arg3);
typedef void (*MobyGetGuberObject_func)(Moby * moby);
typedef void (*MobyEventHandler_func)(Moby * moby, struct GuberEvent * event);
typedef void (*MobyGetDamager_func)(Moby * moby);

typedef struct MobyFunctions
{
    void * FUNC_00;
    MobyGetGuberObject_func GetGuberObject;
    void * FUNC_08;
    void * FUNC_0C;
    MobyGetInterface_func GetMobyInterface;
    MobyEventHandler_func MobyEventHandler;
    void * FUNC_18;
    void * FUNC_1C;
	MobyGetDamager_func GetDamager;
} MobyFunctions;

typedef struct ShieldVars {
	u32 mainColor;
	u32 unk_color_04;
	u32 unk_color_08;
	u32 unk_color_0C;
	u32 lightningColor;
	u32 unk_color_14;
	int texture;
	float mainScale;
	float outerCircleScale;
	float outerCircleGlowRadius;
	u32 textureColor;
	int pad_2c;
	float animScale;
	u32 animBallColor;
	u32 unk_color_38;
	u32 animBallTrailColor;
	float animBallScale;
	float animBallTrailScale;
	int unk_texture_48;
	int unk_texture_4c;
	float unk_scale_50;
	float unk_scale_54;
	float lightningScale;
	u32 lightningColor2;
	int flashTime;
	int pad[3];
} ShieldVars;

typedef struct AmmoPickupVars { // 0x30
/* 0x00 */ struct Moby* pParent;
/* 0x04 */ int unk_04;
/* 0x08 */ int unk_08;
/* 0x0c */ int unk_0c;
/* 0x10 */ int ammoType;
/* 0x14 */ int respawnTime;
/* 0x18 */ int lastPickupTime;
/* 0x1c */ int unk_1c;
/* 0x20 */ int unk_ARGB_20;
/* 0x24 */ int unk_ARGB_24;
/* 0x28 */ int particleColor[2];
} AmmoPickupVars_t;

/*
 * Spawns a moby with the given id and properties size.
 */
Moby * mobySpawn(int id, int propSize);

/*
 * Returns pointer to the start of the moby list.
 */
__LIBUYA_GETTER__ Moby * mobyListGetStart(void);

/*
 * Returns pointer to the end of the moby list.
 */
__LIBUYA_GETTER__ Moby * mobyListGetEnd(void);

/*
 */
__LIBUYA_GETTER__ u16 * mobyGetLoadedMobyClassList(void);

/*
 * Returns non-zero if the given moby is destroyed.
 */
__LIBUYA_GETTER__ int mobyIsDestroyed(Moby * moby);

/*
 * Returns a pointer to the next living moby of the given oclass.
 * Returns NULL if none found.
 */
Moby * mobyFindNextByOClass(Moby * start, int oClass);

/*
 * Returns non-zero if the given o class is loaded in the map.
 */
// int mobyClassIsLoaded(int oClass);

/*
 * Destroys the given moby.
 */
void mobyDestroy(Moby * moby);

/*
 * Gets a pointer to the moby functions
 */
MobyFunctions * mobyGetFunctions(Moby * moby);

/*
 */
// int mobyGetNumSpawnableMobys(void);

/*
 */
// int mobyIsHero(Moby* moby);

/*
 * NAME :		mobyGetDamager
 * DESCRIPTION :
 * 				Returns damage value of given moby.
 * NOTES :
 * ARGS : 
 *     			moby: moby damage value.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int mobyGetDamager(Moby * moby);

Moby* mobyGetByGuberUid(u32 uid);


/*
 * NAME :		mobyGetShieldVars
 * DESCRIPTION :
 * 				Returns Shield Vars so you can edit them.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
ShieldVars* mobyGetShieldVars(void);

/*
 * NAME :		flagIsOnSafeGround
 * DESCRIPTION :
 * 				Returns zero if not on safe ground. -1: default return.
 * NOTES :
 * ARGS : 		Moby* flagMoby
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int flagIsOnSafeGround(Moby* flagMoby);

int flagIsReturning(Moby* flagMoby);
int flagIsBeingPickedUp(Moby* flagMoby);
int flagIsOnSafeGround(Moby* flagMoby);
int flagIsAtBase(Moby* flagMoby);
void flagReturnToBase(Moby* flagMoby, int a0, char returnedByPlayerIdx);
void flagPickup(Moby* flagMoby, int playerIdx);
void flagUpdate(Moby*, int a1);

#endif // _LIBUYA_MOBY_H_
