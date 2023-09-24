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
int HasKeepBaseHealthPadActive = 0;
short PlayerKills[GAME_MAX_PLAYERS];
short PlayerDeaths[GAME_MAX_PLAYERS];

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
 	VariableAddress_t vaUpdateWeaponKill = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0,
		.Hoven = 0,
		.OutpostX12 = 0,
		.KorgonOutpost = 0,
		.Metropolis = 0,
		.BlackwaterCity = 0,
		.CommandCenter = 0,
		.BlackwaterDocks = 0,
		.AquatosSewers = 0,
		.MarcadiaPalace = 0,
#else
		.Lobby = 0,
		.Bakisi = 0x004f8cc8,
		.Hoven = 0,
		.OutpostX12 = 0,
		.KorgonOutpost = 0,
		.Metropolis = 0,
		.BlackwaterCity = 0,
		.CommandCenter = 0,
		.BlackwaterDocks = 0,
		.AquatosSewers = 0,
		.MarcadiaPalace = 0x004ec000,
#endif
	};

	// run base
	((void (*)(int, int))GetAddress(&vaUpdateWeaponKill))(player, weaponid);

	int VampireHealRate[] = {0.25, 0.50, 0.75, 1.00};
	// give player health
	int Heal = (int)PLAYER_MAX_HEALTH * VampireHealRate[gameConfig.grVampire - 1];
	int CurrentHealth = (int)player->pNetPlayer->pNetPlayerData->hitPoints;
	printf("\nCurrentHealth: %d", CurrentHealth);
	printf("\nHeal Rate: %d", Heal);
	printf("\nTotal Heal: %d", (CurrentHealth + Heal));
	playerSetHealth(player, clamp(CurrentHealth + Heal, 0, PLAYER_MAX_HEALTH));
}

void vampireLogic(void)
{
	VariableAddress_t vaUpdateScoreboard = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0,
		.Hoven = 0,
		.OutpostX12 = 0,
		.KorgonOutpost = 0,
		.Metropolis = 0,
		.BlackwaterCity = 0,
		.CommandCenter = 0,
		.BlackwaterDocks = 0,
		.AquatosSewers = 0,
		.MarcadiaPalace = 0,
#else
		.Lobby = 0,
		.Bakisi = 0x00541b78,
		.Hoven = 0,
		.OutpostX12 = 0,
		.KorgonOutpost = 0,
		.Metropolis = 0,
		.BlackwaterCity = 0,
		.CommandCenter = 0,
		.BlackwaterDocks = 0,
		.AquatosSewers = 0,
		.MarcadiaPalace = 0x00534758,
#endif
	};
	if (GetAddress(&vaUpdateScoreboard))
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
	HasDisableSiegeNodeTurrets = 0;
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

	if (gameConfig.grNoBaseDefense_SmallTurrets && !HasDisableSiegeNodeTurrets)
		HasDisableSiegeNodeTurrets = deleteSiegeNodeTurrets();

	if (gameConfig.prChargebootForever)
		chargebootForever();
	
	if (gameConfig.grVampire)
		vampireLogic();

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
