/***************************************************
 * FILENAME :		moby.h
 * 
 * DESCRIPTION :
 * 		Contains moby specific offsets and structures for Deadlocked.
 * 
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_MOBY_H_
#define _LIBUYA_MOBY_H_

#include <tamtypes.h>
#include "math3d.h"
#include "common.h"
#include "gid.h"

struct GuberMoby;
struct GuberEvent;
struct Gid;

//--------------------------------------------------------
enum MobyId
{
	MODEL_ID_PLACE_HOLDER = 0x0000,
	MODEL_ID_WEAPON_VENDER = 0x000b,
	MODEL_ID_BOLT_CRATE = 0x01f4,
	MODEL_ID_HEALTH_CRATE_SP = 0x01f5,
	MODEL_ID_BONUS_BOLTS_CRATE = 0x01f7,
	MODEL_ID_EXPLOSIVE_CRATE = 0x01f9,
	MODEL_ID_AMMO_CRATE = 0x01ff,
	MODEL_ID_TITANIUM_BOLT = 0x046e,
	MODEL_ID_DYNAMO_CONTROL_NODE = 0x0952,
	MODEL_ID_OBANI_JUMP_PAD = 0x09b9,
	MODEL_ID_WATER = 0x0b37,
	MODEL_ID_TELEPORT_PAD_SP = 0x0c60,
	MODEL_ID_LEVEL_MODEL_ANIMATION = 0x0C66,
	MODEL_ID_LEVEL_MODEL_TEXTURE_ANIMATION = 0x0CE9,
	MODEL_ID_BASE_LIGHT = 0x0cbe,
	MODEL_ID_JUMP_PAD_SP = 0x0f05,
	MODEL_ID_TELEPORT_PAD_RC1 = 0x0f51,
	MODEL_ID_MP_PLAYER_CONFIG = 0x106a,
	MODEL_ID_TURBOSLIDER = 0x107c,
	MODEL_ID_HOVERSHIP = 0x107e,
	MODEL_ID_TANK = 0x107f,
	MODEL_ID_WEAPON_GRAVITY_BOMB = 0x108a,
	MODEL_ID_SHOT_GRAVITY_BOMB1 = 0x108e,
	MODEL_ID_SHOT_GRAVITY_BOMB2 = 0x1090,
	MODEL_ID_BALL_BOT = 0x109a,
	MODEL_ID_DRONE_BOT = 0x109d,
	MODEL_ID_RANGER_TORSO = 0x109e,
	MODEL_ID_RANGER_FEET = 0x109f,
	MODEL_ID_WEAPON_LAVA_GUN = 0x10a4,
	MODEL_ID_WEAPON_MORPH_0_RAY = 0x10a7,
	MODEL_ID_WEAPON_HOLOSHIELD = 0x10a9,
	MODEL_ID_TELEPORT_PAD = 0x10c3,
	MODEL_ID_PLAYER_TURRET = 0x10c2,
	MODEL_ID_JUMP_PAD = 0x10c4,
	MODEL_ID_BLACK_SQUARE = 0x10c6,
	MODEL_ID_OBANI_BEAM_DIRECTOR = 0x11c1,
	MODEL_ID_AMMO_PAD = 0x1388,
	MODEL_ID_SMALL_NODE_TURRET = 0x15ca,
	MODEL_ID_HEALTH_PAD = 0x15ee,
	MODEL_ID_DYNAMO_PLATFORM = 0x1607,
	MODEL_ID_HOVEN_BRIDGE_BREAKABLE1 = 0x1632,
	MODEL_ID_HOVEN_BRIDGE_BREAKABLE2 = 0x1633,
	MODEL_ID_HOVEN_BRIDGE_BREAKABLE3 = 0x1634,
	MODEL_ID_HOVEN_BRIDGE_BREAKABLE4 = 0x1635,
	MODEL_ID_HOVEN_BRIDGE_BREAKABLE5 = 0x1636,
	MODEL_ID_HOVEN_BRIDGE_BREAKABLE6 = 0x1637,
	MODEL_ID_HOVEN_BRIDGE_BREAKABLE7 = 0x1638,
	MODEL_ID_HOVEN_BRIDGE_BREAKABLE8 = 0x1639,
	MODEL_ID_STARSHIP_TERMINAL = 0x163a,
	MODEL_ID_DRONE_BOT_CLUSTER_CONFIG = 0x166F,
	MODEL_ID_HOVEN_GLASS = 0x1672,
	MODEL_ID_SKIN_SNOWMAN = 0x16db,
	MODEL_ID_SKIN_RATCHET = 0x171c,
	MODEL_ID_REFRACTOR_LENS = 0x1860,
	MODEL_ID_AMMO_PACK_GRAVITY_BOMB = 0x189c,
	MODEL_ID_AMMO_PACK_BLITZ = 0x189d,
	MODEL_ID_AMMO_PACK_FLUX = 0x189e,
	MODEL_ID_AMMO_PACK_ROCKET_TUBE = 0x189f,
	MODEL_ID_VR_GREEN_SHOCK_FIELD = 0x18c0,
	MODEL_ID_VR_GRAY_PLATFORM = 0x18c9,
	MODEL_ID_VR_BLUE_PLATFORM = 0x18ca,
	MODEL_ID_SKIN_ROBO = 0x18cb,
	MODEL_ID_SKIN_THUG = 0x18cc,
	MODEL_ID_SKIN_THYRANNOID = 0x18cf,
	MODEL_ID_GAME_SYSTEM = 0x18e2,
	MODEL_ID_BRIDGE_TRANSPORT = 0x18e3,
	MODEL_ID_VR_SPAWNING_CHAMBER = 0x18ec,
	MODEL_ID_STARSHIP_DOOR = 0x1910,
	MODEL_ID_SHARSHIP_GLASS_PLATFORM_SMALL = 0x195f,
	MODEL_ID_STARSHIP_GLASS_PLATFORM_LARGE = 0x1961,
	MODEL_ID_RANGER = 0x1975,
	MODEL_ID_HEAD_COMMANDER_CHAIR = 0x1976,
	MODEL_ID_VEHICLE_PAD = 0x1981,
	MODEL_ID_MINI_DRONES_PLATFORM = 0x1983,
	MODEL_ID_VR_SIMULATOR = 0x1987,
	MODEL_ID_SPARKS = 0x19a0,
	MODEL_ID_DESTRUCTABLE_BRIDGE_SIDE = 0x19af,
	MODEL_ID_DESTRUCTABLE_BRIDGE_DESTROYED = 0x19b2,
	MODEL_ID_BLACK_SQUARE2 = 0x19ba,
	MODEL_ID_SWINGSHOT_ORB = 0x19c4,
	MODEL_ID_TELEPORTER_SP = 0x19cd,
	MODEL_ID_SIEGE_NODE_2 = 0x19de,
	MODEL_ID_ROLLING_DRONE_SPAWNER = 0x19f6,
	MODEL_ID_SHOCK_DROID = 0x1a1b,
	MODEL_ID_BOLT_CRANK = 0x1a27,
	MODEL_ID_DESTRUCTABLE_BRIDGE_CENTER = 0x1a29,
	MODEL_ID_SIEGE_NODE = 0x1a47,
	MODEL_ID_NODE_TURRET = 0x1a63,
	MODEL_ID_BLUE_FORCE_FIELD = 0x1a77,
	MODEL_ID_SHOCK_DROID_SPAWNER = 0x1a91,
	MODEL_ID_CRATE_GRAVITY_BOMB = 0x1aba,
	MODEL_ID_CRATE_ROCKET_TUBE = 0x1abb,
	MODEL_ID_CRATE_FLUX = 0x1abc,
	MODEL_ID_CRATE_BLITZ = 0x1abd,
	MODEL_ID_CRATE_LAVA_GUN = 0x1abe,
	MODEL_ID_CRATE_LAVA_GUN = 0x1abf,
	MODEL_ID_CRATE_MORPH_O_RAY = 0x1ac0,
	MODEL_ID_CRATE_MINE = 0x1ac1,
	MODEL_ID_GATTLING_TURRET = 0x1ad6,
	MODEL_ID_SKIN_NINJA = 0x1adc,
	MODEL_ID_HACKER_TERMINAL = 0x1ae0,
	MODEL_ID_OBANI_GLASS_CYLINDER = 0x1ae1,
	MODEL_ID_OBANI_ATMOSPHERE_CLOUD = 0x1ae5,
	MODEL_ID_HEALTH_BOX = 0x1af2,
	MODEL_ID_HEALTH_ORB = 0x1af3,
	MODEL_ID_BASE_GATTLING_TURRET_ANTENNA_COLLISION = 0x1af9,
	MODEL_ID_BASE_GATTLING_TURRET_HOLDER_1_COLLISION = 0x1afa,
	MODEL_ID_BASE_CATTLING_TURRET_HOLDER_2_COLLISION = 0x1afb,
	MODEL_ID_WEAPON_PACK = 0x1afc,
	MODEL_ID_AMMO_PACK_MINE = 0x1b02,
	MODEL_ID_AMMO_PACK_LAVA_GUN = 0x1b03,
	MODEL_ID_AMMO_PACK_HOLOSIELD = 0x1b04,
	MODEL_ID_AMMO_PACK_N60 = 0x1b05,
	MODEL_ID_CRATE_CHARGEBOOTS = 0x1b09,
	MODEL_ID_CHARGEBOOTS_PICKUP = 0x1b0c
	MODEL_ID_KORGON_BRIDGE_PIECE = 0x1b23,
	MODEL_ID_GLASS_CRICLE_PLATFORM = 0x1b3b,
	MODEL_ID_OBANI_BRIDGE_FRAME = 0x1b43,
	MODEL_ID_SASHA = 0x1965,
	MODEL_ID_HELGA = 0x1b66,
	MODEL_ID_SHOCK_DROID_SP = 0x1b6C,
	MODEL_ID_OBANI_ASTEROID = 0x1b92,
	MODEL_ID_OBANI_SATELITE = 0x1ba8,
	MODEL_ID_ANIT_VEHICLE_TURRET = 0x1bbd,
	MODEL_ID_MORPH_DUCK = 0x1bc3,
	MODEL_ID_BIG_AL = 0x1bd1,
	MODEL_ID_OBANI_METAL_BRIDGE = 0x1bea,
	MODEL_ID_OBANI_SATELITE_BREAKABLE = 0x1bed,
	MODEL_ID_OBANI_TROOPER_ENEMY = 0x1bf6,
	MODEL_ID_OBAMI_GLASS_DOME = 0x1bf8,
	MODEL_ID_MACHINE_GUN_ENEMY = 0x1c08,
	MODEL_ID_SKID_MCMARX = 0x1c12,
	MODEL_ID_OBANI_SKID_MCMARX = 0x1c29,
	MODEL_ID_CTF_RED_FLAG = 0x1c2f,
	MODEL_ID_CTF_BLUE_FLAG = 0x1c31,
	MODEL_ID_OBANI_GREEN_FORCE_FIELD = 0x1c41,
	MODEL_ID_BASE_COMPUTER = 0x1c57,
	MODEL_ID_BLUE_DECORATIVE_CRATE = 0x1c7f,
	MODEL_ID_FORCE_FIELD = 0x1cf7,
	MODEL_ID_SKIN_GLADIOLA = 0x1cfb,
	MODEL_ID_SKIN_BRUISER = 0x1cfc,
	MODEL_ID_SKIN_ROBO_ROOSTER = 0x1cfd,
	MODEL_ID_SKIN_BUGINOID = 0x1cfe,
	MODEL_ID_RATCHET_TROPHY = 0x1d4d,
	MODEL_ID_ARMOR_VENDOR = 0x1d7b,
	MODEL_ID_SHIP_PLATFORM = 0x1d99,
	MODEL_ID_SKIN_HOTBOT = 0x1e07,
	MODEL_ID_HOVEN_BREAKABLE_WALL_DECORATION = 0x1e41,
	MODEL_ID_SKIN_BRANIUS = 0x1e4c,
	MODEL_ID_SKIN_BONES = 0x1e4d,
	MODEL_ID_SKIN_TROOPER = 0x1e59,
	MODEL_ID_SKIN_SKRUNCH = 0x1e65,
	MODEL_ID_SKIN_GRAY = 0x1e68,
	MODEL_ID_SKIN_NEFARIOUS = 0x1e69,
	MODEL_ID_SKRUNCH_NPC = 0x1e73,
	MODEL_ID_SKIN_EVIL_CLOWN_RATCHET = 0x1e77,
	MODEL_ID_SKIN_BEACH_BUNNY_RATCHET = 0x1e78,
	MODEL_ID_SKIN_CONSTRUCTOBOT = 0x1e7a,
	MODEL_ID_SKIN_DAN = 0x1e83,
	MODEL_ID_FLOOR_LIGHT = 0x1e96,
	MODEL_ID_TANK_PIPE_BREAKABLE = 0x1e97,
	MODEL_ID_TANK_PIPE_BROKEN = 0x1e98,
	MODEL_ID_CONSOLE_COMPUTER_SMALL = 0x1e99,
	MODEL_ID_COMPUTER_MONITOR = 0x1e9c,
	MODEL_ID_TANK_PIPE_DECORATION = 0x1ea3,
	MODEL_ID_COMPUTER_DESK = 0x1ecf,
	MODEL_ID_METAL_SCREW_MUSHROOM = 0x1ed4,
	MODEL_ID_METAL_FLOWER = 0x1ed6,
	MODEL_ID_METAL_LIGHT = 0x1edc,
	MODEL_ID_OBANI_BRIDGE_GLASS = 0x1ee1,
	MODEL_ID_CRATE_RANDOM_PICKUP = 0x1f1e,
	MODEL_ID_OBANI_ENERGY_CLOUD = 0x1f8b,
};

/*
 * NAME :		Moby
 * 
 * DESCRIPTION :
 * 			Contains the moby struct data.
 * 
 * NOTES :
 *          Mobies are objects that can be spawned in game.
 *          This includes things like vehicles, turrets, mod pads, etc
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct Moby
{
	/*   0 */ VECTOR BSphere;
	/*  10 */ VECTOR Position;
	/*  20 */ char State;
	/*     */ char unk_21[0x47];
	/*  68 */ void * PVar;
	/*     */ char unk_6c[0x3c];
	/*  a8 */ char Triggers;
    /*  a9 */ char StandardDeathCalled;
	/*  aa */ short OClass;
	/*  ac */ char unk_ac[0x54];
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
    Moby* Damager;
    int DamageFlags;
    u8 DamageClass;
    u8 DamageStrength;
    u16 DamageIndex;
    float DamageHp;
    int Flags;
    float DamageHeroHp;
    int ShotUID;
    Moby* Moby;
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
	/*  18 */ Moby* mobyThatHurtMeLast;
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
	/*  50 */ Moby* pTargettedByBogeys[8];
	/*  70 */ Moby* mobyThatFiredAtMe;
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
	/*  38 */ struct MoveVarsAnimCache* pAnimCache;
	/*  3c */ struct MoveVarsAnimCache* pAttachAnimCache;
	/*  40 */ Moby** effectorOverrideList;
	/*  44 */ int effectorOverrideCount;
	/*  48 */ int boundArea;
	/*  4c */ Moby* pIgnoreCollMoby;
	/*  50 */ Moby* pBumpMoby;
	/*  54 */ Moby* pGroundMoby;
	/*  58 */ Moby* pIgnoreEffector;
	/*  5c */ Moby* pAttach;
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
	/* 10c */ struct Path* pLastFollowPath;
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

typedef struct MobyFunctions
{
    void * FUNC_00;
    MobyGetGuberObject_func GetGuberObject;
    void * FUNC_08;
    void * FUNC_0C;
    MobyGetInterface_func GetMobyInterface;
    MobyEventHandler_func MobyEventHandler;
    void * GetDamager;
    void * FUNC_1C;

} MobyFunctions;

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
 *
 */
__LIBUYA_GETTER__ u16 * mobyGetLoadedMobyClassList(void);

/*
 * Returns non-zero if the given moby is destroyed.
 */
__LIBUYA_GETTER__ int mobyIsDestroyed(Moby* moby);

/*
 * Returns a pointer to the next living moby of the given oclass.
 * Returns NULL if none found.
 */
Moby* mobyFindNextByOClass(Moby* start, int oClass);

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
// MobyFunctions * mobyGetFunctions(Moby * moby);

/*
 *
 */
// int mobyGetNumSpawnableMobys(void);

/*
 * 
 */
// int mobyIsHero(Moby* moby);

#endif // _LIBUYA_MOBY_H_
