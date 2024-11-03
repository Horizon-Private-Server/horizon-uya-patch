#include <tamtypes.h>
#include <libuya/string.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/pad.h>
#include <libuya/stdio.h>
#include <libuya/uya.h>
#include "messageid.h"
#include "config.h"

#ifdef UYA_PAL
#define STAGING_BASE_FUNC (0x006c1730)
#else
#define STAGING_BASE_FUNC (0x006bec18)
#endif

typedef enum uiPadButtons {
    UI_PAD_NONE = 0,
    UI_PAD_DPAD_UP,
    UI_PAD_DPAD_DOWN,
    UI_PAD_DPAD_LEFT,
    UI_PAD_DPAD_RIGHT,
    UI_PAD_TRIANGLE,
    UI_PAD_CIRCLE,
    UI_PAD_CROSS,
    UI_PAD_SQUARE,
    UI_PAD_SELECT,
    UI_PAD_START,
    UI_PAD_L1,
    UI_PAD_L2,
    UI_PAD_L3,
    UI_PAD_R1,
    UI_PAD_R2,
    UI_PAD_R3,
    UI_PAD_TOTAL = UI_PAD_R3
} uiPadButtons_t;

typedef struct ChangeTeamRequest {
	u32 Seed;
	int PoolSize;
	char Pool[GAME_MAX_PLAYERS];
} ChangeTeamRequest_t;

int allPlayersReady = -1;

typedef int (*uiVTable_Func)(void * ui, int pad);
uiVTable_Func stagingFunc = (uiVTable_Func)STAGING_BASE_FUNC;

// Data Recieved from server
// int onRecieveSetTeams(void * connection, void * data)
// {
//     int i, j;
//     ChangeTeamRequest_t request;
// 	u32 seed;
// 	char teamByClientId[GAME_MAX_PLAYERS];

// 	memcpy(&request, data, sizeof(ChangeTeamRequest_t));
// 	memcpy(&seed, &request.Seed, 4);
// 	memset(teamByClientId, -1, sizeof(teamByClientId));

// 	printf("pool size: %d\npool: ", request.PoolSize);
// 	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
// 		printf("%d=%d,", i, request.Pool[i]);
// 	printf("\n");

// 	// get game settings
// 	GameSettings* gameSettings = gameGetSettings();
// 	if (gameSettings) {
// 		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
// 			int clientId = gameSettings->PlayerClients[i];
// 			if (clientId >= 0) {
// 				int teamId = teamByClientId[clientId];
// 				if (teamId < 0) {
// 					if (request.PoolSize == 0) {
// 						teamId = 0;
// 					} else {
// 						// psuedo random
// 						sha1(&seed, 4, &seed, 4);

// 						// get pool index from rng
// 						int teamPoolIndex = seed % request.PoolSize;

// 						// set team
// 						teamId = request.Pool[teamPoolIndex];

// 						printf("pool info pid:%d poolIndex:%d poolSize:%d team:%d\n", i, teamPoolIndex, request.PoolSize, teamId);

// 						// remove element from pool
// 						if (request.PoolSize > 0) {
// 							for (j = teamPoolIndex+1; j < request.PoolSize; ++j)
// 								request.Pool[j-1] = request.Pool[j];
// 							request.PoolSize -= 1;
// 						}
// 					}
// 					// set client id team
// 					teamByClientId[clientId] = teamId;
// 				}
// 				// set team
// 				DPRINTF("setting pid:%d to %d\n", i, teamId);
// 				gameSettings->PlayerTeams[i] = teamId;
// 			}
// 		}
// 	}
// 	return sizeof(ChangeTeamRequest_t);
// }

// // Data sent to server
// void onSendSetTeams(void)
// {

// }

void hostStartGame(void)
{
    static int allPlayersReady = 1;
    GameSettings* gs = gameGetSettings();
    int clientId = gameGetMyClientId();
    int i;
    if (!gs || !gameAmIHost())
        return;

    // start
    int playerCount = gs->PlayerCount;
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        // if not all players are ready
        if (gs->PlayerStates[i] == 6) {
            gs->PlayerStates[i] = 7;
        }
    }
    gs->GameLoadStartTime = gameGetTime() + 4000;
}

int patchStaging(void * ui, int pad)
{
    int i;
    int j;
    GameSettings* gs = gameGetSettings();
    int clientId = gameGetMyClientId();
    u32 * uiElements = (u32*)((u32)ui + 0x110);

    // Install net messsages
    // static int init = 0;
    // if (!init) {
	//     netInstallCustomMsgHandler(CUSTOM_MSG_ID_SERVER_REQUEST_TEAM_CHANGE, &onRecieveSetTeams);
    //     init = 1;
    // }

    if (gameAmIHost()) {
        if (pad == UI_PAD_CIRCLE && allPlayersReady == 1) {
            hostStartGame();
            allPlayersReady = 2;
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

	// if (pad != UI_PAD_NONE)
	// 	DPRINTF("\nbutton: %d, ui: %08x", pad, ui);

	return result;
}
