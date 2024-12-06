#include "game.h"
#include "gamesettings.h"
#include "ui.h"
#include "help.h"
#include "interop.h"

#if UYA_PAL
#define GAME_ACTIVE                         (*(int*)0x00241a50)
#define SCENE_LOADED                        (*(int*)0x00245848)
/*
 * Game time (ms).
 */
#define GAME_TIME                           (*(int*)0x001a5df8)
#define GAME_MAP_ID                         (*(int*)0x001f83a8)
#define GAME_CLIENT_ID                      (*(int*)0x001a5cd8)
#define GAME_IS_HOST                        (*(int*)0x001a5e38)
#define GAME_HOST_ID                        (*(int*)0x001a5e3c)
#else
#define GAME_ACTIVE                         (*(int*)0x00241BD0)
#define SCENE_LOADED                        (*(int*)0x002459C8)

#define GAME_IS_CLAN_CHALLENGE              (*(u8*)0x001a5e58)
#define GAME_MY_SKIN                        (*(int*0x001a5e6c)
#define GAME_MY_ACCOUNT_ID                  (*(int*)0x201a5e84)
#define GAME_MY_CLAN_ID                     (*(int*)0x001a5e88)
#define GAME_MY_CLAN_NAME                   (0x001a5e8c)
#define GAME_IS_CLAN_LEADER                 (*(u8*)0x001a5ead)
#define GAME_MY_USERNAME                    (0x001a5eae)
#define GAME_CREATE_GAME_NAME               (0x001a5f0e)
#define GAME_CREATE_GAME_PASSWORD           (0x001a5f4e)
#define GAME_TIME                           (*(int*)0x001a5f78)
#define GAME_MAP_ID                         (*(int*)0x001F8528)
#define GAME_CLIENT_ID                      (*(int*)0x001a5e58)
#define GAME_IS_HOST                        (*(int*)0x001a5fb8)
#define GAME_HOST_ID                        (*(int*)0x001a5fbc)

// UNK Not Ported to PAL
#define GAME_UNK_001a5e54                   (*(int*)0x001a5e54)
#define GAME_UNK_001a5f80                   (*(int*)0x001a5f80)
#define GAME_UNK_001a5f88                   (*(int*)0x001a5f88)
#define GAME_UNK_001a5f8c                   (*(int*)0x001a5f8c)
#define GAME_UNK_001a6444                   (*(int*)0x001a6444)

/*
 * Array of round trip time divided by 2.
 */
// #define GAME_PING_ARRAY                     ((int*)0x001B2420)
// #define GAME_PING_CURRENT_INDEX             (*(int*)0x001B25B4)
// Ping Array (Goes up every 1 second)
// 0x001D3B58
#endif

#define GAME_DEATH_BARRIER                  (*(float*)GetAddress(&vaDeathBarrier))

#define GAME_DATA                           ((GameData*)GetAddress(&vaGAME_DATA))
#define GAME_TIME_LIMIT                     (*(int*)GetAddress(&vaGAME_DATA))
#define GAME_START_TIME                     (*(int*)((u32)GetAddress(&vaGAME_DATA) + 0x4))
#define GAME_TIME_ENDGAME                   (*(u32*)((u32)GetAddress(&vaGAME_DATA) + 0x1f0))
#define GAME_HAS_ENDED                      (GAME_TIME_ENDGAME > 0)
// Set to Team ID that won.
#define GAME_WINNER_TEAM_ID                 (*(int*)((u32)GetAddress(&vaGAME_DATA) + 0x10))
// Player id of the winner. Set to -1 for team win.
#define GAME_WINNER_PLAYER_ID               (*(int*)((u32)GetAddress(&vaGAME_DATA) + 0x14))

#define GAME_GS_FRAME                       (*(int*)GetAddress(&vaGSFrame))

#define SPAWNPOINTS_SIEGE                   (*(u32*)((u32)GetAddress(&vaGAME_DATA) + 0x20c))
#define SPAWNPOINTS_CTF                     (*(u32*)((u32)GetAddress(&vaGAME_DATA) + 0x210))
#define SPAWNPOINTS_DM                      (*(u32*)((u32)GetAddress(&vaGAME_DATA) + 0x214))

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
    return GAME_HAS_ENDED;
}

int gameGetFinishedExitTime(void)
{
    return GAME_TIME_ENDGAME;
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
    switch (gameModeId)
    {
        case GAMERULE_SIEGE: return uiMsgString(HELP_STRING_ID_SIEGE);
        case GAMERULE_CTF: return uiMsgString(HELP_STRING_ID_CTF);
        case GAMERULE_DM: return uiMsgString(HELP_STRING_ID_DEATHMATCH);
        default: return NULL;
    }
}

void gameEnd(int reason)
{
    internal_gameEnd(reason);
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
    GAME_WINNER_TEAM_ID = teamOrPlayerId;
    GAME_WINNER_PLAYER_ID = isTeam ? -1 : teamOrPlayerId;
}

GameData* gameGetData(void)
{
    return GAME_DATA;
}

int gameGetGSFrame(void)
{
    return GAME_GS_FRAME;
}
