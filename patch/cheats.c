/***************************************************
 * FILENAME :		cheats.c
 * DESCRIPTION :
 * 		Contains simple cheats/game settings for UYA.
 * NOTES :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>
#include <kernel.h>

#include <libuya/string.h>
#include <libuya/player.h>
#include <libuya/utils.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/pad.h>
#include <libuya/stdio.h>
#include <libuya/uya.h>
#include <libuya/math.h>
#include <libuya/math3d.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include <libuya/time.h>
#include <libuya/interop.h>
#include <libuya/net.h>
#include <libuya/moby.h>
#include <libuya/guber.h>
#include <libuya/gameplay.h>
#include <libuya/weapon.h>
#include <libuya/map.h>
#include <libuya/sound.h>
#include "module.h"
#include "messageid.h"
#include "config.h"
#include "include/config.h"
#include "include/cheats.h"
#include "interop/cheats.h"
#include "utils.h"

SoundDef TimerTickSoundDef = {1000, 1000, 2000, 2000, 0, 0, 0, 0x10, 133, 0};
TimerVars_t allNodesTimer = {
    .timer_x = SCREEN_WIDTH / 2,
    .timer_y = SCREEN_HEIGHT * 0.8,
    .timerScale = 3,
    .title = 0,
    .title_x = SCREEN_WIDTH / 2,
    .title_y = SCREEN_HEIGHT * 0.73,
    .titleScale = 1,
    .colorBase = 0x80ff0000,
    .colorHigh = 0x80ffffff,
    .font = FONT_DEFAULT,
    .timeValue = 60,
    .timeStartTicking = 10,
    .tickSound = &TimerTickSoundDef,
    .startTime = -1,
    .lastPlayedTickSound = -1,
    .bDynamicScaleTime = 1,
    .status = -1
};

TimerVars_t selectNodesTimer = {
    .timer_x = SCREEN_WIDTH / 2,
    .timer_y = SCREEN_HEIGHT * 0.17,
    .timerScale = 1,
    .title = 0,
    .title_x = SCREEN_WIDTH / 2,
    .title_y = SCREEN_HEIGHT * 0.1,
    .titleScale = 1,
    .colorBase = 0x8060bfee,
    .colorHigh = 0,
    .font = FONT_DEFAULT,
    .timeValue = 10,
    .timeStartTicking = -1,
    .tickSound = 0,
    .startTime = -1,
    .lastPlayedTickSound = -1,
    .bDynamicScaleTime = 0,
    .status = -1
};

int selectedNode = 1;

extern maxNodeCount;
extern siegeGameOver;
extern PlayerKills[GAME_MAX_PLAYERS];
extern PlayerDeaths[GAME_MAX_PLAYERS];
extern PlayerTeams[GAME_MAX_PLAYERS];
extern PatchGameConfig_t gameConfig;
extern PatchPatches_t patched;
extern variableAddressCompact_t vaPlayerRespawnFunc;
extern variableAddressCompact_t vaGiveWeaponFunc;
extern variableAddressCompact_t vaGiveWeaponUpgradeFunc;

/*
 * NAME :		disableWeaponPacks
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void disableWeaponPacks(void)
{
	if (patched.gameConfig.grDisableWeaponPacks)
		return;

	u32 weaponPackSpawnFunc = GetAddress(&vaWeaponPackSpawnFunc);
	*(u32*)weaponPackSpawnFunc = 0;
	*(u32*)(weaponPackSpawnFunc - 0x7BF4) = 0;
	patched.gameConfig.grDisableWeaponPacks = 1;
}

/*
 * NAME :		v2_Setting
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */

void v2_logic(void)
{
	Player ** players = playerGetAll();
	int i, j;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		Player * player = players[i];
		if (!player)
			continue;

		// Loop through each weapon slot
		u8* slot = (u8*)(u32)player + 0x1a32;
		for(j = 0; j < 8; ++j) {
			// Check to see if the weapon slow has a weapon.
			// If so, deofuscate the weapon id and give the player the upgrade.
			if (slot[j] > 0)
				playerGiveWeaponUpgrade(player, playerDeobfuscate(&slot[j], 1));
		}
	}
}
void v2_Setting(int setting, int FirstPass)
{
	if (patched.gameConfig.grV2s)
		return;

	// Disable V2's
	if (setting == 1) {
		// u32 addr = GetAddress(&vaUpdateWeaponKill);
		// if (*(u32*)(addr + 0x27c) == 0x24420001) { // addiu v0, v0, 0x1;
		// 	*(u32*)(addr + 0x27c) = 0; // addiu v0, v0, 0x1;
		// 	*(u32*)(addr + 0x288) = 0; // sb v0, 0x0(t1);
		// 	*(u32*)(addr + 0x298) = 0;
		// 	patched.gameConfig.grV2s = 1;
		// }
		u32 addr = GetAddress(&vaGiveWeaponUpgradeFunc);
		*(u32*)addr = 0x03e00008;
		*(u32*)(addr + 0x4) = 0;
		patched.gameConfig.grV2s = 1;
	}
	// Always V2's
	else {
		// hook v2 logic at end of give weapon function.
		int GiveWeapon_JRRA = (u32)GetAddress(&vaGiveWeaponFunc) + 0x538;
		if (*(u32*)GiveWeapon_JRRA == 0x03e00008)
			HOOK_J(GiveWeapon_JRRA, &v2_logic);
		if (FirstPass)
			v2_logic();

		patched.gameConfig.grV2s = 1;
	}
}

/*
 * NAME :		AutoRespawn
 * DESCRIPTION :
 *              DM Only, Freezes in Siege and CTF due to nodes.
 *              Respawns player automatically.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void RespawnPlayer(int a0)
{
	// set player to register v1's value.
	register int player asm("v1");
	playerRespawn(player);

	// run original function
	uiMsgString(a0);
}
void AutoRespawn(void)
{
	if (patched.gameConfig.grAutoRespawn)
		return;
	//GameOptions * gameOptions = (GameOptions*)0x002417C8;
	//gameOptions->GameFlags.MultiplayerGameFlags.Nodes
	// Siege & CTF: Press X to Respawn
	// *(u32*)0x004A55C8 = (0x0C000000 | ((u32)(&RespawnPlayer) >> 2));

	// DM: Press X To Respawn JAL
	// Freezes in Siege and CTF due to needing to choose nodes, even if nodes are off.
	HOOK_JAL(GetAddress(&vaDM_PressXToRespawn), &RespawnPlayer);
	patched.gameConfig.grAutoRespawn = 1;
}

/*
 * NAME :		setGatlingTurretHealth
 * DESCRIPTION :
 *              
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int setGatlingTurretHealth(int value)
{
    int init = 0;
    Moby * moby = mobyListGetStart();
    // Iterate through mobys and change health
    while ((moby = mobyFindNextByOClass(moby, MOBY_ID_GATLING_TURRET))) {
        if (moby->pVar) {
			// Gatling Turret Health is stored as a float and as it's
			// hexidecimal value.  We use the hex value and multiply it
			// by our wanted value, then store it as it's float health
			int HexHealth = ((u32)moby->pVar + 0x34);
			int NewHealth;
			switch (value) {
				case 1: // .5 Health
					NewHealth = 150; break;
				case 2: // 1.5 Health
					NewHealth = 450; break;
				default: // All Other values
					NewHealth = *(int*)HexHealth * (int)(value - 1);
			}

			*(u32*)HexHealth = NewHealth;
			*(float*)((u32)moby->pVar + 0x30) = NewHealth;
        }
        ++moby; // next moby
    }
	// Init makes this work correctly.  just using "return moby;" doesn't
    init = 1;
    return init;
}

/*
 * NAME :		disableCameraShake
 * DESCRIPTION :
 *              Removes Camera Shake

 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void disableCameraShake(void)
{
	if (patched.config.disableCameraShake)
		return;

	int CameraShake = GetAddress(&vaCameraShakeFunc);
	if (*(u32*)CameraShake == 0x24030460) {
		// JR RA the start of the Camera shake function
		*(u32*)CameraShake = 0x03e00008; // jr ra
		*(u32*)(CameraShake + 0x4) = 0; // nop
		patched.config.disableCameraShake = 1;
	}
}

/*
 * NAME :		DisableRespawning
 * DESCRIPTION :
 *              Only for DM.  Disables respawn timer and pressing x function.

 * NOTES :      Only used as a cheat, doesn't get activated via patch menu.
 * 				Important Note:  Can also do: player->timers.noDeathTimer = 10;
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void disableRespawning(void)
{
	if (patched.disableRespawning)
		return;

	// Disable Timer and respawn text.
    int RespawnUpdater = GetAddress(&vaDM_RespawnUpdater);
    if (*(u32*)RespawnUpdater != 0)
        *(u32*)RespawnUpdater = 0;

	// Disable Respawn Function
	int RespawnFunc = GetAddress(&vaPlayerRespawnFunc);
    if (*(u32*)RespawnFunc != 0) {
		*(u32*)RespawnFunc = 0x03e00008;
        *(u32*)(RespawnFunc + 0x4) = 0;
	}
	patched.disableRespawning = 1;
}

/*
 * NAME :		survivor
 * DESCRIPTION :
 *              Last Person standing wins!  Once a player is dead, they
 *              can't respawn.

 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void survivor(void)
{
	// patched.disableRespawning becomes true in disableRespawning function.
	if (!patched.disableRespawning)
		disableRespawning();

    int DeadPlayers = 0;
	int TeamCount = 0;
    int i;
	Player ** players = playerGetAll();
	GameData * gameData = gameGetData();
    GameSettings * gameSettings = gameGetSettings();
	GameOptions * gameOptions = gameGetOptions();
	int teams = gameOptions->GameFlags.MultiplayerGameFlags.Teams;
    int playerCount = gameSettings->PlayerCount;
	if (teams) {
		if (!TeamCount) {
			for (i = 0; i < playerCount; ++i) {
				if (!players[i])
					continue;

				++PlayerTeams[players[i]->mpTeam];
				++TeamCount;
			}
		}
		for (i = 0; i < playerCount; ++i) {
			if (!players[i])
				continue;
					
		    // Save current deaths for all players, and how many players have died.
			if (gameData->playerStats.frag[i].kills > PlayerDeaths[i]) {
				PlayerDeaths[i] = gameData->playerStats.frag[i].deaths;
				// Subtract player from their team.
				--PlayerTeams[players[i]->mpTeam];
				// if the team of the player who died noe equals zero, subject team coutn.
				if (PlayerTeams[players[i]->mpTeam] == 0)
					--TeamCount;
			}
			// If only one player in game, don't let game end until they die.
			if (playerCount == 1 && TeamCount == 0) {
				gameData->timeEnd = 0;
				gameData->winningTeam = i;
			}
			// if only one team remains
			else if (playerCount > 1 && TeamCount == 1) {
				gameData->timeEnd = 0;
                gameData->winningTeam = i;
			}
		}
	}
	// FFA Survivor
	else {
		for (i = 0; i < playerCount; ++i) {
			// Save current deaths for all players, and how many players have died.
			if (gameData->playerStats.frag[i].deaths > PlayerDeaths[i]) {
				PlayerDeaths[i] = gameData->playerStats.frag[i].deaths;
				++DeadPlayers;
			}
			// If only one player in game, don't let game end until they die.
			if (playerCount == 1 && DeadPlayers == 1) {
				gameData->timeEnd = 0;
				gameData->winningTeam = i;
			}
			// if player count is greater than 1, and Dead Players == Player Count - 1
			else if (playerCount > 1 && DeadPlayers == (playerCount - 1)) {
				// Check to see who has not died
				if (gameData->playerStats.frag[i].deaths == 0) {
					gameData->timeEnd = 0;
					gameData->winningTeam = i;
				}
			}
		}
	}
}

/*
 * NAME :		setRespawnTimer_Player
 * DESCRIPTION :
 *              Sets respawn timer to certain value.  Also can disable pentaly timers.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int setRespawnTimer_Player_Logic(Player *player)
{
	int time = 1.5; // default respawn time
	if (gameConfig.grRespawnTimer_Player) {
		switch (gameConfig.grRespawnTimer_Player) {
			case 1: time = 2; break;
			case 2: time = 2.5; break;
			case 11: time = 0; break;
			case 12: time = 1; break;
			default: time = gameConfig.grRespawnTimer_Player; break;
		}
	}
	if (!gameConfig.grDisablePenaltyTimers) {
		GameData *gameData = gameGetData();
		Moby *mobyInstance = mobyListGetStart();
		int baseIndex = gameData->allYourBaseGameData->baseLightIndex[player->mpTeam];
		int destroyedGatlin = 0;
		int destroyedAirDef = 0;
		Moby *base = mobyInstance + baseIndex;
		if (base->oClass == MOBY_ID_BASE_LIGHT) {
			destroyedGatlin = *(int*)((int)base->pVar + 0xb8) > 2;
			destroyedAirDef = *(int*)((int)base->pVar + 0x40) != 0;
		}
		if (destroyedGatlin && destroyedAirDef) {
			time += 6;
		} else if (destroyedGatlin || destroyedAirDef) {
			time += 2;
		}
	}
	if (player->lastDeathWasSuicide)
		time += gameConfig.grSuicidePenaltyTimer;
	
	return time * GAME_FPS;
}
void setRespawnTimer_Player(void)
{
	if (!patched.gameConfig.grRespawnTimer_Player)
		HOOK_JAL(GetAddress(&vaRespawnTimerFunc_Player), &setRespawnTimer_Player_Logic);

	patched.gameConfig.grRespawnTimer_Player = 1;
}

/*
 * NAME :		onGameplayLoad_disableMoby
 * DESCRIPTION :
 *              Disables the specified moby
 * NOTES :
 * ARGS : 
 * 				gameplay: don't mess with pls.  It's mine!
 *				mobyId: Specified moby ID to disable.
 *				ypos: set mobies y position.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onGameplayLoad_disableMoby(GameplayHeaderDef_t * gameplay, int mobyId, int ypos)
{
	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);
	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t* moby = &mobyInstancesHeader->MobyInstances[i];
		// if the OClass equals needed mobyId, set Y Position to wanted value.
		if (moby->OClass == mobyId)
			moby->PosY = ypos;
	}
}

/*
 * NAME :		onGameplayLoad_hideTeamNodeTurrets
 * DESCRIPTION :
 *              Hides ONLY the team (base) node turrets - the two-per-team turrets
 *              that guard each team's home base - by sinking their vertical
 *              position to 1.  The neutral capture-node turrets are left alone.
 *
 *              A node turret (MOBY_ID_NODE_TURRET / 0x1a63) stores a "Parent Node"
 *              MobyRef at pVar offset 588 (0x24c).  Team turrets are parented to the
 *              team's BASE_LIGHT (0x0cbe); neutral turrets are parented to a
 *              SIEGE_NODE (0x1a47).  We resolve the parent instance's OClass and only
 *              move turrets whose parent is a BASE_LIGHT.  This is map-independent.
 *
 *              Runs at gameplay-load time (before the engine spawns/rewires mobies),
 *              so the parent ref is still a raw static-instance index and the turret
 *              still spawns normally - vanilla code is unaffected.
 */
#define NODE_TURRET_PVAR_PARENT_NODE_OFFSET (588)
void onGameplayLoad_hideTeamNodeTurrets(GameplayHeaderDef_t * gameplay, bool disableAll)
{
	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);
	GameplayPVarDef_t * pvarTable = (GameplayPVarDef_t*)((u32)gameplay + gameplay->PVarTableOffset);
	u32 pvarData = (u32)gameplay + gameplay->PVarDataOffset;

	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t * moby = &mobyInstancesHeader->MobyInstances[i];
		if (moby->OClass != MOBY_ID_NODE_TURRET)
			continue;

		if (!disableAll) {
			// a turret with no pVars cannot be a team turret - and guards against a bad table read
			if (moby->PVarIndex < 0)
				continue;

			// resolve this turret's pVar data and read its Parent Node ref (a static instance index)
			GameplayPVarDef_t * pvar = &pvarTable[moby->PVarIndex];
			int parentIndex = *(int*)(pvarData + pvar->Offset + NODE_TURRET_PVAR_PARENT_NODE_OFFSET);
			if (parentIndex < 0 || parentIndex >= mobyInstancesHeader->StaticCount)
				continue;

			// team turrets are parented to the team BASE_LIGHT; neutral turrets to a SIEGE_NODE
			if (mobyInstancesHeader->MobyInstances[parentIndex].OClass == MOBY_ID_BASE_LIGHT)
				moby->PosY = 1;
		} else {
			moby->PosY = 1;
		}
	}
}



/*
 * NAME :		noPostHitInvinc
 * DESCRIPTION :
 *              Disables the post hit invicibility timer.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int noPostHitInvinc_Logic(void)
{
	// define ntsc and pal timer
	#if UYA_PAL
	int DEFAULT_TIMER = 0x27;
	#else
	int DEFAULT_TIMER = 0x2f;
	#endif
	// if player is getting shot by the gatling turret, set to default timer.
	// Player *p = playerGetFromSlot(0);
	// if (!p || !p->pWhoHitMe->oClass)
	// 	return 1;

	// if (p->pWhoHitMe->oClass == MOBY_ID_GATLING_TURRET_SHOT && p->pWhoHitMe->pParent->oClass == MOBY_ID_GATLING_TURRET)
	// 	return DEFAULT_TIMER;
	
	// else return 1
	return 1;
}
void noPostHitInvinc(void)
{
	if (patched.gameConfig.grNoCooldown)
		return;
	// PAL: 0x27, NTSC: 0x2f
	u32 time = GetAddress(&vaPostHitInvinc);
	HOOK_JAL(time, &noPostHitInvinc_Logic);
	POKE_U32(time + 0x4, 0);
	patched.gameConfig.grNoCooldown = 1;
}

/*
 * NAME :		onGameplayLoad_disableWeaponCrates
 * DESCRIPTION :
 *              Moves weapon crates to the bottom of the map.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onGameplayLoad_disableWeaponCrates(GameplayHeaderDef_t * gameplay)
{
	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);
	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t* Crates = &mobyInstancesHeader->MobyInstances[i];
		switch (Crates->OClass) {
			case MOBY_ID_CRATE_GRAVITY_BOMB:
			case MOBY_ID_CRATE_ROCKET_TUBE:
			case MOBY_ID_CRATE_FLUX:
			case MOBY_ID_CRATE_BLITZ:
			case MOBY_ID_CRATE_LAVA_GUN:
			case MOBY_ID_CRATE_HOLOSHIELD:
			case MOBY_ID_CRATE_MORPH_O_RAY:
			case MOBY_ID_CRATE_MINE:
			case MOBY_ID_CRATE_RANDOM_PICKUP:
			case MOBY_ID_CRATE_CHARGEBOOTS:
			case MOBY_ID_AMMO_PACK_HOLOSIELD:
				Crates->PosY = -100;
		}
	}
}

/*
 * NAME :		onGameplayLoad_disableAmmoPickups
 * DESCRIPTION :
 *              Moves ammo pickups to the bottom of the map.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onGameplayLoad_disableAmmoPickups(GameplayHeaderDef_t * gameplay)
{
	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);
	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t* Pickups = &mobyInstancesHeader->MobyInstances[i];
		switch (Pickups->OClass) {
			case MOBY_ID_AMMO_PACK_GRAVITY_BOMB:
			case MOBY_ID_AMMO_PACK_BLITZ:
			case MOBY_ID_AMMO_PACK_FLUX:
			case MOBY_ID_AMMO_PACK_ROCKET_TUBE:
			case MOBY_ID_AMMO_PACK_MINE:
			case MOBY_ID_AMMO_PACK_LAVA_GUN:
			case MOBY_ID_AMMO_PACK_HOLOSIELD:
			case MOBY_ID_AMMO_PACK_N60:
			case MOBY_ID_CRATE_RANDOM_PICKUP:
			case MOBY_ID_CRATE_CHARGEBOOTS: 
				Pickups->PosY = -100;
		}
	}
}




/*
 * NAME :		radarBlips
 * DESCRIPTION :
 *              Change how far the radarblips can be shown.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void radarBlips(void)
{
	if (patched.gameConfig.grRadarBlipsDistance)
		return;

	u32 float_dist = GetAddress(&vaRadarBlips_FloatVal);
	if (*(u32*)float_dist == 0x3c014499) {
		static const u16 upperDistance[] = { 0x4519, 0x4599, 0x7fff, 0, 0x4419 };
		int setting = gameConfig.grRadarBlipsDistance;
		if (setting >= 1 && setting <= 5) {
			*(u32*)float_dist = 0x3c010000 | upperDistance[setting - 1];
			*(u32*)(float_dist + 0x4) = setting == 3 ? 0x3421ffff : 0x34210000;
		}
		patched.gameConfig.grRadarBlipsDistance = 1;
	}
}

/*
 * NAME :		onGameplayLoad_miscRespawnTimers
 * DESCRIPTION :
 *              Updates Various respawn timers for items (Health Boxes, Weapon Crates and Ammo Packs)
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onGameplayLoad_miscRespawnTimers(GameplayHeaderDef_t * gameplay)
{
	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);
	u32 PVarOffsetPtr = ((u32)gameplay + gameplay->PVarTableOffset);
	u32 PVarDataPtr = ((u32)gameplay + gameplay->PVarDataOffset);
	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t* moby = &mobyInstancesHeader->MobyInstances[i];
		if (gameConfig.grRespawnTimer_HealthBoxes > 0) {
			switch (moby->OClass) {
				case MOBY_ID_HEALTH_BOX_MP: {
					GameplayPVarDef_t* PVarOffset = (GameplayPVarDef_t*)(PVarOffsetPtr + (u32)(moby->PVarIndex * 8));
					u32 data = PVarDataPtr + PVarOffset->Offset;
					*(int*)(data + 0x70) = (gameConfig.grRespawnTimer_HealthBoxes - 1) * 5;
				}
			}
		}
		// if (gameConfig.grRespawnTimer_AmmoPickups > 0) {
		// 	switch (moby->OClass) {
		// 		case MOBY_ID_AMMO_PACK_GRAVITY_BOMB:
		// 		case MOBY_ID_AMMO_PACK_BLITZ:
		// 		case MOBY_ID_AMMO_PACK_FLUX:
		// 		case MOBY_ID_AMMO_PACK_ROCKET_TUBE:
		// 		case MOBY_ID_AMMO_PACK_MINE:
		// 		case MOBY_ID_AMMO_PACK_LAVA_GUN:
		// 		case MOBY_ID_AMMO_PACK_HOLOSIELD:
		// 		case MOBY_ID_AMMO_PACK_N60:
		// 		case MOBY_ID_CHARGEBOOTS_PICKUP: {
		// 			GameplayPVarDef_t* PVarOffset = (GameplayPVarDef_t*)(PVarOffsetPtr + (u32)(moby->PVarIndex * 8));
		// 			AmmoPickupVars_t* data = (AmmoPickupVars_t*)((u32)PVarDataPtr + (u32)PVarOffset->Offset);
		// 			data->respawnTime = (gameConfig.grRespawnTimer_AmmoPickups - 1) * 5;
		// 		}
		// 	}
		// }
		// if (gameConfig.grRespawnTimer_WeaponCrates > 0) {
		// 	switch (moby->OClass) {
		// 		case MOBY_ID_CRATE_CHARGEBOOTS:
		// 		case MOBY_ID_CRATE_GRAVITY_BOMB:
		// 		case MOBY_ID_CRATE_ROCKET_TUBE:
		// 		case MOBY_ID_CRATE_FLUX:
		// 		case MOBY_ID_CRATE_BLITZ:
		// 		case MOBY_ID_CRATE_LAVA_GUN:
		// 		case MOBY_ID_CRATE_HOLOSHIELD:
		// 		case MOBY_ID_CRATE_MORPH_O_RAY:
		// 		case MOBY_ID_CRATE_MINE:
		// 		case MOBY_ID_CRATE_RANDOM_PICKUP: {
		// 			GameplayPVarDef_t* PVarOffset = (GameplayPVarDef_t*)(PVarOffsetPtr + (u32)(moby->PVarIndex * 8));
		// 			u32 data = PVarDataPtr + PVarOffset->Offset;
		// 			*(int*)(data + 0x70) = (gameConfig.grRespawnTimer_WeaponCrates - 1) * 5;
		// 		}
		// 	}
		// }
		// if (gameConfig.grRespawnTimer_SmallTurrets > 0 || gameConfig.grNoBaseDefense_SmallTurrets) {
		// 	int grRespawn = gameConfig.grRespawnTimer_SmallTurrets;
		// 	int grDisable = gameConfig.grNoBaseDefense_SmallTurrets;
		// 	int time = 0; // default
		// 	if ((grDisable && !grRespawn) || (grDisable && grRespawn)) {
		// 		time = -1;
		// 	} else if (!grDisable && grRespawn) {
		// 		time = (grRespawn - 1) * 5;
		// 	}
		// 	if (moby->OClass == MOBY_ID_NODE_TURRET) {
		// 		GameplayPVarDef_t* PVarOffset = (GameplayPVarDef_t*)(PVarOffsetPtr + (u32)(moby->PVarIndex * 8));
		// 		u32 data = PVarDataPtr + PVarOffset->Offset;
		// 		*(int*)(data + 0x70) = (gameConfig.grRespawnTimer_WeaponCrates - 1) * 5;
		// 	}
		// }
	}
}

// Health arbitration uses reliable lobby messages; native pickup events still own respawning.
#define HEALTH_PICKUP_DISTANCE_SQUARED (9.0f)
#define HEALTH_REQUEST_INTERVAL_MS (250)
#define HEALTH_TOKEN_MARKER (0x80000000)
#define HEALTH_PENDING_TOKEN (0x40000000)
#define HEALTH_TOKEN_MASK (0x3fffffff)
static u32 healthSession;
static u32 healthHelloToken;
static u32 healthLastAckToken;
static u32 healthLastAckAction;
static int healthNextRequest;
static int healthWasInGame;

/*
 * NAME :		healthBoxSend
 * DESCRIPTION :
 * 			Sends a health transaction to the plugin.
 * NOTES :
 *
 * ARGS :		Message payload.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
static void healthBoxSend(healthBoxMessage_t *msg)
{
	void *connection = netGetLobbyServerConnection();
	if (connection)
		netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_HEALTH_BOX, sizeof(*msg), msg);
}

/*
 * NAME :		healthBoxFind
 * DESCRIPTION :
 * 			Finds a live health box by its native network UID.
 * NOTES :
 *
 * ARGS :		Box UID.
 * RETURN :	Health box, or NULL.
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
static Moby *healthBoxFind(u32 uid)
{
	if (((uid >> 24) & 0xf) != 7)
		return NULL;
	GuberMoby *gm = (GuberMoby*)guberGetObjectByUID(uid);
	Moby *moby = gm ? gm->Moby : NULL;
	return moby && !mobyIsDestroyed(moby) && moby->oClass == MOBY_ID_HEALTH_BOX_MP
		&& moby->pVar && moby->pUpdate ? moby : NULL;
}

/*
 * NAME :		healthBoxConsumeLocal
 * DESCRIPTION :
 * 			Consumes the box without applying healing a second time.
 * NOTES :
 * 			The native state-5 hero UID slot is unused by this protocol. It retains the token across patch reloads.
 * ARGS :		Box and transaction token.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
static void healthBoxConsumeLocal(Moby *moby, u32 token)
{
	u32 *lastToken = (u32*)((u32)moby->pVar + 0x88);
	if (*lastToken == (token | HEALTH_TOKEN_MARKER) || ((*lastToken & HEALTH_TOKEN_MARKER)
		&& (*lastToken & HEALTH_PENDING_TOKEN) && (*lastToken & HEALTH_TOKEN_MASK) != (token & HEALTH_TOKEN_MASK)))
		return;
	GuberEvent event;
	memset(&event, 0, sizeof(event));
	event.NetEvent.EventID = 1;
	event.NetEvent.NetDataSize = 4;
	u32 noPlayer = 0xffffffff;
	memcpy(event.NetEvent.NetData, &noPlayer, 4);
	((void (*)(Moby*, GuberEvent*))((u32)moby->pUpdate - 0x408))(moby, &event);
	*lastToken = token | HEALTH_TOKEN_MARKER;
}

/*
 * NAME :		healthBoxOnMessage
 * DESCRIPTION :
 * 			Applies grants once and acknowledges the result to the plugin.
 * NOTES :
 *
 * ARGS :		Connection and message payload.
 * RETURN :	Consumed payload size.
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
static int healthBoxOnMessage(void *connection, void *data)
{
	healthBoxMessage_t msg;
	memcpy(&msg, data, sizeof(msg));
	if (connection != netGetLobbyServerConnection() || !isInGame())
		return sizeof(msg);
	if (msg.action == healthBoxReady) {
		if (msg.token == healthHelloToken) {
			if (!healthSession) healthNextRequest = gameGetTime();
			healthSession = msg.session;
		}
		return sizeof(msg);
	}
	if (!healthSession || msg.session != healthSession)
		return sizeof(msg);
	Moby *moby = healthBoxFind(msg.boxUid);
	if (msg.action == healthBoxConsume) {
		if (moby && msg.token)
			healthBoxConsumeLocal(moby, msg.token);
	} else if (msg.action == healthBoxRespawn) {
		if (moby) {
			u32 *lastToken = (u32*)((u32)moby->pVar + 0x88);
			if (!(*lastToken & HEALTH_TOKEN_MARKER) || !(*lastToken & HEALTH_PENDING_TOKEN)) {
				if (moby->state == 6) {
					GuberEvent event;
					memset(&event, 0, sizeof(event));
					event.NetEvent.EventID = 2;
					((void (*)(Moby*, GuberEvent*))((u32)moby->pUpdate - 0x408))(moby, &event);
				}
				*lastToken = msg.token | HEALTH_TOKEN_MARKER;
			}
		}
	} else if (msg.action == healthBoxCatchUp) {
		if (moby)
			*(u32*)((u32)moby->pVar + 0x88) = msg.token ? msg.token | HEALTH_TOKEN_MARKER : 0;
	} else if (msg.action == healthBoxGrant) {
		if (healthLastAckToken == msg.token) {
			msg.action = healthLastAckAction;
		} else {
			msg.action = healthBoxFailed;
			if (moby && msg.token) {
				u32 lastToken = *(u32*)((u32)moby->pVar + 0x88);
				if ((lastToken & HEALTH_TOKEN_MARKER) && (lastToken & HEALTH_TOKEN_MASK) == msg.token) {
					msg.action = healthBoxApplied;
				} else if (moby->state == 4) {
					Player **players = playerGetAll();
					int i;
					for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
						Player *player = players[i];
						if (!player || !player->isLocal || player->guber.Id.UID != msg.playerUid)
							continue;
						VECTOR delta;
						vector_subtract(delta, player->playerPosition, moby->position);
						u32 update = (u32)moby->pUpdate;
						if (vector_sqrmag(delta) < HEALTH_PICKUP_DISTANCE_SQUARED
							&& ((int (*)(Player*, Moby*))(update - 0x7a8))(player, moby)
							&& ((int (*)(Moby*, Player*))(update + 0x968))(moby, player)) {
							healthBoxConsumeLocal(moby, msg.token | HEALTH_PENDING_TOKEN);
							msg.action = healthBoxApplied;
						}
						break;
					}
				}
			}
			healthLastAckToken = msg.token;
			healthLastAckAction = msg.action;
		}
		healthBoxSend(&msg);
	}
	return sizeof(msg);
}

/*
 * NAME :		healthBoxTick
 * DESCRIPTION :
 * 			Negotiates plugin support and clears local state between maps.
 * NOTES :
 *
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void healthBoxTick(void)
{
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_HEALTH_BOX, &healthBoxOnMessage);
	if (!isInGame()) {
		healthSession = 0;
		healthLastAckToken = 0;
		healthWasInGame = 0;
		return;
	}
	if (!healthWasInGame) {
		healthHelloToken = (u32)gameGetTime();
		healthNextRequest = 0;
		healthWasInGame = 1;
	}
	if (!healthSession && gameGetTime() >= healthNextRequest) {
		healthBoxMessage_t msg = {0};
		msg.action = healthBoxHello;
		msg.token = healthHelloToken;
		healthBoxSend(&msg);
		healthNextRequest = gameGetTime() + 1000;
	}
}

/*
 * NAME :		healthPickupMasterHook
 * DESCRIPTION :
 * 			Leaves health approval to the plugin after negotiation.
 * NOTES :
 *
 * ARGS :		Player and pickup.
 * RETURN :	Native eligibility for other pickups.
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
static int healthPickupMasterHook(Player *player, Moby *moby)
{
	if (healthSession && moby->oClass == MOBY_ID_HEALTH_BOX_MP)
		return 0;
	return ((int (*)(Player*, Moby*))((u32)moby->pUpdate - 0x7a8))(player, moby);
}

/*
 * NAME :		healthPickupRespawnHook
 * DESCRIPTION :
 * 			Lets the plugin schedule health respawns after a confirmed grant.
 * NOTES :
 * 			Other pickups retain the native pending-event check.
 * ARGS :		Pickup moby.
 * RETURN :	Nonzero while the plugin owns health respawns.
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
static int healthPickupRespawnHook(Moby *moby)
{
	if (healthSession && moby->oClass == MOBY_ID_HEALTH_BOX_MP
		&& (*(u32*)((u32)moby->pVar + 0x88) & HEALTH_TOKEN_MARKER))
		return 1;
	return ((int (*)(Moby*))((u32)moby->pUpdate - 0xb00))(moby);
}

/*
 * NAME :		healthPickupLocalHook
 * DESCRIPTION :
 * 			Requests a health pickup without healing or consuming it speculatively.
 * NOTES :
 *
 * ARGS :		Local player and pickup.
 * RETURN :	Native eligibility outside the plugin health path.
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
static int healthPickupLocalHook(Player *player, Moby *moby)
{
	u32 update = (u32)moby->pUpdate;
	if (!healthSession || moby->oClass != MOBY_ID_HEALTH_BOX_MP) {
		VECTOR delta;
		vector_subtract(delta, player->playerPosition, moby->position);
		if (vector_sqrmag(delta) >= 4.0f)
			return 0;
		return ((int (*)(Player*, Moby*))(update - 0x7a8))(player, moby);
	}
	if (player->isLocal && gameGetTime() >= healthNextRequest
		&& ((int (*)(Player*, Moby*))(update - 0x7a8))(player, moby)) {
		healthBoxMessage_t msg = {0};
		msg.action = healthBoxRequest;
		msg.session = healthSession;
		msg.boxUid = guberGetUID(moby);
		msg.playerUid = player->guber.Id.UID;
		msg.previousToken = *(u32*)((u32)moby->pVar + 0x88) & HEALTH_TOKEN_MASK;
		msg.respawnMs = *(int*)((u32)moby->pVar + 0x70);
		healthBoxSend(&msg);
		healthNextRequest = gameGetTime() + HEALTH_REQUEST_INTERVAL_MS;
	}
	return 0;
}

/*
 * NAME :		patchHealthPickups
 * DESCRIPTION :
 * 			Installs local health pickups and optionally removes their containers.
 * NOTES :
 * 			State 0 must run before making the orb available. States 5 and 6 retain
 * 			their pending-pickup and consumed/disabled behavior. Respawn keeps state 4.
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchHealthPickups(void)
{
	if (patched.gameConfig.grHealthBoxes)
		return;

	int pending = 0;
	int found = 0;
	int codeChanged = 0;
	Moby *moby = mobyListGetStart();
	while ((moby = mobyFindNextByOClass(moby, MOBY_ID_HEALTH_BOX_MP))) {
		if (!mobyIsDestroyed(moby)) {
			found = 1;
			if (!moby->pUpdate || !moby->pVar) {
				pending = 1;
				++moby;
				continue;
			}

			u32 update = (u32)moby->pUpdate;
			// Offsets verified in all NTSC-U/PAL multiplayer overlays.
			u32 localHook = update + 0x55c;
			u32 masterHook = update - 0xa8;
			u32 respawnHook = update - 0x54;
			u32 nativeRespawn = 0x0c000000 | ((update - 0xb00) >> 2);
			u32 respawnCheck = 0x0c000000 | ((u32)&healthPickupRespawnHook >> 2);
			u32 nativeCheck = 0x0c000000 | ((update - 0x7a8) >> 2);
			u32 localCheck = 0x0c000000 | ((u32)&healthPickupLocalHook >> 2);
			u32 masterCheck = 0x0c000000 | ((u32)&healthPickupMasterHook >> 2);
			u32 radius = *(u32*)(update + 0x528);
			if ((*(u32*)localHook == nativeCheck || *(u32*)localHook == localCheck)
				&& (*(u32*)masterHook == nativeCheck || *(u32*)masterHook == masterCheck)
				&& (*(u32*)respawnHook == nativeRespawn || *(u32*)respawnHook == respawnCheck)
				&& (radius == 0x3c014080 || radius == 0x3c014110)
				&& *(u32*)(update - 0x408) == 0x27bdffd0) {
				if (*(u32*)localHook != localCheck || *(u32*)masterHook != masterCheck || *(u32*)respawnHook != respawnCheck || radius != 0x3c014110) {
					*(u32*)localHook = localCheck;
					*(u32*)masterHook = masterCheck;
					*(u32*)respawnHook = respawnCheck;
					*(u32*)(update + 0x528) = 0x3c014110; // Squared distance: 9.0f.
					codeChanged = 1;
				}
			}

			if (gameConfig.grHealthBoxes == 1) {
				// Preserve the state-0 initialization; finish its state 3 below.
				if (*(u16*)(update + 0x1cc) != 3) {
					*(u16*)(update + 0x1cc) = 3;
					codeChanged = 1;
				}
				// Skip rebuilding the glass on respawn; the native event still creates the orb.
				if (*(u32*)(update - 0x26c) != 0x24050004 || *(u32*)(update - 0x268) != 0xa2050020) {
					*(u32*)(update - 0x26c) = 0x24050004;
					*(u32*)(update - 0x268) = 0xa2050020;
					codeChanged = 1;
				}
			}
			if (codeChanged) {
				FlushCache(0);
				FlushCache(2);
				codeChanged = 0;
			}

			if (gameConfig.grHealthBoxes == 1) {
				// Initialize even distant boxes before changing their state. Native code
				// creates the orb, converts the respawn timer, and handles disabled boxes.
				if (moby->state == 0)
					((void (*)(Moby*))moby->pUpdate)(moby);

				if (!mobyIsDestroyed(moby)) {
					if (moby->state == 0) {
						pending = 1;
					} else if (moby->state >= 1 && moby->state <= 3) {
						// Perform state 3's container cleanup without waiting for proximity.
						// No breaking sound is started by this initialization path.
						moby->collData = 0;
						moby->modeBits = (moby->modeBits & 0xefff) | 0x41;
						moby->collDamage = -1;
						mobySetState(moby, 4, -1);
					}
				}
			}
		}
		++moby;
	}
	patched.gameConfig.grHealthBoxes = found && !pending;
}





/*
 * NAME : patchDroneSpawning
 * DESCRIPTION : Disable cluster creation without changing node/team pvars.
 * NOTES : Load-time only; reloading the map restores the original spawn call.
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
void patchDroneSpawning(void)
{
	if (!gameConfig.grDisableDrones)
		return;

	u32 hook = GetAddressImmediate(&vaDroneClusterSpawnHook);
	if (!hook)
		return;

	u32* code = (u32*)hook;
	// lw a1,0xbc(s3); jal spawnCluster; lw t1,0xa0(s3).
	// Verified in all six stock NTSC-U/PAL overlays containing this controller.
	if (code[-1] != 0x8e6500bc || code[1] != 0x8e6900a0 ||
		(code[0] & 0xfc000000) != 0x0c000000)
		return;

	code[0] = 0;
	FlushCache(0);
	FlushCache(2);
}

/*
 * NAME :		runCheckAllNodes
 * DESCRIPTION :
 *             Checks to see if all nodes are owned by one team, if so, start countdown timer.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runCheckAllNodes(void) {
    int i;
    GameOptions *gameOptions = gameGetOptions();
    GameSettings *gameSettings = gameGetSettings();
    GameData *gameData = gameGetData();
    short nodes[2] = {0, 0};
    // get number of nodes and set timer value
    if (maxNodeCount == -1) {
        maxNodeCount = 0;
        Moby* mobyStart = mobyListGetStart();
        Moby* mobyEnd = mobyListGetEnd();
        while (mobyStart < mobyEnd) {
            if (mobyStart->oClass == MOBY_ID_SIEGE_NODE) {
                ++maxNodeCount;
            }
            ++mobyStart;
        }
    }
    // check which team each node is, and save it into nodes
    for (i = 0; i < maxNodeCount; ++i) {
        int nodesTeam = gameData->allYourBaseGameData->nodeTeam[i];
        if (nodesTeam == 0 || nodesTeam == 1)
            ++nodes[nodesTeam];
    }
    // check if team has all nodes and set timer title and color
    int blue = nodes[0];
    int red = nodes[1];
    int blueTeamAll = blue == maxNodeCount && red == 0;
    int redTeamAll = red == maxNodeCount && blue == 0;
    int teamWithAllNodes = (blueTeamAll && !redTeamAll) ? 0 : (redTeamAll && !blueTeamAll) ? 1 : -1;
    if (teamWithAllNodes > -1 && allNodesTimer.status == -1) {
		allNodesTimer.timeValue = gameConfig.grAllNodesTimer * 30;
		strncpy(allNodesTimer.title, teamWithAllNodes ? "Red Team Wins In" : "Blue Team Wins In", 32);
        allNodesTimer.colorBase = 0x80000000 | TEAM_COLORS[teamWithAllNodes];
        allNodesTimer.status = 0;
    } else if (teamWithAllNodes == -1 && allNodesTimer.status > -1){
        strncpy(allNodesTimer.title, "", 32);
        allNodesTimer.status = -1;
    }
    // check status of timer.  if finished, end game.
    if (allNodesTimer.status == 2) {
        if (siegeGameOver == 1)
            return;

        gameData->winningTeam = teamWithAllNodes;
        gameEnd(2);
        siegeGameOver = 1;
    } else {
        runTimer(&allNodesTimer);
    }

	#ifdef DEBUG
    // left/right: select node; up/down: set node to blue/red team
    Player *player = playerGetFromSlot(0);
    if (playerPadGetButtonDown(player, PAD_LEFT | PAD_R2 | PAD_L2) > 0) {
        if (selectedNode > 1)
            --selectedNode;
        else
            selectedNode = maxNodeCount;
    }
    if (playerPadGetButtonDown(player, PAD_RIGHT | PAD_R2 | PAD_L2) > 0) {
        if (selectedNode < maxNodeCount)
            ++selectedNode;
        else
            selectedNode = 1;
    }
    if (playerPadGetButtonDown(player, PAD_UP | PAD_R2 | PAD_L2) > 0) {
        gameData->allYourBaseGameData->nodeTeam[selectedNode - 1] = 0;
    }
    if (playerPadGetButtonDown(player, PAD_DOWN | PAD_R2 | PAD_L2) > 0) {
        gameData->allYourBaseGameData->nodeTeam[selectedNode - 1] = 1;
    }
    // printf("\nm: %d, n: %d, t: %d, b: %d, r: %d, a: %d", maxNodeCount, selectedNode, gameData->allYourBaseGameData->nodeTeam[selectedNode - 1], nodes[0], nodes[1], teamWithAllNodes);
	#endif
}

/*
 * NAME :		runSelectNodeTimer
 * DESCRIPTION :
 *             if Nodes are active and player dies, this starts a timer for how long they have to choose a node.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runSelectNodeTimer(void)
{
    Player *player = playerGetFromSlot(0);
    int isDead = playerIsDead(player);
    int status = selectNodesTimer.status;
    int resTimer = playerGetRespawnTimer(player);
    int state = playerGetState(player);
    if (state == PLAYER_STATE_WAIT_FOR_RESURRECT && status == -1) {
        selectNodesTimer.timeValue = gameConfig.grNodeSelectTimer;
        selectNodesTimer.status = 0;
    }
    // printf("\n%02i.%02i", (resTimer / 60) % 60, ((resTimer % 60) * 100) / 60);
    if (status > -1 && !isDead) {
        selectNodesTimer.status = -1;
	} else if (status == 2 && resTimer <= 0) {
        playerRespawn(player);
    } else {
        runTimer(&selectNodesTimer);
    }
}

/*
 * NAME :		patchSiegeTimeUp
 * DESCRIPTION :
 *             If time runs out in siege, winning team is deteremined by most base damange dealt.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchSiegeTimeUp_Logic(int reason)
{
    GameData *gameData = gameGetData();
    // get base health
    float blueHealth = gameData->allYourBaseGameData->hudHealth[0];
    float redHealth = gameData->allYourBaseGameData->hudHealth[1];
    // check who wins.  If both bases are same health, it's a tie, like normal.
    if (blueHealth < redHealth || blueHealth > redHealth) {
        gameData->winningTeam = blueHealth < redHealth;
        reason = 2;
    }
    // if both teams have equal health, it's a tie.  reason = 1.
    gameEnd(reason);
}

void patchSiegeTimeUp(void)
{
    if (!patched.gameConfig.grSiegeNoTies)
        HOOK_JAL(GetAddress(&vaGB_UpdateGameController_MasterEndGame_Hook), &patchSiegeTimeUp_Logic);

    patched.gameConfig.grSiegeNoTies = 1;
}
