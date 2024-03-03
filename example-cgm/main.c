/***************************************************
 * FILENAME :		main.c
 * DESCRIPTION :
 * 		PAYLOAD.
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

struct ExampleGameState State;

void initialize(PatchGameConfig_t* gameConfig);
void gameTick(void);

//--------------------------------------------------------------------------
void gameStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig)
{
	GameSettings * gameSettings = gameGetSettings();
	GameOptions * gameOptions = gameGetOptions();
	Player ** players = playerGetAll();
	int gameTime = gameGetTime();

	//
	uyaPreUpdate();

	// Ensure in game
	if (!gameSettings || !isInGame())
		return;

	// Determine if host
	State.IsHost = gameAmIHost();

	// initialize
	if (!State.Initialized)
	{
		initialize(gameConfig);
		return;
	}

	//
	if (!State.GameOver)
	{
		// handle tick
		gameTick();
	}
	else
	{
		// end game
		if (State.GameOver == 1)
		{
			//gameSetWinner(State.WinningTeam, 1);
			//gameEnd(4);
			State.GameOver = 2;
		}
	}

	uyaPostUpdate();
	return;
}

//--------------------------------------------------------------------------
void lobbyStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig)
{
	// called when in staging or after game ends	
}

//--------------------------------------------------------------------------
void loadStart(void)
{
	// TODO: setup hook for loadStart in patch

	// when setup...
	// called right before level finishes loading
	// opportunity to change the gameplay before it's loaded
}
