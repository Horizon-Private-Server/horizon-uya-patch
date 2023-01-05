#include "game.h"
#include "gamesettings.h"
#include "ui.h"

#if UYA_PAL

#define GAME_ACTIVE                         (*(int*)0x002357FC)
#define MENU_ACTIVE                         (*(int*)0)
#define SCENE_LOADED                        (*(int*)0)

/*
 * Game time (ms).
 */
#define GAME_TIME                           (*(int*)0x002257D8)

#define GAME_MAP_ID                         (*(int*)0)

#define GAME_CLIENT_ID                      (*(int*)0)
#define GAME_HOST_ID                        (*(int*)0)

#else

#define GAME_ACTIVE                         (*(int*)0x00241BD0)
#define MENU_ACTIVE                         (*(int*)0x001C3CB0)
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
    return MENU_ACTIVE && GAME_MAP_ID == 0x27 && SCENE_LOADED == 1;
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
        case GAMERULE_SEIGE: return uiMsgString(0x1024);
        case GAMERULE_CTF: return uiMsgString(0x10E2);
        case GAMERULE_DM: return uiMsgString(0x1026);
        default: return NULL;
    }
}
