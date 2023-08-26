/***************************************************
 * FILENAME :		collision.h
 * 
 * DESCRIPTION :
 * 		
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_COLLISION_H_
#define _LIBUYA_COLLISION_H_

#include <tamtypes.h>
#include "moby.h"

/*
 * NAME :		CollLine_Fix
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
int collLine_Fix(VECTOR from, VECTOR to, u64 hitFlag, Moby * moby, u64 t0);

#endif // _LIBUYA_COLLISION_H_
