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
#include "interop/gamerules.h"

extern PatchConfig_t config;
extern PatchGameConfig_t gameConfig;
extern PatchPatches_t patched;
extern PatchConfig_t lobbyPlayerConfigs[GAME_MAX_PLAYERS];

int Gameplay_Hook = 0;
int Gameplay_Func = 0;

int GameRulesInitialized = 0;
int FirstPass = 1;
int HasSetGattlingTurretHealth = 0;
int HasDisableSiegeNodeTurrets = 0;
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
	if (patched.gameConfig.grVampire)
		return;

	HOOK_JAL((u32)GetAddress(&vaUpdateScoreboard) + 0x88, &vampireHeal);
	patched.gameConfig.grVampire = 1;
}

u32 onGameplayLoad(void* a0, long a1)
{
	GameplayHeaderDef_t * gameplay;

	// pointer to gameplay data is stored in $s6
	asm volatile (
		"move %0, $s6"
		: : "r" (gameplay)
	);
    if (gameConfig.grDisableHealthBoxes)
		onGameplayLoad_disableMoby(gameplay, MOBY_ID_HEALTH_BOX_MP, 0);

	if (gameConfig.grDisableDrones)
		onGameplayLoad_disableMoby(gameplay, MOBY_ID_DRONE_BOT_CLUSTER_CONFIG, 100);

	if (gameConfig.grDisableWeaponCrates)
		onGameplayLoad_disableWeaponCrates(gameplay);

	if (gameConfig.grDisableAmmoPickups)
		onGameplayLoad_disableAmmoPickups(gameplay);
	
	if (gameConfig.grDisablePlayerTurrets)
		onGameplayLoad_disableMoby(gameplay, MOBY_ID_PLAYER_TURRET, 0);

	if (gameConfig.prPlayerSize)
		onGameplayLoad_playerSize(gameplay);

	if (gameConfig.grNoBaseDefense_SmallTurrets)
		onGameplayLoad_disableMoby(gameplay, MOBY_ID_NODE_TURRET, 100);

	if (gameConfig.grRespawnTimer_HealthBoxes || gameConfig.grRespawnTimer_WeaponCrates || gameConfig.grRespawnTimer_AmmoPickups)
		onGameplayLoad_miscRespawnTimers(gameplay);

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

	// Set patched settings to 0
	memcpy(&patches, 0, sizeof(PatchPatches_t));

	HasSetGattlingTurretHealth = 0;
	HasDisableSiegeNodeTurrets = 0;
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

	if (gameConfig.grAutoRespawn && gameSettings->GameType == GAMERULE_DM)
		AutoRespawn();

	if (gameConfig.grSetGattlingTurretHealth && !HasSetGattlingTurretHealth)
		HasSetGattlingTurretHealth = setGattlingTurretHealth(gameConfig.grSetGattlingTurretHealth);

	if (gameConfig.grNoBaseDefense_SmallTurrets && !HasDisableSiegeNodeTurrets) {
		deleteNodeTurretsUpdate();
		HasDisableSiegeNodeTurrets = deleteSiegeNodeTurrets();
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
	
	if (gameConfig.grRespawnTimer_Player || gameConfig.grDisablePenaltyTimers)
		setRespawnTimer_Player();

	if (gameConfig.grBaseHealthPadActive && !HasKeepBaseHealthPadActive && gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_BaseAmmoHealth)
		HasKeepBaseHealthPadActive = keepBaseHealthPadActive();

	if (gameConfig.grNoCooldown)
		noPostHitInvinc();

	if (gameConfig.prPlayerSize)
		playerSize();

	if (gameConfig.grHealthBars)
		healthbars();

	if (gameConfig.grRadarBlipsDistance)
		radarBlips();

	if (gameConfig.grRespawnInvincibility)
		respawnInvincTimer();

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
