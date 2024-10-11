#ifndef JUGGERNAUT_GAME_H
#define JUGGERNAUT_GAME_H

#include <tamtypes.h>
#include <libuya/game.h>
#include <libuya/player.h>
#include <libuya/interop.h>
#include "messageid.h"

#define TPS				(60)

enum GameNetMessage {
	CUSTOM_MSG_EXAMPLE = CUSTOM_MSG_ID_GAME_MODE_START,
};

struct PlayerGameData {
	int PlayerIndex;
};

struct JuggernautState {
	int Initialized;
	int GameOver;
	int WinningTeam;
	int IsHost;
	short int currentJuggernaut;
	short int currentJuggernautTeam;
	short int newJuggernaut;
	short int newJuggernautTeam;
	struct PlayerGameData PlayerData[GAME_MAX_PLAYERS];
};

typedef struct ExampleMessage {
	int GameTime;
	int SourcePlayerId;
	char Message[32];
} ExampleMessage_t;

extern struct JuggernautState juggy;

VariableAddress_t vaOnPlayerKill_Hook;
VariableAddress_t vaOnPlayerKill_Func;
VariableAddress_t vaWhoHitMe_Hook;
VariableAddress_t vaWhoHitMe_Func;

void sendExampleMessage(char* message);
void netHookMessages(void);

#endif // JUGGERNAUT_GAME_H
