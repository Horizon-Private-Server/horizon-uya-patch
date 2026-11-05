#include "sound.h"
#include "interop.h"

variableAddressCompact_t vaSoundHeroPlayFunc = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x0050bf28,
	.Hoven = VA_COMPACT_OFFSET(0x0050bf28, 0x0050e040),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0050bf28, 0x00503918),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0050bf28, 0x005010b0),
	.Metropolis = VA_COMPACT_OFFSET(0x0050bf28, 0x00500400),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0050bf28, 0x004fdc98),
	.CommandCenter = VA_COMPACT_OFFSET(0x0050bf28, 0x004fdc60),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0050bf28, 0x005004e0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0050bf28, 0x004ff7e0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0050bf28, 0x004ff160),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00509738,
	.Hoven = VA_COMPACT_OFFSET(0x00509738, 0x0050b790),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00509738, 0x005010a8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00509738, 0x004fe8c0),
	.Metropolis = VA_COMPACT_OFFSET(0x00509738, 0x004fdc10),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00509738, 0x004fb428),
	.CommandCenter = VA_COMPACT_OFFSET(0x00509738, 0x004fb5b0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00509738, 0x004fddf0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00509738, 0x004fd130),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00509738, 0x004fca70),
#endif
};

variableAddressCompact_t vaSoundMobyPlayFunc = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004c5a80,
	.Hoven = VA_COMPACT_OFFSET(0x004c5a80, 0x004c7b98),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004c5a80, 0x004bd470),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004c5a80, 0x004bac08),
	.Metropolis = VA_COMPACT_OFFSET(0x004c5a80, 0x004b9f58),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004c5a80, 0x004b77f0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004c5a80, 0x004b77e8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004c5a80, 0x004ba068),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004c5a80, 0x004b9368),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004c5a80, 0x004b8ce8),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004c33d8,
	.Hoven = VA_COMPACT_OFFSET(0x004c33d8, 0x004c5430),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004c33d8, 0x004bad48),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004c33d8, 0x004b8560),
	.Metropolis = VA_COMPACT_OFFSET(0x004c33d8, 0x004b78b0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004c33d8, 0x004b50c8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004c33d8, 0x004b5280),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004c33d8, 0x004b7ac0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004c33d8, 0x004b6e00),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004c33d8, 0x004b6740),
#endif
};

variableAddressCompact_t vaSoundPlayByOClassFunc = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x005f3488),
	.Bakisi = 0x004c5b28,
	.Hoven = VA_COMPACT_OFFSET(0x004c5b28, 0x004c7c40),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004c5b28, 0x004bd518),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004c5b28, 0x004bacb0),
	.Metropolis = VA_COMPACT_OFFSET(0x004c5b28, 0x004ba000),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004c5b28, 0x004b7898),
	.CommandCenter = VA_COMPACT_OFFSET(0x004c5b28, 0x004b7890),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004c5b28, 0x004ba110),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004c5b28, 0x004b9410),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004c5b28, 0x004b8d90),
#else
	.Lobby = VA_COMPACT_LOBBY(0x005f0d88),
	.Bakisi = 0x004c3480,
	.Hoven = VA_COMPACT_OFFSET(0x004c3480, 0x004c54d8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004c3480, 0x004badf0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004c3480, 0x004b8608),
	.Metropolis = VA_COMPACT_OFFSET(0x004c3480, 0x004b7958),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004c3480, 0x004b5170),
	.CommandCenter = VA_COMPACT_OFFSET(0x004c3480, 0x004b5328),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004c3480, 0x004b7b68),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004c3480, 0x004b6ea8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004c3480, 0x004b67e8),
#endif
};

variableAddressCompact_t vaSoundPlayFunc = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004c57d8,
	.Hoven = VA_COMPACT_OFFSET(0x004c57d8, 0x004c78f0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004c57d8, 0x004bd1c8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004c57d8, 0x004ba960),
	.Metropolis = VA_COMPACT_OFFSET(0x004c57d8, 0x004b9cb0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004c57d8, 0x004b7548),
	.CommandCenter = VA_COMPACT_OFFSET(0x004c57d8, 0x004b7540),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004c57d8, 0x004b9dc0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004c57d8, 0x004b90c0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004c57d8, 0x004b8a40),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004c3130,
	.Hoven = VA_COMPACT_OFFSET(0x004c3130, 0x004c5188),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004c3130, 0x004baaa0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004c3130, 0x004b82b8),
	.Metropolis = VA_COMPACT_OFFSET(0x004c3130, 0x004b7608),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004c3130, 0x004b4e20),
	.CommandCenter = VA_COMPACT_OFFSET(0x004c3130, 0x004b4fd8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004c3130, 0x004b7818),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004c3130, 0x004b6b58),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004c3130, 0x004b6498),
#endif
};
