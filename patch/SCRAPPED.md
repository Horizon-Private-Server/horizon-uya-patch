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