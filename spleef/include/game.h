#ifndef SPLEEF_H
#define SPLEEF_H

#include <tamtypes.h>
#include <libuya/game.h>
#include "messageid.h"

typedef struct SpleefState
{
	int RoundNumber;
	int RoundStartTicks;
	int RoundEndTicks;
	char RoundResult[4];
	char RoundPlayerState[GAME_MAX_PLAYERS];
	short PlayerKills[GAME_MAX_PLAYERS];
	int PlayerPoints[GAME_MAX_PLAYERS];
	int PlayerBoxesDestroyed[GAME_MAX_PLAYERS];
	int RoundInitialized;
	int GameOver;
	int WinningTeam;
	int IsHost;
} SpleefState_t;

extern SpleefState_t SpleefState;

#endif // SPLEEF_H
