#include "interop.h"
#include "guber.h"

VariableAddress_t vaGuberEventCreate = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x005346a8,
	.Hoven = 0x005367c0,
	.OutpostX12 = 0x0052c098,
    .KorgonOutpost = 0x00529830,
	.Metropolis = 0x00528b80,
	.BlackwaterCity = 0x00526418,
	.CommandCenter = 0x005261d8,
    .BlackwaterDocks = 0x005261dc,
    .AquatosSewers = 0x00527d58,
    .MarcadiaPalace = 0x005276d8,
#else
	.Lobby = 0,
	.Bakisi = 0x00531e28,
	.Hoven = 0x00533e80,
	.OutpostX12 = 0x00529798,
    .KorgonOutpost = 0x00526fb0,
	.Metropolis = 0x00526300,
	.BlackwaterCity = 0x00523b18,
	.CommandCenter = 0x00523a98,
    .BlackwaterDocks = 0x005262d8,
    .AquatosSewers = 0x00525618,
    .MarcadiaPalace = 0x00524f58,
#endif
};

VariableAddress_t vaGuberGetObjectByMoby = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004ef6a8,
	.Hoven = 0x004f17c0,
	.OutpostX12 = 0x004e7098,
    .KorgonOutpost = 0x004e4830,
	.Metropolis = 0x004e3b80,
	.BlackwaterCity = 0x004e1418,
	.CommandCenter = 0x004e13e0,
    .BlackwaterDocks = 0x004e3c60,
    .AquatosSewers = 0x004e2f60,
    .MarcadiaPalace = 0x004e28e0,
#else
	.Lobby = 0,
	.Bakisi = 0x004ecf88,
	.Hoven = 0x004eefe0,
	.OutpostX12 = 0x004e48f8,
    .KorgonOutpost = 0x004e2110,
	.Metropolis = 0x004e1460,
	.BlackwaterCity = 0x004dec78,
	.CommandCenter = 0x004dee00,
    .BlackwaterDocks = 0x004e1640,
    .AquatosSewers = 0x004e0980,
    .MarcadiaPalace = 0x004e02c0,
#endif
};
