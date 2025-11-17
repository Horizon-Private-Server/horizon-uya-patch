/***************************************************
 * FILENAME :    stdlib.h
 * 
 * DESCRIPTION :
 * 
 * AUTHOR :      Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_STDLIB_H_
#define _LIBUYA_STDLIB_H_

#include "common.h"


//--------------------------------------------------------
__LIBUYA_INLINE__ void* malloc( unsigned long size );
__LIBUYA_INLINE__ void free( void* ptr );

#endif // _LIBUYA_STDLIB_H_