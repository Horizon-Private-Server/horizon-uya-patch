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

VariableAddress_t vaGSFrame = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00247ec8,
	.Hoven = 0x002480c8,
	.OutpostX12 = 0x00247fc8,
    .KorgonOutpost = 0x00247e48,
	.Metropolis = 0x00247ec8,
	.BlackwaterCity = 0x00247e48,
	.CommandCenter = 0x00247a48,
    .BlackwaterDocks = 0x00247b48,
    .AquatosSewers = 0x00247b48,
    .MarcadiaPalace = 0x00247b48,
#else
	.Lobby = 0,
	.Bakisi = 0x00248048,
	.Hoven = 0x00248248,
	.OutpostX12 = 0x00248148,
    .KorgonOutpost = 0x00247fc8,
	.Metropolis = 0x00248048,
	.BlackwaterCity = 0x00247fc8,
	.CommandCenter = 0x00247bc8,
    .BlackwaterDocks = 0x00247cc8,
    .AquatosSewers = 0x00247cc8,
    .MarcadiaPalace = 0x00247cc8,
#endif
};

VariableAddress_t vaDeathBarrier = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00248130,
	.Hoven = 0x00248330,
	.OutpostX12 = 0x00248230,
    .KorgonOutpost = 0x002480b0,
	.Metropolis = 0x00248130,
	.BlackwaterCity = 0x002480b0,
	.CommandCenter = 0x00247cb0,
    .BlackwaterDocks = 0x00247db0,
    .AquatosSewers = 0x00247db0,
    .MarcadiaPalace = 0x00247db0,
#else
	.Lobby = 0,
	.Bakisi = 0x002482b0,
	.Hoven = 0x002484b0,
	.OutpostX12 = 0x002483b0,
    .KorgonOutpost = 0x00248230,
	.Metropolis = 0x002482b0,
	.BlackwaterCity = 0x00248230,
	.CommandCenter = 0x00247e30,
    .BlackwaterDocks = 0x00247f30,
    .AquatosSewers = 0x00247f30,
    .MarcadiaPalace = 0x00247f30,
#endif
};

VariableAddress_t vaGAME_DATA = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00357260,
	.Hoven = 0x003573A0,
	.OutpostX12 = 0x0034F2A0,
    .KorgonOutpost = 0x0034F120,
	.Metropolis = 0x0034F160,
	.BlackwaterCity = 0x0034F120,
	.CommandCenter = 0x0035FCE0,
    .BlackwaterDocks = 0x0035FE20,
    .AquatosSewers = 0x0035FE60,
    .MarcadiaPalace = 0x0035FDE0,
#else
	.Lobby = 0,
	.Bakisi = 0x003573E0,
	.Hoven = 0x00357520,
	.OutpostX12 = 0x0034F420,
    .KorgonOutpost = 0x0034F2A0,
	.Metropolis = 0x0034F2E0,
	.BlackwaterCity = 0x0034F2A0,
	.CommandCenter = 0x0035FE60,
    .BlackwaterDocks = 0x0035FFA0,
    .AquatosSewers = 0x0035FFE0,
    .MarcadiaPalace = 0x0035FF60,
#endif
};

VariableAddress_t vaGameEndFunc = {
#ifdef UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x005445f8,
    .Hoven = 0x005467c0,
    .OutpostX12 = 0x0053c098,
    .KorgonOutpost = 0x00539780,
    .Metropolis = 0x00538b80,
    .BlackwaterCity = 0x00536368,
    .CommandCenter = 0x00535bc0,
    .BlackwaterDocks = 0x00538440,
    .AquatosSewers = 0x00537740,
    .MarcadiaPalace = 0x005370c0,
#else
    .Lobby = 0,
    .Bakisi = 0x00541ce8,
    .Hoven = 0x00543df0,
    .OutpostX12 = 0x00539708,
    .KorgonOutpost = 0x00536e70,
    .Metropolis = 0x00536270,
    .BlackwaterCity = 0x005339d8,
    .CommandCenter = 0x00533408,
    .BlackwaterDocks = 0x00535c48,
    .AquatosSewers = 0x00534f88,
    .MarcadiaPalace = 0x005348c8,
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
