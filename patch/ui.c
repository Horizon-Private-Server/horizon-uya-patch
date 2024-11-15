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

typedef struct ChangeTeamRequest {
	u32 Seed;
	int PoolSize;
	char Pool[GAME_MAX_PLAYERS];
} ChangeTeamRequest_t;

typedef struct ForceTeamsRequest {
    int AccountIds[GAME_MAX_PLAYERS];
    char Teams[GAME_MAX_PLAYERS];
} ForceTeamsRequest_t;
ForceTeamsRequest_t TeamsRequest;

typedef int (*uiVTable_Func)(void * ui, int pad);
uiVTable_Func createGameFunc = (uiVTable_Func)CREATE_GAME_BASE_FUNC;
uiVTable_Func stagingFunc = (uiVTable_Func)STAGING_BASE_FUNC;
uiVTable_Func buddiesFunc = (uiVTable_Func)BUDDIES_BASE_FUNC;
uiVTable_Func playerDetailsFunc = (uiVTable_Func)PLAYER_DETAILS_BASE_FUNC;
uiVTable_Func statsFunc = (uiVTable_Func)STATS_BASE_FUNC;

// Data Recieved from server
void setTeams(int num)
{
    int i, j;
    ChangeTeamRequest_t request;
	u32 seed;
	char teamByClientId[GAME_MAX_PLAYERS];

    request.Seed = timerGetSystemTime();
    request.PoolSize = num;
	memcpy(&seed, &request.Seed, 4);
	memset(teamByClientId, -1, sizeof(teamByClientId));

	// printf("pool size: %d\npool: ", request.PoolSize);
	// for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	// 	printf("%d=%d,", i, request.Pool[i]);
	// printf("\n");

	// get game settings
	GameSettings* gameSettings = gameGetSettings();
	if (gameSettings) {
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			int clientId = gameSettings->PlayerClients[i];
			if (clientId >= 0) {
				int teamId = teamByClientId[clientId];
				if (teamId < 0) {
					if (request.PoolSize == 0) {
						teamId = 0;
					} else {
						// psuedo random
						sha1(&seed, 4, &seed, 4);

						// get pool index from rng
						int teamPoolIndex = seed %  request.PoolSize;

						// set team
						teamId = request.Pool[teamPoolIndex];

						printf("\npool info pid: %d\npoolIndex: %d\npoolSize: %d\nteam: %d", i, teamPoolIndex, request.PoolSize, teamId);

						// remove element from pool
						if (request.PoolSize > 0) {
							for (j = teamPoolIndex+1; j < request.PoolSize; ++j)
								request.Pool[j-1] = request.Pool[j];
							request.PoolSize -= 1;
						}
					}
					// set client id team
					teamByClientId[clientId] = teamId;
				}
				// set team
				printf("\nsetting pid:%d to %d", i, teamId);
				gameSettings->PlayerTeams[i] = teamId;
			}
		}
	}
}

int onForceTeamsRequest(void * connection, void * data)
{
    // move message payload into local
    memcpy(&TeamsRequest, data, sizeof(ForceTeamsRequest_t));

    return sizeof(ForceTeamsRequest_t);
}

void doTheTeams(void)
{
    int i, j;
    GameSettings* gs = gameGetSettings();
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        // force teams
        int accountId = TeamsRequest.AccountIds[i];
        for (j = 0; j < GAME_MAX_PLAYERS; ++j) {
            if (gs->PlayerAccountIds[j] == accountId) {
                gs->PlayerTeams[j] = TeamsRequest.Teams[i];
                break;
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

    // Install net messsages
    // static int init = 0;
    // if (!init) {
    //     netInstallCustomMsgHandler(CUSTOM_MSG_ID_FORCE_TEAMS_REQUEST, &onForceTeamsRequest);
    //     init = 1;
    // }

    if (gameAmIHost()) {
        if (pad == UI_PAD_CIRCLE && allPlayersReady == 1) {
            // hostStartGame(ui);

            // Force "Start" to be selected
            *(int*)((u32)ui + 0x290) = 4;
            allPlayersReady = 2;
            pad = UI_PAD_CROSS;
        } else if (pad == UI_PAD_L1) {
            // netSendCustomAppMessage(NET_DELIVERY_CRITICAL, netGetLobbyServerConnection(), CUSTOM_MSG_ID_VOTE_REQUEST, sizeof(request), &request);
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_R1) {
            setTeams(2);
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_R2) {
            setTeams(0);
            pad = UI_PAD_NONE;
        } else if (pad == UI_PAD_L2) {
            setTeams(8);
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
