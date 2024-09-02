/***************************************************
 * FILENAME :		team.h
 * DESCRIPTION :
 * 		Contains team specific offsets and structures for Deadlocked.
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_TEAM_H_
#define _LIBUYA_TEAM_H_

#include <tamtypes.h>
#include "gamesettings.h"

typedef enum TEAM_IDS {
    TEAM_BLUE,
    TEAM_RED,
    TEAM_GREEN,
    TEAM_ORANGE,
    TEAM_YELLOW,
    TEAM_PURPLE,
    TEAM_AQUA,
    TEAM_PINK,
    TEAM_WHITE
} TEAM_IDS;

typedef enum SKINS {
    SKIN_RATCHET,
    SKIN_ROBO,
    SKIN_THUG,
    SKIN_THYRRANOID,
    SKIN_BLARG,
    SKIN_NINJA,
    SKIN_SNOWMAN,
    SKIN_BRUISER,
    SKIN_GRAY,
    SKIN_HOTBOT,
    SKIN_GLADIOLA,
    SKIN_EVILCLOWN,
    SKIN_BEACHBUNNY,
    SKIN_ROBOROOSTER,
    SKIN_BUGINOID,
    SKIN_BRAINIUS,
    SKIN_SKRUNCH,
    SKIN_BONES,
    SKIN_NEFARIOUS,
    SKIN_TROOPER,
    SKIN_CONSTRUCTOBOT,
    SKIN_DAN
} SKINS;

const u32 TEAM_COLORS[9];

#endif // _LIBUYA_TEAM_H_
