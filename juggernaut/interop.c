#include <tamtypes.h>
#include <libuya/interop.h>

VariableAddress_t vaOnPlayerKill_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004fa1ec,
	.Hoven = 0x004fc304,
	.OutpostX12 = 0x004f1bdc,
	.KorgonOutpost = 0x004ef374,
	.Metropolis = 0x004ee6c4,
	.BlackwaterCity = 0x004ebf5c,
	.CommandCenter = 0x004ebf24,
	.BlackwaterDocks = 0x004ee7a4,
	.AquatosSewers = 0x004edaa4,
	.MarcadiaPalace = 0x004ed424,
#else
	.Lobby = 0,
	.Bakisi = 0x004f7a6c,
	.Hoven = 0x004f9ac4,
	.OutpostX12 = 0x004ef3dc,
	.KorgonOutpost = 0x004ecbf4,
	.Metropolis = 0x004ebf44,
	.BlackwaterCity = 0x004e975c,
	.CommandCenter = 0x004e98e4,
	.BlackwaterDocks = 0x004ec124,
	.AquatosSewers = 0x004eb464,
	.MarcadiaPalace = 0x004eada4,
#endif
};

VariableAddress_t vaOnPlayerKill_Func = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00544488,
	.Hoven = 0x00546650,
	.OutpostX12 = 0x0053bf28,
	.KorgonOutpost = 0x00539610,
	.Metropolis = 0x00538a10,
	.BlackwaterCity = 0x005361f8,
	.CommandCenter = 0x00535a50,
	.BlackwaterDocks = 0x005382d0,
	.AquatosSewers = 0x005375d0,
	.MarcadiaPalace = 0x00536f50,
#else
	.Lobby = 0,
	.Bakisi = 0x00541b78,
	.Hoven = 0x00543c80,
	.OutpostX12 = 0x00539598,
	.KorgonOutpost = 0x00536d00,
	.Metropolis = 0x00536100,
	.BlackwaterCity = 0x00533868,
	.CommandCenter = 0x00533298,
	.BlackwaterDocks = 0x00535ad8,
	.AquatosSewers = 0x00534e18,
	.MarcadiaPalace = 0x00534758,
#endif
};

VariableAddress_t vaWhoHitMe_Hook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0052916c,
    .Hoven = 0x0052b284,
    .OutpostX12 = 0x00520b5c,
    .KorgonOutpost = 0x0051e2f4,
    .Metropolis = 0x0051d644,
    .BlackwaterCity = 0x0051aedc,
    .CommandCenter = 0x0051ac9c,
    .BlackwaterDocks = 0x0051d51c,
    .AquatosSewers = 0x0051c81c,
    .MarcadiaPalace = 0x0051c19c,
#else
    .Lobby = 0,
    .Bakisi = 0x005268ec,
    .Hoven = 0x00528944,
    .OutpostX12 = 0x0051e25c,
    .KorgonOutpost = 0x0051ba74,
    .Metropolis = 0x0051adc4,
    .BlackwaterCity = 0x005185dc,
    .CommandCenter = 0x0051855c,
    .BlackwaterDocks = 0x0051ad9c,
    .AquatosSewers = 0x0051a0dc,
    .MarcadiaPalace = 0x00519a1c,
#endif
};

VariableAddress_t vaWhoHitMe_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004f8c30,
    .Hoven = 0x004fad48,
    .OutpostX12 = 0x004f0620,
    .KorgonOutpost = 0x004eddb8,
    .Metropolis = 0x004ed108,
    .BlackwaterCity = 0x004ea9a0,
    .CommandCenter = 0x004ea968,
    .BlackwaterDocks = 0x004ed1e8,
    .AquatosSewers = 0x004ec4e8,
    .MarcadiaPalace = 0x004ebe68,
#else
    .Lobby = 0,
    .Bakisi = 0x004f64b0,
    .Hoven = 0x004f8508,
    .OutpostX12 = 0x004ede20,
    .KorgonOutpost = 0x004eb638,
    .Metropolis = 0x004ea988,
    .BlackwaterCity = 0x004e81a0,
    .CommandCenter = 0x004e8328,
    .BlackwaterDocks = 0x004eab68,
    .AquatosSewers = 0x004e9ea8,
    .MarcadiaPalace = 0x004e97e8,
#endif
};
