#include <tamtypes.h>
#include "interop.h"

VariableAddress_t vaCollLineFix = {
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
	.Bakisi = 0x0044c290,
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
