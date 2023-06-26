/***************************************************
 * FILENAME :		main.c
 * 
 * DESCRIPTION :
 * 		PAYLOAD.
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>
#include <string.h>

#include <libuya/time.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/player.h>
#include <libuya/ui.h>
#include <libuya/stdio.h>
#include <libuya/graphics.h>
#include <libuya/net.h>
#include <libuya/uya.h>
#include "module.h"
#include "config.h"
#include "messageid.h"
#include "include/game.h"

#define SPLEEF_SPAWN_MOBY                   (MOBY_ID_CRATE_RANDOM_PICKUP)
#define SPLEEF_BOARD_DIMENSION              (10)
#define SPLEEF_BOARD_LEVELS					(2)
#define SPLEEF_BOARD_LEVEL_OFFSET           (40.0)
#define SPLEEF_BOARD_BOX_SIZE               (4.0)
#define SPLEEF_BOARD_SPAWN_RADIUS           (SPLEEF_BOARD_BOX_SIZE * ((SPLEEF_BOARD_DIMENSION + SPLEEF_BOARD_DIMENSION) / 5))
#define SPLEEF_BOARD_BOX_MAX                (SPLEEF_BOARD_DIMENSION * SPLEEF_BOARD_DIMENSION * SPLEEF_BOARD_LEVELS)

const char * SPLEEF_ROUND_WIN = "First!";
const char * SPLEEF_ROUND_SECOND = "Second!";
const char * SPLEEF_ROUND_THIRD = "Third!";
const char * SPLEEF_ROUND_LOSS = "Better luck next time!";

int Initialized = 0;

SpleefState_t SpleefState;

typedef struct SpleefOutcomeMessage
{
	char Outcome[4];
} SpleefOutcomeMessage_t;

typedef struct SpleefDestroyBoxMessage
{
	short BoxId;
	char PlayerId;
} SpleefDestroyBoxMessage_t;

struct SpleefGameData
{
	u32 Version;
	u32 Rounds;
	int Points[GAME_MAX_PLAYERS];
	int BoxesDestroyed[GAME_MAX_PLAYERS];
};

enum GameNetMessage
{
	CUSTOM_MSG_SET_OUTCOME = CUSTOM_MSG_ID_GAME_MODE_START,
	CUSTOM_MSG_DESTROY_BOX
};

Moby * SpleefBox[SPLEEF_BOARD_DIMENSION * SPLEEF_BOARD_DIMENSION * SPLEEF_BOARD_LEVELS];

// Position that boxes are spawned to.
VECTOR StartPos = {
	400,
	400,
	800,
	0
};

//--------------------------------------------------------------------------
void getWinningPlayer(int * winningPlayerId, int * winningPlayerScore)
{
	int i;
	int pId = 0;
	int pScore = 0;
	Player ** players = playerGetAll();
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
		if (SpleefState.PlayerPoints[i] > pScore)
		{
			pId = i;
			pScore = SpleefState.PlayerPoints[i];
		}
	}

	*winningPlayerScore = pScore;
	*winningPlayerId = pId;
}

//--------------------------------------------------------------------------
void onSetRoundOutcome(char outcome[4])
{
	memcpy(SpleefState.RoundResult, outcome, 4);
	DPRINTF("outcome set to %d,%d,%d,%d\n", outcome[0], outcome[1], outcome[2], outcome[3]);
}

//--------------------------------------------------------------------------
// int onSetRoundOutcomeRemote(void * connection, void * data)
// {
// 	SpleefOutcomeMessage_t * message = (SpleefOutcomeMessage_t*)data;
// 	onSetRoundOutcome(message->Outcome);

// 	return sizeof(SpleefOutcomeMessage_t);
// }

//--------------------------------------------------------------------------
void setRoundOutcome(int first, int second, int third)
{
	SpleefOutcomeMessage_t message;

	// don't allow overwriting existing outcome
	if (SpleefState.RoundResult[0])
		return;

	// don't allow changing outcome when not host
	if (!SpleefState.IsHost)
		return;

	// send out
	message.Outcome[0] = 1;
	message.Outcome[1] = first;
	message.Outcome[2] = second;
	message.Outcome[3] = third;
	// netBroadcastCustomAppMessage(NET_DELIVERY_CRITICAL, netGetDmeServerConnection(), CUSTOM_MSG_SET_OUTCOME, sizeof(SpleefOutcomeMessage_t), &message);

	// set locally
	onSetRoundOutcome(message.Outcome);
}

//--------------------------------------------------------------------------
// void onDestroyBox(int id, int playerId)
// {
// 	Moby* box = SpleefBox[id];
// 	if (box && box->OClass == MOBY_ID_NODE_BOLT_GUARD && !mobyIsDestroyed(box))
// 	{
// 		mobyDestroy(box);
// 	}

// 	SpleefBox[id] = NULL;

// 	// 
// 	if (playerId >= 0)
// 		SpleefState.PlayerBoxesDestroyed[playerId]++;

// 	DPRINTF("box destroyed %d by %d\n", id, playerId);
// }

//--------------------------------------------------------------------------
// int onDestroyBoxRemote(void * connection, void * data)
// {
// 	SpleefDestroyBoxMessage_t * message = (SpleefDestroyBoxMessage_t*)data;

// 	// if the round hasn't ended
// 	if (!SpleefState.RoundEndTicks)
// 		onDestroyBox(message->BoxId, message->PlayerId);

// 	return sizeof(SpleefDestroyBoxMessage_t);
// }

//--------------------------------------------------------------------------
void destroyBox(int id, int playerId)
{
	SpleefDestroyBoxMessage_t message;

	// send out
	message.BoxId = id;
	message.PlayerId = playerId;
	// netBroadcastCustomAppMessage(NET_DELIVERY_CRITICAL, netGetDmeServerConnection(), CUSTOM_MSG_DESTROY_BOX, sizeof(SpleefDestroyBoxMessage_t), &message);

	// 
	if (playerId >= 0)
		SpleefState.PlayerBoxesDestroyed[playerId]++;
		
	DPRINTF("sent destroy box %d\n", id);
}

//--------------------------------------------------------------------------
int gameGetTeamScore(int team, int score)
{
	int i = 0;
	int totalScore = 0;
	Player** players = playerGetAll();

	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		if (players[i] && players[i]->Team == team) {
			totalScore += SpleefState.PlayerPoints[i];
		}
	}
	
	return totalScore;
}

//--------------------------------------------------------------------------
void boxUpdate(Moby * moby)
{
	// MobyColDamage * colDamage = mobyGetDamage(moby, 0xfffffff, 0);
	if (moby->State == 2)
		mobyDestroy(moby);

	((void (*)(Moby*))0x004184E0)(moby);
}

//--------------------------------------------------------------------------
void drawRoundMessage(const char * message, float scale)
{
	GameSettings * gameSettings = gameGetSettings();
	char* rankStrings[] = { "1st", "2nd", "3rd" };
	int fw = gfxGetFontWidth(message, -1, scale);
	float x = 0.5;
	float y = 0.16;
	float p = 0.02;
	float w = maxf(196.0, fw);
	float h = 120.0;
	int i;

	// draw container
	gfxScreenSpaceBox(x-(w/(SCREEN_WIDTH*2.0)), y, (w / SCREEN_WIDTH) + p, (h / SCREEN_HEIGHT) + p, 0x20ffffff);

	// draw message
	y *= SCREEN_HEIGHT;
	x *= SCREEN_WIDTH;
	gfxScreenSpaceText(x, y + 5, scale, scale * 1.5, 0x80FFFFFF, message, -1, 1);

	// draw ranks
	y += 24.0 * scale;
	scale *= 0.5;
	w /= 1.5;
	for (i = 1; i < 4; ++i)
	{
		int pId = SpleefState.RoundResult[i];
		if (pId >= 0)
		{
			y += 18.0 * scale;
			gfxScreenSpaceText(x-(w/2), y, scale, scale, 0x80FFFFFF, rankStrings[i-1], -1, 3);
			gfxScreenSpaceText(x+(w/2), y, scale, scale, 0x80FFFFFF, gameSettings->PlayerNames[pId], -1, 5);
		}
	}
}

//--------------------------------------------------------------------------
void updateGameState(PatchStateContainer_t * gameState)
{
	int i,j;

	// game state update
	if (gameState->UpdateGameState)
	{
		gameState->GameStateUpdate.RoundNumber = SpleefState.RoundNumber + 1;
	}

	// stats
	if (gameState->UpdateCustomGameStats)
	{
    	gameState->CustomGameStatsSize = sizeof(struct SpleefGameData);
		struct SpleefGameData* sGameData = (struct SpleefGameData*)gameState->CustomGameStats.Payload;
		sGameData->Rounds = SpleefState.RoundNumber+1;
		DPRINTF("spleef ran for %d rounds\n", sGameData->Rounds);
		sGameData->Version = 0x00000001;
		
		for (i = 0; i < GAME_MAX_PLAYERS; ++i)
		{
			DPRINTF("%d: %d points %d boxes\n", i, SpleefState.PlayerPoints[i], SpleefState.PlayerBoxesDestroyed[i]);
			sGameData->Points[i] = SpleefState.PlayerPoints[i];
			sGameData->BoxesDestroyed[i] = SpleefState.PlayerBoxesDestroyed[i];
		}
	}
}

//--------------------------------------------------------------------------
void resetRoundState(void)
{
	GameSettings * gameSettings = gameGetSettings();
	Player ** players = playerGetAll();
	int gameTime = gameGetTime();
	int i,j,k, count=0;
	VECTOR pos, rot = {0,0,0,0}, center;
	Moby* hbMoby = 0;

	// 
	SpleefState.RoundInitialized = 0;
	SpleefState.RoundStartTicks = gameTime;
	SpleefState.RoundEndTicks = 0;
	SpleefState.RoundResult[0] = 0;
	SpleefState.RoundResult[1] = -1;
	SpleefState.RoundResult[2] = -1;
	SpleefState.RoundResult[3] = -1;
	memset(SpleefState.RoundPlayerState, -1, GAME_MAX_PLAYERS);

	// Center
	center[0] = StartPos[0] + (SPLEEF_BOARD_BOX_SIZE * (SPLEEF_BOARD_DIMENSION / (float)2.0));
	center[1] = StartPos[1] + (SPLEEF_BOARD_BOX_SIZE * (SPLEEF_BOARD_DIMENSION / (float)2.0));
	center[2] = StartPos[2];

	// spawn players
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
		Player * p = players[i];
		if (!p)
			continue;

		// set state
		SpleefState.RoundPlayerState[i] = 0;

		// move to spawn
		float theta = (p->PlayerId / (float)gameSettings->PlayerCount) * (float)2.0 * MATH_PI;
		while (theta > MATH_TAU)
			theta -= MATH_PI;

		pos[0] = center[0] + (cosf(theta) * SPLEEF_BOARD_SPAWN_RADIUS);
		pos[1] = center[1] + (sinf(theta) * SPLEEF_BOARD_SPAWN_RADIUS);
		pos[2] = center[2] + (float)30;

		// 
		rot[2] = theta - MATH_PI;

		// 
		playerRespawn(p);
		playerSetPosRot(p, pos, rot);
	}

	// reset boxes
	vector_copy(pos, StartPos);
	memset(rot, 0, sizeof(rot));

	// Spawn boxes
	for (k = 0; k < SPLEEF_BOARD_LEVELS; ++k)
	{
		for (i = 0; i < SPLEEF_BOARD_DIMENSION; ++i)
		{
			for (j = 0; j < SPLEEF_BOARD_DIMENSION; ++j)
			{
				// delete old one
				int boxId = (k * SPLEEF_BOARD_DIMENSION * SPLEEF_BOARD_DIMENSION) + (i * SPLEEF_BOARD_DIMENSION) + j;
				if (!SpleefBox[boxId] || SpleefBox[boxId]->OClass != SPLEEF_SPAWN_MOBY || mobyIsDestroyed(SpleefBox[boxId]))
				{
					// spawn
					SpleefBox[boxId] = hbMoby = mobySpawn(SPLEEF_SPAWN_MOBY, 0);

					if (hbMoby)
					{
						vector_copy(hbMoby->Position, pos);

						hbMoby->UpdateDist = 0xFF;
						hbMoby->Drawn = 0x01;
						hbMoby->DrawDist = 0x0080;
						hbMoby->Opacity = 0x80;
						hbMoby->State = 1;

						hbMoby->Scale = (float)0.0425 * SPLEEF_BOARD_BOX_SIZE;
						// hbMoby->Lights = 0x202;
						// hbMoby->GuberMoby = 0;
						hbMoby->PUpdate = &boxUpdate;

						// For this model the vector here is copied to 0x80 in the moby
						// This fixes the occlusion bug
						// vector_copy(hbMoby->LSphere, StartUNK_80);

						++count;
					}
				}

				pos[1] += SPLEEF_BOARD_BOX_SIZE;
			}

			pos[0] += SPLEEF_BOARD_BOX_SIZE;
			pos[1] = StartPos[1];
		}

		pos[0] = StartPos[0];
		pos[1] = StartPos[1];
		pos[2] -= SPLEEF_BOARD_LEVEL_OFFSET;
	}

	// 
	SpleefState.RoundInitialized = 1;

	// this has to be here otherwise the rounds won't reset correctly
	// think that this eats cycles and that helps sync things maybe?
	// not super sure
	printf("count: %d,, new: %08x\n", count, (u32)hbMoby);

	// 
#if DEBUG
	if (hbMoby)
		hbMoby->Opacity = 0xFF;
	printf("Round %d started\n", SpleefState.RoundNumber);
#endif
}

//--------------------------------------------------------------------------
int whoKilledMeHook(void)
{
	return 0;
}

void initialize(PatchGameConfig_t* gameConfig, PatchStateContainer_t* gameState)
{
	static int startDelay = 60 * 0.2;
	static int waitingForClientsReady = 0;
	GameSettings * gameSettings = gameGetSettings();
	GameOptions * gameOptions = gameGetOptions();
	Player ** players = playerGetAll();
	int i;

	// Set death barrier
	gameSetDeathHeight(StartPos[2] - (SPLEEF_BOARD_LEVEL_OFFSET * SPLEEF_BOARD_LEVELS) - 10);

	// Set survivor
	// gameOptions->GameFlags.MultiplayerGameFlags.Survivor = 1;
	// gameOptions->GameFlags.MultiplayerGameFlags.RespawnTime = -1;

	// Hook set outcome net event
	// netInstallCustomMsgHandler(CUSTOM_MSG_SET_OUTCOME, &onSetRoundOutcomeRemote);
	// netInstallCustomMsgHandler(CUSTOM_MSG_DESTROY_BOX, &onDestroyBoxRemote);
	
	// Disable normal game ending
	// *(u32*)0x006219B8 = 0;	// survivor (8)
	// *(u32*)0x00621A10 = 0;  // survivor (8)

	if (startDelay) {
		--startDelay;
		return;
	}
  
	// wait for all clients to be ready
	// or for 15 seconds
	if (!gameState->AllClientsReady && waitingForClientsReady < (5 * 60)) {
		gfxScreenSpaceText(0.5, 0.5, 1, 1, 0x80FFFFFF, "Waiting For Players...", -1, 4);
		++waitingForClientsReady;
		return;
	}

	// clear spleefbox array
	memset(SpleefBox, 0, sizeof(SpleefBox));
	memset(&SpleefState, 0, sizeof(SpleefState));

	// Initialize scoreboard
	// initializeScoreboard();
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
		Player * p = players[i];
		SpleefState.PlayerPoints[i] = 0;
	}

	// patch who killed me to prevent damaging others
	// *(u32*)0x005E07C8 = 0x0C000000 | ((u32)&whoKilledMeHook >> 2);
	// *(u32*)0x005E11B0 = *(u32*)0x005E07C8;

	// initialize state
	SpleefState.GameOver = 0;
	SpleefState.RoundNumber = 0;
	memset(SpleefState.PlayerKills, 0, sizeof(SpleefState.PlayerKills));
	memset(SpleefState.PlayerBoxesDestroyed, 0, sizeof(SpleefState.PlayerBoxesDestroyed));
	resetRoundState();

	Initialized = 1;
}

void gameStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
	GameSettings * gameSettings = gameGetSettings();
	Player ** players = playerGetAll();
	Player * localPlayer = (Player*)PLAYER_STRUCT;
	GameData * gameData = gameGetData();
	int i;

	// Ensure in game
	if (!gameSettings || !isInGame())
		return;

	// Determine if host
	SpleefState.IsHost = gameIsHost(localPlayer->Guber.Id.GID.HostId);

	if (!Initialized)
		initialize(gameConfig, gameState);

	// int killsToWin = gameGetOptions()->GameFlags.MultiplayerGameFlags.KillsToWin;
	int killsToWin = 3;
	// 
	updateGameState(gameState);

#if DEBUG
	if (padGetButton(0, PAD_L3 | PAD_R3) > 0)
		SpleefState.GameOver = 1;
#endif

	if (!gameHasEnded() && !SpleefState.GameOver)
	{
		if (SpleefState.RoundResult[0])
		{
			if (SpleefState.RoundEndTicks)
			{
				// draw round message
				if (SpleefState.RoundResult[1] == localPlayer->PlayerId)
				{
					drawRoundMessage(SPLEEF_ROUND_WIN, 1.5);
				}
				else if (SpleefState.RoundResult[2] == localPlayer->PlayerId)
				{
					drawRoundMessage(SPLEEF_ROUND_SECOND, 1.5);
				}
				else if (SpleefState.RoundResult[3] == localPlayer->PlayerId)
				{
					drawRoundMessage(SPLEEF_ROUND_THIRD, 1.5);
				}
				else
				{
					drawRoundMessage(SPLEEF_ROUND_LOSS, 1.5);
				}

				// handle when round properly ends
				if (gameGetTime() > SpleefState.RoundEndTicks)
				{
					// increment round
					++SpleefState.RoundNumber;

					// reset round state
					resetRoundState();
				}
			}
			else
			{
				// Handle game outcome
				for (i = 1; i < 4; ++i)
				{
					int playerIndex = SpleefState.RoundResult[i];
					if (playerIndex >= 0) {
						SpleefState.PlayerPoints[playerIndex] += 4 - i;
						DPRINTF("player %d score %d\n", playerIndex, SpleefState.PlayerPoints[playerIndex]);
					}
				}

				// set when next round starts
				SpleefState.RoundEndTicks = gameGetTime() + (TIME_SECOND * 5);

				// update winner
				int winningScore = 0;
				getWinningPlayer(&SpleefState.WinningTeam, &winningScore);
				if (killsToWin > 0 && winningScore >= killsToWin)
				{
					SpleefState.GameOver = 1;
				}
			}
		}
		else
		{
			// iterate each player
			for (i = 0; i < GAME_MAX_PLAYERS; ++i)
			{
				SpleefState.PlayerKills[i] = gameData->PlayerStats.Kills[i];
			}

			// host specific logic
			if (SpleefState.IsHost && (gameGetTime() - SpleefState.RoundStartTicks) > (5 * TIME_SECOND))
			{
				int playersAlive = 0, playerCount = 0, lastPlayerAlive = -1;
				for (i = 0; i < GAME_MAX_PLAYERS; ++i)
				{
					if (SpleefState.RoundPlayerState[i] >= 0)
						++playerCount;
					if (SpleefState.RoundPlayerState[i] == 0)
						++playersAlive;
				}

				for (i = 0; i < GAME_MAX_PLAYERS; ++i)
				{
					Player * p = players[i];

					if (p)
					{
						// check if player is dead
						if (playerIsDead(p) || SpleefState.RoundPlayerState[i] == 1)
						{
							// player newly died
							if (SpleefState.RoundPlayerState[i] == 0)
							{
								DPRINTF("player %d died\n", i);
								SpleefState.RoundPlayerState[i] = 1;

								// set player to first/second/third if appropriate
								if (playersAlive < 4)
								{
									SpleefState.RoundResult[playersAlive] = i;
									DPRINTF("setting %d place to player %d\n", playersAlive, i);
								}
							}
						}
						else
						{
							lastPlayerAlive = i;
						}
					}
				}

				if ((playersAlive == 1 && playerCount > 1) || playersAlive == 0)
				{
					// end
					DPRINTF("end round: playersAlive:%d playerCount:%d\n", playersAlive, playerCount);
					if (lastPlayerAlive >= 0)
					{
						SpleefState.RoundResult[1] = lastPlayerAlive;
						DPRINTF("last player alive is %d\n", lastPlayerAlive);
					}
					setRoundOutcome(SpleefState.RoundResult[1], SpleefState.RoundResult[2], SpleefState.RoundResult[3]);
				}
			}
		}
	}
	else
	{
		// set winner
		gameSetWinner(SpleefState.WinningTeam, 0);

		// end game
		if (SpleefState.GameOver == 1)
		{
			gameEnd(4);
			SpleefState.GameOver = 2;
		}
	}

	// 
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
		Player * p = players[i];
		if (!p)
			continue;

		if (!playerIsDead(p))
			playerSetHealth(p, PLAYER_MAX_HEALTH);
		else
			playerSetHealth(p, 0);
	}
	return;
}

void setLobbyGameOptions(void)
{
		// Set needed options
	// static char options[] = { 
	// 	0, 0, 			// 0x06 - 0x08
	// 	0, 0, 0, 0, 	// 0x08 - 0x0C
	// 	1, 1, 1, 0,  	// 0x0C - 0x10
	// 	0, 1, 0, 0,		// 0x10 - 0x14
	// 	-1, -1, 0, 1,	// 0x14 - 0x18
	// };

	// // set game options
	// GameOptions * gameOptions = gameGetOptions();
	// GameSettings* gameSettings = gameGetSettings();
	// if (!gameOptions || !gameSettings || gameSettings->GameLoadStartTime <= 0)
	// 	return;
		
	// // apply options
	// memcpy((void*)&gameOptions->GameFlags.Raw[6], (void*)options, sizeof(options)/sizeof(char));

	// gameOptions->GameFlags.MultiplayerGameFlags.Juggernaut = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.Vehicles = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.Puma = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.Hoverbike = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.Landstalker = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.Hovership = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.SpawnWithChargeboots = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.SpecialPickups = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.UnlimitedAmmo = 1;
	// gameOptions->GameFlags.MultiplayerGameFlags.Survivor = 1;
	// gameOptions->GameFlags.MultiplayerGameFlags.RespawnTime = -1;
}

//--------------------------------------------------------------------------
void loadStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
	// TODO: setup hook for loadStart in patch

	// when setup...
	// called right before level finishes loading
	// opportunity to change the gameplay before it's loaded
	setLobbyGameOptions();
}

//--------------------------------------------------------------------------
void lobbyStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
	int activeId = uiGetActive();
	// static int initializedScoreboard = 0;

	// 
	updateGameState(gameState);

	// scoreboard
	switch (activeId)
	{
		// case 0x15C:
		// {
		// 	if (initializedScoreboard)
		// 		break;

		// 	setEndGameScoreboard();
		// 	initializedScoreboard = 1;

		// 	// patch rank computation to keep rank unchanged for base mode
		// 	POKE_U32(0x0077ACE4, 0x4600BB06);
		// 	break;
		// }
		case UI_ID_GAME_LOBBY:
		{
			//setLobbyGameOptions();
			break;
		}
	}
}