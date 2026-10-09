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
#include <libuya/hud.h>
#include "module.h"
#include "messageid.h"
#include "config.h"
#include "interop/patch.h"
#include "include/config.h"
#include "include/cheats.h"

#define GLOBAL_GAME_MODULES_START							((GameModule*)0x000cf000)
#define EXCEPTION_HANDLER									(0x000c8000)
#define GAME_UPDATE_SENDRATE								(5 * TIME_SECOND)

#if UYA_PAL
#define STAGING_START_BUTTON_STATE							(*(short*)0x006c2d80)
#define RANK_TABLE                              			((u32)0x001a6a64)
#define UI_PTR_FUNC_CREATE_GAME								(0x0047cfec)
#define UI_PTR_FUNC_ADVANCED_OPTIONS						(0x0047d124)
#define UI_PTR_FUNC_STAGING									(0x0047e9ac)
// #define UI_PTR_FUNC_BUDDIES									(0x0047c834)
// #define UI_PTR_FUNC_PLAYER_DETAILS							(0x0047e44c)
// #define UI_PTR_FUNC_STATS									(0x0047eab4)
#define UI_PTR_FUNC_KEYBOARD								(0x0047dbac)
#define STAGING_JALR_HEADSET_SET_COLOR						(0x006c234c)
#define nwVoiceUpdateFunc									(0x0019c2c0)
#else
#define STAGING_START_BUTTON_STATE							(*(short*)0x006C0268)
#define RANK_TABLE                              			((u32)0x001a6Be4)
#define UI_PTR_FUNC_CREATE_GAME								(0x0047d0ac)
#define UI_PTR_FUNC_ADVANCED_OPTIONS						(0x0047d1e4)
#define UI_PTR_FUNC_STAGING									(0x0047ea6c)
// #define UI_PTR_FUNC_BUDDIES									(0x0047c8f4)
// #define UI_PTR_FUNC_PLAYER_DETAILS							(0x0047e50c)
// #define UI_PTR_FUNC_STATS									(0x0047eb74)
#define UI_PTR_FUNC_KEYBOARD								(0x0047dc6c)
#define STAGING_JALR_HEADSET_SET_COLOR						(0x006bf834)
#define nwVoiceUpdateFunc									(0x0019c400)
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
// int patchBuddies(void * ui, long pad);
// int patchPlayerDetails(void * ui, long pad);
// int patchStats(void * ui, int pad);
int patchKeyboard(void * ui, int pad);

void grGameStart(void);
void grLobbyStart(void);
void grLoadStart(void);

// void runPing(void);
// void runSpectate(void);

void patchControllerDeadzone(void);

#ifdef SCAVENGER_HUNT
void scavHuntRun(void);
#endif

int hasInitialized = 0;
int lastMenuInvokedTime = 0;
int lastGameState = 0;
int sentGameStart = 0;
int isInStaging = 0;
int location = LOCATION_NULL;
int hasInstalledExceptionHandler = 0;
int mapOverrideResponse = 9001;
int expectedMapVersion = -1;
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
int flagTrackedLastCarrierIdx[2] = {-1, -1};

#if DSCRPRINT
#define MAX_DEBUG_SCR_PRINT_LINES       (16)
#define MAX_DEBUG_SCR_PRINT_LINE_LEN    (64)
char dscrprintlines[MAX_DEBUG_SCR_PRINT_LINES][MAX_DEBUG_SCR_PRINT_LINE_LEN];
int dscrprintlinescount = 0;
#endif

extern VariableAddress_t vaFluxShotDrawFunc;
extern VariableAddress_t vaFluxDrawDispatchA;
extern VariableAddress_t vaFluxDrawDispatchB;
extern float _lodScale;
extern void* _correctTieLod;
extern int _correctTieLod_Jump;
extern VariableAddress_t vaPlayerRespawnFunc;
extern VariableAddress_t vaPlayerSetPosRotFunc;
extern VariableAddress_t vaFlagUpdate_Func;
extern void FlushCache(int);
extern MenuElem_ListData_t dataCustomMaps;
#ifdef SCAVENGER_HUNT
extern scavHuntEnabled;
extern scavHuntShownPopup;
#endif

// int isUnloading __attribute__((section(".config"))) = 0;
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
	.disableDpadMovement = 0,
	.hideFluxReticle = 0,
	.fluxShotColor = 0,
	.fluxGlowColor = 0,
	.dlStyleFlips = 0,
	.enableTeamInfo = 0,
	.preferredGameServer = 0,
	.kothScrollSpeed = 0,
	.kothHillTransparency = 0,
	.kothHillFxId = FX_VISIBOMB_HORIZONTAL_LINES,
	.playerSyncRate = 0,
	.controllerDeadzone = 0,
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
int hasSonyMACAddress(void)
{
	int i;
	static int hasSonyMACAddressResult = -1;
	u8 mac[6];

	// use cached result
	// since the MAC address isn't going to change in the lifetime of the patch
	if (hasSonyMACAddressResult >= 0) return hasSonyMACAddressResult;

	// if we can't get the mac address, then assume we're on a PS2
	// but don't save result so that we run again
	if (!getMACAddress(mac)) return 1;

	// latest PCSX2 uses fixed 4 bytes (00:04:1F:82) followed by 2 bytes generated from the host net adapter
	// we'll assume that any client matching the first 4 bytes are on pcsx2
	// could have false positives
	// this must be checked before the sony list, which contains 00:04:1F
	if (mac[0] == 0x00 && mac[1] == 0x04 && mac[2] == 0x1F && mac[3] == 0x82)
		return hasSonyMACAddressResult = 0;

	// check if the first half of our mac address
	// matches any known sony mac addresses
	u32 firstHalfMacAddress = (mac[0] << 16) | (mac[1] << 8) | (mac[2] << 0);
	for (i = 0; i < SONY_MAC_ADDRESSES_COUNT; ++i) {
		if (SONY_MAC_ADDRESSES[i] == firstHalfMacAddress) return hasSonyMACAddressResult = 1;
	}

	return hasSonyMACAddressResult = 0;
}

//------------------------------------------------------------------------------
void sendClientType(void)
{
	static int sent = 0;
	static int stall = 0;
	ClientSetClientTypeRequest_t msg;

	void * connection = netGetLobbyServerConnection();
	if (!connection) { sent = 0; return; }

	if (sent) return;
	if (!getMACAddress(msg.mac)) return;

	// stall
	if (stall) { stall--; return; }

	// a real PS2 has a sony network adapter, anything else is an emulator
	msg.ClientType = hasSonyMACAddress() ? CLIENT_TYPE_NORMAL : CLIENT_TYPE_PCSX2;

	// send
	if (netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_SET_CLIENT_TYPE, sizeof(ClientSetClientTypeRequest_t), &msg)) { stall = 60; return; }
	sent = 1;
	DPRINTF("sent client type %d\n", msg.ClientType);
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
				case GAMETYPE_SIEGE: return "Siege at ";
				case GAMETYPE_CTF:  return "CTF at ";
				case GAMETYPE_DM:  return "DM at ";
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
		// if (patchStateContainer.CustomMapId > 0)
		// 	return MapLoaderState.MapName;

		return mapGetName(gameGetCurrentMapId());
	} else {
		location = LOCATION_LOADING;
		return "Loading";
	}
}
void runExceptionHandler(void)
{
	// invoke exception display installer
	if (*(u32*)EXCEPTION_HANDLER != 0) {
		if (!hasInstalledExceptionHandler) {
			((void (*)(void))EXCEPTION_HANDLER)();
			hasInstalledExceptionHandler = 1;
		}

		char * mapStr = checkMap();		// change "a fatal error as occured." to region and map.
		strncpy((char*)(EXCEPTION_HANDLER + 0x794), regionStr, 6);
		strncpy((char*)(EXCEPTION_HANDLER + 0x79a), mapStr, 20);
		
		// change display to match progressive scan resolution
		if (gfxGetIsProgressiveScan()) {
			*(u16*)(EXCEPTION_HANDLER + 0x9F4) = 0x0083;
			*(u16*)(EXCEPTION_HANDLER + 0x9F8) = 0x210E;
		} else {
			*(u16*)(EXCEPTION_HANDLER + 0x9F4) = 0x0183;
			*(u16*)(EXCEPTION_HANDLER + 0x9F8) = 0x2278;
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
		u32 ui = uiGetMenu(UI_MENU_EDIT_PROFILE_CONTROLS);
		if (ui && uiGetActiveMenu(UI_MENU_EDIT_PROFILE, 0)) {
			u32 cameraRotationUIPtr = *(u32*)(ui + 0x11C);
			if (cameraRotationUIPtr) {
				// max speed
				*(u32*)(cameraRotationUIPtr + 0x7C) = MAX_CAMERA_SPEED;

				// draw %
				if (uiGetActiveMenu(UI_MENU_EDIT_PROFILE_CONTROLS, 1)) {
					sprintf(buf, "%d%%", *(u32*)(cameraRotationUIPtr + 0x80));
					gfxScreenSpaceText(340, 310, 1, 1, 0x8069cbf2, buf, -1, 2, FONT_BOLD);
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
			gfxScreenSpaceText(273, 0.504 * SCREEN_WIDTH, 1, 1, 0x8069cbf2, buf, -1, 2, FONT_BOLD);
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
			int PlayerState = playerDeobfuscate(&player->state, 0);
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
					VECTOR temp = {0,0,0,0};
					VECTOR correction = {0.5,0.5,0.5,1.0};
					vector_subtract(out, hitMoby->position, (float*)event);
					vector_multiply(temp, hitMoby->rMtx.v2, correction);
					vector_add(out, out, temp);

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

		gfxScreenSpaceText(SCREEN_WIDTH - 5, 5, 0.75, 0.75, 0x80FFFFFF, buf, -1, 2, FONT_BOLD);
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
		if (p->weaponHeldId != GADGET_ID_FLUX) {
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
	if (gameSettings->GameLevel <= MAP_ID_BLACKWATER_DOCKS || patchStateContainer.CustomMapId > 0) {
		// If Maps Button Toggle isn't set to "Default"
		if (MapToggle != -1) {
			// Run Map Main Logic only if gametype is deathmatch.
			if (gameSettings->GameType == GAMETYPE_DM)
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
 * NAME :		flagGetTrackerIndex
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :		
 */
int flagGetTrackerIndex(Moby* flagMoby)
{
	if (!flagMoby)
		return -1;

	switch (flagMoby->oClass) {
		case MOBY_ID_CTF_RED_FLAG: return 0;
		case MOBY_ID_CTF_BLUE_FLAG: return 1;
		default: return -1;
	}
}

/*
 * NAME :		flagClearTrackedLastCarrier
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :		
 */
void flagClearTrackedLastCarrier(Moby* flagMoby)
{
	int flagIdx = flagGetTrackerIndex(flagMoby);
	if (flagIdx >= 0)
		flagTrackedLastCarrierIdx[flagIdx] = -1;
}

/*
 * NAME :		flagTrackCarrier
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :		
 */
void flagTrackCarrier(Moby* flagMoby, flagPVars_t* pvars)
{
	int flagIdx = flagGetTrackerIndex(flagMoby);
	if (flagIdx < 0 || !pvars)
		return;

	if (pvars->carrierIdx >= 0 && pvars->carrierIdx < GAME_MAX_PLAYERS) {
		flagTrackedLastCarrierIdx[flagIdx] = pvars->carrierIdx;
		return;
	}

	if (flagIsAtBase(flagMoby))
		flagTrackedLastCarrierIdx[flagIdx] = -1;
}

/*
 * NAME :		flagGetLastCarrierIdx
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :		
 */
int flagGetLastCarrierIdx(Moby* flagMoby)
{
	int flagIdx = flagGetTrackerIndex(flagMoby);
	if (flagIdx >= 0 && flagTrackedLastCarrierIdx[flagIdx] >= 0)
		return flagTrackedLastCarrierIdx[flagIdx];

	return -1;
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
	
	flagPVars_t* pvars = (flagPVars_t*)flagMoby->pVar;
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
	if (player->mpTeam == pvars->team) {
		flagReturnToBase(flagMoby, 0, pIdx);
		flagClearTrackedLastCarrier(flagMoby);
	} else {
		int flagIdx = flagGetTrackerIndex(flagMoby);
		if (flagIdx >= 0)
			flagTrackedLastCarrierIdx[flagIdx] = pIdx;
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
	
	flagPVars_t* pvars = (flagPVars_t*)flagMoby->pVar;
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

static int flagPickupDelayReady(Moby* flagMoby, flagPVars_t* pvars, Player* player, int gameTime)
{
	int lastCarrierIdx;

	if (!flagMoby || !pvars || !player)
		return 0;

	if (gameConfig.customModeId == CUSTOM_MODE_MIDFLAG) // Logic doesnt work for midflag
		return 1;

	if (flagIsAtBase(flagMoby))
		return 1;

	lastCarrierIdx = flagGetLastCarrierIdx(flagMoby);
	if (player->mpIndex == lastCarrierIdx) {
		// self: player who dropped the flag
		if ((pvars->timeFlagDropped + (TIME_SECOND * 1.5)) > gameTime)
			return 0;
	} else if (player->mpTeam != pvars->team) {
		// team: dropper's teammates
		if ((pvars->timeFlagDropped + (TIME_SECOND * 0.5)) > gameTime)
			return 0;
	} else {
		// enemy: flag's own team returning it
		if ((pvars->timeFlagDropped + (TIME_SECOND * 0.5)) > gameTime)
			return 0;
	}

	return 1;
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
	flagPVars_t* pvars = (flagPVars_t*)flagMoby->pVar;

	// if flag moby or pvars don't exist, stop.
	if (!flagMoby || !pvars)
		return;

	flagTrackCarrier(flagMoby, pvars);

    // if flag state is not 1 (being picked up) and if flag is returning to base
    if (flagMoby->state != 1 || flagIsReturning(flagMoby))
        return;

    // flag is being picked up
    if (flagIsBeingPickedUp(flagMoby))
        return;
    
	// return to base if flag has been idle for 40 seconds and not already at base.
	if ((pvars->timeFlagDropped + (TIME_SECOND * 40)) < gameTime && !flagIsAtBase(flagMoby) && !flagIsAtBase(flagMoby)) {
		flagReturnToBase(flagMoby, 0, 0xff);
		flagClearTrackedLastCarrier(flagMoby);
		return;
	}

    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player* player = players[i];
        if (!player)
            continue;

        // Don't allow input from players whom are dead
        if (playerDeobfuscate(&player->stateType, 0) == PLAYER_TYPE_DEATH)
            continue;

        // skip player if they've only been alive for < 180ms
        if (player->timers.timeAlive < 180)
            continue;

        // skip if player state is in vehicle and critterMode is on
		if (player->camera && player->camera->camHeroData.critterMode)
			continue;

    	// skip player if in vehicle
		if (player->vehicle && playerDeobfuscate(&player->state, 0) == PLAYER_STATE_VEHICLE)
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

		if (!flagPickupDelayReady(flagMoby, pvars, player, gameTime))
			continue; // Gate here for flag pickup delay

		// player is on different team than flag and player isn't already holding flag
		if (player->mpTeam != pvars->team) {
			if (!player->flagMoby) {
				flagRequestPickup(flagMoby, player->mpIndex);
				return;
			}
		} else {
			// if player is on same team as flag and close enough to return it
			vector_subtract(t, pvars->basePos, flagMoby->position);
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
		flagPVars_t* pvars = (flagPVars_t*)flagMoby->pVar;
		if (pvars && flagPickupDelayReady(flagMoby, pvars, remotePlayer, gameGetTime()))
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
			FlushCache(0); // ensure execution does not use stale cached code
			FlushCache(2);
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
	static int FinishedConvertingTracks = 0;
	static int TotalTracks = 0;
	static short CurrentTrack = -1;
	static short NextTrack = -1;
	int DefaultMultiplayerTracks = 13;
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

	#if UYA_PAL
	int CodeSegmentCheck = *(u32*)0x01FFFD00 == 0x00575CC8;
	#else
	int CodeSegmentCheck = *(u32*)0x01FFFD00 == 0x00574F88;
	#endif
	if (!musicGetSector() || CodeSegmentCheck)
		return;
	
	if (config.enableSingleplayerMusic) {
		if (!FinishedConvertingTracks) {
			int CustomSector = 0x1d8a;
			u32 NewTracksLocation = 0x001f8588; // Overwrites current tracks too.
			if (musicGetSector() != CustomSector)
				musicSetSector(CustomSector);

			int MultiplayerSector = *(u32*)0x001F8584;
			int Stack = 0x000269300;
			// int WAD_Table = 0x001f7f88; // Kept for historical purposes.
			int a;
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
							++TotalTracks;
						}
					}
				}
				// Zero out stack to finish the job.
				memset((u32*)Stack, 0, 0x1818);
				
				FinishedConvertingTracks = 1;
			}
		}
	}

	// If in game
	if (isInGame()) {
		music_Globals* music = musicGetGlobals();
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
		if (music->play.track == -1 && music->play.status == 0) {
			// plays a random track
			int randomTrack = randRangeInt(*(int*)musicTrackRangeMin(), *(int*)musicTrackRangeMax()) % *(int*)musicTrackRangeMax();
			DPRINTF("\nrandomTrack: %d", randomTrack);
			musicPlayTrack(randomTrack, MUSIC_FLAG_KEEP_PLAYING_AFTER, 1024);
		}
		// save current and next track
		if (music->play.status == MUSIC_STATUS_LOADING && CurrentTrack < 0 && NextTrack < 0 && music->play.track != -1) {
			CurrentTrack = music->play.track;
		} else if ((music->play.status == MUSIC_STATUS_KEEP_PLAYING_AFTER || music->play.status == MUSIC_STATUS_STOP_PLAYING_AFTER) && NextTrack < 0) {
			NextTrack = music->play.track;
		} else if (NextTrack != music->play.track) {
			CurrentTrack = NextTrack;
			NextTrack = music->play.track;
		}
		if ((CurrentTrack > DefaultMultiplayerTracks * 2) && CurrentTrack > -1 && (music->play.remain <= 0x3000) && music->play.queuelen == MUSIC_QUEUELEN_PLAYING) {
			musicTransitionTrack(0 ,0 ,0 ,0);
		}
		// printf("\n t: %x c: %x, n: %x", *(int*)musicTrackRangeMax(), CurrentTrack, NextTrack);
		// R3 + Up: Random Track
		int randomTrackButton = padGetButtonDown(0, PAD_UP | PAD_R3) > 0;
		if (randomTrackButton)
			music->play.status = MUSIC_STATUS_PLAY_NEXT;
	} else if (isInMenus() && FinishedConvertingTracks) {
		FinishedConvertingTracks = 0;
		TotalTracks = 0;
		CurrentTrack = -1;
		NextTrack = -1;
	}
}

/*
 * NAME :		patchCameraPull
 * DESCRIPTION :	Disables Camera Pull for player weapons
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchCameraPull(void)
{

	Player* p = playerGetFromSlot(0);
	// p->fps.vars.cameraZ.target_slowness_factor_quick = 0; // doesn't do anything
	// p->fps.vars.cameraZ.target_slowness_factor_aim = 0; // how much aim assist "locks onto" enemies X/horizontally
	// p->fps.vars.cameraY.target_slowness_factor = 0; // how much aim assist "locks onto" enemies Y/vertically
	p->fps.vars.cameraY.strafe_turn_factor = 0; // camera pull var 1
	p->fps.vars.cameraY.strafe_tilt_factor = 0; //camera pull var 2
}

/*
 * NAME :		teamInfo
 * DESCRIPTION :	Displays teamate health and cycle weapon upgrade status
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			JelloGiant
 */
void teamInfo(void)
{
	float height_start = SCREEN_HEIGHT;
	float height_step = SCREEN_HEIGHT / 18;
	float width_start = SCREEN_WIDTH / 20;
	int misc_pad = 3; // misc aligning and spacing
	u32 icon_colors[2] = {0x80C0C0C0, 0x50d04040}; //v1, v2
	Player* localPlayer = playerGetFromSlot(0);
	GameSettings * gameSettings = gameGetSettings();
	if (!localPlayer || !localPlayer->isLocal || !localPlayer->pMoby)
		return;
	int teamColor = localPlayer->mpTeam;
	Player ** players = playerGetAll();
	int i;
	float height_spacing = 1; // teamates listed vertically
	char name [5]; // first 4 chars of name will be displayed
	char buf[9]; // 4 chars + space + 1-3 digits of health + null terminator
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		Player* p = players[i];
		if (p && !p->isLocal && p->mpTeam == teamColor && p->pNetPlayer && p->pNetPlayer->pNetPlayerData) {
			int raw_upgrades = p->pNetPlayer->pNetPlayerData->rank[1];
			int flux_color  = icon_colors[(raw_upgrades & 0x080000 ? 1 : 0)];
			int blitz_color = icon_colors[(raw_upgrades & 0x040000 ? 1 : 0)];
			int gbomb_color = icon_colors[(raw_upgrades & 0x200000 ? 1 : 0)];
			strncpy(name, gameSettings->PlayerNames[p->mpIndex], 4);
			name[4] = '\0';
			sprintf(buf, "%s %d", name, playerMapHealth(p->pNetPlayer->pNetPlayerData->hitPoints));
			gfxScreenSpaceText(misc_pad,height_start - (height_step * height_spacing) + misc_pad, .8, .8, 0x8069cbf2, buf, -1, 0, FONT_BOLD);
			gfxDrawHUDIcon(SPRITE_WEAPON_GRAVITY_BOMB, width_start * 3, height_start - (height_step * height_spacing), 16, gbomb_color);
			gfxDrawHUDIcon(SPRITE_WEAPON_GLITZ_GUN, width_start * 4 , height_start - (height_step * height_spacing), 16, blitz_color);
			gfxDrawHUDIcon(SPRITE_WEAPON_FLUX_RIFLE_4, width_start * 5, height_start - (height_step * height_spacing), 16, flux_color);
			if (p->flagMoby) {
				gfxDrawHUDIcon(SPRITE_FLAG, width_start * 6, height_start - (height_step * height_spacing), 16, icon_colors[0]);
			}
			height_spacing +=1;
		}
	}
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
	
	if (voteToEndState.TimeoutTime > gameTime) {
		int haveVoted = 0;
		if (player)
			haveVoted = voteToEndState.Votes[player->mpIndex];

		// draw
		int secondsLeft = (voteToEndState.TimeoutTime - gameTime) / TIME_SECOND;
		char* buttonCombo = "(\x18+\x19) ";
		snprintf(buf, sizeof(buf), "%sVote to End (%d/%d)    %d...", haveVoted ? "" : buttonCombo, voteToEndState.Count, votesNeeded, secondsLeft);
		gfxScreenSpaceText(12, SCREEN_HEIGHT - 18, 1, 1, 0x80000000, buf, -1, 0, FONT_BOLD);
		gfxScreenSpaceText(10, SCREEN_HEIGHT - 20, 1, 1, 0x80FFFFFF, buf, -1, 0, FONT_BOLD);
			
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
 * NAME :		handleGadgetEvent
 * DESCRIPTION :
 * 			Reads gadget events and patches them if needed.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void handleGadgetEvents(int player, char gadgetEventType, int dispatchTime, short gadgetId, int gadgetType, struct tNW_GadgetEventMessage * message)
{
	// Force all incoming weapon shot events to happen immediately.
	const int MAX_DELAY = TIME_SECOND * 0;

	int original_activeTime = -1;
	if (message) {

		original_activeTime = message->ActiveTime;
		// put clamp on max delay
		int delta = dispatchTime - gameGetTime();
		if (delta > MAX_DELAY) {
			dispatchTime = gameGetTime() + MAX_DELAY;
			if (message) message->ActiveTime = dispatchTime;
		} else if (delta < 0) {
			dispatchTime = gameGetTime() - 1;
			if (message) message->ActiveTime = dispatchTime;
		}
  } else if (dispatchTime < 0) {
    dispatchTime = gameGetTime() - TIME_SECOND;
  }
	// if (original_activeTime == 0x1 || original_activeTime > 0x10000000 || original_activeTime == 13) // weird bug with flux rifle
	// the flux's (gadgetId == 3) activeTime is -1 if it doesn't hit and a GuberId if it does hit. Don't override the guberId.
	if (gadgetId == 3)
		if (message)
			if (original_activeTime != -1)
				message->ActiveTime = original_activeTime; // set it back to the guber ID

/*
	DPRINTF("handleGadgetEvents called with:\n");
	DPRINTF("  player: %08x\n", player);
	DPRINTF("  gadgetEventType: %d\n", (int)gadgetEventType);
	DPRINTF("  dispatchTime: %d\n", dispatchTime);
	DPRINTF("  gadgetId: %d\n", gadgetId);
	DPRINTF("  gadgetType: %d\n", gadgetType);

	if (message) {
			DPRINTF("  message:\n");
			DPRINTF("    GadgetId: %d\n", message->GadgetId);
			DPRINTF("    PlayerIndex: %d\n", (int)message->PlayerIndex);
			DPRINTF("    GadgetEventType: %d\n", (int)message->GadgetEventType);
			DPRINTF("    ExtraData: %d\n", (int)message->ExtraData);
			DPRINTF("		 Original ActiveTime: %d\n", original_activeTime);
			DPRINTF("    ActiveTime: %d\n", message->ActiveTime);
			DPRINTF("    TargetUID: %u\n", message->TargetUID);
			DPRINTF("    FiringLoc: [%.2f, %.2f, %.2f]\n",
							message->FiringLoc[0], message->FiringLoc[1], message->FiringLoc[2]);
			DPRINTF("    TargetDir: [%.2f, %.2f, %.2f]\n",
							message->TargetDir[0], message->TargetDir[1], message->TargetDir[2]);
			DPRINTF("Broadcasting message from: %p\n", (void*)handleGadgetEvents);
	} else {
			DPRINTF("  message: NULL\n");
	}
*/
	// run base command
	((void (*)(int, char, int, short, int, struct tNW_GadgetEventMessage*))GetAddress(&vaGadgetEventFunc))(player, gadgetEventType, dispatchTime, gadgetId, gadgetType, message);
}

void patchGadgetEvents(void)
{
	if (patched.gadgetEvents)
		return;

	HOOK_JAL(GetAddress(&vaGadgetEventHook), &handleGadgetEvents);
	patched.gadgetEvents = 1;
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
	// if player is found and presses needed button, equip hypershot.
	if (p && playerPadGetButtonDown(p, hypershotGetButton()) > 0)
		playerEquipWeapon(p, GADGET_ID_SWINGSHOT);
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
	runPlayerPositionSmooth();
}

/*
 * NAME :		patchHideFluxReticle
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchHideFluxReticle(void)
{
	if (patched.config.hideFluxReticle == config.hideFluxReticle)
		return;
 
	Moby* mobyStart = mobyListGetStart();
	Moby* mobyEnd = mobyListGetEnd();
	while (mobyStart < mobyEnd) {
		if (mobyStart->oClass == MOBY_ID_WEAPON_FLUX_RIFLE) {
			int reticule = ((u32)mobyStart->pUpdate + 0x3ac);
			if (mobyStart->pUpdate && *(u32*)reticule != 0) {
				*(u32*)reticule = 0x24040000 | config.hideFluxReticle;
				patched.config.hideFluxReticle = config.hideFluxReticle;
			}
			break;
		}
		++mobyStart;
	}
}

/*
 * Preset colours, shared by every recolourable effect.
 *
 * Each renderer builds its colour word as 0x00RRGGBB from two immediates, so a
 * preset is a whole colour word. Index 0 leaves the engine's colour alone.
 */
static u32 fluxColorList[FLUX_COLOR_COUNT] = {
	0x00804000,     // Vanilla      (128,64,0)   the engine's own colour
	0x000000FF,     // Red          (255,0,0)
	0x0000FF00,     // Green        (0,255,0)
	0x00FF0000,     // Blue         (0,0,255)
	0x00000000,     // Black        (0,0,0)
	0x00FFFFFF,     // White        (255,255,255)
	0x00800080,     // Purple       (128,0,128)
	0x00AC5AFF,     // Pink         (255,90,172)
	0x0000FFFF,     // Yellow       (255,255,0)
	0x00FFA000,     // Light Blue   (0,160,255)
	0x0035C835,     // Light Green  (53,200,53)
	0x004747FF,     // Light Red    (255,71,71)
	0x0000008B,     // Dark Red     (139,0,0)
	0x00006400,     // Dark Green   (0,100,0)
	0x008B0000,     // Dark Blue    (0,0,139)
};

/*
 * NAME :		applyFluxBeamColor
 * DESCRIPTION :
 * 			Writes one colour word into the beam trail's three colour builders.
 * NOTES :
 *          The trail builds each colour from two immediates; the register numbers
 *          differ between builds, so only the immediate halves are replaced.
 *          The shared `lui` at +0x38 is left alone -- its immediate is dropped by
 *          the per-segment `or` that adds the alpha.
 * ARGS :
 *          anchor: the base `lui` of the beam renderer
 *          color:  0x00RRGGBB
 * RETURN :
 * AUTHOR :			Philip762
 */
static void applyFluxBeamColor(u32 anchor, u32 color)
{
	u32 want;

	want = (*(u32*)(anchor + 0x00) & 0xFFFF0000) | (color >> 16);
	POKE_U32(anchor + 0x00, want);

	want = (*(u32*)(anchor + 0x0C) & 0xFFFF0000) | (color & 0xFFFF);
	POKE_U32(anchor + 0x0C, want);

	/*
	 * The two tween colours need their `lui` written too, not just their `ori`.
	 * Leaving +0x38/+0x44 at the stock 0x00FF pins the red channel to maximum on
	 * two of the beam's three colour stops, so every picked colour with R < 0xFF
	 * drifts toward red (blue renders magenta, green renders yellow).
	 */
	want = (*(u32*)(anchor + 0x38) & 0xFFFF0000) | (color >> 16);
	POKE_U32(anchor + 0x38, want);

	want = (*(u32*)(anchor + 0x40) & 0xFFFF0000) | (color & 0xFFFF);
	POKE_U32(anchor + 0x40, want);

	want = (*(u32*)(anchor + 0x44) & 0xFFFF0000) | (color >> 16);
	POKE_U32(anchor + 0x44, want);

	want = (*(u32*)(anchor + 0x4C) & 0xFFFF0000) | (color & 0xFFFF);
	POKE_U32(anchor + 0x4C, want);
}

/*
 * NAME :		applyFluxGlowColor
 * DESCRIPTION :
 * 			Writes one colour word into the beam's decoration builders.
 * NOTES :
 *          FUN_004098d8 builds its colour four separate times, once per quad type
 *          it draws. Each is `lui rt,0x0080` + `ori rt,rt,0x4020` at a fixed
 *          offset from the same anchor, with the register r17 in every build and
 *          the `ori` 0x0C after its `lui`.
 * ARGS :
 *          anchor: the base `lui` of the beam renderer
 *          color:  0x00RRGGBB
 * RETURN :
 * AUTHOR :			Philip762
 */
static void applyFluxGlowColor(u32 anchor, u32 color)
{
	static const u16 sites[4][2] = {
		{ 0x668, 0x66C },
		{ 0x754, 0x760 },
		{ 0x844, 0x850 },
		{ 0x9A0, 0x9AC },
	};
	u32 want;
	int i;

	for (i = 0; i < 4; ++i) {
		want = (*(u32*)(anchor + sites[i][0]) & 0xFFFF0000) | (color >> 16);
		POKE_U32(anchor + sites[i][0], want);

		want = (*(u32*)(anchor + sites[i][1]) & 0xFFFF0000) | (color & 0xFFFF);
		POKE_U32(anchor + sites[i][1], want);
	}
}

/*
 * ============================== MAP SCOPE ==============================
 * The feature acts on every build whose beam anchor is tabled in the interop table,
 * and on no other.
 *
 * This used to be hard-coded to Bakisi (`gameGetCurrentMapId() == MAP_ID_BAKISI`).
 * That was right while Bakisi was the only verified map, but the anchor is now tabled
 * and dump-verified for all 22 builds, so the restriction had stopped being a safety
 * net and become a silent no-op everywhere else: on Hoven, OutpostX12 and the rest
 * nothing was poked at all and the beam stayed the engine's default colour.
 *
 * Asking the interop table instead of a map id is the stronger check anyway. It
 * cannot drift: a build with no verified address resolves to 0 and is skipped, which
 * is exactly the "do not poke an address you have not verified" rule the map id was
 * standing in for.
 * ======================================================================
 */

/*
 * NAME :		fluxMapInScope
 * DESCRIPTION :
 * 			Does this build have a verified beam anchor to write to?
 * NOTES :
 *          GetAddress resolves through __LocalGetAddress, which is indexed by the
 *          CURRENT map, so this cannot drift out of step with the map that is loaded.
 *
 *          UPDATE PHASE ONLY. Resolving an address is the one thing this feature has
 *          repeatedly crashed on when done from the draw phase; the draw path reads
 *          fluxBeamAnchorCached instead.
 * ARGS :
 * RETURN :		1 if the feature should act
 * AUTHOR :			Philip762
 */
static int fluxMapInScope(void)
{
	return GetAddress(&vaFluxBeamColor) != 0;
}

/*
 * ---------------------------------------------------------------------------
 * Per-player Flux colours (beam + glow).
 *
 * The beam and glow colours are static instruction immediates, normally written
 * from the LOCAL player's config. Drawing somebody else's Flux in their colours
 * means swapping those immediates for the duration of that shot's own draw call,
 * then putting the local player's back.
 *
 * The swap is scoped by intercepting the draw callback rather than the renderer:
 * the engine registers a per-moby draw function through gfxRegisterDrawFunction, so
 * redirecting it yields a callback that receives the moby and therefore knows whose
 * shot it is. Applying the colours any earlier (say at update time) would leave them
 * latched for whichever shot happened to draw last.
 *
 * Colour INDICES travel over the wire, not resolved colours, so a receiver always
 * resolves them through its own preset table -- a client with a different table then
 * draws a different-but-valid colour instead of a garbage one.
 * ---------------------------------------------------------------------------
 */

#define FLUX_COLOR_RESEND_MS   (2000)

/* The shot's registered draw function, relative to the beam colour anchor:
 * vaFluxBeamColor - 0x5D0 == FUN_00408d40 on every build. */
#define FLUX_DRAW_FUNC_DELTA   (0x5D0)

/*
 * Offset from a shot moby's pVar to the Player who fired it.
 *
 * Read, never written. It is a pointer into the player table, so it is validated
 * before being dereferenced -- see onFluxShotDraw and fluxApplyOwnerColors.
 */
#define FLUX_SHOT_OWNER_OFFSET (0x3C)


static const u32 fluxBeamVanilla = 0x00FFFF00;
static const u32 fluxGlowVanilla = 0x00FF1010;

static char fluxPlayerBeam[GAME_MAX_PLAYERS];
static char fluxPlayerGlow[GAME_MAX_PLAYERS];

static int fluxColorLastBeam = -1;
static int fluxColorLastGlow = -1;
static int fluxColorResendTime = 0;

/*
 * NAME :		fluxColorResolve
 * DESCRIPTION :
 * 			Turns preset indices into engine colour words.
 * NOTES :
 *          Index 0 means the engine's own colour, which is not in the preset list,
 *          so it maps to the vanilla word rather than list[0].
 * ARGS :
 *          beam/glow:        preset indices
 *          outBeam/outGlow:  receive the colour words
 * RETURN :
 * AUTHOR :			Philip762
 */
static void fluxColorResolve(int beam, int glow, u32 *outBeam, u32 *outGlow)
{
	*outBeam = (beam > 0 && beam < FLUX_COLOR_COUNT)
	           ? fluxColorList[beam] : fluxBeamVanilla;
	*outGlow = (glow > 0 && glow < FLUX_COLOR_COUNT)
	           ? fluxColorList[glow] : fluxGlowVanilla;
}

/*
 * NAME :		fluxPlayerIndexFromPtr
 * DESCRIPTION :
 * 			Maps a shot owner to the index its colours are stored under.
 * NOTES :
 *          THE INDEX IS THE ENGINE'S OWN PlayerId, `owner->fps.vars.camSettingsIndex`.
 *          player.h documents that field as `camSettingsIndex // aka: PlayerId`, and it
 *          is exactly what the SEND path broadcasts.
 *
 *          It used to scan playerGetFromSlot(i) for a matching pointer instead -- and
 *          that is a DIFFERENT NUMBERING. Using one numbering to send and the other to
 *          receive is what made every remote shot render with the local player's
 *          colour, and the two-client log shows both halves failing at once:
 *
 *              client 1 (sends idx=0, receives idx=1)
 *                  FLUXHOOK   last owner=0x00319F80 slot=-1
 *              client 2 (sends idx=1, receives idx=0)
 *                  FLUXHOOK   last owner=0x003151C0 slot=5
 *
 *          Client 1 could not resolve the remote owner at all, so fluxApplyOwnerColors
 *          returned without touching the immediates and the shot kept whatever was
 *          already in them -- the local player's colours. Client 2 DID resolve it, but
 *          to slot 5, while client 1's colours had been stored under index 0, so it
 *          rendered the wrong table entry.
 *
 *          Reading the same field on both ends makes them agree BY CONSTRUCTION rather
 *          than by two independent guesses.
 *
 *          The value is RANGE CHECKED, not trusted. It is read out of a moby's pVar
 *          block, and that block is recycled from shot to shot; a stale or unrelated
 *          pointer there would otherwise index the colour arrays out of bounds.
 * ARGS :
 *          owner: the player to identify
 * RETURN :		PlayerId (0..GAME_MAX_PLAYERS-1), or -1
 * AUTHOR :			Philip762
 */
static int fluxPlayerIndexFromPtr(Player *owner)
{
	int id;

	if (owner == 0)
		return -1;

	id = (int)owner->fps.vars.camSettingsIndex;

	if (id < 0 || id >= GAME_MAX_PLAYERS)
		return -1;

	return id;
}

/*
 * NAME :		onFluxColorsRemote
 * DESCRIPTION :
 * 			Stores a broadcast colour choice for one player.
 * ARGS :
 *          connection: unused
 *          data:       PlayerFluxColors_t
 * RETURN :		payload size
 * AUTHOR :			Philip762
 */
int onFluxColorsRemote(void *connection, void *data)
{
	PlayerFluxColors_t *msg = (PlayerFluxColors_t*)data;

	(void)connection;

	/* data can arrive from a mismatched build; never trust the index */
	if (data == 0) {
		DPRINTF("FLUXRECV null payload\n");
		return sizeof(PlayerFluxColors_t);
	}

	if (msg->PlayerIdx < 0 || msg->PlayerIdx >= GAME_MAX_PLAYERS) {
		DPRINTF("FLUXRECV rejected idx=%d (max %d)\n",
		        (int)msg->PlayerIdx, GAME_MAX_PLAYERS);
		return sizeof(PlayerFluxColors_t);
	}

	fluxPlayerBeam[(int)msg->PlayerIdx] = msg->Beam;
	fluxPlayerGlow[(int)msg->PlayerIdx] = msg->Glow;

	return sizeof(PlayerFluxColors_t);
}

/*
 * NAME :		patchFluxColorSync
 * DESCRIPTION :
 * 			Publishes the local player's Flux colours to the other clients.
 * NOTES :
 *          Broadcast because the DME server relays it to the other clients in the
 *          game -- the same route the vote-to-end state uses. Sent on change and
 *          re-sent periodically so a client that missed one converges.
 *
 *          The sender identifies itself with its own camera-settings index, which is
 *          the player id playerSync already sends as its PlayerIdx.
 * ARGS :
 * RETURN :
 * AUTHOR :			Philip762
 */
void patchFluxColorSync(void)
{
	void *connection = netGetDmeServerConnection();
	Player *local = playerGetFromSlot(0);
	PlayerFluxColors_t msg;
	int beam = config.fluxShotColor;
	int glow = config.fluxGlowColor;
	int playerId;
	int changed = (beam != fluxColorLastBeam || glow != fluxColorLastGlow);

	if (!isInGame())
		return;

	/*
	 * Only broadcasts from a map the feature can act on. A client on an unverified
	 * build has nothing to send and nothing to apply.
	 */
	if (!fluxMapInScope())
		return;

	if (connection == 0 || local == 0)
		return;

	if (!changed) {
		if (fluxColorResendTime != 0 && gameGetTime() < fluxColorResendTime)
			return;
	}

	fluxColorLastBeam = beam;
	fluxColorLastGlow = glow;
	fluxColorResendTime = gameGetTime() + FLUX_COLOR_RESEND_MS;

	/*
	 * PlayerIdx must be the engine's own PlayerId, because the receiver both STORES
	 * by it and RE-DERIVES it from the owning Player at draw time
	 * (fluxPlayerIndexFromPtr). Sending anything else -- a slot index, a camera
	 * setting -- makes the two ends disagree and every remote shot renders with the
	 * local player's colours, which is the bug this comment exists to prevent.
	 *
	 * Bounds-checked here as well as on the receiver: an out-of-range id would be
	 * rejected by every other client, so it is better to not broadcast at all and say
	 * so than to have the fault look like a receive-side problem.
	 */
	playerId = (int)local->fps.vars.camSettingsIndex;

	if (playerId < 0 || playerId >= GAME_MAX_PLAYERS) {
		DPRINTF("FLUXSEND refused: local PlayerId %d out of range (max %d)\n",
		        playerId, GAME_MAX_PLAYERS);
		return;
	}

	msg.PlayerIdx = (char)playerId;
	msg.Beam = (char)beam;
	msg.Glow = (char)glow;
	msg.Padding = 0;

	netBroadcastCustomAppMessage(connection, CUSTOM_MSG_ID_PLAYER_FLUX_COLORS,
	                             sizeof(msg), &msg);
}

/*
 * ---------------------------------------------------------------------------
 * Per-player Flux rendering -- bisection stage gate.
 * ---------------------------------------------------------------------------
 *
 * The hook address is settled and verified: vaFluxShotDrawFunc ==
 * vaFluxBeamColor - FLUX_DRAW_FUNC_DELTA (0x5D0), the code there has the moby draw
 * callback prologue (`daddu s0,a0,zero`, so the moby arrives in a0) and jal's both
 * renderers from inside. verify_flux_draw_table.py asserts all of that per build.
 *
 * Stage 4 is the complete feature: the hook writes the shot owner's colours into the
 * immediates, lets the original draw through them, then puts the local colours back.
 * Stages 0-3 were bisection rungs and are gone; every one of them is subsumed here.
 */
#define FLUX_DRAW_STAGE 4

/*
 * Install the shot-draw hook. 0 leaves the hook out entirely (the shot then always
 * draws with the local player's colours) and is only useful for isolating a fault.
 *
 * ================== WHICH MAPS THE HOOK ACTUALLY REACHES ==================
 * The colour write itself (patchFluxShotColor) now works on every build with a tabled
 * anchor -- see the MAP SCOPE note above. The HOOK is narrower, because it needs two
 * more tabled addresses than the colour does:
 *
 *   vaFluxShotDrawFunc   tabled for all 22 builds
 *   vaFluxDrawDispatchA  tabled for NTSC Bakisi only
 *   vaFluxDrawDispatchB  tabled for NTSC Bakisi only
 *
 * fluxInstallDrawDispatchHook skips any build whose dispatch entry is 0, so off
 * Bakisi the local colour applies but each player's shot draws with the LOCAL
 * player's colours rather than the shooter's. That is a graceful degradation, not a
 * fault: the draw-table redirect alone does not survive a frame, because the engine
 * re-registers the original callback every frame and only the pre-dispatch hook
 * re-applies the redirect.
 *
 * RunDrawRoutines is not at a fixed address across builds -- the literal
 * `jal 0x00456158` from ntsc.40.Bakisi matches nothing in the other dumps -- so each
 * build needs its own discovery before its two call sites can be tabled.
 * =========================================================================
 *
 * BAKISI ADDRESSES, VERIFIED TWO WAYS (on-disk ntsc.40.Bakisi.bin AND the Ghidra
 * program UYA_mp_bakisi_eeMemory.bin -- which for Bakisi agrees with the dump
 * byte-for-byte, unlike Hoven where Ghidra held a different binary):
 *
 *   vaFluxBeamColor     .Bakisi = 0x00409310   3C040080  lui a0,0x0080
 *   vaFluxShotDrawFunc  .Bakisi = 0x00408D40   27BDFFF0  addiu sp,sp,-16
 *                                               FFB00000  sd s0,0(sp)
 *                                               0080802D  daddu s0,a0,zero
 *   anchor - FLUX_DRAW_FUNC_DELTA  == 0x00409310 - 0x5D0 == 0x00408D40  (matches)
 *
 * Ghidra also confirms HOW the callback reaches the engine: the only xref to
 * 0x00408D40 is a PARAM reference from FUN_00408cc8, where
 *
 *     jal 0x00456108          ; gfxRegisterDrawFunction(callback, moby)
 *      addiu a0, a0, -0x72c0  ; delay slot builds 0x00408D40
 *
 * registers it. So the engine learns the callback address by value at registration
 * time, and HOOK_JAL is effective only because the body at that address is what
 * actually executes.
 *
 * KNOWN UNRESOLVED, reported honestly rather than hidden: sessions with the hook
 * installed have ended in a PCSX2 recompiler fault. The Bakisi run is the useful
 * one:
 *
 *     FLUXHOOK stage 4: shot draw 0x00408D40 -> 0x000D4280
 *     FLUXHOOK   remote=0 local=0 no-owner=0 no-slot=0 no-pvar=0 no-moby=0
 *     [EE] Impossible block clearing failure
 *     R5900 Exception: Jump to unaligned address (PC: 0x000000fe)
 *
 * ALL SIX COUNTERS ZERO means the hook was never entered, on any install, in the
 * whole session -- so the fault is reached without any of this patch's draw code
 * running. That is why the diagnostics below now record a ring of events with full
 * context: the counters alone cannot distinguish "never called" from "called with a
 * plausible-looking wrong owner".
 */
#define FLUX_INSTALL_DRAW_HOOK 1

/*
 * There is deliberately NO saved "original draw function" pointer any more.
 *
 * The previous version overwrote the draw function's entry with HOOK_JAL and cached
 * the address here so the hook could call through it. That is exactly what crashed on
 * Bakisi (see the note above), so the draw function is no longer patched at all and
 * there is nothing to cache. The original callback now arrives as an ARGUMENT, from
 * the registration wrapper that the engine calls.
 */

/*
 * NOTE -- the guard above must stay NARROW.
 *
 * Everything the patch calls unconditionally has to live outside it. When the guard
 * was widened to cover a function that main() calls, the build failed to LINK:
 *
 *     main.o: undefined reference to `patchFluxRestoreLocal'
 *
 * So anything on the unconditional path does not belong inside FLUX_INSTALL_DRAW_HOOK.
 * check_guarded_symbols.py now fails the suite for this class of mistake.
 */

/*
 * ===========================================================================
 * Everything from here to the matching #endif belongs to the draw hook.
 *
 * It is ALL inside FLUX_INSTALL_DRAW_HOOK on purpose. With the switch at 0 none
 * of it has a caller, and this build promotes an unused static to an error:
 *
 *     main.c:3099: warning: `fluxApplyOwnerColors' defined but not used
 *     main.c:3141: warning: `fluxRestoreLocalIfSwapped' defined but not used
 *     make: *** Error 1
 *
 * So anything only the hook reaches -- its state, its counters, its colour
 * helpers -- must be inside this guard. Anything main() calls unconditionally
 * must stay OUTSIDE it; that is the opposite mistake, and it broke the link once
 * (see the note above).
 * ===========================================================================
 */
/*
 * Registered-draw replacement table.
 *
 * RegisterDrawFunction caps its own table at 0x40 entries, so this can never need
 * more than that, and it is only consulted when a shot is registered or drawn.
 * pMoby is fully aligned, so masking its low bits is a safe way to derive the slot
 * and means no hashing and no allocation.
 */
#define FLUX_REG_MAX 0x40

/*
 * The engine's own registered-draw table, from RegisterDrawFunction (Bakisi
 * 0x00456108). Each value is the base-plus-offset arithmetic the instruction actually
 * performs, and the stores confirm which array is which:
 *
 *     lui  a2,0x25 ; lw    a2,-0x7f74(a2)  -> 0x00250000 - 0x7f74 = 0x0024808C count
 *     lui  a0,0x26 ; addiu a0,a0,-0x5480   -> 0x00260000 - 0x5480 = 0x0025AB80 Func[]
 *     lui  v1,0x26 ; addiu v1,v1,-0x5380   -> 0x00260000 - 0x5380 = 0x0025AC80 Moby[]
 *
 *     sw   a3,0x0(v0)   v0 from a0-base   Func[i] = callback   (a3 = a0 = 1st arg)
 *     sw   t0,0x0(v1)   v1 from v1-base   Moby[i] = moby       (t0 = a1 = 2nd arg)
 *
 * Func[] is what the engine calls through when it draws a registered moby, so writing
 * it intercepts the draw WITHOUT patching any code.
 *
 * ============================ TWO ERRORS WERE MADE HERE ====================
 * Both silently disabled the entire per-player feature while everything else looked
 * healthy, and both produced the SAME symptom as "the engine never calls us": no
 * FLUXHOOK table line, no FLUXDRAW events, and every player seeing local colours.
 * Nothing crashed, so it read as a feature that simply did not work.
 *
 *   1. count was written as 0x0025808C. The correct value is 0x0024808C -- a 64KB
 *      error, 0x25 instead of 0x24 in the low half. Reading the wrong word produced a
 *      count that clamped to 0x40, so the scan walked 64 slots of unrelated memory
 *      and never matched anything.
 *
 *   2. Func[] and Moby[] were SWAPPED. The scan then compared a MOBY POINTER against
 *      the shot-draw address, which can never be equal.
 *
 * Because these are easy to get wrong by eye, check_draw_table_swap.py now DERIVES
 * every value from the instruction encoding instead of trusting these literals.
 * ==========================================================================
 */
#define FLUX_REGDRAW_COUNT   (0x0024808C)
#define FLUX_REGDRAW_FUNCS   (0x0025AB80)
#define FLUX_REGDRAW_MOBYS   (0x0025AC80)

/* RegisterDrawFunction itself caps the table at 0x40, so this cannot be exceeded. */
#define FLUX_REGDRAW_MAX     (0x40)

/*
 * How many times onFluxDrawWrapper has been ENTERED.
 *
 * This exists because "the wrapper was never called" and "the wrapper was called but
 * took the no-owner path" looked IDENTICAL in the log: both showed drawn=0 and all
 * counters zero. Without this there is no way to tell whether the redirect works at
 * all, which is exactly the question that matters.
 *
 * Incremented on entry, before any early return, so a call that bails out still counts.
 */
static u32 fluxWrapperCalls = 0;

/*
 * The pre-dispatch hook.
 *
 * fluxDispatchTarget is the ORIGINAL destination of the `jal RunDrawRoutines`
 * instruction this patch replaced, decoded back out of the word it overwrote. It is
 * stored rather than tabled because it cannot then disagree with the build it was
 * installed in.
 *
 * fluxDispatchSites counts how many of the two call sites are redirecting at us, and
 * is recomputed every frame rather than latched: GetAddress is indexed by the current
 * map, so the addresses -- and therefore the answer -- legitimately change on a map
 * load.
 */
static u32 fluxDispatchTarget = 0;
static int fluxDispatchSites = 0;

/*
 * How many of the two call sites were REFUSED. A refusal is silent by design (the
 * installer must not write where its preconditions do not hold), so it is counted
 * here and reported from the update phase.
 */
static int fluxDispatchRefused = 0;

typedef struct {
	Moby *moby;                    /* the moby the callback was registered for */
	void (*callback)(Moby*);       /* the engine's own draw callback for it     */
} FluxRegEntry_t;

static FluxRegEntry_t fluxRegTable[FLUX_REG_MAX];

static FluxRegEntry_t *fluxRegSlot(Moby *moby)
{
	return &fluxRegTable[((u32)moby >> 4) & (FLUX_REG_MAX - 1)];
}

/*
 * The real draw callback for a moby we wrapped, or 0 if we did not wrap it.
 */
static void (*fluxRegLookup(Moby *moby))(Moby*)
{
	FluxRegEntry_t *e;

	if (moby == 0)
		return 0;

	e = fluxRegSlot(moby);
	if (e->moby != moby)
		return 0;

	return e->callback;
}

#if FLUX_INSTALL_DRAW_HOOK

/*
 * set while a remote shot's colours are in the immediates. Used to make the restore
 * cheap and idempotent: nothing else in the patch touches these four instructions,
 * so only the hook ever sets it and only the hook ever clears it.
 */
static int fluxColorSwapped = 0;

/*
 * Per-frame snapshot of everything the DRAW phase needs to know, taken in the UPDATE
 * phase.
 *
 * The pre-dispatch hook and the wrapper both run in the draw phase, and the two values
 * they need -- the map scope and the resolved addresses -- are exactly the two that
 * must not be recomputed there. GetAddress resolves through __LocalGetAddress, which is
 * indexed by the CURRENT map, and an extra engine call in the draw phase is the one
 * thing this hook has already been shown to be sensitive to (see the DRAW PHASE
 * MUST NOT PRINT note below).
 *
 * Caching them costs three update-phase reads and removes every lookup from the hook.
 */
static int fluxScopeCached = 0;
static u32 fluxShotDrawCached = 0;
static u32 fluxBeamAnchorCached = 0;

/*
 * ---------------------------------------------------------------------------
 * THE DRAW PHASE MUST NOT PRINT.
 *
 * Everything below runs either from the engine's draw callback or immediately before
 * the draw-table dispatch, and a version of this hook that logged from there produced,
 * every single time:
 *
 *     FLUXDRAW[local] call=1 .. call=12
 *     [EE] Impossible block clearing failure
 *     TLB Miss, pc=0x408630 addr=0x8 [store]
 *     Trap exception
 *
 * Twelve lines, then dead -- and every one of them was `local`, so no colour had been
 * applied and nothing had been swapped. The only engine work the hook did was call
 * printf. Three earlier runs showed the same thing, every trap landing in the engine's
 * printf (0x00128c70) immediately after a line from the hook.
 *
 * So the draw path performs NO logging and NO map lookup: everything it needs is
 * resolved in the update phase and cached above, and the feature's one remaining log
 * line is emitted from main(), which runs in the update phase.
 * ---------------------------------------------------------------------------
 */

/*
 * NAME :		fluxApplyOwnerColors
 * DESCRIPTION :
 * 			Writes a shot owner's colours into the beam/glow immediates.
 * NOTES :
 *          Returns 1 when it wrote, 0 when the owner is local or unknown -- in which
 *          case the configured colours are already correct and nothing is touched.
 *          A local shot MUST be skipped rather than written with the local preset,
 *          because the two are normally the same and rewriting them every draw would
 *          be pure waste.
 *
 *          The four write sites are patched instructions in live engine code; see
 *          applyFluxBeamColor for the offsets and why they are safe to rewrite.
 * ARGS :
 *          anchor: address of the beam colour immediates
 *          owner:  the player who fired the shot
 * RETURN :		1 if the immediates were changed
 * AUTHOR :			Philip762
 */
static int fluxApplyOwnerColors(u32 anchor, Player *owner)
{
	u32 beam;
	u32 glow;
	int slot;

	if (anchor == 0 || owner == 0 || owner->isLocal)
		return 0;

	slot = fluxPlayerIndexFromPtr(owner);
	if (slot < 0)
		return 0;

	fluxColorResolve(fluxPlayerBeam[slot], fluxPlayerGlow[slot], &beam, &glow);
	applyFluxBeamColor(anchor, beam);
	applyFluxGlowColor(anchor, glow);
	fluxColorSwapped = 1;

	return 1;
}

/*
 * NAME :		fluxRestoreLocalIfSwapped
 * DESCRIPTION :
 * 			Puts the configured colours back once a remote shot has drawn.
 * NOTES :
 *          MUST run after the original draw call, not before: the renderers that sit
 *          on these immediates are jal'd from inside the original, so the colours have
 *          to still be in place while it runs.
 *
 *          The anchor comes from the update-phase cache rather than a fresh
 *          GetAddress. That is not an optimisation: GetAddress resolves through
 *          __LocalGetAddress, which is indexed by the CURRENT map, and this runs in the
 *          DRAW phase. Re-resolving the map here is engine work in the one context the
 *          hook has been shown to be fragile in, and the value cannot have changed since
 *          main() ran earlier in the same frame.
 * ARGS :
 * RETURN :
 * AUTHOR :			Philip762
 */
static void fluxRestoreLocalIfSwapped(void)
{
	u32 anchor;
	u32 beam;
	u32 glow;

	if (!fluxColorSwapped)
		return;

	fluxColorSwapped = 0;

	anchor = fluxBeamAnchorCached;
	if (anchor == 0)
		return;

	fluxColorResolve(config.fluxShotColor, config.fluxGlowColor, &beam, &glow);
	applyFluxBeamColor(anchor, beam);
	applyFluxGlowColor(anchor, glow);
}

/*
 * ============================ WHY NOT PATCH THE DRAW FUNCTION ============
 * An earlier version of this patch hooked the shot draw function by overwriting
 * its entry instruction with HOOK_JAL. On Bakisi that CRASHED:
 *
 *   FLUXHOOK install map=40 anchor=0x00409310 shotDraw=0x00408D40 \
 *                 was 0x27BDFFF0 -> 0x000D4540
 *   FLUXHOOK installed, entry now 0x0C035150 (expect jal)
 *   ... 3.5 seconds later ...
 *   R5900 Exception: Jump to unmapped recLUT page (PC: 0x7efc0180)
 *
 * Every address in that run was verified correct against ntsc.40.Bakisi.bin AND
 * against Ghidra: anchor 0x00409310, shotDraw 0x00408D40, the JAL encoding, and all
 * eight colour immediates. The hook function itself was never entered -- there is no
 * "drawn N time(s)" and no FLUXDRAW line anywhere in the run.
 *
 * The reason is visible in the engine (Ghidra, FUN_00408cc8):
 *
 *     jal   0x00456108            ; RegisterDrawFunction(callback, moby)
 *      addiu a0, a0, -0x72c0      ; delay slot builds 0x00408D40
 *
 * and RegisterDrawFunction stores that POINTER into a global table. So the engine
 * reaches the draw code by calling through a saved pointer, and rewriting the first
 * instruction of the target breaks the recompiler's block dispatch for that address.
 *
 * Therefore the draw function's code is NOT touched. Instead the REGISTRATION call
 * is intercepted: when the Flux shot's callback is registered, this patch registers
 * a wrapper in its place and remembers the real callback per moby. The draw code is
 * left byte-identical to the retail game.
 * ========================================================================
 */


/*
 * installed by main() inside the FLUX_INSTALL_DRAW_HOOK block, and only there */
static void onFluxShotDraw(Moby *moby, void (*original)(Moby*));

/*
 * NAME :		onFluxShotDraw
 * DESCRIPTION :
 * 			Replacement for the Flux shot's draw callback: draws each shot with the
 *          colours of the player who fired it.
 * NOTES :
 *          Ordering is the whole trick, and it is not interchangeable:
 *
 *            1. read the owner from the shot's pVar
 *            2. write THAT owner's colours into the beam/glow immediates
 *            3. call the original, which draws the shot
 *            4. put the local player's colours back
 *
 *          Step 2 must precede step 3 because the beam and glow renderers are jal'd
 *          from INSIDE the original, so the immediates have to already hold the right
 *          values when it runs. Step 4 must follow it for the same reason in reverse:
 *          the next shot drawn may be local, and it must not inherit a remote player's
 *          colours.
 *
 *          A local shot is deliberately left alone (fluxApplyOwnerColors returns 0 for
 *          it) -- the configured colours are what is already in the immediates, so
 *          there is nothing to write and nothing to restore.
 *
 *          The original is passed in BY THE CALLER -- the wrapper that the engine
 *          actually registered -- rather than being read from a global. That is what
 *          lets the draw function's own code stay untouched: nothing here needs the
 *          original's address, because the registration wrapper captured it.
 *
 *          Nothing here prints: the DRAW PHASE MUST NOT PRINT note below records
 *          why, and it is not a style preference.
 * ARGS :
 *          moby:     the Flux shot being drawn
 *          original: the engine's own draw callback for this moby
 * RETURN :
 * AUTHOR :			Philip762
 */
static void onFluxShotDraw(Moby *moby, void (*original)(Moby*))
{
	int swapped = 0;

	/*
	 * The engine only ever calls a draw callback with a live moby, so this is a
	 * guard rather than an expected path. It is here because this is the FIRST thing
	 * reached when the engine calls the address, and passing a null moby on to the
	 * original -- which dereferences it -- would fault.
	 */
	if (moby == 0) {
		if (original != 0)
			original(moby);
		return;
	}

	/*
	 * The owner is stored at pVar + FLUX_SHOT_OWNER_OFFSET, so pVar is validated
	 * BEFORE it is dereferenced. A shot whose pVar is not set up yet is simply drawn
	 * with whatever colours are already in the immediates.
	 */
	if (moby->pVar != 0) {
		Player *owner = *(Player**)((char*)moby->pVar + FLUX_SHOT_OWNER_OFFSET);

		/*
		 * Only a REMOTE owner needs anything done. A local shot is the common case and
		 * the configured colours are already in the immediates, so it is left alone --
		 * fluxApplyOwnerColors would reject it anyway.
		 *
		 * Nothing here prints: see the DRAW PHASE MUST NOT PRINT note below.
		 */
		if (owner != 0 && !owner->isLocal) {
			/*
			 * The anchor comes from the update-phase cache, not a fresh GetAddress.
			 * GetAddress resolves through __LocalGetAddress, which is indexed by the
			 * CURRENT map, and this is the DRAW phase -- the one context the hook has
			 * been shown to be fragile in. main() already resolved it earlier in this
			 * same frame, so the value cannot be stale.
			 *
			 * This WRITES the owner's colours into the beam/glow immediates.
			 */
			swapped = fluxApplyOwnerColors(fluxBeamAnchorCached, owner);
		}
	}

	/*
	 * Draw AFTER the colours are in place: the beam and glow renderers are jal'd from
	 * inside the original, so the immediates must already hold the right values.
	 */
	if (original != 0)
		original(moby);

	/*
	 * ...and put the LOCAL player's colours back AFTER it. Without this the next shot
	 * drawn, which may well be a local one, would inherit a remote player's colours.
	 */
	if (swapped)
		fluxRestoreLocalIfSwapped();
}

/*
 * NAME :		onFluxDrawWrapper
 * DESCRIPTION :
 * 			The callback the engine actually calls for the Flux shot moby.
 * NOTES :
 *          Looks the real callback up in the table and hands it to onFluxShotDraw
 *          together with the moby. A moby with no entry is simply left undrawn rather
 *          than called through a null pointer.
 *
 *          This lives INSIDE the hook guard because with FLUX_INSTALL_DRAW_HOOK 0 the
 *          feature is off and nothing registers it.
 * ARGS :
 *          moby: the moby being drawn
 * RETURN :
 * AUTHOR :			Philip762
 */
static void onFluxDrawWrapper(Moby *moby)
{
	void (*original)(Moby*) = fluxRegLookup(moby);

	/*
	 * Counted on ENTRY, before any early return. This is the only evidence that the
	 * engine actually dispatched to us, so it must not sit behind a branch.
	 */
	fluxWrapperCalls++;

	if (original == 0) {
		/*
		 * The moby is not in our replacement table, so we do not know its real
		 * callback. FALL BACK TO THE SHOT DRAW FUNCTION ANYWAY.
		 *
		 * Returning instead would silently draw NOTHING. That matters more than it
		 * looks: a shot that is never drawn is a moby the engine still owns, and
		 * several hundred TLB misses at a single tiny address -- pc values in
		 * 0x00408xxx and 0x00409xxx, the shot draw function's own range, repeatedly
		 * loading addr=0x23a0 -- is what a moby left undrawn looks like. Never drop
		 * the draw.
		 *
		 * There is nothing to guess: the engine's own callback for the Flux shot is
		 * the interop entry, which is exactly what a registered shot's callback is.
		 * fluxRegLookup normally supplies it; this is the safety net for a table miss
		 * (hash collision, or a registration we did not observe).
		 */
		original = (void (*)(Moby*))fluxShotDrawCached;

		if (original == 0)
			return;

		/*
		 * Remember it, so the per-moby lookup hits from the next frame on. The value
		 * comes from the update-phase cache, so this still resolves no address in the
		 * draw phase.
		 */
		{
			FluxRegEntry_t *e = fluxRegSlot(moby);

			e->moby = moby;
			e->callback = original;
		}
	}

	onFluxShotDraw(moby, original);
}

/*
 * NAME :		fluxRunDrawScan
 * DESCRIPTION :
 * 			Points every registered Flux-shot slot in the engine's draw table at
 * 			onFluxDrawWrapper.
 * NOTES :
 *          MUST NOT PRINT. It runs from the DRAW phase (see onFluxPreDrawDispatch),
 *          and printing there is the one thing that is known to crash -- see the
 *          measurements recorded in the DRAW PHASE MUST NOT PRINT note below.
 *
 *          Idempotent WITHIN a frame: once a slot holds the wrapper it no longer equals
 *          shotDraw and is skipped, so running this from both the update phase and the
 *          pre-dispatch hook cannot double-wrap a slot. It is NOT idempotent ACROSS
 *          frames, because the engine re-registers the original every frame -- which is
 *          the entire reason the pre-dispatch hook exists.
 *
 *          Everything it needs is read from the update-phase cache, so it performs no
 *          map lookup and calls into the engine not at all.
 * ARGS :
 * RETURN :
 * AUTHOR :			Philip762
 */
static void fluxRunDrawScan(void)
{
	volatile u32 *funcs;
	volatile u32 *mobys;
	u32 shotDraw = fluxShotDrawCached;
	u32 count;
	u32 i;

	if (!fluxScopeCached || shotDraw == 0)
		return;

	/*
	 * RegisterDrawFunction caps the table at 0x40, so a count past that is a read of a
	 * not-yet-initialised word (observed at map entry: raw=623728696) and walking it
	 * would touch unrelated memory.
	 */
	count = *(volatile u32*)FLUX_REGDRAW_COUNT;
	if (count > FLUX_REGDRAW_MAX)
		count = FLUX_REGDRAW_MAX;

	funcs = (volatile u32*)FLUX_REGDRAW_FUNCS;
	mobys = (volatile u32*)FLUX_REGDRAW_MOBYS;

	for (i = 0; i < count; ++i) {
		FluxRegEntry_t *e;

		if (funcs[i] != shotDraw)
			continue;

		e = fluxRegSlot((Moby*)mobys[i]);
		e->moby = (Moby*)mobys[i];
		e->callback = (void (*)(Moby*))shotDraw;

		/*
		 * Only now the actual redirect. Writing Func[i] is a DATA write: the engine's
		 * own code is untouched, and data writes cannot disturb the recompiler's
		 * translated blocks -- which is what every crash in this feature has been about.
		 */
		funcs[i] = (u32)&onFluxDrawWrapper;
	}
}

/*
 * NAME :		onFluxPreDrawDispatch
 * DESCRIPTION :
 * 			Runs immediately before the engine dispatches the draw table: swaps the
 * 			Flux shot's slots, then performs the dispatch the engine asked for.
 * NOTES :
 *          THIS FUNCTION MUST NOT PRINT. It is entered from the DRAW phase.
 *
 *          The engine calls this because its own `jal RunDrawRoutines` was replaced by
 *          `jal onFluxPreDrawDispatch` (vaFluxDrawDispatchA / B). The return address the
 *          engine pushed still points just past that call site, so returning normally
 *          resumes the caller exactly as the original call did; fluxDispatchTarget was
 *          decoded from the instruction that was overwritten, so the original dispatch
 *          still happens too.
 *
 *          WHY HERE AND NOWHERE ELSE. RegisterDrawFunction only appends, and the engine
 *          zeroes the count before re-registering each frame, so a Func[] write made
 *          from the patch's own per-frame entry (main) is overwritten before the table
 *          is drawn. This is the only point in the frame that is provably after the last
 *          registration and before the dispatch. The log that proved it:
 *
 *              FLUXHOOK   slot 2 moby=0x01B2E980 func=0x000D46C4   <- wrapper IN table
 *              FLUXHOOK drawn 0 time(s)                            <- never dispatched
 *              FLUXHOOK   ... wcalls=0
 *
 * ARGS :
 * RETURN :
 * AUTHOR :			Philip762
 */
static void onFluxPreDrawDispatch(void)
{
	void (*runDrawRoutines)(void) = (void (*)(void))fluxDispatchTarget;

	fluxRunDrawScan();

	if (runDrawRoutines != 0)
		runDrawRoutines();
}

/*
 * NAME :		fluxInstallDrawDispatchHook
 * DESCRIPTION :
 * 			Redirects the engine's two RunDrawRoutines call sites at
 * 			onFluxPreDrawDispatch.
 * NOTES :
 *          Every precondition is checked BEFORE a byte is written, and a site that fails
 *          any of them is left completely untouched and counted in fluxDispatchRefused:
 *
 *            - the word must not already be our hook (so a repeated call is a no-op and
 *              never re-encodes -- important because a wrong re-encode would be a
 *              silent, permanent corruption);
 *            - the word must BE a jal (opcode 3). A site that is not a call is not the
 *              site we think it is, and writing there would destroy an instruction;
 *            - the DELAY SLOT must be nop. HOOK_JAL replaces only the jal itself, so a
 *              site with a live delay slot would silently change what the caller does;
 *            - the decoded target must be inside EE RAM.
 *
 *          A zero address means the build is not covered (see vaFluxDrawDispatchA) and
 *          is skipped, NOT defaulted to the NTSC address.
 *
 *          This is the ONLY code the Flux feature patches. Compare with what was tried
 *          and crashed: overwriting a function's ENTRY instruction destroys its prologue
 *          and re-enters the recompiler on a block that is live. Replacing `jal X` with
 *          `jal Y` destroys nothing and keeps the block's shape.
 * ARGS :
 * RETURN :
 * AUTHOR :			Philip762
 */
static void fluxInstallDrawDispatchHook(void)
{
	u32 sites[2];
	int installed = 0;
	int refused = 0;
	u32 selfJal = ADDR2JAL((u32)&onFluxPreDrawDispatch);
	u32 i;

	sites[0] = GetAddress(&vaFluxDrawDispatchA);
	sites[1] = GetAddress(&vaFluxDrawDispatchB);

	for (i = 0; i < 2; ++i) {
		u32 site = sites[i];
		volatile u32 *slot;
		u32 insn;
		u32 target;

		if (site == 0)
			continue;                /* this build is not covered */

		slot = (volatile u32*)site;
		insn = *slot;

		if (insn == selfJal) {
			installed++;             /* already redirecting at us */
			continue;
		}

		if ((insn >> 26) != 0x03) {
			refused++;
			continue;
		}

		if (*(volatile u32*)(site + 4) != 0x00000000) {
			refused++;
			continue;
		}

		target = JAL2ADDR(insn);
		if (target < 0x00100000 || target >= 0x02000000) {
			refused++;
			continue;
		}

		fluxDispatchTarget = target;

		HOOK_JAL(site, &onFluxPreDrawDispatch);
		FlushCache(0);               /* same pair the other hooks use */
		FlushCache(2);

		installed++;
	}

	fluxDispatchSites = installed;
	fluxDispatchRefused = refused;
}

#endif /* FLUX_INSTALL_DRAW_HOOK */


/*
 * NAME :		patchFluxShotColor
 * DESCRIPTION :
 * 			Recolours the Flux Rifle's beam and its decoration band to the
 *          configured presets.
 * NOTES :
 *          Both are written from the one anchor tabled in vaFluxBeamColor, which
 *          points at the base `lui` of the beam renderer. A zero anchor means the
 *          layout did not match in this build; the stock colours are kept and
 *          nothing is written.
 *
 *          Each colour is tracked separately, so changing one does not disturb the
 *          other. The writes are idempotent, so this re-applies after a map change
 *          without needing to be told one happened.
 * ARGS :
 * RETURN :
 * AUTHOR :			Philip762
 */
void patchFluxShotColor(void)
{
	/*
	 * Index 0 must RESTORE the engine's own colour, not skip the write -- skipping
	 * would leave the last custom poke in place. The vanilla words are constant
	 * across every build (asserted by verify_flux_color.py), so they can simply be
	 * written like any other preset. They live at file scope now because the
	 * per-player path has to restore the same values.
	 */
	u32 anchor = GetAddress(&vaFluxBeamColor);
	u32 beam;
	u32 glow;
	int changedBeam;
	int changedGlow;

	if (!isInGame())
		return;

	/*
	 * The scope check comes BEFORE any write, so on a build with no verified anchor
	 * the engine's own immediates are never touched.
	 */
	if (!fluxMapInScope())
		return;

	if (anchor == 0)
		return;


	changedBeam = config.fluxShotColor != patched.fluxShotColor;
	changedGlow = config.fluxGlowColor != patched.fluxGlowColor;

	/*
	 * A preset index outside the list would index out of bounds in
	 * fluxColorResolve, so clamp the config and say so -- a bad config from the
	 * server should be visible, not silently read past the end of the array.
	 */
	if (config.fluxShotColor < 0 || config.fluxShotColor >= FLUX_COLOR_COUNT) {
		DPRINTF("FLUXWARN beam preset %d out of range 0..%d, clamping to 0\n",
		        (int)config.fluxShotColor, FLUX_COLOR_COUNT - 1);
		config.fluxShotColor = 0;
		changedBeam = 1;
		patched.fluxShotColor = -1;
	}

	if (config.fluxGlowColor < 0 || config.fluxGlowColor >= FLUX_COLOR_COUNT) {
		DPRINTF("FLUXWARN glow preset %d out of range 0..%d, clamping to 0\n",
		        (int)config.fluxGlowColor, FLUX_COLOR_COUNT - 1);
		config.fluxGlowColor = 0;
		changedGlow = 1;
		patched.fluxGlowColor = -1;
	}

	if (!changedBeam && !changedGlow)
		return;

	fluxColorResolve(config.fluxShotColor, config.fluxGlowColor, &beam, &glow);

	if (changedBeam) {
		patched.fluxShotColor = config.fluxShotColor;
		applyFluxBeamColor(anchor, beam);
	}

	if (changedGlow) {
		patched.fluxGlowColor = config.fluxGlowColor;
		applyFluxGlowColor(anchor, glow);
	}
}

/*
 * NAME :		patchControllerDeadzone
 * DESCRIPTION : Applies the configured analog stick deadzone.
 * NOTES :
 *          Analog stick input is a value from 0 to 255 with 127 being considered the center.
 *          The deadzone value is an int where everything below it is ignored.
 * 			The input itself is processed in 3 steps:
 * 				1. Compare the input value to the deadzone value. If the input is less than the deadzone value, it is ignored.
 * 				2. If the input is greater than the deadzone value, it is re-origined by subtracting the deadzone value from it.
 * 				3. The re-origined value is then divided by (127 - deadzone value) to scale it to the range of 0 to 1.0. This is done to ensure that the full range of the stick is still usable after applying the deadzone.
 * 			At each of these steps (CompareInput, OffsetInput and ScaleInput), a word is changed to adjust the deadzone.
 * ARGS :
 * RETURN :
 * AUTHOR :			Philip762
 */
void patchControllerDeadzone(void)
{
	if (!isInGame())
		return;

	int radius;
	switch (config.controllerDeadzone)
	{
		case 0: radius = 48; break;
		case 1: radius = 38; break;
		case 2: radius = 25; break;
		case 3: radius = 19; break;
		case 4: radius = 13; break;
		case 5: radius = 10; break;
		case 6: radius = 6; break;
		case 7: radius = 4; break;
		case 8: radius = 3; break;
		case 9: radius = 1; break;
		case 10: radius = 0; break;
		default: radius = 48; break;
	}

	u32 compareAddress = GetAddress(&vaPadDeadzone_CompareInput);
	u32 offsetAddress = GetAddress(&vaPadDeadzone_OffsetInput);
	u32 scaleAddress = GetAddress(&vaPadDeadzone_ScaleInput);

	// The divisor must be 127 - radius for full travel to reach the 1.0 clamp.
	float divisor = 127.0f - (float)radius;
	u32 divisorWord = 0x3C010000 | (*(u32*)&divisor >> 16);

	// The immediates are the low halfword of an I-type instruction.
	POKE_U16(compareAddress, (u16)radius);
	POKE_U16(offsetAddress, (u16)(-radius));
	POKE_U32(scaleAddress, divisorWord);
}

/*
 * NAME :		patchSideFlipJoystickVal
 * DESCRIPTION :Patches joystick offset value for checking if to side flip or not.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchSideFlipJoystickVal(void)
{
	if (!gameConfig.prDisableDlStyleFlips && patched.config.dlStyleFlips == config.dlStyleFlips)
		return;

	u16 val = config.dlStyleFlips ? 0x3f80 : 0x3f66;
	POKE_U16(GetAddress(&vaSideFlipJoystickVal), val);
	patched.config.dlStyleFlips = config.dlStyleFlips;
}

/*
 * NAME :		patchSwingshotGunBug
 * DESCRIPTION :Changes how close/far a player needs to be to a swingshot for the gadget to be taken out.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int patchSwingshotGunBug_Logic(VECTOR from, VECTOR to, int hitFlag, Moby *pMoby, int *collDamage)
{
	float offset = 0.7f;
	VECTOR fromVec, toVec, dir, offsetVec;
	vector_copy(fromVec, from);
	vector_copy(toVec, to);

	// get direction, and normalize.
	vector_subtract(dir, toVec, fromVec);
	vector_normalize(dir, dir);
	// generate offset
	vector_scale(offsetVec, dir, offset);
	// subtract offset
	vector_subtract(fromVec, fromVec, offsetVec);

	// keep original height.
	toVec[2] = fromVec[2];

	return CollLine_Fix(fromVec, toVec, hitFlag, pMoby, collDamage);
}
void patchSwingshotGunBug(void)
{
	if (patched.swingshotGunBug == 1 || patchStateContainer.CustomMapId == 0)
		return;

	HOOK_JAL(GetAddress(&vaPatchSwingshotGunBug_Hook), &patchSwingshotGunBug_Logic);
	patched.swingshotGunBug = 1;
}

/*
 * NAME :		onMobyUpdate
 * DESCRIPTION :Patches the player packets for smoother movement.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			json
 */
void onMobyUpdate(Moby* moby)
{
	if (gameConfig.grNewPlayerSync) {
		playerSyncTick();
		// KOTH already runs through the normal main-loop module pass. Skip the
		// extra NPS-side dispatch so KOTH doesn't tick twice per frame.
		if (gameConfig.customModeId != CUSTOM_MODE_KOTH)
			processGameModules();

		((void (*)(Moby*))GetAddress(&vaOnMobyUpdate_Func))(moby);

		playerSyncPostTick();
	} else {
		((void (*)(Moby*))GetAddress(&vaOnMobyUpdate_Func))(moby);	
	}
}		


/*
 * NAME :		runHolidays
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
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
			if (day >= 24  && day <= 26)
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
 * NAME :		patchColors
 * DESCRIPTION : Patches color extended table for new colors
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchColors(void)
{
	if (patched.colorsExtTable)
		return;

	COLOR_EXT_TABLE->white = 0x80ffffff;
	COLOR_EXT_TABLE->gray = 0x80808080; // Patch Gray in game due to it being black in game.
	COLOR_EXT_TABLE->black2 = 0x80e0e040; // Aqua

	patched.colorsExtTable = 1;
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
	if (uiGetActiveMenu(UI_MENU_STAGING, 0) != 0) {
		// check if game started
		if (!sentGameStart && gameSettings->GameLoadStartTime > 0) {
			// check if host
			if (gameAmIHost())
				netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_GAME_LOBBY_STARTED, 0, gameSettings);

			// request server time
			// requestServerTime();

			#ifdef SCAVENGER_HUNT
			// request latest scavenger hunt settings
      		scavHuntQueryForRemoteSettings();
			#endif

			sentGameStart = 1;
		}
	} else {
		sentGameStart = 0;
	}
}

/*
 * NAME :		patchHeadsetSprite
 * DESCRIPTION: Patches staging sprite for custom map detection
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchHeadsetSprite(GameSettings* gs, int clientId)
{
	UiMenu_t* ui = uiGetActiveMenu(UI_MENU_STAGING, 0);
	if (!ui)
		return;
	
    UiStagingElements_t* child = &ui->pChildren;
    // hide voice header
    child->voiceHeadingSprite->state = 0;
	// check state and set color
	u32 color = 0;
	u32 sprite = SPRITE_HUD_X;
	if (clientId >= 0) {
		switch (gs->PlayerStates[clientId]) {
			case GS_PLAYER_STATE_MAP_NONE: {
				sprite = SPRITE_HUD_X;
				color = 0x80000000 | TEAM_COLORS[TEAM_AQUA];
				break;
			}
			case GS_PLAYER_STATE_MAP_VERSION_OLDER: {
				sprite = SPRITE_HUD_X; //SPRITE_HUD_BASE;
				color = 0x80000000 | TEAM_COLORS[TEAM_RED];
				break;
			}
			case GS_PLAYER_STATE_MAP_VERSION_NEWER: {
				sprite = SPRITE_HUD_X; // SPRITE_HUD_BASE;
				color = 0x80000000 | TEAM_COLORS[TEAM_GREEN];
				break;
			}
		}
		// If state is not MAP_NONE, color remains 0 (transparent) - has correct version
	}
	child->voiceSprite[clientId]->sprite = sprite;
	child->voiceSprite[clientId]->vTable->setColor(child->voiceSprite[clientId], color);
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
	int noCustomMAp = patchStateContainer.CustomMapId == 0;
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
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		int isMe = gs->PlayerClients[i] == clientId;
		if (isMe && i > 0) {
			switch (gs->PlayerStates[i]) {
				case GS_PLAYER_STATE_UNREADY: {
					// Player doesn't have map.
					if (mapOverrideResponse < 0 && mapOverrideResponse != -3) {
						gameSetClientState(i, GS_PLAYER_STATE_MAP_NONE);
						netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_REQUEST_MAP_OVERRIDE, 0, NULL);	
					}
					// Player has map, but wrong version.
					if (mapOverrideResponse >= 0 && expectedMapVersion >= 0 && mapOverrideResponse != expectedMapVersion) {
						if (mapOverrideResponse > expectedMapVersion) {
							gameSetClientState(i, GS_PLAYER_STATE_MAP_VERSION_NEWER);
						} else if (mapOverrideResponse < expectedMapVersion) {
							gameSetClientState(i, GS_PLAYER_STATE_MAP_VERSION_OLDER);
						}
						netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_REQUEST_MAP_OVERRIDE, 0, NULL);	
					}
					break;
				}
				case GS_PLAYER_STATE_MAP_NONE:
				case GS_PLAYER_STATE_MAP_VERSION_OLDER:
				case GS_PLAYER_STATE_MAP_VERSION_NEWER: {
					if (mapOverrideResponse >= 0 || mapOverrideResponse == -3 || mapOverrideResponse == expectedMapVersion) {
						gameSetClientState(i, GS_PLAYER_STATE_UNREADY);
						netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_REQUEST_MAP_OVERRIDE, 0, NULL);	
					}
					break;
				}
				case GS_PLAYER_STATE_KICK: showNoMapPopup = 0; break;
				case GS_PLAYER_STATE_READY: {
					// Player doesn't have map.
					if (mapOverrideResponse < 0 && mapOverrideResponse != -3) {
						gameSetClientState(i, GS_PLAYER_STATE_MAP_NONE);
						showNoMapPopup = 1;
						netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_REQUEST_MAP_OVERRIDE, 0, NULL);	
					}
					// Player has map, but wrong version.
					if (mapOverrideResponse >= 0 && expectedMapVersion >= 0 && mapOverrideResponse != expectedMapVersion) {
						if (mapOverrideResponse > expectedMapVersion) {
							gameSetClientState(i, GS_PLAYER_STATE_MAP_VERSION_NEWER);
						} else if (mapOverrideResponse < expectedMapVersion) {
							gameSetClientState(i, GS_PLAYER_STATE_MAP_VERSION_OLDER);
						}
						netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_REQUEST_MAP_OVERRIDE, 0, NULL);	
					}
					break;
				}
			}
		}
		patchHeadsetSprite(gs, i);
	}
}

bool setPlayerHudTexture(HANDLE_ID container, SpriteTex_Hud_e texture) {
	// 0x50013 maps to current player's icon, 14 the 2nd player, 15 the 3rd, and so on
	int playerNum = container - 0x50013;
	Player ** players = playerGetAll();
	Player* player = players[playerNum]; 
	SpriteTex_Hud_e tex = texture;
	if (player->flagMoby) {
		tex = SPRITE_HUD_FLAG;
	}
	return hudSetSprite(container, tex);
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
		// memset(patchStateContainer.GameStateUpdate.Nodes, 0, sizeof(patchStateContainer.GameStateUpdate.Nodes));

		if (gameSettings->GameType == GAMETYPE_SIEGE) {	
			for (i = 0; i < 8; ++i) {
				if (gameData->allYourBaseGameData->nodeTeam[i] == 0)
					++patchStateContainer.GameStateUpdate.Nodes[0];
				else if (gameData->allYourBaseGameData->nodeTeam[i] == 1)
					++patchStateContainer.GameStateUpdate.Nodes[1];
			}
			patchStateContainer.GameStateUpdate.TeamScores[0] = gameData->allYourBaseGameData->hudHealth[0];
			patchStateContainer.GameStateUpdate.TeamScores[1] = gameData->allYourBaseGameData->hudHealth[1];
		} else if (gameSettings->GameType == GAMETYPE_CTF) {
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
		} else if (gameSettings->GameType == GAMETYPE_DM) {
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

	if (uiGetActiveMenu(UI_MENU_ONLINE_AGREEMENT, 0) != 0) return;
	if (uiGetActiveMenu(UI_MENU_ANNOUNCEMENTS, 0) != 0) return;

	lastMenuInvokedTime = gameGetTime();
	if (!hasInitialized) {
		padEnableInput();
		onConfigInitialize();
		// error location for loading maps: 000d45dc
		// refreshCustomMapList();
		memset(&voteToEndState, 0, sizeof(voteToEndState));
		hasInitialized = 1;
	}
	if (hasInitialized == 1) {
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
	#ifdef SCAVENGER_HUNT
	if (!scavHuntShownPopup && !config.disableScavengerHunt && scavHuntEnabled){
		uiShowOkDialog("Scavenger Hunt", "The Horizon Scavenger Hunt is live! Hunt for Horizon Bolts for a chance to win prizes! Join our discord for more info: discord.gg/horizonps");
    	scavHuntShownPopup = 1;
	}
	#endif
  //
  #if DSCRPRINT
  float y = 10;
  for (i = 0; i < MAX_DEBUG_SCR_PRINT_LINES; ++i) {
    gfxScreenSpaceText(10, y, 1, 1, 0x80FFFFFF, dscrprintlines[i], -1, 0, FONT_BOLD);
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
	// Call this first
	uyaPreUpdate();

	//
	#if DSCRPRINT
	int i;
	float y = 10;
	for (i = 0; i < MAX_DEBUG_SCR_PRINT_LINES; ++i) {
		gfxScreenSpaceText(10, y, 1, 1, 0x80FFFFFF, dscrprintlines[i], -1, 0, FONT_BOLD);
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
  	// netInstallCustomMsgHandler(CUSTOM_MSG_ID_CLIENT_RESPONSE_DATE_SETTINGS, &onServerTimeResponse);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_PLAYER_FLUX_COLORS, &onFluxColorsRemote);
	/*
	 * Redirect the Flux shot's registered draw callback -- by editing the ENGINE'S
	 * DRAW TABLE, and by redirecting the single call that dispatches that table.
	 *
	 * THE WHOLE DESIGN IN ONE PARAGRAPH
	 * ---------------------------------
	 * RegisterDrawFunction stores the shot's callback POINTER into a plain data array,
	 * and the scan below rewrites those entries to point at onFluxDrawWrapper -- so the
	 * per-shot colour switch happens inside the engine's own draw callback, with the
	 * moby as the only argument. The scan, however, writes too EARLY on its own: the
	 * engine zeroes the table count at the start of every frame and re-registers, so the
	 * per-frame call below is undone before the table is drawn. That is why the hook on
	 * the dispatch call sites (vaFluxDrawDispatchA/B) exists, and why the scan is run
	 * from there as well.
	 *
	 * The evidence that pinned it down -- the wrapper demonstrably IN the table, and
	 * demonstrably never called:
	 *
	 *     FLUXHOOK table raw=3 count=3 swaps=1 wcalls=0
	 *     FLUXHOOK   slot 2 moby=0x01B2E980 func=0x000D46C4     <- our wrapper
	 *     FLUXHOOK drawn 0 time(s)
	 *
	 * WHY NOT HOOK RegisterDrawFunction
	 * ---------------------------------
	 * That was tried and it crashed. RegisterDrawFunction (0x00456108 on Bakisi) has
	 * THIRTY-SIX call sites all over the engine, and hooking its entry with HOOK_JAL
	 * produced, 166 ms after the second install and while the level was still loading:
	 *
	 *     R5900 Exception: Jump to unaligned address (PC: 0x000000fe)
	 *
	 * 0x000000fe is a jump through a near-NULL pointer, not a real EE address. Any
	 * code patch on a routine reached from 36 places is a large risk surface, and the
	 * previous attempt to patch the DRAW function failed the same way.
	 *
	 * WHY THE DISPATCH HOOK IS SAFE
	 * -----------------------------
	 * RegisterDrawFunction stores the callback POINTER into a plain data array:
	 *
	 *     lui  a2,0x25 ; lw   a2,-0x7f74(a2)   RegisteredDrawsCount  @ 0x0024808C
	 *     lui  a0,0x26 ; addiu a0,a0,-0x5480   RegisteredDrawRoutines_Func  @ 0x0025AB80
	 *     lui  v1,0x26 ; addiu v1,v1,-0x5380   RegisteredDrawRoutines_Moby  @ 0x0025AC80
	 *     sw   a3,0x0(v0)                      Func[i] = callback
	 *     sw   t0,0x0(v1)                      Moby[i] = moby
	 *
	 * so the redirect itself is a DATA write, and data writes cannot disturb the
	 * recompiler's translated blocks -- which is what every crash so far has been about.
	 *
	 * The table is only ever APPENDED to (RegisteredDrawsCount increments), so the scan
	 * stops at the count and never reads uninitialised slots.
	 *
	 * The one code patch is `jal RunDrawRoutines` -> `jal onFluxPreDrawDispatch`, at the
	 * only two call sites the binary has, each with a nop delay slot. It replaces a call
	 * with a call: no prologue is destroyed and no delay slot is reinterpreted. See
	 * fluxInstallDrawDispatchHook, which validates the word it is about to overwrite
	 * before writing it.
	 */
#if FLUX_INSTALL_DRAW_HOOK
	{
		/*
		 * Refresh the draw-phase cache and install the pre-dispatch hook, in that
		 * order.
		 *
		 * The hook resolves nothing itself. GetAddress indexes __LocalGetAddress by the
		 * CURRENT map, and the hook runs in the DRAW phase, so the scope and the two
		 * addresses are resolved here, once per frame, and the hook only reads them.
		 *
		 * Installing must also come after the cache is refreshed: patching while the
		 * map id is stale would write at one build's addresses before we know they are
		 * that build's.
		 */
		fluxScopeCached = fluxMapInScope();
		fluxShotDrawCached = fluxScopeCached ? GetAddress(&vaFluxShotDrawFunc) : 0;
		fluxBeamAnchorCached = fluxScopeCached ? GetAddress(&vaFluxBeamColor) : 0;

		fluxInstallDrawDispatchHook();

		/*
		 * The scan also runs from here, where it is too early to survive to the
		 * dispatch -- that is the bug the pre-dispatch hook exists to fix. It is kept
		 * because it costs nothing, it is idempotent within a frame, and it still
		 * redirects the table on a build whose dispatch call sites are not tabled. Such
		 * a build then degrades to "colours apply to the local player only" instead of
		 * to a patch that does nothing at all.
		 */
		fluxRunDrawScan();
	}
#endif
#ifdef DEBUG
#if FLUX_INSTALL_DRAW_HOOK
	/*
	 * The feature's only logging: one line per map change, and one every 5s.
	 *
	 * Everything else that used to live here has been removed -- the per-frame table
	 * dump with its per-slot listing, the draw-event ring and all six of its counters,
	 * the FLUXDIAG state dump, and the one-shot table probe. Each answered a question
	 * that is now settled, and the ring sat in the DRAW phase, which is the one context
	 * this feature has repeatedly crashed in. Removing it takes work out of the hot
	 * path rather than just noise out of the log.
	 *
	 * Wrapped in #ifdef DEBUG TOGETHER WITH ITS LOCALS. DPRINTF compiles to nothing
	 * when DEBUG is undefined, so a release build would otherwise be left with locals
	 * that nothing reads -- and this build promotes unused variables to errors. Keeping
	 * the guard outside the declarations is what makes the whole block vanish.
	 *
	 * The three numbers are the ones that actually distinguish a failure:
	 *
	 *   sites   how many RunDrawRoutines call sites were redirected. 2 on a covered
	 *           build. 0 means the interop entry resolved to zero, i.e. the map scope
	 *           picked the wrong build's table; refused>0 means a site was found but did
	 *           not look like `jal X` + a nop delay slot, so it was left untouched.
	 *   run     the address decoded out of the instruction that was overwritten, i.e.
	 *           what the hook calls to still dispatch the table.
	 *   wrapper how many times the shot draw callback was dispatched to us. This is the
	 *           ONLY evidence the redirect works, and it read 0 for several runs while
	 *           the wrapper sat in the table -- which is exactly why it is reported.
	 */
	{
		static int fluxScopeLogged = -1;
		static u32 fluxStatusLogged = 0;
		int mapId = gameGetCurrentMapId();
		u32 now = gameGetTime();

		if (mapId != fluxScopeLogged) {
			fluxScopeLogged = mapId;
			DPRINTF("FLUXMAP now id=%d %s\n", mapId,
			        fluxMapInScope() ? "(anchor tabled, colour active)"
			                         : "(no tabled anchor, feature idle)");
		}

		if ((now - fluxStatusLogged) >= 5000) {
			fluxStatusLogged = now;
			DPRINTF("FLUXHOOK dispatch sites=%d refused=%d run=0x%08X "
			        "wrapper=%u\n",
			        fluxDispatchSites, fluxDispatchRefused,
			        fluxDispatchTarget, fluxWrapperCalls);
		}
	}
#endif
#endif

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

	// Run Ping.  (onRemote doesn't respond :( )
	// runPing();

	#ifdef SCAVENGER_HUNT
	// Run Scavenger Hunt
	scavHuntRun();
	#endif

	// Run Send Gameupdate for Helga
	patchStateContainer.UpdateGameState = runSendGameUpdate();

	// 
	runCameraSpeedPatch();

	// 
	onConfigUpdate();

	// 
	sendClientType();

	// find and hook multiplayer moby to hook
	static Moby* mpMoby = NULL;
	if (!isInGame()) {
		mpMoby = NULL;
	} else if (!mpMoby) {
		mpMoby = mobyFindNextByOClass(mobyListGetStart(), 0x106A);
		if (mpMoby) {
			DPRINTF("mpmoby hooked!: %08x\n", mpMoby);
			mpMoby->pUpdate = &onMobyUpdate;
		}
	}
	// else if (isUnloading && mpMoby) {
	// 	DPRINTF("mpmoby unhooked from unloading\n!");
	// 	mpMoby->pUpdate = NULL;
	// } // dont understand this well enough to keep

	// Adds Single Player Music to Multiplayer
	if (config.enableSingleplayerMusic)
		runCampaignMusic();

	// 
	runVoteToEndLogic();

	// Holiday easter eggs!
	// runHolidays();

	patchColors();

	if(isInGame()) {
		// Patch remap buttons configuration
		HOOK_JAL(0x0013cae0, &patchSceReadPad_memcpy);

		// fix weird overflow caused by player sync
		// also randomly (rarely) triggered by other things too
		POKE_U32(GetAddress(&vaPlayerSyncFixOverflow1), 0x00622023); // unique address only in uya
		POKE_U32(GetAddress(&vaPlayerSyncFixOverflow2), 0x00412023); // collline_fix 004b8078 in DL
		POKE_U32(GetAddress(&vaPlayerSyncFixOverflow3), 0x00612023); // collline_fix 004b8084 in DL
		POKE_U32(GetAddress(&vaPlayerSyncFixOverflow4), 0x00622023);  //collline_fix  0x004b80a0 in DL

		POKE_U32(GetAddress(&vaPlayerSyncFixLagout1), 0x24020000); // fix lagout when pnetPlayer lastRecievedPacket becomes too old

		POKE_U32(GetAddress(&vaPlayerSyncFixTurretDelay), 0); // fix turret delays when net time is out fo sync

		POKE_U32(GetAddress(&vaWaitingForResponse_Addr), 0x24020001); // patch out artificial "waiting for response" lag out

		HOOK_JAL(GetAddress(&vaGameTimeUpdate_Hook), GetAddress(&vaNWUpdate_Func)); // poll nwupdate instead of updatepad for consistent game time

		// replace player arrow with flag sprite on map when someone is holding flag
		HOOK_JAL(GetAddress(&vaSetTextureArrow_Hook), &setPlayerHudTexture);

		// Patch Dead Jumping/Crouching
		patchDeadJumping();

		// Patch Emulator Lag
		patchGadgetEvents();

		// Patch Dead Shooting
		patchDeadShooting();

		// Run Game Rules if in game.
		grGameStart();

		// Patch Flux Niking
		patchSniperNiking();

		// Patch Flux Wall Sniping
		patchSniperWallSniping();

		// Patch bug if too close to swingshot, weaepons don't appear/shoot.
		// patchSwingshotGunBug();

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
		patchCTFFlag();

		// Patch Level of Detail
		patchLevelOfDetail();

		// Patch Analog Stick Deadzone
		patchControllerDeadzone();

		// Patch Weapon Ordering when Respawning
		patchResurrectWeaponOrdering();

		// Runs FPS Counter
		runFpsCounter();

		// Run Spectate
		// if (config.enableSpectate)
		// 	runSpectate();

		if (config.alwaysShowHealth)
			patchAlwaysShowHealth();

		if (config.enableTeamInfo)
			teamInfo();

		// Patches the Map and Scoreboard for player toggalability!
		patchMapAndScoreboardToggle();

		if (config.aimAssist)
			patchCameraPull();

		// Patch hiding of Flux Reticle
		patchHideFluxReticle();

		/*
		 * Publish our Flux colours, and backstop the restore of the local colours if
		 * a remote shot swapped them. Set PATCH_FLUX_COLOR_SYNC to 0 to disable the
		 * whole per-player Flux feature (send, receive and rendering).
		 */
#define PATCH_FLUX_COLOR_SYNC 1
#if PATCH_FLUX_COLOR_SYNC
		patchFluxColorSync();
#endif

		// Patch Flux Shot Colour
		patchFluxShotColor();

		if (config.hypershotEquipButton)
			hypershotEquipButton();

		// Patch Side Flipping joystick offset value
		// patchSideFlipJoystickVal();

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
		// If in Lobby, run these game rules.
		grLobbyStart();

		// Patches loading popup from not showing if patch menu is open.
		patchLoadingPopup();

		playerSyncTick();
		
		// Patch Menus (Staging, create game, ect.)
		if (patched.uiModifiers == 0) {
			// POKE_U32(UI_PTR_FUNC_CREATE_GAME, &patchCreateGame);
			// POKE_U32(UI_PTR_FUNC_ADVANCED_OPTIONS, &patchAdvancedOptions);
			POKE_U32(UI_PTR_FUNC_STAGING, &patchStaging);
			// POKE_U32(UI_PTR_FUNC_BUDDIES, &patchBuddies);
			// POKE_U32(UI_PTR_FUNC_PLAYER_DETAILS, &patchPlayerDetails);
			// POKE_U32(UI_PTR_FUNC_STATS, &patchStats);
			POKE_U32(UI_PTR_FUNC_KEYBOARD, &patchKeyboard);
			POKE_U32(STAGING_JALR_HEADSET_SET_COLOR, 0);
			patched.uiModifiers = 1;
		}

		// Patch Rank Table
		if (!patched.rankTable) {
			// Sets deviation rank higher than that of player deviation, this way it goes by rank, not deviation.
			int rangeMultiplier = 1;
			*(float*)RANK_TABLE = 1000000000.00; // Deviation 1 (275.0)
			*(float*)(RANK_TABLE + 0x4) = 0; // Deviation 2 (250.0)
			*(float*)(RANK_TABLE + 0x8) = 0; // Deviation 3 (100.0)
			*(float*)(RANK_TABLE + 0xc) = 1000 * rangeMultiplier; // Rank Rage 1 (0.0 - 1000.0 - 1349.0) (2 bolts)
			*(float*)(RANK_TABLE + 0x10) = 1300 * rangeMultiplier; // Rank Rage 2 (1350.0 - 1699.0) (3 bolts)
			*(float*)(RANK_TABLE + 0x14) = 1700 * rangeMultiplier; // Rank Range 3 (1700.0 and Above) (4 bolts)
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
				// Reset patchStateContainer.CustomMapId to none
				patchStateContainer.CustomMapId = 0;

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
