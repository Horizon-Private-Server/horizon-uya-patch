/***************************************************
 * FILENAME :		cheats.c
 * 
 * DESCRIPTION :
 * 		Contains simple cheats/game settings for UYA.
 * 
 * NOTES :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>

#include <libuya/string.h>
#include <libuya/player.h>
#include <libuya/utils.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/pad.h>
#include <libuya/stdio.h>
#include <libuya/uya.h>
#include <libuya/math3d.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include <libuya/time.h>
#include <libuya/interop.h>
#include <libuya/net.h>
#include <libuya/moby.h>
#include <libuya/gameplay.h>
#include "module.h"
#include "messageid.h"
#include "config.h"
#include "include/config.h"
#include "include/cheats.h"
#include "interop/cheats.h"

extern PlayerKills[GAME_MAX_PLAYERS];
extern PlayerDeaths[GAME_MAX_PLAYERS];
extern PlayerTeams[GAME_MAX_PLAYERS];
extern PatchGameConfig_t gameConfig;
extern VariableAddress_t vaPlayerRespawnFunc;
extern VariableAddress_t vaGiveWeaponFunc;

/*
 * NAME :		disableWeaponPacks
 * 
 * DESCRIPTION :
 * 
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void disableWeaponPacks(void)
{
	const int patched = 0;
	if (patched)
		return;

	u32 weaponPackSpawnFunc = GetAddress(&vaWeaponPackSpawnFunc);
	if (weaponPackSpawnFunc) {
		*(u32*)weaponPackSpawnFunc = 0;
		*(u32*)(weaponPackSpawnFunc - 0x7BF4) = 0;
		patched = 1;
	}
}

/*
 * NAME :		spawnWeaponPackOnDeath
 * 
 * DESCRIPTION :
 * 
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int SpawnedPack = 0;
void SpawnPack(int a0, int a1, int a2)
{
	static u32 respawnTimerFunc = 0;
	static u32 SpawnWeaponPackFunc = 0;
	if (!respawnTimerFunc) {
		respawnTimerFunc = GetAddress(&vaRespawnTimerFunc);
		SpawnWeaponPackFunc = GetAddress(&vaSpawnWeaponPackFunc);
	}
    // Run Original Respawn Timer Hook
	((void (*)(int, int, int))respawnTimerFunc)(a0, a1, a2);
	
	// if pack already spawned, don't spawn more.
	if (SpawnedPack == 1)
		return;
	// set player to register v1's value.
	register int player asm("s3");
	// Spawn Pack
	((void (*)(u32))SpawnWeaponPackFunc)(player);
	// It now spawned pack, so set to true.
	SpawnedPack = 1;
}

void spawnWeaponPackOnDeath(void)
{
	static int patched = 0;
    // if Health is greater than zero and pack has spawned
    // This will be checking constantly, instead of just when the player dies.
    Player ** players = playerGetAll();
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
    	if (!players[i])
    		continue;

		Player * player = players[i];
		if (player->IsLocal && playerGetHealth(player) > 0)
            SpawnedPack = 0;
	}

	// if Patched don't run following.
	if (patched)
		return;

    // Disable normal Weapon Pack spawns
    disableWeaponPacks();
	// get needed hook address
	u32 RespawnTimerHook = GetAddress(&vaRespawnTimerHook);
    // Hook CTF/Siege SpawnPack
    HOOK_JAL(RespawnTimerHook, &SpawnPack);
    // Hook DM SpawnPack
    HOOK_JAL(((u32)RespawnTimerHook + 0x2100), &SpawnPack);

	// finished patching
	patched = 1;
}

/*
 * NAME :		v2_Setting
 * 
 * DESCRIPTION :
 * 
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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

		for (j = 1; j < 10; ++j)
			playerGiveWeaponUpgrade(player, j);
	}
}
void v2_Setting(int setting, int FirstPass)
{
	static int patched = 0;
	if (patched)
		return;

	// Disable V2's
	if (setting == 1) {
		// Prevent Weapon Meter value from going up.
		// u32 addr = GetAddress(&vaCheckWeaponKill);
		// if (*(u32*)(addr + 0x138) == 0x24630001) { // addiu v1, v1, 0x1;
		// 	*(u32*)(addr + 0xb8) = 0; // addiu v0, v0, 0x1;
		// 	*(u32*)(addr + 0x108) = 0; // addiu v0, v0, 0x1;
		// 	*(u32*)(addr + 0x138) = 0; // addiu v1, v1, 0x1;
		// }
		u32 addr = GetAddress(&vaUpdateWeaponKill);
		if (*(u32*)(addr + 0x27c) == 0x24420001) { // addiu v0, v0, 0x1;
			*(u32*)(addr + 0x27c) = 0; // addiu v0, v0, 0x1;
			*(u32*)(addr + 0x288) = 0; // sb v0, 0x0(t1);
			*(u32*)(addr + 0x298) = 0;
			patched = 1;
		}
	}
	// Always V2's
	else {
		// hook v2 logic at end of give weapon function.
		int GiveWeapon_JRRA = (u32)GetAddress(&vaGiveWeaponFunc) + 0x538;
		if (*(u32*)GiveWeapon_JRRA == 0x03e00008)
			HOOK_J(GiveWeapon_JRRA, &v2_logic);
		if (FirstPass)
			v2_logic();

		patched = 1;
	}
}

/*
 * NAME :		AutoRespawn
 * 
 * DESCRIPTION :
 *              DM Only, Freezes in Siege and CTF due to nodes.
 *              Respawns player automatically.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
	static int patched = 0;
	if (patched)
		return;
	//GameOptions * gameOptions = (GameOptions*)0x002417C8;
	//gameOptions->GameFlags.MultiplayerGameFlags.Nodes
	// Siege & CTF: Press X to Respawn
	// *(u32*)0x004A55C8 = (0x0C000000 | ((u32)(&RespawnPlayer) >> 2));

	// DM: Press X To Respawn JAL
	// Freezes in Siege and CTF due to needing to choose nodes, even if nodes are off.
	HOOK_JAL(GetAddress(&vaDM_PressXToRespawn), &RespawnPlayer);
	patched = 1;

}

/*
 * NAME :		setGatlinTurretHealth
 * 
 * DESCRIPTION :
 *              
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int setGattlingTurretHealth(int value)
{
    int init = 0;
    Moby * moby = mobyListGetStart();
    // Iterate through mobys and change health
    while ((moby = mobyFindNextByOClass(moby, MOBY_ID_GATTLING_TURRET))) {
        if (moby->PVar) {
			// Gattling Turret Health is stored as a float and as it's
			// hexidecimal value.  We use the hex value and multiply it
			// by our wanted value, then store it as it's float health
			int HexHealth = ((u32)moby->PVar + 0x34);
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
			*(float*)((u32)moby->PVar + 0x30) = NewHealth;
        }
        ++moby; // next moby
    }
	// Init makes this work correctly.  just using "return moby;" doesn't
    init = 1;
    return init;
}

/*
 * NAME :		deleteSiegeNodeTurret
 * 
 * DESCRIPTION :
 *              
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int deleteSiegeNodeTurrets(void)
{
    int init = 0;
    Moby * moby = mobyListGetStart();
    while ((moby = mobyFindNextByOClass(moby, MOBY_ID_SIEGE_NODE))) {
        if (moby->PUpdate)
			*(u32*)((u32)moby->PUpdate + 0x810) = 0;

        ++moby; // next moby
    }
    init = 1;
    return init;
}
void deleteNodeTurretsUpdate(void)
{
	static int patched = 0;
	if (patched)
		return;

	u32 UpdateFunc = GetAddress(&vaNodeTurret_UpdateFunc);
	if (*(u32*)UpdateFunc == 0x27bdffe0) {
		*(u32*)UpdateFunc = 0x03e0008;
		*(u32*)((u32)UpdateFunc + 0x4) = 0;
		patched = 1;
	}
}

/*
 * NAME :		ChargebootForever
 * 
 * DESCRIPTION :
 *              Lets Player keep Chargebooting if R2 is held.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void chargebootForever(void)
{
	int i;
	Player ** players = playerGetAll();
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		Player * player = players[i];
		if (!player)
			continue;

		if (player->timers.IsChargebooting == 1 && playerPadGetButton(player, PAD_R2) > 0 && player->timers.state > 55)
			player->timers.state = 55;
	}
}

/*
 * NAME :		disableCameraShake
 * 
 * DESCRIPTION :
 *              Removes Camera Shake

 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void disableCameraShake(void)
{
	static int patched = 0;
	if (patched)
		return;

	int CameraShake = GetAddress(&vaCameraShakeFunc);
	if (*(u32*)CameraShake == 0x24030460) {
		*(u32*)CameraShake = 0x03e00008;
		*(u32*)(CameraShake + 0x4) = 0;
		patched = 1;
	}
}

/*
 * NAME :		DisableRespawning
 * 
 * DESCRIPTION :
 *              Only for DM.  Disables respawn timer and pressing x function.

 * NOTES :      Only used as a cheat, doesn't get activated via patch menu.
 * 				Important Note:  Can also do: player->timers.noDeathTimer = 10;
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void disableRespawning(void)
{
	static int patched = 0;
	if (patched)
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
	patched = 1;
}

/*
 * NAME :		survivor
 * 
 * DESCRIPTION :
 *              Last Person standing wins!  Once a player is dead, they
 *              can't respawn.

 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void survivor(void)
{
	disableRespawning();

    static int DeadPlayers = 0;
	static int TeamCount = 0;
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
			if (gameData->PlayerStats.DeathMatch[i].Deaths > PlayerDeaths[i]) {
				PlayerDeaths[i] = gameData->PlayerStats.DeathMatch[i].Deaths;
				// Subtract player from their team.
				--PlayerTeams[players[i]->mpTeam];
				// if the team of the player who died noe equals zero, subject team coutn.
				if (PlayerTeams[players[i]->mpTeam] == 0)
					--TeamCount;
			}
			// If only one player in game, don't let game end until they die.
			if (playerCount == 1 && TeamCount == 0) {
				gameData->TimeEnd = 0;
				gameData->WinningTeam = i;
			}
			// if only one team remains
			else if (playerCount > 1 && TeamCount == 1) {
				gameData->TimeEnd = 0;
                gameData->WinningTeam = i;
			}
		}
	}
	// FFA Survivor
	else {
		for (i = 0; i < playerCount; ++i) {
			// Save current deaths for all players, and how many players have died.
			if (gameData->PlayerStats.DeathMatch[i].Deaths > PlayerDeaths[i]) {
				PlayerDeaths[i] = gameData->PlayerStats.DeathMatch[i].Deaths;
				++DeadPlayers;
			}
			// If only one player in game, don't let game end until they die.
			if (playerCount == 1 && DeadPlayers == 1) {
				gameData->TimeEnd = 0;
				gameData->WinningTeam = i;
			}
			// if player count is greater than 1, and Dead Players == Player Count - 1
			else if (playerCount > 1 && DeadPlayers == (playerCount - 1)) {
				// Check to see who has not died
				if (gameData->PlayerStats.DeathMatch[i].Deaths == 0) {
					gameData->TimeEnd = 0;
					gameData->WinningTeam = i;
				}
			}
		}
	}
}

/*
 * NAME :		setRespawnTimer_Player
 * 
 * DESCRIPTION :
 *              Sets respawn timer to certain value.  Also can disable pentaly timers.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void setRespawnTimer_Player(void)
{
	static int patched = 0;
	if (patched)
		return;

    int RespawnAddr = GetAddress(&vaRespawnTimerFunc_Player);
	if (gameConfig.grRespawnTimer_Player) {
	    int Seconds;
		switch (gameConfig.grRespawnTimer_Player) {
			case 10:
			case 11:
				Seconds = gameConfig.grRespawnTimer_Player - 10; break;
			default:
				Seconds = gameConfig.grRespawnTimer_Player + 1; break;
		}
        int RespawnTime = Seconds * GAME_FPS;
        
		// Set Default Respawn Timer
        if (*(u16*)(RespawnAddr + 0x10) != RespawnTime)
		    *(u16*)(RespawnAddr + 0x10) = RespawnTime;

		// Set Penalty Timer #1 based off of current respawn timer.
		if (!gameConfig.grDisablePenaltyTimers)
			*(u16*)(RespawnAddr + 0x78) = (Seconds + 1.5) * GAME_FPS;
	}
	if (gameConfig.grDisablePenaltyTimers) {
		// Jump to end of function after respawn timer is set.
		if (*(u32*)(RespawnAddr + 0x28) == 0x14440003)
			*(u32*)(RespawnAddr + 0x28) = 0x1000001A;

		// Gatlin Turret Destroyed (RespawnTime + This)
		// *(u16*)(RespawnAddr + 0x80) = RespawnTime;
		// Anti-Air Turret Destroyed (RespawnTime + This)
		// *(u16*)(RespawnAddr + 0x8c) = RespawnTime;
	}
	patched = 1;
}

/*
 * NAME :		onGameplayLoad_disableMoby
 * 
 * DESCRIPTION :
 *              Disables the specified moby
 * NOTES :
 * 
 * ARGS : 
 * 				gameplay: don't mess with pls.  It's mine!
 *				mobyId: Specified moby ID to disable.
 *				ypos: set mobies y position.
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onGameplayLoad_disableMoby(GameplayHeaderDef_t * gameplay, int mobyId, int ypos)
{
	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);
	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t* moby = &mobyInstancesHeader->MobyInstances[i];
		if (moby->OClass == mobyId){
			moby->PosY = ypos;
		}
	}
}

/*
 * NAME :		keepBaseHealthPadActive
 * 
 * DESCRIPTION :
 *              Keeps Base Ammo/Health Pads Active even if Gatlin Turrets are destroyed.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int keepBaseHealthPadActive(void)
{
	int init = 0;
	Moby * a = mobyListGetStart();
	while ((a = mobyFindNextByOClass(a, MOBY_ID_HEALTH_PAD))) {
		if (a->PUpdate) {
			*(u32*)((u32)a->PUpdate + 0x68) = 0x24020001;
			*(u32*)((u32)a->PUpdate + 0x8C) = 0x24150001;
		}
		++a;
	}
	Moby * b = mobyListGetStart();
	while ((b = mobyFindNextByOClass(b, MOBY_ID_AMMO_PAD))) {
		if (b->PUpdate) {
			*(u32*)((u32)b->PUpdate + 0x74) = 0x24020001;
			*(u32*)((u32)b->PUpdate + 0x90) = 0x24020001;
		}
		++b;
	}
	init = 1;
	return init;
}

/*
 * NAME :		noPostHitInvinc
 * 
 * DESCRIPTION :
 *              Disables the post hit invicibility timer.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void noPostHitInvinc(void)
{
	static int patched = 0;
	if (patched)
		return;
	// PAL: 0x27, NTSC: 0x2f
	int time = GetAddress(&vaPostHitInvinc);
	*(u32*)(time) = 0x24020001;
	*(u32*)(time + 0x308) = 0x24020001;
	patched = 1;
}

/*
 * NAME :		onGameplayLoad_disableWeaponCrates
 * 
 * DESCRIPTION :
 *              Moves weapon crates to the bottom of the map.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * 
 * DESCRIPTION :
 *              Moves ammo pickups to the bottom of the map.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * NAME :		playerSize
 * 
 * DESCRIPTION :
 *              Modifies the player's size (and other objects)
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerSize(void)
{
	int i, j;
	Player** players = playerGetAll();
	float size;
	float cameraHeight;
	float tpHeight;
	float moveSpeed = 1;
	
	switch (gameConfig.prPlayerSize)
	{
		case 1: size = 1.5; cameraHeight = 0.75; tpHeight = 3; moveSpeed = 1.5; break; // large
		case 2: size = 3; cameraHeight = 2.5; tpHeight = 5; moveSpeed = 3; break; // giant
		case 3: size = 0.2; cameraHeight = -0.80; tpHeight = 0.3; moveSpeed = 0.5; break; // tiny
		case 4: size = 0.666; cameraHeight = -0.25; tpHeight = 1.25; moveSpeed = 0.75; break; // small
	}

	// disable fixed player scale
	//*(u32*)0x005D1580 = 0;

	//
	// float m = 1024 * size;
	// asm (".set noreorder;");
	// *(u16*)0x004f79fc = *(u16*)((u32)&m + 2);

	// // chargeboot distance
	// m = 6 * size;
	// asm (".set noreorder;");
	// *(u16*)0x0049A688 = *(u16*)((u32)&m + 2);

	// // chargeboot height
	// m = tpHeight;
	// asm (".set noreorder;");
	// *(u16*)0x0049A6B4 = *(u16*)((u32)&m + 2);

	// // chargeboot look at height
	// m = tpHeight;
	// asm (".set noreorder;");
	// *(u16*)0x0049a658 = *(u16*)((u32)&m + 2);

	// // third person distance and height
	// *(float*)0x002391FC = 4 * size;
	// *(float*)0x00239200 = tpHeight;
	// *(float*)0x00239180 = tpHeight + 0.5;

	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		Player * player = players[i];
		if (player) {
			if (player->PlayerMoby)
				player->PlayerMoby->Scale = 0.25 * size;

			// update camera
			player->fps.Vars.CameraPositionOffset[0] = -6 * size;
			player->fps.Vars.CameraPositionOffset[2] = cameraHeight;
			player->fps.Vars.range = 2 + cameraHeight;
		}
	}
}

/*
 * NAME :		onGameplayLoad_playerSize
 * 
 * DESCRIPTION :
 *              Modifies the player's size (and other objects) while game is loading.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onGameplayLoad_playerSize(GameplayHeaderDef_t * gameplay)
{
	float size;
	float cameraHeight;
	float tpHeight;
	float moveSpeed = 1;
	switch (gameConfig.prPlayerSize)
	{
		case 1: size = 1.5; cameraHeight = 0.75; tpHeight = 3; moveSpeed = 1.5; break; // large
		case 2: size = 3; cameraHeight = 2.5; tpHeight = 5; moveSpeed = 3; break; // giant
		case 3: size = 0.2; cameraHeight = -0.80; tpHeight = 0.3; moveSpeed = 0.5; break; // tiny
		case 4: size = 0.666; cameraHeight = -0.25; tpHeight = 1.25; moveSpeed = 0.75; break; // small
	}

	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);
	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t* moby = &mobyInstancesHeader->MobyInstances[i];
		switch (moby->OClass) {
			case MOBY_ID_TURBOSLIDER:
			case MOBY_ID_HOVERSHIP:
			case MOBY_ID_TANK:
			case MOBY_ID_VEHICLE_PAD:
			case MOBY_ID_WEAPON_WRENCH:
			case MOBY_ID_WEAPON_BLITZ_CANNON:
			case MOBY_ID_WEAPON_MINI_ROCKET_TUBE:
			case MOBY_ID_WEAPON_GRAVITY_BOMB:
			case MOBY_ID_WEAPON_N60_STORM:
			case MOBY_ID_WEAPON_FLUX_RIFLE:
			case MOBY_ID_WEAPON_MINE:
			case MOBY_ID_WEAPON_LAVA_GUN:
			case MOBY_ID_WEAPON_MORPH_0_RAY:
			case MOBY_ID_WEAPON_HOLOSHIELD:
			case MOBY_ID_PLAYER_TURRET:
			case MOBY_ID_TELEPORT_PAD:
			case MOBY_ID_JUMP_PAD:
			case MOBY_ID_AMMO_PAD:
			case MOBY_ID_HEALTH_PAD:
			case MOBY_ID_AMMO_PACK_GRAVITY_BOMB:
			case MOBY_ID_AMMO_PACK_BLITZ:
			case MOBY_ID_AMMO_PACK_FLUX:
			case MOBY_ID_AMMO_PACK_ROCKET_TUBE:
			case MOBY_ID_AMMO_PACK_MINE:
			case MOBY_ID_AMMO_PACK_LAVA_GUN:
			case MOBY_ID_AMMO_PACK_HOLOSIELD:
			case MOBY_ID_AMMO_PACK_N60:
			case MOBY_ID_CHARGEBOOTS_PICKUP:
			case MOBY_ID_CRATE_CHARGEBOOTS:
			case MOBY_ID_CRATE_GRAVITY_BOMB:
			case MOBY_ID_CRATE_ROCKET_TUBE:
			case MOBY_ID_CRATE_FLUX:
			case MOBY_ID_CRATE_BLITZ:
			case MOBY_ID_CRATE_LAVA_GUN:
			case MOBY_ID_CRATE_HOLOSHIELD:
			case MOBY_ID_CRATE_MORPH_O_RAY:
			case MOBY_ID_CRATE_MINE:
			case MOBY_ID_CRATE_RANDOM_PICKUP:
			case MOBY_ID_SHOT_MINI_ROCKET_TUBE:
			case MOBY_ID_SHOT_GRAVITY_BOMB1:
			case MOBY_ID_SHOT_GRAVITY_BOMB2:
			case MOBY_ID_SHOT_GRAVITY_BOMB3:
			case MOBY_ID_SHOT_FLUX_RIFLE:
			case MOBY_ID_SHOT_MINE:
			case MOBY_ID_WEAPON_PACK:
			case MOBY_ID_CTF_RED_FLAG:
			case MOBY_ID_CTF_BLUE_FLAG:
			case MOBY_ID_SWINGSHOT_ORB:
			case MOBY_ID_HEALTH_BOX_MP:
			case MOBY_ID_HEALTH_ORB_MP:
			case MOBY_ID_PLAYER_ICE_CUBE:
			case MOBY_ID_MORPH_DUCK:
			case MOBY_ID_SIEGE_NODE_2:
			case MOBY_ID_ROLLING_DRONE_SPAWNER:
			case MOBY_ID_RANGER_TORSO:
			case MOBY_ID_RANGER_FEET:
			case MOBY_ID_BALL_BOT:
			case MOBY_ID_DRONE_BOT:
			case MOBY_ID_SHOCK_DROID:
			case MOBY_ID_BOLT_CRANK:
			case MOBY_ID_SIEGE_NODE:
			case MOBY_ID_NODE_TURRET:
			case MOBY_ID_SMALL_NODE_TURRET:
			case MOBY_ID_SHOCK_DROID_SPAWNER:
			case MOBY_ID_GATTLING_TURRET:
			case MOBY_ID_ANIT_VEHICLE_TURRET:
			case MOBY_ID_BASE_COMPUTER:
			{
				float CurrentScale = moby->Scale;
				moby->Scale = CurrentScale * size;
			}
		}
	}
}

/*
 * NAME :		healthbar_Logic
 * 
 * DESCRIPTION :
 *              Shows a healthbar above the players name.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void healthbars_Logic(float nameX, float nameY, u32 nameColor, char * nameStr, int strLen)
{
	asm(".set noreorder;");
	// run base function
	gfxScreenSpaceTextCenter(nameX, nameY, nameColor, nameStr, strLen);

	// get the register and copy name into buff.
	register u32 nameReg asm("s0");
	char buff[15];
	strncpy(buff, nameReg, 15);

	int i;
	Player ** players = playerGetAll();
	GameSettings * gs = gameGetSettings();
	int playercount = ((int)gs->PlayerCount - 1);
	for(i = 0; i < GAME_MAX_PLAYERS; ++i) {
		if (!players[i])
			return;

		// compare strings: if 0, strings match exactly.
		int comp = strncmp(buff, gs->PlayerNames[i], 0x15);
		if (!comp) {
			Player * player = players[i];
			int CurrentHealth = playerGetHealth(player);
			if (CurrentHealth > 0 && !playerIsDead(player)) {
				// printf("\ni: %d, buff: %s, health: %d", i, buff, CurrentHealth);
				float x = (float)nameX;// / SCREEN_WIDTH;
				float y = (float)nameY;// / SCREEN_HEIGHT;
				float health = (float)((int)CurrentHealth / (int)PLAYER_MAX_HEALTH);
				float w = (0.05 * 1) + 0.02, h = 0.02, p = 0.002;
				float right = w * health;
				x -= w / 2;
				gfxScreenSpaceBox(x,y,w-p,h-p, 0x80000000);
				gfxScreenSpaceBox(x,y,right,h, nameColor);
			}
		}
	}
}
/*
 * NAME :		healthbars
 * 
 * DESCRIPTION :
 *              Hooks healthbars_Logic
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void healthbars(void)
{
	static int patched = 0;
	if (patched)
		return;

	HOOK_JAL(GetAddress(&vaHealthbars_Hook), &healthbars_Logic);
	patched = 1;
}

/*
 * NAME :		radarBlips
 * 
 * DESCRIPTION :
 *              Change how far the radarblips can be shown.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void radarBlips(void)
{
	static int patched = 0;
	if (patched)
		return;

	u32 float_dist = GetAddress(&vaRadarBlips_FloatVal);
	if (*(u32*)float_dist == 0x3c014499) {
		switch (gameConfig.grRadarBlipsDistance) {
			case 1: { // Always
				*(u32*)float_dist = 0x3c017fff;
				*(u32*)(float_dist + 0x4) = 0x3421ffff;
				break;
			}
			case 2: { // Off
				*(u32*)float_dist = 0x3c010000;
				*(u32*)(float_dist + 0x4) = 0x34210000;
				break;	
			}
		}
		patched = 1;
	}
}

/*
 * NAME :		onGameplayLoad_miscRespawnTimers
 * 
 * DESCRIPTION :
 *              Updates Various respawn timers for items (Health Boxes, Weapon Crates and Ammo Packs)
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
		if (gameConfig.grRespawnTimer_AmmoPickups > 0) {
			switch (moby->OClass) {
				case MOBY_ID_AMMO_PACK_GRAVITY_BOMB:
				case MOBY_ID_AMMO_PACK_BLITZ:
				case MOBY_ID_AMMO_PACK_FLUX:
				case MOBY_ID_AMMO_PACK_ROCKET_TUBE:
				case MOBY_ID_AMMO_PACK_MINE:
				case MOBY_ID_AMMO_PACK_LAVA_GUN:
				case MOBY_ID_AMMO_PACK_HOLOSIELD:
				case MOBY_ID_AMMO_PACK_N60:
				case MOBY_ID_CHARGEBOOTS_PICKUP: {
					GameplayPVarDef_t* PVarOffset = (GameplayPVarDef_t*)(PVarOffsetPtr + (u32)(moby->PVarIndex * 8));
					AmmoPickupVars_t* data = (AmmoPickupVars_t*)((u32)PVarDataPtr + (u32)PVarOffset->Offset);
					data->respawnTime = (gameConfig.grRespawnTimer_AmmoPickups - 1) * 5;
				}
			}
		}
		if (gameConfig.grRespawnTimer_WeaponCrates > 0) {
			switch (moby->OClass) {
				case MOBY_ID_CRATE_CHARGEBOOTS:
				case MOBY_ID_CRATE_GRAVITY_BOMB:
				case MOBY_ID_CRATE_ROCKET_TUBE:
				case MOBY_ID_CRATE_FLUX:
				case MOBY_ID_CRATE_BLITZ:
				case MOBY_ID_CRATE_LAVA_GUN:
				case MOBY_ID_CRATE_HOLOSHIELD:
				case MOBY_ID_CRATE_MORPH_O_RAY:
				case MOBY_ID_CRATE_MINE:
				case MOBY_ID_CRATE_RANDOM_PICKUP: {
					GameplayPVarDef_t* PVarOffset = (GameplayPVarDef_t*)(PVarOffsetPtr + (u32)(moby->PVarIndex * 8));
					u32 data = PVarDataPtr + PVarOffset->Offset;
					*(int*)(data + 0x70) = (gameConfig.grRespawnTimer_WeaponCrates - 1) * 5;
				}
			}
		}
	}
}

/*
 * NAME :		runInvincibilityTimer
 * 
 * DESCRIPTION :
 *              Adds a small invincibility timer when a player respawns.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int runInvincibilityTimer(Player* player, int a1)
{
	static u32 PlayerInvincibleTimer_Func = 0;
	int hurtPlayer = a1;
	// if timer is greater than zero, player can't be hurt.
	if (player->timers.unkTimer_346 > 0) {
		timeDecTimerShort(&player->timers.unkTimer_346);
		hurtPlayer = 0;
	}
	// if previous player state was resurrecting, start timer.
	if (playerDeobfuscate(&player->PreviousState, 0, 0) == PLAYER_STATE_WAIT_FOR_RESURRECT)
		player->timers.unkTimer_346 = 0x78;

	// run base function with our a1
	if (!PlayerInvincibleTimer_Func)
		PlayerInvincibleTimer_Func = GetAddress(&vaPlayerInvincibleTimer_Func);
	
	DPRINTF("\nhurtPlayer: %d", hurtPlayer);
	return ((int (*)(Player*, int))PlayerInvincibleTimer_Func)(player, hurtPlayer);
}
void respawnInvincTimer(void)
{
	static int patched = 0;
	if (patched)
		return;

	HOOK_JAL(GetAddress(&vaPlayerInvincibleTimer_Hook), &runInvincibilityTimer);
	patched = 1;
}
