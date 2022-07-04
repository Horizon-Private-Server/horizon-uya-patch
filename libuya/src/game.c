#include "game.h"

#ifdef UYA_PAL
#define GAME_ACTIVE                         (*(int*)0x002357FC)
/*
 * Game time (ms).
 */
#define GAME_TIME                           (*(int*)0x002257D8)

#define GAME_CLIENT_ID                      (*(int*)0)
#define GAME_HOST_ID                        (*(int*)0)
#else
#define GAME_ACTIVE                         (*(int*)0x00241BD0)
/*
 * Game time (ms).
 */
#define GAME_TIME                           (*(int*)0x00225958)

#define GAME_CLIENT_ID                      (*(int*)0x001a5e58)
#define GAME_HOST_ID                        (*(int*)0x001a5fbc)
#endif

int gameIsIn(void)
{
    return GAME_ACTIVE == 1;
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
