/***************************************************
 * FILENAME :        main.c
 * DESCRIPTION :
 *      KOTH custom game mode entrypoints.
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
#include "include/koth.h"

struct KothState {
    int Initialized;
    int GameOver;
    int IsHost;
};

static struct KothState State;
static int isCustomMap = 0;

static void setLobbyGameOptions(PatchStateContainer_t *gameState);

//--------------------------------------------------------------------------
void gameStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t *gameState)
{
    GameSettings * gameSettings = gameGetSettings();

    uyaPreUpdate();

    // Ensure in game
    if (!gameSettings || !isInGame())
        return;

    // Determine if host
    State.IsHost = gameAmIHost();

    // initialize
    if (!State.Initialized) {
        State.Initialized = 1;
        isCustomMap = gameConfig ? gameConfig->isCustomMap : 0;
        kothSetConfig(gameConfig);
        kothReset();
        return;
    }

    // tick
    if (!State.GameOver) {
        kothTick();
    }

    uyaPostUpdate();
    return;
}

//--------------------------------------------------------------------------
void lobbyStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t *gameState)
{
    u32 menu;

    // Lobby
    if (menu = uiGetActiveMenu(UI_MENU_STAGING, 0), menu > 0) {
        setLobbyGameOptions(gameState);
    } else if (menu = uiGetActiveMenu(UI_MENU_END_GAME_DETAILS, 0), menu > 0) {
        // scoreboard spot if needed
    }
}

//--------------------------------------------------------------------------
void loadStart(struct GameModule * module, PatchStateContainer_t * gameState)
{
    setLobbyGameOptions(gameState);
}

static void setLobbyGameOptions(PatchStateContainer_t *gameState)
{
    // set game options
    GameOptions * gameOptions = gameGetOptions();
    GameSettings* gameSettings = gameGetSettings();
    if (!gameOptions || !gameSettings || gameSettings->GameLoadStartTime <= 0)
        return;

    // Keep DM base rules but ensure frag limit is off; KOTH handles scoring.
    gameSettings->GameType = GAMETYPE_DM;
    gameOptions->GameFlags.MultiplayerGameFlags.FragLimit = 0;
    gameOptions->GameFlags.MultiplayerGameFlags.Chargeboots = 1;
    gameOptions->GameFlags.MultiplayerGameFlags.PlayerNames = 1;
}
