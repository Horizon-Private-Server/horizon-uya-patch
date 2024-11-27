#include <tamtypes.h>
#include <libuya/string.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/pad.h>
#include <libuya/stdio.h>
#include <libuya/uya.h>
#include <libuya/random.h>
#include <libuya/sha1.h>
#include <libuya/time.h>
#include <libuya/ui.h>
#include "messageid.h"
#include "config.h"

#ifdef UYA_PAL
#define STAGING_BASE_FUNC (0x006c1730)
#define CREATE_GAME_BASE_FUNC (0x0069a630)
#define BUDDIES_BASE_FUNC (0x00690de8)
#define PLAYER_DETAILS_BASE_FUNC (0x006b48c8)
#define STATS_BASE_FUNC (0x006c5cc8)
#define REQUEST_TEAM_CHANGE_BASE_FUNC (0)
#else
#define STAGING_BASE_FUNC (0x006bec18)
#define CREATE_GAME_BASE_FUNC (0x00697e20)
#define BUDDIES_BASE_FUNC (0x0068e6f8)
#define PLAYER_DETAILS_BASE_FUNC (0x006b1f60)
#define STATS_BASE_FUNC (0x006c31b8)
#define REQUEST_TEAM_CHANGE_BASE_FUNC (0x006c1848)
#endif

typedef enum uiPadButtons {
    UI_PAD_NONE = 0,
    UI_PAD_DPAD_UP = 1,
    UI_PAD_DPAD_DOWN = 2,
    UI_PAD_DPAD_LEFT = 3,
    UI_PAD_DPAD_RIGHT = 4,
    UI_PAD_TRIANGLE = 5,
    UI_PAD_CIRCLE = 6,
    UI_PAD_CROSS = 7,
    UI_PAD_SQUARE = 8,
    UI_PAD_SELECT = 9,
    UI_PAD_START = 10,
    UI_PAD_L1 = 11,
    UI_PAD_L2 = 12,
    UI_PAD_L3 = 13,
    UI_PAD_R1 = 14,
    UI_PAD_R2 = 15,
    UI_PAD_R3 = 16,
    UI_PAD_TOTAL = UI_PAD_R3
} uiPadButtons_e;

typedef enum playerOptions {
    PLAYER_OPTION_TEAMSKIN = 0,
    PLAYER_OPTION_KICK,
    PLAYER_OPTION_ADD_BUDDY,
    PLAYER_OPTION_IGNORE
} PlayerOptions_e;

typedef int (*uiVTable_Func)(void * ui, int pad);
uiVTable_Func createGameFunc = (uiVTable_Func)CREATE_GAME_BASE_FUNC;
uiVTable_Func stagingFunc = (uiVTable_Func)STAGING_BASE_FUNC;
uiVTable_Func buddiesFunc = (uiVTable_Func)BUDDIES_BASE_FUNC;
uiVTable_Func playerDetailsFunc = (uiVTable_Func)PLAYER_DETAILS_BASE_FUNC;
uiVTable_Func statsFunc = (uiVTable_Func)STATS_BASE_FUNC;

typedef void (*requestTeamChange_Func)(void * ui, int index);
requestTeamChange_Func requestTeamChange = (requestTeamChange_Func)REQUEST_TEAM_CHANGE_BASE_FUNC;

// Craete playerOptions stack
char * playerOptions[4] = {
    "Change Team",
    "Kick Player",
    "Add to Buddies",
    "Ignore Player"
};

void setTeams(int numTeams)
{
    int i, j;
	u32 seed = timerGetSystemTime();
	char teamByClientId[GAME_MAX_PLAYERS];
	char Pool[GAME_MAX_PLAYERS];
    int PlayerSize;

	memset(teamByClientId, -1, sizeof(teamByClientId));
	// get game settings
	GameSettings* gameSettings = gameGetSettings();
	if (gameSettings) {
        // fill pool with needed teams
        PlayerSize = gameSettings->PlayerCount;
        for (i = 0; i < PlayerSize; ++i)
            Pool[i] = i % 2;

		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			int clientId = gameSettings->PlayerClients[i];
			if (clientId >= 0) {
				int teamId = teamByClientId[clientId];
				if (teamId < 0) {
					if (PlayerSize == 0 || numTeams == 0) {
						teamId = 0;
                    } else if (numTeams == 8) {
                        teamId = i;
					} else {
						// psuedo random
						sha1(&seed, 4, &seed, 4);

						// get pool index from rng
						int teamPoolIndex = seed % PlayerSize;

						// set team
						teamId = Pool[teamPoolIndex];

						printf("\npool info pid: %d\npoolIndex: %d\npoolSize: %d\nteam: %d", i, teamPoolIndex, PlayerSize, teamId);

						// remove element from pool
						if (PlayerSize > 0) {
							for (j = teamPoolIndex+1; j < PlayerSize; ++j)
								Pool[j-1] = Pool[j];
                        
							PlayerSize -= 1;
						}
					}
					// set client id team
					teamByClientId[clientId] = teamId;
				}
				// set team
				printf("\nsetting pid:%d  team to %d", i, teamId);
				gameSettings->PlayerTeams[i] = teamId;
			}
		}
	}
}

/*
 * This function is needed to return the correct
 * index of where the selected player is at.
 * If people leave/get kicked, it messes up
 * the selectedItem -> needed player order.
*/
int getPlayerIndex(void * ui,int selectedPlayer)
{
    GameSettings * gs = gameGetSettings();
    int uiElement = *(int*)(ui + (selectedPlayer + 0xf) * 4 + 0x110);
    long compareNames;
    int i = 0;
    while (((gs == 0 || (gs->PlayerClients[i] == -1)) || (compareNames = strcmp(uiElement + 0x14, gs->PlayerNames[i]), compareNames != 0))) {
        ++i;
        if (7 < i) return -1;
    }
    return i;
}

void optionChangeTeamSkin(void * ui, GameSettings * gs, int selectedItem, int isBot, int isTeams)
{
    // Get correct player index
    int i =  getPlayerIndex(ui, selectedItem);
    // check gametype
    int isDeathmatch = gs->GameType == GAMERULE_DM;
    // Get the needed number of teams dependent on gametype.
    int numTeams = (isDeathmatch) ? 8 : 2;

    // if not a bot and
    if (!isBot && isTeams) {
        // ended up not needing this...keeping just in case.
        // requestTeamChange(ui, selectedItem);
        short gsTeam = gs->PlayerTeams[i];
        if (isDeathmatch)
            gsTeam = (gsTeam > (numTeams - 1)) ? 0 : gsTeam + 1;
        else
            gsTeam = !gsTeam;
    } else if (isBot) {
        // Open Team/Skin Menu
        uiShowChangeTeamSkinDialog(gs->PlayerTeams[i], gs->PlayerSkins[i], numTeams, isTeams, 1, 1);
        // Save chosen Team/Skin
        u32 getUI = uiGetPointer(UIP_CHANGE_SKIN_TEAM);
        gs->PlayerTeams[i] = *(int*)(*(u32*)(getUI + 0x110) + 0x146c);
        gs->PlayerSkins[i] = *(int*)(*(u32*)(getUI + 0x114) + 0x146c);
    }
}

void getOptions(GameSettings * gs, int selectedPlayer, int isBot, int isTeams)
{
    char changeTeamSkinOption = "Change Team";
    if (isBot) {
        sprintf(changeTeamSkinOption, "Change Team/Skin");
    }

    memset(playerOptions[0], changeTeamSkinOption, 16);
    // playerOptions[2] = addBuddyOption;
    // playerOptions[3] = ignorePlayerOption;
}

int openPlayerOptions(void * ui, GameSettings * gs, int itemSelected)
{
    int selectedItem = itemSelected - 0xf;
    int selectedPlayer = getPlayerIndex(ui, selectedItem);
    int account_id = gs->PlayerAccountIds[selectedPlayer];
    int isBot = (account_id >= 883) && (account_id <= 1880);
    int isTeams = gameGetOptions()->GameFlags.MultiplayerGameFlags.Teams;
    // if selected player isn't first player (host)
    if (selectedItem > 0) {
        // getOptions(gs, selectedPlayer, isBot, isTeams);
        char * title = "Player Options";
        int optionsSize = sizeof(playerOptions)/sizeof(char*);
        // if bot, change title and remove last two options.
        if (isBot) {
            title = "Bot Options";
            optionsSize -= 2;
        }
        int select = uiShowSelectDialog(title, playerOptions, optionsSize, 0);
        switch (select) {
            case PLAYER_OPTION_TEAMSKIN: optionChangeTeamSkin(ui, gs, selectedItem, isBot, isTeams); break;
            case PLAYER_OPTION_KICK: {
                int kickDialog = uiShowYesNoDialog("", "Are you sure you want to kick this player?");
                if (kickDialog == 1) {
                    gs->PlayerStates[selectedPlayer] = 5;
                }
                break;
            };
            case PLAYER_OPTION_ADD_BUDDY: break;
            case PLAYER_OPTION_IGNORE: break;
        }
        return UI_PAD_NONE;
    }
    // return pressing X if nothing else.
    return UI_PAD_CROSS;
}

int patchStaging(void * ui, int pad)
{
    static int allPlayersReady = -1;
    int i;
    int j;
    GameSettings* gs = gameGetSettings();
    int clientId = gameGetMyClientId();
    // ui address: 0x01dc06a8
    u32 * uiElements = (u32*)((u32)ui + 0x110);
    int itemSelected = *(int*)(ui + 0x290);

    if (gameAmIHost()) {
        if (pad == UI_PAD_CIRCLE && allPlayersReady == 1) {
            // Force "Start" to be selected
            *(int*)((u32)ui + 0x290) = 4;
            allPlayersReady = 2;
            pad = UI_PAD_CROSS;
        } else if (pad == UI_PAD_CROSS) {
            // open new player options if host
            // pad = openPlayerOptions(ui, gs, itemSelected);
        } else if (pad == UI_PAD_L1) {
            // int gsClientID = gs->PlayerClients[getPlayerIndex(ui, itemSelected - 0xf)];
            // u64 data = 0x0000000000040000;
            // ((void(*)(u8, u32, u32, u32, u32, u64))0x0014AD08)(0x50, *(u32*)0x001a5e54, gsClientID, *(u32*)0x002407c0, 0x20, &data);
            // setTeams(2);
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_R1) {
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_L2) {
            // setTeams(8);
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_R2) {
            // ui, selectedTeam, SelectedSkin, controllerPort, numTeamColors, bChangeTeams, bDan, bNefarious
            // setTeams(0);
            pad = UI_PAD_NONE;
        }
    } else {
	    // Patch Unkick
        u32 leaveGamePopup = uiGetActiveSubPointer(11);
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			// Check ClientID and if they are kicked and not viewing the kicked popup
			if (gs->PlayerClients[i] == clientId && gs->PlayerStates[i] == 5 && leaveGamePopup > 0) {
                // Force close leave game popup.
                *(u32*)0x01C5C284 = 0x2c;
			}
		}
    }

	// call game function we're replacing
	int result = stagingFunc(ui, pad);

    if (gameAmIHost()) {
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            // Set host to always ready
            if (gs->PlayerStates[0] != 7)
                gs->PlayerStates[0] = 6;

            // if not all players are ready
            if (gs->PlayerClients[i] >= 0 && gs->PlayerStates[i] != 6 && gs->PlayerStates[i] != 7)
                allPlayersReady = 0;
            else if (gs->PlayerClients[i] >= 0 && gs->PlayerStates[i] == 6)
                allPlayersReady = 1;
            else if (allPlayersReady == 2)
                allPlayersReady = -1;
        }
        
        // Replace "\x11 UNREADY" with "\x11 STSART"
        int str = uiMsgString(0x16d9);
        if (allPlayersReady)
	        strncpy((char*)str, "\x11 START", 9);
        else
	        strncpy((char*)str, "         ", 9);
     
    }
    
	return result;
}

int patchCreateGame(void * ui, long pad)
{
    // ui address: 0x01d20954
    u32 * uiElements = (u32*)((u32)ui + 0x110);
    int itemSelected = *(int*)(ui + 0x290);

    int result = createGameFunc(ui, pad);

    return result;
}

int patchBuddies(void * ui, long pad)
{
    u32 * uiElements = (u32*)((u32)ui + 0x110);
    int itemSelected = *(int*)(ui + 0x290);

    int result = buddiesFunc(ui, pad);

    return result;
}

int patchPlayerDetails(void * ui, long pad)
{
    u32 * uiElements = (u32*)((u32)ui + 0x110);
    int itemSelected = *(int*)(ui + 0x290);

    int result = playerDetailsFunc(ui, pad);

    return result;
}

int patchStats(void * ui, int pad)
{
    u32 * uiElements = (u32*)((u32)ui + 0x110);
    int itemSelected = *(int*)(ui + 0x290);

    int result = statsFunc(ui, pad);

    return result;
}
