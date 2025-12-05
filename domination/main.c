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
#include <libuya/gameplay.h>
#include <libuya/interop.h>
#include <libuya/utils.h>
#include <libuya/transition.h>
#include "module.h"
#include "config.h"
#include "messageid.h"
#include "include/game.h"

struct CGMState State;

void initialize(PatchGameConfig_t*gameConfig);
void gameTick(void);
void onGameplayLoad_adjustSiegePadTies(GameplayHeaderDef_t * gameplay, float targetZ);


int isCustomMap = 0;
int Gameplay_Hook = 0;
int Gameplay_Func = 0;

const char *tip = "DOMINATE DOMINATE DOMINATE DOMINATE DOMINATE";

VariableAddress_t vaGameplayHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0046db20,
    .Hoven = 0x0046f6d0,
    .OutpostX12 = 0x004664d0,
    .KorgonOutpost = 0x00464060,
    .Metropolis = 0x004633a0,
    .BlackwaterCity = 0x00460bd0,
    .CommandCenter = 0x004614c8,
    .BlackwaterDocks = 0x00463d48,
    .AquatosSewers = 0x00463048,
    .MarcadiaPalace = 0x004629c8,
#else
    .Lobby = 0,
    .Bakisi = 0x0046bfb8,
    .Hoven = 0x0046daa8,
    .OutpostX12 = 0x004648e8,
    .KorgonOutpost = 0x004624f8,
    .Metropolis = 0x00461838,
    .BlackwaterCity = 0x0045efe8,
    .CommandCenter = 0x0045faa0,
    .BlackwaterDocks = 0x004622e0,
    .AquatosSewers = 0x00461620,
    .MarcadiaPalace = 0x00460f60,
#endif
};

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
void onGameplayLoad(void* a0, long a1)
{
	GameplayHeaderDef_t * gameplay;

	// pointer to gameplay data is stored in $s6
	asm volatile (
		"move %0, $s6"
		: : "r" (gameplay)
	);
	
	// set seige pad to y 10
	onGameplayLoad_adjustSiegePadTies(gameplay, 10.0f);
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
	// gameSettings->GameType = GAMETYPE_SIEGE;
	// gameOptions->GameFlags.MultiplayerGameFlags.Nodes = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_Bots = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_BaseAmmoHealth = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_SmallTurrets = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.BaseTeleporter = 0;
	// gameOptions->GameFlags.MultiplayerGameFlags.ChaosModeAttritionMode = 1;
	// gameOptions->GameFlags.MultiplayerGameFlags.PlayerNames = 1;
	// gameOptions->GameFlags.MultiplayerGameFlags.Chargeboots = 1;
}

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

	setLobbyGameOptions(gameState);

	// only handle when loading level
	GameSettings* gs = gameGetSettings();
	if (!gs || gs->GameStartTime >= 0)
		return;

	// Reset Gameplay hook and function each time the we start a new game.
	Gameplay_Hook = 0;
	Gameplay_Func = 0;

	Gameplay_Hook = GetAddressImmediate(&vaGameplayHook);
	Gameplay_Func = JAL2ADDR(*(u32*)Gameplay_Hook);
	
	if (Gameplay_Hook != 0)
		HOOK_JAL(Gameplay_Hook, &onGameplayLoad);

	// set tip
	memcpy((char*)TRANSITION_MULTIPLAYER->m_sTipToDisplay, tip, 256);

}
