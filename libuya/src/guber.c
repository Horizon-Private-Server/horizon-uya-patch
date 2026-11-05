#include "interop.h"
#include "guber.h"

#if UYA_PAL
#define FIRST_GUBER_MOBY_PTR						((GuberMoby**)0x00246350)
#else
#define FIRST_GUBER_MOBY_PTR						((GuberMoby**)0x002464d0)
#endif

#define MASTER_LIST									(GetAddress(&vaMasterList))

variableAddressCompact_t vaGuberEventCreate = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x005346a8,
	.Hoven = VA_COMPACT_OFFSET(0x005346a8, 0x005367c0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005346a8, 0x0052c098),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005346a8, 0x00529830),
	.Metropolis = VA_COMPACT_OFFSET(0x005346a8, 0x00528b80),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005346a8, 0x00526418),
	.CommandCenter = VA_COMPACT_OFFSET(0x005346a8, 0x005261d8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005346a8, 0x005261dc),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005346a8, 0x00527d58),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005346a8, 0x005276d8),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00531e28,
	.Hoven = VA_COMPACT_OFFSET(0x00531e28, 0x00533e80),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00531e28, 0x00529798),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00531e28, 0x00526fb0),
	.Metropolis = VA_COMPACT_OFFSET(0x00531e28, 0x00526300),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00531e28, 0x00523b18),
	.CommandCenter = VA_COMPACT_OFFSET(0x00531e28, 0x00523a98),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00531e28, 0x005262d8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00531e28, 0x00525618),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00531e28, 0x00524f58),
#endif
};

variableAddressCompact_t vaGuberGetObjectByUID = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004ef0d0,
	.Hoven = VA_COMPACT_OFFSET(0x004ef0d0, 0x004f11e8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ef0d0, 0x004e6ac0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ef0d0, 0x004e4258),
	.Metropolis = VA_COMPACT_OFFSET(0x004ef0d0, 0x004e35a8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ef0d0, 0x004e0e40),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ef0d0, 0x004e0e08),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ef0d0, 0x004e3688),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ef0d0, 0x004e2988),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ef0d0, 0x004e2308),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004ec9b0,
	.Hoven = VA_COMPACT_OFFSET(0x004ec9b0, 0x004eea08),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ec9b0, 0x004e4320),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ec9b0, 0x004e1b38),
	.Metropolis = VA_COMPACT_OFFSET(0x004ec9b0, 0x004e0e88),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ec9b0, 0x004de6a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ec9b0, 0x004de828),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ec9b0, 0x004e1068),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ec9b0, 0x004e03a8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ec9b0, 0x004dfce8),
#endif
};

variableAddressCompact_t vaGuberGetObjectByMoby = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004ef6a8,
	.Hoven = VA_COMPACT_OFFSET(0x004ef6a8, 0x004f17c0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ef6a8, 0x004e7098),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ef6a8, 0x004e4830),
	.Metropolis = VA_COMPACT_OFFSET(0x004ef6a8, 0x004e3b80),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ef6a8, 0x004e1418),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ef6a8, 0x004e13e0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ef6a8, 0x004e3c60),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ef6a8, 0x004e2f60),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ef6a8, 0x004e28e0),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004ecf88,
	.Hoven = VA_COMPACT_OFFSET(0x004ecf88, 0x004eefe0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ecf88, 0x004e48f8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ecf88, 0x004e2110),
	.Metropolis = VA_COMPACT_OFFSET(0x004ecf88, 0x004e1460),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ecf88, 0x004dec78),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ecf88, 0x004dee00),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ecf88, 0x004e1640),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ecf88, 0x004e0980),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ecf88, 0x004e02c0),
#endif
};

variableAddressCompact_t vaGuberGetUID = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004ef678,
	.Hoven = VA_COMPACT_OFFSET(0x004ef678, 0x004f1790),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ef678, 0x004e7068),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ef678, 0x004e4800),
	.Metropolis = VA_COMPACT_OFFSET(0x004ef678, 0x004e3b50),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ef678, 0x004e13e8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ef678, 0x004e13b0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ef678, 0x004e3c30),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ef678, 0x004e2f30),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ef678, 0x004e28b0),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004ecf58,
	.Hoven = VA_COMPACT_OFFSET(0x004ecf58, 0x004eefb0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ecf58, 0x004e48c8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ecf58, 0x004e20e0),
	.Metropolis = VA_COMPACT_OFFSET(0x004ecf58, 0x004e1430),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ecf58, 0x004dec48),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ecf58, 0x004dedd0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ecf58, 0x004e1610),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ecf58, 0x004e0950),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ecf58, 0x004e0290),
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
variableAddressCompact_t vaGetMaster = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004f0010,
	.Hoven = VA_COMPACT_OFFSET(0x004f0010, 0x004f2128),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f0010, 0x004e7a00),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f0010, 0x004e5198),
	.Metropolis = VA_COMPACT_OFFSET(0x004f0010, 0x004e44e8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f0010, 0x004e1d80),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f0010, 0x004e1d48),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f0010, 0x004e45c8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f0010, 0x004e38c8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f0010, 0x004e3248),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004ed8f0,
	.Hoven = VA_COMPACT_OFFSET(0x004ed8f0, 0x004ef948),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ed8f0, 0x004e5260),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ed8f0, 0x004e2a78),
	.Metropolis = VA_COMPACT_OFFSET(0x004ed8f0, 0x004e1dc8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ed8f0, 0x004df5e0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ed8f0, 0x004df768),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ed8f0, 0x004e1fa8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ed8f0, 0x004e12e8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ed8f0, 0x004e0c28),
#endif
};
variableAddressCompact_t vaCreateMaster = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004f00b0,
	.Hoven = VA_COMPACT_OFFSET(0x004f00b0, 0x004f21c8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f00b0, 0x004e7aa0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f00b0, 0x004e5238),
	.Metropolis = VA_COMPACT_OFFSET(0x004f00b0, 0x004e4588),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f00b0, 0x004e1e20),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f00b0, 0x004e1de8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f00b0, 0x004e4668),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f00b0, 0x004e3968),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f00b0, 0x004e32e8),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004ed990,
	.Hoven = VA_COMPACT_OFFSET(0x004ed990, 0x004ef9e8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ed990, 0x004e5300),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ed990, 0x004e2b18),
	.Metropolis = VA_COMPACT_OFFSET(0x004ed990, 0x004e1e68),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ed990, 0x004df680),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ed990, 0x004df808),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ed990, 0x004e2048),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ed990, 0x004e1388),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ed990, 0x004e0cc8),
#endif
};
variableAddressCompact_t vaDeleteMaster = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004f0170,
	.Hoven = VA_COMPACT_OFFSET(0x004f0170, 0x004f2288),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f0170, 0x004e7b60),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f0170, 0x004e52f8),
	.Metropolis = VA_COMPACT_OFFSET(0x004f0170, 0x004e4648),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f0170, 0x004e1ee0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f0170, 0x004e1ea8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f0170, 0x004e4728),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f0170, 0x004e3a28),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f0170, 0x004e33a8),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004eda50,
	.Hoven = VA_COMPACT_OFFSET(0x004eda50, 0x004efaa8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004eda50, 0x004e53c0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004eda50, 0x004e2bd8),
	.Metropolis = VA_COMPACT_OFFSET(0x004eda50, 0x004e1f28),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004eda50, 0x004df740),
	.CommandCenter = VA_COMPACT_OFFSET(0x004eda50, 0x004df8c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004eda50, 0x004e2108),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004eda50, 0x004e1448),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004eda50, 0x004e0d88),
#endif
};

/*
 * Returns the first guber moby.
 */
GuberMoby * guberMobyGetFirst(void)
{
    return *FIRST_GUBER_MOBY_PTR;
}
