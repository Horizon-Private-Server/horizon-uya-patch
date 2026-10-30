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
#include <libuya/utils.h>
#include <libuya/net.h>
#include <libuya/team.h>
#include <libuya/stdlib.h>
#include "messageid.h"
#include "config.h"

#ifdef UYA_PAL
// UI Functions
#define STAGING_BASE_FUNC (0x006c1730)
#define GAME_DETAILS_BASE_FUNC (0x006a3ab0)
#define GAME_DETAILS_OPEN_FUNC (0x006a3690)
#define PLAYER_LIST_COLOR_CALL (0x006819fc)
#define PLAYER_LIST_COLOR_FUNC (0x006119e8)
#define CREATE_GAME_BASE_FUNC (0x0069a630)
#define ADVANCED_OPTIONS_BASE_FUNC (0x0069ce50)
// #define BUDDIES_BASE_FUNC (0x00690de8)
// #define PLAYER_DETAILS_BASE_FUNC (0x006b48c8)
// #define STATS_BASE_FUNC (0x006c5cc8)
#define KEYBOARD_BASE_FUNC (0x6aaa08)

// Needed Staging Functions
#define OPTION_REQUEST_TEAM_CHANGE_BASE_FUNC (0x006c4360)
#define BUDDY_IGNORE_LIST_PTR (*(u32*)0x0024d760)
#define GET_BUDDY_LIST_BASE_FUNC (0x006cd768)
#define GET_IGNORE_LIST_BASE_FUNC (0x006d5d68)
// #define OPTION_ADD_BUDDY_BASE_FUNC (0x006cdc30)
// #define OPTION_REMOVE_BUDDY_BASE_FUNC (0x006cddb0)
#define OPTION_IGNORE_PLAYER_BASE_FUNC (0x006d6210)
#define OPTION_UNIGNORE_PLAYER_BASE_FUNC (0x006d6398)
#else
// UI Functions
#define STAGING_BASE_FUNC (0x006bec18)
#define GAME_DETAILS_BASE_FUNC (0x006a12a0)
#define GAME_DETAILS_OPEN_FUNC (0x006a0e80)
#define PLAYER_LIST_COLOR_CALL (0x0067fafc)
#define PLAYER_LIST_COLOR_FUNC (0x0060f120)
#define CREATE_GAME_BASE_FUNC (0x00697e20)
#define ADVANCED_OPTIONS_BASE_FUNC (0x0069a640)
// #define BUDDIES_BASE_FUNC (0x0068e6f8)
// #define PLAYER_DETAILS_BASE_FUNC (0x006b1f60)
// #define STATS_BASE_FUNC (0x006c31b8)
#define KEYBOARD_BASE_FUNC (0x006a80b8)

// Needed Staging Functions
#define OPTION_REQUEST_TEAM_CHANGE_BASE_FUNC (0x006c1848)
#define BUDDY_IGNORE_LIST_PTR (*(u32*)0x0024d860)
#define GET_BUDDY_LIST_BASE_FUNC (0x006cafe0)
#define GET_IGNORE_LIST_BASE_FUNC (0x006d35d8)
// #define OPTION_ADD_BUDDY_BASE_FUNC (0x006cb4a8)
// #define OPTION_REMOVE_BUDDY_BASE_FUNC (0x006cb628)
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

typedef struct gameDetailsPlayer {
	/* 0x000 */ char name[32];
	/* 0x020 */ s32 accountId;
	/* 0x024 */ u8 stats[256];
} gameDetailsPlayer_t;
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

typedef struct buddyIgnoreInfo {
/* 0x00 */ int currentWorldId;
/* 0x04 */ short buddyCount;
/* 0x06 */ short ignoredCount;
/* 0x08 */ char client_ids[GAME_MAX_PLAYERS];
/* 0x10 */ char buddies[GAME_MAX_PLAYERS];
/* 0x18 */ char ignored[GAME_MAX_PLAYERS];
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

typedef struct BuddyIgnoreRequest { // 0x2c
/* 0x00 */ char messageID[21];
/* 0x15 */ char sessionKey[17];
/* 0x28 */ int accountID;
} BuddyIgnoreRequest_t;

typedef struct keyboardNew { // 0x4
/* 0x0 */ u8 index;
/* 0x1 */char text[3];
} keyboardNew_t;

char* patchGameDetailsUI_GetPackedGameName(void);
int patchGameDetailsUI_ParseGameName(char* string, char** customMap, int* customMode);

typedef int (*uiVTable_Func)(UiMenu_t * ui, int pad);
uiVTable_Func createGameFunc = (uiVTable_Func)CREATE_GAME_BASE_FUNC;
uiVTable_Func advancedOptionsFunc = (uiVTable_Func)ADVANCED_OPTIONS_BASE_FUNC;
uiVTable_Func stagingFunc = (uiVTable_Func)STAGING_BASE_FUNC;
uiVTable_Func gameDetailsFunc = (uiVTable_Func)GAME_DETAILS_BASE_FUNC;
static int gameDetailsSpritesInitialized;

// uiVTable_Func buddiesFunc = (uiVTable_Func)BUDDIES_BASE_FUNC;
// uiVTable_Func playerDetailsFunc = (uiVTable_Func)PLAYER_DETAILS_BASE_FUNC;
// uiVTable_Func statsFunc = (uiVTable_Func)STATS_BASE_FUNC;
uiVTable_Func keyboardFunc = (uiVTable_Func)KEYBOARD_BASE_FUNC;

typedef int (*buddy_Func)(int stack, int account_id);
buddy_Func getBuddyList = (buddy_Func)GET_BUDDY_LIST_BASE_FUNC;
buddy_Func getIgnoreList = (buddy_Func)GET_IGNORE_LIST_BASE_FUNC;
// buddy_Func addBuddy = (buddy_Func)OPTION_ADD_BUDDY_BASE_FUNC;
// buddy_Func removeBuddy = (buddy_Func)OPTION_REMOVE_BUDDY_BASE_FUNC;
buddy_Func ignorePlayer = (buddy_Func)OPTION_IGNORE_PLAYER_BASE_FUNC;
buddy_Func unignorePlayer = (buddy_Func)OPTION_UNIGNORE_PLAYER_BASE_FUNC;

typedef void (*requestTeamChange_Func)(void * ui, int index);
requestTeamChange_Func requestTeamChange = (requestTeamChange_Func)OPTION_REQUEST_TEAM_CHANGE_BASE_FUNC;

extern isConfigMenuActive;
extern mapOverrideResponse;

char changeTeamStr[] = "Change Team";
char kickPlayerStr[] = "Kick Player";
char addBuddyStr[] = "Add to Buddies";
char removeBuddyStr[] = "Remove Buddy";
char ignorePlayerStr[] = "Ignore Player";
char unignorePlayeStr[] = "Unignore Player";

char *playerOptions[4];

char *randomTeamOptions[4] = {
    "Random Teams",
    // "Top vs Bottom (BvR)",
    // "Top vs Buttom (RvB)",
    "All Blue",
    "All Red",
    "Free For All"
};

buddyIgnoreInfo_t info = {
    .currentWorldId = -1
};

keyboardNew_t keys[] = {
    {KEY_1, "\x08 D"}, // Default
    {KEY_2, "\x09 B"}, // Blue
    {KEY_3, "\x0aG"}, // Green
    {KEY_4, "\x0bP"}, // Pink
    {KEY_5, "\x0cW"}, // White
    {KEY_6, "\x0dG"}, // Gray
    {KEY_7, "\x0e B"}, // Black
    {KEY_8, "\x0f A"}, // Aqua
    {KEY_Q, "\x10"}, // Cross
    {KEY_W, "\x11"}, // Circle
    {KEY_E, "\x12"}, // Triangle
    {KEY_R, "\x13"}, // Square
    {KEY_T, "\x14"}, // L1
    {KEY_Y, "\x15"}, // R1
    {KEY_U, "\x16"}, // L2
    {KEY_I, "\x17"}, // R2
    {KEY_O, "\x18"}, // L3
    {KEY_P, "\x19"}, // R3
    {KEY_BRACKET_OPEN, "\x1a"} // Select
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
    if (!ui || !gs || selectedPlayer < 0 || selectedPlayer >= GAME_MAX_PLAYERS)
        return -1;

    int uiElement = *(int*)(ui + (selectedPlayer + 0xf) * 4 + 0x110);
    if (!uiElement)
        return -1;

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
	if (!gs || i < 0 || i >= GAME_MAX_PLAYERS)
		return;
    // Get the needed number of teams dependent on gametype.
    int numTeams = (gs->GameType == GAMETYPE_DM) ? 8 : 2;

    if (isBot) {
        // Open Team/Skin Menu
        uiShowChangeTeamSkinDialog(gs->PlayerTeams[i], gs->PlayerSkins[i], numTeams, isTeams, 1, 1);
		if (!netGetLobbyServerConnection() || gameGetSettings() != gs ||
			uiGetActiveMenu(UI_MENU_STAGING, 0) != ui)
			return;
        // Save chosen Team/Skin
        u32 getUI = uiGetMenu(UI_MENU_CHANGE_SKIN_TEAM);
        gs->PlayerTeams[i] = *(int*)(*(u32*)(getUI + 0x110) + 0x146c);
        gs->PlayerSkins[i] = *(int*)(*(u32*)(getUI + 0x114) + 0x146c);
    } else if (!isBot && isTeams) {
        // Keep this function just in case.
        // requestTeamChange(ui, selectedItem);
        int gsTeam = gs->PlayerTeams[i];
        // Change Teams depending on game type.
        int team = 0;
        if (gs->GameType == GAMETYPE_DM) {
            if (gsTeam >= (numTeams - 1)) {
                team = 0;
            } else {
                team = gsTeam + 1;
            }
        } else {
            team = !gsTeam;
        }
        // Set players team
        gs->PlayerTeams[i] = team;
        // Unready the player.
        gs->PlayerStates[i] = 0;
    }
}

/*
    using the first "removeBuddy" function worked, but
    always gave a network error for some reason.
*/
int addOrRemoveBuddy(int selectedIndex, int account_id)
{
    int i = selectedIndex;
    u32 msg[11];
    msg[0] = 0x001A0031;
    msg[1] = 0x00000000;
    msg[2] = 0x00000007;
    msg[3] = 0x00000000;
    msg[4] = 0x00000000;
    msg[5] = 0x00003500;
    msg[6] = 0x00000000;
    msg[7] = 0x00000000;
    msg[8] = 0x00000000;
    msg[9] = 0x00000000;
    msg[10] = account_id;
    #ifdef UYA_PAL
        u32 removeOrAdd = (info.buddies[i]) ? 0x0017ed28 : 0x0017eca8;
    #else
        u32 removeOrAdd = (info.buddies[i]) ? 0x0017ee18 : 0x0017ed98;
    #endif
    // flip return type.  (True usually equals 0 with medius functions)
    return 0 == ((int (*)(void *, u32, int))removeOrAdd)(msg, 0x00197858, 0);

}

int optionAddRemoveBuddy(int account_id, int selectedIndex)
{
    int i = selectedIndex;
    int ret = 0;
    // if isn't buddy, add to buddies
    if (info.buddies[i] == 0) {
        // if ignored, return 0.
        if (info.ignored[i] == 1) {
            uiShowOkDialog("Error","You must unignore this player before adding them to your buddies.");
            return ret;
        }
        // check if buddy list is full, if so, show ok dialog and return 0.
        if (info.buddyCount > MAX_BUDDIES) {
            uiShowOkDialog("", uiMsgString(0x184d));
            return ret;
        }
        // ret = addBuddy(BUDDY_LIST_STACK, account_id);
        ret = addOrRemoveBuddy(i, account_id);
        if (ret) {
            info.buddies[i] = 1;
            info.buddyCount += 1;
        } else if (!ret) {
            char buff[64];
            snprintf(buff, 64, "n: %d; c: %d; b: %d; i: %d;", i, info.client_ids[i], info.buddies[i], info.ignored[i]);
            uiShowOkDialog("Add Buddy Error", buff);
        }
        return ret;
    } else if (info.buddies[i] == 1) {
        // else Remove Buddy if is buddy
        // ret = removeBuddy(BUDDY_LIST_STACK, account_id);
        ret = addOrRemoveBuddy(i, account_id);
        if (ret) {
            info.buddies[i] = 0;
            info.buddyCount -= 1;
        } else if (!ret) {
            char buff[64];
            snprintf(buff, 64, "n: %d; ret: %d; c: %d; b: %d; i: %d;", i, ret, info.client_ids[i], info.buddies[i], info.ignored[i]);
            uiShowOkDialog("Remove Buddy Error", buff);
        }
        return ret;
    }
    // all else, return 0.
    return ret;
}

int optionIgnorePlayer(int account_id, int selectedIndex)
{
    int i = selectedIndex;
    int ret = 0;
    // if isn't ignored
    if (info.ignored[i] == 0) {
        // if ignored, return 0.
        if (info.buddies[i] == 1) {
            uiShowOkDialog("","You must remove buddy before you can ignore them.");
            return ret;
        }
        // check if ignore list is full, if so, show ok dialog and return 0.
        if (info.ignored[i] > MAX_IGNORED) {
            uiShowOkDialog("", uiMsgString(0x18d3));
            return ret;
        }
        ret = ignorePlayer(IGNORE_LIST_STACK, account_id);
        if (ret) {
            info.ignored[i] = 1;
            info.ignoredCount += 1;
        } else if (!ret) {
            char buff[64];
            snprintf(buff, 64, "n: %d; c: %d; b: %d; i: %d;", i, info.client_ids[i], info.buddies[i], info.ignored[i]);
            uiShowOkDialog("Ignore Player Error", buff);
        }
        return ret;
    } else if (info.ignored[i] == 1){
        // else Unignore
        ret = unignorePlayer(IGNORE_LIST_STACK, account_id);
        if (ret) {
            info.ignored[i] = 0;
            info.ignoredCount -= 1;
        } else if (ret == 0) {
            char buff[64];
            snprintf(buff, 64, "n: %d; ret: %d c: %d; b: %d; i: %d;", i, ret, info.client_ids[i], info.buddies[i], info.ignored[i]);
            uiShowOkDialog("Unignore Player Error", buff);
        }
        return ret;
    }
    // all else, return 0.
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
            break;
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
            break;
        }
    }
    return r;
}

void updateInfo(GameSettings *gs, int selectedIndex)
{
    int i = selectedIndex;
    int account_id = gs->PlayerAccountIds[i];
    int client_id = gs->PlayerClients[i];
    int isHost = gameAmIHost();
	if (!BUDDY_IGNORE_LIST_PTR || !BUDDY_LIST_STACK || !IGNORE_LIST_STACK ||
		!*(u32*)(BUDDY_LIST_STACK + 0x40) || !*(u32*)(IGNORE_LIST_STACK + 0x40))
		return;
    struct buddyListInfo_t * buddy = (struct buddyListInfo_t *)(*(u32*)BUDDY_LIST_START);
    struct ignoreListInfo_t * ignore = (struct ignoreListInfo_t*)(*(u32*)IGNORE_LIST_START);

    // Check if client is alraedy in info list.
    if (info.client_ids[i] != client_id) {
        // store client id
        info.client_ids[i] = client_id;
        info.buddies[i] = 0;
        info.ignored[i] = 0;

        // Check if uddy
        if (info.buddyCount > 0) {
            int isBuddy = loopBuddyList(account_id, buddy, info.buddyCount);
            info.buddies[i] = isBuddy;
            if (isBuddy)
                info.ignored[i] = 0;
        }
        // Check Ignored: if ignore list isn't zero, and player isn't a buddy
        if (info.ignoredCount > 0 && info.buddies[i] == 0) {
            int isIgnored = loopIgnoreList(account_id, ignore, info.ignoredCount);
            // check if ignored
            info.ignored[i] = isIgnored;
        }
    }

    // update player options
    int offset = 2;
    if (isHost) {
        offset = 0;
        playerOptions[PLAYER_OPTION_TEAMSKIN] = changeTeamStr;
        playerOptions[PLAYER_OPTION_KICK] = kickPlayerStr;
    }
    playerOptions[PLAYER_OPTION_ADD_BUDDY - offset] = info.buddies[i] ? removeBuddyStr : addBuddyStr;
    playerOptions[PLAYER_OPTION_IGNORE - offset] = info.ignored[i] ? unignorePlayeStr : ignorePlayerStr;
}

int openPlayerOptions(void * ui, GameSettings * gs, int itemSelected, int isTeams)
{
    int selectedItem = itemSelected - 0xf;
    if (!ui || !gs || selectedItem < 0 || selectedItem >= GAME_MAX_PLAYERS)
        return UI_PAD_CROSS;

    int i = getPlayerIndex(ui, selectedItem);
    if (i < 0 || i >= GAME_MAX_PLAYERS)
        return UI_PAD_CROSS;

    int account_id = gs->PlayerAccountIds[i];
    int client_id = gs->PlayerClients[i];
    int isBot = (account_id >= 883) && (account_id <= 1880);
    int isHost = gameAmIHost();
    // if selected player isn't first player (host)
    if (selectedItem > -1 && client_id != gameGetMyClientId()) {
        // if not botupdate info struct and player option names.
        if (!isBot)
            updateInfo(gs, i);
        
#ifdef DEBUG
        int a;
        printf("\n========");
        printf("\nbuddies: %d, ignored: %d", info.buddyCount, info.ignoredCount);
        for (a = 0; a < 8; ++a) {
            printf("\nc: %d, b: %d, i: %d", info.client_ids[a], info.buddies[a], info.ignored[a]);
        }
        printf("\n========");
#endif

        char * title = "Player Options";
        int size = sizeof(playerOptions)/sizeof(char*);
        int optionsSize = (isHost) ? size : size - 2;
        // if bot, change title and remove last two options.
        if (isBot) {
            title = "Bot Options";
            optionsSize -= 2;
        }
        int select = uiShowSelectDialog(title, playerOptions, optionsSize, 0);
		if (!netGetLobbyServerConnection() || gameGetSettings() != gs ||
			uiGetActiveMenu(UI_MENU_STAGING, 0) != ui)
			return UI_PAD_NONE;
        int selection = -1;
        if (select > PLAYER_OPTION_CANCEL)
            selection = (isHost) ? select : (select + 2);

        switch (selection) {
            case PLAYER_OPTION_TEAMSKIN: optionChangeTeamSkin(ui, gs, selectedItem, isBot, isTeams); break;
            case PLAYER_OPTION_KICK: {
                char kickingPlayer[32];
                char * kickEm = uiMsgString(0x15ff);
                snprintf(kickingPlayer, 32, kickEm, gs->PlayerNames[i]);
                int kickDialog = uiShowYesNoDialog("", kickingPlayer);
                if (kickDialog == 1 && netGetLobbyServerConnection() && gameGetSettings() == gs &&
					uiGetActiveMenu(UI_MENU_STAGING, 0) == ui) {
                    gs->PlayerStates[i] = 5;
                }
                break;
            }
            case PLAYER_OPTION_ADD_BUDDY: optionAddRemoveBuddy(account_id, i); break;
            case PLAYER_OPTION_IGNORE: optionIgnorePlayer(account_id, i); break;
        }
        return UI_PAD_NONE;
    }
    // return pressing X if nothing else.
    return UI_PAD_CROSS;
}

void openTeamsOptions(GameSettings * gs, int isTeams)
{
    int size = sizeof(randomTeamOptions)/sizeof(char*);
    int optionsSize = (gs->GameType == GAMETYPE_DM) ? size : (size - 1);
    int select = uiShowSelectDialog("Team Options", randomTeamOptions, size, 0);
    if (select > TEAM_OPTION_CANCEL && netGetLobbyServerConnection() && gameGetSettings() == gs)
        setTeams(select);
}
/*
 * NAME : setGameDetailsPlayerListColor
 * DESCRIPTION : Replace only the unfocused details player-list text color.
 * NOTES : Native list state, vtable, and selection remain unchanged.
 * ARGS : Native text handle and requested color.
 * RETURN : Native color setter result.
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int setGameDetailsPlayerListColor(int handle, u32 color)
{
	UiMenu_t* ui = uiGetActiveMenu(UI_MENU_GAME_DETAILS, 0);
	if (!ui)
		ui = uiGetActiveMenu(UI_MENU_GAME_DETAILS, 1);
	if (ui && color == 0x80808080) {
		UiGameDetailsElements_t* elements = (UiGameDetailsElements_t*)ui->pChildren;
		UiElementList_t* list = elements->playerList;
		if (list && handle == *(int*)((u8*)list + 0x10178))
			color = 0x8066ccff;
	}
	return ((int (*)(int, u32))PLAYER_LIST_COLOR_FUNC)(handle, color);
}

/*
 * NAME : patchGameDetailsOpen
 * DESCRIPTION : Recreate team sprites each time Game Details opens.
 * NOTES : Native setup reuses sprite handle IDs after discarding the old container.
 * ARGS : Game Details menu.
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
void patchGameDetailsOpen(UiMenu_t* ui)
{
	gameDetailsSpritesInitialized = 0;
	((void (*)(UiMenu_t*))GAME_DETAILS_OPEN_FUNC)(ui);
}

/*
 * NAME : patchGameDetails
 * DESCRIPTION : Update the native player list and its staging-style team sprites.
 * NOTES : Map/mode labels and joining remain in their existing handlers.
 * ARGS : Details menu and native UI pad action.
 * RETURN : Native menu update result.
 * AUTHOR : Troy "Metroynome" Pruitt
 */
int patchGameDetails(UiMenu_t* ui, int pad)
{
	static UiElementSprite_t (*teamSprites)[2];
	int row;
	int layer;
	if (!ui)
		return 0;

	// The selected game's packed name supplies its running/join settings.
	int flags = patchGameDetailsUI_ParseGameName(patchGameDetailsUI_GetPackedGameName(), NULL, NULL);
	int blockJoin = (flags & (GAME_NAME_FLAGS_VALID | GAME_NAME_RUNNING)) ==
		(GAME_NAME_FLAGS_VALID | GAME_NAME_RUNNING);
#ifdef JOIN_IN_PROGRESS
	blockJoin = blockJoin && !(flags & GAME_NAME_JOIN_ENABLED);
#endif
	if (blockJoin && ui->selectedIndex == 23 && pad == UI_PAD_CROSS)
		pad = UI_PAD_NONE;

	int previousSelection = ui->selectedIndex;
	int result = gameDetailsFunc(ui, pad);
	// The native menu releases its graphics container when leaving Details.
	if (!netGetLobbyServerConnection() ||
		(uiGetActiveMenu(UI_MENU_GAME_DETAILS, 0) != ui &&
		uiGetActiveMenu(UI_MENU_GAME_DETAILS, 1) != ui)) {
		gameDetailsSpritesInitialized = 0;
		return result;
	}
	if (ui->childCount < sizeof(UiGameDetailsElements_t) / sizeof(void*))
		return result;

	UiGameDetailsElements_t* elements = (UiGameDetailsElements_t*)ui->pChildren;
	elements->localPlayers->state = 0;
	// Up from Join selects Players; X is then handled by the native menu
	// to enter its list. Down from Players skips the hidden Local Players row.
	int target = -1;
	if (previousSelection == 23 && pad == UI_PAD_DPAD_UP)
		target = 20;
	else if (previousSelection == 20 && pad == UI_PAD_DPAD_DOWN)
		target = 23;
	else if (ui->selectedIndex < 0 &&
		(pad == UI_PAD_DPAD_UP || pad == UI_PAD_DPAD_DOWN ||
		pad == UI_PAD_DPAD_LEFT || pad == UI_PAD_DPAD_RIGHT))
		target = previousSelection == 20 ? 23 : 20;

	if (target >= 0 && ui->selectedIndex != target) {
		if (ui->selectedIndex >= 0 && ui->selectedIndex < ui->childCount) {
			UiElementText_t* previous = ui->pChildren[ui->selectedIndex];
			if (previous->state == 4)
				((UiMenuVTable_t*)previous->vTable)->setState(previous, 3);
		}
		UiElementText_t* child = ui->pChildren[target];
		((UiMenuVTable_t*)child->vTable)->setState(child, 4);
		ui->selectedIndex = target;
	}
	if (blockJoin)
		elements->join->state = 2;
	if (*(int*)((u8*)ui + 0x314) == 22)
		*(int*)((u8*)ui + 0x314) = 20;
	UiElementList_t* list = elements->playerList;
	if (!list || list->type != UI_ELEMENT_LIST || !elements->rankSprite[0])
		return result;

	HOOK_JAL(PLAYER_LIST_COLOR_CALL, &setGameDetailsPlayerListColor);

	// Keep the native sprite elements on the game heap, outside patch/mode RAM.
	if (!teamSprites) {
		teamSprites = malloc(sizeof(*teamSprites) * GAME_MAX_PLAYERS);
		if (!teamSprites)
			return result;
	}

	// Use the same native sprite element and vtable as the rank/skill bolts.
	for (row = 0; row < GAME_MAX_PLAYERS; ++row) {
		for (layer = 0; layer < 2; ++layer) {
			UiElementSprite_t* sprite = &teamSprites[row][layer];
			if (!gameDetailsSpritesInitialized) {
				memcpy(sprite, elements->rankSprite[0], sizeof(*sprite));
				sprite->unk_64 = -1;
				sprite->sprite = SPRITE_HUD_DOT;
				sprite->selectorBoxSize[2] = layer ? 0.021f : 0.030f;
				sprite->selectorBoxSize[3] = layer ? 0.028f : 0.040f;
				((UiMenuVTable_t*)sprite->vTable)->return_zero_00c(sprite);
			}
			sprite->state = 0;
			((UiMenuVTable_t*)sprite->vTable)->draw(sprite);
		}
	}
	gameDetailsSpritesInitialized = 1;

	int count = *(int*)((u8*)ui + 0x2bc);
	gameDetailsPlayer_t* players = *(gameDetailsPlayer_t**)((u8*)ui + 0x2c4);
	int first = *(int*)((u8*)list + 0x10580);
	int visible = *(int*)((u8*)list + 0x1057c);
	if (!players || count < 1 || count > GAME_MAX_PLAYERS || first < 0 ||
		visible < 1 || visible > GAME_MAX_PLAYERS)
		return result;

	// Native list coordinates are centered; its text rows use 0.0505 spacing.
	float x = list->selectorBoxSize[0] + list->selectorBoxSize[2] * 0.5f - 0.010f;
	float y = list->selectorBoxSize[1] - list->selectorBoxSize[3] * 0.5f + 0.022f;
	for (row = 0; row < visible && first + row < list->itemCount && first + row < count; ++row) {
		// Native list population uses the same order as these roster records.
		u8* team = &players[first + row].stats[0xfc];
		if (team[0] != 'U' || team[1] != 'Y' || team[2] != 'A' || team[3] >= 8)
			continue;

		for (layer = 0; layer < 2; ++layer) {
			UiElementSprite_t* sprite = &teamSprites[row][layer];
			sprite->state = 1;
			sprite->selectorBoxSize[0] = x;
			sprite->selectorBoxSize[1] = y + row * 0.0505f;
			sprite->sprite = SPRITE_HUD_DOT;
			sprite->vTable->setColor(sprite, layer ?
				0x80000000 | TEAM_COLORS[team[3]] : 0x706ec8ff);
			((UiMenuVTable_t*)sprite->vTable)->draw(sprite);
		}
	}
	return result;
}
// extern forceMapOverride;
/*
 * NAME : patchStaging
 * DESCRIPTION : Apply staging options while the native lobby session is valid.
 * NOTES : Recheck session pointers after native updates and modal dialogs.
 * ARGS : Staging menu and native UI pad action.
 * RETURN : Native menu update result.
 * AUTHOR : Troy "Metroynome" Pruitt
 */
int patchStaging(UiMenu_t* ui, int pad)
{
    static int init_staging = 0;
    static int allPlayersReady = -1;
    static int pressedL1 = -1;
    static int holdingL1time = 0; 
    int i;
    int j;
    GameSettings* gs;
    GameOptions* options;
    int clientId;
    int isTeams;
    int itemSelected;
    int isLoading;
    int isRequstingTeamChange;

    // check for connection before reading staging/game pointers; the disconnect popup can tear these down.
    if (!ui)
        return 0;
	if (!netGetLobbyServerConnection()) {
		init_staging = 0;
		allPlayersReady = -1;
		pressedL1 = -1;
		return stagingFunc(ui, pad);
	}

    gs = gameGetSettings();
    options = gameGetOptions();
    if (!gs || !options)
        return stagingFunc(ui, pad);

    clientId = gameGetMyClientId();
    isTeams = options->GameFlags.MultiplayerGameFlags.Teams;
    itemSelected = *(int*)((u32)ui + 0x290);
    isLoading = *(u8*)((u32)ui + 0x2b6);
    isRequstingTeamChange = *(u8*)((u32)ui + 0x2c8);

    // Change pad buttons for all players
    if (pad == UI_PAD_CROSS) {
        pad = openPlayerOptions(ui, gs, itemSelected, isTeams);
		if (!netGetLobbyServerConnection() || gameGetSettings() != gs ||
			uiGetActiveMenu(UI_MENU_STAGING, 0) != ui)
			return 0;
    }
    // Change pad buttons on for host
    if (gameAmIHost()) {
        if (pad == UI_PAD_CIRCLE && allPlayersReady == 1) {
            // Force "Start" to be selected
            ui->selectedIndex = 4;
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

    // The original staging function may tear down these pointers after Disconnect.
	gs = gameGetSettings();
	options = gameGetOptions();
	if (!netGetLobbyServerConnection() || !gs || !options ||
		uiGetActiveMenu(UI_MENU_STAGING, 0) != ui) {
		init_staging = 0;
		allPlayersReady = -1;
		pressedL1 = -1;
		return result;
	}
	isTeams = options->GameFlags.MultiplayerGameFlags.Teams;

    //  init staging if not done already, or equals 2
    if (!init_staging || info.currentWorldId != gameGetWorldId()) {
		// Disconnect cleanup can remove these lists before the lobby connection.
		if (!BUDDY_IGNORE_LIST_PTR || !BUDDY_LIST_STACK || !IGNORE_LIST_STACK ||
			!*(u32*)(BUDDY_LIST_STACK + 0x40) || !*(u32*)(IGNORE_LIST_STACK + 0x40))
			return result;
        getBuddyList(BUDDY_LIST_STACK, 2);
        getIgnoreList(IGNORE_LIST_STACK, 0);
		if (!netGetLobbyServerConnection() || gameGetSettings() != gs ||
			uiGetActiveMenu(UI_MENU_STAGING, 0) != ui || !BUDDY_IGNORE_LIST_PTR ||
			!BUDDY_LIST_STACK || !IGNORE_LIST_STACK)
			return result;
        info.currentWorldId = gameGetWorldId();
        info.buddyCount = BUDDY_LIST_COUNT;
        info.ignoredCount = IGNORE_LIST_COUNT;
        memset(info.client_ids, -1, sizeof(info.client_ids));
        memset(info.buddies, -1, sizeof(info.buddies));
        memset(info.ignored, -1, sizeof(info.ignored));
        init_staging = 1;
    }

    // if host
    if (gameAmIHost()) {
        // Opem Teams Options Menu
        // if teams and config menu is closed and host hasn't started game
        if (isTeams && !isConfigMenuActive && gs->PlayerStates[0] != 7) {
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
				if (!netGetLobbyServerConnection() || gameGetSettings() != gs ||
					uiGetActiveMenu(UI_MENU_STAGING, 0) != ui)
					return result;
            }
        }

        // Set host to always ready
        if (gs->PlayerStates[0] != 7)
            gs->PlayerStates[0] = 6;    
        
        // Replace "\x11 UNREADY" with "\x11 STSART"
        int str = uiMsgString(0x16d9);
        if (str && allPlayersReady)
	        strncpy((char*)str, "\x11 START", 9);
        else if (str)
	        strncpy((char*)str, "         ", 9);
        
        // check if all players are ready
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            // if not all players are ready
            if (gs->PlayerClients[i] > -1 && gs->PlayerStates[i] < 6) {
                allPlayersReady = 0;
            } else if (gs->PlayerClients[i] > -1 && gs->PlayerStates[i] == 6) {
                allPlayersReady = 1;
            } else if (allPlayersReady == 2) {
                allPlayersReady = -1;
            }
        }
    } else {
        // Nont Host Stuff
        // Make it so all clients can scroll over users names
        if (*(u32*)OPTIONS_SCROLL_CHECK == 0x24050001)
            *(u32*)OPTIONS_SCROLL_CHECK = 0x24050003;

        // Unkick (never worked)
        // for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        //     // if server client id matches local client id.
        //     if (gs->PlayerClients[i] == clientId) {
        //         // Patch Unkick: if leave game pop is up, close it.
        //         u32 popup = uiGetActiveMenu(11, 1);
        //         if (gs->PlayerStates[i] == 5 && popup > 0) {
        //             // Compare popup text and "Leave Game" string.
        //             if (strcmp(*(u32*)(popup + 0x110) + 0x64, uiMsgString(0x1643))) {
        //                 *(u32*)0x01C5C284 = 0x2c;
        //             }
        //         }
        //     }
        // }
    }
	return result;
}

int patchCreateGame(UiMenu_t* ui, long pad)
{
    int time_limit = 120; // in minutes
 
    // check for connetion
    if (!netGetLobbyServerConnection())
        return createGameFunc(ui, pad);

    int result = createGameFunc(ui, pad);

    UiCreateGameElements_t * createGame = &ui->pChildren;
    if (createGame->time->rangeMax != time_limit) {
        // Change Time Limit
        createGame->time->rangeMax = time_limit;
        // Change time limit if frag limit is 0.
        *(u32*)((u32)CREATE_GAME_BASE_FUNC + 0x3f8) = 0x24040000 | time_limit;
    }
    // Let password field always be active.
    if (createGame->usePassword->state == 2) {
        createGame->usePassword->state = 3;
        createGame->password->state = 3;
    }
    // Lock local players to 1
    if (createGame->localPlayers->state == 3) {
        createGame->localPlayers->state = 2;
        createGame->localPlayers->rangeMax = 1;
    }

    return result;
}

int patchAdvancedOptions(UiMenu_t* ui, int pad)
{
    static int savedTeams = 0;
    int frag_limit = 50;
    int ctfCaps_limit = 50;

    // check for connetion
    if (!netGetLobbyServerConnection())
        return advancedOptionsFunc(ui, pad);

    int result = advancedOptionsFunc(ui, pad);
    UiAdvancedOptionsElements_t* ao = &ui->pChildren;
    UiMenu_t* menu = uiGetMenu(UI_MENU_CREATE_GAME);
    UiCreateGameElements_t* createGame = &menu->pChildren;
    int isDeathmatch = createGame->mode->selectedIndex == 2;
    if (isDeathmatch) {
        // set savedteams to last team value.
        savedTeams = ao->teams->selectedIndex;
        if (ao->fragLimit->rangeMax != frag_limit)
            ao->fragLimit->rangeMax = frag_limit;
    } else if (createGame->mode->selectedIndex == 1) {
        if (ao->ctfCaps->rangeMax != ctfCaps_limit)
            ao->ctfCaps->rangeMax = ctfCaps_limit;
    }
    if (isDeathmatch && ao->teams->selectedIndex != savedTeams)
        ao->teams->selectedIndex = savedTeams;
    
    return result;
}

// int patchBuddies(void * ui, long pad)
// {
//     static int pressedSquare = -1;
//     static int holdingSquareTime = 0;
//     u32 * uiElements = (u32*)((u32)ui + 0x110);
//     int itemSelected = *(int*)(ui + 0x290);
//     int isLoading = *(u8*)(ui + 0x2b6);

//     // check for connetion
//     if (!netGetLobbyServerConnection())
//         return buddiesFunc(ui, pad);

//     int result = buddiesFunc(ui, pad);

//     return result;
// }

// int patchPlayerDetails(void * ui, long pad)
// {
//     u32 * uiElements = (u32*)((u32)ui + 0x110);
//     int itemSelected = *(int*)(ui + 0x290);
//     int account_id = *(u32*)(ui + 0x2a8);

//     // check for connetion
//     if (!netGetLobbyServerConnection())
//         return playerDetailsFunc(ui, pad);

//     int result = playerDetailsFunc(ui, pad);

//     return result;
// }

// int patchStats(void * ui, int pad)
// {
//     u32 * uiElements = (u32*)((u32)ui + 0x110);
//     int itemSelected = *(int*)(ui + 0x290);

//     // check for connetion
//     if (!netGetLobbyServerConnection())
//         return statsFunc(ui, pad);

//     int result = statsFunc(ui, pad);

//     return result;
// }

int patchKeyboard(UiMenu_t * ui, int pad)
{
    int i;
    int selectedIndex = ui->selectedIndex;
    // UiElement_t * keyboardEnd = (UiElement_t*)((u32)ui + 0x294);
    // int keyboardSize = *(u32*)(u32)ui + 0x298;
    // UiElement_t * keyboardStart = (UiElement_t*)((u32)ui + 0x2a4);
    int currentPage = ((u32)ui + 0x2b0);
    int pages = ((u32)ui + 0x2b4);
    static int setPages = 0;

    int result = keyboardFunc(ui, pad);

    // if on clan details or cities menu, then update keyboard.
    if (uiGetActiveMenu(UI_MENU_CLAN_DETAILS, 0) > 0 || uiGetActiveMenu(UI_MENU_CITY, 0) > 0) {
        // get children
        // Update number of pages.  We have to account for different number of pages per language.
        if (setPages !=  *(int*)pages) {
            *(int*)pages += 1;
            setPages =  *(int*)pages;
        }
        // Change SHIFT to COLOR if on page 2
        if (*(int*)currentPage ==  *(int*)pages - 2) {
            strncpy(ui->pChildren[KEY_SHIFT]->title, "\x14MISC", 6);
        } else {
            strncpy(ui->pChildren[KEY_SHIFT]->title, "\x14SHIFT", 6); 
        }
        // Update keyboard
        if (*(int*)currentPage ==  *(int*)pages - 1) {
            for (i = 0; i < COUNT_OF(keys); ++i) {
                // don't let the use of buttons in clan tags
                if (uiGetActiveMenu(UI_MENU_CLAN_DETAILS, 0) > 0 && keys[i].index < KEY_1)
                    break;

                strncpy(ui->pChildren[keys[i].index]->title, keys[i].text, 3);
            }
        }
    }

    return result;  
}
