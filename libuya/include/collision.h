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

typedef struct COLL_DAM_OUT { // 0x40
/* 0x00 */ VECTOR ip;
/* 0x10 */ VECTOR momentum;
/* 0x20 */ Moby *pDamager;
/* 0x24 */ int damageFlags;
/* 0x28 */ unsigned char damageClass;
/* 0x29 */ unsigned char damageStrength;
/* 0x2a */ short unsigned int damageIndex;
/* 0x2c */ float damageHp;
/* 0x30 */ int flags;
/* 0x34 */ float damageHeroHp;
/* 0x38 */ int shotUID;
/* 0x3c */ Moby *pMoby;
} COLL_DAM_OUT_t;

typedef struct COLL_DAM_IN { // 0x30
/* 0x00 */ VECTOR momentum;
/* 0x10 */ Moby *pMoby;
/* 0x14 */ int damageFlags;
/* 0x18 */ unsigned char damageClass;
/* 0x19 */ unsigned char damageStrength;
/* 0x1a */ short unsigned int damageIndex;
/* 0x1c */ float damageHp;
/* 0x20 */ int flags;
/* 0x24 */ float damageHeroHp;
/* 0x28 */ int shotUID;
/* 0x2c */ int pad[1];
} COLL_DAM_IN_t;

/*
 * NAME :		CollMobysSphere_Fix
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int CollMobysSphere_Fix(float radius, VECTOR *position,Moby *pMoby, int ignoreId, COLL_DAM_OUT_t *damage);

/*
 * NAME :		CollMobysSphere_Fix_GetHitMobies
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
__LIBUYA_GETTER__ Moby** CollMobysSphere_Fix_GetHitMobies(void);

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

/*
 * NAME :		collHotspot
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
// same as calling: CollLine_Fix_GetHitCollisionId() & 0xf;
int CollHotspot(void);

/*
 * NAME :		colGetOutput
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
CollOutput* colGetOutput(void);

/*
 * NAME :		CollSoundSurface
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int CollSoundSurface(void);

/*
 * NAME :		CollSoundSurface
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :		1.0f is safe, less than 1.0f: collision occurred partway along the sweep
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
float CollMovingSphere(float radius, VECTOR from, VECTOR to, int flags, Moby *pSkip);

/*
 * NAME :		CollHeroSphere
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :		Returns number of polygons hit
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int CollHeroSphere(float radius, VECTOR position, int flags);

/*
 * NAME :		CollSphere_Fix
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :		Returns number of polygons hit
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int CollSphere_Fix(float radius, VECTOR position, int flags);

#endif // _LIBUYA_COLLISION_H_
