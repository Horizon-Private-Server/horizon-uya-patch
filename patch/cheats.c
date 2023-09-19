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

extern PlayerKills[GAME_MAX_PLAYERS];
extern PlayerDeaths[GAME_MAX_PLAYERS];
extern PatchGameConfig_t gameConfig;
extern VariableAddress_t vaPlayerRespawnFunc;

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
int SpawnedPack[2] = {0,0};
void SpawnPack(int a0, int a1, int a2, int a3)
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
	((void (*)(int, int, int, int))GetAddress(&vaRespawnTimerFunc))(a0, a1, a2, a3);

    // Spawn Pack if Health <= zero and if not spawned already.
    // get all players
    Player ** players = playerGetAll();
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
    	if (!players[i])
    		continue;

		Player * player = players[i];
        // if player health is zero, and pack hasn't spawned, spawn pack
		if (playerGetHealth(player) <= 0 && playerIsLocal(player) && SpawnedPack[i] == 0)
		{
            // Spawn Pack
            ((void (*)(u32))GetAddress(&vaSpawnWeaponPackFunc))(player);
            // It now spawned pack, so set to true.
            SpawnedPack[i] = 1;
        }
	}
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
    		continue;

		Player * player = players[i];
		if (playerGetHealth(player) > 0 && playerIsLocal(player))
		{
            SpawnedPack[i] = 0;
		}
	}
}

/*
 * NAME :		disableV2s
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
void disableV2s(void)
{
    VariableAddress_t vaWeaponMeterAddress = {
#ifdef UYA_PAL
        .Lobby = 0,
        .Bakisi = 0x00544508,
        .Hoven = 0x005466d0,
        .OutpostX12 = 0x0053bfa8,
        .KorgonOutpost = 0x00539690,
        .Metropolis = 0x00538a90,
        .BlackwaterCity = 0x00536278,
        .CommandCenter = 0x00535ad0,
        .BlackwaterDocks = 0x00538350,
        .AquatosSewers = 0x00537650,
        .MarcadiaPalace = 0x00536fd0,
#else
        .Lobby = 0,
        .Bakisi = 0x00541bf8,
        .Hoven = 0x00543d00,
        .OutpostX12 = 0x00539618,
        .KorgonOutpost = 0x00536d80,
        .Metropolis = 0x00536180,
        .BlackwaterCity = 0x005338e8,
        .CommandCenter = 0x00533318,
        .BlackwaterDocks = 0x00535b58,
        .AquatosSewers = 0x00534e98,
        .MarcadiaPalace = 0x005347d8,
#endif
    };
    u32 addr = GetAddress(&vaWeaponMeterAddress);
    if (*(u32*)addr == 0x10400003) // beq v0, zero
		*(u32*)addr = 0x10000003;
}

/*
 * NAME :		disableHealthboxes
 * 
 * DESCRIPTION :
 *              Removes all health boxes from the map.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
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

int setGattlingTurretHealth(int value)
{
    int MultiplyBy[] = {.5, 1.5, 2, 3, 4};
    int init = 0;
    Moby * moby = mobyListGetStart();
    // Iterate through mobys and change health
    while ((moby = mobyFindNextByOClass(moby, MOBY_ID_GATTLING_TURRET)))
    {
        if (moby->PVar)
        {
			// Gattling Turret Health is stored as a float and as it's
			// hexidecimal value.  We use the hex value and multiply it
			// by our wanted value, then store it as it's float health
			int HexHealth = ((u32)moby->PVar + 0x34);
            int NewHealth = (u32)(*(u32*)HexHealth * MultiplyBy[value - 1]);
			*(u32*)HexHealth = NewHealth;
			*(float*)((u32)moby->PVar + 0x30) = NewHealth;
        }
        ++moby; // next moby
    }
    init = 1;
    return init;
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
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void vampireLogic(int healRate)
{
	int i;
	Player ** playerObjects = playerGetAll();
	Player * player;
	GameData * gameData = gameGetData();

	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
		// Check if player has killed someone
		if (gameData->PlayerStats[i].Kills > PlayerKills[i])
		{
			// Try to heal if player exists
			player = playerObjects[i];
			if (player)
                playerSetHealth(player, clamp(((int)player->pNetPlayer->pNetPlayerData->hitPoints + healRate), 0, PLAYER_MAX_HEALTH));

			// Update our cached kills count
			PlayerKills[i] = gameData->PlayerStats[i].Kills;
		}
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
 * 
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
    // Don't let players respawn
    disableRespawning();

    static int DeadPlayers = 0;
    int i;
	GameData * gameData = gameGetData();
    GameSettings * gameSettings = gameGetSettings();
    int playerCount = gameSettings->PlayerCount;
    for (i = 0; i < playerCount; ++i)
    {
        // Save current deaths for all players, and how many players have died.
        if (gameData->PlayerStats[i].Deaths > PlayerDeaths[i])
        {
            PlayerDeaths[i] = gameData->PlayerStats[i].Deaths;
            ++DeadPlayers;
        }

        // If only one player in game, don't let game end until they die.
        if (playerCount == 1 && DeadPlayers == 1)
        {
			gameData->TimeEnd = 0;
            gameData->WinningTeam = i;
        }
        // if player count is greater than 1, and Dead Players == Player Count - 1
        else if (playerCount > 1 && DeadPlayers == (playerCount - 1))
        {
            // Check to see who has not died
            if (gameData->PlayerStats[i].Deaths == 0)
            {
				gameData->TimeEnd = 0;
                gameData->WinningTeam = i;
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
#if UYA_PAL
        static int FPS = 50;
#else
        static int FPS = 60;
#endif
        int Seconds = gameConfig.grRespawnTimer - 1;
        int RespawnTime = Seconds * FPS;
        
		// Set Default Respawn Timer
        if (*(u16*)(RespawnAddr + 0x10) != RespawnTime)
		    *(u16*)(RespawnAddr + 0x10) = RespawnTime;

		// Set Penalty Timer #1 based off of current respawn timer.
		if (!gameConfig.grDisablePenaltyTimers)
			*(u16*)(RespawnAddr + 0x78) = (Seconds + 1.5) * FPS;
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
 * NAME :		disableDrones
 * 
 * DESCRIPTION :
 *              Destroys the drones.
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
 void onGameplayLoad_disableDrones(GameplayHeaderDef_t * gameplay)
{
	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);

	// iterate each moby, moving all pickups to below the map
	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t* droneConfig = &mobyInstancesHeader->MobyInstances[i];
		if (droneConfig->OClass == MOBY_ID_DRONE_BOT_CLUSTER_CONFIG) {
			droneConfig->PosY = -100;
		}
		// GameplayMobyDef_t* dronePlayerConfig = &mobyInstancesHeader->MobyInstances[i];
		// if (dronePlayerConfig->OClass == MOBY_ID_DRONE_BOT_CLUSTER_UPDATER) {
		// 	dronePlayerConfig->PosY = -200;
		// }
		// GameplayMobyDef_t* drones = &mobyInstancesHeader->MobyInstances[i];
		// if (drones->OClass == MOBY_ID_DRONE_BOT) {
		// 	drones->PosY = -100;
		// }
	}
}
// void disableDrones(void)
// {
// 	// Moby * a = mobyListGetStart();
// 	// // Remove drone cluster update function. (this is for main configuration)
// 	// while ((a = mobyFindNextByOClass(a, MOBY_ID_DRONE_BOT_CLUSTER_CONFIG))) {
// 	// 	a->PUpdate = 0;
// 	// 	mobyDestroy(a);
// 	// 	++a;
// 	// }
// 	// Moby * b = mobyListGetStart();
// 	// // Remove drone update function. (This is for the player activator)
// 	// while ((b = mobyFindNextByOClass(b, MOBY_ID_DRONE_BOT_CLUSTER_UPDATER))) {
// 	// 	b->PUpdate = 0;
// 	// 	mobyDestroy(b);
// 	// 	++b;
// 	// }
// 	Moby * c = mobyListGetStart();
// 	while ((c = mobyFindNextByOClass(c, MOBY_ID_DRONE_BOT))) {
// 		c->PVar = 0;
// 		c->PUpdate = 0;
// 		c->Scale = 0;
// 		memset(c->BSphere, 0, sizeof(c->BSphere));
// 		//memset(c->Position, 0, sizeof(c->Position));
// 		++c;
// 	}
// }

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
