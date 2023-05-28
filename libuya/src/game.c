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
#define GAME_TIME                           (*(int*)0x002257D8)

#define GAME_MAP_ID                         (*(int*)0x001f83a8)

#define GAME_CLIENT_ID                      (*(int*)0x001a5cd8)
#define GAME_HOST_ID                        (*(int*)0x001a5e3c)

#else

#define GAME_ACTIVE                         (*(int*)0x00241BD0)
#define SCENE_LOADED                        (*(int*)0x002459C8)

/*
 * Game time (ms).
 */
#define GAME_TIME                           (*(int*)0x00225958)

#define GAME_MAP_ID                         (*(int*)0x001F8528)

#define GAME_CLIENT_ID                      (*(int*)0x001a5e58)
#define GAME_HOST_ID                        (*(int*)0x001a5fbc)

#endif

#ifdef UYA_PAL
    VariableAddress_t vaGameEndFunc = {
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
    };
#else
    VariableAddress_t vaGameEndFunc = {
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
    };
#endif

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

int gameGetTime(void)
{
    return GAME_TIME;
}

int gameGetMyClientId(void)
{
    return GAME_CLIENT_ID;
}

int gameAmIHost(void)
{
    return GAME_CLIENT_ID == GAME_HOST_ID;
}

int gameGetCurrentMapId(void)
{
    return GAME_MAP_ID;
}

char* gameGetGameModeName(int gameModeId)
{
    switch (gameModeId)
    {
        case GAMERULE_SEIGE: return uiMsgString(HELP_STRING_ID_SEIGE);
        case GAMERULE_CTF: return uiMsgString(HELP_STRING_ID_CTF);
        case GAMERULE_DM: return uiMsgString(HELP_STRING_ID_DEATHMATCH);
        default: return NULL;
    }
}

void gameEnd(int reason)
{
    internal_gameEnd(reason);
}
