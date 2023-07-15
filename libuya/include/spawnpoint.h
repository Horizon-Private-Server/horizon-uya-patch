#ifndef _LIBUYA_SPAWNPOINT_H_
#define _LIBUYA_SPAWNPOINT_H_

#include "math3d.h"
#include "common.h"

typedef struct SpawnPoint
{
    MATRIX M0;
    MATRIX M1;
} SpawnPoint;

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


#endif // _LIBUYA_SPAWNPOINT_H_
