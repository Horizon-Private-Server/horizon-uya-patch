#include "interop.h"
#include "hud.h"

#define HUD_RADAR_PLAYERDATA ((HudRadar_PlayerData_t*)GetAddress(&vaHudRadar_PlayerData))

VariableAddress_t vaHudRadar_PlayerData = {
#if UYA_PAL
	.Lobby = 0x017515ec,
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

HudRadar_PlayerData_t *hudGetMapSettings(void)
{
    return HUD_RADAR_PLAYERDATA;
}
