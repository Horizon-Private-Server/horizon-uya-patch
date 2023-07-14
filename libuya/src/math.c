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

//--------------------------------------------------------
// -- https://stackoverflow.com/a/28050328
// float cosf(float x)
// {
//     const float tp = 1./(2.*MATH_PI);
//     x *= tp;
//     x -= .25 + floorf(x + .25);
//     x *= 16. * (fabsf(x) - .5);
//     x += .225 * x * (fabsf(x) - 1.);
//     return x;
// }

//--------------------------------------------------------
// float sinf(float theta)
// {
//     return cosf(clampAngle(theta - (MATH_PI / 2)));
// }

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
    if (theta > MATH_PI)
        theta -= MATH_TAU;
    else if (theta < -MATH_PI)
        theta += MATH_TAU;

    return theta;
}


//--------------------------------------------------------
// https://gist.github.com/volkansalma/2972237
float atan2f(float y, float x)
{
    //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
    //Volkan SALMA

    const float ONEQTR_PI = MATH_PI / 4.0;
	const float THRQTR_PI = 3.0 * MATH_PI / 4.0;
	float r, angle;
	float abs_y = fabsf(y) + 1e-10f;      // kludge to prevent 0/0 condition
	if ( x < 0.0f )
	{
		r = (x + abs_y) / (abs_y - x);
		angle = THRQTR_PI;
	}
	else
	{
		r = (x - abs_y) / (x + abs_y);
		angle = ONEQTR_PI;
	}
	angle += (0.1963f * r * r - 0.9817f) * r;
	if ( y < 0.0f )
		return( -angle );     // negate if in quad III or IV
	else
		return( angle );
}
