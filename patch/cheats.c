/***************************************************
 * FILENAME :		cheats.c
 * DESCRIPTION :
 * 		Contains simple cheats/game settings for UYA.
 * NOTES :
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
#include <libuya/math.h>
#include <libuya/math3d.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include <libuya/time.h>
#include <libuya/interop.h>
#include <libuya/net.h>
#include <libuya/moby.h>
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

Tweaker_GravityBomb_t gBombTweaker[] = {
	{12, 0.00001, 12, 15}, // UYA Default
	{16, 0.00001, 16, 24} // DL Default
};

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
extern VariableAddress_t vaPlayerRespawnFunc;
extern VariableAddress_t vaGiveWeaponFunc;
extern VariableAddress_t vaGiveWeaponUpgradeFunc;

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
				playerGiveWeaponUpgrade(player, playerDeobfuscate(&slot[j], 1, 1));
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
 * NAME :		deleteSiegeNodeTurret
 * DESCRIPTION :
 *              
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int deleteSiegeNodeTurrets(void)
{
    int init = 0;
    Moby * moby = mobyListGetStart();
    while ((moby = mobyFindNextByOClass(moby, MOBY_ID_SIEGE_NODE))) {
        if (moby->pUpdate)
			*(u32*)((u32)moby->pUpdate + 0x810) = 0;

        ++moby; // next moby
    }
    init = 1;
    return init;
}
void deleteNodeTurretsUpdate(void)
{
	if (patched.gameConfig.grNoBaseDefense_SmallTurrets)
		return;

	u32 UpdateFunc = GetAddress(&vaNodeTurret_UpdateFunc);
	if (*(u32*)UpdateFunc == 0x27bdffe0) {
		*(u32*)UpdateFunc = 0x03e0008;
		*(u32*)((u32)UpdateFunc + 0x4) = 0;
		patched.gameConfig.grNoBaseDefense_SmallTurrets = 1;
	}
}

/*
 * NAME :		ChargebootForever
 * DESCRIPTION :
 *              Lets Player keep Chargebooting if R2 is held.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void chargebootForever(void)
{
	int i;
	Player** players = playerGetAll();
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		Player* player = players[i];
		if (!player)
			continue;
		// if Is Chargebooting and player is holding R2 and player's state timer is greater than 55
		if (player->timers.noFpsCamTimer == 1 && playerPadGetButton(player, PAD_R2) > 0 && player->timers.state > 55)
			player->timers.state = 55;
	}
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
void setRespawnTimer_Player(void)
{
	Player *player = playerGetFromSlot(0);
    int RespawnAddr = GetAddress(&vaRespawnTimerFunc_Player);
	if (gameConfig.grRespawnTimer_Player || gameConfig.grSuicidePenaltyTimer) {
	    int Seconds = 0;
		if (gameConfig.grRespawnTimer_Player) {
			switch (gameConfig.grRespawnTimer_Player) {
				case 1: Seconds = 2; break;
				case 2: Seconds = 2.5; break;
				case 11: Seconds = 0; break;
				case 12: Seconds = 1; break;
				default:
					Seconds += gameConfig.grRespawnTimer_Player; break;
			}
		}
		if (gameConfig.grSuicidePenaltyTimer && playerIsDead(player) && player->lastDeathWasSuicide) {
			Seconds += gameConfig.grSuicidePenaltyTimer;
		}
        int RespawnTime = Seconds * GAME_FPS;
        
		// Set Default Respawn Timer
        if (*(u16*)(RespawnAddr + 0x10) != RespawnTime)
		    *(u16*)(RespawnAddr + 0x10) = RespawnTime;

		// Set Penalty Timer #1 based off of current respawn timer.
		if (!gameConfig.grDisablePenaltyTimers)
			*(u16*)(RespawnAddr + 0x78) = (Seconds + 1.5) * GAME_FPS;
	}
	if (gameConfig.grDisablePenaltyTimers && !patched.gameConfig.grDisablePenaltyTimers) {
		// Jump to end of function after respawn timer is set.
		if (*(u32*)(RespawnAddr + 0x28) == 0x14440003)
			*(u32*)(RespawnAddr + 0x28) = 0x1000001A;

		// Gatlin Turret Destroyed (RespawnTime + This)
		// *(u16*)(RespawnAddr + 0x80) = RespawnTime;
		// Anti-Air Turret Destroyed (RespawnTime + This)
		// *(u16*)(RespawnAddr + 0x8c) = RespawnTime;
		patched.gameConfig.grDisablePenaltyTimers = 1;
	}
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
 * NAME :		keepBaseHealthPadActive
 * DESCRIPTION :
 *              Keeps Base Ammo/Health Pads Active even if Gatlin Turrets are destroyed.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int keepBaseHealthPadActive(void)
{
	int init = 0;
	Moby * a = mobyListGetStart();
	// if moby list finds the health pad
	while ((a = mobyFindNextByOClass(a, MOBY_ID_HEALTH_PAD))) {
		if (a->pUpdate) {
			*(u32*)((u32)a->pUpdate + 0x68) = 0x24020001;
			*(u32*)((u32)a->pUpdate + 0x8C) = 0x24150001;
		}
		++a;
	}
	Moby * b = mobyListGetStart();
	// if moby list finds the ammo pad
	while ((b = mobyFindNextByOClass(b, MOBY_ID_AMMO_PAD))) {
		if (b->pUpdate) {
			*(u32*)((u32)b->pUpdate + 0x74) = 0x24020001;
			*(u32*)((u32)b->pUpdate + 0x90) = 0x24020001;
		}
		++b;
	}
	init = 1;
	return init;
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
 * NAME :		healthbar_Logic
 * DESCRIPTION :
 *              Shows a healthbar above the players name.
 * NOTES :
 * ARGS : 
 * RETURN :
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
				float w = (0.05 * 1) + 0.02;
				float h = 0.02;
				float p = 0.002;
				float right = w * health;
				x -= w / 2;
				gfxScreenSpaceBox(x, y, w - p, h - p, 0x80000000);
				gfxScreenSpaceBox(x, y, right, h, nameColor);
			}
		}
	}
}
/*
 * NAME :		healthbars
 * DESCRIPTION :
 *              Hooks healthbars_Logic
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void healthbars(void)
{
	if (patched.gameConfig.grHealthBars)
		return;

	HOOK_JAL(GetAddress(&vaHealthbars_Hook), &healthbars_Logic);
	patched.gameConfig.grHealthBars = 1;
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
		switch (gameConfig.grRadarBlipsDistance) {
			case 1: { // 2x
				*(u32*)float_dist = 0x3c014519;
				*(u32*)(float_dist + 0x4) = 0x34210000;
				break;	
			}
			case 2: { // 4x
				*(u32*)float_dist = 0x3c014599;
				*(u32*)(float_dist + 0x4) = 0x34210000;
				break;	
			}
			case 3: { // Always
				*(u32*)float_dist = 0x3c017fff;
				*(u32*)(float_dist + 0x4) = 0x3421ffff;
				break;
			}
			case 4: { // Off
				*(u32*)float_dist = 0x3c010000;
				*(u32*)(float_dist + 0x4) = 0x34210000;
				break;	
			}
			case 5: { // .5x
				*(u32*)float_dist = 0x3c014419;
				*(u32*)(float_dist + 0x4) = 0x34210000;
				break;	
			}
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

/*
 * NAME :		runInvincibilityTimer
 * DESCRIPTION :
 *              Adds a small invincibility timer when a player respawns.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int runInvincibilityTimer(Player* player, int a1)
{
	int hurtPlayer = a1;
	// if timer is greater than zero, player can't be hurt.
	if (player->timers.unkTimer_346 > 0) {
		timeDecTimerShort(&player->timers.unkTimer_346);
		hurtPlayer = 0;
	}
	// if previous player state was resurrecting, start timer.
	// Default timer is 2 frames (or 120 because 120/60pfs = 2)
#if UYA_PAL
	short timer = 100;
#else
	short timer = 120;
#endif
	if (playerDeobfuscate(&player->previousState, 0, 0) == PLAYER_STATE_WAIT_FOR_RESURRECT)
		player->timers.unkTimer_346 = timer;

	// run base function with our a1
	DPRINTF("\nhurtPlayer: %d", hurtPlayer);
	return ((int (*)(Player*, int))GetAddress(&vaPlayerInvincibleTimer_Func))(player, hurtPlayer);
}
void respawnInvincTimer(void)
{
	if (patched.gameConfig.grRespawnInvincibility)
		return;

	HOOK_JAL(GetAddress(&vaPlayerInvincibleTimer_Hook), &runInvincibilityTimer);
	patched.gameConfig.grRespawnInvincibility = 1;
}

/*
 * NAME :		disableHealthContainer
 * DESCRIPTION :
 *              Removes the health's glass container by setting the state to already broken.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void disableHealthContainer(void)
{
	if (patched.gameConfig.grHealthBoxes)
		return;

	Moby *moby = mobyListGetStart();
	while ((moby = mobyFindNextByOClass(moby, MOBY_ID_HEALTH_BOX_MP))) {
		if (moby->pUpdate){
			moby->state = 3;
			*(u16*)((u32)moby->pUpdate + 0x1cc) = 3; // switch state 0, set to breaking state
			// always set state to 4 (broken stsate) when respawning health
			*(u32*)((u32)moby->pUpdate - 0x26c) = 0x24050004; // addiu a0, zero, 4;
			*(u32*)((u32)moby->pUpdate - 0x268) = 0xA2050020; // sb a0, 0x20(s0);
			DPRINTF("\nHealth Box Containers Disabled!");
		}
		++moby;
	}
	patched.gameConfig.grHealthBoxes = 1;
}

/*
 * NAME :		onGameplayLoad_destructableBridges
 * DESCRIPTION :
 *              Updates Various bridge pieces healths to a number thta makes it so it can't be destoyed.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onGameplayLoad_destructableBridges(GameplayHeaderDef_t * gameplay)
{
	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);
	u32 PVarOffsetPtr = ((u32)gameplay + gameplay->PVarTableOffset);
	u32 PVarDataPtr = ((u32)gameplay + gameplay->PVarDataOffset);
	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t* moby = &mobyInstancesHeader->MobyInstances[i];
		switch (moby->OClass) {
			case MOBY_ID_BLACKWATER_CITY_DESTRUCTABLE_BRIDGE_SIDE:
			case MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_SIDE: {
				GameplayPVarDef_t* PVarOffset = (GameplayPVarDef_t*)(PVarOffsetPtr + (u32)(moby->PVarIndex * 8));
				u32 data = PVarDataPtr + PVarOffset->Offset;
				*(u32*)(data + 0x4) = 0x7f7fffff;
			}
			case MOBY_ID_BLACKWATER_CITY_DESTRUCTABLE_BRIDGE_CENTER:
			case MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_CENTER: {
				GameplayPVarDef_t* PVarOffset = (GameplayPVarDef_t*)(PVarOffsetPtr + (u32)(moby->PVarIndex * 8));
				u32 data = PVarDataPtr + PVarOffset->Offset;
				*(u32*)(data + 0x10) = 0x7f7fffff;
			}
		}
	}
}

void destructableBridges(void)
{
	if (patched.gameConfig.grDestructableBridges)
		return;

	if (gameGetSettings()->GameLevel == MAP_ID_KORGON) {
		Moby* moby = mobyListGetStart();
		Moby* mobyEnd = mobyListGetEnd();
		while (moby < mobyEnd) {
			if (moby->oClass == MOBY_ID_KORGON_BRIDGE_PIECE) {
					// Copy Extended Bridge Locatoin to Retracted Bridge Location
					void * data = moby->pVar;
					vector_copy(data + 0x90, data + 0xa0);
					// Copy Extended Bridge Location to moby
					vector_copy(moby->position, data + 0xa0);

					// Bolt Crank Code
					// grab associated bolt crank from bridge peice
					int boltCrankInstanceNum = *(int*)(data + 0x8c);
					Moby* list = mobyListGetStart();
					// delete related bolt crank
					Moby* boltCrank = list + boltCrankInstanceNum;
					if (boltCrank->oClass == MOBY_ID_BOLT_CRANK)
						mobyDestroy(boltCrank);
					// *(float*)(boltCrank + 0x18) -= 1.5f;
			}
			++moby;
		}
	}
	patched.gameConfig.grDestructableBridges = 1;
}

/*
 * NAME :		modifyWeaponTweakers
 * DESCRIPTION :
 *             Modifies weapon tweakers (behavior)
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void modifyWeaponTweakers(void)
{
	if (gameConfig.prGravityBombTweakers && patched.gameConfig.prGravityBombTweakers == 0) {
		TweakersGravityBomb_t * gBomb = weaponGravityBombTweakers();
		int i = gameConfig.prGravityBombTweakers;
		gBomb->maxThrowDist = gBombTweaker[i].maxThrowDist;
		gBomb->minThrowDist = gBombTweaker[i].minThrowDist;
		gBomb->gravity = gBombTweaker[i].gravity;
		gBomb->maxThrowSpeed = gBombTweaker[i].maxThrowSpeed;

		patched.gameConfig.prGravityBombTweakers = 1;
	}
}

/*
 * NAME :		onGameplayLoad_disableDrones
 * DESCRIPTION :
 *             Modifies weapon tweakers (behavior)
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onGameplayLoad_disableDrones(GameplayHeaderDef_t * gameplay)
{
	int i;
	GameplayMobyHeaderDef_t * mobyInstancesHeader = (GameplayMobyHeaderDef_t*)((u32)gameplay + gameplay->MobyInstancesOffset);
	u32 PVarOffsetPtr = ((u32)gameplay + gameplay->PVarTableOffset);
	u32 PVarDataPtr = ((u32)gameplay + gameplay->PVarDataOffset);
	for (i = 0; i < mobyInstancesHeader->StaticCount; ++i) {
		GameplayMobyDef_t* moby = &mobyInstancesHeader->MobyInstances[i];
		if (moby->OClass ==  MOBY_ID_DRONE_BOT_CLUSTER_CONFIG) {
			GameplayPVarDef_t* PVarOffset = (GameplayPVarDef_t*)(PVarOffsetPtr + (u32)(moby->PVarIndex * 8));
			u32 data = PVarDataPtr + PVarOffset->Offset;
			*(int*)(data + 0xb8) = 1;
			*(int*)(data + 0xa0) = 1;
		}
	}
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
    char title[32];
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
		allNodesTimer.timeValue = (gameConfig.grAllNodesTimer * .5) * 60;
        char *whichTeam = !teamWithAllNodes ? "Blue" : "Red";
        sprintf(title, "%s Team Wins In", whichTeam);
        strncpy(allNodesTimer.title, title, 32);
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
