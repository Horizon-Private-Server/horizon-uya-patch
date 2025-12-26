#include "interop.h"
#include "hud.h"

#define HUD_RADAR_PLAYERDATA ((HudMap_t*)GetAddress(&vaHudRadar_PlayerData))
#define ENGINE_INSTANCE ((Engine_t*)GetAddress(&vaEngine))
#define ENGINE_DATA ((EngineData_t*)((u32)GetAddress(&vaEngine) + 0x4))

VariableAddress_t vaHudRadar_PlayerData = {
#if UYA_PAL
	.Lobby = 0x00499ff0,
	.Bakisi = 0x003c0630,
	.Hoven = 0x003bfc70,
	.OutpostX12 = 0x003b7b70,
	.KorgonOutpost = 0x003b7930,
	.Metropolis = 0x003b76b0,
	.BlackwaterCity = 0x003b78f0,
	.CommandCenter = 0x003c7e70,
	.BlackwaterDocks = 0x003c8270,
	.AquatosSewers = 0x003c89f0,
	.MarcadiaPalace = 0x003c7f70,
#else
	.Lobby = 0x0049a0b0,
	.Bakisi = 0x003c0770,
	.Hoven = 0x003bfdb0,
	.OutpostX12 = 0x003b7cb0,
	.KorgonOutpost = 0x003b7a70,
	.Metropolis = 0x003b77f0,
	.BlackwaterCity = 0x003b7a30,
	.CommandCenter = 0x003c7fb0,
	.BlackwaterDocks = 0x003c83b0,
	.AquatosSewers = 0x003c8b30,
	.MarcadiaPalace = 0x003c80b0,
#endif
};

VariableAddress_t vaGetCurrentCanvas_JAL = {
#if UYA_PAL
	.Lobby = 0x005dd86c,
	.Bakisi = 0x004aff54,
	.Hoven = 0x004b206c,
	.OutpostX12 = 0x004a7944,
	.KorgonOutpost = 0x004a50dc,
	.Metropolis = 0x004a442c,
	.BlackwaterCity = 0x004a1cc4,
	.CommandCenter = 0x004a1cbc,
	.BlackwaterDocks = 0x004a453c,
	.AquatosSewers = 0x004a383c,
	.MarcadiaPalace = 0x004a31bc,
#else
	.Lobby = 0x005db2c4,
	.Bakisi = 0x004ada04,
	.Hoven = 0x004afa5c,
	.OutpostX12 = 0x004a5374,
	.KorgonOutpost = 0x004a2b8c,
	.Metropolis = 0x004a1edc,
	.BlackwaterCity = 0x0049f6f4,
	.CommandCenter = 0x0049f8ac,
	.BlackwaterDocks = 0x004a20ec,
	.AquatosSewers = 0x004a142c,
	.MarcadiaPalace = 0x004a0d6c,
#endif
};

VariableAddress_t vaEngine = {
#if UYA_PAL
	.Lobby = 0x0024cd50,
	.Bakisi = 0x00249260,
	.Hoven = 0x00249460,
	.OutpostX12 = 0x00249350,
	.KorgonOutpost = 0x002491d0,
	.Metropolis = 0x00249250,
	.BlackwaterCity = 0x002491d0,
	.CommandCenter = 0x00248dd0,
	.BlackwaterDocks = 0x00248ed0,
	.AquatosSewers = 0x00248ed0,
	.MarcadiaPalace = 0x00248ed0,
#else
	.Lobby = 0x0024ce50,
	.Bakisi = 0x002493e0,
	.Hoven = 0x002495e0,
	.OutpostX12 = 0x002494d0,
	.KorgonOutpost = 0x00249350,
	.Metropolis = 0x002493d0,
	.BlackwaterCity = 0x00249350,
	.CommandCenter = 0x00248f50,
	.BlackwaterDocks = 0x00249050,
	.AquatosSewers = 0x00249050,
	.MarcadiaPalace = 0x00249050,
#endif
};

VariableAddress_t vaGetPreLoadedImageBufferSource = {
#if UYA_PAL
    .Lobby = 0x005e7d20,
    .Bakisi = 0x004ba3c0,
    .Hoven = 0x004bc4d8,
    .OutpostX12 = 0x004b1db0,
    .KorgonOutpost = 0x004af548,
    .Metropolis = 0x004ae898,
    .BlackwaterCity = 0x004ac130,
    .CommandCenter = 0x004ac128,
    .BlackwaterDocks = 0x004ae9a8,
    .AquatosSewers = 0x004adca8,
    .MarcadiaPalace = 0x004ad628,
#else
    .Lobby = 0x005e57b0,
    .Bakisi = 0x004b7ea8,
    .Hoven = 0x004b9f00,
    .OutpostX12 = 0x004af818,
    .KorgonOutpost = 0x004ad030,
    .Metropolis = 0x004ac380,
    .BlackwaterCity = 0x004a9b98,
    .CommandCenter = 0x004a9d50,
    .BlackwaterDocks = 0x004ac590,
    .AquatosSewers = 0x004ab8d0,
    .MarcadiaPalace = 0x004ab210,
#endif
};

HudMap_t *hudGetMapData(void)
{
    return HUD_RADAR_PLAYERDATA;
}

Engine_t *hudGetInstance(void)
{
	return ENGINE_INSTANCE;
}

EngineData_t *hudGetEngineData(void)
{
	return ENGINE_DATA;
}

static Engine_t *hudInitInstance(void)
{
	Engine_t *instance = ENGINE_INSTANCE;
	EngineData_t *data = ENGINE_DATA;
	instance->data = data;
	memset(data->heaps, 0, 8);
	memset(data, 0, 0x14);
	memset(data->data_source, 0, 0x30);
	memset(data->postdraws, 0, 0x10);
	memset(data->predraws, 0, 0x10);
	data->postdraw_counter = 0;
	data->predraw_counter = 0;
	data->current_canvas = 0;
	return instance;
}

int hudGetCurrentCanvas(void)
{
	Engine_t *instance = ENGINE_INSTANCE;
	if (instance->data == NULL)
		instance = hudInitInstance();
	if (instance->data == NULL)
		return -1;
	
	return instance->data->current_canvas;
}

