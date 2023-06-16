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
    MOBY_ID_HEALTH_BOX_MULT = 0x1AF2,
    MOBY_ID_HEALTH_ORB_MULT = 0x1AF3,
    MOBY_ID_WEAPON_PACK = 0x1AFC,
    MOBY_ID_CHARGEBOOTS_PICKUP_MODEL = 0x1B0C,
    MOBY_ID_SWINGSHOT_ORB = 0x19C4,
    MOBY_ID_SKIN_RATCHET = 0x251C,
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
