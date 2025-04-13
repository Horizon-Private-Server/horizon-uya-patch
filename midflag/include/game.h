#ifndef MIDFLAG_H
#define MIDFLAG_H

#include <tamtypes.h>
#include <libuya/game.h>
#include <libuya/player.h>
#include "messageid.h"

#define TPS (60)

enum GameNetMessage {
	CUSTOM_MSG_MIDFLAG = CUSTOM_MSG_ID_GAME_MODE_START,
};

struct CGMState {
	int Initialized;
	int GameOver;
	int WinningTeam;
	int IsHost;
};

typedef struct ExampleMessage {
	int GameTime;
	int SourcePlayerId;
	char Message[32];
} ExampleMessage_t;

extern struct CGMState State;

#endif // MIDFLAG_H
