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

struct CGMState State;

void initialize(PatchGameConfig_t*gameConfig);
void gameTick(void);

int isCustomMap = 0;

//--------------------------------------------------------------------------
void gameStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t *gameState)
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
	if (!State.Initialized) {
		isCustomMap = gameConfig->isCustomMap;
		initialize(gameConfig);
		return;
	}

	//
	if (!State.GameOver) {
		// handle tick
		gameTick();
	} else {
		// end game
		if (State.GameOver == 1) {
			State.GameOver = 2;
		}
	}

	uyaPostUpdate();
	return;
}

//--------------------------------------------------------------------------
void lobbyStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t *gameState)
{
	u32 menu;
	static int initializedScoreboard = 0;

	// Lobby
	if (menu = uiGetActiveMenu(UI_MENU_STAGING, 0), menu > 0) {
		setLobbyGameOptions(gameState);
	} else if (menu = uiGetActiveMenu(UI_MENU_END_GAME_DETAILS, 0), menu > 0) {
		// scoreboard stuff
	}
}

//--------------------------------------------------------------------------
void loadStart(struct GameModule * module, PatchStateContainer_t * gameState)
{
	setLobbyGameOptions();
}

void setLobbyGameOptions(PatchStateContainer_t *gameState)
{
	int i;

	// set game options
	GameOptions * gameOptions = gameGetOptions();
	GameSettings* gameSettings = gameGetSettings();
	if (!gameOptions || !gameSettings || gameSettings->GameLoadStartTime <= 0)
		return;
	
	// apply options
	gameSettings->GameType = GAMETYPE_CTF;
	gameOptions->GameFlags.MultiplayerGameFlags.Nodes = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_Bots = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_BaseAmmoHealth = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_SmallTurrets = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseTeleporter = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.ChaosModeAttritionMode = 1;
	gameOptions->GameFlags.MultiplayerGameFlags.PlayerNames = 1;
	gameOptions->GameFlags.MultiplayerGameFlags.Chargeboots = 1;
}
