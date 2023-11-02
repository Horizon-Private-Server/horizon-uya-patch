/***************************************************
 * FILENAME :		main.c
 * 
 * DESCRIPTION :
 * 		Infected entrypoint and logic.
 * 
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
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


#define INFECTED_TEAM			(TEAM_GREEN)
#define PLAYER_SPEED			((float)4.0)

#if UYA_PAL
#define PLAYER_SPEED_ADDR		(0x00246010)
#else
#define PLAYER_SPEED_ADDR		(0x00246190)
#endif

struct InfectedGameData
{
	u32 Version;
	int Infections[GAME_MAX_PLAYERS];
	char IsInfected[GAME_MAX_PLAYERS];
	char IsFirstInfected[GAME_MAX_PLAYERS];
};


int InfectedMask = 0;
int WinningTeam = 0;
int Initialized = 0;
char FirstInfected[GAME_MAX_PLAYERS];
int Infections[GAME_MAX_PLAYERS];
char InfectedPopupBuffer[64];
const char * InfectedPopupFormat = "%s has been infected!";

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

VariableAddress_t vaShieldTriggerVars = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x002478b0,
	.Hoven = 0x00247a78,
	.OutpostX12 = 0x00247a78,
	.KorgonOutpost = 0x002477f0,
	.Metropolis = 0x002478b8,
	.BlackwaterCity = 0x002476f0,
	.CommandCenter = 0x00247588,
	.BlackwaterDocks = 0x00247648,
	.AquatosSewers = 0x00247658,
	.MarcadiaPalace = 0x00247688,
#else
	.Lobby = 0,
	.Bakisi = 0x00247a30,
	.Hoven = 0x00247bf8,
	.OutpostX12 = 0x00247af8,
	.KorgonOutpost = 0x00247970,
	.Metropolis = 0x00247a38,
	.BlackwaterCity = 0x00247870,
	.CommandCenter = 0x00247708,
	.BlackwaterDocks = 0x002477c8,
	.AquatosSewers = 0x002477d8,
	.MarcadiaPalace = 0x00247808,
#endif
};

void disableWeaponPacks(void)
{
	VariableAddress_t vaWeaponPackSpawnFunc = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x004FA350,
		.Hoven = 0x004FC468,
		.OutpostX12 = 0x004F1D40,
		.KorgonOutpost = 0x004EF4D8,
		.Metropolis = 0x004EE828,
		.BlackwaterCity = 0x004EC0C0,
		.CommandCenter = 0x004EC088,
		.BlackwaterDocks = 0x004EE908,
		.AquatosSewers = 0x004EDC08,
		.MarcadiaPalace = 0x004ED588,
#else
		.Lobby = 0,
		.Bakisi = 0x004F7BD0,
		.Hoven = 0x004F9C28,
		.OutpostX12 = 0x004EF540,
		.KorgonOutpost = 0x004ECD58,
		.Metropolis = 0x004EC0A8,
		.BlackwaterCity = 0x004E98C0,
		.CommandCenter = 0x004E9A48,
		.BlackwaterDocks = 0x004EC288,
		.AquatosSewers = 0x004EB5C8,
		.MarcadiaPalace = 0x004EAF08,
#endif
	};

	u32 weaponPackSpawnFunc = GetAddress(&vaWeaponPackSpawnFunc);
	if (weaponPackSpawnFunc) {
		*(u32*)weaponPackSpawnFunc = 0;
		*(u32*)(weaponPackSpawnFunc - 0x7BF4) = 0;
	}
}

int disableHealthboxes(void)
{
    int count = 0;
    Moby* moby = mobyListGetStart();

    // Iterate through mobys and disable healthboxes
    while ((moby = mobyFindNextByOClass(moby, MOBY_ID_HEALTH_BOX_MP)))
    {
        // move to 0,0,0
        memset(moby->Position, 0, sizeof(moby->Position));

        // move orb to 0,0,0
        if (moby->PVar)
        {
            void * subPtr = (void*)(*(u32*)(moby->PVar));
            if (subPtr)
            {
                Moby * orb = (Moby*)(*(u32*)(subPtr + 0x48));
                if (orb)
                {
                    memset(orb->Position, 0, sizeof(orb->Position));
                    ++count;
                }
            }
        }

        ++moby; // increment moby
    }
    return count;
}

/*
 * NAME :		isInfected
 * 
 * DESCRIPTION :
 * 			Returns true if the given player is infected.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 		playerId:	Player index
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
inline int isInfected(int playerId)
{
	return InfectedMask & (1 << playerId);
}

/*
 * NAME :		infect
 * 
 * DESCRIPTION :
 * 			Infects the given player.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 		playerId:	Player index
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void infect(int playerId)
{
	InfectedMask |= (1 << playerId);

	GameSettings * gameSettings = gameGetSettings();
	if (!gameSettings)
		return;

	InfectedPopupBuffer[0] = 0;
	sprintf(InfectedPopupBuffer, InfectedPopupFormat, gameSettings->PlayerNames[playerId]);
	InfectedPopupBuffer[63] = 0;

	uiShowPopup(0, InfectedPopupBuffer, -1);
}

/*
 * NAME :		processPlayer
 * 
 * DESCRIPTION :
 * 			Process player.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 		player:		Player to process.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void processPlayer(Player * player)
{
	if (!player)
		return;

	int teamId = player->mpTeam;
	GameData * gameData = gameGetData();
	if (isInfected(player->mpIndex)) {
		// If not on the right team then set it
		if (teamId != INFECTED_TEAM)
			playerSetTeam(player, INFECTED_TEAM);

		*(float*)PLAYER_SPEED_ADDR = PLAYER_SPEED;
		
		// Force wrench
		if (player->WeaponHeldId != WEAPON_ID_WRENCH && player->WeaponHeldId != WEAPON_ID_SWINGSHOT)
			playerEquipWeapon(player, WEAPON_ID_WRENCH);
	}
	// If the player is already on the infected team
	// or if they've died
	// then infect them
	else if (teamId == INFECTED_TEAM || gameData->PlayerStats[player->mpIndex].Deaths > 0)
	{
		infect(player->mpIndex);
	}
	// Process survivor logic
	else
	{

	}
}

/*
 * NAME :		getRandomSurvivor
 * 
 * DESCRIPTION :
 * 			Returns a random survivor.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 		seed :		Used to determine the random survivor.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
Player * getRandomSurvivor(u32 seed)
{
	Player ** playerObjects = playerGetAll();

	int value = (seed % GAME_MAX_PLAYERS) + 1;
	int i = 0;
	int counter = 0;

	while (counter < value) {
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			if (playerObjects[i] && !isInfected(playerObjects[i]->mpIndex)) {
				++counter;
				if (value == counter)
					return playerObjects[i];
			}
		}
		// This means that there are no survivors left
		if (counter == 0)
			return NULL;
	}
	return NULL;
}

/*
 * NAME :		onPlayerKill
 * 
 * DESCRIPTION :
 * 			Triggers whenever a player is killed.
 * 			Handles detection of when a player infects another.
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void onPlayerKill(char * fragMsg)
{

	// call base function
	((void (*)(char*))GetAddress(&vaOnPlayerKill_Func))(fragMsg);

	char weaponId = fragMsg[3];
	char killedPlayerId = fragMsg[2];
	char sourcePlayerId = fragMsg[0];

	if (sourcePlayerId >= 0 && isInfected(sourcePlayerId) && killedPlayerId >= 0 && !isInfected(killedPlayerId)) {
		Infections[sourcePlayerId]++;
	}
}

/*
 * NAME :		updateGameState
 * 
 * DESCRIPTION :
 * 			Updates the gamemode state for the server stats.
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void updateGameState(PatchStateContainer_t * gameState)
{
	int i,j;

	// stats
	if (gameState->UpdateCustomGameStats) {
    	gameState->CustomGameStatsSize = sizeof(struct InfectedGameData);
		struct InfectedGameData* sGameData = (struct InfectedGameData*)gameState->CustomGameStats.Payload;
		sGameData->Version = 0x00000001;

		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			sGameData->Infections[i] = Infections[i];
			sGameData->IsInfected[i] = isInfected(i);
			sGameData->IsFirstInfected[i] = FirstInfected[i];
			DPRINTF("%d: infections:%d infected:%d firstInfected:%d\n", i, Infections[i], isInfected(i) != 0, FirstInfected[i]);
		}
	}
}

/*
 * NAME :		initialize
 * 
 * DESCRIPTION :
 * 			Initializes the gamemode.
 * 
 * NOTES :
 * 			This is called only once at the start.
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void initialize(void)
{
	// No packs
	disableWeaponPacks();

	// 
	memset(FirstInfected, 0, sizeof(FirstInfected));
	memset(Infections, 0, sizeof(Infections));

	// hook into player kill event
	HOOK_JAL(GetAddress(&vaOnPlayerKill_Hook), &onPlayerKill);

	// Disable health boxes
	if (disableHealthboxes())
		Initialized = 1;
}


/*
 * NAME :		gameStart
 * 
 * DESCRIPTION :
 * 			Infected game logic entrypoint.
 * 
 * NOTES :
 * 			This is called only when in game.
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void gameStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
	int i = 0;
	int infectedCount = 0;
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

	// 
	updateGameState(gameState);

	if (!gameHasEnded())
	{
		// Reset to infected team
		// If one player isn't infected then their team
		// is set to winning team
		WinningTeam = INFECTED_TEAM;

		// Iterate through players
		for (i = 0; i < GAME_MAX_PLAYERS; ++i)
		{
			if (!players[i])
				continue;
			
			// Process
			processPlayer(players[i]);

			// Count
			++playerCount;
			if (isInfected(players[i]->mpIndex))
			{
				++infectedCount;
			}
			else
			{
				WinningTeam = players[i]->mpTeam;
			}
		}
	}

	// 
	gameData->WinningTeam = WinningTeam;

	if (!gameHasEnded())
	{
		// If no survivors then end game
		if (playerCount == infectedCount)
		{
			// End game
			gameEnd(2);
		}
		else if (infectedCount == 0)
		{
			// Infect first player after 10 seconds
			if ((gameGetTime() - gameSettings->GameStartTime) > (10 * TIME_SECOND))
			{
				Player * survivor = getRandomSurvivor(gameSettings->GameStartTime);
				if (survivor)
				{
					infect(survivor->mpIndex);
					FirstInfected[survivor->mpIndex] = 1;
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
	gameOptions->GameFlags.MultiplayerGameFlags.Turboslider = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.Hovership = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.Vehicle_02 = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.Vehicle_03 = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.Nodes = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_Bots = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_BaseAmmoHealth = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_SmallTurrets = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.BaseTeleporter = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.ChaosModeAttritionMode = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.Chargeboots = 1;
	gameOptions->GameFlags.MultiplayerGameFlags.UnlimitedAmmo = 0;
	gameOptions->GameFlags.MultiplayerGameFlags.PlayerNames = 1;

	// set everyone to blue
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		if (gameSettings->PlayerClanTags[i] >= 0)
			gameSettings->PlayerTeams[i] = 0;
	}
}

/*
 * NAME :		lobbyStart
 * 
 * DESCRIPTION :
 * 			Infected lobby logic entrypoint.
 * 
 * NOTES :
 * 			This is called only when in lobby.
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void lobbyStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
	u32 menu = 0;
	static int initializedScoreboard = 0;

	// 
	updateGameState(gameState);

	// Lobby
	if(menu = uiGetActivePointer(UIP_CREATE_GAME), menu != 0) {
		setLobbyGameOptions();
	}
	// Scoreboard
	else if (menu = uiGetActivePointer(UIP_END_GAME_DETAILS), menu != 0) {

	}
}

/*
 * NAME :		loadStart
 * 
 * DESCRIPTION :
 * 			Load logic entrypoint.
 * 
 * NOTES :
 * 			This is called only when the game has finished reading the level from the disc and before it has started processing the data.
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void loadStart(void)
{
  setLobbyGameOptions();
}
