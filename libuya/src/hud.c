#include "interop.h"
#include "hud.h"

#define HUD_RADAR_PLAYERDATA ((HudMap_t*)GetAddress(&vaHudRadar_PlayerData))
#define ENGINE_INSTANCE ((Engine_t*)GetAddress(&vaEngine))
#define ENGINE_DATA ((EngineData_t*)((u32)GetAddress(&vaEngine) + 0x4))

static const HudEntry_t hud_radar[] = {
    // Root
    {HUD_RADAR_ROOT, HUD_RADAR_MAP_CONTAINER},
    {HUD_RADAR_ROOT, HUD_RADAR_NODE_CONTAINER},
    {HUD_RADAR_ROOT, HUD_RADAR_VEHICLE_CONTAINER},
    {HUD_RADAR_ROOT, HUD_RADAR_PLAYER_CONTAINER},
    {HUD_RADAR_ROOT, HUD_RADAR_TEXT_CONTAINER},
    // Map container children
    {HUD_RADAR_MAP_CONTAINER, HUD_RADAR_MAP_BG},
    {HUD_RADAR_MAP_CONTAINER, HUD_RADAR_CURSOR_SELECT},
    {HUD_RADAR_MAP_CONTAINER, HUD_RADAR_CURSOR_PLAYER},
    {HUD_RADAR_MAP_CONTAINER, HUD_RADAR_BASE_TEAM1},
    {HUD_RADAR_MAP_CONTAINER, HUD_RADAR_BASE_TEAM2},
    // Text container children
    {HUD_RADAR_TEXT_CONTAINER, HUD_RADAR_TEXT_RESPAWN},
    {HUD_RADAR_TEXT_CONTAINER, HUD_RADAR_TEXT_NODE_SEL},
};

static const HudEntry_t hud_siege_ctf[] = {
    // Root containers
    {HUD_SIEGE_ROOT, HUD_SIEGE_MAIN_CONTAINER},
    {HUD_SIEGE_ROOT, HUD_CTF_CONTAINER},
    {HUD_SIEGE_ROOT, HUD_SIEGE_FLAG_CONTAINER},
    
    // Main container children (Siege score display)
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_FRAME_BG_2},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_FRAME_BG_1},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_BAR_BG_2},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_BAR_FG_2},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_ICON_1},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_BAR_RED_BG},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_BAR_BLUE_BG},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_BAR_BLUE},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_BAR_RED},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_NODE_1},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_NODE_2},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_NODE_3},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_NODE_4},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_NODE_5},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_NODE_6},
    {HUD_SIEGE_MAIN_CONTAINER, HUD_SIEGE_NODE_7},
    
    // CTF container children
    {HUD_CTF_CONTAINER, HUD_CTF_FRAME_BG_2},
    {HUD_CTF_CONTAINER, HUD_CTF_BAR_BLUE},
    {HUD_CTF_CONTAINER, HUD_CTF_BAR_RED},
    {HUD_CTF_CONTAINER, HUD_CTF_ICON},
    {HUD_CTF_CONTAINER, HUD_CTF_FRAME_BG_1},
    {HUD_CTF_CONTAINER, HUD_CTF_TEXT_1},
    {HUD_CTF_CONTAINER, HUD_CTF_TEXT_2},
    
    // Flag container children
    {HUD_SIEGE_FLAG_CONTAINER, HUD_SIEGE_FLAG_8},
    {HUD_SIEGE_FLAG_CONTAINER, HUD_SIEGE_FLAG_1},
    {HUD_SIEGE_FLAG_CONTAINER, HUD_SIEGE_FLAG_2},
    {HUD_SIEGE_FLAG_CONTAINER, HUD_SIEGE_FLAG_3},
    {HUD_SIEGE_FLAG_CONTAINER, HUD_SIEGE_FLAG_4},
    {HUD_SIEGE_FLAG_CONTAINER, HUD_SIEGE_FLAG_5},
    {HUD_SIEGE_FLAG_CONTAINER, HUD_SIEGE_FLAG_6},
    {HUD_SIEGE_FLAG_CONTAINER, HUD_SIEGE_FLAG_7},
};

variableAddressCompact_t vaHudRadar_PlayerData = {
#if UYA_PAL
	.Lobby = 0x00499ff0,
	.Bakisi = VA_COMPACT_BASE(0x003c0630),
	.Hoven = VA_COMPACT_OFFSET(0x003c0630, 0x003bfc70),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003c0630, 0x003b7b70),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003c0630, 0x003b7930),
	.Metropolis = VA_COMPACT_OFFSET(0x003c0630, 0x003b76b0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003c0630, 0x003b78f0),
	.CommandCenter = VA_COMPACT_OFFSET(0x003c0630, 0x003c7e70),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003c0630, 0x003c8270),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003c0630, 0x003c89f0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003c0630, 0x003c7f70),
#else
	.Lobby = 0x0049a0b0,
	.Bakisi = VA_COMPACT_BASE(0x003c0770),
	.Hoven = VA_COMPACT_OFFSET(0x003c0770, 0x003bfdb0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003c0770, 0x003b7cb0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003c0770, 0x003b7a70),
	.Metropolis = VA_COMPACT_OFFSET(0x003c0770, 0x003b77f0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003c0770, 0x003b7a30),
	.CommandCenter = VA_COMPACT_OFFSET(0x003c0770, 0x003c7fb0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003c0770, 0x003c83b0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003c0770, 0x003c8b30),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003c0770, 0x003c80b0),
#endif
};

variableAddressCompact_t vaGetCurrentCanvas_JAL = {
#if UYA_PAL
	.Lobby = 0x005dd86c,
	.Bakisi = VA_COMPACT_BASE(0x004aff54),
	.Hoven = VA_COMPACT_OFFSET(0x004aff54, 0x004b206c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004aff54, 0x004a7944),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004aff54, 0x004a50dc),
	.Metropolis = VA_COMPACT_OFFSET(0x004aff54, 0x004a442c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004aff54, 0x004a1cc4),
	.CommandCenter = VA_COMPACT_OFFSET(0x004aff54, 0x004a1cbc),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004aff54, 0x004a453c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004aff54, 0x004a383c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004aff54, 0x004a31bc),
#else
	.Lobby = 0x005db2c4,
	.Bakisi = VA_COMPACT_BASE(0x004ada04),
	.Hoven = VA_COMPACT_OFFSET(0x004ada04, 0x004afa5c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ada04, 0x004a5374),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ada04, 0x004a2b8c),
	.Metropolis = VA_COMPACT_OFFSET(0x004ada04, 0x004a1edc),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ada04, 0x0049f6f4),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ada04, 0x0049f8ac),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ada04, 0x004a20ec),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ada04, 0x004a142c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ada04, 0x004a0d6c),
#endif
};

variableAddressCompact_t vaEngine = {
#if UYA_PAL
	.Lobby = 0x0024cd50,
	.Bakisi = VA_COMPACT_BASE(0x00249260),
	.Hoven = VA_COMPACT_OFFSET(0x00249260, 0x00249460),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00249260, 0x00249350),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00249260, 0x002491d0),
	.Metropolis = VA_COMPACT_OFFSET(0x00249260, 0x00249250),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00249260, 0x002491d0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00249260, 0x00248dd0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00249260, 0x00248ed0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00249260, 0x00248ed0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00249260, 0x00248ed0),
#else
	.Lobby = 0x0024ce50,
	.Bakisi = VA_COMPACT_BASE(0x002493e0),
	.Hoven = VA_COMPACT_OFFSET(0x002493e0, 0x002495e0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x002493e0, 0x002494d0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x002493e0, 0x00249350),
	.Metropolis = VA_COMPACT_OFFSET(0x002493e0, 0x002493d0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x002493e0, 0x00249350),
	.CommandCenter = VA_COMPACT_OFFSET(0x002493e0, 0x00248f50),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x002493e0, 0x00249050),
	.AquatosSewers = VA_COMPACT_OFFSET(0x002493e0, 0x00249050),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x002493e0, 0x00249050),
#endif
};

variableAddressCompact_t vaGetPreLoadedImageBufferSource = {
#if UYA_PAL
	.Lobby = 0x005e7d20,
	.Bakisi = VA_COMPACT_BASE(0x004ba3c0),
	.Hoven = VA_COMPACT_OFFSET(0x004ba3c0, 0x004bc4d8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ba3c0, 0x004b1db0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ba3c0, 0x004af548),
	.Metropolis = VA_COMPACT_OFFSET(0x004ba3c0, 0x004ae898),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ba3c0, 0x004ac130),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ba3c0, 0x004ac128),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ba3c0, 0x004ae9a8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ba3c0, 0x004adca8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ba3c0, 0x004ad628),
#else
	.Lobby = 0x005e57b0,
	.Bakisi = VA_COMPACT_BASE(0x004b7ea8),
	.Hoven = VA_COMPACT_OFFSET(0x004b7ea8, 0x004b9f00),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004b7ea8, 0x004af818),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004b7ea8, 0x004ad030),
	.Metropolis = VA_COMPACT_OFFSET(0x004b7ea8, 0x004ac380),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004b7ea8, 0x004a9b98),
	.CommandCenter = VA_COMPACT_OFFSET(0x004b7ea8, 0x004a9d50),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004b7ea8, 0x004ac590),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004b7ea8, 0x004ab8d0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004b7ea8, 0x004ab210),
#endif
};

variableAddressCompact_t vaWidgetSetScale = {
#if UYA_PAL
	.Lobby = 0x00611bb8,
	.Bakisi = VA_COMPACT_BASE(0x004e3f28),
	.Hoven = VA_COMPACT_OFFSET(0x004e3f28, 0x004e6040),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e3f28, 0x004db918),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e3f28, 0x004d90b0),
	.Metropolis = VA_COMPACT_OFFSET(0x004e3f28, 0x004d8400),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e3f28, 0x004d5c98),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e3f28, 0x004d5c60),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e3f28, 0x004d84e0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e3f28, 0x004d77e0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e3f28, 0x004d7160),
#else
	.Lobby = 0x0060f4a0,
	.Bakisi = VA_COMPACT_BASE(0x004e1868),
	.Hoven = VA_COMPACT_OFFSET(0x004e1868, 0x004e38c0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e1868, 0x004d91d8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e1868, 0x004d69f0),
	.Metropolis = VA_COMPACT_OFFSET(0x004e1868, 0x004d5d40),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e1868, 0x004d3558),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e1868, 0x004d36e0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e1868, 0x004d5f20),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e1868, 0x004d5260),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e1868, 0x004d4ba0),
#endif
};

variableAddressCompact_t vaWidgetSetPosition = {
#if UYA_PAL
	.Lobby = 0x00610908,
	.Bakisi = VA_COMPACT_BASE(0x004e3060),
	.Hoven = VA_COMPACT_OFFSET(0x004e3060, 0x004e5178),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e3060, 0x004daa50),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e3060, 0x004d81e8),
	.Metropolis = VA_COMPACT_OFFSET(0x004e3060, 0x004d7538),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e3060, 0x004d4dd0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e3060, 0x004d4d98),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e3060, 0x004d7618),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e3060, 0x004d6918),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e3060, 0x004d6298),
#else
	.Lobby = 0x0060e1f0,
	.Bakisi = VA_COMPACT_BASE(0x004e09a0),
	.Hoven = VA_COMPACT_OFFSET(0x004e09a0, 0x004e29f8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e09a0, 0x004d8310),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e09a0, 0x004d5b28),
	.Metropolis = VA_COMPACT_OFFSET(0x004e09a0, 0x004d4e78),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e09a0, 0x004d2690),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e09a0, 0x004d2818),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e09a0, 0x004d5058),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e09a0, 0x004d4398),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e09a0, 0x004d3cd8),
#endif
};

variableAddressCompact_t vaWidgetSetColor = {
#if UYA_PAL
	.Lobby = 0x00611838,
	.Bakisi = VA_COMPACT_BASE(0x004e3ba8),
	.Hoven = VA_COMPACT_OFFSET(0x004e3ba8, 0x004e5cc0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e3ba8, 0x004db598),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e3ba8, 0x004d8d30),
	.Metropolis = VA_COMPACT_OFFSET(0x004e3ba8, 0x004d8080),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e3ba8, 0x004d5918),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e3ba8, 0x004d58e0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e3ba8, 0x004d8160),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e3ba8, 0x004d7460),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e3ba8, 0x004d6de0),
#else
	.Lobby = 0x0060f120,
	.Bakisi = VA_COMPACT_BASE(0x004e14e8),
	.Hoven = VA_COMPACT_OFFSET(0x004e14e8, 0x004e3540),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e14e8, 0x004d8e58),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e14e8, 0x004d6670),
	.Metropolis = VA_COMPACT_OFFSET(0x004e14e8, 0x004d59c0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e14e8, 0x004d31d8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e14e8, 0x004d3360),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e14e8, 0x004d5ba0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e14e8, 0x004d4ee0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e14e8, 0x004d4820),
#endif
};

variableAddressCompact_t vaWidgetSetTexture = {
#if UYA_PAL
	.Lobby = 0x00611910,
	.Bakisi = VA_COMPACT_BASE(0x004e3c80),
	.Hoven = VA_COMPACT_OFFSET(0x004e3c80, 0x004e5d98),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e3c80, 0x004db670),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e3c80, 0x004d8e08),
	.Metropolis = VA_COMPACT_OFFSET(0x004e3c80, 0x004d8158),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e3c80, 0x004d59f0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e3c80, 0x004d59b8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e3c80, 0x004d8238),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e3c80, 0x004d7538),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e3c80, 0x004d6eb8),
#else
	.Lobby = 0x0060f1f8,
	.Bakisi = VA_COMPACT_BASE(0x004e15c0),
	.Hoven = VA_COMPACT_OFFSET(0x004e15c0, 0x004e3618),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e15c0, 0x004d8f30),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e15c0, 0x004d6748),
	.Metropolis = VA_COMPACT_OFFSET(0x004e15c0, 0x004d5a98),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e15c0, 0x004d32b0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e15c0, 0x004d3438),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e15c0, 0x004d5c78),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e15c0, 0x004d4fb8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e15c0, 0x004d48f8),
#endif
};

variableAddressCompact_t vaCreateWidgdetFrameContainer = {
#if UYA_PAL
	.Lobby = 0x00612f48,
	.Bakisi = VA_COMPACT_BASE(0x004e4e70),
	.Hoven = VA_COMPACT_OFFSET(0x004e4e70, 0x004e6f88),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e4e70, 0x004dc860),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e4e70, 0x004d9ff8),
	.Metropolis = VA_COMPACT_OFFSET(0x004e4e70, 0x004d9348),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e4e70, 0x004d6be0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e4e70, 0x004d6ba8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e4e70, 0x004d9428),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e4e70, 0x004d8728),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e4e70, 0x004d80a8),
#else
	.Lobby = 0x00610830,
	.Bakisi = VA_COMPACT_BASE(0x004e27b0),
	.Hoven = VA_COMPACT_OFFSET(0x004e27b0, 0x004e4808),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e27b0, 0x004da120),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e27b0, 0x004d7938),
	.Metropolis = VA_COMPACT_OFFSET(0x004e27b0, 0x004d6c88),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e27b0, 0x004d44a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e27b0, 0x004d4628),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e27b0, 0x004d6e68),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e27b0, 0x004d61a8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e27b0, 0x004d5ae8),
#endif
};

variableAddressCompact_t vaCreateWidget3d_2D = {
#if UYA_PAL
	.Lobby = 0x006129e8,
	.Bakisi = VA_COMPACT_BASE(0x004e4910),
	.Hoven = VA_COMPACT_OFFSET(0x004e4910, 0x004e6a28),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e4910, 0x004dc300),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e4910, 0x004d9a98),
	.Metropolis = VA_COMPACT_OFFSET(0x004e4910, 0x004d8de8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e4910, 0x004d6680),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e4910, 0x004d6648),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e4910, 0x004d8ec8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e4910, 0x004d81c8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e4910, 0x004d7b48),
#else
	.Lobby = 0x006102d0,
	.Bakisi = VA_COMPACT_BASE(0x004e2250),
	.Hoven = VA_COMPACT_OFFSET(0x004e2250, 0x004e42a8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e2250, 0x004d9bc0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e2250, 0x004d73d8),
	.Metropolis = VA_COMPACT_OFFSET(0x004e2250, 0x004d6728),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e2250, 0x004d3f40),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e2250, 0x004d40c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e2250, 0x004d6908),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e2250, 0x004d5c48),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e2250, 0x004d5588),
#endif
};

variableAddressCompact_t vaCreateWidgetRectangle = {
#if UYA_PAL
	.Lobby = 0x00612c30,
	.Bakisi = VA_COMPACT_BASE(0x004e4b58),
	.Hoven = VA_COMPACT_OFFSET(0x004e4b58, 0x004e6c70),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e4b58, 0x004dc548),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e4b58, 0x004d9ce0),
	.Metropolis = VA_COMPACT_OFFSET(0x004e4b58, 0x004d9030),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e4b58, 0x004d68c8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e4b58, 0x004d6890),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e4b58, 0x004d9110),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e4b58, 0x004d8410),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e4b58, 0x004d7d90),
#else
	.Lobby = 0x00610518,
	.Bakisi = VA_COMPACT_BASE(0x004e2498),
	.Hoven = VA_COMPACT_OFFSET(0x004e2498, 0x004e44f0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e2498, 0x004d9e08),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e2498, 0x004d7620),
	.Metropolis = VA_COMPACT_OFFSET(0x004e2498, 0x004d6970),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e2498, 0x004d4188),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e2498, 0x004d4310),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e2498, 0x004d6b50),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e2498, 0x004d5e90),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e2498, 0x004d57d0),
#endif
};

variableAddressCompact_t vaCreateWidgetText = {
#if UYA_PAL
	.Lobby = 0x00612ae0,
	.Bakisi = VA_COMPACT_BASE(0x004e4a08),
	.Hoven = VA_COMPACT_OFFSET(0x004e4a08, 0x004e6b20),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e4a08, 0x004dc3f8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e4a08, 0x004d9b90),
	.Metropolis = VA_COMPACT_OFFSET(0x004e4a08, 0x004d8ee0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e4a08, 0x004d6778),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e4a08, 0x004d6740),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e4a08, 0x004d8fc0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e4a08, 0x004d82c0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e4a08, 0x004d7c40),
#else
	.Lobby = 0x006103c8,
	.Bakisi = VA_COMPACT_BASE(0x004e2348),
	.Hoven = VA_COMPACT_OFFSET(0x004e2348, 0x004e43a0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e2348, 0x004d9cb8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e2348, 0x004d74d0),
	.Metropolis = VA_COMPACT_OFFSET(0x004e2348, 0x004d6820),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e2348, 0x004d4038),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e2348, 0x004d41c0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e2348, 0x004d6a00),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e2348, 0x004d5d40),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e2348, 0x004d5680),
#endif
};

variableAddressCompact_t vaCreateWidgetTextArea = {
#if UYA_PAL
	.Lobby = 0x00612e50,
	.Bakisi = VA_COMPACT_BASE(0x004e4d78),
	.Hoven = VA_COMPACT_OFFSET(0x004e4d78, 0x004e6e90),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e4d78, 0x004dc768),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e4d78, 0x004d9f00),
	.Metropolis = VA_COMPACT_OFFSET(0x004e4d78, 0x004d9250),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e4d78, 0x004d6ae8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e4d78, 0x004d6ab0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e4d78, 0x004d9330),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e4d78, 0x004d8630),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e4d78, 0x004d7fb0),
#else
	.Lobby = 0x00610738,
	.Bakisi = VA_COMPACT_BASE(0x004e26b8),
	.Hoven = VA_COMPACT_OFFSET(0x004e26b8, 0x004e4710),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e26b8, 0x004da028),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e26b8, 0x004d7840),
	.Metropolis = VA_COMPACT_OFFSET(0x004e26b8, 0x004d6b90),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e26b8, 0x004d43a8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e26b8, 0x004d4530),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e26b8, 0x004d6d70),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e26b8, 0x004d60b0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e26b8, 0x004d59f0),
#endif
};

variableAddressCompact_t vaWidgetSetFrameContainerFlags = {
#if UYA_PAL
	.Lobby = 0x00611018,
	.Bakisi = VA_COMPACT_BASE(0x004e3690),
	.Hoven = VA_COMPACT_OFFSET(0x004e3690, 0x004e57a8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e3690, 0x004db080),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e3690, 0x004d8818),
	.Metropolis = VA_COMPACT_OFFSET(0x004e3690, 0x004d7b68),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e3690, 0x004d5400),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e3690, 0x004d53c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e3690, 0x004d7c48),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e3690, 0x004d6f48),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e3690, 0x004d68c8),
#else
	.Lobby = 0x0060e900,
	.Bakisi = VA_COMPACT_BASE(0x004e0fd0),
	.Hoven = VA_COMPACT_OFFSET(0x004e0fd0, 0x004e3028),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e0fd0, 0x004d8940),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e0fd0, 0x004d6158),
	.Metropolis = VA_COMPACT_OFFSET(0x004e0fd0, 0x004d54a8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e0fd0, 0x004d2cc0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e0fd0, 0x004d2e48),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e0fd0, 0x004d5688),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e0fd0, 0x004d49c8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e0fd0, 0x004d4308),
#endif
};

variableAddressCompact_t vaWidgetSetTextScale = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004e3408),
	.Hoven = VA_COMPACT_OFFSET(0x004e3408, 0x004e5520),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e3408, 0x004dadf8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e3408, 0x004d8590),
	.Metropolis = VA_COMPACT_OFFSET(0x004e3408, 0x004d78e0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e3408, 0x004d5178),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e3408, 0x004d5140),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e3408, 0x004d79c0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e3408, 0x004d6cc0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e3408, 0x004d6640),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004e0d48),
	.Hoven = VA_COMPACT_OFFSET(0x004e0d48, 0x004e2da0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e0d48, 0x004d86b8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e0d48, 0x004d5ed0),
	.Metropolis = VA_COMPACT_OFFSET(0x004e0d48, 0x004d5220),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e0d48, 0x004d2a38),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e0d48, 0x004d2bc0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e0d48, 0x004d5400),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e0d48, 0x004d4740),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e0d48, 0x004d4080),
#endif
};

variableAddressCompact_t vaWidgetSetRotation = {
#if UYA_PAL
	.Lobby = 0x00610e68,
	.Bakisi = VA_COMPACT_BASE(0x004e34e0),
	.Hoven = VA_COMPACT_OFFSET(0x004e34e0, 0x004e55f8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e34e0, 0x004daed0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e34e0, 0x004d8668),
	.Metropolis = VA_COMPACT_OFFSET(0x004e34e0, 0x004d79b8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e34e0, 0x004d5250),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e34e0, 0x004d5218),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e34e0, 0x004d7a98),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e34e0, 0x004d6d98),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e34e0, 0x004d6718),
#else
	.Lobby = 0x0060e750,
	.Bakisi = VA_COMPACT_BASE(0x004e0e20),
	.Hoven = VA_COMPACT_OFFSET(0x004e0e20, 0x004e2e78),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e0e20, 0x004d8790),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e0e20, 0x004d5fa8),
	.Metropolis = VA_COMPACT_OFFSET(0x004e0e20, 0x004d52f8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e0e20, 0x004d2b10),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e0e20, 0x004d2c98),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e0e20, 0x004d54d8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e0e20, 0x004d4818),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e0e20, 0x004d4158),
#endif
};

variableAddressCompact_t vaWidgetSetDropShadowOffset = {
#if UYA_PAL
	.Lobby = 0x00610bc0,
	.Bakisi = VA_COMPACT_BASE(0x004e3318),
	.Hoven = VA_COMPACT_OFFSET(0x004e3318, 0x004e5430),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e3318, 0x004dad08),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e3318, 0x004d84a0),
	.Metropolis = VA_COMPACT_OFFSET(0x004e3318, 0x004d77f0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e3318, 0x004d5088),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e3318, 0x004d5050),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e3318, 0x004d78d0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e3318, 0x004d6bd0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e3318, 0x004d6550),
#else
	.Lobby = 0x0060e4a8,
	.Bakisi = VA_COMPACT_BASE(0x004e0c58),
	.Hoven = VA_COMPACT_OFFSET(0x004e0c58, 0x004e2cb0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e0c58, 0x004d85c8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e0c58, 0x004d5de0),
	.Metropolis = VA_COMPACT_OFFSET(0x004e0c58, 0x004d5130),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e0c58, 0x004d2948),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e0c58, 0x004d2ad0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e0c58, 0x004d5310),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e0c58, 0x004d4650),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e0c58, 0x004d3f90),
#endif
};

variableAddressCompact_t vaWidgetSetAlignmentFlags = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004e3780),
	.Hoven = VA_COMPACT_OFFSET(0x004e3780, 0x004e5898),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e3780, 0x004db170),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e3780, 0x004d8908),
	.Metropolis = VA_COMPACT_OFFSET(0x004e3780, 0x004d7c58),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e3780, 0x004d54f0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e3780, 0x004d54b8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e3780, 0x004d7d38),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e3780, 0x004d7038),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e3780, 0x004d69b8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004e10c0),
	.Hoven = VA_COMPACT_OFFSET(0x004e10c0, 0x004e3118),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e10c0, 0x004d8a30),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e10c0, 0x004d6248),
	.Metropolis = VA_COMPACT_OFFSET(0x004e10c0, 0x004d5598),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e10c0, 0x004d2db0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e10c0, 0x004d2f38),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e10c0, 0x004d5778),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e10c0, 0x004d4ab8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e10c0, 0x004d43f8),
#endif
};

variableAddressCompact_t vaAddFrameToContainer = {
#if UYA_PAL
	.Lobby = 0x00612238,
	.Bakisi = VA_COMPACT_BASE(0x004e42c0),
	.Hoven = VA_COMPACT_OFFSET(0x004e42c0, 0x004e63d8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e42c0, 0x004dbcb0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e42c0, 0x004d9448),
	.Metropolis = VA_COMPACT_OFFSET(0x004e42c0, 0x004d8798),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e42c0, 0x004d6030),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e42c0, 0x004d5ff8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e42c0, 0x004d8878),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e42c0, 0x004d7b78),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e42c0, 0x004d74f8),
#else
	.Lobby = 0x0060fb20,
	.Bakisi = VA_COMPACT_BASE(0x004e1c00),
	.Hoven = VA_COMPACT_OFFSET(0x004e1c00, 0x004e3c58),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e1c00, 0x004d9570),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e1c00, 0x004d6d88),
	.Metropolis = VA_COMPACT_OFFSET(0x004e1c00, 0x004d60d8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e1c00, 0x004d38f0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e1c00, 0x004d3a78),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e1c00, 0x004d62b8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e1c00, 0x004d55f8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e1c00, 0x004d4f38),
#endif
};

variableAddressCompact_t vaSetLayer = {
#if UYA_PAL
	.Lobby = 0x0060f1c0,
	.Bakisi = VA_COMPACT_BASE(0x004e1920),
	.Hoven = VA_COMPACT_OFFSET(0x004e1920, 0x004e3a38),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e1920, 0x004d9310),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e1920, 0x004d6aa8),
	.Metropolis = VA_COMPACT_OFFSET(0x004e1920, 0x004d5df8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e1920, 0x004d3690),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e1920, 0x004d3658),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e1920, 0x004d5ed8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e1920, 0x004d51d8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e1920, 0x004d4b58),
#else
	.Lobby = 0x0060cac0,
	.Bakisi = VA_COMPACT_BASE(0x004df278),
	.Hoven = VA_COMPACT_OFFSET(0x004df278, 0x004e12d0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004df278, 0x004d6be8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004df278, 0x004d4400),
	.Metropolis = VA_COMPACT_OFFSET(0x004df278, 0x004d3750),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004df278, 0x004d0f68),
	.CommandCenter = VA_COMPACT_OFFSET(0x004df278, 0x004d10f0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004df278, 0x004d3930),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004df278, 0x004d2c70),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004df278, 0x004d25b0),
#endif
};

variableAddressCompact_t vaClearLayer = {
#if UYA_PAL
	.Lobby = 0x0060f298,
	.Bakisi = VA_COMPACT_BASE(0x004e19f8),
	.Hoven = VA_COMPACT_OFFSET(0x004e19f8, 0x004e3b10),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004e19f8, 0x004d93e8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004e19f8, 0x004d6b80),
	.Metropolis = VA_COMPACT_OFFSET(0x004e19f8, 0x004d5ed0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004e19f8, 0x004d3768),
	.CommandCenter = VA_COMPACT_OFFSET(0x004e19f8, 0x004d3730),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004e19f8, 0x004d5fb0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004e19f8, 0x004d52b0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004e19f8, 0x004d4c30),
#else
	.Lobby = 0x0060cb98,
	.Bakisi = VA_COMPACT_BASE(0x004df350),
	.Hoven = VA_COMPACT_OFFSET(0x004df350, 0x004e13a8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004df350, 0x004d6cc0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004df350, 0x004d44d8),
	.Metropolis = VA_COMPACT_OFFSET(0x004df350, 0x004d3828),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004df350, 0x004d1040),
	.CommandCenter = VA_COMPACT_OFFSET(0x004df350, 0x004d11c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004df350, 0x004d3a08),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004df350, 0x004d2d48),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004df350, 0x004d2688),
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

int hudGetCurrentCanvasIndex(void)
{
	Engine_t *instance = ENGINE_INSTANCE;
	if (instance->data == NULL)
		instance = hudInitInstance();
	if (instance->data == NULL)
		return -1;
	
	return instance->data->current_canvas;
}

// Returns pointer to the current canvas
Canvas_t *hudGetCurrentCanvas(void)
{
	Engine_t *instance = ENGINE_INSTANCE;
	EngineData_t *data;
	
	if (instance->data == NULL)
		instance = hudInitInstance();
	if (instance->data == NULL)
		return NULL;
	
	data = instance->data;
	return data->canvases[data->current_canvas];
}

// Keep your existing helper functions the same
bool hudSetCurrentCanvasLayer(int whichLayer, HANDLE_ID handle_id)
{
	Canvas_t *c = hudGetCurrentCanvas();
	if(!c) return 0;

	return hudSetLayer(c, whichLayer, handle_id);
}

bool hudClearCurrentCanvasLayer(int whichLayer)
{
	Canvas_t *c = hudGetCurrentCanvas();
	if(!c) return 0;

	return hudClearLayer(c, whichLayer);
}
