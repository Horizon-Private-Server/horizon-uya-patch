#include <tamtypes.h>
#include "gamesettings.h"
#include "math.h"
#include "help.h"

#if UYA_PAL

/*
 * Points to the active game settings.
 */
#define GLOBAL_GAMESETTINGS			            ((GameSettings*)(*(u32*)0x00241640))


/*
 * Points to the active game options.
 */
#define GLOBAL_GAMEOPTIONS			            ((GameOptions*)((u32)GLOBAL_GAMESETTINGS + 0x188))

/*
 */
#define GAME_KILLS_TO_WIN                       (*(u8*)0x00241654)
#define VEHICLES_ALLOWED                        (*(u8*)0x00241657)
#define GAME_WEAPONS                            (*(u8*)0x0024165A)
#define CTF_CAP_LIMIT                           (*(u8*)0x0024165C)

#else

/*
 * Points to the active game settings.
 */
#define GLOBAL_GAMESETTINGS			            ((GameSettings*)(*(u32*)0x002417C0))


/*
 * Points to the active game options.
 */
#define GLOBAL_GAMEOPTIONS			            ((GameOptions*)((u32)GLOBAL_GAMESETTINGS + 0x188))

/*
 */
#define GAME_KILLS_TO_WIN                       (*(u8*)0x002417D4)
#define VEHICLES_ALLOWED                        (*(u8*)0x002417D7)
#define GAME_WEAPONS                            (*(u8*)0x002417DA)
#define CTF_CAP_LIMIT                           (*(u8*)0x002417DC)

#endif



//--------------------------------------------------------
GameSettings * gameGetSettings(void)
{
    return GLOBAL_GAMESETTINGS;
}

//--------------------------------------------------------
GameOptions * gameGetOptions(void)
{
    return GLOBAL_GAMEOPTIONS;
}
