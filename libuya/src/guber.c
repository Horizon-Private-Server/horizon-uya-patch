#include "interop.h"
#include "guber.h"

#if UYA_PAL
#define FIRST_GUBER_MOBY_PTR						((GuberMoby**)0x00246350)
#else
#define FIRST_GUBER_MOBY_PTR						((GuberMoby**)0x002464d0)
#endif

#define MASTER_LIST									(GetAddress(&vaMasterList))

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

VariableAddress_t vaGuberGetObjectByUID = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004ef0d0,
    .Hoven = 0x004f11e8,
    .OutpostX12 = 0x004e6ac0,
    .KorgonOutpost = 0x004e4258,
    .Metropolis = 0x004e35a8,
    .BlackwaterCity = 0x004e0e40,
    .CommandCenter = 0x004e0e08,
    .BlackwaterDocks = 0x004e3688,
    .AquatosSewers = 0x004e2988,
    .MarcadiaPalace = 0x004e2308,
#else
    .Lobby = 0,
    .Bakisi = 0x004ec9b0,
    .Hoven = 0x004eea08,
    .OutpostX12 = 0x004e4320,
    .KorgonOutpost = 0x004e1b38,
    .Metropolis = 0x004e0e88,
    .BlackwaterCity = 0x004de6a0,
    .CommandCenter = 0x004de828,
    .BlackwaterDocks = 0x004e1068,
    .AquatosSewers = 0x004e03a8,
    .MarcadiaPalace = 0x004dfce8,
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

VariableAddress_t vaGuberGetUID = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004ef678,
    .Hoven = 0x004f1790,
    .OutpostX12 = 0x004e7068,
    .KorgonOutpost = 0x004e4800,
    .Metropolis = 0x004e3b50,
    .BlackwaterCity = 0x004e13e8,
    .CommandCenter = 0x004e13b0,
    .BlackwaterDocks = 0x004e3c30,
    .AquatosSewers = 0x004e2f30,
    .MarcadiaPalace = 0x004e28b0,
#else
    .Lobby = 0,
    .Bakisi = 0x004ecf58,
    .Hoven = 0x004eefb0,
    .OutpostX12 = 0x004e48c8,
    .KorgonOutpost = 0x004e20e0,
    .Metropolis = 0x004e1430,
    .BlackwaterCity = 0x004dec48,
    .CommandCenter = 0x004dedd0,
    .BlackwaterDocks = 0x004e1610,
    .AquatosSewers = 0x004e0950,
    .MarcadiaPalace = 0x004e0290,
#endif
};

VariableAddress_t vaMasterList = {
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
    .Bakisi = 0x004ed8f0,
    .Hoven = 0,
    .OutpostX12 = 0,
    .KorgonOutpost = 0,
    .Metropolis = 0,
    .BlackwaterCity = 0,
    .CommandCenter = 0,
    .BlackwaterDocks = 0,
    .AquatosSewers = 0,
    .MarcadiaPalace = 0,
#endif
};
VariableAddress_t vaGetMaster = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004f0010,
    .Hoven = 0x004f2128,
    .OutpostX12 = 0x004e7a00,
    .KorgonOutpost = 0x004e5198,
    .Metropolis = 0x004e44e8,
    .BlackwaterCity = 0x004e1d80,
    .CommandCenter = 0x004e1d48,
    .BlackwaterDocks = 0x004e45c8,
    .AquatosSewers = 0x004e38c8,
    .MarcadiaPalace = 0x004e3248,
#else
    .Lobby = 0,
    .Bakisi = 0x004ed8f0,
    .Hoven = 0x004ef948,
    .OutpostX12 = 0x004e5260,
    .KorgonOutpost = 0x004e2a78,
    .Metropolis = 0x004e1dc8,
    .BlackwaterCity = 0x004df5e0,
    .CommandCenter = 0x004df768,
    .BlackwaterDocks = 0x004e1fa8,
    .AquatosSewers = 0x004e12e8,
    .MarcadiaPalace = 0x004e0c28,
#endif
};
VariableAddress_t vaCreateMaster = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004f00b0,
    .Hoven = 0x004f21c8,
    .OutpostX12 = 0x004e7aa0,
    .KorgonOutpost = 0x004e5238,
    .Metropolis = 0x004e4588,
    .BlackwaterCity = 0x004e1e20,
    .CommandCenter = 0x004e1de8,
    .BlackwaterDocks = 0x004e4668,
    .AquatosSewers = 0x004e3968,
    .MarcadiaPalace = 0x004e32e8,
#else
    .Lobby = 0,
    .Bakisi = 0x004ed990,
    .Hoven = 0x004ef9e8,
    .OutpostX12 = 0x004e5300,
    .KorgonOutpost = 0x004e2b18,
    .Metropolis = 0x004e1e68,
    .BlackwaterCity = 0x004df680,
    .CommandCenter = 0x004df808,
    .BlackwaterDocks = 0x004e2048,
    .AquatosSewers = 0x004e1388,
    .MarcadiaPalace = 0x004e0cc8,
#endif
};
VariableAddress_t vaDeleteMaster = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004f0170,
    .Hoven = 0x004f2288,
    .OutpostX12 = 0x004e7b60,
    .KorgonOutpost = 0x004e52f8,
    .Metropolis = 0x004e4648,
    .BlackwaterCity = 0x004e1ee0,
    .CommandCenter = 0x004e1ea8,
    .BlackwaterDocks = 0x004e4728,
    .AquatosSewers = 0x004e3a28,
    .MarcadiaPalace = 0x004e33a8,
#else
    .Lobby = 0,
    .Bakisi = 0x004eda50,
    .Hoven = 0x004efaa8,
    .OutpostX12 = 0x004e53c0,
    .KorgonOutpost = 0x004e2bd8,
    .Metropolis = 0x004e1f28,
    .BlackwaterCity = 0x004df740,
    .CommandCenter = 0x004df8c8,
    .BlackwaterDocks = 0x004e2108,
    .AquatosSewers = 0x004e1448,
    .MarcadiaPalace = 0x004e0d88,
#endif
};

/*
 * Returns the first guber moby.
 */
GuberMoby * guberMobyGetFirst(void)
{
    return *FIRST_GUBER_MOBY_PTR;
}
