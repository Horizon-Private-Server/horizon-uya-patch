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
#else
#define STAGING_BASE_FUNC (0x006bec18)
#define CREATE_GAME_BASE_FUNC (0x00697e20)
#define BUDDIES_BASE_FUNC (0x0068e6f8)
#define PLAYER_DETAILS_BASE_FUNC (0x006b1f60)
#define STATS_BASE_FUNC (0x006c31b8)
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
} uiPadButtons_t;

typedef int (*uiVTable_Func)(void * ui, int pad);
uiVTable_Func createGameFunc = (uiVTable_Func)CREATE_GAME_BASE_FUNC;
uiVTable_Func stagingFunc = (uiVTable_Func)STAGING_BASE_FUNC;
uiVTable_Func buddiesFunc = (uiVTable_Func)BUDDIES_BASE_FUNC;
uiVTable_Func playerDetailsFunc = (uiVTable_Func)PLAYER_DETAILS_BASE_FUNC;
uiVTable_Func statsFunc = (uiVTable_Func)STATS_BASE_FUNC;

const char * SELECT_VOTE_TITLE = "first try?";
const char * SELECT_VOTE_NAMES[] = {
  "OMG!",
  "IT TWAS",
  "HUG YOUR MOTHER",
  "SUPPORT YOURSELF",
  "GIVE ME HUGS! :D"
};
const int SELECT_VOTE_NAMES_SIZE = sizeof(SELECT_VOTE_NAMES)/sizeof(char*);

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
        } else if (pad == UI_PAD_L1) {
            // setTeams(2);
            // int a = uiShowSelectDialog(SELECT_VOTE_TITLE, SELECT_VOTE_NAMES, SELECT_VOTE_NAMES_SIZE, 0);
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_R1) {
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_L2) {
            // setTeams(8);
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_R2) {
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
