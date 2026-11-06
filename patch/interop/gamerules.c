#include <libuya/interop.h>

//================================================
//=============  GameplayHook
//================================================
variableAddressCompact_t vaGameplayHook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0046db20),
	.Hoven = VA_COMPACT_OFFSET(0x0046db20, 0x0046f6d0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0046db20, 0x004664d0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0046db20, 0x00464060),
	.Metropolis = VA_COMPACT_OFFSET(0x0046db20, 0x004633a0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0046db20, 0x00460bd0),
	.CommandCenter = VA_COMPACT_OFFSET(0x0046db20, 0x004614c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0046db20, 0x00463d48),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0046db20, 0x00463048),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0046db20, 0x004629c8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0046bfb8),
	.Hoven = VA_COMPACT_OFFSET(0x0046bfb8, 0x0046daa8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0046bfb8, 0x004648e8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0046bfb8, 0x004624f8),
	.Metropolis = VA_COMPACT_OFFSET(0x0046bfb8, 0x00461838),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0046bfb8, 0x0045efe8),
	.CommandCenter = VA_COMPACT_OFFSET(0x0046bfb8, 0x0045faa0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0046bfb8, 0x004622e0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0046bfb8, 0x00461620),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0046bfb8, 0x00460f60),
#endif
};

//================================================
//=============  vampireLogic
//================================================
variableAddressCompact_t vaUpdateScoreboard = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00544488),
	.Hoven = VA_COMPACT_OFFSET(0x00544488, 0x00546650),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00544488, 0x0053bf28),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00544488, 0x00539610),
	.Metropolis = VA_COMPACT_OFFSET(0x00544488, 0x00538a10),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00544488, 0x005361f8),
	.CommandCenter = VA_COMPACT_OFFSET(0x00544488, 0x00535a50),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00544488, 0x005382d0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00544488, 0x005375d0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00544488, 0x00536f50),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00541b78),
	.Hoven = VA_COMPACT_OFFSET(0x00541b78, 0x00543c80),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00541b78, 0x00539598),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00541b78, 0x00536d00),
	.Metropolis = VA_COMPACT_OFFSET(0x00541b78, 0x00536100),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00541b78, 0x00533868),
	.CommandCenter = VA_COMPACT_OFFSET(0x00541b78, 0x00533298),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00541b78, 0x00535ad8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00541b78, 0x00534e18),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00541b78, 0x00534758),
#endif
};