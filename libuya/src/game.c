#include "game.h"

#ifdef UYA_PAL
#define GAME_ACTIVE                         (*(int*)0x002357FC)
/*
 * Game time (ms).
 */
#define GAME_TIME                           (*(int*)0x002257D8)
#else
#define GAME_ACTIVE                         (*(int*)0x00241BD0)
/*
 * Game time (ms).
 */
#define GAME_TIME                           (*(int*)0x00225958)
#endif

int gameIsIn(void)
{
    return GAME_ACTIVE == 1;
    
    if (!GAME_ACTIVE != 1)
        return 0;
}

int gameGetTime(void)
{
    return GAME_TIME;
}
