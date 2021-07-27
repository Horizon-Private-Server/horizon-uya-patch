/***************************************************
 * FILENAME :		string.h
 * 
 * DESCRIPTION :
 * 
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_STRING_H_
#define _LIBUYA_STRING_H_

#include "common.h"

//--------------------------------------------------------
__LIBUYA_INLINE__ void * memset(void * ptr, int value, int num);
__LIBUYA_INLINE__ void * memcpy(void * destination, const void * source, int num);
__LIBUYA_INLINE__ char * strncpy(char *dest, const char *src, int n);

#endif // _LIBUYA_STRING_H_
