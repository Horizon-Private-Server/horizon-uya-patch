/***************************************************
 * FILENAME :		weapon.h
 * DESCRIPTION :
 * 		Contains weapon specific offsets and structures for UYA.
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_WEAPON_H_
#define _LIBUYA_WEAPON_H_

#include <tamtypes.h>
#include "common.h"
#include "math3d.h"

/*
 * NAME :		WEAPON_IDS
 * DESCRIPTION :
 * 			Defines the value for each weapon id.
 * NOTES :
 *          
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
enum WEAPON_IDS
{
    WEAPON_ID_N60 = 1,
    WEAPON_ID_BLITZ = 2,
    WEAPON_ID_FLUX = 3,
    WEAPON_ID_ROCKET = 4,
    WEAPON_ID_GBOMB = 5,
    WEAPON_ID_MINE = 6,
    WEAPON_ID_LAVA = 7,
    WEAPON_ID_MORPH = 9,
    WEAPON_ID_WRENCH = 10,
    WEAPON_ID_SWINGSHOT = 11,
    WEAPON_ID_HOLO = 12,
    WEAPON_ID_GRAVITYBOOTS = 14,
    WEAPON_ID_CHARGEBOOTS = 15,
    WEAPON_ID_N60_V2 = 16,
    WEAPON_ID_BLITZ_V2 = 17,
    WEAPON_ID_FLUX_V2 = 18,
    WEAPON_ID_ROCKET_V2 = 19,
    WEAPON_ID_GBOMB_V2 = 20,
    WEAPON_ID_MINE_V2 = 21,
    WEAPON_ID_LAVA_V2 = 22,
    WEAPON_ID_MORPH_V2 = 23,
    WEAPON_ID_WRENCH_V2 = 24,
};

typedef struct TweakersGravityBomb {
/* 0x00 */ float maxThrowDist;
/* 0x04 */ float minThrowDist;
/* 0x08 */ float gravity;
/* 0x0c */ float maxThrowSpeed;
/* 0x10 */ float mp_maxThrowSpeed;
} TweakersGravityBomb_t;

typedef struct WeaponQuickSelect {
    char Slot[7];
} WeaponQuickSelect;

typedef struct Weapons {
    union {
        char Slot[12];
        struct {
            char N60;
            char Blitz;
            char Flux;
            char Rocket;
            char GBomb;
            char Mine;
            char Lava;
            char Morph;
            char Wrench;
            char Swingshot;
            char Holo;
        }
    }
} Weapons;

struct tNW_GadgetEventMessage { // 0x24
	/*   0 */ short int GadgetId;
	/*   2 */ char PlayerIndex;
	/*   3 */ char GadgetEventType;
	/*   4 */ char ExtraData;
	/*   8 */ int ActiveTime;
	/*   c */ unsigned int TargetUID;
	/*  10 */ float FiringLoc[3];
	/*  1c */ float TargetDir[3];
};

typedef struct GadgetEvent { // 0x50
	/*   0 */ unsigned char gadgetID;
	/*   1 */ unsigned char cPlayerIndex;
	/*   2 */ char cGadgetType;
	/*   3 */ char gadgetEventType;
	/*   4 */ int iActiveTime;
	/*   8 */ unsigned int targetUID;
	/*  10 */ VECTOR targetOffsetQuat;
	/*  20 */ struct GadgetEvent* pNextGadgetEvent;
	/*  24 */ struct tNW_GadgetEventMessage gadgetEventMsg;
} GadgetEvent;

typedef struct GadgetDef { // 0x80
    /* 0x00 */ short int pickupTag;
    /* 0x02 */ short int quickSelectTag;
    /* 0x04 */ short int mobyClass;
	/* 0x06 */ short int mobyClass2;
    /* 0x08 */ char isWeapon;
	/* 0x09 */ char type; // Unsure
	/* 0x0a */ char joint; // Unsure
	/* 0x0b */ char handGadgetType;
    /* 0x0c */ short int fullFireAnim;
	/* 0x0e */ short int armFireAnimDefault;
	/* 0x10 */ short int armFireAnimCrouch;
	/* 0x12 */ short int sprite;
	/* 0x14 */ short int ammotag;
	/* 0x16 */ short int upgAmmotag;
    /* 0x18 */ short int unk_18; 
    /* 0x1a */ short int unk_1a;
    /* 0x1c */ short int ammoClass;
	/* 0x1e */ short int ammoAmount;
    /* 0x20 */ short int unk_20; 
    /* 0x22 */ short int unk_22;
    /* 0x24 */ float metersPerSec;
	/* 0x28 */ float shotsPerSec;
    /* 0x2c */ short int unk_2c; 
    /* 0x2e */ short int unk_2e;
    /* 0x30 */ float damage;
	/* 0x34 */ float damage2;
    /* 0x38 */ char unk_38[0x38];
    /* 0x70 */ int gadgetTargetFunc;
    /* 0x74 */ char unk_74[0xc];
} GadgetDef;

/*
 * Returns pointer to the start of the weapon list.
 */
__LIBUYA_GETTER__ GadgetDef * weaponGadgetList(void);

// Returns true if given Weapon ID is available in game.
int weaponIsEnabled(int weaponId);

TweakersGravityBomb_t * weaponGravityBombTweakers(void);

#endif // _LIBUYA_WEAPON_H_
