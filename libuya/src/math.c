#include "math.h"
#include "interop.h"

variableAddressCompact_t vaFastCosf = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0045dcd8),
	.Hoven = VA_COMPACT_OFFSET(0x0045dcd8, 0x0045f888),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045dcd8, 0x00456688),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045dcd8, 0x00454218),
	.Metropolis = VA_COMPACT_OFFSET(0x0045dcd8, 0x00453558),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045dcd8, 0x00450d88),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045dcd8, 0x00451680),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045dcd8, 0x00453f00),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045dcd8, 0x00453200),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045dcd8, 0x00452b80),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0045cc28),
	.Hoven = VA_COMPACT_OFFSET(0x0045cc28, 0x0045e718),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045cc28, 0x00455558),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045cc28, 0x00453168),
	.Metropolis = VA_COMPACT_OFFSET(0x0045cc28, 0x004524a8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045cc28, 0x0044fc58),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045cc28, 0x00450710),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045cc28, 0x00452f50),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045cc28, 0x00452290),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045cc28, 0x00451bd0),
#endif
};

variableAddressCompact_t vaFastSinf = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0045dcf0),
	.Hoven = VA_COMPACT_OFFSET(0x0045dcf0, 0x0045f8a0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045dcf0, 0x004566a0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045dcf0, 0x00454230),
	.Metropolis = VA_COMPACT_OFFSET(0x0045dcf0, 0x00453570),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045dcf0, 0x00450da0),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045dcf0, 0x00451698),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045dcf0, 0x00453f18),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045dcf0, 0x00453218),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045dcf0, 0x00452b98),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0045cc40),
	.Hoven = VA_COMPACT_OFFSET(0x0045cc40, 0x0045e730),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045cc40, 0x00455570),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045cc40, 0x00453180),
	.Metropolis = VA_COMPACT_OFFSET(0x0045cc40, 0x004524c0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045cc40, 0x0044fc70),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045cc40, 0x00450728),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045cc40, 0x00452f68),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045cc40, 0x004522a8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045cc40, 0x00451be8),
#endif
};

variableAddressCompact_t vaFastArcTan = {
#if UYA_PAL
	.Lobby = 0x00590b30,
	.Bakisi = VA_COMPACT_BASE(0x0045dda0),
	.Hoven = VA_COMPACT_OFFSET(0x0045dda0, 0x0045f950),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045dda0, 0x00456750),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045dda0, 0x004542e0),
	.Metropolis = VA_COMPACT_OFFSET(0x0045dda0, 0x00453620),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045dda0, 0x00450e50),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045dda0, 0x00451748),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045dda0, 0x00453fc8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045dda0, 0x004532c8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045dda0, 0x00452c48),
#else
	.Lobby = 0x0058f980,
	.Bakisi = VA_COMPACT_BASE(0x0045ccf0),
	.Hoven = VA_COMPACT_OFFSET(0x0045ccf0, 0x0045e7e0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045ccf0, 0x00455620),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045ccf0, 0x00453230),
	.Metropolis = VA_COMPACT_OFFSET(0x0045ccf0, 0x00452570),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045ccf0, 0x0044fd20),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045ccf0, 0x004507d8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045ccf0, 0x00453018),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045ccf0, 0x00452358),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045ccf0, 0x00451c98),
#endif
};

//--------------------------------------------------------
float acosf(float v)
{
    return (MATH_PI / 2) - asinf(v);
}

//--------------------------------------------------------
float lerpf(float a, float b, float t)
{
    return (b-a)*t + a;
}

//--------------------------------------------------------
float fabsf(float f)
{
    union { float x; unsigned int i; } u = { f };
    u.i &= 0x7fffffff;
    return u.x;
}

//--------------------------------------------------------
float signf(float a)
{
    return (a < 0) ? -1 : 1;
}

//--------------------------------------------------------
float maxf(float a, float b)
{
    return (a < b) ? b : a;
}

//--------------------------------------------------------
float minf(float a, float b)
{
    return (a > b) ? b : a;
}

//--------------------------------------------------------
float lerpfAngle(float a, float b, float t)
{
    if (fabsf(a-b) < MATH_PI)
        return clampAngle(lerpf(a, b, t));
    else if (a < b)
        return clampAngle(lerpf(a, b - MATH_TAU, t));
    else
        return clampAngle(lerpf(a, b + MATH_TAU, t));
}

//--------------------------------------------------------
float clamp(float v, float min, float max)
{
    if (v < min)
        return min;
    if (v > max)
        return max;

    return v;
}

//--------------------------------------------------------
/*
float clampAngle(float theta)
{
  float f1 = (theta + MATH_PI) / MATH_TAU;
  return (f1 - (float)(int)f1) * MATH_TAU - MATH_PI;
}
*/

//--------------------------------------------------------

float clampAngle(float theta)
{
    if (theta > MATH_PI)
        theta -= MATH_TAU;
    else if (theta < -MATH_PI)
        theta += MATH_TAU;

    return theta;
}


//--------------------------------------------------------
float fastSubRots(float input0, float input1)
{
    input0 = input0 - input1;
    int i = input0 < -MATH_PI;
    if (MATH_PI <= input0)
        input0 = (input0 - MATH_PI) - MATH_PI;

    if (i)
        input0 = input0 + MATH_PI + MATH_PI;
    
    return input0;
}

float fastDiffRots(float input0, float input1)
{
    float diff = fabsf(input0 - input1);
    if (MATH_PI <= diff) 
        diff = MATH_TAU - diff;

    return diff;
}

//float sqrtf(float number) {
//    float x = number * 0.5f;
//    float y = number;
//    long i = *(long*)&y;           // evil float bit hack
//    i = 0x5f3759df - (i >> 1);     // what the hell?
//    y = *(float*)&i;
//    y = y * (1.5f - (x * y * y));  // 1st iteration
//    return number * y;
//}

float sqrtf(float x)
{
    if (x <= 0.0f) return 0.0f;
    float out;
    __asm__ volatile (
        "sqrt.s %0, %1\n"
         : "=f"(out)
        : "f"(x)
        );
    return out;
    }