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
int HasDeleteSiegeNodeTurrets = 0;
int HasKeepBaseHealthPadActive = 0;
short PlayerKills[GAME_MAX_PLAYERS];
short PlayerDeaths[GAME_MAX_PLAYERS];

int VampireHealRate[] = {
	PLAYER_MAX_HEALTH * 0.25,
	PLAYER_MAX_HEALTH * 0.50,
	PLAYER_MAX_HEALTH * 1.00
};

u32 onGameplayLoad(void* a0, long a1)
{
	GameplayHeaderDef_t * gameplay;

	// pointer to gameplay data is stored in $s6
	asm volatile (
		"move %0, $s6"
		: : "r" (gameplay)
	);
	if (gameConfig.grAllowDrones)
		onGameplayLoad_disableDrones(gameplay);

	if (gameConfig.grDisableWeaponCrates)
		onGameplayLoad_removeWeaponCrates(gameplay);

	if (gameConfig.grDisableAmmoPickups)
		onGameplayLoad_removeAmmoPickups(gameplay);

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
	}

	HasDisabledHealthboxes = 0;
	HasSetGattlingTurretHealth = 0;
	HasDeleteSiegeNodeTurrets = 0;
	HasKeepBaseHealthPadActive = 0;
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

	if (gameConfig.grNoBaseDefense_SmallTurrets && !HasDeleteSiegeNodeTurrets)
		HasDeleteSiegeNodeTurrets = deleteSiegeNodeTurrets();

	if (gameConfig.prChargebootForever)
		chargebootForever();
	
	if (gameConfig.grVampire)
		vampireLogic(VampireHealRate[gameConfig.grVampire - 1]);

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
