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
// UI Functions
#define STAGING_BASE_FUNC (0x006c1730)
#define CREATE_GAME_BASE_FUNC (0x0069a630)
#define BUDDIES_BASE_FUNC (0x00690de8)
#define PLAYER_DETAILS_BASE_FUNC (0x006b48c8)
#define STATS_BASE_FUNC (0x006c5cc8)

// Needed Staging Functions
#define OPTION_REQUEST_TEAM_CHANGE_BASE_FUNC (0x006c4360)
#define BUDDY_IGNORE_LIST_PTR (*(u32*)0x0024d760)
#define GET_BUDDY_LIST_BASE_FUNC (0x006cd768)
#define GET_IGNORE_LIST_BASE_FUNC (0x006d5d68)
#define OPTION_ADD_BUDDY_BASE_FUNC (0x006cdc30)
#define OPTION_REMOVE_BUDDY_BASE_FUNC (0x006cddb0)
#define OPTION_IGNORE_PLAYER_BASE_FUNC (0x006d6210)
#define OPTION_UNIGNORE_PLAYER_BASE_FUNC (0x006d6398)
#else
// UI Functions
#define STAGING_BASE_FUNC (0x006bec18)
#define CREATE_GAME_BASE_FUNC (0x00697e20)
#define BUDDIES_BASE_FUNC (0x0068e6f8)
#define PLAYER_DETAILS_BASE_FUNC (0x006b1f60)
#define STATS_BASE_FUNC (0x006c31b8)

// Needed Staging Functions
#define OPTION_REQUEST_TEAM_CHANGE_BASE_FUNC (0x006c1848)
#define BUDDY_IGNORE_LIST_PTR (*(u32*)0x0024d860)
#define GET_BUDDY_LIST_BASE_FUNC (0x006cafe0)
#define GET_IGNORE_LIST_BASE_FUNC (0x006d35d8)
#define OPTION_ADD_BUDDY_BASE_FUNC (0x006cb4a8)
#define OPTION_REMOVE_BUDDY_BASE_FUNC (0x006cb628)
#define OPTION_IGNORE_PLAYER_BASE_FUNC (0x006d3a80)
#define OPTION_UNIGNORE_PLAYER_BASE_FUNC (0x006d3c08)
#endif

#define MAX_BUDDIES (64)
#define MAX_IGNORED (25)
#define OPTIONS_SCROLL_CHECK ((u32)STAGING_BASE_FUNC + 0xae0)
#define BUDDY_LIST_STACK (*(u32*)(BUDDY_IGNORE_LIST_PTR + 0x34))
#define BUDDY_LIST_COUNT (*(int*)(BUDDY_LIST_STACK + *(int*)(BUDDY_LIST_STACK + 0x4) * 0xc + 0x2c))
#define BUDDY_LIST_START (*(u32*)(BUDDY_LIST_STACK + 0x40) + 0x10)
#define BUDDY_LIST_END (*(u32*)(BUDDY_LIST_STACK + 0x40) + 0x1c)
#define IGNORE_LIST_STACK (*(u32*)(BUDDY_IGNORE_LIST_PTR + 0x40))
#define IGNORE_LIST_COUNT (*(int*)(IGNORE_LIST_STACK + *(int*)(IGNORE_LIST_STACK + 0x4) * 0xc + 0x2c))
#define IGNORE_LIST_START (*(u32*)(IGNORE_LIST_STACK + 0x40) + 0x10)
#define IGNORE_LIST_END (*(u32*)(IGNORE_LIST_STACK + 0x40) + 0x1c)

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

typedef enum initStaging {
    INIT_NOPE = 0,
    INIT_DONE = 1,
    INIT_GAME_START = 2,
    INIT_GAME_START_DONE = 3
} initStaging_e;

typedef enum playerOptions {
    PLAYER_OPTION_CANCEL = -1,
    PLAYER_OPTION_TEAMSKIN = 0,
    PLAYER_OPTION_KICK,
    PLAYER_OPTION_ADD_BUDDY,
    PLAYER_OPTION_IGNORE
} PlayerOptions_e;

typedef enum teamOptions {
    TEAM_OPTION_CANCEL = -1,
    TEAM_OPTION_RANDOM = 0,
    TEAM_OPTION_BLUE,
    TEAM_OPTION_RED,
    // TEAM_OPTION_TVB_BVR,
    // TEAM_OPTION_TVB_RVB,
    TEAM_OPTION_FFA
} TeamOptions_e;

typedef int (*uiVTable_Func)(void * ui, int pad);
uiVTable_Func createGameFunc = (uiVTable_Func)CREATE_GAME_BASE_FUNC;
uiVTable_Func stagingFunc = (uiVTable_Func)STAGING_BASE_FUNC;
uiVTable_Func buddiesFunc = (uiVTable_Func)BUDDIES_BASE_FUNC;
uiVTable_Func playerDetailsFunc = (uiVTable_Func)PLAYER_DETAILS_BASE_FUNC;
uiVTable_Func statsFunc = (uiVTable_Func)STATS_BASE_FUNC;

typedef int (*buddy_Func)(int stack, int account_id);
buddy_Func getBuddyList = (buddy_Func)GET_BUDDY_LIST_BASE_FUNC;
buddy_Func getIgnoreList = (buddy_Func)GET_IGNORE_LIST_BASE_FUNC;
buddy_Func addBuddy = (buddy_Func)GET_IGNORE_LIST_BASE_FUNC;
buddy_Func removeBuddy = (buddy_Func)OPTION_REMOVE_BUDDY_BASE_FUNC;
buddy_Func ignorePlayer = (buddy_Func)OPTION_IGNORE_PLAYER_BASE_FUNC;
buddy_Func unignorePlayer = (buddy_Func)OPTION_UNIGNORE_PLAYER_BASE_FUNC;

typedef void (*requestTeamChange_Func)(void * ui, int index);
requestTeamChange_Func requestTeamChange = (requestTeamChange_Func)OPTION_REQUEST_TEAM_CHANGE_BASE_FUNC;

// Craete playerOptions stack
char * playerOption_Host[4][20] = {
    "Change Team",
    "Kick Player",
    "Add to Buddies",
    "Ignore Player"
};

char * playerOption_Client[2][20] = {
    "Add to Buddies",
    "Ignore Player"
};

char * addBuddyStr = "Add to Buddies";
char * removeBuddyStr = "Remove Buddy";
char * ignorePlayerStr = "Ignore Player";
char * unignorePlayeStr = "Unignore Player";

char * randomTeamOptions[4] = {
    "Random Teams",
    // "Top vs Bottom (BvR)",
    // "Top vs Buttom (RvB)",
    "All Blue",
    "All Red",
    "Free For All"
};

typedef struct buddyIgnoreInfo {
/* 0x00 */ short buddyCount;
/* 0x02 */ short ignoredCount;
/* 0x04 */ char client_ids[GAME_MAX_PLAYERS];
/* 0x0c */ char buddies[GAME_MAX_PLAYERS];
/* 0x14 */ char ignored[GAME_MAX_PLAYERS];
} buddyIgnoreInfo_t;

typedef struct buddyListInfo { // 0xd0
	/* 0x00 */ char messageID[21];
	/* 0x18 */ int statusCode;
	/* 0x1c */ int accountID;
	/* 0x20 */ char accountName[32];
	/* 0x40 */ char onlineState[0x8c];
	/* 0xcc */ int EndOfList;
} buddyListInfo_t;

typedef struct ignoreListInfo { // 0x48
	/* 0x00 */ char messageID[21];
	/* 0x18 */ int statusCode;
	/* 0x1c */ int accountID;
	/* 0x20 */ char accountName[32];
	/* 0x40 */ int onlinetatus;
	/* 0x44 */ int EndOfList;
} ignoreListInfo_t;

buddyIgnoreInfo_t info;

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

int optionAddRemoveBuddy(int account_id, int selectedIndex)
{
    int i = selectedIndex;
    int ret = 0;
    // if isn't buddy
    if (info.buddies[i] == 0 && info.ignored[i] == 0) {
        // check if buddy list is full, if so, show ok dialog and return 0.
        if (BUDDY_LIST_COUNT > MAX_BUDDIES) {
            uiShowOkDialog("", uiMsgString(0x184d));
            return 0;
        }
        ret = addBuddy(BUDDY_LIST_STACK, account_id);
    } else if (info.buddies[i] == 1){
        // else Remove Buddy if is buddy
        ret = removeBuddy(BUDDY_LIST_STACK, account_id);
    }
    // if ignored/unignored, update needed info struct.
    if (ret && info.buddies[i] > -1) {
        info.buddies[i] = !info.buddies[i];
        info.buddyCount = (info.buddies[i] ? (info.buddyCount + 1) : (info.buddyCount - 1));
    } else {
        // if Already added/removed buddy, or other problem, so ok dialog.
        char buff[64];
        snprintf(buff, 64, "There was an error trying to add/remove buddy.\n\nn: %d; c: %d; b: %d; i: %d;", i, info.client_ids[i], info.buddies[i], info.ignored[i]);
        uiShowOkDialog("Error", buff);
    }
    // return ret
    return ret;
}

int optionIgnorePlayer(int account_id, int selectedIndex)
{
    int i = selectedIndex;
    int ret = 0;
    // if isn't ignored
    if (info.ignored[i] == 0 && info.buddies[i] == 0) {
        // check if ignore list is full, if so, show ok dialog and return 0.
        if (IGNORE_LIST_COUNT > MAX_IGNORED) {
            uiShowOkDialog("", uiMsgString(0x18d3));
            return 0;
        }
        ret = ignorePlayer(IGNORE_LIST_STACK, account_id);
    } else if (info.ignored[i] == 1){
        // else Remove unignore player if ignored
        ret = unignorePlayer(BUDDY_LIST_STACK, account_id);
    }
    // if ignored/unignored, update needed info struct.
    if (ret &&  info.ignored[i] > -1) {
        info.ignored[i] = !info.ignored[i];
        info.ignoredCount = (info.ignored[i] ? (info.ignoredCount + 1) : (info.ignoredCount - 1));
    } else {
        // if Already ignored, or already unignoredy, or other problem, so ok dialog.
        char buff[64];
        snprintf(buff, 64, "There was an error trying to ignore/unignore player.\n\nn: %d; c: %d; b: %d; i: %d;", i, info.client_ids[i], info.buddies[i], info.ignored[i]);
        uiShowOkDialog("Error", buff);
    }
    // return ret
    return ret;
}

int loopBuddyList(int account_id, buddyListInfo_t * list, int listCount)
{
    int k;
    int r = 0;
    for (k = 0; k < listCount; ++k) {
        // if account id is found on list, it's a buddy!
        if (list[k].accountID == account_id){
            r = 1;
        }
    }
    return r;
}

int loopIgnoreList(int account_id, ignoreListInfo_t * list, int listCount)
{
    int k;
    int r = 0;
    for (k = 0; k < listCount; ++k) {
        // if account id is found on list, it's a buddy!
        if (list[k].accountID == account_id){
            r = 1;
        }
    }
    return r;
}

void updateInfo(GameSettings *gs, int selectedIndex)
{
    int k;
    int i = selectedIndex;
    int account_id = gs->PlayerAccountIds[i];
    int client_id = gs->PlayerClients[i];
    int isHost = gameAmIHost();
    struct buddyListInfo_t * buddy = (struct buddyListInfo_t *)(*(u32*)BUDDY_LIST_START);
    struct ignoreListInfo_t * ignore = (struct ignoreListInfo_t*)(*(u32*)IGNORE_LIST_START);

    // Check if client is alraedy in info list.
    if (info.client_ids[i] != client_id) {
        // store client id
        info.client_ids[i] = client_id;
        info.buddies[i] = 0;
        info.ignored[i] = 0;

        // Check if B uddy
        if (info.buddyCount > 0) {
            // Update Buddy list
            getBuddyList(BUDDY_LIST_STACK, 2);

            int isBuddy = loopBuddyList(account_id, buddy, info.buddyCount);
            info.buddies[i] = isBuddy;
            if (isBuddy)
                info.ignored[i] = 0;
        }
        // Check Ignored: if ignore list isn't zero, and player isn't a buddy
        if (info.ignoredCount > 0 && info.buddies[i] == 0) {
            // reload ignore list
            getIgnoreList(IGNORE_LIST_STACK, 0);
            // check if ignored
            info.ignored[i] = loopIgnoreList(account_id, ignore, info.ignoredCount);
        }
    }

    // update player options
    if (isHost) {
        strncpy((char*)playerOption_Host[2], (info.buddies[i]) ? removeBuddyStr : addBuddyStr, 20);
        strncpy((char*)playerOption_Host[3], (info.ignored[i]) ? unignorePlayeStr : ignorePlayerStr, 20);
    } else {
        strncpy((char*)playerOption_Client[0], (info.buddies[i]) ? removeBuddyStr : addBuddyStr, 20);
        strncpy((char*)playerOption_Client[1], (info.ignored[i]) ? unignorePlayeStr : ignorePlayerStr, 20);
    }
}

int openPlayerOptions(void * ui, GameSettings * gs, int itemSelected, int isTeams)
{
    int selectedItem = itemSelected - 0xf;
    int i = getPlayerIndex(ui, selectedItem);
    int account_id = gs->PlayerAccountIds[i];
    int client_id = gs->PlayerClients[i];
    int isBot = (account_id >= 883) && (account_id <= 1880);
    int isHost = gameAmIHost();
    // if selected player isn't first player (host)
    if (selectedItem > -1 && client_id != gameGetMyClientId()) {
        // if not botupdate info struct and player option names.
        // if (!isBot)
        updateInfo(gs, i);
        
        char * title = "Player Options";
        int optionsSize = (isHost) ? 4 : 2;
        // if bot, change title and remove last two options.
        if (isBot) {
            title = "Bot Options";
            // optionsSize -= 2;
        }
        int select = uiShowSelectDialog(title, (isHost) ? playerOption_Host : playerOption_Client, optionsSize, 0);
        int selection = -1;
        if (select > PLAYER_OPTION_CANCEL)
            selection = (isHost) ? select : (select + 2);

        switch (selection) {
            case PLAYER_OPTION_TEAMSKIN: optionChangeTeamSkin(ui, gs, selectedItem, isBot, isTeams); break;
            case PLAYER_OPTION_KICK: {
                int kickDialog = uiShowYesNoDialog("", "Are you sure you want to kick this player?");
                if (kickDialog == 1) {
                    gs->PlayerStates[i] = 5;
                }
                break;
            }
            case PLAYER_OPTION_ADD_BUDDY: {
                // if not on ignore list, then must be adding/removing buddy.
                if (info.ignored[i] == 0) {
                    optionAddRemoveBuddy(account_id, i);
                } else if (info.ignored[i] == 1) {
                    // if player is ignored, unignore first, then add buddy.
                    // player isn't a buddy because they were ignored.
                    int r = optionIgnorePlayer(account_id, i);
                    // once unignored, add buddy.
                    if (r) {
                        optionAddRemoveBuddy(account_id, i);
                    }
                } else {
                    char buff[64];
                    snprintf(buff, 64, "n: %d; c: %d; b: %d; i: %d;", i, info.client_ids[i], info.buddies[i], info.ignored[i]);
                    uiShowOkDialog("Add/Remove Buddy Err", buff);
                }
                break;
            }
            case PLAYER_OPTION_IGNORE: {
                // if not on buddy list, then must be ignoreing/unignoring buddy.
                if (info.buddies[i] == 0) {
                    optionIgnorePlayer(account_id, i);
                } else if (info.buddies[i] == 1) {
                    // if player is buddy, remove buddy first, then ignore.
                    // player can't be buddy and ignored.
                    int r = optionAddRemoveBuddy(account_id, i);
                    // once removed buddy, ignore.
                    if (r) {
                        optionIgnorePlayer(account_id, i);
                    }
                } else {
                    char buff[64];
                    snprintf(buff, 64, "n: %d; c: %d; b: %d; i: %d;", i, info.client_ids[i], info.buddies[i], info.ignored[i]);
                    uiShowOkDialog("Ignore/Unignore Err", buff);
                }
                break;
            }
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
    if (select > TEAM_OPTION_CANCEL)
        setTeams(select);
}

int patchStaging(void * ui, int pad)
{
    static int init_staging = INIT_NOPE;
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

    //  init staging if not done already, or equals 2
    if ((!init_staging && gs->GameStartTime <= 0) || init_staging == INIT_GAME_START) {
        info.buddyCount = BUDDY_LIST_COUNT;
        info.ignoredCount = IGNORE_LIST_COUNT;
        memset(info.client_ids, -1, sizeof(info.client_ids));
        memset(info.buddies, -1, sizeof(info.buddies));
        memset(info.ignored, -1, sizeof(info.ignored));
        init_staging += 1;
        // if game is starting, set init back to zero for next game.
        if (init_staging == INIT_GAME_START_DONE)
            init_staging = 0;
    }

    if (gs->GameStartTime > 0 && init_staging == INIT_DONE)
        init_staging = INIT_GAME_START;

    // if host
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
    int account_id = *(u32*)(ui + 0x2a8);

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
