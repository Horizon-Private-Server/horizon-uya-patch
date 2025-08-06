#include "math.h"
#include "interop.h"

VariableAddress_t vaFastCosf = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0045dcd8,
	.Hoven = 0x0045f888,
	.OutpostX12 = 0x00456688,
    .KorgonOutpost = 0x00454218,
	.Metropolis = 0x00453558,
	.BlackwaterCity = 0x00450d88,
	.CommandCenter = 0x00451680,
    .BlackwaterDocks = 0x00453f00,
    .AquatosSewers = 0x00453200,
    .MarcadiaPalace = 0x00452b80,
#else
	.Lobby = 0,
	.Bakisi = 0x0045cc28,
	.Hoven = 0x0045e718,
	.OutpostX12 = 0x00455558,
    .KorgonOutpost = 0x00453168,
	.Metropolis = 0x004524a8,
	.BlackwaterCity = 0x0044fc58,
	.CommandCenter = 0x00450710,
    .BlackwaterDocks = 0x00452f50,
    .AquatosSewers = 0x00452290,
    .MarcadiaPalace = 0x00451bd0,
#endif
};

VariableAddress_t vaFastSinf = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0045dcf0,
	.Hoven = 0x0045f8a0,
	.OutpostX12 = 0x004566a0,
    .KorgonOutpost = 0x00454230,
	.Metropolis = 0x00453570,
	.BlackwaterCity = 0x00450da0,
	.CommandCenter = 0x00451698,
    .BlackwaterDocks = 0x00453f18,
    .AquatosSewers = 0x00453218,
    .MarcadiaPalace = 0x00452b98,
#else
	.Lobby = 0,
	.Bakisi = 0x0045cc40,
	.Hoven = 0x0045e730,
	.OutpostX12 = 0x00455570,
    .KorgonOutpost = 0x00453180,
	.Metropolis = 0x004524c0,
	.BlackwaterCity = 0x0044fc70,
	.CommandCenter = 0x00450728,
    .BlackwaterDocks = 0x00452f68,
    .AquatosSewers = 0x004522a8,
    .MarcadiaPalace = 0x00451be8,
#endif
};

VariableAddress_t vaFastArcTan = {
#if UYA_PAL
    .Lobby = 0x00590b30,
    .Bakisi = 0x0045dda0,
    .Hoven = 0x0045f950,
    .OutpostX12 = 0x00456750,
    .KorgonOutpost = 0x004542e0,
    .Metropolis = 0x00453620,
    .BlackwaterCity = 0x00450e50,
    .CommandCenter = 0x00451748,
    .BlackwaterDocks = 0x00453fc8,
    .AquatosSewers = 0x004532c8,
    .MarcadiaPalace = 0x00452c48,
#else
    .Lobby = 0x0058f980,
    .Bakisi = 0x0045ccf0,
    .Hoven = 0x0045e7e0,
    .OutpostX12 = 0x00455620,
    .KorgonOutpost = 0x00453230,
    .Metropolis = 0x00452570,
    .BlackwaterCity = 0x0044fd20,
    .CommandCenter = 0x004507d8,
    .BlackwaterDocks = 0x00453018,
    .AquatosSewers = 0x00452358,
    .MarcadiaPalace = 0x00451c98,
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
float clampAngle(float theta)
{
  float f1 = (theta + MATH_PI) / MATH_TAU;
  return (f1 - (float)(int)f1) * MATH_TAU - MATH_PI;
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

float sqrtf(float number) {
    float x = number * 0.5f;
    float y = number;
    long i = *(long*)&y;           // evil float bit hack
    i = 0x5f3759df - (i >> 1);     // what the hell?
    y = *(float*)&i;
    y = y * (1.5f - (x * y * y));  // 1st iteration
    return number * y;
}