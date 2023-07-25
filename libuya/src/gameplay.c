#include <tamtypes.h>
#include "interop.h"

VariableAddress_t vaGamplayHook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0047130c,
	.Hoven = 0x00472ebc,
	.OutpostX12 = 0x00469cbc,
    .KorgonOutpost = 0x0046784c,
	.Metropolis = 0x00466b8c,
	.BlackwaterCity = 0x004643bc,
	.CommandCenter = 0x00464cb4,
    .BlackwaterDocks = 0x00467534,
    .AquatosSewers = 0x00466834,
    .MarcadiaPalace = 0x004661b4,
#else
	.Lobby = 0,
	.Bakisi = 0x0046f7a4,
	.Hoven = 0x00471294,
	.OutpostX12 = 0x004680d4, // New Hook: 004678A4 (s1)
    .KorgonOutpost = 0x00465ce4,
	.Metropolis = 0x00465024,
	.BlackwaterCity = 0x004627d4,
	.CommandCenter = 0x0046328c,
    .BlackwaterDocks = 0x00465acc,
    .AquatosSewers = 0x00464e0c,
    .MarcadiaPalace = 0x0046474c,
#endif
};

VariableAddress_t vaGamplayHookValue = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0C11B69A,
	.Hoven = 0x0C11BD86,
	.OutpostX12 = 0x0C119906,
    .KorgonOutpost = 0x0C118FEA,
	.Metropolis = 0x0C118CBA,
	.BlackwaterCity = 0x0C1182C6,
	.CommandCenter = 0x0C118504,
    .BlackwaterDocks = 0x0C118F24,
    .AquatosSewers = 0x0C118BE4,
    .MarcadiaPalace = 0x0C118A44,
#else
	.Lobby = 0,
	.Bakisi = 0x0C11BDE9,
	.Hoven = 0x0C11C4A5,
	.OutpostX12 = 0x0C11A035,
    .KorgonOutpost = 0x0C119739,
	.Metropolis = 0x0C119409,
	.BlackwaterCity = 0x0C1189F5,
	.CommandCenter = 0x0C118CA3,
    .BlackwaterDocks = 0x0C1196B3,
    .AquatosSewers = 0x0C1196B3,
    .MarcadiaPalace = 0x0C1191D3,
#endif
};

VariableAddress_t vaGameplayFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0046da68,
	.Hoven = 0x0046f618,
	.OutpostX12 = 0x00466418,
    .KorgonOutpost = 0x00463fa8,
	.Metropolis = 0x004632e8,
	.BlackwaterCity = 0x00460b18,
	.CommandCenter = 0x00461410,
    .BlackwaterDocks = 0x00463c90,
    .AquatosSewers = 0x00462f90,
    .MarcadiaPalace = 0x00462910,
#else
	.Lobby = 0,
	.Bakisi = 0x0046bf00,
	.Hoven = 0x0046d9f0,
	.OutpostX12 = 0x00464830,
    .KorgonOutpost = 0x00462440,
	.Metropolis = 0x00461780,
	.BlackwaterCity = 0x0045ef30,
	.CommandCenter = 0x0045f9e8,
    .BlackwaterDocks = 0x00462228,
    .AquatosSewers = 0x00461568,
    .MarcadiaPalace = 0x00460ea8,
#endif
};

