/***************************************************
 * FILENAME :		main.c
 * DESCRIPTION :
 * 		Manages and applies all UYA patches.
 * NOTES :
 * 		Each offset is determined per game region.
 * 		This is to ensure compatibility between versions of UYA PAL/NTSC.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>
#include <libuya/string.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/pad.h>
#include <libuya/stdio.h>
#include <libuya/uya.h>
#include <libuya/math3d.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include <libuya/time.h>
#include <libuya/collision.h>
#include <libuya/net.h>
#include <libuya/interop.h>
#include <libuya/utils.h>
#include <libuya/player.h>
#include <libuya/map.h>
#include <libuya/guber.h>
#include <libuya/music.h>
#include <libuya/team.h>
#include "module.h"
#include "messageid.h"
#include "config.h"
#include "interop/patch.h"
#include "include/config.h"
#include "include/cheats.h"

#define GLOBAL_GAME_MODULES_START							((GameModule*)0x000CF000)
#define EXCEPTION_DISPLAY_ADDR								(0x000C8000)
#define GAME_UPDATE_SENDRATE								(5 * TIME_SECOND)

#if UYA_PAL
#define STAGING_START_BUTTON_STATE							(*(short*)0x006c2d80)
#define RANK_TABLE                              			((u32)0x001a6a64)
#define UI_PTR_FUNC_CREATE_GAME								(0x0047cfec)
#define UI_PTR_FUNC_STAGING									(0x0047e9ac)
#define UI_PTR_FUNC_BUDDIES									(0x0047c834)
#define UI_PTR_FUNC_PLAYER_DETAILS							(0x0047e44c)
#define UI_PTR_FUNC_STATS									(0x0047eab4)
#else
#define STAGING_START_BUTTON_STATE							(*(short*)0x006C0268)
#define RANK_TABLE                              			((u32)0x001a6Be4)
#define UI_PTR_FUNC_CREATE_GAME								(0x0047d0ac)
#define UI_PTR_FUNC_STAGING									(0x0047ea6c)
#define UI_PTR_FUNC_BUDDIES									(0x0047c8f4)
#define UI_PTR_FUNC_PLAYER_DETAILS							(0x0047e50c)
#define UI_PTR_FUNC_STATS									(0x0047eb74)
#endif

void onConfigOnlineMenu(void);
void onConfigGameMenu(void);
void onConfigUpdate(void);
void configMenuEnable(void);
void configMenuDisable(void);

void runMapLoader(void);
void onMapLoaderOnlineMenu(void);

int patchCreateGame(void *ui, long pad);
int patchStaging(void * ui, long pad);
int patchBuddies(void * ui, long pad);
int patchPlayerDetails(void * ui, long pad);
int patchStats(void * ui, int pad);

void grGameStart(void);
void grLobbyStart(void);
void grLoadStart(void);

void runSpectate(void);
void scavHuntRun(void);
#if TEST
void runTest(void);
#endif

int hasInitialized = 0;
int lastMenuInvokedTime = 0;
int lastGameState = 0;
int sentGameStart = 0;
int isInStaging = 0;
int location = LOCATION_NULL;
int hasInstalledExceptionHandler = 0;
char mapOverrideResponse = 1;
char showNoMapPopup = 0;
int isConfigMenuActive = 0;
int redownloadCustomModeBinaries = 0;
char weaponOrderBackup[2][3] = { {0,0,0}, {0,0,0} };
float lastFps = 0;
int renderTimeMs = 0;
float averageRenderTimeMs = 0;
int updateTimeMs = 0;
float averageUpdateTimeMs = 0;
const char * patchStr = "PATCH CONFIG";
#if UYA_PAL
const char * regionStr = "PAL:  ";
#else
const char * regionStr = "NTSC: ";
#endif
int lastLodLevel = 2;
short int QuickSelectTimeCurrent = 0;

#if DSCRPRINT
#define MAX_DEBUG_SCR_PRINT_LINES       (16)
#define MAX_DEBUG_SCR_PRINT_LINE_LEN    (64)
char dscrprintlines[MAX_DEBUG_SCR_PRINT_LINES][MAX_DEBUG_SCR_PRINT_LINE_LEN];
int dscrprintlinescount = 0;
#endif

extern float _lodScale;
extern void* _correctTieLod;
extern int _correctTieLod_Jump;
extern VariableAddress_t vaPlayerRespawnFunc;
extern VariableAddress_t vaPlayerSetPosRotFunc;
extern VariableAddress_t vaFlagUpdate_Func;
extern MenuElem_ListData_t dataCustomMaps;
extern SelectedCustomMapId;
extern scavHuntEnabled;
extern scavHuntShownPopup;

PatchConfig_t config __attribute__((section(".config"))) = {
	.enableAutoMaps = 0,
	.disableCameraShake = 0,
	.levelOfDetail = 2,
	.enableFpsCounter = 0,
	.playerFov = 0,
	.enableSpectate = 0,
	.alwaysShowHealth = 0,
	.mapScoreToggle_MapBtn = 0,
	.mapScoreToggle_ScoreBtn = 0,
	.disableScavengerHunt = 0,
	.enableSingleplayerMusic = 0,
	.quickSelectTimeDelay = 0,
	.aimAssist = 0,
	.cycleWeapon1 = 0,
	.cycleWeapon2 = 0,
	.cycleWeapon3 = 0,
	.hypershotEquipButton = 0,
	.disableDpadMovement = 0
};

PatchGameConfig_t gameConfig;
PatchGameConfig_t gameConfigHostBackup;
PatchPatches_t patched;
VoteToEndState_t voteToEndState;
PatchStateContainer_t patchStateContainer;

PatchPointers_t patchPointers = {
  .ServerTimeMonth = 0,
  .ServerTimeDay = 0,
  .ServerTimeHour = 0,
  .ServerTimeMinute = 0,
  .ServerTimeSecond = 0,
};

struct FlagPVars
{
	VECTOR BasePosition;
	short CarrierIdx;
	short LastCarrierIdx;
	short Team;
	char UNK_16[6];
	int TimeFlagDropped;
};

typedef struct RankTable {
    // Deviation 1 (275.0)
    // Deviation 2 (250.0)
    // Deviation 3 (100.0)
    // Rank 1 (0.0 - 1000.0 - 1349.0)
    // Rank 2 (1350.0 - 1699.0)
    // Rank 3 (1700.0 and Above)
    float deviation[3];
    float range[3];
} RankTable_t;

#if DSCRPRINT
//------------------------------------------------------------------------------
void clearScrPrintLine(void)
{
  memset(dscrprintlines, 0, sizeof(dscrprintlines));
  dscrprintlinescount = 0;
}

//------------------------------------------------------------------------------
void pushScrPrintLine(char* str)
{
  int i = dscrprintlinescount;
  
  if (i >= MAX_DEBUG_SCR_PRINT_LINES) {

    memmove(&dscrprintlines[0], &dscrprintlines[1], (MAX_DEBUG_SCR_PRINT_LINES - 1) * MAX_DEBUG_SCR_PRINT_LINE_LEN * sizeof(char));
    i = MAX_DEBUG_SCR_PRINT_LINES - 1;
  }

  strncpy(dscrprintlines[i], str, MAX_DEBUG_SCR_PRINT_LINE_LEN);
  dscrprintlinescount = i + 1;
}
#endif

#if BENCHMARK
static long TestBegan = 0;
static long TestEnd = 0;

long getMsSinceTestBegan(void)
{
	return (timerGetSystemTime() - TestBegan) / SYSTEM_TIME_TICKS_PER_MS;
}
void benchmark_timePrint(char *title)
{
	printf("%s: %dms\n", title, (int)getMsSinceTestBegan());
	// reset time begun after each new printf
	TestBegan = timerGetSystemTime();
}
#endif

//------------------------------------------------------------------------------
int getMACAddress(u8 output[6])
{
	int i;
	static int hasMACAddress = 0;
	static u8 macAddress[6];
	u8 buf[16];

#if UYA_PAL
	void* cd = (void*)0x001D3CC0;
	void* net_buf = (void*)0x001D3D00;
#else
	void* cd = (void*)0x001D3E40;
	void* net_buf = (void*)0x001D3E80;
#endif

	// use cached result
	// since the MAC address isn't going to change in the lifetime of the patch
	if (hasMACAddress) {
		memcpy(output, macAddress, 6);
		return 1;
	}

	// sceInetInterfaceControl get physical address
	int r = netInterfaceControl(cd, net_buf, 1, 13, buf, sizeof(buf));
	if (r) {
		memset(output, 0, 6);
		return 0;
	}

	//
	memcpy(macAddress, buf, 6);
	memcpy(output, buf, 6);
	DPRINTF("mac %02X:%02X:%02X:%02X:%02X:%02X\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
	return hasMACAddress = 1;
}

//------------------------------------------------------------------------------
void sendMACAddress(void)
{
	static int sent = 0;
	static int stall = 0;
	u8 mac[6];

	void * connection = netGetLobbyServerConnection();
	if (!connection) { sent = 0; return; }

	if (sent) return;
	if (!getMACAddress(mac)) return;

	// stall
	if (stall) { stall--; return; }

	// send
	if (netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_SET_MACHINE_ID, sizeof(mac), mac)) { stall = 60; return; }
	sent = 1;
	DPRINTF("sent mac\n");
}

//------------------------------------------------------------------------------
void requestServerTime(void)
{
  void* connection = netGetLobbyServerConnection();
	if (!connection) return;
	
	netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_REQUEST_DATE_SETTINGS, 0, NULL);
}

//------------------------------------------------------------------------------
void onServerTimeResponse(void* connection, void* data)
{
	DateResponse_t response;
	if (!connection) return;

	memcpy(&response, data, sizeof(DateResponse_t));
	DPRINTF("\nDate: %d/%d\nTime: %02d:%02d", response.Month, response.Day, response.Hour, response.Minute);
	patchPointers.ServerTimeMonth = response.Month;
	// not adding 1 to the date broke things on January 31st.
	patchPointers.ServerTimeDay = response.Day;
	patchPointers.ServerTimeHour = response.Hour;
	patchPointers.ServerTimeMinute = response.Minute;
	patchPointers.ServerTimeSecond = response.Second;
}

//------------------------------------------------------------------------------
char * checkGameType(void)
{
	if (isInMenus() || isInGame()) {
		GameSettings * gs = gameGetSettings();
		if (gs) {
			switch(gs->GameType) {
				case GAMERULE_SIEGE: return "Siege at ";
				case GAMERULE_CTF:  return "CTF at ";
				case GAMERULE_DM:  return "DM at ";
			}
		}
	}
	// if Not in in game or menus or gamesettings not found.
	return "";
}
char * checkMap(void)
{
	if (isInMenus()) {
		if (isInStaging) {
			location = LOCATION_STAGING;
			return "Staging";
		} else {
			location = LOCATION_ONLINE_LOBBY;
			return "Online Lobby";
		}
	} else if (isInGame()) {
		location = LOCATION_IN_GAME;
		if (SelectedCustomMapId > 0)
			return MapLoaderState.MapName;

		return mapGetName(gameGetCurrentMapId());
	} else {
		location = LOCATION_LOADING;
		return "Loading Screen";
	}
}
void runExceptionHandler(void)
{
	// invoke exception display installer
	if (*(u32*)EXCEPTION_DISPLAY_ADDR != 0) {
		if (!hasInstalledExceptionHandler) {
			((void (*)(void))EXCEPTION_DISPLAY_ADDR)();
			hasInstalledExceptionHandler = 1;
		}

		char * mapStr = checkMap();		// change "a fatal error as occured." to region and map.
		strncpy((char*)(EXCEPTION_DISPLAY_ADDR + 0x794), regionStr, 6);
		strncpy((char*)(EXCEPTION_DISPLAY_ADDR + 0x79a), mapStr, 20);
		
		// change display to match progressive scan resolution
		if (gfxGetIsProgressiveScan()) {
			*(u16*)(EXCEPTION_DISPLAY_ADDR + 0x9F4) = 0x0083;
			*(u16*)(EXCEPTION_DISPLAY_ADDR + 0x9F8) = 0x210E;
		} else {
			*(u16*)(EXCEPTION_DISPLAY_ADDR + 0x9F4) = 0x0183;
			*(u16*)(EXCEPTION_DISPLAY_ADDR + 0x9F8) = 0x2278;
		}
	}
}

/*
 * NAME :		botsInGame
 * DESCRIPTION :Returns 1 if bots are in game. returns 0 if not.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int botsInGame(void)
{
	GameSettings *gs = gameGetSettings();
	if (!gs)
		return 0;

	int i;
	for (i = 1; i < GAME_MAX_PLAYERS; ++i) {
		int account_id = gs->PlayerAccountIds[i];
		if (account_id >= 883 && account_id <= 1880) {
			DPRINTF("\nBOTS IN GAME! UH OH, AI COMIN' FOR YA!\n");
			return 1;
		}
	}
	return 0;
}

/*
 * NAME :		runCameraSpeedPatch
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runCameraSpeedPatch(void)
{
	char buf[12];
	const short MAX_CAMERA_SPEED = 300;
	if (isInMenus()) {
		// overwrite menu camera controls max cam speed
		// also display speed text next to input
		u32 ui = uiGetPointer(UIP_CONTROLS);
		if (ui && uiGetActivePointer(UIP_EDIT_PROFILE)) {
			u32 cameraRotationUIPtr = *(u32*)(ui + 0x11C);
			if (cameraRotationUIPtr) {
				// max speed
				*(u32*)(cameraRotationUIPtr + 0x7C) = MAX_CAMERA_SPEED;

				// draw %
				if (uiGetActiveSubPointer(UIP_CONTROLS)) {
					sprintf(buf, "%d%%", *(u32*)(cameraRotationUIPtr + 0x80));
					gfxScreenSpaceText(340, 310, 1, 1, 0x8069cbf2, buf, -1, 2);
				}
			}
		}
	} else if (isInGame()) {
		// replaces limiter function so that input can go past default 100
		u32 updateCameraSpeedIGFunc = GetAddress(&vaUpdateCameraSpeedIGFunc);
		*(u16*)(updateCameraSpeedIGFunc + 0x130) = MAX_CAMERA_SPEED;
		*(u16*)(updateCameraSpeedIGFunc + 0x154) = MAX_CAMERA_SPEED+1;

		// if start menu isn't open
		int p = playerGetFromSlot(0)->pauseOn;
		if (!p)
			return;

		// if not on correct pause screen
		u32 img = gfxGetPreLoadedImageBufferSource(0);
		if (img && *(int*)(img + 0xc) == 5) {
			// replace drawing function denominator to scale input down to 0 to our MAX
			u32 drawCameraSpeedInputIGFunc = GetAddress(&vaDrawCameraSpeedInputIGFunc);
			if (drawCameraSpeedInputIGFunc) {
				asm __volatile(
						"mtc1 %0, $f12\n"
						"cvt.s.w $f12, $f12\n"
						"mfc1 $t0, $f12\n"
						"srl $t0, $t0, 16\n"
						"sh $t0, 0(%1)"
						: : "r" (MAX_CAMERA_SPEED), "r" (drawCameraSpeedInputIGFunc)
				);
			}

			// Draw peercentage
			char buf[12];
			#ifdef UYA_PAL
				int PLAYER_ROTATION = 0x001a5894;
			#else
				int PLAYER_ROTATION = 0x001a5a14;
			#endif
			sprintf(buf, "%d%%", *(int*)PLAYER_ROTATION);
			gfxScreenSpaceText(273, 0.504 * SCREEN_WIDTH, 1, 1, 0x8069cbf2, buf, -1, 2);
		}
	}
}

/*
 * NAME :		patchKillStealing_Hook
 * DESCRIPTION :
 * 			Filters out hits when player is already dead.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int patchKillStealing_Hook(Player * target, Moby * damageSource, u64 a2)
{
	// if player is already dead return 0
	if (target->pNetPlayer->pNetPlayerData->hitPoints <= 0)
		return 0;

	// pass through
	return ((int (*)(Player *, Moby *, u64))GetAddress(&vaWhoHitMeFunc))(target, damageSource, a2);
}
/*
 * NAME :		patchKillStealing
 * DESCRIPTION :
 * 			Patches who hit me on weapon hit with patchKillStealing_Hook.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchKillStealing(void)
{
	if (patched.killStealing)
		return;

	HOOK_JAL(GetAddress(&vaWhoHitMeHook), &patchKillStealing_Hook);
	patched.killStealing = 1;
}

/*
 * NAME :		patchDeadJumping
 * DESCRIPTION :
 * 			Patches Dead Jumping by setting the can't move timer.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchDeadJumping(void)
{
	Player ** players = playerGetAll();
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
    	if (!players[i])
    		continue;

		Player * player = players[i];
		if (player->isLocal && (playerIsDead(player) || playerGetHealth(player) <= 0)) {
			// get current player state
			int PlayerState = playerDeobfuscate(&player->state, 0, 0);
			// if player is on bolt crank, set player state to idle.
			if (PlayerState == PLAYER_STATE_BOLT_CRANK)
				playerSetPlayerState(player, PLAYER_STATE_IDLE);

			player->timers.noInput = 10;
		}
	}
}

/*
 * NAME :		patchDeadShooting_Hook
 * DESCRIPTION :
 * 			If player is dead, don't let them shoot.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int patchDeadShooting_Hook(int pad)
{
	// Get player struct from pad
	int pStruct = (*(u32*)((u32)pad + 0x570)) - 0x430c;
	Player * p = (Player*)pStruct;
	// if player health is zero or less, return 0.
	if (p->pNetPlayer->pNetPlayerData->hitPoints <= 0)
		return 0;

	// If not dead, run normal function.
	return ((int (*)(int))GetAddress(&vaPatchDeadShooting_ShootingFunc))(pad);
}

/*
 * NAME :		patchDeathShooting
 * DESCRIPTION :
 * 			Patches the shooting hook with patchDeadShooting_Hook
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchDeadShooting(void)
{
	if (patched.deadShooting)
		return;

	HOOK_JAL(GetAddress(&vaPatchDeadShooting_ShootingHook), &patchDeadShooting_Hook);
	patched.deadShooting = 1;
}

int patchSniperWallSniping_Hook(VECTOR from, VECTOR to, Moby* shotMoby, Moby* moby, u64 t0)
{
	// hit through terrain
	// if we've hit a target
	// we check if we've hit by reading the source guber event
	// which is passed in 0x5C of the shot's pvars
	if (shotMoby && shotMoby->pVar) {
		int shotOwner = *(u32*)((u32)shotMoby + 0x90) >> 28;
		if (shotOwner != gameGetMyClientId()) {
		void * event = *(void**)(shotMoby->pVar + 0x5C);
			if (event) {
				u32 hitGuberUid = *(u32*)(event + 0x3C);
				if (hitGuberUid != 0xFFFFFFFF) {
					return CollLine_Fix(from, to, 1, moby, t0);
				}
			}
		}
	}

	// pass through
	return CollLine_Fix(from, to, 0, moby, t0);
}

/*
 * NAME :		patchSniperWallSniping
 * DESCRIPTION :
 * 			Send Weapon Shots more reliably.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchSniperWallSniping(void)
{
	if (gameConfig.grFluxShotsAlwaysHit) {
		// hook when collision checking is done on the sniper shot
		u32 hookAddr = GetAddress(&vaSniperShotCollLineFixHook);
		if (hookAddr) {
			POKE_U32(hookAddr + 0x04, 0x0260302D);
			HOOK_JAL(hookAddr, &patchSniperWallSniping_Hook);
		}

		// change sniper shot initialization code to write the guber event to the shot's pvars
		// for use later by patchSniperWallSniping_Hook
		hookAddr = GetAddress(&vaSniperShotCreatedHook);
		if (hookAddr) {
			POKE_U32(hookAddr, 0xAE35005C);
		}
	}
}

void patchSniperNiking_Hook(float f12, VECTOR out, VECTOR in, void * event)
{
	// call base
	((void (*)(float, VECTOR, VECTOR))GetAddress(&vaGetSniperShotDirectionFunc))(f12 * 0.01666666666, out, in);

#if UYA_PAL
	Moby** collHitMoby = (Moby**)0x0025b898;
#else
	Moby** collHitMoby = (Moby**)0x0025ba18;
#endif

	if (event) {
		int hitGuberId = *(int*)(event + 0x3C);
		int sourceId = *(u8*)(event + 0x36) & 0xF;
		if (sourceId != gameGetMyClientId()) {
			DPRINTF("sniper hit %08X\n", hitGuberId);

			if (hitGuberId != -1) {
				// hit something
				Moby* hitMoby = mobyGetByGuberUid(hitGuberId);
				if (hitMoby) {
					DPRINTF("sniper hit %08X\n", (u32)hitMoby);
					vector_subtract(out, hitMoby->position, (float*)event);
					out[2] += 0.5;
					return;
				}
			}
		}
	}
}

/*
 * NAME :		patchSniperNiking
 * DESCRIPTION :
 * 			Send Weapon Shots more reliably.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchSniperNiking(void)
{
	u32 hookAddr = GetAddress(&vaGetSniperShotDirectionHook);
	if (hookAddr) {
		POKE_U32(hookAddr - 0x0C, 0x46000306);
		POKE_U32(hookAddr + 0x04, 0x02803021);
		HOOK_JAL(hookAddr, &patchSniperNiking_Hook);
	}
}

/*
 * NAME :		patchWeaponShotLag
 * DESCRIPTION :
 * 			Send Weapon Shots more reliably.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchWeaponShotLag(void)
{
	if (patched.weaponShotLag)
		return;

	int TCP = 0x24040040;

	// Send all weapon shots reliably (Use TCP instead of UDP)
	// int AllWeaponsAddr = GetAddress(&vaAllWeaponsUDPtoTCP);
	// if (*(u32*)AllWeaponsAddr == 0x906407D4)
	// 	*(u32*)AllWeaponsAddr = TCP;

	// Send Flux shots reliably (Use TCP instead of UDP)
	int FluxAddr = GetAddress(&vaFluxUDPtoTCP);
	if (*(u32*)FluxAddr == 0x90A407D4)
		*(u32*)FluxAddr = TCP;

	patched.weaponShotLag = 1;
}

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

/*
 * NAME :		patchLevelOfDetail
 * DESCRIPTION :
 * 			Sets the level of detail.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchLevelOfDetail(void)
{
	if (!patched.config.levelOfDetail) {
		u32 hook = GetAddress(&vaLevelOfDetail_Hook);
		if (*(u32*)hook == 0x02C3B020) {
			HOOK_J(hook, &_correctTieLod);
			// patch jump instruction in correctTieLod to jump back to needed address.
			u32 val = ((u32)hook + 0x8);
			*(u32*)(&_correctTieLod_Jump) = 0x08000000 | (val / 4);
		}

		patched.config.levelOfDetail = 1;
	}

	int lod = config.levelOfDetail;
	int lodChanged = lod != lastLodLevel;
	switch (lod) {
		case 0: // Potato
		{
			_lodScale = 0.2;
			int TerrainTiesDistance = 320;
			int ShrubDistance = 500;
			if (lodChanged) {
				u32 LOD_Shrubs = GetAddress(&vaLevelOfDetail_Shrubs);
				u32 LOD_Ties = GetAddress(&vaLevelOfDetail_Ties);
				u32 LOD_Terrain = GetAddress(&vaLevelOfDetail_Terrain);
				*(float*)LOD_Shrubs = ShrubDistance;
				*(u32*)LOD_Ties = TerrainTiesDistance;
				*(float*)LOD_Terrain = TerrainTiesDistance * 1024;
			}
			break;
		}
		case 1: // Low
				{
			_lodScale = 0.4;
			int TerrainTiesDistance = 480;
			int ShrubDistance = 250;
			if (lodChanged) {
				u32 LOD_Shrubs = GetAddress(&vaLevelOfDetail_Shrubs);
				u32 LOD_Ties = GetAddress(&vaLevelOfDetail_Ties);
				u32 LOD_Terrain = GetAddress(&vaLevelOfDetail_Terrain);
				*(float*)LOD_Shrubs = ShrubDistance;
				*(u32*)LOD_Ties = TerrainTiesDistance;
				*(float*)LOD_Terrain = TerrainTiesDistance * 1024;
			}
			break;
		}
		case 2: // Normal
		{
			_lodScale = 1.0;
			int TerrainTiesDistance = 960;
			int ShrubDistance = 500;
			if (lodChanged) {
				u32 LOD_Shrubs = GetAddress(&vaLevelOfDetail_Shrubs);
				u32 LOD_Ties = GetAddress(&vaLevelOfDetail_Ties);
				u32 LOD_Terrain = GetAddress(&vaLevelOfDetail_Terrain);
				*(float*)LOD_Shrubs = ShrubDistance;
				*(u32*)LOD_Ties = TerrainTiesDistance;
				*(float*)LOD_Terrain = TerrainTiesDistance * 1024;
			}

			break;
		}
		case 3: // High
		{
			_lodScale = 5.0;
			int TerrainTiesDistance = 4800;
			int ShrubDistance = 2500;
			if (lodChanged) {
				u32 LOD_Shrubs = GetAddress(&vaLevelOfDetail_Shrubs);
				u32 LOD_Ties = GetAddress(&vaLevelOfDetail_Ties);
				u32 LOD_Terrain = GetAddress(&vaLevelOfDetail_Terrain);
				*(float*)LOD_Shrubs = ShrubDistance;
				*(u32*)LOD_Ties = TerrainTiesDistance;
				*(float*)LOD_Terrain = TerrainTiesDistance * 1024;
			}
			break;
		}
	}
	// backup lod
	lastLodLevel = config.levelOfDetail;
}

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
			weaponOrderBackup[0][i] = playerDeobfuscate(&player->quickSelect.Slot[i], 1, 1);
	}

	// call hooked WeaponStripMe function after backup
	playerStripWeapons(player);
}

int patchResurrectWeaponOrdering_ConvertToWeaponId(int id)
{
	int weapon;
	switch (id) {
		case 8: return WEAPON_ID_MORPH;
		case 9:	return WEAPON_ID_HOLO;
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
	int index = player->fps.vars.cam_slot;
	// Set loadout/cycle weapons.  If not chosen, it will be set to default cycle.
	char cycle[] = {
		config.cycleWeapon1 > 0 ? patchResurrectWeaponOrdering_ConvertToWeaponId(config.cycleWeapon1) : WEAPON_ID_GBOMB,
		config.cycleWeapon2 > 0 ? patchResurrectWeaponOrdering_ConvertToWeaponId(config.cycleWeapon2) : WEAPON_ID_BLITZ,
		config.cycleWeapon3 > 0 ? patchResurrectWeaponOrdering_ConvertToWeaponId(config.cycleWeapon3) : WEAPON_ID_FLUX
	};

	// if Loadout Weapons Only is not on
	if (!gameConfig.prLoadoutWeaponsOnly) {
		// then try and overwrite given weapon order if weapons match equipped weapons before death
		playerGiveRandomWeapons(player, weaponCount);
		
		// restore backup if they match (regardless of order) newly assigned weapons
		for (i = 0; i < 3; ++i) {
			// if respawned weapons match backup weapons
			u8 backedUpSlotValue = weaponOrderBackup[index][i];
			for(j = 0; j < 3; ++j) {
				if (backedUpSlotValue == playerDeobfuscate(&player->quickSelect.Slot[j], 1, 1)) {
					++matchCount;
					DPRINTF("\ni: %d, j: %d, Matched: %d", i, j, playerDeobfuscate(&player->quickSelect.Slot[j], 1, 1));
				}
			}
		}
		// if weaponOrderBackup matches respawnd weapons, then check for cycle weapons (regardless of order)
		if (matchCount == 3) {
			for (i = 0; i < 3; ++i) {
				u8 backedUpSlotValue = weaponOrderBackup[index][i];
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
			weaponOrderBackup[index][i] = cycle[i];
	}
	// we found a match, or loadout weapons only is on.
	if (matchCount == 3 || gameConfig.prLoadoutWeaponsOnly) {
		// set equipped weapon in order
		for (i = 0; i < 3; ++i)
			playerGiveWeapon(player, weaponOrderBackup[index][i], 1);

		// equip each weapon from last slot to first slot to keep correct order.
		for (i = 2; i >= 0; --i)
			playerEquipWeapon(player, weaponOrderBackup[index][i]);
	}
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

	patched.resurrectWeaponOrdering = 1;
}

/*
 * NAME :		runFpsCounter_Logic
 * DESCRIPTION :
 * 			Logic for the FPS counter and drawing the text to screen.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runFpsCounter_Logic(void)
{
	char buf[64];
	static int lastGameTime = 0;
	static int tickCounter = 0;

	// initialize time
	if (tickCounter == 0 && lastGameTime == 0)
		lastGameTime = gameGetTime();
	
	// update fps every FPS frames
	++tickCounter;
	if (tickCounter >= GAME_FPS)
	{
		int currentTime = gameGetTime();
		lastFps = tickCounter / ((currentTime - lastGameTime) / (float)TIME_SECOND);
		lastGameTime = currentTime;
		tickCounter = 0;
	}

	// render if enabled
	if (config.enableFpsCounter)
	{
		if (averageRenderTimeMs > 0) {
			snprintf(buf, 64, "EE: %.1fms GS: %.1fms FPS: %.2f", averageUpdateTimeMs, averageRenderTimeMs, lastFps);
		} else {
			snprintf(buf, 64, "FPS: %.2f", lastFps);
		}

		gfxScreenSpaceText(SCREEN_WIDTH - 5, 5, 0.75, 0.75, 0x80FFFFFF, buf, -1, 2);
	}
}

/*
 * NAME :		runFpsCounter_drawHook
 * DESCRIPTION :
 * 			Logic for the GS counter.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runFpsCounter_drawHook(void)
{
	static int renderTimeCounterMs = 0;
	static int frames = 0;
	static long ticksIntervalStarted = 0;

	long t0 = timerGetSystemTime();
	((void (*)(void))GetAddress(&vaFpsCounter_DrawFunc))();
	long t1 = timerGetSystemTime();

	renderTimeMs = (t1 - t0) / SYSTEM_TIME_TICKS_PER_MS;

	renderTimeCounterMs += renderTimeMs;
	++frames;

	// update every 500 ms
	if ((t1 - ticksIntervalStarted) > (SYSTEM_TIME_TICKS_PER_MS * 500)) {
		averageRenderTimeMs = renderTimeCounterMs / (float)frames;
		renderTimeCounterMs = 0;
		frames = 0;
		ticksIntervalStarted = t1;
	}
}

/*
 * NAME :		runFpsCounter_updateHook
 * DESCRIPTION :
 * 			Logic for the EE counter.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runFpsCounter_updateHook(void)
{
	static int updateTimeCounterMs = 0;
	static int frames = 0;
	static long ticksIntervalStarted = 0;

	long t0 = timerGetSystemTime();
	((void (*)(void))GetAddress(&vaFpsCounter_UpdateFunc))();
	long t1 = timerGetSystemTime();

	updateTimeMs = (t1 - t0) / SYSTEM_TIME_TICKS_PER_MS;

	updateTimeCounterMs += updateTimeMs;
	frames++;

	// update every 500 ms
	if ((t1 - ticksIntervalStarted) > (SYSTEM_TIME_TICKS_PER_MS * 500)) {
		averageUpdateTimeMs = updateTimeCounterMs / (float)frames;
		updateTimeCounterMs = 0;
		frames = 0;
		ticksIntervalStarted = t1;
	}
}

/*
 * NAME :		runFpsCounter
 * DESCRIPTION :
 * 			Hooks functions for showing the EE, GS and FPS.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runFpsCounter(void)
{
	u32 hook = GetAddress(&vaFpsCounter_Hooks);
	HOOK_JAL(hook, &runFpsCounter_updateHook);
	HOOK_JAL(((u32)hook + 0x60), &runFpsCounter_drawHook);

	runFpsCounter_Logic();
}

/*
 * NAME :		patchFov
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchFov(void)
{
	float normalFOV = 1.11;
	float newFOV = normalFOV + (config.playerFov / 10.0) * 1;
	Player *p = playerGetFromSlot(0);
	// if not in FPS View, set
	if (p->fps.active == 1) {
		p->camera->fov.ideal = newFOV;
		// set Patched FOV to false
		patched.config.playerFov = 0;
	} else {
		// if in FPS and not holding Flux, use new FOV, else use normal FOV.
		if (p->weaponHeldId != WEAPON_ID_FLUX) {
			// set to new FOV
			p->camera->fov.ideal = newFOV;
			patched.config.playerFov = 0;
		// if in FPS and patched.config.playerFov is false
		// or if the ideal camera is greater than the normal FOV.
		} else if (!patched.config.playerFov || p->camera->fov.ideal > normalFOV) {
			// set to normal FOV.
			p->camera->fov.ideal = normalFOV;
			patched.config.playerFov = 1;
		}
	}
	p->camera->fov.changeType = 3;
	p->camera->fov.state = 1;
}

/*
 * NAME :		patchUnkick_Logic
 * DESCRIPTION :
 * 				Logic behind fixingi the unkick exploit.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int patchUnkick_Logic(u32 uip)
{
#if UYA_PAL
	// run base command
	int r = ((int (*)(u32))0x00686e40)(uip);
#else
	// run base command
	int r = ((int (*)(u32))0x00684320)(uip);
#endif
	// which option is selected for popup.
	int selectedOption = 0x01d20234;
	int i;
	GameSettings * gs = gameGetSettings();
	if (gs) {
		int clientId = gameGetMyClientId();
		for (i = 1; i < GAME_MAX_PLAYERS; ++i) {
			// Check ClientID and if they are kicked and not viewing the kicked popup
			if (gs->PlayerClients[i] == clientId && gs->PlayerStates[i] == 5 && *(u32*)selectedOption != 3) {
				// Select "No" Option
				*(u32*)selectedOption = 2;
				// Press X
#if UYA_PAL
				*(u8*)0x00225800 = 0xbf;
#else
				*(u8*)0x00225983 = 0xbf;
#endif
			}
		}
	}
	return r;
}

/*
 * NAME :		patchUnkick
 * DESCRIPTION :
 * 				Hook for the patchUnkick_Logic.
 * 				Hooks into the popup loop.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchUnkick(void)
{
	// Hook into popup loop
#if UYA_PAL
	int hook = 0x006872b0;
#else
	int hook = 0x00684790;
#endif
	if (hook)
		HOOK_JAL(hook, &patchUnkick_Logic);
}

/*
 * NAME :		patchDeathBarrierBug
 * DESCRIPTION :
 * 				Patches death barrier bug/teleport glitch
 * 				that let players fall off the map into the base.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchDeathBarrierBug(void)
{
	int i;
	Player *player = playerGetFromSlot(0);
	GameSettings *gameSettings = gameGetSettings();
	// if Map doesn't match Metroplis, exit.
	if (gameSettings->GameLevel != MAP_ID_METROPOLIS)
		return;

	// if player is local
	if (player && player->isLocal) {
		float deathbarrier = gameGetDeathHeight();
		float pY = player->playerPosition[2];
		//DPRINTF("deathheight: %d\nplayery: %d\ninbasehack: %d\n", (int)deathbarrier, (int)pY, player->inBaseHack);
		// if player is above death barrier and inBaseHack equals 1.
		if (player->inBaseHack && deathbarrier < pY) {
			player->inBaseHack = 0;
		} else if (!player->inBaseHack && deathbarrier > pY) {
			player->inBaseHack = 1;
		}
	}
}

/*
 * NAME :		patchCreateGameMenu
 * DESCRIPTION :
 * 				Patches various options on the create game screen
 * 				such as higher frag limit and time limit.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchCreateGameMenu_Option(int option, int new_value)
{
	if (*(int*)option != new_value)
		*(int*)option = new_value;
}
// void patchCreateGameMenu_LastSettings(void)
// {
// 	u32 menu = uiGetPointer(UIP_CREATE_GAME);
// 	// Normal Options
// 	// Enable Password if Disabled
// 	int use_password = (*(u32*)(menu + 0x130) + 0x4);
// 	int password = (*(u32*)(menu + 0x134) + 0x4);
// 	// If "USE PASWORD" is "Yes" and also disabled to edit
// 	if (*(u32*)use_password == 2) {
// 		patchCreateGameMenu_Option(use_password, 3); // enable "use password"
// 		patchCreateGameMenu_Option(password, 3); // enable password option
// 	}

// 	// Advanced Options
// 	menu = uiGetPointer(UIP_CREATE_GAME_ADVANCED_OPTIONS);
// 	int nodes = (*(u32*)(menu + 0x114) + 0x64);
// 	patchCreateGameMenu_Option(nodes, lastNodes);

// 	lastSettings = 0;
// }
void patchCreateGameMenu(void)
{
	u32 menu = uiGetActivePointer(UIP_CREATE_GAME);
	if (!menu) return;

	// Patch LastSettings options
	// printf("\nlastsettings: %d", lastSettings);
	// if (lastSettings)
	// 	patchCreateGameMenu_LastSettings();

	// Modify Normal Options
// 	int time_limit = (*(u32*)(menu + 0x12c) + 0x70);
// 	patchCreateGameMenu_Option(time_limit, 120); // Time Limit = 120 Minutes
// 	// Assembly time limit max (if frag limit = none, it reads from this)
// #if UYA_PAL
// 	*(u32*)0x0069aa28 = 0x24020078;
// #else
// 	*(u32*)0x00698218 = 0x24020078;
// #endif

	// if in Advanced Options
	menu = uiGetActiveSubPointer(UIP_CREATE_GAME_ADVANCED_OPTIONS);
	if (!menu) return;
	int frag_limit = (*(u32*)(menu + 0x12c) + 0x70);
	patchCreateGameMenu_Option(frag_limit, 50); // Frag Limit = 50
}

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
	Player *player = (Player *)PLAYER_STRUCT;
	u32 old_value = 0xae002514; // sw zero,0x2514(s0)
	if (config.alwaysShowHealth && *(u32*)healthbar_timer == old_value) {
		*(u32*)healthbar_timer = 0;
		player->hudHealthTimer = 3 * GAME_FPS;
	} else if (!config.alwaysShowHealth && *(u32*)healthbar_timer == 0) {
		*(u32*)healthbar_timer = old_value;
	}
}

/*
 * NAME :		patchMapAndScoreboardToggle
 * DESCRIPTION :
 * 				Lets the player choose how they want to open the
 * 				Scoreboard or Map (via Select, L3 or R3)
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchMapAndScoreboardToggle(void)
{
	static int ShowScoreboard = 1;
	static int ShowMap = 1;
	int ScoreboardToggle = 0;
	int MapToggle = 0;
	GameSettings * gameSettings = gameGetSettings();
	if (!gameSettings)
		return;

	if (!config.mapScoreToggle_ScoreBtn && !config.mapScoreToggle_MapBtn)
		return;

	switch (config.mapScoreToggle_ScoreBtn) {
		case 0: ScoreboardToggle = -1; break;
		case 1: ScoreboardToggle = PAD_SELECT; break;
		case 2: ScoreboardToggle = PAD_L3; break;
		case 3: ScoreboardToggle = PAD_R3; break;
	}
	switch (config.mapScoreToggle_MapBtn) {
		case 0: MapToggle = -1; break;
		case 1: MapToggle = PAD_SELECT; break;
		case 2: MapToggle = PAD_L3; break;
		case 3: MapToggle = PAD_R3; break;
	}

	// Disable Select Button for Toggling original Map/Scoreboard
	if (MapToggle != -1 && ScoreboardToggle != -1)
		POKE_U32(GetAddress(&vaMapScore_SelectBtn_Addr), 0);
	else
		POKE_U32(GetAddress(&vaMapScore_SelectBtn_Addr), GetAddress(&vaMapScore_SelectBtn_Val));

	// If Scoreboard Button Toggle isn't set to "Default"
	if (ScoreboardToggle != -1) {
		// Run Scoreboard Main Logic
		((void (*)(int, int))GetAddress(&vaMapScore_SeigeCTFScoreboard_AlwaysRun))(0, 10);
	
		// if Scoreboard's chosen button is pressed
		if (padGetButtonDown(0, ScoreboardToggle) > 0) {
			((void (*)(int, int))GetAddress(&vaMapScore_ScoreboardToggle))(0, ShowScoreboard);
			ShowScoreboard = !ShowScoreboard;
		}
	}
	// Check to see if Level ID is less than or equal to blackwater docks, or if not on custom map.
	// This is due to Aquatos and Marcadia not having a mini-map.
	if (gameSettings->GameLevel <= MAP_ID_BLACKWATER_DOCKS || SelectedCustomMapId > 0) {
		// If Maps Button Toggle isn't set to "Default"
		if (MapToggle != -1) {
			// Run Map Main Logic only if gametype is deathmatch.
			if (gameSettings->GameType == GAMERULE_DM)
				((void (*)(int, int))GetAddress(&vaMapScore_SeigeCTFMap_AlwaysRun))(0, 10);

			// if Maps chosen button is pressed
			if (padGetButtonDown(0, MapToggle) > 0) {
				((void (*)(int, int))GetAddress(&vaMapScore_MapToggle))(0, ShowMap);
				ShowMap = !ShowMap;
			}
		}
	}
}

/*
 * NAME :		flagHandlePickup
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void flagHandlePickup(Moby* flagMoby, int pIdx)
{
	Player** players = playerGetAll();
	Player* player = players[pIdx];
	if (!player || !flagMoby)
		return;
	
	struct FlagPVars* pvars = (struct FlagPVars*)flagMoby->pVar;
	if (!pvars)
		return;

	// fi flag state isn't 1
	if (flagMoby->state != 1)
		return;

	// flag is currently returning
	if (flagIsReturning(flagMoby))
		return;

	// flag is currently being picked up
	if (flagIsBeingPickedUp(flagMoby))
		return;

	// only allow actions by living players
	if (playerIsDead(player))
		return;

	// Handle pickup/return
	if (player->mpTeam == pvars->Team) {
		flagReturnToBase(flagMoby, 0, pIdx);
	} else {
		flagPickup(flagMoby, pIdx);
		player->flagMoby = flagMoby;
	}
	DPRINTF("player %d picked up flag %X at %d\n", player->mpIndex, flagMoby->oClass, gameGetTime());
}

/*
 * NAME :		flagRequestPickup
 * DESCRIPTION :
 * 			Requests to either pickup or return the given flag.
 * 			If host, this request is automatically passed to the handler.
 * 			If not host, this request is sent over the net to the host.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void flagRequestPickup(Moby* flagMoby, int pIdx)
{
	static int requestCounters[GAME_MAX_PLAYERS] = {0,0,0,0,0,0,0,0};
	Player** players = playerGetAll();
	Player* player = players[pIdx];
	if (!player || !flagMoby)
		return;
	
	struct FlagPVars* pvars = (struct FlagPVars*)flagMoby->pVar;
	if (!pvars)
		return;

	if (gameAmIHost()) {
		// handle locally
		flagHandlePickup(flagMoby, pIdx);
	} else if (requestCounters[pIdx] == 0) {
		// send request to host
		void* dmeConnection = netGetDmeServerConnection();
		if (dmeConnection) {
			ClientRequestPickUpFlag_t msg;
			msg.GameTime = gameGetTime();
			msg.PlayerId = player->mpIndex;
			msg.FlagUID = guberGetUID(flagMoby);
			netSendCustomAppMessage(dmeConnection, gameGetHostId(), CUSTOM_MSG_ID_FLAG_REQUEST_PICKUP, sizeof(ClientRequestPickUpFlag_t), &msg);
			requestCounters[pIdx] = 10;
			DPRINTF("sent request flag pickup %d\n", gameGetTime());
		}
	}
	else
	{
		requestCounters[pIdx]--;
	}
}

/*
 * NAME :		customFlagLogic
 * DESCRIPTION :
 * 			Reimplements flag pickup logic but runs through our host authoritative logic.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void customFlagLogic(Moby* flagMoby)
{
	VECTOR t;
	int i;
	Player** players = playerGetAll();
	int gameTime = gameGetTime();
	GameOptions* gameOptions = gameGetOptions();
	struct FlagPVars* pvars = (struct FlagPVars*)flagMoby->pVar;

	// if flag moby or pvars don't exist, stop.
	if (!flagMoby || !pvars)
		return;

    // if flag state is not 1 (being picked up) and if flag is returning to base
    if (flagMoby->state != 1 || flagIsReturning(flagMoby))
        return;

    // flag is being picked up
    if (flagIsBeingPickedUp(flagMoby))
        return;
    
	// return to base if flag has been idle for 40 seconds and not already at base.
	if ((pvars->TimeFlagDropped + (TIME_SECOND * 40)) < gameTime && !flagIsAtBase(flagMoby) && !flagIsAtBase(flagMoby)) {
		flagReturnToBase(flagMoby, 0, 0xff);
		return;
	}

    if ((pvars->TimeFlagDropped + (TIME_SECOND * 1.5)) > gameTime)
        return;

    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player* player = players[i];
        if (!player)
            continue;

        // Don't allow input from players whom are dead
        if (playerDeobfuscate(&player->stateType, 0, 0) == PLAYER_TYPE_DEATH)
            return;

        // skip player if they've only been alive for < 180ms
        if (player->timers.timeAlive < 180)
            return;

        // skip if player state is in vehicle and critterMode is on
		if (player->camera && player->camera->camHeroData.critterMode)
			continue;

    	// skip player if in vehicle
		if (player->vehicle && playerDeobfuscate(&player->state, 0, 0) == PLAYER_STATE_VEHICLE)
			continue;

        // skip if player is on teleport pad
		// AQuATOS BUG: player->ground.pMoby points to wrong area
		if (player->ground.pMoby && player->ground.pMoby->oClass == MOBY_ID_TELEPORT_PAD)
			continue;

		// player must be within 2 units of flag
		vector_subtract(t, flagMoby->position, player->playerPosition);
		float sqrDistance = vector_sqrmag(t);
		if (sqrDistance > (2*2))
			continue;

		// player is on different team than flag and player isn't already holding flag
		if (player->mpTeam != pvars->Team) {
			if (!player->flagMoby) {
				flagRequestPickup(flagMoby, player->mpIndex);
				return;
			}
		} else {
			// if player is on same team as flag and close enough to return it
			vector_subtract(t, pvars->BasePosition, flagMoby->position);
			float sqrDistanceToBase = vector_sqrmag(t);
			if (sqrDistanceToBase > 0.1) {
				flagRequestPickup(flagMoby, player->mpIndex);
				return;
			}
		}
	}
}

/*
 * NAME :		onRemoteClientRequestPickUpFlag
 * DESCRIPTION :
 * 			Handles when a remote client sends the CUSTOM_MSG_ID_FLAG_REQUEST_PICKUP message.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int onRemoteClientRequestPickUpFlag(void * connection, void * data)
{
	int i;
	ClientRequestPickUpFlag_t msg;
	Player** players;
	memcpy(&msg, data, sizeof(msg));

	// ignore if not in game
	if (!isInGame() || !gameAmIHost())
		return sizeof(ClientRequestPickUpFlag_t);

	DPRINTF("remote player %d requested pick up flag %X at %d\n", msg.PlayerId, msg.FlagUID, msg.GameTime);

	// get list of players
	players = playerGetAll();

	// get remote player or ignore message
	Player** allRemote = playerGetAll();
	Player* remotePlayer = allRemote[msg.PlayerId];
	if (!remotePlayer)
		return sizeof(ClientRequestPickUpFlag_t);

	// get flag
	GuberMoby* gm = (GuberMoby*)guberGetObjectByUID(msg.FlagUID);
	if (gm && gm->Moby) {
		Moby* flagMoby = gm->Moby;
		flagHandlePickup(flagMoby, msg.PlayerId);
	}
	return sizeof(ClientRequestPickUpFlag_t);
}
/*
 * NAME :		patchCTFFlag
 * DESCRIPTION :
 * 			Patch CTF Flag update function with our own
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchCTFFlag(void)
{
	if (!isInGame())
		return;

	VECTOR t;
	int i = 0;
	Player** players = playerGetAll();


	u32 flagFunc = 0;
	if (!patched.ctfLogic) {
		netInstallCustomMsgHandler(CUSTOM_MSG_ID_FLAG_REQUEST_PICKUP, &onRemoteClientRequestPickUpFlag);
		if (!flagFunc)
			flagFunc = GetAddress(&vaFlagUpdate_Func);
		
		if (flagFunc) {
			*(u32*)flagFunc = 0x03e00008;
			*(u32*)(flagFunc + 0x4) = 0x0000102D;
		}
		patched.ctfLogic = 1;
	}

	GuberMoby* gm = guberMobyGetFirst();
	while (gm) {
		if (gm->Moby) {
			switch (gm->Moby->oClass) {
				case MOBY_ID_CTF_RED_FLAG:
				case MOBY_ID_CTF_BLUE_FLAG:
				{
					customFlagLogic(gm->Moby);
					// Master * master = masterGet(gm->Guber.Id.UID);
					// if (master)
					// 	masterDelete(master);

					break;
				}
			}
		}
		gm = (GuberMoby*)gm->Guber.Next;
	}
}

/*
 * NAME :		patchQuickSelectTimer
 * DESCRIPTION :	Uses a timer for quick select, so I can fine tune the delay of opening it.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int quickSelectTimer(int a0)
{
	if (config.quickSelectTimeDelay > 0) {
		if (QuickSelectTimeCurrent < config.quickSelectTimeDelay)
			++QuickSelectTimeCurrent;

		return QuickSelectTimeCurrent == config.quickSelectTimeDelay;
	}

	// return if quickSelectTimeDelay is off. 
	return ((int (*)(int))GetAddress(&vaQuickSelectCheck_Func))(a0);
}
void patchQuickSelectTimer(void)
{
	if (!patched.config.quickSelectTimeDelay) {
		HOOK_JAL(GetAddress(&vaQuickSelectCheck_Hook), &quickSelectTimer);
		patched.config.quickSelectTimeDelay = 1;
	}
	if (config.quickSelectTimeDelay) {
		Player* p = playerGetFromSlot(0);
		if (playerPadGetButtonUp(p, PAD_TRIANGLE))
			QuickSelectTimeCurrent = 0;
	}
}

/*
 * NAME :		runCampaignMusic
 * DESCRIPTION :	Adds Singple Player Tracks to Multiplayer
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runCampaignMusic(void)
{
	static int CustomSector = 0x1d8a;
	static int FinishedConvertingTracks = 0;
	static char SetupMusic = 0;
	int DefaultMultiplayerTracks = 13;
	static int TotalTracks = 0;
	static int AddedTracks = 0;
	int SetRangeMaxData = 0;
	static short CurrentTrack = 0;
	static short NextTrack = 0;
	// We go by each wad because we have to have Multiplayer one first.
	static short wadArray[][2] = {
		// wad, song per wad
		// Commented tracks/sectors are due to them being dialoge or messed up.
		{0x54d, 14}, // Multiplayer
		{0x3f9, 2}, // Veldin
		{0x403, 4}, // Florana
		{0x40d, 3}, // Starship Phoenix
		{0x417, 3}, // Marcadia
		{0x421, 4}, // Daxx (This doesn't have padding between music and dialog)
		{0x42b, 1}, // 
		{0x435, 4}, // Annihilation Nation
		{0x43f, 1}, // Aquatos
		{0x449, 1}, // Tyhrranosis
		{0x453, 1}, // Zeldrin Starport
		{0x45d, 1}, // Obani Gemini
		{0x467, 1}, // Blackwater City
		{0x471, 2}, // Holostar Studios
		{0x47b, 1}, // Koros
		{0x485, 1}, // Metropolis
		{0x48f, 1}, // Crash Site
		{0x499, 1}, // Aridia
		{0x4a3, 1}, // Quark's Hideout
		{0x4ad, 1}, // Mylon - Bioliterator
		{0x4b7, 2}, // Obani Draco
		{0x4c1, 1}, // Mylon - Command Center
		{0x4cb, 1}, // 
		{0x4d5, 1}, // Insomniac Museum
		// {0x4df, 0}, // 
		{0x4e9, 1}, // 
		{0x4f3, 1}, // 
		{0x4fd, 1}, // 
		{0x507, 1}, //
		{0x511, 1}, // 
		{0x51b, 1}, // 
		{0x525, 1}, // 
		{0x52f, 1}, // 
		{0x539, 1}, // 
		{0x543, 1}  // 
	};
	// if music isn't loaded or enable singleplayermusic isn't on, or in menus, return.
	u32 CodeSegmentPointer = *(u32*)0x01FFFD00;
	#if UYA_PAL
	int CodeSegmentPointerValue = 0x00575CC8;
	#else
	int CodeSegmentPointerValue = 0x00574F88;
	#endif
	if (!musicGetSector() || CodeSegmentPointer == CodeSegmentPointerValue)
		return;
	
	if (config.enableSingleplayerMusic) {
		u32 NewTracksLocation = 0x001F8588; // Overwrites current tracks too.
		if (!FinishedConvertingTracks) {
			// Set custom Sector
			if (musicGetSector() != CustomSector)
				musicSetSector(CustomSector);

			int MultiplayerSector = *(u32*)0x001F8584;
			int Stack = 0x000269300;
			// int WAD_Table = 0x001f7f88; // Kept for historical purposes.
			int a;
			// Zero out stack by the appropriate heap size (0x2a0 in this case)
			// This makes sure we get the correct values we need later on.
			memset((u32*)Stack, 0, 0x1818);

			// Loop through each WAD ID
			for(a = 0; a < (sizeof(wadArray)/sizeof(wadArray[0])); a++) {
				int WAD = wadArray[a][0];
				// Check if Map Sector is not zero
				if (WAD != 0) {
					internal_wadGetSectors(WAD, 1, Stack);
					int WAD_Sector = *(u32*)(Stack + 0x4);

					// make sure WAD_Sector isn't zero
					if (WAD_Sector != 0) {
						DPRINTF("WAD: 0x%X\n", WAD);
						DPRINTF("WAD Sector: 0x%X\n", WAD_Sector);

						// do music stuffs~
						// Get SP 2 MP Offset for current WAD_Sector.
						// In UYA we use our own sector.
						int SP2MP = WAD_Sector - CustomSector;
						// Remember we skip the first track because it is the start of the sp track, not the body of it.
						int b = 0;
						int Songs = Stack + (a == 0 ? 0x8 : 0x18);
						int j;
						for (j = 0; j < wadArray[a][1]; ++j) {
							int Track_LeftAudio = *(u32*)(Songs + b);
							int Track_RightAudio = *(u32*)((u32)(Songs + b) + 0x8);
							int ConvertedTrack_LeftAudio = SP2MP + Track_LeftAudio;
							int ConvertedTrack_RightAudio = SP2MP + Track_RightAudio;
							*(u32*)(NewTracksLocation) = (u32)ConvertedTrack_LeftAudio;
							*(u32*)(NewTracksLocation + 0x8) = (u32)ConvertedTrack_RightAudio;
							NewTracksLocation += 0x10;
							b += (a == 0) ? 0x10 : 0x20;
							++AddedTracks;
							++TotalTracks;
						}
					}
				}
				// Zero out stack to finish the job.
				memset((u32*)Stack, 0, 0x1818);
			}
			DPRINTF("\nTracks: %d", AddedTracks);
			FinishedConvertingTracks = 1;
		}
		
		// Due to us overwriting original tracks, no need to do extra math like in DL.
		// int MusicFunctionData = (CodeSegmentPointer + 0x1A8);
		// if (*(u16*)MusicFunctionData != AddedTracks) {
		// 	*(u16*)MusicFunctionData = AddedTracks;
		// }
	}

	// If in game
	if (isInGame()) {
		music_Playing* music = musicGetTrackInfo();
		// printf("\nafter trans");
		// double check if min/max info are correct
		if (config.enableSingleplayerMusic) {
			if (*(int*)musicTrackRangeMax() != (TotalTracks - 4) || *(int*)musicTrackRangeMin() != 4) {
				*(int*)musicTrackRangeMin() = 4;
				*(int*)musicTrackRangeMax() = TotalTracks - 4;
			}
		} else {
			if (*(int*)musicTrackRangeMax() != (DefaultMultiplayerTracks - *(int*)musicTrackRangeMin()) || *(int*)musicTrackRangeMin() != 4) {
				*(int*)musicTrackRangeMin() = 4;
				*(int*)musicTrackRangeMax() = DefaultMultiplayerTracks - *(int*)musicTrackRangeMin();
			}
		}
		// Fixes bug where music doesn't always want to start playing at start of game
		// might not be needed anymore due to forcing Min/Max Track info above
		if (music->track == -1 && music->status == 0) {
			// plays a random track
			int randomTrack = randRangeInt(*(int*)musicTrackRangeMin(), *(int*)musicTrackRangeMax()) % *(int*)musicTrackRangeMax();
			DPRINTF("\nrandomTrack: %d", randomTrack);
			musicPlayTrack(randomTrack, FLAG_KEEP_PLAYING_AFTER, 1024);
		}
		// If Status is 8 and both Current Track and Next Track equal zero
		if (music->unpause == UNPAUSE_LOADING && CurrentTrack == 0 && NextTrack == 0 && music->track != -1) {
			// Set CurrentTrack to Track.  This will make it so we know which was is currently playing.
			// The game automatically sets the track variable to the next track to play after the music starts.
			CurrentTrack = music->track;
		} else if ((music->unpause == UNPAUSE_KEEP_PLAYING_AFTER || music->unpause == UNPAUSE_STOP_PLAYING_AFTER) && NextTrack == 0) {
			// Set NextTrack to Track value.
			NextTrack = music->track;
		}
		// If NextTrack does not equal the Track, that means that the song has switched.
		// We need to move the NextTrack value into the CurrentTrack value, because it is now
		// playing that track.  Then we set the NextTrack to the Track value.
		else if (NextTrack != music->track) {
			CurrentTrack = NextTrack;
			NextTrack = music->track;
		}
		// If CurrentTrack is ger than the default Multiplayer tracks
		// and if CurrentTrack does not equal -1
		// and if the track duration is below 0x3000
		// and if Status2 is 2, or Current Playing
		// printf("\nbefore trans");
		if ((CurrentTrack > DefaultMultiplayerTracks * 2) && CurrentTrack != -1 && (music->remain <= 0x3000) && music->queuelen == QUEUELEN_PLAYING) {
			// This technically cues track 1 (the shortest track) with no sound to play.
			// Doing this lets the current playing track to fade out.
			musicTransitionTrack(0,0,0,0);
		}
	} else if (isInMenus() && FinishedConvertingTracks) {
		FinishedConvertingTracks = 0;
		SetupMusic = 0;
		TotalTracks = 0;
		AddedTracks = 0;
		CurrentTrack = 0;
		NextTrack = 0;
	}
}

/*
 * NAME :		patchAimAssist
 * DESCRIPTION :	Disables Aim Assist for player weapons
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchAimAssist(void)
{
	if (patched.config.aimAssist)
		return;

	Player* p = playerGetFromSlot(0);
	p->fps.vars.cameraZ.target_slowness_factor_quick = 0;
	p->fps.vars.cameraZ.target_slowness_factor_aim = 0;
	p->fps.vars.cameraY.target_slowness_factor = 0;
	p->fps.vars.cameraY.strafe_turn_factor = 0;
	p->fps.vars.cameraY.strafe_tilt_factor = 0;
	patched.config.aimAssist = 1;
}

/*
 * NAME :		onClientVoteToEndStateUpdateRemote
 * DESCRIPTION :
 * 			Receives when the host updates the vote to end state.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int onClientVoteToEndStateUpdateRemote(void * connection, void * data)
{
	memcpy(&voteToEndState, data, sizeof(voteToEndState));
	return sizeof(voteToEndState);
}

/*
 * NAME :		onClientVoteToEndRemote
 * DESCRIPTION :
 * 			Receives when another client has voted to end.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int onClientVoteToEndRemote(void * connection, void * data)
{
	int playerId;
	memcpy(&playerId, data, sizeof(playerId));
	onClientVoteToEnd(playerId);
	return sizeof(playerId);
}

/*
 * NAME :		onClientVoteToEnd
 * DESCRIPTION :
 * 			Handles when a client votes to end.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void onClientVoteToEnd(int playerId)
{
	int i;
	GameSettings* gs = gameGetSettings();
	if (!gs) return;
	if (!gameAmIHost()) return;

	// set
	voteToEndState.Votes[playerId] = 1;

	// tally votes
	voteToEndState.Count = 0;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
		if (voteToEndState.Votes[i]) voteToEndState.Count += 1;

	// set timeout
	if (voteToEndState.TimeoutTime <= 0)
		voteToEndState.TimeoutTime = gameGetTime() + TIME_SECOND*30;

	// send update
	netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_VOTE_TO_END_STATE_UPDATED, sizeof(voteToEndState), &voteToEndState);
	DPRINTF("End player:%d timeout:%d\n", playerId, voteToEndState.TimeoutTime - gameGetTime());
}

/*
 * NAME :		sendClientVoteForEnd
 * DESCRIPTION :
 * 			Broadcasts to all other clients in lobby that this client has voted to end.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void sendClientVoteForEnd(void)
{
	Player* p1 = playerGetFromSlot(0);
	if (!p1) return;

	int playerId = p1->mpIndex;
	if (!gameAmIHost()) {
		netSendCustomAppMessage(netGetDmeServerConnection(), -1, CUSTOM_MSG_ID_PLAYER_VOTED_TO_END, 4, &playerId);
	} else {
		onClientVoteToEnd(playerId);
	}
}

/*
 * NAME :		voteToEndNumberOfVotesRequired
 * DESCRIPTION :
 * 			Returns the number of votes required for the vote to pass.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int voteToEndNumberOfVotesRequired(void)
{
	GameSettings* gs = gameGetSettings();
	GameData* gameData = gameGetData();
	int i;
	int playerCount = 0;

	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		if (gs->PlayerClients[i] >= 0) playerCount += 1;
	}

	// if (gameData->NumTeams > 2)
	// 	return playerCount;
	// else
	return (int)(playerCount * 0.60);
}

/*
 * NAME :		runVoteToEndLogic
 * DESCRIPTION :
 * 			Handles all logic related to when a team Ends.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void runVoteToEndLogic(void)
{
	if (!isInGame()) { patchStateContainer.VoteToEndPassed = 0; memset(&voteToEndState, 0, sizeof(voteToEndState)); return; }
	if (gameHasEnded()) return;
	if (voteToEndState.Count <= 0 || voteToEndState.TimeoutTime <= 0) return;

	int i;
	int gameTime = gameGetTime();
	GameData* gameData = gameGetData();
	GameSettings* gs = gameGetSettings();
	Player* player = playerGetFromSlot(0);
	char buf[64];

	int votesNeeded = voteToEndNumberOfVotesRequired();
	if (voteToEndState.Count >= votesNeeded && gameAmIHost()) {
		// reset
		memset(&voteToEndState, 0, sizeof(voteToEndState));
		// pass to modules
		patchStateContainer.VoteToEndPassed = 1;
		// end game
		gameEnd(4);
		return;
	}

	int haveVoted = 0;
	if (player)
		haveVoted = voteToEndState.Votes[player->mpIndex];

	if (voteToEndState.TimeoutTime > gameTime) {
		// draw
		int secondsLeft = (voteToEndState.TimeoutTime - gameTime) / TIME_SECOND;
		snprintf(buf, sizeof(buf), "(L3 + R3) Vote to End (%d/%d)    %d...", voteToEndState.Count, votesNeeded, secondsLeft);
		gfxScreenSpaceText(12, SCREEN_HEIGHT - 18, 1, 1, 0x80000000, buf, -1, 0);
		gfxScreenSpaceText(10, SCREEN_HEIGHT - 20, 1, 1, 0x80FFFFFF, buf, -1, 0);
		
		// vote to end
		if (!haveVoted && padGetButtonDown(0, PAD_L3 | PAD_R3) > 0)
    		sendClientVoteForEnd();
	
	} else if (gameAmIHost() && voteToEndState.TimeoutTime < gameTime) {
		// reset
		memset(&voteToEndState, 0, sizeof(voteToEndState));
		netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_VOTE_TO_END_STATE_UPDATED, sizeof(voteToEndState), &voteToEndState);
	}
}

/*
 * NAME :		hypershotGetButton
 * DESCRIPTION :
 * 				Retruns the needed value for the hysershot button config
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int hypershotGetButton(void)
{
	switch (config.hypershotEquipButton) {
		case 1: return PAD_CIRCLE;
		case 2: return PAD_LEFT;
		case 3: return PAD_DOWN;
		case 4: return PAD_RIGHT;
		case 5: return PAD_UP;
		case 6: return PAD_L3;
		case 7: return PAD_R3;
		default: return 0;
	}
}

/*
 * NAME :		hypershotEquipBehavior
 * DESCRIPTION :
 * 				Handles the hypershot equip behavior
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void hypershotEquipButton(void)
{
	// Force weaposn to only be taken out with only R1, and not both R1 or Circle.
	if (!patched.config.hypershotEquipButton && hypershotGetButton() == PAD_CIRCLE) {
		u32 a = GetAddress(&vaHypershotEquipButton_bits);
		u32 pad = 0x24020000 | 0x8;
		POKE_U32(a, pad);
		POKE_U32(a + 0x4, pad);
		patched.config.hypershotEquipButton = 1;
	}
	// get Player 1 struct
	Player *p = playerGetFromSlot(0);
	// if player is found, and not holding flag, and prsses needed button.
	if (p && !p->flagMoby && playerPadGetButtonDown(p, hypershotGetButton()) > 0)
		playerEquipWeapon(p, WEAPON_ID_SWINGSHOT);
}

int remapButtons(pad)
{
	// disable the default action for the chosen hypershot button.
	if (config.hypershotEquipButton) {
		u16 hypershot = hypershotGetButton();
		if (hypershotGetButton() != PAD_CIRCLE) {
			if ((pad & hypershot) == 0)
				return 0xffff & (pad | hypershot);
		}
	}

	if (config.disableDpadMovement) {
		// Make a mask of the bits we want to filter
		u16 mask = (PAD_LEFT | PAD_RIGHT | PAD_UP | PAD_DOWN);
		// only grab the mask filter from the pad
		u16 dpad = (pad ^ mask) & 0x00f0;
		// if any dpad button is pressed, return data as if it's not pressed.
		if ((dpad & pad) == 0)
			return 0xffff & (pad | dpad);
	}


	switch (pad ^ 0xffff) {
		// EXAMPLE: if Presssing X, return by telling it to press circle instead.
		// case PAD_CROSS:
		// 	return PAD_CIRCLE ^ (0xffff & (pad | PAD_CROSS));

		default: return pad;

	}
}

void patchSceReadPad_memcpy(void * destination, void * source, int num)
{
	Player * player = playerGetFromSlot(0);
	// make sure the pause menu is not open.  this way the pause menu can still be used.
	// Also check to see is player is alive, if not, don't run.  (used for Siege if picking node to spawn at)
	if (player && !player->pauseOn  && !playerIsDead(player)) {
		u32 paddata = (void*)((u32)source + 0x2);
		// edit the pad data.
		*(u16*)paddata = remapButtons(*(u16*)paddata);
	}
	// finish up by running the original function we took over.
	memcpy(destination, source, num);
}

/*
 * NAME :		patchLoadingPopup_isConfigMenuOpen
 * DESCRIPTION :
 * 				If patch menu is open, do not show the loading popup.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int patchLoadingPopup_isConfigMenuOpen(void* a0, long a1)
{
	// if patch menu is open, return 0 to not show the loading popup.
	if (isConfigMenuActive)
		return 0;

	// return base if config menu isn't open.
	#if UYA_PAL
	int r = ((int (*)(void *, long))0x006cea18)(a0, a1);
	#else
	int r = ((int (*)(void *, long))0x006cc290)(a0, a1);
	#endif
	return r;
}
/*
 * NAME :		patchLoadingPopup
 * DESCRIPTION :
 * 				Hooks "patchLoadingPopup_isConfigMenuOpen"
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchLoadingPopup(void)
{
	if (patched.loadingPopup)
		return;

	#if UYA_PAL
	HOOK_JAL(0x006c40e0, &patchLoadingPopup_isConfigMenuOpen);
	#else
	HOOK_JAL(0x006c15c8, &patchLoadingPopup_isConfigMenuOpen);
	#endif

	patched.loadingPopup = 1;
}

/*
 * NAME :		runPlayerPositionSmooth
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void runPlayerPositionSmooth(void)
{
	static VECTOR smoothVelocity[GAME_MAX_PLAYERS];
	static int applySmoothVelocityForFrames[GAME_MAX_PLAYERS];

	Player ** players = playerGetAll();
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		Player* p = players[i];
		if (p && !p->isLocal && p->pMoby) {
			// determine if the player has strayed too far from the remote player's position
			// it is critical that we run this each frame, regardless if we're already smoothing the player's velocity
			// in case something has changed, we want to recalcuate the smooth velocity instantly
			// instead of waiting for the (possibly) 10 smoothing frames to complete
    		if (p->pNetPlayer && p->pNetPlayer->pNetPlayerData) {
				VECTOR dt = {0, 0, 0, 0};
				VECTOR rPos = {0, 0, 0, 0};
				VECTOR lPos = {0, 0, 1, 0};
				vector_copy(dt, (float*)(p->RemoteHero.syncPosDifference));
				vector_copy(rPos, (float*)(p->RemoteHero.receivedSyncPos));

				// apply when remote player's simulated position
				// is more than 2 units from the received position
				// at the time of receipt
				// meaning, this value only updates when receivedSyncPos (0x4d40) is updated
				// not every tick
				float dist = vector_length(dt);
				if (dist > 2) {
					// reset syncPosDifference
					// since it updates every 4 frames (I think)
					// and we don't want to run this 4 times in a row on the same data
					vector_write((float*)(p->RemoteHero.syncPosDifference), 0);

					// we want to apply this delta over multiple frames for the smoothest result
					// however there are cases where we want to teleport
					// in cases where the player has fallen under the ground on our screen
					// we want to teleport them back up, since gravity will counteract our interpolation
					int applyOverTicks = 10;
					rPos[2] += 1;
					vector_add(lPos, lPos, p->playerPosition);
					if (dt[2] > 0 && (dt[2] / dist) > 0.8 && CollLine_Fix(lPos, rPos, 2, p->pMoby, 0)) {
						applyOverTicks = 1;
					}

					// indicate number of ticks to apply additives over
					// scale dt by number of ticks to add
					// so that we add exactly dt over those frames
					applySmoothVelocityForFrames[i] = applyOverTicks;
					vector_scale(smoothVelocity[i], dt, 1.0 / applyOverTicks);
				}
			}
			// apply adds
			if (applySmoothVelocityForFrames[i] > 0) {
				vector_add(p->playerPosition, p->playerPosition, smoothVelocity[i]);
				vector_copy(p->pMoby->position, p->playerPosition);
				--applySmoothVelocityForFrames[i];
			}
		}
	}
}

/*
 * NAME :		runPlayerSync
 * DESCRIPTION: Runs various functions to help smooth players movements.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runPlayerSync(void)
{
	if (!config.enablePlayerSync)
		return;
	
	runPlayerPositionSmooth();
}

void runHolidays(void)
{
	if (!PATCH_POINTERS || patched.holidays)
		return;

	int i;
	int month = PATCH_POINTERS->ServerTimeMonth;
	int day = PATCH_POINTERS->ServerTimeDay;
	int skin = -1;
	switch(month) {
		case 10: {
			if (day == 31)
				skin = SKIN_BONES;

			break;
		}
		case 12: {
			if (day >= 24)
				skin = SKIN_SNOWMAN;

			break;
		}
	}

	// DPRINTF("\nLocation/Month/Day/Skin: l:%d/m:%d/d:%d/s:%d/", location, month, day, skin);
	// DPRINTF("\nDate: %02d/%02d\nTime: %02d:%02d:%02d", PATCH_POINTERS->ServerTimeMonth,  PATCH_POINTERS->ServerTimeDay,  PATCH_POINTERS->ServerTimeHour, PATCH_POINTERS->ServerTimeMinute, PATCH_POINTERS->ServerTimeSecond);

	if (location == LOCATION_LOADING) {
		if (skin > -1) {
			GameSettings *gs = gameGetSettings();
			for (i = 0; i < gs->PlayerCount; ++i) {
				gs->PlayerSkins[i] = skin;
			}
			// patched.holidays = 1;
		}
	}

}

/*
 * NAME :		runGameStartMessager
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void runGameStartMessager(void)
{
	GameSettings * gameSettings = gameGetSettings();
	if (!gameSettings)
		return;

	// in staging
	if (uiGetActivePointer(UIP_STAGING) != 0) {
		// check if game started
		if (!sentGameStart && gameSettings->GameLoadStartTime > 0) {
			// check if host
			if (gameAmIHost())
				netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_GAME_LOBBY_STARTED, 0, gameSettings);

			// request server time
			requestServerTime();

			// request latest scavenger hunt settings
      		scavHuntQueryForRemoteSettings();

			sentGameStart = 1;
		}
	} else {
		sentGameStart = 0;
	}
}

/*
 * NAME :		runCheckGameMapInstalled
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void runCheckGameMapInstalled(void)
{
	int i;
	GameSettings* gs = gameGetSettings();
	if (!gs || !isInMenus())
		return;

	// if start game button is enabled
	// then disable it if maps are enabled
	if (gameAmIHost()) {
		if (mapOverrideResponse < 0) {
			if (STAGING_START_BUTTON_STATE == 3) {
				STAGING_START_BUTTON_STATE = 2;
				showNoMapPopup = 1;
			}
		} else {
			STAGING_START_BUTTON_STATE = 3;
		}
	}
	
	int clientId = gameGetMyClientId();
	if (mapOverrideResponse < 0) {
		for (i = 1; i < GAME_MAX_PLAYERS; ++i) {
			if (gs->PlayerClients[i] == clientId && gs->PlayerStates[i] == 6) {
		#if UYA_PAL
				((void (*)(u32, u32, u32))0x006c4308)(uiGetActivePointer(UIP_STAGING), 5, 0);
		#else
				((void (*)(u32, u32, u32))0x006c17f0)(uiGetActivePointer(UIP_STAGING), 5, 0);
		#endif
				gs->PlayerStates[i] = 0; // unready up
				showNoMapPopup = 1;
				netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_REQUEST_MAP_OVERRIDE, 0, NULL);
			} else if (gs->PlayerClients[i] == clientId && gs->PlayerStates[i] == 5) {
				showNoMapPopup = 0;
			}
		}
	}
}

/*
 * NAME :		setupPatchConfigInGame
 * DESCRIPTION :
 * 				Changes the "CONTINUE" start option to "PATCH MENU"
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void setupPatchConfigInGame(void)
{
	// u32 ConfigEnableFunc = 0x0C000000 | ((u32)&configMenuEnable >> 2);
	// Original If: *(u32*)(Addr + 0x8) != ConfigEnableFunc
	if (!patched.configStartOption) {
		u32 Addr = GetAddress(&vaPauseMenuAddr);
		// Insert needed ID, returns string.
		int str = uiMsgString(0x1115); // Washington, D.C. string ID
		// Replace "Washington, D.C." string with ours.
		strncpy((char*)str, patchStr, 13);
		// Set "CONTINUE" string ID to our ID.
		*(u32*)Addr = 0x1115;
		// Pointer to "CONTINUE" function
		u32 ReturnFunction = *(u32*)(Addr + 0x8);
		// Hook Patch Config into end of "CONTINUE" function.
		HOOK_J((ReturnFunction + 0x54), &configMenuEnable);
		patched.configStartOption = 1;
	}
}

/*
 * NAME :		runSendGameUpdate
 * DESCRIPTION : Sends the current game info to the server.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynopme" Pruitt
 */
int runSendGameUpdate(void)
{
	static int lastGameUpdate = 0;
	static int newGame = 0;
	GameSettings * gameSettings = gameGetSettings();
	GameOptions * gameOptions = gameGetOptions();
	GameData * gameData = gameGetData();
	Player** players = playerGetAll();
	int gameTime = gameGetTime();
	int i;
	void * connection = netGetLobbyServerConnection();

	// skip if not online, in lobby, or the game host
	if (!connection || !gameSettings || !gameAmIHost()) {
		lastGameUpdate = -GAME_UPDATE_SENDRATE;
		newGame = 1;
		return 0;
	}

	// skip if time since last update is less than sendrate
	if ((gameTime - lastGameUpdate) < GAME_UPDATE_SENDRATE)
		return 0;

	// update last sent time
	lastGameUpdate = gameTime;

	// construct
	patchStateContainer.GameStateUpdate.TeamsEnabled = gameOptions->GameFlags.MultiplayerGameFlags.Teams;
	patchStateContainer.GameStateUpdate.Version = 1;

	// copy over client ids
	memcpy(patchStateContainer.GameStateUpdate.ClientIds, gameSettings->PlayerClients, sizeof(patchStateContainer.GameStateUpdate.ClientIds));

	// reset some stuff whenever we enter a new game
	if (newGame) {
		memset(patchStateContainer.GameStateUpdate.TeamScores, 0, sizeof(patchStateContainer.GameStateUpdate.TeamScores));
		newGame = 0;
	}

	// copy teams over
	memcpy(patchStateContainer.GameStateUpdate.Teams, gameSettings->PlayerTeams, sizeof(patchStateContainer.GameStateUpdate.Teams));

	// 
	if (isInGame()) {
		int i;
		// reset
		memset(patchStateContainer.GameStateUpdate.TeamScores, 0, sizeof(patchStateContainer.GameStateUpdate.TeamScores));
		memset(patchStateContainer.GameStateUpdate.Nodes, 0, sizeof(patchStateContainer.GameStateUpdate.Nodes));

		if (gameSettings->GameType == GAMERULE_SIEGE) {	
			for (i = 0; i < 8; ++i) {
				if (gameData->allYourBaseGameData->nodeTeam[i] == 0)
					++patchStateContainer.GameStateUpdate.Nodes[0];
				else if (gameData->allYourBaseGameData->nodeTeam[i] == 1)
					++patchStateContainer.GameStateUpdate.Nodes[1];
			}
			patchStateContainer.GameStateUpdate.TeamScores[0] = gameData->allYourBaseGameData->hudHealth[0];
			patchStateContainer.GameStateUpdate.TeamScores[1] = gameData->allYourBaseGameData->hudHealth[1];
		} else if (gameSettings->GameType == GAMERULE_CTF) {
			// Nodes are turned off
			patchStateContainer.GameStateUpdate.Nodes[0] = -1;
			patchStateContainer.GameStateUpdate.Nodes[1] = -1;
			// Check if nodes are on
			if (gameOptions->GameFlags.MultiplayerGameFlags.Nodes) {
				for (i = 0; i < 8; ++i) {
					if (gameData->allYourBaseGameData->nodeTeam[i] == 0)
						++patchStateContainer.GameStateUpdate.Nodes[0];
					else if (gameData->allYourBaseGameData->nodeTeam[i] == 1)
						++patchStateContainer.GameStateUpdate.Nodes[1];
				}	
			}
			patchStateContainer.GameStateUpdate.TeamScores[0] = gameData->CTFGameData->blueTeamCaptures;
			patchStateContainer.GameStateUpdate.TeamScores[1] = gameData->CTFGameData->redTeamCaptures;
		} else if (gameSettings->GameType == GAMERULE_DM) {
			for (i = 0; i < gameSettings->PlayerCount; ++i) {
				int team = gameSettings->PlayerTeams[i];
				int kills = gameData->playerStats.frag[i].kills;
				int deaths = gameData->playerStats.frag[i].deaths;
				patchStateContainer.GameStateUpdate.TeamScores[team] += kills - deaths;
			}
		}
	}
	return 1;
}

/*
 * NAME :		processGameModules
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void processGameModules()
{
	// Start at the first game module
	GameModule * module = GLOBAL_GAME_MODULES_START;

	// Game settings
	GameSettings * gamesettings = gameGetSettings();

	// Iterate through all the game modules until we hit an empty one
	while (module->GameEntrypoint || module->LobbyEntrypoint) {
		// Ensure we have game settings
		if (gamesettings) {
			// Check the module is enabled
			if (module->State > GAMEMODULE_OFF) {
				// If in game, run game entrypoint
				if (isInGame()) {
					// Invoke module
					if (module->GameEntrypoint)
						module->GameEntrypoint(module, &config, &gameConfig);
				} else if (isInMenus()) {
					// Invoke lobby module if still active
					if (module->LobbyEntrypoint) {
						module->LobbyEntrypoint(module, &config, &gameConfig);
					}
				}
			}
		}
		// If we aren't in a game then try to turn the module off
		// ONLY if it's temporarily enabled
		else if (module->State == GAMEMODULE_TEMP_ON) {
			module->State = GAMEMODULE_OFF;
		} else if (module->State == GAMEMODULE_ALWAYS_ON) {
			// Invoke lobby module if still active
			if (isInMenus() && module->LobbyEntrypoint) {
				module->LobbyEntrypoint(module, &config, &gameConfig);
			}
		}

		++module;
	}
}

/*
 * NAME :		onOnlineMenu
 * DESCRIPTION :
 * 			Called every ui update.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void onOnlineMenu(void)
{
  int i;

	// call normal draw routine
#ifdef UYA_PAL
    ((void (*)(void))0x0067C9C0)();
#else
	((void (*)(void))0x00679f08)();
#endif
	lastMenuInvokedTime = gameGetTime();
	if (!hasInitialized) {
		padEnableInput();
		onConfigInitialize();
		refreshCustomMapList();
		memset(&voteToEndState, 0, sizeof(voteToEndState));
		hasInitialized = 1;
	}
	if (hasInitialized == 1 && uiGetActivePointer(UIP_ONLINE_LOBBY) != 0) {
		uiShowOkDialog("System", "Patch has been successfully loaded.");
		hasInitialized = 2;
	}
  
	// map loader
	onMapLoaderOnlineMenu();

	// settings
	onConfigOnlineMenu();

	// 
	if (showNoMapPopup) {
		if (mapOverrideResponse == -1) {
			uiShowOkDialog("Custom Maps", "You have not installed the map modules.");
		} else {
			char buf[32];
			sprintf(buf, "Please install %s to play.", MapLoaderState.MapName);
			uiShowOkDialog("Custom Maps", buf);
		}
		showNoMapPopup = 0;
	}

	if (!scavHuntShownPopup && !config.disableScavengerHunt && scavHuntEnabled){
		uiShowOkDialog("Scavenger Hunt", "The Horizon Scavenger Hunt is live! Hunt for Horizon Bolts for a chance to win prizes! Join our discord for more info: discord.gg/horizonps");
    	scavHuntShownPopup = 1;
	}

  //
  #if DSCRPRINT
  float y = 10;
  for (i = 0; i < MAX_DEBUG_SCR_PRINT_LINES; ++i) {
    gfxScreenSpaceText(10, y, 1, 1, 0x80FFFFFF, dscrprintlines[i], -1, 0);
    y += 20;
  }
  #endif
}

/*
 * NAME :		main
 * DESCRIPTION :
 * 			Applies all patches and modules.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int main(void)
{
	#if DEBUG
	Player * p = playerGetFromSlot(0);
	// 82: Test Server,  85: Prod Server
	if (p->pNetPlayer->pNetPlayerData->accountId == 82) {
		static int num = 0;
		if (padGetButtonDown(0, PAD_L3 | PAD_R3) > 0) {
			gameEnd(num);
			num = 0;
		}
		if (padGetButtonDown(0, PAD_L1 | PAD_UP) > 0) num = 1;
		if (padGetButtonDown(0, PAD_L1 | PAD_DOWN) > 0) num = 2;
		if (padGetButtonDown(0, PAD_L1 | PAD_LEFT) > 0) num = 3;
		if (padGetButtonDown(0, PAD_L1 | PAD_RIGHT) > 0) num = 4;
	}
	#endif
	// Call this first
	uyaPreUpdate();

  //
  #if DSCRPRINT
  int i;
  float y = 10;
  for (i = 0; i < MAX_DEBUG_SCR_PRINT_LINES; ++i) {
    gfxScreenSpaceText(10, y, 1, 1, 0x80FFFFFF, dscrprintlines[i], -1, 0);
    y += 20;
  }
  #endif

	// update patch pointers
	PATCH_POINTERS = &patchPointers;

	// auto enable pad input to prevent freezing when popup shows
	if (isInMenus() && lastMenuInvokedTime > 0 && (gameGetTime() - lastMenuInvokedTime) > TIME_SECOND) {
		padEnableInput();
		lastMenuInvokedTime = 0;
	}

	//
  	netInstallCustomMsgHandler(CUSTOM_MSG_ID_CLIENT_RESPONSE_DATE_SETTINGS, &onServerTimeResponse);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_PLAYER_VOTED_TO_END, &onClientVoteToEndRemote);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_VOTE_TO_END_STATE_UPDATED, &onClientVoteToEndStateUpdateRemote);
	
	// Run map loader
	runMapLoader();

	// run exception handler
	runExceptionHandler();

	// run game start check
	// sends game started message to server
	// when host (us) start the game
	runGameStartMessager();

	// 
	runCheckGameMapInstalled();

	// Run Scavenger Hunt
	scavHuntRun();

	// Run Send Gameupdate for Helga
	patchStateContainer.UpdateGameState = runSendGameUpdate();


	// 
	runCameraSpeedPatch();

	// 
	onConfigUpdate();

	// 
	sendMACAddress();

	// Adds Single Player Music to Multiplayer
	if (config.enableSingleplayerMusic)
		runCampaignMusic();

	// 
	runVoteToEndLogic();

	runHolidays();

	#if TEST
	void runTest(void);
	#endif

	if(isInGame()) {
		// Patch remap buttons configuration
		HOOK_JAL(0x0013cae0, &patchSceReadPad_memcpy);

		// Patch Dead Jumping/Crouching
		patchDeadJumping();

		// Patch Dead Shooting
		patchDeadShooting();

		// Run Game Rules if in game.
		grGameStart();

		// Patch Flux Niking
		patchSniperNiking();

		// Patch Flux Wall Sniping
		patchSniperWallSniping();

		// Patches FOV to let it be user selectable.
		if (config.playerFov != 0)
			patchFov();

		// Patch Quick Select to use custom timer.
		patchQuickSelectTimer();

		// Patch Kill Stealing
		patchKillStealing();

		// Patch sending weapon shots via UDB to TCP.
		patchWeaponShotLag();

		// Patch Death Barrier Bug/Teleporter Glitch
		patchDeathBarrierBug();

		// Patch CTF Flag Logic with our own.
		if (gameConfig.grFlagHotspots)
			patchCTFFlag();

		// Patch Level of Detail
		patchLevelOfDetail();

		// Patch Weapon Ordering when Respawning
		patchResurrectWeaponOrdering();

		// Runs FPS Counter
		runFpsCounter();

		// Run Playersynv v1
		// runPlayerSync();

		// Run Spectate
		// if (config.enableSpectate)
		// 	runSpectate();

		// Patches gadget events as they come in.
		if(gameConfig.grWeaponShotLag)
			patchGadgetEvents();

		if (config.alwaysShowHealth)
			patchAlwaysShowHealth();

		// Patches the Map and Scoreboard for player toggalability!
		patchMapAndScoreboardToggle();

		if (config.aimAssist)
			patchAimAssist();

		if (config.hypershotEquipButton)
			hypershotEquipButton();

		// close config menu on transition to lobby
		if (lastGameState != 1)
			configMenuDisable();

		// Updates Start Menu to have "Patch Config" option.
		// Logic for opening menu as well.
		setupPatchConfigInGame();

		// trigger config menu update
		onConfigGameMenu();

		lastGameState = 1;
	} else if (isInMenus()) {
		// Patch various options on Create Game Screen
		// Freezes on PAL
		// patchCreateGameMenu();

		// If in Lobby, run these game rules.
		grLobbyStart();

		// Patches loading popup from not showing if patch menu is open.
		patchLoadingPopup();

		// Patch Menus (Staging, create game, ect.)
		if (patched.uiModifiers == 0) {
			POKE_U32(UI_PTR_FUNC_CREATE_GAME, &patchCreateGame);
			POKE_U32(UI_PTR_FUNC_STAGING, &patchStaging);
			POKE_U32(UI_PTR_FUNC_BUDDIES, &patchBuddies);
			POKE_U32(UI_PTR_FUNC_PLAYER_DETAILS, &patchPlayerDetails);
			POKE_U32(UI_PTR_FUNC_STATS, &patchStats);
			patched.uiModifiers = 1;
		}

		// Patch Rank Table
		if (!patched.rankTable) {
			// Sets deviation rank higher than that of player deviation, this way it goes by rank, not deviation.
			int rangeMultiplier = 100;
			*(float*)RANK_TABLE = 1000000000.00; // Deviation 1
			*(float*)(RANK_TABLE + 0x4) = 0; // Deviation 2
			*(float*)(RANK_TABLE + 0x8) = 0; // Deviation 3
			*(float*)(RANK_TABLE + 0xc) = 1000 * rangeMultiplier; // Rank Rage 1 (2 bolts)
			*(float*)(RANK_TABLE + 0x10) = 1300 * rangeMultiplier; // Rank Rage 2 (3 bolts)
			*(float*)(RANK_TABLE + 0x14) = 1700 * rangeMultiplier; // Rank Range 3 (4 bolts)
			patched.rankTable = 1;
		}

		// Reset Level of Detail to -1
		lastLodLevel = -1;

#ifdef UYA_PAL
		if (*(u32*)0x00576120 == 0) {
			*(u32*)0x005760E4 = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
			*(u32*)0x0057611C = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		}

		// popup is visible
		if (*(u32*)0x002467dc == 1) {
			configMenuDisable();
			padEnableInput();
		}
#else
		if (*(u32*)0x005753E0 == 0) {
			*(u32*)0x005753A4 = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
			*(u32*)0x005753DC = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		}

		// popup is visible
		if (*(u32*)0x0024693C == 1) {
			configMenuDisable();
			padEnableInput();
		}
#endif

		// send patch game config on create game
		GameSettings * gameSettings = gameGetSettings();
		if (gameSettings && gameSettings->GameLoadStartTime < 0) {
			// if host and just entered staging, send patch game config
			if (gameAmIHost() && !isInStaging) {
				// copy over last game config as host
				memcpy(&gameConfig, &gameConfigHostBackup, sizeof(PatchGameConfig_t));
				// Reset SelectedCustomMapId to none
				SelectedCustomMapId = 0;

				// send
				configTrySendGameConfig();
			}

			if (!isInStaging)
				memset(&patched, 0, sizeof(PatchPatches_t));

			isInStaging = 1;
		} else {
			isInStaging = 0;
		}
	}

	// process modules
	processGameModules();

	if (patchStateContainer.UpdateGameState) {
		patchStateContainer.UpdateGameState = 0;
		netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_SET_GAME_STATE, sizeof(UpdateGameStateRequest_t), &patchStateContainer.GameStateUpdate);
	}

	// Call this last
	uyaPostUpdate();

	return 0;
}
