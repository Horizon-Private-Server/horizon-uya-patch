/***************************************************
 * FILENAME :		game.c
 * DESCRIPTION :
 * 		PAYLOAD.
 * AUTHOR :			Troy "Metroynome" Pruitt
 */

#include <tamtypes.h>
#include <string.h>
#include <libuya/stdio.h>
#include <libuya/time.h>
#include <libuya/player.h>
#include <libuya/utils.h>
#include <libuya/team.h>

#include "config.h"
#include "include/game.h"

const char * JuggernautPopup = "%s is the Juggernaut!\0";
int ShieldTex = 0;
char CS[12];

//--------------------------------------------------------------------------
int checkJuggernaut(int playerIdx)
{
    return juggy.newJuggernaut == playerIdx;
}

void giveJuggernaut(Player * player)
{
	GameSettings * gameSettings = gameGetSettings();
	if (!gameSettings)
		return;

    juggy.newJuggernaut = player->mpIndex;
	juggy.newJuggernautTeam = player->mpTeam;
	char buf[64];
	sprintf(buf, JuggernautPopup, gameSettings->PlayerNames[juggy.newJuggernaut]);
	uiShowPopup(player, buf, 3);
}

void juggernautLogic(void)
{
	Player * player = playerGetFromSlot(juggy.currentJuggernaut);
	if (!player)
		return;

	int j;
	for(j = 1; j < 10; ++j) {
		playerGiveWeaponUpgrade(player, j);
	}
}

//--------------------------------------------------------------------------
void processPlayer(Player * player)
{
	GameData * gameData = gameGetData();
	GameSettings * gameSettings = gameGetSettings();
	// DoCheats(player);
	if (checkJuggernaut(player->mpIndex)) {
		DPRINTF("processPlayer: checked juggy passed for player %d\n", player->mpIndex)
	
		// Check if player has shield, if not, enable.
		if (!playerHasShield(player) && !playerIsDead(player) && !player->pSheepMoby && !player->vehicle) {
			// if Local, run shield trigger, if not, run playerGiveShield function
			if (player->isLocal)
				player->shieldTrigger = 1;
			else
				playerGiveShield(player);
		}

		// if current juggy matches new juggy, don't run juggernaut logic again.
		if (juggy.currentJuggernaut == juggy.newJuggernaut) {
			// if current juggernaut and death was suicide.
			if (checkJuggernaut(player->mpIndex) && playerIsDead(player) && player->lastDeathWasSuicide){
				Player * p = getRandomPlayer(gameSettings->GameStartTime);
				if (p) {
					giveJuggernaut(p);
					DPRINTF("Player %d suicided! Juggernaut given to player %d\n", player->mpIndex, p->mpIndex);
				}
			}
		}
		DPRINTF("Running juggy logic!\n");
	}

	juggernautLogic();

	// Proccess Player: Is and isn't juggy
	// Updates shield color to match team color.
	ShieldVars * shield = mobyGetShieldVars();
	u32 color = TEAM_COLORS[juggy.currentJuggernautTeam];
	shield->mainColor = 0x20000000 | color;
	shield->lightningColor = 0x20000000 | color;

	// set new juggernaut as current
	if (juggy.newJuggernaut > -1) {
		juggy.currentJuggernaut = juggy.newJuggernaut;
		juggy.currentJuggernautTeam = juggy.newJuggernautTeam;
	}
}

//--------------------------------------------------------------------------
Player * getRandomPlayer(u32 seed)
{
	Player ** player = playerGetAll();
	int value = (seed % GAME_MAX_PLAYERS) + 1;
	int i = 0;
	int counter = 0;

	while (counter < value) {
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			// if player isn 't already juggy, and isn't dead.
			if (player[i] && !checkJuggernaut(player[i]->mpIndex) && !playerIsDead(player[i])) {
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

//--------------------------------------------------------------------------
void gameTick(void)
{
	int i;
	GameSettings * gameSettings = gameGetSettings();

	// run custom game logic here
	// DPRINTF("custom game mode!! %d\n", gameGetTime());
	// if no juggernaut, set random player.
	if (juggy.newJuggernaut == -1) {
		// Juggernaut first player after 3 seconds
		if ((gameGetTime() - gameSettings->GameStartTime) > (3 * TIME_SECOND)) {
			Player * p = getRandomPlayer(gameSettings->GameStartTime);
			if (p) {
				giveJuggernaut(p);
				DPRINTF("Gave Juggernaut to player %d\n", p->mpIndex);
			}
		}
	}

	 // If there is a jugggernaut, proccess players
	else if (juggy.newJuggernaut > -1) {
		Player ** players = playerGetAll();
		// Iterate through players
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			if (players[i])
				processPlayer(players[i]);
		}
	}
}

//--------------------------------------------------------------------------
int patchWhoHitMe(Player* player, int a1)
{
	int hurtPlayer = a1;
	if (juggy.currentJuggernaut != player->mpIndex)
		hurtPlayer = 0;

	return ((int (*)(Player*, int))GetAddress(&vaWhoHitMe_Func))(player, hurtPlayer);
}

//--------------------------------------------------------------------------
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
		juggy.newJuggernaut = killer;
    }
}

//--------------------------------------------------------------------------
void initialize(PatchGameConfig_t* gameConfig)
{
	// hook messages
	// netHookMessages();

	// give a 1 second delay before finalizing the initialization.
	// this helps prevent the slow loaders from desyncing
	static int startDelay = 60 * 1;
	if (startDelay > 0) {
		--startDelay;
		return;
	}
	// Set Lobby Options (UYA Needs to do this in lobby and before game)
	// setLobbyGameOptions();
	
	// reset juggy struct
	memset(&juggy, 0, sizeof(juggy));

	// Set current and new juggernaut to -1
	juggy.newJuggernaut = -1;
	juggy.currentJuggernaut = -1;
	juggy.newJuggernautTeam = -1;
	juggy.currentJuggernautTeam = -1;

    // patch WhoHitMe so non-juggy players can't hurt each other.
    HOOK_JAL(GetAddress(&vaWhoHitMe_Hook), &patchWhoHitMe);

	// hook into player kill event
	HOOK_JAL(GetAddress(&vaOnPlayerKill_Hook), &onPlayerKill);
	
	// finalize initialization
	juggy.Initialized = 1;
	DPRINTF("Juggernaut Initilized!\n");
}
