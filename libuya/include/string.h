/***************************************************
 * FILENAME :		string.h
 * DESCRIPTION :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_STRING_H_
#define _LIBUYA_STRING_H_

#include "common.h"

//--------------------------------------------------------
__LIBUYA_INLINE__ void * memset(void * ptr, int value, int num);
__LIBUYA_INLINE__ void * memcpy(void * destination, const void * source, int num);
__LIBUYA_INLINE__ void * memmove(void * destination, const void * source, int num);
__LIBUYA_INLINE__ char * strncpy(char *dest, const char *src, int n);
__LIBUYA_INLINE__ char * strcat(char *dest, const char *src);
__LIBUYA_INLINE__ char * strncat(char *dest, const char *src, int n);
__LIBUYA_INLINE__ int strcmp(char *str1, char *str2);
__LIBUYA_INLINE__ int strncmp(char * str1, char * str2, int n);
__LIBUYA_INLINE__ int strlen(char *str);

#endif // _LIBUYA_STRING_H_
