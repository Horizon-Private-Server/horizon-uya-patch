#include "game.h"
#include "gamesettings.h"
#include "ui.h"
#include "help.h"

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
