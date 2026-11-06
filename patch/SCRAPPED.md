# Patch Gadget Events
path: `horizon-uya-patch/patch/main.c:if(isInGame)`
```c
		// Patches gadget events as they come in.
		if(gameConfig.grWeaponShotLag)
			patchGadgetEvents();
```
path: `horizon-uya-patch/patch/main.c`
```c
/*
 * NAME :		handleGadgetEvent
 * DESCRIPTION :
 * 			Reads gadget events and patches them if needed.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void handleGadgetEvents(int player, char gadgetEventType, int activeTime, short gadgetId, int gadgetType, struct tNW_GadgetEventMessage * message)
{
	// get top of player struct
	Player * p = (Player*)((u32)player - 0x1a40);

	// Force all incoming weapon shot events to happen immediately.
	const int MAX_DELAY = TIME_SECOND * 0;
	int startTime = activeTime;
	// put clamp on max delay
	int delta = activeTime - gameGetTime();
	if (delta > MAX_DELAY) {
		activeTime = gameGetTime() + MAX_DELAY;
		if (message)
			message->ActiveTime = activeTime;
	} else if (delta < 0) {
		activeTime = gameGetTime() - 1;
		if (message)
			message->ActiveTime = activeTime;
	}

	// run base command
	((void (*)(int, char, int, short, int, struct tNW_GadgetEventMessage*))GetAddress(&vaGadgetEventFunc))(player, gadgetEventType, activeTime, gadgetId, gadgetType, message);
}

/*
 * NAME :		patchGadgetEvents
 * DESCRIPTION :
 * 			Hook for the handleGadgetEVentLag function.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchGadgetEvents(void)
{
	if (patched.gadgetEvents)
		return;

	HOOK_JAL(GetAddress(&vaGadgetEventHook), &handleGadgetEvents);
	patched.gadgetEvents = 1;
}
```
path: `horizon-uya-patch/patch/config.c`
```c
  { "Fix Weapon Shot Lag", toggleActionHandler, menuStateHandler_Default, &gameConfig.grWeaponShotLag, "Send shots reliably." },
```
path: `horizon-uya-patch/common/config.h`
```c
  char grWeaponShotLag;

  //insie "patched" table
    char gadgetEvents;
```
path: `horizon-uya-patch/patch/interop/patch.c`
```c
//================================================
//=============  patchGadgetEvents
//================================================
VariableAddress_t vaGadgetEventFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00546510,
    .Hoven = 0x005486d8,
    .OutpostX12 = 0x0053dfb0,
    .KorgonOutpost = 0x0053b698,
    .Metropolis = 0x0053aa98,
    .BlackwaterCity = 0x00538280,
    .CommandCenter = 0x00537ad8,
    .BlackwaterDocks = 0x0053a358,
    .AquatosSewers = 0x00539658,
    .MarcadiaPalace = 0x00538fd8,
#else
    .Lobby = 0,
    .Bakisi = 0x00543c00,
    .Hoven = 0x00545d08,
    .OutpostX12 = 0x0053b620,
    .KorgonOutpost = 0x00538d88,
    .Metropolis = 0x00538188,
    .BlackwaterCity = 0x005358f0,
    .CommandCenter = 0x00535320,
    .BlackwaterDocks = 0x00537b60,
    .AquatosSewers = 0x00536ea0,
    .MarcadiaPalace = 0x005367e0,
#endif
};
VariableAddress_t vaGadgetEventHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0054b23c,
    .Hoven = 0x0054d404,
    .OutpostX12 = 0x00542cdc,
    .KorgonOutpost = 0x005403c4,
    .Metropolis = 0x0053f7c4,
    .BlackwaterCity = 0x0053cfac,
    .CommandCenter = 0x0053c804,
    .BlackwaterDocks = 0x0053f084,
    .AquatosSewers = 0x0053e384,
    .MarcadiaPalace = 0x0053dd04,
#else
    .Lobby = 0,
    .Bakisi = 0x00548894,
    .Hoven = 0x0054a99c,
    .OutpostX12 = 0x005402b4,
    .KorgonOutpost = 0x0053da1c,
    .Metropolis = 0x0053ce1c,
    .BlackwaterCity = 0x0053a584,
    .CommandCenter = 0x00539fb4,
    .BlackwaterDocks = 0x0053c7f4,
    .AquatosSewers = 0x0053bb34,
    .MarcadiaPalace = 0x0053b474,
#endif
};
```
path: `horizon-uya-patch/patch/interop/patch.h`
```c
VariableAddress_t vaGadgetEventFunc;
VariableAddress_t vaGadgetEventHook;
```

# Spawn Packs on Death
path: `horizon-uya-patch/patch/cheats.c`
```c
/*
 * NAME :		spawnWeaponPackOnDeath
 * DESCRIPTION :
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int SpawnedPack = 0;
void SpawnPack(int a0, int a1, int a2)
{
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
    // if Health is greater than zero and pack has spawned
    // This will be checking constantly, instead of just when the player dies.
    Player ** players = playerGetAll();
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
    	if (!players[i])
    		continue;

		Player * player = players[i];
		if (player->isLocal && playerGetHealth(player) > 0)
            SpawnedPack = 0;
	}

	// if Patched don't run following.
	if (patched.spawnWeaponPackOnDeath)
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
	patched.spawnWeaponPackOnDeath = 1;
}
```
path: `horizon-uya-patch/patch/interop/cheats.c`
```c
//================================================
//=============  spawnWeaponPackOnDeath - SpawnPack
//================================================
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
```
path: `horizon-uya-patch/patch/interop/cheats.h`
```c
VariableAddress_t vaRespawnTimerFunc;
VariableAddress_t vaSpawnWeaponPackFunc;
VariableAddress_t vaRespawnTimerHook;
```
path: `horizon-uya-patch/patch/gamerules.c`
```c
	if (gameConfig.grDisableWeaponPacks == 1)
		disableWeaponPacks();
	else if (gameConfig.grDisableWeaponPacks == 2)
		spawnWeaponPackOnDeath();
```
path: horizon-uya-patch/patch/config.c`
```c
MenuElem_ListData_t dataWeaponPacks = {
    .value = &gameConfig.grDisableWeaponPacks,
    .stateHandler = NULL,
    .count = 2,
    .items = { "Default", "Off", "On Death" }
};
```




# Player Size
path: `horizon-uya-patch/common/config.h`
```c
  char prPlayerSize;
```

path: `horizon-uya-patch/patch/config.c`
```c
MenuElem_ListData_t dataPlayerSize = {
    .value = &gameConfig.prPlayerSize,
    .stateHandler = NULL,
    .count = 5,
    .items = { "Normal", "Large", "Giant", "Tiny", "Small" }
};

  { "Player Size", listActionHandler, menuStateHandler_Default, &dataPlayerSize, "Changes the size of the player model." },
```

path: `horizon-uya-patch/patch/gamerules.c`
```c
	if (gameConfig.prPlayerSize)
		playerSize();

	if (gameConfig.prPlayerSize)
		onGameplayLoad_playerSize(gameplay);
```

path: `horizon-uya-patch/patch/include/cheats.h`
```c
void onGameplayLoad_playerSize(GameplayHeaderDef_t * gameplay);
void playerSize(void);
```

path: `horizon-uya-patch/patch/cheats.c`
```c
/*
 * NAME :		playerSize
 * DESCRIPTION :
 *              Modifies the player's size (and other objects)
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void playerSize(void)
{
	int i, j;
	Player** players = playerGetAll();
	float size = 1;
	float cameraHeight = 1;
	float tpHeight = 1;
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
			if (player->pMoby)
				player->pMoby->scale = 0.25 * size;

			// update camera
			player->fps.vars.positionOffset[0] = -6 * size;
			player->fps.vars.positionOffset[2] = cameraHeight;
			player->fps.vars.range = 2 + cameraHeight;
		}
	}
}

/*
 * NAME :		onGameplayLoad_playerSize
 * DESCRIPTION :
 *              Modifies the player's size (and other objects) while game is loading.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void onGameplayLoad_playerSize(GameplayHeaderDef_t * gameplay)
{
	float size = 1;
	float cameraHeight = 1;
	float tpHeight = 1;
	float moveSpeed = 1;
	switch (gameConfig.prPlayerSize) {
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
			case MOBY_ID_GATLING_TURRET:
			case MOBY_ID_ANIT_VEHICLE_TURRET:
			case MOBY_ID_BASE_COMPUTER:
			{
				float CurrentScale = moby->Scale;
				moby->Scale = CurrentScale * size;
			}
		}
	}
}
```

# AFK Patch
path: `horizon-uya-patch/patch/main.c`
```c
void patchAFK(void)
{
	int AFK_Wait_Time = 15; // in Minutes

	int isAFK = -1;
	int gameTime = gameGetTime();
	static int afk_time = 0;
	if (afk_time == 0)
		afk_time = gameTime + (AFK_Wait_Time * TIME_SECOND);

	PAD * src = (PAD*)((u32)P1_PAD - 0x80);
	// if no buttons/analogs are pressed
	if (src->handsOff && src->handsOffStick) {
		// if is already AFK, return.
		if (isAFK == 1)
			return;

		// if time left to go afk is greater than the game time, then not afk yet, return.
		if (afk_time > gameTime)
			return;

		isAFK = 1;
		patchAFK_SendResponse(isAFK);
	} else {
		afk_time = gameTime + (TIME_SECOND * AFK_Wait_Time);
		if (isAFK == 0)
			return;

		isAFK = 0;
		patchAFK_SendResponse(isAFK);
	}
}
```

# End Game When the Last Opposing Team Leaves
path: `horizon-uya-patch/patch/join.c:global variables`
```c
static int joinHadOpponents;
```
path: `horizon-uya-patch/patch/join.c`
The previous `joinUpdateHost` implementation is preserved below. The opposing-team departure check worked in the two-player CTF test, but is disabled for now. Host migration and world-report handling remain active.
```c
/*
 * NAME : joinUpdateHost
 * DESCRIPTION : Keep native host migration and end games whose last opposing team departed.
 * NOTES : A game that never had opposing teams may continue with one team.
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinUpdateHost(GameSettings* settings, int supported)
{
	if (!settings || !isInGame()) {
		joinHadOpponents = 0;
		joinPreviousHost = -1;
		return;
	}
	joinKeepWorldOnLeave = 0;
	if (!supported)
		return;
	if (joinHostLoadTime != settings->GameLoadStartTime) {
		joinHostLoadTime = settings->GameLoadStartTime;
		joinHadOpponents = 0;
		joinPreviousHost = -1;
	}
	u32 teams = 0;
	int otherClients = 0;
	int remaining = -1;
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		int client = settings->PlayerClients[i];
		int team = settings->PlayerTeams[i];
		if (client < 0 || settings->PlayerAccountIds[i] < 0)
			continue;
		if (client != GAME_CLIENT_ID)
			++otherClients;
		if (team >= 0 && team < GAME_MAX_PLAYERS) {
			teams |= 1 << team;
			remaining = i;
		}
	}
	joinKeepWorldOnLeave = otherClients > 0;
	if (teams & (teams - 1))
		joinHadOpponents = 1;
	int host = gameGetHostId();
	if (joinPreviousHost >= 0 && joinPreviousHost != host && gameAmIHost()) {
		joinLastTimeSent = 0;
		((void (*)(void))JOIN_FORCE_WORLD_REPORT)();
	}
	joinPreviousHost = host;
	if (joinHadOpponents && teams && !(teams & (teams - 1)) && gameAmIHost()) {
		GameData* game = gameGetData();
		GameOptions* options = gameGetOptions();
		if (!game || !options)
			return;
		game->winningTeam = settings->PlayerTeams[remaining];
		game->winningPlayer = options->GameFlags.MultiplayerGameFlags.Teams ? -1 : remaining;
		joinKeepWorldOnLeave = 0;
		gameEnd(GAME_END_TEAM_WIN);
	}
}
```

# Gravity Bomb -> B6 Obliterator
path: `horizon-uya-patch/patch/cheats.c`
```c
Tweaker_GravityBomb_t gBombTweaker[] = {
	{12, 0.00001, 12, 15}, // UYA Default
	{16, 0.00001, 16, 24} // DL Default
};
```
path: `horizon-uya-patch/patch/cheats.c`
```c
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
```
path: `horizon-uya-patch/patch/config.c`
```c
  { "Gravity Bomb->B6 Obliterator", toggleActionHandler, menuStateHandler_Default, &gameConfig.prGravityBombTweakers, "Changes the Gravity Bomb Physics to that of the B6 Obliterator from Ratchet: Deadlocked!" },
```
path: `horizon-uya-patch/patch/include/cheats.h`
```c
void modifyWeaponTweakers(void);
```
path: `horizon-uya-patch/patch/include/cheats.h`
```c
typedef struct Tweaker_GravityBomb {
	float maxThrowDist;
	float minThrowDist;
	float gravity;
	float maxThrowSpeed;
} Tweaker_GravityBomb_t;
```
path: `horizon-uya-patch/patch/gamerules.c`
```c
	modifyWeaponTweakers();
```
path: `horizon-uya-patch/common/config.h`
```c
	char prGravityBombTweakers;
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
        public bool prGravityBombTweakers { get; set; }
                    writer.Write(prGravityBombTweakers);
            prGravityBombTweakers = reader.ReadBoolean();
                && prGravityBombTweakers == other.prGravityBombTweakers
```


# Infinite Chargeboot
path: `horizon-uya-patch/patch/cheats.c`
```c
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
```
# Respawn Invicibility
path: `horizon-uya-patch/patch/cheats.c`
```c
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
	if (playerDeobfuscate(&player->previousState, 0) == PLAYER_STATE_WAIT_FOR_RESURRECT)
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

```

# Player Respawn Invincibility
path: `horizon-uya-patch/patch/cheats.c`
```c
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
	if (playerDeobfuscate(&player->previousState, 0) == PLAYER_STATE_WAIT_FOR_RESURRECT)
		player->timers.unkTimer_346 = timer;

	// run base function with our a1
	DPRINTF("\nhurtPlayer: %d", hurtPlayer);
	return ((int (*)(Player*, int))GetAddress(&vaPlayerInvincibleTimer_Func))(player, hurtPlayer);
}
```
path: `horizon-uya-patch/patch/cheats.c`
```c
void respawnInvincTimer(void)
{
	if (patched.gameConfig.grRespawnInvincibility)
		return;

	HOOK_JAL(GetAddress(&vaPlayerInvincibleTimer_Hook), &runInvincibilityTimer);
	patched.gameConfig.grRespawnInvincibility = 1;
}
```
path: `horizon-uya-patch/common/config.h`
```c
	char grRespawnInvincibility;
```
path: `horizon-uya-patch/patch/config.c`
```c
  { "Respawn Invincibility", toggleActionHandler, menuStateHandler_Default, &gameConfig.grRespawnInvincibility, "Adds a small invincibility timer to players whom have just respawned." },
				gameConfig.grRespawnInvincibility = 0;
```
path: `horizon-uya-patch/patch/interop/cheats.c`
```c
//================================================
//=============  playerInvicibleTimer
//================================================
variableAddressCompact_t vaPlayerInvincibleTimer_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0052916c),
	.Hoven = VA_COMPACT_OFFSET(0x0052916c, 0x0052b284),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0052916c, 0x00520b5c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0052916c, 0x0051e2f4),
	.Metropolis = VA_COMPACT_OFFSET(0x0052916c, 0x0051d644),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0052916c, 0x0051aedc),
	.CommandCenter = VA_COMPACT_OFFSET(0x0052916c, 0x0051ac9c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0052916c, 0x0051d51c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0052916c, 0x0051c81c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0052916c, 0x0051c19c),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005268ec),
	.Hoven = VA_COMPACT_OFFSET(0x005268ec, 0x00528944),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005268ec, 0x0051e25c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005268ec, 0x0051ba74),
	.Metropolis = VA_COMPACT_OFFSET(0x005268ec, 0x0051adc4),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005268ec, 0x005185dc),
	.CommandCenter = VA_COMPACT_OFFSET(0x005268ec, 0x0051855c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005268ec, 0x0051ad9c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005268ec, 0x0051a0dc),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005268ec, 0x00519a1c),
#endif
};
```
path: `horizon-uya-patch/patch/interop/cheats.c`
```c
variableAddressCompact_t vaPlayerInvincibleTimer_Func = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004f8c30),
	.Hoven = VA_COMPACT_OFFSET(0x004f8c30, 0x004fad48),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f8c30, 0x004f0620),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f8c30, 0x004eddb8),
	.Metropolis = VA_COMPACT_OFFSET(0x004f8c30, 0x004ed108),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f8c30, 0x004ea9a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f8c30, 0x004ea968),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f8c30, 0x004ed1e8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f8c30, 0x004ec4e8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f8c30, 0x004ebe68),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004f64b0),
	.Hoven = VA_COMPACT_OFFSET(0x004f64b0, 0x004f8508),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f64b0, 0x004ede20),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f64b0, 0x004eb638),
	.Metropolis = VA_COMPACT_OFFSET(0x004f64b0, 0x004ea988),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f64b0, 0x004e81a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f64b0, 0x004e8328),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f64b0, 0x004eab68),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f64b0, 0x004e9ea8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f64b0, 0x004e97e8),
#endif
};
```
path: `horizon-uya-patch/patch/interop/cheats.h`
```c
variableAddressCompact_t vaPlayerInvincibleTimer_Hook;
variableAddressCompact_t vaPlayerInvincibleTimer_Func;
```
path: `horizon-uya-patch/patch/include/cheats.h`
```c
void respawnInvincTimer(void);
```
path: `horizon-uya-patch/patch/gamerules.c`
```c
	if (gameConfig.grRespawnInvincibility)
		respawnInvincTimer();
```
path: `horizon-uya-patch/patch/config.c`
```c
				gameConfig.grRespawnInvincibility = 0;
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
        public bool grRespawnInvicibility { get; set; }
                    writer.Write(grRespawnInvicibility);
            grRespawnInvicibility = reader.ReadBoolean();
                && grRespawnInvicibility == other.grRespawnInvicibility
```

# Health/Ammo Pads Always Active
path: `horizon-uya-patch/patch/cheats.c`
```c
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
```
path: `horizon-uya-patch/patch/include/cheats.h`
```c
int keepBaseHealthPadActive(void);
```
path: `horizon-uya-patch/patch/gamerules.c`
```c
int HasKeepBaseHealthPadActive = 0;
	HasKeepBaseHealthPadActive = 0;
	if (gameConfig.grBaseHealthPadActive && !HasKeepBaseHealthPadActive && gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_BaseAmmoHealth)
		HasKeepBaseHealthPadActive = keepBaseHealthPadActive();
```
path: `horizon-uya-patch/patch/config.c`
```c
  { "Health/Ammo Pads Always Active", toggleActionHandler, menuStateHandler_BaseDefenses, &gameConfig.grBaseHealthPadActive, "Let the Health and Ammo pads in each base always stay active, even if parts of the base are destroyed." },
				gameConfig.grBaseHealthPadActive = 0;
```
path: `horizon-uya-patch/common/config.h`
```c
	char grBaseHealthPadActive;
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
        public bool grBaseHealthPadActive { get; set; }
                    writer.Write(grBaseHealthPadActive);
            grBaseHealthPadActive = reader.ReadBoolean();
                && grBaseHealthPadActive == other.grBaseHealthPadActive
```

# Destructable Bridges
path: `horizon-uya-patch/patch/cheats.c`
```c
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
```
path: `horizon-uya-patch/patch/cheats.c`
```c
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
```
path: `horizon-uya-patch/patch/include/cheats.h`
```c
void onGameplayLoad_destructableBridges(GameplayHeaderDef_t * gameplay);
void destructableBridges(void);
```
path: `horizon-uya-patch/patch/gamerules.c`
```c
	if (gameConfig.grDestructableBridges)
		onGameplayLoad_destructableBridges(gameplay);
	if (gameConfig.grDestructableBridges)
		destructableBridges();
```
path: `horizon-uya-patch/patch/config.c`
```c
  { "Destructable Bridges", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDestructableBridges, "turn on/off the ability to destroy bridges." },
				gameConfig.grDestructableBridges = preset == 4;
				gameConfig.grDestructableBridges = 0;
```
path: `horizon-uya-patch/common/config.h`
```c
	char grDestructableBridges;
```
path: `horizon-uya-patch/patch/join.c`
```c
	if (gameConfig.grDestructableBridges &&
	    (oClass == MOBY_ID_KORGON_BRIDGE_PIECE ||
	    oClass == MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_CENTER ||
	    oClass == MOBY_ID_BLACKWATER_CITY_DESTRUCTABLE_BRIDGE_CENTER))
		return 0;
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
        public bool grDestructableBridges { get; set; }
                    writer.Write(grDestructableBridges);
            grDestructableBridges = reader.ReadBoolean();
                && grDestructableBridges == other.grDestructableBridges
```

# Always Show Health
path: `horizon-uya-patch/patch/main.c`
```c
/*
 * NAME :		patchAlwaysShowHealth
 * DESCRIPTION :
 * 				Always shows the players health bar.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchAlwaysShowHealth(void)
{
	u32 healthbar_timer = GetAddress(&vaHealthBarTimerSaveZero);;
	Player *player = playerGetFromSlot(0);
	u32 old_value = 0xae002514; // sw zero,0x2514(s0)
	if (config.alwaysShowHealth && *(u32*)healthbar_timer == old_value) {
		*(u32*)healthbar_timer = 0;
		player->hudHealthTimer = 3 * GAME_FPS;
	} else if (!config.alwaysShowHealth && *(u32*)healthbar_timer == 0) {
		*(u32*)healthbar_timer = old_value;
	}
}
```
path: `horizon-uya-patch/patch/interop/patch.c`
```c
//================================================
//=============  patchAlwaysShowHealthBar
//================================================
variableAddressCompact_t vaHealthBarTimerSaveZero = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00548020),
	.Hoven = VA_COMPACT_OFFSET(0x00548020, 0x0054a1e8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00548020, 0x0053fac0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00548020, 0x0053c5a8),
	.Metropolis = VA_COMPACT_OFFSET(0x00548020, 0x0053c5a8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00548020, 0x00539d90),
	.CommandCenter = VA_COMPACT_OFFSET(0x00548020, 0x005395e8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00548020, 0x0053be68),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00548020, 0x0053b168),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00548020, 0x0053aae8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00545718),
	.Hoven = VA_COMPACT_OFFSET(0x00545718, 0x00547820),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00545718, 0x0053d138),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00545718, 0x0053a8a0),
	.Metropolis = VA_COMPACT_OFFSET(0x00545718, 0x00539ca0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00545718, 0x00537408),
	.CommandCenter = VA_COMPACT_OFFSET(0x00545718, 0x00536e38),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00545718, 0x00539678),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00545718, 0x005389b8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00545718, 0x005382f8),
#endif
};
```
path: `horizon-uya-patch/patch/main.c`
```c
	.alwaysShowHealth = 0,
		if (config.alwaysShowHealth)
			patchAlwaysShowHealth();
```
path: `horizon-uya-patch/patch/config.c`
```c
  { "Always Show Health", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.alwaysShowHealth, "Let that health bar hide no longer!  This will make it so the health bar is always visible." },
```
path: `horizon-uya-patch/common/config.h`
```c
  char alwaysShowHealth;
```
path: `horizon-uya-patch/patch/interop/patch.h`
```c
variableAddressCompact_t vaHealthBarTimerSaveZero;
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
        public bool alwaysShowHealth { get; set; }
                    writer.Write(alwaysShowHealth);
            alwaysShowHealth = reader.ReadBoolean();
```

# Retired Flux Config Toggles (fixes now always enabled)
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
        public bool grFluxShotsAlwaysHit { get; set; }
        public bool grFluxNikingDisabled { get; set; }
                    writer.Write(grFluxShotsAlwaysHit);
                    writer.Write(grFluxNikingDisabled);
            grFluxShotsAlwaysHit = reader.ReadBoolean();
            grFluxNikingDisabled = reader.ReadBoolean();
                && grFluxShotsAlwaysHit == other.grFluxShotsAlwaysHit
                && grFluxNikingDisabled == other.grFluxNikingDisabled
```
path: `horizon-uya-patch/patch/config.c`
```c
				gameConfig.grFluxShotsAlwaysHit = 1;
				gameConfig.grFluxNikingDisabled = 1;
				gameConfig.grFluxShotsAlwaysHit = 1;
				gameConfig.grFluxNikingDisabled = 1;
```

# Player Health Bars Above Names
path: `horizon-uya-patch/patch/cheats.c`
```c
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
```
path: `horizon-uya-patch/patch/cheats.c`
```c
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
```
path: `horizon-uya-patch/patch/interop/cheats.c`
```c
//================================================
//=============  healthbars
//================================================
variableAddressCompact_t vaHealthbars_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00507c70),
	.Hoven = VA_COMPACT_OFFSET(0x00507c70, 0x00509d88),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00507c70, 0x004ff660),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00507c70, 0x004fcdf8),
	.Metropolis = VA_COMPACT_OFFSET(0x00507c70, 0x004fc148),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00507c70, 0x004f99e0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00507c70, 0x004f99a8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00507c70, 0x004fc228),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00507c70, 0x004fb528),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00507c70, 0x004faea8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00505480),
	.Hoven = VA_COMPACT_OFFSET(0x00505480, 0x005074d8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00505480, 0x004fcdf0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00505480, 0x004fa608),
	.Metropolis = VA_COMPACT_OFFSET(0x00505480, 0x004f9958),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00505480, 0x004f7170),
	.CommandCenter = VA_COMPACT_OFFSET(0x00505480, 0x004f72f8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00505480, 0x004f9b38),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00505480, 0x004f8e78),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00505480, 0x004f87b8),
#endif
};
```
path: `horizon-uya-patch/patch/include/cheats.h`
```c
void healthbars(void);
```
path: `horizon-uya-patch/patch/interop/cheats.h`
```c
variableAddressCompact_t vaHealthbars_Hook;
```
path: `horizon-uya-patch/patch/gamerules.c`
```c
	if (gameConfig.grHealthBars)
		healthbars();
```
path: `horizon-uya-patch/patch/config.c`
```c
  // { "Health Bars", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grHealthBars, "Draws a healthbar above each player's nametag." },
```
path: `horizon-uya-patch/common/config.h`
```c
	char grHealthBars;
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
        public bool grHealthBars { get; set; }
                    writer.Write(grHealthBars);
            grHealthBars = reader.ReadBoolean();
                && grHealthBars == other.grHealthBars
```

# Original Loadout and Weapon Cycle Ordering
path: `horizon-uya-patch/patch/main.c`
```c
/*
 * NAME :		patchResurrectWeaponOrdering_HookWeaponStripMe
 * DESCRIPTION :
 * 			Invoked during the resurrection process, when the game wishes to remove all weapons from the given player.
 * 			Before we continue to remove the player's weapons, we backup the list of equipped weapons.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchResurrectWeaponOrdering_HookWeaponStripMe(Player * player)
{
	int i;
	// backup currently equipped weapons
	if (player->isLocal) {
		for (i = 0; i < 3; ++i)
			weaponOrderBackup[0][i] = playerDeobfuscate(&player->quickSelect.Slot[i], 1);
	}

	// call hooked WeaponStripMe function after backup
	playerStripWeapons(player);
}

int patchResurrectWeaponOrdering_ConvertToWeaponId(int id)
{
	int weapon;
	switch (id) {
		case 8: return GADGET_ID_MORPH;
		case 9:	return GADGET_ID_HOLO;
		default: return id;
	}
}
/*
 * NAME :		patchResurrectWeaponOrdering_HookGiveMeRandomWeapons
 * DESCRIPTION :
 * 			Invoked during the resurrection process, when the game wishes to give the given player a random set of weapons.
 * 			After the weapons are randomly assigned to the player, we check to see if the given weapons are the same as the last equipped weapon backup.
 * 			If they contain the same list of weapons (regardless of order), then we force the order of the new set of weapons to match the backup.
 * 			Consider the scenario:
 * 				Player dies with 								Fusion, B6, Magma Cannon
 * 				Player is assigned 							B6, Fusion, Magma Cannon
 * 				Player resurrects with  				Fusion, B6, Magma Cannon
 * 			If prLoadoutWeaponsOnly
 * 				Player will only spawn with their selected loadout (or default to cycle weapons)
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchResurrectWeaponOrdering_HookGiveMeRandomWeapons(Player* player, int weaponCount)
{
	int i, j;
	char matchCount = 0;
	char cycleWeaponCount = 0;
	int index = player->mpIndex;

	char cycle[] = {
		config.cycleWeapon1 > 0 ? patchResurrectWeaponOrdering_ConvertToWeaponId(config.cycleWeapon1) : GADGET_ID_GBOMB,
		config.cycleWeapon2 > 0 ? patchResurrectWeaponOrdering_ConvertToWeaponId(config.cycleWeapon2) : GADGET_ID_BLITZ,
		config.cycleWeapon3 > 0 ? patchResurrectWeaponOrdering_ConvertToWeaponId(config.cycleWeapon3) : GADGET_ID_FLUX
	};

	// if Loadout Weapons Only is not on
	if (!gameConfig.prLoadoutWeaponsOnly) {
		// then try and overwrite given weapon order if weapons match equipped weapons before death
		playerGiveRandomWeapons(player, weaponCount);

		// restore backup if they match (regardless of order) newly assigned weapons
		for (i = 0; i < 3; ++i) {
			// if respawned weapons match backup weapons
			u8 backedUpSlotValue = weaponOrderBackup[0][i];
			for(j = 0; j < 3; ++j) {
				if (backedUpSlotValue == playerDeobfuscate(&player->quickSelect.Slot[j], 1)) {
					++matchCount;
					DPRINTF("\ni: %d, j: %d, Matched: %d", i, j, playerDeobfuscate(&player->quickSelect.Slot[j], 1));
				}
			}
		}
		// if weaponOrderBackup matches respawnd weapons, then check for cycle weapons (regardless of order)
		if (matchCount == 3) {
			for (i = 0; i < 3; ++i) {
				u8 backedUpSlotValue = weaponOrderBackup[0][i];
				for(j = 0; j < 3; ++j) {
					if (backedUpSlotValue == cycle[j]) {
						++cycleWeaponCount;
						DPRINTF("\ni: %d, j: %d, Cycle: %d", i, j, cycle[j]);
					}
				}
			}
		}
		DPRINTF("\nMatch Count: %d; Cycle Count: %d", matchCount, cycleWeaponCount);
	}
	// if cycleWeaponCount matches, set backup weapons.
	// or if Party Rule LoadWeapons Only is on, force set to needed weapons.
	if (cycleWeaponCount == 3 || gameConfig.prLoadoutWeaponsOnly) {
		for (i = 0; i < 3; ++i)
			weaponOrderBackup[0][i] = cycle[i];
	}
	// we found a match, or loadout weapons only is on.
	if (matchCount == 3 || gameConfig.prLoadoutWeaponsOnly) {
		// set equipped weapon in order
		for (i = 0; i < 3; ++i)
			playerGiveWeapon(player, weaponOrderBackup[0][i], 1);

		// equip each weapon from last slot to first slot to keep correct order.
		for (i = 2; i >= 0; --i)
			playerEquipWeapon(player, weaponOrderBackup[0][i]);
	}
}
/*
 * NAME :		spawnWithLoadoutWeapons
 * DESCRIPTION :
 *              Spawns players loudout at start of game.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void spawnWithLoadoutWeapons(void)
{
	Player *player = playerGetFromSlot(0);
	// Strip all Weapons
	patchResurrectWeaponOrdering_HookWeaponStripMe(player);
	// Give needed weapons
	patchResurrectWeaponOrdering_HookGiveMeRandomWeapons(player, 3);
	// Give chargeboots if needed
	if (gameGetOptions()->GameFlags.MultiplayerGameFlags.Chargeboots == 1)
		playerGiveWeapon(player, GADGET_ID_CHARGEBOOTS, 0);
}

/*
 * NAME :		patchResurrectWeaponOrdering
 * DESCRIPTION :
 * 			Installs necessary hooks such that when respawning with same weapons,
 * 			they are equipped in the same order.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchResurrectWeaponOrdering(void)
{
	if (patched.resurrectWeaponOrdering)
		return;

	u32 hook_StripMe = ((u32)GetAddress(&vaPlayerRespawnFunc) + 0x40);
	u32 hook_RandomWeapons = hook_StripMe + 0x1c;
	HOOK_JAL(hook_StripMe, &patchResurrectWeaponOrdering_HookWeaponStripMe);
	HOOK_JAL(hook_RandomWeapons, &patchResurrectWeaponOrdering_HookGiveMeRandomWeapons);
	// set weapons at start of game.
	spawnWithLoadoutWeapons();

	patched.resurrectWeaponOrdering = 1;
}
```

# Removed Loadout Configuration
path: `horizon-uya-patch/patch/main.c`
```c
	.cycleWeapon1 = 0,
```
path: `horizon-uya-patch/patch/main.c`
```c
	.cycleWeapon2 = 0,
```
path: `horizon-uya-patch/patch/main.c`
```c
	.cycleWeapon3 = 0,
```
path: `horizon-uya-patch/patch/config.c`
```c
static const char * const loadoutWeaponItems[] = {
	"Off", "N60 Storm", "Blitz Cannon", "Flux Rifle", "Mini Rocket Tube", "Gravity Bomb", "Mine Glove", "Lava Gun", "Morph-O-Ray", "Holo Shield Glove"
};
```
path: `horizon-uya-patch/patch/config.c`
```c
MenuElem_ListData_t dataCycleWeapon1 = {
    .value = &config.cycleWeapon1,
    .stateHandler = NULL,
    .count = 10,
    .items = loadoutWeaponItems
};
```
path: `horizon-uya-patch/patch/config.c`
```c
MenuElem_ListData_t dataCycleWeapon2 = {
    .value = &config.cycleWeapon2,
    .stateHandler = NULL,
    .count = 10,
    .items = loadoutWeaponItems
};
```
path: `horizon-uya-patch/patch/config.c`
```c
MenuElem_ListData_t dataCycleWeapon3 = {
    .value = &config.cycleWeapon3,
    .stateHandler = NULL,
    .count = 10,
    .items = loadoutWeaponItems
};
```
path: `horizon-uya-patch/patch/config.c`
```c
    // if (gameConfig.prLoadoutWeaponsOnly) {
    //   gameConfig.grDisableWeaponPacks = 1;
    //   gameConfig.grDisableWeaponCrates = 1;
    //   gameConfig.grDisableAmmoPickups = 1;
    // }
```
path: `horizon-uya-patch/patch/config.c`
```c
  // { "Loadout Weapon 1", listActionHandler, menuStateHandler_DisabledInGame, &dataCycleWeapon1, "Main Weapon 1.  Defaults to Gravity Bomb if nothing is selected." },
```
path: `horizon-uya-patch/patch/config.c`
```c
  // { "Loadout Weapon 2", listActionHandler, menuStateHandler_DisabledInGame, &dataCycleWeapon2, "Main Weapon 2.  Defaults to Blitz Cannon if nothing is selected." },
```
path: `horizon-uya-patch/patch/config.c`
```c
  // { "Loadout Weapon 3", listActionHandler, menuStateHandler_DisabledInGame, &dataCycleWeapon3, "Main Weapon 3.  Defaults to Flux Rifle if nothing is selected." },
```
path: `horizon-uya-patch/patch/config.c`
```c
  { "Loadout Weapons Only", toggleActionHandler, menuStateHandler_Default, &gameConfig.prLoadoutWeaponsOnly, "Only allow the use of each players Loadout weapons, regardless of what weapons are enabled." },
```
path: `horizon-uya-patch/patch/config.c`
```c
				gameConfig.prLoadoutWeaponsOnly = 0;
```
path: `horizon-uya-patch/common/config.h`
```c
  char cycleWeapon1;
```
path: `horizon-uya-patch/common/config.h`
```c
  char cycleWeapon2;
```
path: `horizon-uya-patch/common/config.h`
```c
  char cycleWeapon3;
```
path: `horizon-uya-patch/common/config.h`
```c
	char prLoadoutWeaponsOnly;
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
        public bool prLoadoutWeaponsOnly { get; set; }
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
                    writer.Write(prLoadoutWeaponsOnly);
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
            prLoadoutWeaponsOnly = reader.ReadBoolean();
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Game.cs`
```cs
                && prLoadoutWeaponsOnly == other.prLoadoutWeaponsOnly
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
        public byte cycleWeapon1 { get; set; }
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
        public byte cycleWeapon2 { get; set; }
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
        public byte cycleWeapon3 { get; set; }
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
                    writer.Write(cycleWeapon1);
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
                    writer.Write(cycleWeapon2);
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
                    writer.Write(cycleWeapon3);
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
            cycleWeapon1 = reader.ReadByte();
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
            cycleWeapon2 = reader.ReadByte();
```
path: `horizon-uya-plugin/Horizon.Plugin.UYA/Player.cs`
```cs
            cycleWeapon3 = reader.ReadByte();
```
