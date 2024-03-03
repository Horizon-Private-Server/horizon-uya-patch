/***************************************************
 * FILENAME :		collision.h
 * DESCRIPTION :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */

#ifndef _LIBUYA_COLLISION_H_
#define _LIBUYA_COLLISION_H_

#include <tamtypes.h>
#include "moby.h"

typedef struct CollOutput { // 0x80
	/* 0x00 */ void *grid;
	/* 0x04 */ int pad[3];
	/* 0x10 */ int call_cnt;
	/* 0x14 */ int damage_next;
	/* 0x18 */ Moby *pMoby;
	/* 0x1c */ int poly;
	/* 0x20 */ VECTOR ip;
	/* 0x30 */ VECTOR push;
	/* 0x40 */ VECTOR normal;
	/* 0x50 */ VECTOR v0;
	/* 0x60 */ VECTOR v1;
	/* 0x70 */ VECTOR v2;
} CollOutput;

/*
 * NAME :		CollMobysSphere_Fix
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
// int CollMobysSphere_Fix(VECTOR position, u64 hitFlag, Moby * moby, u64 a3, float radius);


/*
 * NAME :		CollMobysSphere_Fix_GetHitMobies
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
// __LIBUYA_GETTER__ Moby** CollMobysSphere_Fix_GetHitMobies(void);

/*
 * NAME :		CollLine_Fix
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int CollLine_Fix(VECTOR from, VECTOR to, u64 hitFlag, Moby * moby, u64 t0);

/*
 * NAME :		CollLine_Fix_GetHitMoby
 * DESCRIPTION :
 * 		 
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
__LIBUYA_GETTER__ Moby* CollLine_Fix_GetHitMoby(void);

/*
 * NAME :		CollLine_Fix_GetHitPosition
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
__LIBUYA_GETTER__ float* CollLine_Fix_GetHitPosition(void);

/*
 * NAME :		CollLine_Fix_GetHitNormal
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
__LIBUYA_GETTER__ float* CollLine_Fix_GetHitNormal(void);

/*
 * NAME :		CollLine_Fix_GetHitCollisionId
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
__LIBUYA_GETTER__ int CollLine_Fix_GetHitCollisionId(void);

// same as calling: CollLine_Fix_GetHitCollisionId() & 0xf;
int CollHotspot(void);

#endif // _LIBUYA_COLLISION_H_
