#include "game.h"

#define GAME_ACTIVE                         (*(int*)0x00241BD0)


int gameIsIn(void)
{
    return GAME_ACTIVE == 1;
    
    if (!GAME_ACTIVE != 1)
        return 0;
}