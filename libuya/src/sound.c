#include "math3d.h"
#include "common.h"
#include "sound.h"
#include "interop.h"

VariableAddress_t vaSoundHeroPlayFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0050bf28,
	.Hoven = 0x0050e040,
	.OutpostX12 = 0x00503918,
    .KorgonOutpost = 0x005010b0,
	.Metropolis = 0x00500400,
	.BlackwaterCity = 0x004fdc98,
	.CommandCenter = 0x004fdc60,
    .BlackwaterDocks = 0x005004e0,
    .AquatosSewers = 0x004ff7e0,
    .MarcadiaPalace = 0x004ff160,
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
	.Bakisi = 0x004c5a80,
	.Hoven = 0x004c7b98,
	.OutpostX12 = 0x004bd470,
    .KorgonOutpost = 0x004bac08,
	.Metropolis = 0x004b9f58,
	.BlackwaterCity = 0x004b77f0,
	.CommandCenter = 0x004b77e8,
    .BlackwaterDocks = 0x004ba068,
    .AquatosSewers = 0x004b9368,
    .MarcadiaPalace = 0x004b8ce8,
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

VariableAddress_t vaSoundPlayByOClassFunc = {
#if UYA_PAL
    .Lobby = 0x005f3488,
    .Bakisi = 0x004c5b28,
    .Hoven = 0x004c7c40,
    .OutpostX12 = 0x004bd518,
    .KorgonOutpost = 0x004bacb0,
    .Metropolis = 0x004ba000,
    .BlackwaterCity = 0x004b7898,
    .CommandCenter = 0x004b7890,
    .BlackwaterDocks = 0x004ba110,
    .AquatosSewers = 0x004b9410,
    .MarcadiaPalace = 0x004b8d90,
#else
    .Lobby = 0x005f0d88,
    .Bakisi = 0x004c3480,
    .Hoven = 0x004c54d8,
    .OutpostX12 = 0x004badf0,
    .KorgonOutpost = 0x004b8608,
    .Metropolis = 0x004b7958,
    .BlackwaterCity = 0x004b5170,
    .CommandCenter = 0x004b5328,
    .BlackwaterDocks = 0x004b7b68,
    .AquatosSewers = 0x004b6ea8,
    .MarcadiaPalace = 0x004b67e8,
#endif
};

VariableAddress_t vaSoundPlayFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004c57d8,
	.Hoven = 0x004c78f0,
	.OutpostX12 = 0x004bd1c8,
    .KorgonOutpost = 0x004ba960,
	.Metropolis = 0x004b9cb0,
	.BlackwaterCity = 0x004b7548,
	.CommandCenter = 0x004b7540,
    .BlackwaterDocks = 0x004b9dc0,
    .AquatosSewers = 0x004b90c0,
    .MarcadiaPalace = 0x004b8a40,
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
