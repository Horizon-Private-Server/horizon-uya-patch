#include "game.h"
#include "gamesettings.h"
#include "ui.h"
#include "help.h"
#include "interop.h"

#define GAME_DATA                           ((GameData*)GetAddress(&vaGAME_DATA))

#define GAME_DEATH_BARRIER                  (*(float*)GetAddress(&vaDeathBarrier))
#define GAME_GS_FRAME                       (*(int*)GetAddress(&vaGSFrame))
#define GAME_GAMEMODE_TIME                  (*(int*)((u32)GetAddress(&vaGSFrame) + 0x7c))
#define GAME_GS_USAGE                       (*(float*)((u32)GetAddress(&vaGSFrame) + 0x80))
#define GAME_CPU_USAGE                      (*(float*)((u32)GetAddress(&vaGSFrame) + 0x84))

variableAddressCompact_t vaGSFrame = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00247ec8),
	.Hoven = VA_COMPACT_OFFSET(0x00247ec8, 0x002480c8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00247ec8, 0x00247fc8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00247ec8, 0x00247e48),
	.Metropolis = VA_COMPACT_OFFSET(0x00247ec8, 0x00247ec8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00247ec8, 0x00247e48),
	.CommandCenter = VA_COMPACT_OFFSET(0x00247ec8, 0x00247a48),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00247ec8, 0x00247b48),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00247ec8, 0x00247b48),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00247ec8, 0x00247b48),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00248048),
	.Hoven = VA_COMPACT_OFFSET(0x00248048, 0x00248248),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00248048, 0x00248148),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00248048, 0x00247fc8),
	.Metropolis = VA_COMPACT_OFFSET(0x00248048, 0x00248048),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00248048, 0x00247fc8),
	.CommandCenter = VA_COMPACT_OFFSET(0x00248048, 0x00247bc8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00248048, 0x00247cc8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00248048, 0x00247cc8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00248048, 0x00247cc8),
#endif
};

variableAddressCompact_t vaDeathBarrier = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00248130),
	.Hoven = VA_COMPACT_OFFSET(0x00248130, 0x00248330),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00248130, 0x00248230),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00248130, 0x002480b0),
	.Metropolis = VA_COMPACT_OFFSET(0x00248130, 0x00248130),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00248130, 0x002480b0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00248130, 0x00247cb0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00248130, 0x00247db0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00248130, 0x00247db0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00248130, 0x00247db0),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x002482b0),
	.Hoven = VA_COMPACT_OFFSET(0x002482b0, 0x002484b0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x002482b0, 0x002483b0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x002482b0, 0x00248230),
	.Metropolis = VA_COMPACT_OFFSET(0x002482b0, 0x002482b0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x002482b0, 0x00248230),
	.CommandCenter = VA_COMPACT_OFFSET(0x002482b0, 0x00247e30),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x002482b0, 0x00247f30),
	.AquatosSewers = VA_COMPACT_OFFSET(0x002482b0, 0x00247f30),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x002482b0, 0x00247f30),
#endif
};

variableAddressCompact_t vaGAME_DATA = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00357260),
	.Hoven = VA_COMPACT_OFFSET(0x00357260, 0x003573a0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00357260, 0x0034f2a0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00357260, 0x0034f120),
	.Metropolis = VA_COMPACT_OFFSET(0x00357260, 0x0034f160),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00357260, 0x0034f120),
	.CommandCenter = VA_COMPACT_OFFSET(0x00357260, 0x0035fce0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00357260, 0x0035fe20),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00357260, 0x0035fe60),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00357260, 0x0035fde0),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x003573e0),
	.Hoven = VA_COMPACT_OFFSET(0x003573e0, 0x00357520),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003573e0, 0x0034f420),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003573e0, 0x0034f2a0),
	.Metropolis = VA_COMPACT_OFFSET(0x003573e0, 0x0034f2e0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003573e0, 0x0034f2a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x003573e0, 0x0035fe60),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003573e0, 0x0035ffa0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003573e0, 0x0035ffe0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003573e0, 0x0035ff60),
#endif
};

variableAddressCompact_t vaGameEndFunc = {
#ifdef UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005445f8),
	.Hoven = VA_COMPACT_OFFSET(0x005445f8, 0x005467c0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005445f8, 0x0053c098),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005445f8, 0x00539780),
	.Metropolis = VA_COMPACT_OFFSET(0x005445f8, 0x00538b80),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005445f8, 0x00536368),
	.CommandCenter = VA_COMPACT_OFFSET(0x005445f8, 0x00535bc0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005445f8, 0x00538440),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005445f8, 0x00537740),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005445f8, 0x005370c0),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00541ce8),
	.Hoven = VA_COMPACT_OFFSET(0x00541ce8, 0x00543df0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00541ce8, 0x00539708),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00541ce8, 0x00536e70),
	.Metropolis = VA_COMPACT_OFFSET(0x00541ce8, 0x00536270),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00541ce8, 0x005339d8),
	.CommandCenter = VA_COMPACT_OFFSET(0x00541ce8, 0x00533408),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00541ce8, 0x00535c48),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00541ce8, 0x00534f88),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00541ce8, 0x005348c8),
#endif
};

VariableAddress_t vaModeRequestChange = {
#if UYA_PAL
    .Lobby = 0x005bf798,
    .Bakisi = 0x00491f18,
    .Hoven = 0x00494030,
    .OutpostX12 = 0x00489908,
    .KorgonOutpost = 0x00486fd8,
    .Metropolis = 0x004863f0,
    .BlackwaterCity = 0x00483ca8,
    .CommandCenter = 0x00483c80,
    .BlackwaterDocks = 0x00486500,
    .AquatosSewers = 0x0172dd50,
    .MarcadiaPalace = 0x00485180,
#else
    .Lobby = 0x005bd7d8,
    .Bakisi = 0x0048ffb0,
    .OutpostX12 = 0x00487920,
    .KorgonOutpost = 0x00485070,
    .Metropolis = 0x00484488,
    .BlackwaterCity = 0x00481cc0,
    .CommandCenter = 0x00481e58,
    .BlackwaterDocks = 0x00484698,
    .AquatosSewers = 0x004839d8,
    .MarcadiaPalace = 0x00483318,
#endif
};
    
    

__LIBUYA_GETTER__ int isInGame(void)
{
    return GAME_ACTIVE && GAME_MAP_ID != 0x27  && SCENE_LOADED == 1;
}

__LIBUYA_GETTER__ int isInMenus(void)
{
    return !GAME_ACTIVE && GAME_MAP_ID == 0x27 && SCENE_LOADED == 1;
}

__LIBUYA_GETTER__ int isSceneLoading(void)
{
    return SCENE_LOADED != 1;
}

int gameHasEnded(void)
{
    return GAME_DATA->gameIsOver > 0;
}

int gameGetFinishedExitTime(void)
{
    return GAME_DATA->gameIsOver;
}

int gameGetTime(void)
{
    return GAME_TIME;
}

int gameIsHost(int hostId)
{
    return hostId == GAME_HOST_ID;
}

int gameAmIHost(void)
{
    return GAME_CLIENT_ID == GAME_HOST_ID;
}

int gameGetHostId(void)
{
    return GAME_HOST_ID;
}

int gameGetMyClientId(void)
{
    return GAME_CLIENT_ID;
}

int gameGetMyAccountId(void)
{
    return GAME_NET_INFO->myAccountId;
}

int gameGetCurrentMapId(void)
{
    return GAME_MAP_ID;
}

char* gameGetGameModeName(int gameModeId)
{
    switch (gameModeId) {
        case GAMETYPE_SIEGE: return uiMsgString(0x1024);
        case GAMETYPE_CTF: return uiMsgString(0x10e2);
        case GAMETYPE_DM: return uiMsgString(0x1026);
        default: return NULL;
    }
}

float gameGetDeathHeight(void)
{
    return GAME_DEATH_BARRIER;
}

void gameSetDeathHeight(float height)
{
    GAME_DEATH_BARRIER = height;
}

void gameSetWinner(int teamOrPlayerId, int isTeam)
{
    GAME_DATA->winningTeam = teamOrPlayerId;
    GAME_DATA->winningPlayer = isTeam ? -1 : teamOrPlayerId;
}

GameData* gameGetData(void)
{
    return GAME_DATA;
}

int gameGetGSFrame(void)
{
    return GAME_GS_FRAME;
}

int gameGetWorldId(void)
{
    return GAME_WORLD_ID;
}

int gameGetGameModeTime(void)
{
    return GAME_GAMEMODE_TIME;
}

float gameGetGsUsage(void)
{
    return GAME_GS_USAGE;
}

float gameGetCpuUsage(void)
{
    return GAME_CPU_USAGE;
}

void gameIsGameRanked(bool yayOrNay)
{
    GAME_NET_INFO->isGameRanked = yayOrNay;
}
