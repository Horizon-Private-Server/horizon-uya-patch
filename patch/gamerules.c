/***************************************************
 * FILENAME :		gamerules.c
 * 
 * DESCRIPTION :
 * 		Gamerules entrypoint and logic.
 * 
 * NOTES :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>
#include <string.h>

#include <libuya/time.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/map.h>
#include <libuya/player.h>
#include <libuya/stdio.h>
#include <libuya/pad.h>
#include <libuya/uya.h>
#include <libuya/graphics.h>
#include <libuya/math3d.h>
#include <libuya/utils.h>
#include <libuya/net.h>
#include <libuya/gameplay.h>
#include <libuya/interop.h>
#include <libuya/moby.h>
#include "module.h"
#include "messageid.h"
#include "include/config.h"
#include "include/cheats.h"

// config
extern PatchConfig_t config;

// game config
extern PatchGameConfig_t gameConfig;

// lobby clients patch config
extern PatchConfig_t lobbyPlayerConfigs[GAME_MAX_PLAYERS];

// gameplay hook addresses
extern VariableAddress_t vaGameplayHook;

int Gameplay_Hook = 0;
int Gameplay_Func = 0;

int GameRulesInitialized = 0;
int FirstPass = 1;
int HasDisabledHealthboxes = 0;
int HasSetGattlingTurretHealth = 0;
int HasDisableSiegeNodeTurrets = 0;
int HasDisableNodeTurrets = 0;
int HasKeepBaseHealthPadActive = 0;
short PlayerKills[GAME_MAX_PLAYERS];
short PlayerDeaths[GAME_MAX_PLAYERS];
short PlayerTeams[GAME_MAX_PLAYERS];

// Vampire Logistics :D
int healRate = 0;
int VampireHealRate[] = {3, 7, 11, 15};

/*
 * NAME :		vampireLogic
 * 
 * DESCRIPTION :
 *              Handles Vampire Logic.  Heals player if they kill another.

 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void vampireHeal(Player * player, int weaponid)
{
	// Get Health
	int CurrentHealth = playerGetHealth(player);
	// Check to see if player isn't dead
	if (CurrentHealth > 0 && !playerIsDead(player)) {
		// Get New Health
		int HEALME = (CurrentHealth + healRate);
		// if new health greater than max, set player health to max else set to new health
		if (HEALME >= PLAYER_MAX_HEALTH) {
			playerSetHealth(player, 15);
		} else if (HEALME < PLAYER_MAX_HEALTH) {
			playerSetHealth(player, HEALME);
		}
	}
	// run update weapon kill
	((void (*)(int, int))GetAddress(&vaUpdateWeaponKill))(player, weaponid);
}
void vampireLogic()
{
	VariableAddress_t vaUpdateScoreboard = {
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
	HOOK_JAL((u32)GetAddress(&vaUpdateScoreboard) + 0x88, &vampireHeal);
}

u32 onGameplayLoad(void* a0, long a1)
{
	GameplayHeaderDef_t * gameplay;

	// pointer to gameplay data is stored in $s6
	asm volatile (
		"move %0, $s6"
		: : "r" (gameplay)
	);
	if (gameConfig.grDisableDrones)
		onGameplayLoad_disableMoby(gameplay, MOBY_ID_DRONE_BOT_CLUSTER_CONFIG);

	if (gameConfig.grDisableWeaponCrates)
		onGameplayLoad_disableWeaponCrates(gameplay);

	if (gameConfig.grDisableAmmoPickups)
		onGameplayLoad_disableAmmoPickups(gameplay);
	
	if (gameConfig.grDisablePlayerTurrets)
		onGameplayLoad_disableMoby(gameplay, MOBY_ID_PLAYER_TURRET);

	if (gameConfig.prPlayerSize)
		onGameplayLoad_playerSize(gameplay);

	// run base
	((void (*)(void*, long))Gameplay_Func)(a0, a1);
}

/*
 * NAME :		grInitialize
 * 
 * DESCRIPTION :
 * 			Initializes this module.
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
void grInitialize(GameSettings *gameSettings, GameOptions *gameOptions)
{
	int i;

	// Initialize player kills/deaths to 0
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
		PlayerKills[i] = 0;
		PlayerDeaths[i] = 0;
		PlayerTeams[i] = 0;
	}

	HasDisabledHealthboxes = 0;
	HasSetGattlingTurretHealth = 0;
	HasDisableSiegeNodeTurrets = 0;
	HasDisableNodeTurrets = 0;
	HasKeepBaseHealthPadActive = 0;
	healRate = 0;
	GameRulesInitialized = 1;
}

/*
 * NAME :		grGameStart
 * 
 * DESCRIPTION :
 * 			Gamerules game logic entrypoint.
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
void grGameStart(void)
{
	int i = 0;
	GameSettings * gameSettings = gameGetSettings();
	GameOptions * gameOptions = gameGetOptions();

	// Initialize
	if (GameRulesInitialized != 1)
		grInitialize(gameSettings, gameOptions);

	if (gameConfig.grDisableWeaponPacks == 1)
		disableWeaponPacks();
	else if (gameConfig.grDisableWeaponPacks == 2)
		spawnWeaponPackOnDeath();
	
	if (gameConfig.grV2s)
		v2_Setting(gameConfig.grV2s, FirstPass);

    if (gameConfig.grDisableHealthBoxes && !HasDisabledHealthboxes)
		HasDisabledHealthboxes = disableHealthboxes();

	if (gameConfig.grAutoRespawn && gameSettings->GameType == GAMERULE_DM)
		AutoRespawn();

	if (gameConfig.grSetGattlingTurretHealth && !HasSetGattlingTurretHealth)
		HasSetGattlingTurretHealth = setGattlingTurretHealth(gameConfig.grSetGattlingTurretHealth);

	if (gameConfig.grNoBaseDefense_SmallTurrets && !HasDisableSiegeNodeTurrets && !HasDisableNodeTurrets) {
		HasDisableSiegeNodeTurrets = deleteSiegeNodeTurrets();
		HasDisableNodeTurrets = deleteNodeTurrets();
	}

	if (gameConfig.prChargebootForever)
		chargebootForever();
	
	if (gameConfig.grVampire){
		healRate = VampireHealRate[gameConfig.grVampire - 1];
		vampireLogic();
	}

	if (config.disableCameraShake)
		disableCameraShake();

	if (gameConfig.prSurvivor)
		survivor();
	
	if (gameConfig.grRespawnTimer || gameConfig.grDisablePenaltyTimers)
		setRespawnTimer();

	if (gameConfig.grBaseHealthPadActive && !HasKeepBaseHealthPadActive && gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_BaseAmmoHealth)
		HasKeepBaseHealthPadActive = keepBaseHealthPadActive();

	if (gameConfig.grNoCooldown)
		noPostHitInvinc();

	if (gameConfig.prPlayerSize)
		playerSize();

	// if (gameConfig.grHealthBars)
	// 	healthbars();

	FirstPass = 0;
}

/*
 * NAME :		grLobbyStart
 * 
 * DESCRIPTION :
 * 			Gamerules lobby logic entrypoint.
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
void grLobbyStart(void)
{
	GameSettings * gameSettings = gameGetSettings();
	GameOptions * gameOptions = gameGetOptions();
	// if in staging
	if (gameSettings && gameSettings->GameLoadStartTime < 0) {
		// Let Gameconfig set GameOptions.
		if (gameConfig.grNoBaseDefense_Bots)
			gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_Bots = 0;
		
		if (gameConfig.grNoBaseDefense_SmallTurrets)
			gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_SmallTurrets = 0;
	} else {
		// If we're not in staging then reset
		GameRulesInitialized = 0;
		FirstPass = 1;
	}
}

/*
 * NAME :		grLoadStart
 * 
 * DESCRIPTION :
 * 			Gamerules load logic entrypoint.
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
void grLoadStart(void)
{
	// only handle when loading level
	GameSettings* gs = gameGetSettings();
	if (!gs || gs->GameStartTime >= 0)
		return;

	// Returns needed hook and needed function.
	Gameplay_Hook = GetAddressImmediate(&vaGameplayHook);
	// Convert the JAL to the function address and save for later.
	Gameplay_Func = JAL2ADDR(*(u32*)Gameplay_Hook);
	
	if (Gameplay_Hook != 0)
		*(u32*)Gameplay_Hook = 0x0C000000 | (u32)&onGameplayLoad / 4;
}
