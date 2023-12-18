#ifndef _LIBUYA_SPAWNPOINT_H_
#define _LIBUYA_SPAWNPOINT_H_

#include "math3d.h"
#include "common.h"
#include "player.h"

struct Cuboid { // 0x80
	/* 0x00 */ mtx3 matrix;
	/* 0x30 */ vec4 pos;
	/* 0x40 */ mtx3 imatrix;
	/* 0x70 */ vec4f rot;
};

typedef struct SpawnPoint
{
    MATRIX M0;
    MATRIX M1;
} SpawnPoint;

typedef struct SpawnPointPosRot
{
    VECTOR position;
    VECTOR rotation;
} SpawnPointPosRot;

/*
 * NAME :		spawnPointGetCount
 * 
 * DESCRIPTION :
 * 			
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int spawnPointGetCount(void);

/*
 * NAME :		spawnPointGet
 * 
 * DESCRIPTION :
 * 			
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ SpawnPoint * spawnPointGet(int index);

/*
 * NAME :		spawnPointSet
 * 
 * DESCRIPTION :
 * 			
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_SETTER__ void spawnPointSet(SpawnPoint * sp, int index);

/*
 * NAME :		spawnPointGetRandom
 * 
 * DESCRIPTION :    Get a random spawnpoint and puts them into the vPosition
 *                  pointer given and vRotation pointer given.
 * 			
 * 
 * NOTES :          
 * 
 * ARGS :           Player * player: pointer to player struct.
 *                  vPosition: Pointer, where position data is saved.
 *                  vrotation: Pointer, where rotation data is saved.
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int spawnPointGetRandom(Player * player, u32 vPosition, u32 vRotation);


#endif // _LIBUYA_SPAWNPOINT_H_
