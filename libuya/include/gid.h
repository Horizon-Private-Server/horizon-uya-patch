/***************************************************
 * FILENAME :		gid.h
 * 
 * DESCRIPTION :
 * 
 * NOTES :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GID_H_
#define _LIBUYA_GID_H_

#include <tamtypes.h>

/*
 * NAME :		Gid
 * 
 * DESCRIPTION :
 * 			Contains the gid definition struct data.
 * 
 * NOTES :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct
{
    u32 ObjectIndex : 12;
    u32 ObjectCount : 12;
    u32 ObjectType : 4;
    u32 HostId : 4;
} Gid;

#endif // _LIBUYA_GID_H_
