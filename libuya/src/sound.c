#include "math3d.h"
#include "common.h"
#include "interop.h"

VariableAddress_t vaSoundHeroPlayFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0,
	.Hoven = 0,
	.OutpostX12 = 0,
    .KorgonOutpost = 0,
	.Metropolis = 0,
	.BlackwaterCity = 0,
	.CommandCenter = 0,
    .BlackwaterDocks = 0,
    .AquatosSewers = 0,
    .MarcadiaPalace = 0,
#else
	.Lobby = 0,
	.Bakisi = 0x00509738,
	.Hoven = 0x0050b790,
	.OutpostX12 = 0x005010a8,
    .KorgonOutpost = 0x004fe8c0,
	.Metropolis = 0x004fdc10,
	.BlackwaterCity = 0x004fb428,
	.CommandCenter = 0x004fb5b0,
    .BlackwaterDocks = 0x004fddf0,
    .AquatosSewers = 0x004fd130,
    .MarcadiaPalace = 0x004fca70,
#endif
};

VariableAddress_t vaSoundMobyPlayFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0,
	.Hoven = 0,
	.OutpostX12 = 0,
    .KorgonOutpost = 0,
	.Metropolis = 0,
	.BlackwaterCity = 0,
	.CommandCenter = 0,
    .BlackwaterDocks = 0,
    .AquatosSewers = 0,
    .MarcadiaPalace = 0,
#else
	.Lobby = 0,
	.Bakisi = 0x004c33d8,
	.Hoven = 0x004c5430,
	.OutpostX12 = 0x004bad48,
    .KorgonOutpost = 0x004b8560,
	.Metropolis = 0x004b78b0,
	.BlackwaterCity = 0x004b50c8,
	.CommandCenter = 0x004b5280,
    .BlackwaterDocks = 0x004b7ac0,
    .AquatosSewers = 0x004b6e00,
    .MarcadiaPalace = 0x004b6740,
#endif
};

VariableAddress_t vaSoundPlayFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0,
	.Hoven = 0,
	.OutpostX12 = 0,
    .KorgonOutpost = 0,
	.Metropolis = 0,
	.BlackwaterCity = 0,
	.CommandCenter = 0,
    .BlackwaterDocks = 0,
    .AquatosSewers = 0,
    .MarcadiaPalace = 0,
#else
	.Lobby = 0,
	.Bakisi = 0x004c3130,
	.Hoven = 0x004c5188,
	.OutpostX12 = 0x004baaa0,
    .KorgonOutpost = 0x004b82b8,
	.Metropolis = 0x004b7608,
	.BlackwaterCity = 0x004b4e20,
	.CommandCenter = 0x004b4fd8,
    .BlackwaterDocks = 0x004b7818,
    .AquatosSewers = 0x004b6b58,
    .MarcadiaPalace = 0x004b6498,
#endif
};
