/***************************************************
 * FILENAME :		main.c
 * DESCRIPTION :
 * 		Infected entrypoint and logic.
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * AUTHOR :			Troy "Metroynome" Pruitt
 */

#include <tamtypes.h>

#include <libuya/stdio.h>
#include <libuya/time.h>
#include "module.h"
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/graphics.h>
#include <libuya/player.h>
#include <libuya/ui.h>
#include <libuya/string.h>
#include <libuya/utils.h>
#include <libuya/interop.h>
#include <libuya/moby.h>


#define ARRAY_SIZE(x)			(sizeof(x)/sizeof(x[0]))
#define CHEATCMP(str)			strncmp(str, CS, sizeof(str)-1)

struct JuggernautGameData {
	u32 Version;
};

int Initialized = 0;
int WhoIsJuggernaut = -1;
const char * JuggernautPopup = "%s is the Juggernaut!\0";
int ShieldTex = 0;
char CS[12];

VariableAddress_t vaOnPlayerKill_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004fa1ec,
	.Hoven = 0x004fc304,
	.OutpostX12 = 0x004f1bdc,
	.KorgonOutpost = 0x004ef374,
	.Metropolis = 0x004ee6c4,
	.BlackwaterCity = 0x004ebf5c,
	.CommandCenter = 0x004ebf24,
	.BlackwaterDocks = 0x004ee7a4,
	.AquatosSewers = 0x004edaa4,
	.MarcadiaPalace = 0x004ed424,
#else
	.Lobby = 0,
	.Bakisi = 0x004f7a6c,
	.Hoven = 0x004f9ac4,
	.OutpostX12 = 0x004ef3dc,
	.KorgonOutpost = 0x004ecbf4,
	.Metropolis = 0x004ebf44,
	.BlackwaterCity = 0x004e975c,
	.CommandCenter = 0x004e98e4,
	.BlackwaterDocks = 0x004ec124,
	.AquatosSewers = 0x004eb464,
	.MarcadiaPalace = 0x004eada4,
#endif
};

VariableAddress_t vaOnPlayerKill_Func = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00544488,
	.Hoven = 0x00546650,
	.OutpostX12 = 0x0053bf28,
	.KorgonOutpost = 0x00539610,
	.Metropolis = 0x00538a10,
	.BlackwaterCity = 0x005361f8,
	.CommandCenter = 0x00535a50,
	.BlackwaterDocks = 0x005382d0,
	.AquatosSewers = 0x005375d0,
	.MarcadiaPalace = 0x00536f50,
#else
	.Lobby = 0,
	.Bakisi = 0x00541b78,
	.Hoven = 0x00543c80,
	.OutpostX12 = 0x00539598,
	.KorgonOutpost = 0x00536d00,
	.Metropolis = 0x00536100,
	.BlackwaterCity = 0x00533868,
	.CommandCenter = 0x00533298,
	.BlackwaterDocks = 0x00535ad8,
	.AquatosSewers = 0x00534e18,
	.MarcadiaPalace = 0x00534758,
#endif
};

VariableAddress_t vaWhoHitMe_Hook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0052916c,
    .Hoven = 0x0052b284,
    .OutpostX12 = 0x00520b5c,
    .KorgonOutpost = 0x0051e2f4,
    .Metropolis = 0x0051d644,
    .BlackwaterCity = 0x0051aedc,
    .CommandCenter = 0x0051ac9c,
    .BlackwaterDocks = 0x0051d51c,
    .AquatosSewers = 0x0051c81c,
    .MarcadiaPalace = 0x0051c19c,
#else
    .Lobby = 0,
    .Bakisi = 0x005268ec,
    .Hoven = 0x00528944,
    .OutpostX12 = 0x0051e25c,
    .KorgonOutpost = 0x0051ba74,
    .Metropolis = 0x0051adc4,
    .BlackwaterCity = 0x005185dc,
    .CommandCenter = 0x0051855c,
    .BlackwaterDocks = 0x0051ad9c,
    .AquatosSewers = 0x0051a0dc,
    .MarcadiaPalace = 0x00519a1c,
#endif
};
VariableAddress_t vaWhoHitMe_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004f8c30,
    .Hoven = 0x004fad48,
    .OutpostX12 = 0x004f0620,
    .KorgonOutpost = 0x004eddb8,
    .Metropolis = 0x004ed108,
    .BlackwaterCity = 0x004ea9a0,
    .CommandCenter = 0x004ea968,
    .BlackwaterDocks = 0x004ed1e8,
    .AquatosSewers = 0x004ec4e8,
    .MarcadiaPalace = 0x004ebe68,
#else
    .Lobby = 0,
    .Bakisi = 0x004f64b0,
    .Hoven = 0x004f8508,
    .OutpostX12 = 0x004ede20,
    .KorgonOutpost = 0x004eb638,
    .Metropolis = 0x004ea988,
    .BlackwaterCity = 0x004e81a0,
    .CommandCenter = 0x004e8328,
    .BlackwaterDocks = 0x004eab68,
    .AquatosSewers = 0x004e9ea8,
    .MarcadiaPalace = 0x004e97e8,
#endif
};

/*
 * NAME :	    patchWhoHitMe
 * DESCRIPTION :
 *             Patches the "Got Hit" function so those who are not juggernaut
 *              can't hurt each other.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int patchWhoHitMe(Player* player, int a1)
{
	int hurtPlayer = a1;
	if (checkJuggernaut(player))
		hurtPlayer = 0;

	return ((int (*)(Player*, int))GetAddress(&vaWhoHitMe_Func))(player, hurtPlayer);
}

/*
 * NAME :		giveJuggernaut
 * DESCRIPTION :
 * 			gives the player juggernaut
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int checkJuggernaut(Player * player)
{
    return WhoIsJuggernaut == player->mpIndex;
}

/*
 * NAME :		giveJuggernaut
 * DESCRIPTION :
 * 			gives the player juggernaut
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void giveJuggernaut(Player * player)
{
    WhoIsJuggernaut = player->mpIndex;
	GameSettings * gameSettings = gameGetSettings();
	if (!gameSettings)
		return;

	char buf[64];
	sprintf(buf, JuggernautPopup, gameSettings->PlayerNames[player->mpIndex]);
	uiShowPopup(player, buf, 3);
}

void updateShield(Player* player)
{
    // Modify shield vars
	ShieldVars * shield = mobyGetShieldVars();
	shield->mainColor = player->pMoby->primaryColor;
	shield->lightningColor = 0x20000000 |player->pMoby->primaryColor;
	// shield->texture = 0x45;
	// shield->outerCircleGlowRadius = 2.0;
	// shield->animScale = 0;
}

/*
 * NAME :		processPlayer
 * DESCRIPTION :
 * 			Process player.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void processPlayer(Player * player)
{
	if (!player)
		return;

	GameData * gameData = gameGetData();
	DoCheats(player);
	if (checkJuggernaut(player->mpIndex)) {
        // Check if player has shield, if not, enable.
		if (!playerHasShield(player) && !playerIsDead(player) && !player->pSheepMoby) {
			// if Local, run shield trigger, if not, run playerGiveShield function
			if (player->isLocal)
				player->shieldTrigger = 1;
			else
				playerGiveShield(player);
		}
        // Updates shield color to match team color.
        updateShield(player);

        // Upgrade Weapons to v2
        int j;
        for(j = 1; j < 10; ++j) {
			playerGiveWeaponUpgrade(player, j);
        }
	}
}

/*
 * NAME :		getRandomPlayer
 * DESCRIPTION :
 * 			Returns a random survivor.
 * NOTES :
 * ARGS : 
 * 		seed :		Used to determine the random survivor.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
Player * getRandomPlayer(u32 seed)
{
	Player ** player = playerGetAll();
	int value = (seed % GAME_MAX_PLAYERS) + 1;
	int i = 0;
	int counter = 0;

	while (counter < value) {
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			if (player[i] && !checkJuggernaut(player[i])) {
				++counter;
				if (value == counter)
					return player[i];
			}
		}
		if (counter == 0)
			return NULL;
	}
	return NULL;
}

/*
 * NAME :		onPlayerKill
 * DESCRIPTION :
 * 			Triggers whenever a player is killed.
 * 			Handles who turns juggernaut next.
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onPlayerKill(char * fragMsg)
{
	// call base function
	((void (*)(char*))GetAddress(&vaOnPlayerKill_Func))(fragMsg);

	char killer = fragMsg[0];
	char killed = fragMsg[2];

	if (killer >= 0 && checkJuggernaut(killer)
        && killed >= 0
        && !checkJuggernaut(killed)
    ) {
		WhoIsJuggernaut = killer;
    }
}

/*
 * NAME :		initialize
 * DESCRIPTION :
 * 			Initializes the gamemode.
 * NOTES :
 * 			This is called only once at the start.
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void initialize(void)
{
	setLobbyGameOptions();

    // patch WhoHitMe so non-juggy players can't hurt each other.
    HOOK_JAL(GetAddress(&vaWhoHitMe_Hook), &patchWhoHitMe);

	// hook into player kill event
	HOOK_JAL(GetAddress(&vaOnPlayerKill_Hook), &onPlayerKill);

	printf("Juggernaut Initilized!\n");
	Initialized = 1;
}


/*
 * NAME :		gameStart
 * DESCRIPTION :
 * 			Infected game logic entrypoint.
 * NOTES :
 * 			This is called only when in game.
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void gameStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
	int i = 0;
	int playerCount = 0;
	GameSettings * gameSettings = gameGetSettings();
	GameOptions * gameOptions = gameGetOptions();
	GameData * gameData = gameGetData();
	Player ** players = playerGetAll();

	// Ensure in game
	if (!gameSettings || !isInGame())
		return;

	if (!Initialized)
		initialize();

	if (!gameHasEnded()) {
		if (WhoIsJuggernaut >= 0) {
		// Iterate through players
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			if (!players[i])
				continue;
			
			// Process
			processPlayer(players[i]);

			// Count
			// ++playerCount;
			// if (isInfected(players[i]->mpIndex))
			// 	++infectedCount;
			// else
			// 	WinningTeam = players[i]->mpTeam;
		}
		} else {
			// Juggernaut first player after 3 seconds
			if ((gameGetTime() - gameSettings->GameStartTime) > (3 * TIME_SECOND)) {
				Player * p = getRandomPlayer(gameSettings->GameStartTime);
				if (p) {
					giveJuggernaut(p);
					WhoIsJuggernaut = p->mpIndex;
				}
			}
		}
	}
	return;
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
	gameOptions->GameFlags.MultiplayerGameFlags.Teams = 1;
	gameOptions->GameFlags.MultiplayerGameFlags.Nodes = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_Bots = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_BaseAmmoHealth = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_SmallTurrets = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseTeleporter = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.ChaosModeAttritionMode = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.PlayerNames = 1;

	// set everyone to blue
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		if (gameSettings->PlayerClanTags[i] >= 0)
			gameSettings->PlayerTeams[i] = 0;
	}
}

/*
 * NAME :		lobbyStart
 * DESCRIPTION :
 * 			Infected lobby logic entrypoint.
 * NOTES :
 * 			This is called only when in lobby.
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void lobbyStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
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

/*
 * NAME :		loadStart
 * DESCRIPTION :
 * 			Load logic entrypoint.
 * NOTES :
 * 			This is called only when the game has finished reading the level from the disc and before it has started processing the data.
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void loadStart(void)
{
  setLobbyGameOptions();
}


void SwapShield(void)
{
	if (ShieldTex == 0)
		ShieldTex = 0x5c;
	else if (ShieldTex > 0x62)
		ShieldTex = 0;
	else
		++ShieldTex;

	Clear();
}
void Clear(void)
{
	int i = 0;
	for (i = 0; i < ARRAY_SIZE(CS); i++) {
		CS[i] = ' ';
	}
}
// Logic to run selected codes if combo is correct.
void AddToCS(char c, Player * currentPlayer)
{
	int a = 0;
	for (a = ARRAY_SIZE(CS) - 2; a >= 0; a--) {
		CS[a + 1] = CS[a];
	}
	CS[0] = c;

	// "UUDDLRLRCX"	-	UP UP DOWN DOWN LEFT RIGHT LEFT RIGHT CIRCLE CROSS
	if (!CHEATCMP("XCRLRLDDUU")) {
		SwapShield();
	}
}
void DoCheats(Player * currentPlayer)
{
	if (padGetButtonDown(0, PAD_TRIANGLE))
		AddToCS('T', currentPlayer);

	if (padGetButtonDown(0, PAD_CIRCLE))
		AddToCS('C', currentPlayer);

	if (padGetButtonDown(0, PAD_CROSS))
		AddToCS('X', currentPlayer);

	if (padGetButtonDown(0, PAD_SQUARE))
		AddToCS('S', currentPlayer);

	if (padGetButtonDown(0, PAD_UP))
		AddToCS('U', currentPlayer);

	if (padGetButtonDown(0, PAD_DOWN) > 0)
		AddToCS('D', currentPlayer);

	if (padGetButtonDown(0, PAD_LEFT) > 0)
		AddToCS('L', currentPlayer);

	if (padGetButtonDown(0, PAD_RIGHT) > 0)
		AddToCS('R', currentPlayer);

	if (padGetButtonDown(0, PAD_L1) > 0)
		AddToCS('1', currentPlayer);

	if (padGetButtonDown(0, PAD_L2) > 0)
		AddToCS('2', currentPlayer);

	if (padGetButtonDown(0, PAD_R1) > 0)
		AddToCS('3', currentPlayer);

	if (padGetButtonDown(0, PAD_R2) > 0)
		AddToCS('4', currentPlayer);

}
