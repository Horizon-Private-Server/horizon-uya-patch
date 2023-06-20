/***************************************************
 * FILENAME :		weapon.h
 * 
 * DESCRIPTION :
 * 		Contains weapon specific offsets and structures for UYA.
 * 
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_WEAPON_H_
#define _LIBUYA_WEAPON_H_

#include "common.h"

/*
 * NAME :		WEAPON_IDS
 * 
 * DESCRIPTION :
 * 			Defines the value for each weapon id.
 * 
 * NOTES :
 *          
 * 
 * AUTHOR :			Troy "Agent Moose" Pruitt
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

typedef struct WeaponQuickSelect {
    char slot1;
    char slot2;
    char slot3;
} WeaponQuickSelect;

typedef struct WeaponAmmo {
    char N60;
    char Blitz;
    char Flux;
    char Rocket;
    char GBomb;
    char Mine;
    char Lava;
    char Empty_1; // Morph-O-Ray
    char Empty_2; // Wrench
    char Empty_3; // Swingshot
    char Holo;
} WeaponAmmo;

typedef struct WeaponMeter {
    char N60;
    char Blitz;
    char Flux;
    char Rocket;
    char GBomb;
    char Mine;
    char Lava;
    char Morph;
    char Empty_1; // Wrench
    char Empty_2; // Swingshot
    char Holo;
} WeaponMeter;

#endif // _LIBUYA_WEAPON_H_
