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

#define CLIENT_UPDATE_A0                        (*(u32*)0x0024d760 + 0x3c)
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

#define CLIENT_UPDATE_A0                        (*(u32*)0x0024d860 + 0x3c)
#endif

void internal_netUpdatetNWGameSettings(void*, int pid, char* name, int skin, int team, int state, void*);

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
void gameSetClientState(int pid, char state)
{
    GameSettings* gs = gameGetSettings();
    if (!gs)
        return;
    
    gs->PlayerStates[pid] = state;
    internal_netUpdatetNWGameSettings(*(void**)(CLIENT_UPDATE_A0), pid, gs->PlayerNames[pid], gs->PlayerSkins[pid], gs->PlayerTeams[pid], state, (void*)((u32)gs + 0x80));
}

//--------------------------------------------------------
void gameSetClientTeam(int pid, char team)
{
    GameSettings* gs = gameGetSettings();
    if (!gs)
        return;
    
    gs->PlayerTeams[pid] = team;
    internal_netUpdatetNWGameSettings(*(void**)(CLIENT_UPDATE_A0), pid, gs->PlayerNames[pid], gs->PlayerSkins[pid], team, gs->PlayerStates[pid], (void*)((u32)gs + 0x80));
}

//--------------------------------------------------------
void gameSetClientSkin(int pid, char skin)
{
    GameSettings* gs = gameGetSettings();
    if (!gs)
        return;
    
    gs->PlayerSkins[pid] = skin;
    internal_netUpdatetNWGameSettings(*(void**)(CLIENT_UPDATE_A0), pid, gs->PlayerNames[pid], skin, gs->PlayerTeams[pid], gs->PlayerStates[pid], (void*)((u32)gs + 0x80));
}

//--------------------------------------------------------
void gameSetClientName(int pid, char* name)
{
    GameSettings* gs = gameGetSettings();
    if (!gs)
        return;
    
    strncpy(gs->PlayerNames[pid], name, 0x16);
    internal_netUpdatetNWGameSettings(*(void**)(CLIENT_UPDATE_A0), pid, name, gs->PlayerSkins[pid], gs->PlayerTeams[pid], gs->PlayerStates[pid], (void*)((u32)gs + 0x80));
}
