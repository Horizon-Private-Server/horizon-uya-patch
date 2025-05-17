#include <tamtypes.h>
#include "gamesettings.h"
#include "math.h"
#include "help.h"
#include "team.h"

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

#define CLIENT_UPDATE_A0                        (*(u32*)(*(u32*)0x0024d760) + 0x3c)
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

#define CLIENT_UPDATE_A0                        (*(u32*)(*(u32*)0x0024d860) + 0x3c)
#endif

void internal_netUpdatetNWGameSettings(int stack, int isLocalSelected, char* name, int skin, int team, int state, int gs);

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

//--------------------------------------------------------
void gameSetClientState(int cliendId, int state)
{
    GameSettings* gs = gameGetSettings();
    if (!gs)
        return;
    
    gs->PlayerStates[cliendId] = state;
    internal_netUpdatetNWGameSettings(CLIENT_UPDATE_A0, 0, gs->PlayerNames[cliendId], gs->PlayerSkins[cliendId], gs->PlayerTeams[cliendId], state, (u32)gs + 0x80);
}

//--------------------------------------------------------
void gameSetClientTeam(int cliendId, int team)
{
    GameSettings* gs = gameGetSettings();
    if (!gs)
        return;
    
    gs->PlayerTeams[cliendId] = team;
    internal_netUpdatetNWGameSettings(CLIENT_UPDATE_A0, 0, gs->PlayerNames[cliendId], gs->PlayerSkins[cliendId], team, gs->PlayerStates[cliendId], (u32)gs + 0x80);
}

//--------------------------------------------------------
void gameSetClientSkin(int cliendId, int skin)
{
    GameSettings* gs = gameGetSettings();
    if (!gs)
        return;
    
    gs->PlayerSkins[cliendId] = skin;
    internal_netUpdatetNWGameSettings(CLIENT_UPDATE_A0, 0, gs->PlayerNames[cliendId], skin, gs->PlayerTeams[cliendId], gs->PlayerStates[cliendId], (u32)gs + 0x80);
}

//--------------------------------------------------------
void gameSetClientName(int cliendId, char* name)
{
    GameSettings* gs = gameGetSettings();
    if (!gs)
        return;
    
    strncpy(gs->PlayerNames[cliendId], name, 0x16);
    internal_netUpdatetNWGameSettings(CLIENT_UPDATE_A0, 0, name, gs->PlayerSkins[cliendId], gs->PlayerTeams[cliendId], gs->PlayerStates[cliendId], (u32)gs + 0x80);
}
