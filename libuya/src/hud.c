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

VariableAddress_t vaWidgetSetScale = {
#if UYA_PAL
	.Lobby = 0x00611bb8,
	.Bakisi = 0x004e3f28,
	.Hoven = 0x004e6040,
	.OutpostX12 = 0x004db918,
	.KorgonOutpost = 0x004d90b0,
	.Metropolis = 0x004d8400,
	.BlackwaterCity = 0x004d5c98,
	.CommandCenter = 0x004d5c60,
	.BlackwaterDocks = 0x004d84e0,
	.AquatosSewers = 0x004d77e0,
	.MarcadiaPalace = 0x004d7160,
#else
	.Lobby = 0x0060f4a0,
	.Bakisi = 0x004e1868,
	.Hoven = 0x004e38c0,
	.OutpostX12 = 0x004d91d8,
	.KorgonOutpost = 0x004d69f0,
	.Metropolis = 0x004d5d40,
	.BlackwaterCity = 0x004d3558,
	.CommandCenter = 0x004d36e0,
	.BlackwaterDocks = 0x004d5f20,
	.AquatosSewers = 0x004d5260,
	.MarcadiaPalace = 0x004d4ba0,
#endif
};

VariableAddress_t vaWidgetSetPosition = {
#if UYA_PAL
	.Lobby = 0x00610908,
	.Bakisi = 0x004e3060,
	.Hoven = 0x004e5178,
	.OutpostX12 = 0x004daa50,
	.KorgonOutpost = 0x004d81e8,
	.Metropolis = 0x004d7538,
	.BlackwaterCity = 0x004d4dd0,
	.CommandCenter = 0x004d4d98,
	.BlackwaterDocks = 0x004d7618,
	.AquatosSewers = 0x004d6918,
	.MarcadiaPalace = 0x004d6298,
#else
	.Lobby = 0x0060e1f0,
	.Bakisi = 0x004e09a0,
	.Hoven = 0x004e29f8,
	.OutpostX12 = 0x004d8310,
	.KorgonOutpost = 0x004d5b28,
	.Metropolis = 0x004d4e78,
	.BlackwaterCity = 0x004d2690,
	.CommandCenter = 0x004d2818,
	.BlackwaterDocks = 0x004d5058,
	.AquatosSewers = 0x004d4398,
	.MarcadiaPalace = 0x004d3cd8,
#endif
};

VariableAddress_t vaWidgetSetColor = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004e3ba8,
	.Hoven = 0x004e5cc0,
	.OutpostX12 = 0x004db598,
	.KorgonOutpost = 0x004d8d30,
	.Metropolis = 0x004d8080,
	.BlackwaterCity = 0x004d5918,
	.CommandCenter = 0x004d58e0,
	.BlackwaterDocks = 0x004d8160,
	.AquatosSewers = 0x004d7460,
	.MarcadiaPalace = 0x004d6de0,
#else
	.Lobby = 0,
	.Bakisi = 0x004e14e8,
	.Hoven = 0x004e3540,
	.OutpostX12 = 0x004d8e58,
	.KorgonOutpost = 0x004d6670,
	.Metropolis = 0x004d59c0,
	.BlackwaterCity = 0x004d31d8,
	.CommandCenter = 0x004d3360,
	.BlackwaterDocks = 0x004d5ba0,
	.AquatosSewers = 0x004d4ee0,
	.MarcadiaPalace = 0x004d4820,
#endif
};

VariableAddress_t vaWidgetSetTexture = {
#if UYA_PAL
	.Lobby = 0x00611910,
	.Bakisi = 0x004e3c80,
	.Hoven = 0x004e5d98,
	.OutpostX12 = 0x004db670,
	.KorgonOutpost = 0x004d8e08,
	.Metropolis = 0x004d8158,
	.BlackwaterCity = 0x004d59f0,
	.CommandCenter = 0x004d59b8,
	.BlackwaterDocks = 0x004d8238,
	.AquatosSewers = 0x004d7538,
	.MarcadiaPalace = 0x004d6eb8,
#else
	.Lobby = 0x0060f1f8,
	.Bakisi = 0x004e15c0,
	.Hoven = 0x004e3618,
	.OutpostX12 = 0x004d8f30,
	.KorgonOutpost = 0x004d6748,
	.Metropolis = 0x004d5a98,
	.BlackwaterCity = 0x004d32b0,
	.CommandCenter = 0x004d3438,
	.BlackwaterDocks = 0x004d5c78,
	.AquatosSewers = 0x004d4fb8,
	.MarcadiaPalace = 0x004d48f8,
#endif
};

VariableAddress_t vaCreateWidgetRectangle = {
#if UYA_PAL
	.Lobby = 0x00612c30,
	.Bakisi = 0x004e4b58,
	.Hoven = 0x004e6c70,
	.OutpostX12 = 0x004dc548,
	.KorgonOutpost = 0x004d9ce0,
	.Metropolis = 0x004d9030,
	.BlackwaterCity = 0x004d68c8,
	.CommandCenter = 0x004d6890,
	.BlackwaterDocks = 0x004d9110,
	.AquatosSewers = 0x004d8410,
	.MarcadiaPalace = 0x004d7d90,
#else
	.Lobby = 0x00610518,
	.Bakisi = 0x004e2498,
	.Hoven = 0x004e44f0,
	.OutpostX12 = 0x004d9e08,
	.KorgonOutpost = 0x004d7620,
	.Metropolis = 0x004d6970,
	.BlackwaterCity = 0x004d4188,
	.CommandCenter = 0x004d4310,
	.BlackwaterDocks = 0x004d6b50,
	.AquatosSewers = 0x004d5e90,
	.MarcadiaPalace = 0x004d57d0,
#endif
};

VariableAddress_t vaCreateWidgetText = {
#if UYA_PAL
	.Lobby = 0x00612ae0,
	.Bakisi = 0x004e4a08,
	.Hoven = 0x004e6b20,
	.OutpostX12 = 0x004dc3f8,
	.KorgonOutpost = 0x004d9b90,
	.Metropolis = 0x004d8ee0,
	.BlackwaterCity = 0x004d6778,
	.CommandCenter = 0x004d6740,
	.BlackwaterDocks = 0x004d8fc0,
	.AquatosSewers = 0x004d82c0,
	.MarcadiaPalace = 0x004d7c40,
#else
	.Lobby = 0x006103c8,
	.Bakisi = 0x004e2348,
	.Hoven = 0x004e43a0,
	.OutpostX12 = 0x004d9cb8,
	.KorgonOutpost = 0x004d74d0,
	.Metropolis = 0x004d6820,
	.BlackwaterCity = 0x004d4038,
	.CommandCenter = 0x004d41c0,
	.BlackwaterDocks = 0x004d6a00,
	.AquatosSewers = 0x004d5d40,
	.MarcadiaPalace = 0x004d5680,
#endif
};

VariableAddress_t vaCreateWidgetTextArea = {
#if UYA_PAL
	.Lobby = 0x00612e50,
	.Bakisi = 0x004e4d78,
	.Hoven = 0x004e6e90,
	.OutpostX12 = 0x004dc768,
	.KorgonOutpost = 0x004d9f00,
	.Metropolis = 0x004d9250,
	.BlackwaterCity = 0x004d6ae8,
	.CommandCenter = 0x004d6ab0,
	.BlackwaterDocks = 0x004d9330,
	.AquatosSewers = 0x004d8630,
	.MarcadiaPalace = 0x004d7fb0,
#else
	.Lobby = 0x00610738,
	.Bakisi = 0x004e26b8,
	.Hoven = 0x004e4710,
	.OutpostX12 = 0x004da028,
	.KorgonOutpost = 0x004d7840,
	.Metropolis = 0x004d6b90,
	.BlackwaterCity = 0x004d43a8,
	.CommandCenter = 0x004d4530,
	.BlackwaterDocks = 0x004d6d70,
	.AquatosSewers = 0x004d60b0,
	.MarcadiaPalace = 0x004d59f0,
#endif
};

VariableAddress_t vaSetFrameContainerFlags = {
#if UYA_PAL
	.Lobby = 0x00611018,
	.Bakisi = 0x004e3690,
	.Hoven = 0x004e57a8,
	.OutpostX12 = 0x004db080,
	.KorgonOutpost = 0x004d8818,
	.Metropolis = 0x004d7b68,
	.BlackwaterCity = 0x004d5400,
	.CommandCenter = 0x004d53c8,
	.BlackwaterDocks = 0x004d7c48,
	.AquatosSewers = 0x004d6f48,
	.MarcadiaPalace = 0x004d68c8,
#else
	.Lobby = 0x0060e900,
	.Bakisi = 0x004e0fd0,
	.Hoven = 0x004e3028,
	.OutpostX12 = 0x004d8940,
	.KorgonOutpost = 0x004d6158,
	.Metropolis = 0x004d54a8,
	.BlackwaterCity = 0x004d2cc0,
	.CommandCenter = 0x004d2e48,
	.BlackwaterDocks = 0x004d5688,
	.AquatosSewers = 0x004d49c8,
	.MarcadiaPalace = 0x004d4308,
#endif
};

VariableAddress_t vaSetTextScale = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004e3408,
	.Hoven = 0x004e5520,
	.OutpostX12 = 0x004dadf8,
	.KorgonOutpost = 0x004d8590,
	.Metropolis = 0x004d78e0,
	.BlackwaterCity = 0x004d5178,
	.CommandCenter = 0x004d5140,
	.BlackwaterDocks = 0x004d79c0,
	.AquatosSewers = 0x004d6cc0,
	.MarcadiaPalace = 0x004d6640,
#else
	.Lobby = 0,
	.Bakisi = 0x004e0d48,
	.Hoven = 0x004e2da0,
	.OutpostX12 = 0x004d86b8,
	.KorgonOutpost = 0x004d5ed0,
	.Metropolis = 0x004d5220,
	.BlackwaterCity = 0x004d2a38,
	.CommandCenter = 0x004d2bc0,
	.BlackwaterDocks = 0x004d5400,
	.AquatosSewers = 0x004d4740,
	.MarcadiaPalace = 0x004d4080,
#endif
};

VariableAddress_t vaAddFrameToContainer = {
#if UYA_PAL
	.Lobby = 0x00612238,
	.Bakisi = 0x004e42c0,
	.Hoven = 0x004e63d8,
	.OutpostX12 = 0x004dbcb0,
	.KorgonOutpost = 0x004d9448,
	.Metropolis = 0x004d8798,
	.BlackwaterCity = 0x004d6030,
	.CommandCenter = 0x004d5ff8,
	.BlackwaterDocks = 0x004d8878,
	.AquatosSewers = 0x004d7b78,
	.MarcadiaPalace = 0x004d74f8,
#else
	.Lobby = 0x0060fb20,
	.Bakisi = 0x004e1c00,
	.Hoven = 0x004e3c58,
	.OutpostX12 = 0x004d9570,
	.KorgonOutpost = 0x004d6d88,
	.Metropolis = 0x004d60d8,
	.BlackwaterCity = 0x004d38f0,
	.CommandCenter = 0x004d3a78,
	.BlackwaterDocks = 0x004d62b8,
	.AquatosSewers = 0x004d55f8,
	.MarcadiaPalace = 0x004d4f38,
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
