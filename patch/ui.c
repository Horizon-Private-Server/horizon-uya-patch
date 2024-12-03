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
#include <libuya/graphics.h>
#include "messageid.h"
#include "config.h"

#ifdef UYA_PAL
#define STAGING_BASE_FUNC (0x006c1730)
#define CREATE_GAME_BASE_FUNC (0x0069a630)
#define BUDDIES_BASE_FUNC (0x00690de8)
#define PLAYER_DETAILS_BASE_FUNC (0x006b48c8)
#define STATS_BASE_FUNC (0x006c5cc8)
#define REQUEST_TEAM_CHANGE_BASE_FUNC (0x006c4360)
#else
#define STAGING_BASE_FUNC (0x006bec18)
#define CREATE_GAME_BASE_FUNC (0x00697e20)
#define BUDDIES_BASE_FUNC (0x0068e6f8)
#define PLAYER_DETAILS_BASE_FUNC (0x006b1f60)
#define STATS_BASE_FUNC (0x006c31b8)
#define REQUEST_TEAM_CHANGE_BASE_FUNC (0x006c1848)
#endif

#define OPTIONS_SCROLL_CHECK ((u32)STAGING_BASE_FUNC + 0xae0)

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

typedef enum playerOptions_Host {
    PLAYER_OPTION_TEAMSKIN = 0,
    PLAYER_OPTION_KICK,
    PLAYER_OPTION_ADD_BUDDY,
    PLAYER_OPTION_IGNORE
} PlayerOptions_e;

typedef enum teamOptions {
    TEAM_OPTION_RANDOM = 0,
    TEAM_OPTION_BLUE,
    TEAM_OPTION_RED,
    TEAM_OPTION_FFA
} TeamOptions_e;

typedef int (*uiVTable_Func)(void * ui, int pad);
uiVTable_Func createGameFunc = (uiVTable_Func)CREATE_GAME_BASE_FUNC;
uiVTable_Func stagingFunc = (uiVTable_Func)STAGING_BASE_FUNC;
uiVTable_Func buddiesFunc = (uiVTable_Func)BUDDIES_BASE_FUNC;
uiVTable_Func playerDetailsFunc = (uiVTable_Func)PLAYER_DETAILS_BASE_FUNC;
uiVTable_Func statsFunc = (uiVTable_Func)STATS_BASE_FUNC;

typedef void (*requestTeamChange_Func)(void * ui, int index);
requestTeamChange_Func requestTeamChange = (requestTeamChange_Func)REQUEST_TEAM_CHANGE_BASE_FUNC;

// Craete playerOptions stack
char * playerOption_Host[4] = {
    "Change Team",
    "Kick Player",
    "Add to Buddies",
    "Ignore Player"
};

char * playerOption_Client[2] = {
    "Add to Buddies",
    "Ignore Player"
};

char * randomTeamOptions[4] = {
    "Random Teams",
    "All Blue",
    "All Red",
    "Free For All"
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
					if (PlayerSize == 0 || numTeams == TEAM_OPTION_BLUE || numTeams == TEAM_OPTION_RED) {
						teamId = numTeams - 1;
                    } else if (numTeams == TEAM_OPTION_FFA) {
                        teamId = i;
					} else if (numTeams == TEAM_OPTION_RANDOM) {
						// psuedo random
						sha1(&seed, 4, &seed, 4);

						// get pool index from rng
						int teamPoolIndex = seed % PlayerSize;

						// set team
						teamId = Pool[teamPoolIndex];

						// printf("\npool info pid: %d\npoolIndex: %d\npoolSize: %d\nteam: %d", i, teamPoolIndex, PlayerSize, teamId);

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
				// printf("\nsetting pid:%d  team to %d", i, teamId);
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
    // Get the needed number of teams dependent on gametype.
    int numTeams = (gs->GameType == GAMERULE_DM) ? 8 : 2;

    if (isBot) {
        // Open Team/Skin Menu
        uiShowChangeTeamSkinDialog(gs->PlayerTeams[i], gs->PlayerSkins[i], numTeams, isTeams, 1, 1);
        // Save chosen Team/Skin
        u32 getUI = uiGetPointer(UIP_CHANGE_SKIN_TEAM);
        gs->PlayerTeams[i] = *(int*)(*(u32*)(getUI + 0x110) + 0x146c);
        gs->PlayerSkins[i] = *(int*)(*(u32*)(getUI + 0x114) + 0x146c);
    } else if (!isBot && isTeams) {
        // Keep this function just in case.
        // requestTeamChange(ui, selectedItem);
        // reset request
        int gsTeam = gs->PlayerTeams[i];
        // Change Teams depending on game type.
        int team = 0;
        if (gs->GameType == GAMERULE_DM) {
            if (gsTeam >= (numTeams - 1)) {
                team = 0;
            } else {
                team = gsTeam + 1;
            }
        } else {
            team = !gsTeam;
        }
        gs->PlayerTeams[i] = team;
    }
}

int openPlayerOptions(void * ui, GameSettings * gs, int itemSelected, int isTeams)
{
    int selectedItem = itemSelected - 0xf;
    int selectedPlayer = getPlayerIndex(ui, selectedItem);
    int account_id = gs->PlayerAccountIds[selectedPlayer];
    int client_id = gs->PlayerClients[selectedPlayer];
    int isBot = (account_id >= 883) && (account_id <= 1880);
    int isHost = gameAmIHost();
    // if selected player isn't first player (host)
    if (selectedItem > -1 && client_id != gameGetMyClientId()) {
        // getOptions(gs, selectedPlayer, isBot, isTeams);
        char * title = "Player Options";
        int optionsSize = (isHost) ? 4 : 2;
        // if bot, change title and remove last two options.
        if (isBot) {
            title = "Bot Options";
            optionsSize -= 2;
        }
        int select = uiShowSelectDialog(title, (isHost) ? playerOption_Host : playerOption_Client, optionsSize, 0);
        int selection = -1;
        if (select > -1)
            selection = (isHost) ? select : (select + 2);

        switch (selection) {
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

void openTeamsOptions(GameSettings * gs, int isTeams)
{
    int size = sizeof(randomTeamOptions)/sizeof(char*);
    int optionsSize = (gs->GameType == GAMERULE_DM) ? size : (size - 1);
    int select = uiShowSelectDialog("Team Options", randomTeamOptions, size, 0);
    if (select > -1)
        setTeams(select);
}

int patchStaging(void * ui, int pad)
{
    static int allPlayersReady = -1;
    static int pressedL1 = -1;
    static int holdingL1time = 0; 
    int i;
    int j;
    GameSettings* gs = gameGetSettings();
    int clientId = gameGetMyClientId();
    int isTeams = gameGetOptions()->GameFlags.MultiplayerGameFlags.Teams;
    u32 * uiElements = (u32*)((u32)ui + 0x110);
    int itemSelected = *(int*)(ui + 0x290);
    int isLoading = *(u8*)(ui + 0x2b6);
    int isRequstingTeamChange = *(u8*)(ui + 0x2c8);

    // Change pad buttons for all players
    if (pad == UI_PAD_CROSS) {
        pad = openPlayerOptions(ui, gs, itemSelected, isTeams);
    }
    // Change pad buttons on for host
    if (gameAmIHost()) {
        if (pad == UI_PAD_CIRCLE && allPlayersReady == 1) {
            // Force "Start" to be selected
            *(int*)((u32)ui + 0x290) = 4;
            allPlayersReady = 2;
            pad = UI_PAD_CROSS;
        } else if (pad == UI_PAD_R1) {
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_L2) {
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_R2) {
            pad = UI_PAD_NONE;
        }
    }

	// call game function we're replacing
	int result = stagingFunc(ui, pad);

    if (gameAmIHost()) {
        // Opem Teams Options Menu
        if (isTeams) {
            // if L1 isn't pressed and no other menu is open
            if (!padGetButton(0, PAD_L1)) {
                // reset pressed l1 if nothing is pressed.
                pressedL1 = -1;
                holdingL1time = gameGetTime() + .75 * TIME_SECOND;
            } else if (padGetButton(0, PAD_L1) && pressedL1 < 1 && (*(u32*)0x01c5c114 == 0)) {
                pressedL1 = 0;
            }
            if (pressedL1 == 0) {
                pressedL1 = 1;
                setTeams(TEAM_OPTION_RANDOM);
            } else if (pressedL1 == 1 && holdingL1time <= gameGetTime() && *(u32*)0x01c5c114 == 0) {
                pressedL1 = 2;
                openTeamsOptions(gs, isTeams);
            }
        }
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            // Set host to always ready
            if (gs->PlayerStates[0] != 7)
                gs->PlayerStates[0] = 6;

            // if not all players are ready
            if (gs->PlayerClients[i] > -1 && gs->PlayerStates[i] < 6) {
                allPlayersReady = 0;
            } else if (gs->PlayerClients[i] > -1 && gs->PlayerStates[i] == 6) {
                allPlayersReady = 1;
            } else if (allPlayersReady == 2) {
                allPlayersReady = -1;
            }
        }
        
        // Replace "\x11 UNREADY" with "\x11 STSART"
        int str = uiMsgString(0x16d9);
        if (allPlayersReady)
	        strncpy((char*)str, "\x11 START", 9);
        else
	        strncpy((char*)str, "         ", 9);
        
    } else {
        // Nont Host Stuff
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            // if server client id matches local client id.
            if (gs->PlayerClients[i] == clientId) {
                // Patch Unkick: if leave game pop is up, close it.
                u32 popup = uiGetActiveSubPointer(11);
                if (gs->PlayerStates[i] == 5 && popup > 0) {
                    // Compare popup text and "Leave Game" string.
                    if (strcmp(*(u32*)(popup + 0x110) + 0x64, uiMsgString(0x1643))) {
                        *(u32*)0x01C5C284 = 0x2c;
                    }
                }
            }
        }
        // Make it so all clients can scroll over users names
        if (*(u32*)OPTIONS_SCROLL_CHECK == 0x24050001)
            *(u32*)OPTIONS_SCROLL_CHECK = 0x24050003;
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
