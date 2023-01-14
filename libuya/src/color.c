#include "game.h"
#include "color.h"
#include "interop.h"

#if UYA_PAL
VariableAddress_t vaColorLerpFunc = {
	.Lobby = 0x005915e8,
	.Bakisi = 0x0045e858,
	.Hoven = 0x00460408,
	.OutpostX12 = 0x00457208,
    .KorgonOutpost = 0x00454d98,
	.Metropolis = 0x004540d8,
	.BlackwaterCity = 0x00451908,
	.CommandCenter = 0x00452200,
    .BlackwaterDocks = 0x00454a80,
    .AquatosSewers = 0x00453d80,
    .MarcadiaPalace = 0x00453700,
};
#else
VariableAddress_t vaColorLerpFunc = {
	.Lobby = 0x00590438,
	.Bakisi = 0x0045d7a8,
	.Hoven = 0x0045f298,
	.OutpostX12 = 0x004560d8,
    .KorgonOutpost = 0x00453ce8,
	.Metropolis = 0x00453ce8,
	.BlackwaterCity = 0x004507d8,
	.CommandCenter = 0x00451290,
    .BlackwaterDocks = 0x00453ad0,
    .AquatosSewers = 0x00452e10,
    .MarcadiaPalace = 0x00452750,
};
#endif

u32 internal_colorLerp(u32, u32, float);

/*
 * NAME :		colorLerp
 * 
 * DESCRIPTION :
 * 			Interpolates the color from a to b by t.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          a     :           Start color.
 *          b     :           End color.
 *          t     :           Amount to interpolate by.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
u32 colorLerp(u32 a, u32 b, float t)
{
    return internal_colorLerp(a, b, t);
}
