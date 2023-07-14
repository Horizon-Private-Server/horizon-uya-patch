/***************************************************
 * FILENAME :		math.h
 * 
 * DESCRIPTION :
 * 		Contains many math related function definitions found in UYA.
 * 
 * NOTES :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_MATH_H_
#define _LIBUYA_MATH_H_

#include "common.h"

//--------------------------------------------------------
#define MATH_PI                         ((float)3.141596)
#define MATH_TAU                        (MATH_PI * (float)2.0)
#define MATH_E                          ((float)2.7182818)
#define MATH_DT                         ((float)0.166666666)
#define MATH_RAD2DEG                    ((float)180 / MATH_PI)
#define MATH_DEG2RAD                    (MATH_PI / (float)180)

//--------------------------------------------------------
// __LIBUYA_INLINE__ float cosf(float theta);
// __LIBUYA_INLINE__ float sinf(float theta);
__LIBUYA_INLINE__ float acosf(float v);
__LIBUYA_INLINE__ float asinf(float v);
__LIBUYA_INLINE__ float sqrtf(float f);
__LIBUYA_INLINE__ float powf(float base, float exp);
__LIBUYA_INLINE__ float fabsf(float f);
__LIBUYA_INLINE__ float lerpf(float a, float b, float t);
__LIBUYA_INLINE__ float signf(float a);
__LIBUYA_INLINE__ float maxf(float a, float b);
__LIBUYA_INLINE__ float floorf(float a);
__LIBUYA_INLINE__ float lerpfAngle(float a, float b, float t);
__LIBUYA_INLINE__ float clamp(float v, float min, float max);
__LIBUYA_INLINE__ float clampAngle(float theta);
__LIBUYA_INLINE__ double fmod(double x, double y);
__LIBUYA_INLINE__ float atan2f(float y, float x);

#endif // _LIBUYA_MATH_H_
