#ifndef EXAMPLE_GAME_H
#define EXAMPLE_GAME_H

#include <tamtypes.h>
#include <libuya/game.h>
#include <libuya/player.h>
#include "messageid.h"

#define TPS																		(60)

enum GameNetMessage
{
	CUSTOM_MSG_EXAMPLE = CUSTOM_MSG_ID_GAME_MODE_START,
};

struct PlayerGameData
{
	int PlayerIndex;
};

struct ExampleGameState
{
	int Initialized;
	int GameOver;
	int WinningTeam;
	int IsHost;
	struct PlayerGameData PlayerData[GAME_MAX_PLAYERS];
};

typedef struct ExampleMessage
{
	int GameTime;
	int SourcePlayerId;
	char Message[32];
} ExampleMessage_t;

extern struct ExampleGameState State;

void sendExampleMessage(char* message);
void netHookMessages(void);

#endif // EXAMPLE_GAME_H
