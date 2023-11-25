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
    VariableAddress_t vaRespawnTimerFunc = {
#if UYA_PAL
        .Lobby = 0,
        .Bakisi = 0x004e3690,
        .Hoven = 0x004e57a8,
        .OutpostX12 = 0x004db080,
        .KorgonOutpost = 0x004d8818,
        .Metropolis = 0x004d7b68,
        .BlackwaterCity = 0x004d5400,
        .CommandCenter = 0x004d53c8,
        .BlackwaterDocks = 0x004d7c48,
        .AquatosSewers = 0x004d6f48,
        .MarcadiaPalace = 0x004d68c8,
#else
        .Lobby = 0,
        .Bakisi = 0x004e0fd0,
        .Hoven = 0x004e3028,
        .OutpostX12 = 0x004D8940,
        .KorgonOutpost = 0x004d6158,
        .Metropolis = 0x004d54a8,
        .BlackwaterCity = 0x004d2cc0,
        .CommandCenter = 0x004d2e48,
        .BlackwaterDocks = 0x004d5688,
        .AquatosSewers = 0x004d49c8,
        .MarcadiaPalace = 0x004d4308,
#endif
    };

    VariableAddress_t vaSpawnWeaponPackFunc = {
#if UYA_PAL
        .Lobby = 0,
        .Bakisi = 0x004fb188,
        .Hoven = 0x004fd2a0,
        .OutpostX12 = 0x004f2b78,
        .KorgonOutpost = 0x004f0310,
        .Metropolis = 0x004ef660,
        .BlackwaterCity = 0x004ecef8,
        .CommandCenter = 0x004ecec0,
        .BlackwaterDocks = 0x004ef740,
        .AquatosSewers = 0x004eea40,
        .MarcadiaPalace = 0x004ee3c0,
#else
        .Lobby = 0,
        .Bakisi = 0x004f8a08,
        .Hoven = 0x004faa60,
        .OutpostX12 = 0x004f0378,
        .KorgonOutpost = 0x004edb90,
        .Metropolis = 0x004ecee0,
        .BlackwaterCity = 0x004ea6f8,
        .CommandCenter = 0x004ea880,
        .BlackwaterDocks = 0x004ed0c0,
        .AquatosSewers = 0x004ec400,
        .MarcadiaPalace = 0x004ebd40,
#endif
    };

    // Run Original Respawn Timer Hook
	((void (*)(int, int, int))GetAddress(&vaRespawnTimerFunc))(a0, a1, a2);
	
	// if pack already spawned, don't spawn more.
	if (SpawnedPack == 1)
		return;
	// set player to register v1's value.
	register int player asm("s3");
	// Spawn Pack
	((void (*)(u32))GetAddress(&vaSpawnWeaponPackFunc))(player);
	// It now spawned pack, so set to true.
	SpawnedPack = 1;
}

void spawnWeaponPackOnDeath(void)
{
    VariableAddress_t vaRespawnTimerHook = {
#if UYA_PAL
        .Lobby = 0,
        .Bakisi = 0x004b00f4,
        .Hoven = 0x004b220c,
        .OutpostX12 = 0x004a7ae4,
        .KorgonOutpost = 0x004a527c,
        .Metropolis = 0x004a45cc,
        .BlackwaterCity = 0x004a1e64,
        .CommandCenter = 0x004a1e5c,
        .BlackwaterDocks = 0x004a46dc,
        .AquatosSewers = 0x004a39dc,
        .MarcadiaPalace = 0x004a335c,
#else
        .Lobby = 0,
        .Bakisi = 0x004adba4,
        .Hoven = 0x004afbfc,
        .OutpostX12 = 0x004a5514,
        .KorgonOutpost = 0x004a2d2c,
        .Metropolis = 0x004a207c,
        .BlackwaterCity = 0x0049f894,
        .CommandCenter = 0x0049fa4c,
        .BlackwaterDocks = 0x004a228c,
        .AquatosSewers = 0x004a15cc,
        .MarcadiaPalace = 0x004a0f0c,
#endif
    };

    // Disable normal Weapon Pack spawns
    disableWeaponPacks();

    // Hook CTF/Siege SpawnPack
    if (*(u32*)GetAddress(&vaRespawnTimerHook) != (0x0C000000 | ((u32)(&SpawnPack) >> 2)))
        HOOK_JAL(GetAddress(&vaRespawnTimerHook), &SpawnPack);
    
    // Hook DM SpawnPack
    if (*(u32*)((u32)GetAddress(&vaRespawnTimerHook) + 0x2100) != (0x0C000000 | ((u32)(&SpawnPack) >> 2)))
        HOOK_JAL(((u32)GetAddress(&vaRespawnTimerHook) + 0x2100), &SpawnPack);

    // if Health is greater than zero and pack has spawned
    // This will be checking constantly, instead of just when the player dies.
    Player ** players = playerGetAll();
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
    	if (!players[i])
    		return;

		Player * player = players[i];
		if (player->IsLocal && playerGetHealth(player) > 0)
            SpawnedPack = 0;
	}
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
VariableAddress_t vaCheckWeaponKill = {
#ifdef UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00544488,
	.Hoven = 0x00546650,
	.OutpostX12 = 0x0053bf28,
	.KorgonOutpost = 0x00539610,
	.Metropolis = 0x00538a910,
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
VariableAddress_t vaUpdateWeaponKill = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004fb448,
	.Hoven = 0x004fd560,
	.OutpostX12 = 0x004f2e38,
	.KorgonOutpost = 0x004f05d0,
	.Metropolis = 0x004ef920,
	.BlackwaterCity = 0x004ed1b8,
	.CommandCenter = 0x004ed180,
	.BlackwaterDocks = 0x004efa00,
	.AquatosSewers = 0x004eed00,
	.MarcadiaPalace = 0x004ee680,
#else
	.Lobby = 0,
	.Bakisi = 0x004f8cc8,
	.Hoven = 0x004fad20,
	.OutpostX12 = 0x004f0638,
	.KorgonOutpost = 0x004ede50,
	.Metropolis = 0x004ed1a0,
	.BlackwaterCity = 0x004ea9b8,
	.CommandCenter = 0x004eab40,
	.BlackwaterDocks = 0x004ed380,
	.AquatosSewers = 0x004ec6c0,
	.MarcadiaPalace = 0x004ec000,
#endif
};
void v2_logic(void)
{
	Player ** players = playerGetAll();
	int i, j;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		Player * player = players[i];
		if (!player)
			return;

		for(j = 1; j < 10; ++j)
			playerGiveWeaponUpgrade(player, j);
	}
}
void v2_Setting(int setting, int FirstPass)
{
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
		}
	}
	// Always V2's
	else {
		// hook v2 logic ad end of give weapon function.
		int GiveWeapon_JRRA = (u32)GetAddress(&vaGiveWeaponFunc) + 0x538;
		if (*(u32*)GiveWeapon_JRRA == 0x03e00008)
			HOOK_J(GiveWeapon_JRRA, &v2_logic);
		if (FirstPass)
			v2_logic();
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
	VariableAddress_t vaDM_PressXToRespawn = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x004b22b0,
		.Hoven = 0x004b43c8,
		.OutpostX12 = 0x004a9ca0,
		.KorgonOutpost = 0x004a7438,
		.Metropolis = 0x004a6788,
		.BlackwaterCity = 0x004a4020,
		.CommandCenter = 0x004a4018,
		.BlackwaterDocks = 0x004a6898,
		.AquatosSewers = 0x004a5b98,
		.MarcadiaPalace = 0x004a5518,
#else
		.Lobby = 0,
		.Bakisi = 0x004afd60,
		.Hoven = 0x004b1db8,
		.OutpostX12 = 0x004a76d0,
		.KorgonOutpost = 0x004a4ee8,
		.Metropolis = 0x004a4238,
		.BlackwaterCity = 0x004a1a50,
		.CommandCenter = 0x004a1c08,
		.BlackwaterDocks = 0x004a4448,
		.AquatosSewers = 0x004a3788,
		.MarcadiaPalace = 0x004a30c8,
#endif
	};
	//GameOptions * gameOptions = (GameOptions*)0x002417C8;
	//gameOptions->GameFlags.MultiplayerGameFlags.Nodes
	// Siege & CTF: Press X to Respawn
	// *(u32*)0x004A55C8 = (0x0C000000 | ((u32)(&RespawnPlayer) >> 2));

	// DM: Press X To Respawn JAL
	// Freezes in Siege and CTF due to needing to choose nodes, even if nodes are off.
	int hook = GetAddress(&vaDM_PressXToRespawn);
	int jal = (0x0C000000 | ((u32)(&RespawnPlayer) >> 2));
	if (*(u32*)hook != jal)
		*(u32*)hook = jal;

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
    static int MultiplyBy[] = {.5, 1.5, 2, 3, 4};
    int init = 0;
    Moby * moby = mobyListGetStart();
    // Iterate through mobys and change health
    while ((moby = mobyFindNextByOClass(moby, MOBY_ID_GATTLING_TURRET))) {
        if (moby->PVar) {
			// Gattling Turret Health is stored as a float and as it's
			// hexidecimal value.  We use the hex value and multiply it
			// by our wanted value, then store it as it's float health
			int HexHealth = ((u32)moby->PVar + 0x34);
			int NewHealth = *(int*)HexHealth * (int)MultiplyBy[value - 1];
			// if set to .5 health
			if ((value - 1) == 0)
				NewHealth = 150;

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
	VariableAddress_t vaNodeTurret_UpdateFunc = {
	#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x0041ef60,
		.Hoven = 0x004209b8,
		.OutpostX12 = 0x004178e8,
		.KorgonOutpost = 0x00414f58,
		.Metropolis = 0x004147e8,
		.BlackwaterCity = 0x00410650,
		.CommandCenter = 0x0041acc0,
		.BlackwaterDocks = 0x0041d510,
		.AquatosSewers = 0x0041c828,
		.MarcadiaPalace = 0x0041c190,
	#else
		.Lobby = 0,
		.Bakisi = 0x0041e6f8,
		.Hoven = 0x00420088,
		.OutpostX12 = 0x00416fd0,
		.KorgonOutpost = 0x004146e8,
		.Metropolis = 0x00413f80,
		.BlackwaterCity = 0x0040fda0,
		.CommandCenter = 0x0041a488,
		.BlackwaterDocks = 0x0041ccc0,
		.AquatosSewers = 0x0041bff0,
		.MarcadiaPalace = 0x0041b940,
	#endif
	};
	u32 UpdateFunc = GetAddress(&vaNodeTurret_UpdateFunc);
	if (*(u32*)UpdateFunc == 0x27bdffe0) {
		*(u32*)UpdateFunc = 0x03e0008;
		*(u32*)((u32)UpdateFunc + 0x4) = 0;
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
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
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
	VariableAddress_t vaCameraShakeFunc = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00446eb0,
		.Hoven = 0x00448a30,
		.OutpostX12 = 0x0043f830,
		.KorgonOutpost = 0x0043d3f0,
		.Metropolis = 0x0043c730,
		.BlackwaterCity = 0x00439f30,
		.CommandCenter = 0x0043abb0,
		.BlackwaterDocks = 0x0043d430,
		.AquatosSewers = 0x0043c730,
		.MarcadiaPalace = 0x0043c0b0,
#else
		.Lobby = 0,
		.Bakisi = 0x00446010,
		.Hoven = 0x00447ad0,
		.OutpostX12 = 0x0043e910,
		.KorgonOutpost = 0x0043c550,
		.Metropolis = 0x0043b890,
		.BlackwaterCity = 0x00439010,
		.CommandCenter = 0x0043c690,
		.BlackwaterDocks = 0x0043c690,
		.AquatosSewers = 0x0043b9d0,
		.MarcadiaPalace = 0x0043b310,
#endif
	};
	int CameraShake = GetAddress(&vaCameraShakeFunc);
	if (*(u32*)CameraShake == 0x24030460)
	{
		*(u32*)CameraShake = 0x03e00008;
		*(u32*)(CameraShake + 0x4) = 0;
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
    VariableAddress_t vaDM_RespawnUpdater = {
#if UYA_PAL
        .Lobby = 0,
        .Bakisi = 0x004b21f4,
        .Hoven = 0x004b430c,
        .OutpostX12 = 0x004a9be4,
        .KorgonOutpost = 0x004a737c,
        .Metropolis = 0x004a66cc,
        .BlackwaterCity = 0x004a3f64,
        .CommandCenter = 0x004a3f5c,
        .BlackwaterDocks = 0x004a67dc,
        .AquatosSewers = 0x004a5adc,
        .MarcadiaPalace = 0x004a545c,
#else
        .Lobby = 0,
        .Bakisi = 0x004afca4,
        .Hoven = 0x004b1cfc,
        .OutpostX12 = 0x004a7614,
        .KorgonOutpost = 0x004a4e2c,
        .Metropolis = 0x004a417c,
        .BlackwaterCity = 0x004a1994,
        .CommandCenter = 0x004a1b4c,
        .BlackwaterDocks = 0x004a438c,
        .AquatosSewers = 0x004a36cc,
        .MarcadiaPalace = 0x004a300c,
#endif
    };
	// Disable Timer and respawn text.
    int RespawnUpdater = GetAddress(&vaDM_RespawnUpdater);
    if (*(u32*)RespawnUpdater != 0)
        *(u32*)RespawnUpdater = 0;

	// Disable Respawn Function
	int RespawnFunc = GetAddress(&vaPlayerRespawnFunc);
    if (*(u32*)RespawnFunc != 0)
	{
		*(u32*)RespawnFunc = 0x03e00008;
        *(u32*)(RespawnFunc + 0x4) = 0;
	}
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
			if (gameData->PlayerStats[i].Deaths > PlayerDeaths[i]) {
				PlayerDeaths[i] = gameData->PlayerStats[i].Deaths;
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
			if (gameData->PlayerStats[i].Deaths > PlayerDeaths[i]) {
				PlayerDeaths[i] = gameData->PlayerStats[i].Deaths;
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
				if (gameData->PlayerStats[i].Deaths == 0) {
					gameData->TimeEnd = 0;
					gameData->WinningTeam = i;
				}
			}
		}
	}
}

/*
 * NAME :		setRespawnTimer
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
void setRespawnTimer(void)
{
	VariableAddress_t vaRespawnTimerFunc = {
	// Uses the start of the Respawn Timer Function
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x003f6098,
		.Hoven = 0x003f4f20,
		.OutpostX12 = 0x003ece18,
		.KorgonOutpost = 0x003eccd8,
		.Metropolis = 0x003eadb8,
		.BlackwaterCity = 0x003e7ba0,
		.CommandCenter = 0x003f6a88,
		.BlackwaterDocks = 0x003f89e8,
		.AquatosSewers = 0x003f85f0,
		.MarcadiaPalace = 0x003f6b88,
#else
		.Lobby = 0,
		.Bakisi = 0x003f5ba8,
		.Hoven = 0x003f49b0,
		.OutpostX12 = 0x003ec8a8,
		.KorgonOutpost = 0x003ec7c8,
		.Metropolis = 0x003ea8c8,
		.BlackwaterCity = 0x003e7650,
		.CommandCenter = 0x003f6580,
		.BlackwaterDocks = 0x003f84e0,
		.AquatosSewers = 0x003f80e8,
		.MarcadiaPalace = 0x003f6680,
#endif
	};
    int RespawnAddr = GetAddress(&vaRespawnTimerFunc);
	if (gameConfig.grRespawnTimer)
	{
	    int Seconds;
		switch (gameConfig.grRespawnTimer) {
			case 10:
			case 11:
				Seconds = gameConfig.grRespawnTimer - 10; break;
			default:
				Seconds = gameConfig.grRespawnTimer + 1; break;
		}
        int RespawnTime = Seconds * GAME_FPS;
        
		// Set Default Respawn Timer
        if (*(u16*)(RespawnAddr + 0x10) != RespawnTime)
		    *(u16*)(RespawnAddr + 0x10) = RespawnTime;

		// Set Penalty Timer #1 based off of current respawn timer.
		if (!gameConfig.grDisablePenaltyTimers)
			*(u16*)(RespawnAddr + 0x78) = (Seconds + 1.5) * GAME_FPS;
	}
	if (gameConfig.grDisablePenaltyTimers)
	{
		// Jump to end of function after respawn timer is set.
		if (*(u32*)(RespawnAddr + 0x28) == 0x14440003)
			*(u32*)(RespawnAddr + 0x28) = 0x1000001A;

		// Gatlin Turret Destroyed (RespawnTime + This)
		// *(u16*)(RespawnAddr + 0x80) = RespawnTime;
		// Anti-Air Turret Destroyed (RespawnTime + This)
		// *(u16*)(RespawnAddr + 0x8c) = RespawnTime;
	}
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
	VariableAddress_t vaPostHitInvinc = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x005274b4,
		.Hoven = 0x005295cc,
		.OutpostX12 = 0x0051eea4,
		.KorgonOutpost = 0x0051c63c,
		.Metropolis = 0x0051b98c,
		.BlackwaterCity = 0x00519224,
		.CommandCenter = 0x00518fe4,
		.BlackwaterDocks = 0x0051b864,
		.AquatosSewers = 0x0051ab64,
		.MarcadiaPalace = 0x0051a4e4,
#else
		.Lobby = 0,
		.Bakisi = 0x00524c34,
		.Hoven = 0x00526c8c,
		.OutpostX12 = 0x0051c5a4,
		.KorgonOutpost = 0x00519dbc,
		.Metropolis = 0x0051910c,
		.BlackwaterCity = 0x00516924,
		.CommandCenter = 0x005168a4,
		.BlackwaterDocks = 0x005190e4,
		.AquatosSewers = 0x00518424,
		.MarcadiaPalace = 0x00517d64,
#endif
	};
	// PAL: 0x27, NTSC: 0x2f
	int time = GetAddress(&vaPostHitInvinc);
	*(u32*)(time) = 0x24020001;
	*(u32*)(time + 0x308) = 0x24020001;
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
	float size, cameraHeight, tpHeight, moveSpeed = 1;
	
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
	float size, cameraHeight, tpHeight, moveSpeed = 1;
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
	VariableAddress_t vaHealthbars_Hook = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00507c70,
		.Hoven = 0x00509d88,
		.OutpostX12 = 0x004ff660,
		.KorgonOutpost = 0x004fcdf8,
		.Metropolis = 0x004fc148,
		.BlackwaterCity = 0x004f99e0,
		.CommandCenter = 0x004f99a8,
		.BlackwaterDocks = 0x004fc228,
		.AquatosSewers = 0x004fb528,
		.MarcadiaPalace = 0x004faea8,
#else
		.Lobby = 0,
		.Bakisi = 0x00505480,
		.Hoven = 0x005074d8,
		.OutpostX12 = 0x004fcdf0,
		.KorgonOutpost = 0x004fa608,
		.Metropolis = 0x004f9958,
		.BlackwaterCity = 0x004f7170,
		.CommandCenter = 0x004f72f8,
		.BlackwaterDocks = 0x004f9b38,
		.AquatosSewers = 0x004f8e78,
		.MarcadiaPalace = 0x004f87b8,
#endif
	};
	u32 hook = GetAddress(&vaHealthbars_Hook);
	if (hook)
		HOOK_JAL(hook, &healthbars_Logic);
}

VariableAddress_t vaRadarBlips_FloatVal = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004aec24,
    .Hoven = 0x004b0d3c,
    .OutpostX12 = 0x004a6614,
    .KorgonOutpost = 0x004a3dac,
    .Metropolis = 0x004a30fc,
    .BlackwaterCity = 0x004a0994,
    .CommandCenter = 0x004a098c,
    .BlackwaterDocks = 0x004a320c,
    .AquatosSewers = 0x004a250c,
    .MarcadiaPalace = 0x004a1e8c,
#else
    .Lobby = 0,
    .Bakisi = 0x004ac6d4,
    .Hoven = 0x004ae72c,
    .OutpostX12 = 0x004a4044,
    .KorgonOutpost = 0x004a185c,
    .Metropolis = 0x004a0bac,
    .BlackwaterCity = 0x0049e3c4,
    .CommandCenter = 0x0049e57c,
    .BlackwaterDocks = 0x004a0dbc,
    .AquatosSewers = 0x004a00fc,
    .MarcadiaPalace = 0x0049fa3c,
#endif
};
void radarBlips(void)
{
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
	}
}
