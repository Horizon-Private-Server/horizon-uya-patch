/***************************************************
 * FILENAME :		map.h
 * 
 * DESCRIPTION :
 * 		Contains map specific offsets and structures for UYA.
 * 
 * NOTES :
 * 
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_MAP_H_
#define _LIBUYA_MAP_H_

#include <tamtypes.h>

/*
 *
 */
enum MAP_IDS
{
    MAP_ID_BAKISI           = 40,
    MAP_ID_HOVEN            = 41,
    MAP_ID_OUTPOST_X12      = 42,
    MAP_ID_KORGON           = 43,
    MAP_ID_METROPOLIS       = 44,
    MAP_ID_BLACKWATER_CITY  = 45,
    MAP_ID_COMMAND_CENTER   = 46,
    MAP_ID_BLACKWATER_DOCKS = 47,
    MAP_ID_AQUATOS_SEWERS   = 48,
    MAP_ID_MARCADIA         = 49,
};

enum MAP_MASK_IDS
{
    MAP_MASK_NONE               = 0,
    MAP_MASK_BAKISI             = (1 << 0),
    MAP_MASK_HOVEN              = (1 << 1),
    MAP_MASK_OUTPOST_X12        = (1 << 2),
    MAP_MASK_KORGON             = (1 << 3),
    MAP_MASK_METROPOLIS         = (1 << 4),
    MAP_MASK_BLACKWATER_CITY    = (1 << 5),
    MAP_MASK_COMMAND_CENTER     = (1 << 6),
    MAP_MASK_BLACKWATER_DOCKS   = (1 << 7),
    MAP_MASK_AQUATOS_SEWERS     = (1 << 8),
    MAP_MASK_MARCADIA           = (1 << 9),
    MAP_MASK_ALL                = -1
};

int mapMaskHasMask(int value, int maskValue);
int mapIdToMask(int mapId);
int mapMaskToId(int mapMask);
char * mapGetName(int mapId);

#endif // _LIBUYA_MAP_H_
