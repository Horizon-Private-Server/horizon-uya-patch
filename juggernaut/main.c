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

struct JuggernautState juggy;

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
	juggy.IsHost = gameAmIHost();

	// initialize
	if (!juggy.Initialized)	{
		initialize(gameConfig);
		return;
	}

	//
	if (!juggy.GameOver) {
		// handle tick
		gameTick();
	} else {
		// end game
		if (juggy.GameOver == 1) {
			//gameSetWinner(State.WinningTeam, 1);
			//gameEnd(4);
			juggy.GameOver = 2;
		}
	}

	uyaPostUpdate();
	return;
}

//--------------------------------------------------------------------------
void lobbyStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig)
{
	u32 menu;
	static int initializedScoreboard = 0;

	// Lobby
	if (menu = uiGetActivePointer(UIP_STAGING), menu > 0) {
		setLobbyGameOptions();
	} else if (menu = uiGetActivePointer(UIP_END_GAME_DETAILS), menu > 0) {
		// scoreboard stuff
	}
}

//--------------------------------------------------------------------------
void loadStart(void)
{
	setLobbyGameOptions();
}

void setLobbyGameOptions(void)
{
	int i;

	// set game options
	GameOptions * gameOptions = gameGetOptions();
	GameSettings* gameSettings = gameGetSettings();
	if (!gameOptions || !gameSettings || gameSettings->GameLoadStartTime <= 0)
		return;
		
	// apply options
	gameSettings->GameType = GAMERULE_DM;
	// gameOptions->GameFlags.MultiplayerGameFlags.Teams = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.Nodes = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_Bots = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_BaseAmmoHealth = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_SmallTurrets = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseTeleporter = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.ChaosModeAttritionMode = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.PlayerNames = 1;
}
