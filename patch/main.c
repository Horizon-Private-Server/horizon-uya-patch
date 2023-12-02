/***************************************************
 * FILENAME :		main.c
 * 
 * DESCRIPTION :
 * 		Manages and applies all UYA patches.
 * 
 * NOTES :
 * 		Each offset is determined per game region.
 * 		This is to ensure compatibility between versions of UYA PAL/NTSC.
 * 		
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
#include "module.h"
#include "messageid.h"
#include "config.h"
#include "include/interop.h"
#include "include/config.h"
#include "include/cheats.h"

#define GLOBAL_GAME_MODULES_START							((GameModule*)0x000CF000)
#define EXCEPTION_DISPLAY_ADDR								(0x000C8000)
#if UYA_PAL
#define STAGING_START_BUTTON_STATE							(*(short*)0x006c2d80)
#else
#define STAGING_START_BUTTON_STATE							(*(short*)0x006C0268)
#endif

void onConfigOnlineMenu(void);
void onConfigGameMenu(void);
void onConfigUpdate(void);
void configMenuEnable(void);
void configMenuDisable(void);

void runMapLoader(void);
void onMapLoaderOnlineMenu(void);

void grGameStart(void);
void grLobbyStart(void);
void grLoadStart(void);

void runSpectate(void);

int dlBytesReceived = 0;
int dlTotalBytes = 0;
int hasInitialized = 0;
int lastMenuInvokedTime = 0;
int lastGameState = 0;
int sentGameStart = 0;
int isInStaging = 0;
int hasInstalledExceptionHandler = 0;
char mapOverrideResponse = 1;
char showNoMapPopup = 0;
char weaponOrderBackup[2][3] = { {0,0,0}, {0,0,0} };
u32 currentUI = 0;
u32 previousUI = 0;
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
int fovChange_Hook = 0;
int fovChange_Func = 0;
int lastSettings = 0;
int lastNodes = 0;

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
};

PatchGameConfig_t gameConfig;
PatchGameConfig_t gameConfigHostBackup;

struct FlagPVars
{
	VECTOR BasePosition;
	short CarrierIdx;
	short LastCarrierIdx;
	short Team;
	char UNK_16[6];
	int TimeFlagDropped;
};

//------------------------------------------------------------------------------
int onServerDownloadDataRequest(void * connection, void * data)
{
	ServerDownloadDataRequest_t* request = (ServerDownloadDataRequest_t*)data;

	// copy bytes to target
	dlTotalBytes = request->TotalSize;
	dlBytesReceived += request->DataSize;
	memcpy((void*)request->TargetAddress, request->Data, request->DataSize);
	printf("DOWNLOAD: %d/%d, writing %d to %08X\n", dlBytesReceived, request->TotalSize, request->DataSize, request->TargetAddress);
  
	// respond
	if (connection)
	{
		ClientDownloadDataResponse_t response;
		response.Id = request->Id;
		response.BytesReceived = dlBytesReceived;
		netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_DOWNLOAD_DATA_RESPONSE, sizeof(ClientDownloadDataResponse_t), &response);
	}

  // reset at end
  if (dlBytesReceived >= request->TotalSize)
  {
    dlTotalBytes = 0;
    dlBytesReceived = 0;
  }

	return sizeof(ServerDownloadDataRequest_t) - sizeof(request->Data) + request->DataSize;
}

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
  if (hasMACAddress)
  {
    memcpy(output, macAddress, 6);
    return 1;
  }

  // sceInetInterfaceControl get physical address
  int r = netInterfaceControl(cd, net_buf, 1, 13, buf, sizeof(buf));
  if (r)
  {
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
int runCheckUI(void)
{
	if (isInMenus())
	{
		u32 MostCurrentUI = *(u32*)0x01C5C110;
		if (currentUI == 0 && previousUI == 0)
		{
			currentUI = MostCurrentUI;
			return 1;
		}
		else if (currentUI != MostCurrentUI)
		{
			previousUI = currentUI;
			currentUI = MostCurrentUI;
			return 1;
		}
	}
	return 0;
}
char * checkGameType(void)
{
	if (isInMenus() || isInGame())
	{
		GameSettings * gs = gameGetSettings();
		if (gs)
		{
			switch(gs->GameType)
			{
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
	if (isInMenus())
	{
		if (isInStaging)
		{
			return "Staging";
		}
		else
		{
			return "Online Lobby";
		}
	}
	else if (isInGame())
	{
		return mapGetName(gameGetCurrentMapId());
	}
	else
	{
		return "Loading Screen";
	}
}
void runExceptionHandler(void)
{
	// invoke exception display installer
	if (*(u32*)EXCEPTION_DISPLAY_ADDR != 0)
	{
		if (!hasInstalledExceptionHandler)
		{
			((void (*)(void))EXCEPTION_DISPLAY_ADDR)();
			hasInstalledExceptionHandler = 1;
		}
		
		// change "a fatal error as occured." to region and map.
		char * mapStr = checkMap();
		strncpy((char*)(EXCEPTION_DISPLAY_ADDR + 0x794), regionStr, 6);
		strncpy((char*)(EXCEPTION_DISPLAY_ADDR + 0x79a), mapStr, 20);
		
		// change display to match progressive scan resolution
		if (gfxGetIsProgressiveScan())
		{
			*(u16*)(EXCEPTION_DISPLAY_ADDR + 0x9F4) = 0x0083;
			*(u16*)(EXCEPTION_DISPLAY_ADDR + 0x9F8) = 0x210E;
		}
		else
		{
			*(u16*)(EXCEPTION_DISPLAY_ADDR + 0x9F4) = 0x0183;
			*(u16*)(EXCEPTION_DISPLAY_ADDR + 0x9F8) = 0x2278;
		}
	}
}

/*
 * NAME :		runCameraSpeedPatch
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
void runCameraSpeedPatch(void)
{
	char buf[12];
	const short MAX_CAMERA_SPEED = 300;
	if (isInMenus())
	{
		// overwrite menu camera controls max cam speed
		// also display speed text next to input
		u32 ui = uiGetPointer(UIP_CONTROLS);
		if (ui && uiGetPointer(UIP_EDIT_PROFILE) == uiGetActivePointerSlot(0))
		{
			u32 cameraRotationUIPtr = *(u32*)(ui + 0x11C);
			if (cameraRotationUIPtr)
			{
				// max speed
				*(u32*)(cameraRotationUIPtr + 0x7C) = MAX_CAMERA_SPEED;

				// draw %
				if (uiGetPointer(UIP_CONTROLS) == uiGetActivePointerSlot(1))
				{
					sprintf(buf, "%d%%", *(u32*)(cameraRotationUIPtr + 0x80));
					gfxScreenSpaceText(340, 310, 1, 1, 0x8069cbf2, buf, -1, 2);
				}
			}
		}

	}
	else if (isInGame())
	{
		// overwrite in game camera speed control max

		// replaces limiter function so that input can go past default 100
		u32 updateCameraSpeedIGFunc = GetAddress(&vaUpdateCameraSpeedIGFunc);
		if (updateCameraSpeedIGFunc) {
			*(u16*)(updateCameraSpeedIGFunc + 0x130) = MAX_CAMERA_SPEED;
			*(u16*)(updateCameraSpeedIGFunc + 0x154) = MAX_CAMERA_SPEED+1;
		}

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
		// draw percentage if in start menu
		u32 img = gfxGetPreLoadedImageBufferSource(0);
		int p = playerGetFromSlot(0)->PauseOn;
		if (p && *(int*)(img + 0xc) == 5) {
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
 * 
 * DESCRIPTION :
 * 			Filters out hits when player is already dead.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * 
 * DESCRIPTION :
 * 			Patches who hit me on weapon hit with patchKillStealing_Hook.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchKillStealing(void)
{
	int the_hook = GetAddress(&vaWhoHitMeHook);
	int the_patch = 0x0C000000 | ((u32)&patchKillStealing_Hook >> 2);
	// static int the_original_jal;
	if (*(u32*)the_hook != the_patch)
	{
		// the_original_jal = ConvertJALtoAddress(*(u32*)the_hook);
		*(u32*)the_hook = the_patch;
	}
}

/*
 * NAME :		patchDeathJumping
 * 
 * DESCRIPTION :
 * 			Patches Dead Jumping by setting the can't move timer.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchDeadJumping(void)
{
	Player ** players = playerGetAll();
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
    	if (!players[i])
    		continue;

		Player * player = players[i];
		if (playerIsLocal(player) && playerIsDead(player))
		{
			// get current player state
			int PlayerState = playerDeobfuscate(&player->State, 0, 0);
			// if player is on bolt crank, set player state to idle.
			if (PlayerState == PLAYER_STATE_BOLT_CRANK)
				playerSetPlayerState(player, PLAYER_STATE_IDLE);

			player->timers.noInput = 10;
		}
	}
}

/*
 * NAME :		patchDeadShooting_Hook
 * 
 * DESCRIPTION :
 * 			If player is dead, don't let them shoot.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * 
 * DESCRIPTION :
 * 			Patches the shooting hook with patchDeadShooting_Hook
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchDeadShooting(void)
{
	if (*(u32*)GetAddress(&vaPatchDeadShooting_ShootingHook) != 0x0C000000 | ((u32)&patchDeadShooting_Hook >> 2))
		*(u32*)GetAddress(&vaPatchDeadShooting_ShootingHook) = 0x0C000000 | ((u32)&patchDeadShooting_Hook >> 2);
}

int patchSniperWallSniping_Hook(VECTOR from, VECTOR to, Moby* shotMoby, Moby* moby, u64 t0)
{
	// hit through terrain
	// if we've hit a target
	// we check if we've hit by reading the source guber event
	// which is passed in 0x5C of the shot's pvars
	if (shotMoby && shotMoby->PVar) {
    int shotOwner = *(u32*)((u32)shotMoby + 0x90) >> 28;
    if (shotOwner != gameGetMyClientId()) {
      void * event = *(void**)(shotMoby->PVar + 0x5C);
      if (event) {
        u32 hitGuberUid = *(u32*)(event + 0x3C);
        if (hitGuberUid != 0xFFFFFFFF) {
          return collLine_Fix(from, to, 1, moby, t0);
        }
      }
    }
	}

	// pass through
	return collLine_Fix(from, to, 0, moby, t0);
}

/*
 * NAME :		patchSniperWallSniping
 * 
 * DESCRIPTION :
 * 			Send Weapon Shots more reliably.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchSniperWallSniping(void)
{
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

void patchSniperNiking_Hook(float f12, VECTOR out, VECTOR in, void * event)
{
	// call base
	u32 getShotDirectionFunction = GetAddress(&vaGetSniperShotDirectionFunc);
	if (getShotDirectionFunction) {
		((void (*)(float, VECTOR, VECTOR))getShotDirectionFunction)(f12 * 0.01666666666, out, in);
	}

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
					vector_subtract(out, hitMoby->Position, (float*)event);
					out[2] += 0.5;
					return;
				}
			}
		}
	}
}

/*
 * NAME :		patchSniperNiking
 * 
 * DESCRIPTION :
 * 			Send Weapon Shots more reliably.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * 
 * DESCRIPTION :
 * 			Send Weapon Shots more reliably.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchWeaponShotLag(void)
{
	int TCP = 0x24040040;

	// Send all weapon shots reliably (Use TCP instead of UDP)
	// int AllWeaponsAddr = GetAddress(&vaAllWeaponsUDPtoTCP);
	// if (*(u32*)AllWeaponsAddr == 0x906407D4)
	// 	*(u32*)AllWeaponsAddr = TCP;

	// Send Flux shots reliably (Use TCP instead of UDP)
	int FluxAddr = GetAddress(&vaFluxUDPtoTCP);
	if (*(u32*)FluxAddr == 0x90A407D4)
		*(u32*)FluxAddr = TCP;
}

/*
 * NAME :		handleGadgetEvent
 * 
 * DESCRIPTION :
 * 			Reads gadget events and patches them if needed.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void handleGadgetEvents(int message, char GadgetEventType, int ActiveTime, short GadgetId, int t0, int StackPointer)
{
	int GEF = GetAddress(&vaGadgetEventFunc);
	Player * player = (Player*)((u32)message - 0x1a40);
	struct tNW_GadgetEventMessage * msg = (struct tNW_GadgetEventMessage*)message;
	// GadgetEventType 7 = Niked, or splash damage.
	if (msg && GadgetEventType == 7)
	{
		if(GadgetId == 3)
		{
			GadgetEventType = 8;
		}
	}
	// GadgetEventType 8 = Hit Something
	// else if (msg && msg->GadgetEventType == 8)
	// {
	// 	int delta = ActiveTime - gameGetTime();
	// 	// Make player hold correct weapon.
	// 	if (player->WeaponHeldId != msg->GadgetId)
	// 	{
	// 		playerEquipWeapon(player, msg->GadgetId);
	// 	}
	// 	// Set weapon shot event time to now if its in the future
	// 	if (player->WeaponHeldId == msg->GadgetId && (delta > 0 || delta < -TIME_SECOND))
	// 	{
	// 		ActiveTime = gameGetTime();
	// 	}
	// }
	// run base command
	((void (*)(int, char, int, short, int, int))GEF)(message, GadgetEventType, ActiveTime, GadgetId, t0, StackPointer);
}

/*
 * NAME :		patchGadgetEvents
 * 
 * DESCRIPTION :
 * 			Hook for the handleGadgetEVentLag function.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchGadgetEvents(void)
{
	HOOK_JAL(GetAddress(&vaGadgetEventHook), &handleGadgetEvents);
}

/*
 * NAME :		patchLevelOfDetail
 * 
 * DESCRIPTION :
 * 			Sets the level of detail.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchLevelOfDetail(void)
{
	if (*(u32*)GetAddress(&vaLevelOfDetail_Hook) == 0x02C3B020) {
		HOOK_J(GetAddress(&vaLevelOfDetail_Hook), &_correctTieLod);
		// patch jump instruction in correctTieLod to jump back to needed address.
		u32 val = ((u32)GetAddress(&vaLevelOfDetail_Hook) + 0x8);
		*(u32*)(&_correctTieLod_Jump) = 0x08000000 | (val / 4);
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
				*(float*)GetAddress(&vaLevelOfDetail_Shrubs) = ShrubDistance;
				*(u32*)GetAddress(&vaLevelOfDetail_Ties) = TerrainTiesDistance;
				*(float*)GetAddress(&vaLevelOfDetail_Terrain) = TerrainTiesDistance * 1024;
			}
			break;
		}
		case 1: // Low
				{
			_lodScale = 0.4;
			int TerrainTiesDistance = 480;
			int ShrubDistance = 250;
			if (lodChanged) {
				*(float*)GetAddress(&vaLevelOfDetail_Shrubs) = ShrubDistance;
				*(u32*)GetAddress(&vaLevelOfDetail_Ties) = TerrainTiesDistance;
				*(float*)GetAddress(&vaLevelOfDetail_Terrain) = TerrainTiesDistance * 1024;
			}
			break;
		}
		case 2: // Normal
		{
			_lodScale = 1.0;
			int TerrainTiesDistance = 960;
			int ShrubDistance = 500;
			if (lodChanged) {
				*(float*)GetAddress(&vaLevelOfDetail_Shrubs) = ShrubDistance;
				*(u32*)GetAddress(&vaLevelOfDetail_Ties) = TerrainTiesDistance;
				*(float*)GetAddress(&vaLevelOfDetail_Terrain) = TerrainTiesDistance * 1024;
			}

			break;
		}
		case 3: // High
		{
			_lodScale = 5.0;
			int TerrainTiesDistance = 4800;
			int ShrubDistance = 2500;
			if (lodChanged) {
				*(float*)GetAddress(&vaLevelOfDetail_Shrubs) = ShrubDistance;
				*(u32*)GetAddress(&vaLevelOfDetail_Ties) = TerrainTiesDistance;
				*(float*)GetAddress(&vaLevelOfDetail_Terrain) = TerrainTiesDistance * 1024;
			}
			break;
		}
	}
	// backup lod
	lastLodLevel = config.levelOfDetail;
}

/*
 * NAME :		patchResurrectWeaponOrdering_HookWeaponStripMe
 * 
 * DESCRIPTION :
 * 			Invoked during the resurrection process, when the game wishes to remove all weapons from the given player.
 * 			Before we continue to remove the player's weapons, we backup the list of equipped weapons.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchResurrectWeaponOrdering_HookWeaponStripMe(Player * player)
{
	// backup currently equipped weapons
	if (player->IsLocal) {
		int LocalPlayerIndex = player->fps.Vars.cam_slot;
		weaponOrderBackup[LocalPlayerIndex][0] = playerDeobfuscate(&player->QuickSelect.Slot[0], 1, 1);
		weaponOrderBackup[LocalPlayerIndex][1] = playerDeobfuscate(&player->QuickSelect.Slot[1], 1, 1);
		weaponOrderBackup[LocalPlayerIndex][2] = playerDeobfuscate(&player->QuickSelect.Slot[2], 1, 1);
	}

	// call hooked WeaponStripMe function after backup
	playerStripWeapons(player);
}

/*
 * NAME :		patchResurrectWeaponOrdering_HookGiveMeRandomWeapons
 * 
 * DESCRIPTION :
 * 			Invoked during the resurrection process, when the game wishes to give the given player a random set of weapons.
 * 			After the weapons are randomly assigned to the player, we check to see if the given weapons are the same as the last equipped weapon backup.
 * 			If they contain the same list of weapons (regardless of order), then we force the order of the new set of weapons to match the backup.
 * 
 * 			Consider the scenario:
 * 				Player dies with 								Fusion, B6, Magma Cannon
 * 				Player is assigned 							B6, Fusion, Magma Cannon
 * 				Player resurrects with  				Fusion, B6, Magma Cannon
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void patchResurrectWeaponOrdering_HookGiveMeRandomWeapons(Player* player, int weaponCount)
{
	int i, j, matchCount = 0;

	// call hooked GiveMeRandomWeapons function first
	playerGiveRandomWeapons(player, weaponCount);

	// then try and overwrite given weapon order if weapons match equipped weapons before death
	if (player->IsLocal) {
		int LocalPlayerIndex = player->fps.Vars.cam_slot;
		// restore backup if they match (regardless of order) newly assigned weapons
		for (i = 0; i < 3; i++) {
			u8 backedUpSlotValue = weaponOrderBackup[LocalPlayerIndex][i];
			for(j = 0; j < 3; j++) {
				if (backedUpSlotValue == playerDeobfuscate(&player->QuickSelect.Slot[j], 1, 1)) {
					matchCount++;
				}
			}
		}
		// if we found a match, set
		if (matchCount == 3) {
			// set equipped weapon in order
			for (i = 0; i < 3; ++i) {
				playerGiveWeapon(player, weaponOrderBackup[LocalPlayerIndex][i]);
			}

			// equip each weapon from last slot to first slot to keep correct order.
			playerEquipWeapon(player, weaponOrderBackup[LocalPlayerIndex][2]);
			playerEquipWeapon(player, weaponOrderBackup[LocalPlayerIndex][1]);
			playerEquipWeapon(player, weaponOrderBackup[LocalPlayerIndex][0]);
		}
	}
}

/*
 * NAME :		patchResurrectWeaponOrdering
 * 
 * DESCRIPTION :
 * 			Installs necessary hooks such that when respawning with same weapons,
 * 			they are equipped in the same order.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchResurrectWeaponOrdering(void)
{
	u32 hook_StripMe = ((u32)GetAddress(&vaPlayerRespawnFunc) + 0x40);
	u32 hook_RandomWeapons = hook_StripMe + 0x1c;
	HOOK_JAL(hook_StripMe, &patchResurrectWeaponOrdering_HookWeaponStripMe);
	HOOK_JAL(hook_RandomWeapons, &patchResurrectWeaponOrdering_HookGiveMeRandomWeapons);
}

/*
 * NAME :		runFpsCounter_Logic
 * 
 * DESCRIPTION :
 * 			Logic for the FPS counter and drawing the text to screen.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * 
 * DESCRIPTION :
 * 			Logic for the GS counter.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
	if ((t1 - ticksIntervalStarted) > (SYSTEM_TIME_TICKS_PER_MS * 500))
	{
		averageRenderTimeMs = renderTimeCounterMs / (float)frames;
		renderTimeCounterMs = 0;
		frames = 0;
		ticksIntervalStarted = t1;
	}
}

/*
 * NAME :		runFpsCounter_updateHook
 * 
 * DESCRIPTION :
 * 			Logic for the EE counter.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
	if ((t1 - ticksIntervalStarted) > (SYSTEM_TIME_TICKS_PER_MS * 500))
	{
		averageUpdateTimeMs = updateTimeCounterMs / (float)frames;
		updateTimeCounterMs = 0;
		frames = 0;
		ticksIntervalStarted = t1;
	}
}

/*
 * NAME :		runFpsCounter
 * 
 * DESCRIPTION :
 * 			Hooks functions for showing the EE, GS and FPS.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void runFpsCounter(void)
{
	int hook = GetAddress(&vaFpsCounter_Hooks);
	int update = GetAddress(&vaFpsCounter_UpdateFunc);
	int draw = GetAddress(&vaFpsCounter_DrawFunc);
	HOOK_JAL(hook, &runFpsCounter_updateHook);
	HOOK_JAL(((u32)hook + 0x60), &runFpsCounter_drawHook);
	runFpsCounter_Logic();
}

/*
 * NAME :		writeFov
 * 
 * DESCRIPTION :
 * 			Replaces game's SetFov function. Hook installed by patchFov().
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void writeFov(int cameraIdx, int a1, int a2, u32 ra, float fov, float f13, float f14, float f15)
{
	static float lastFov = 0;

	GameCamera* camera = cameraGetGameCamera(cameraIdx);
	if (!camera)
		return;

	// save last fov
	// or reuse last if fov passed is 0
	if (fov > 0)
		lastFov = fov;
	else if (lastFov > 0)
		fov = lastFov;
	else
		fov = lastFov = camera->fov.ideal;

	// apply our fov modifier
	// only if not scoping with sniper
	int FluxRA = GetAddress(&vaFieldOfView_FluxRA);
	if (ra != FluxRA && ra != ((u32)FluxRA + 0xe0))
		fov += (config.playerFov / 10.0) * 1;

	if (a2 > 2) {
		if (a2 != 3) return;
		camera->fov.limit = f15;
		camera->fov.changeType = a2;
		camera->fov.ideal = fov;
		camera->fov.state = 1;
		camera->fov.gain = f13;
		camera->fov.damp = f14;
		return;
	}
	else if (a2 < 1) {
		if (a2 != 0) return;
		camera->fov.ideal = fov;
		camera->fov.changeType = 0;
		camera->fov.state = 1;
		return;
	}

	if (a1 == 0) {
		camera->fov.ideal = fov;
		camera->fov.changeType = 0;
	}
	else {
		camera->fov.changeType = a2;
		camera->fov.init = camera->fov.actual;
		camera->fov.timer = (short)a2;
		camera->fov.timerInv = 1.0 / (float)a2;
	}
	camera->fov.state = 1;
}

/*
 * NAME :		fovChange
 * 
 * DESCRIPTION :
 * 			Rewrites the FOV (via SetPosRot) when player dies.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void fovChange(u32 a0)
{
	// run base
	((void (*)(u32))GetAddress(&vaSetPOSRot_fovChange_Func))(a0);

	writeFov(0, 0, 3, 0, 0, 0.05, 0.2, 0);
}

/*
 * NAME :		patchFov
 * 
 * DESCRIPTION :
 * 			Installs SetFov override hook.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void patchFov(void)
{
	static int ingame = 0;
	static int lastFov = 0;
	if (!isInGame()) {
		ingame = 0;
		fovChange_Hook = 0;
		fovChange_Func = 0;
		return;
	}

	// replace SetFov function
	HOOK_J(GetAddress(&vaFieldOfView_Hook), &writeFov);
	POKE_U32((u32)GetAddress(&vaFieldOfView_Hook) + 0x4, 0x03E0382d);

	// modify SetPosRot Func. (Needed when player dies)
	HOOK_JAL(GetAddress(&vaSetPOSRot_fovChange_Hook), &fovChange);

	// initialize fov at start of game
	if (!ingame || lastFov != config.playerFov) {
		GameCamera* camera = cameraGetGameCamera(0);
		if (!camera)
			return;

		writeFov(0, 0, 3, 0, 0, 0.05, 0.2, 0);
		lastFov = config.playerFov;
		ingame = 1;
	}
}

/*
 * NAME :		patchUnkick_Logic
 * 
 * DESCRIPTION :
 * 				Logic behind fixingi the unkick exploit.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * 
 * DESCRIPTION :
 * 				Hook for the patchUnkick_Logic.
 * 				Hooks into the popup loop.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * 
 * DESCRIPTION :
 * 				Patches death barrier bug/teleport glitch
 * 				that let players fall off the map into the base.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchDeathBarrierBug(void)
{
	int i;
	Player *player = playerGetFromSlot(0);
	// if player is local
	if (player && playerIsLocal(player)) {
		float deathbarrier = gameGetDeathHeight();
		float pY = player->PlayerPosition[2];
		//DPRINTF("deathheight: %d\nplayery: %d\ninbasehack: %d\n", (int)deathbarrier, (int)pY, player->InBaseHack);
		// if player is above death barrier and inBaseHack equals 1.
		if (player->InBaseHack && deathbarrier < pY) {
			player->InBaseHack = 0;
		} else if (!player->InBaseHack && deathbarrier > pY) {
			player->InBaseHack = 1;
		}
	}
}

/*
 * NAME :		patchCreateGameMenu
 * 
 * DESCRIPTION :
 * 				Patches various options on the create game screen
 * 				such as higher frag limit and time limit.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchCreateGameMenu_Option(int option, int new_value)
{
	if (*(int*)option != new_value)
		*(int*)option = new_value;
}
void patchCreateGameMenu_LastSettings(void)
{
	u32 menu = uiGetPointer(UIP_CREATE_GAME);
	// Normal Options
	// Enable Password if Disabled
	int use_password = (*(u32*)(menu + 0x130) + 0x4);
	int password = (*(u32*)(menu + 0x134) + 0x4);
	// If "USE PASWORD" is "Yes" and also disabled to edit
	if (*(u32*)use_password == 2) {
		patchCreateGameMenu_Option(use_password, 3); // enable "use password"
		patchCreateGameMenu_Option(password, 3); // enable password option
	}

	// Advanced Options
	menu = uiGetPointer(UIP_CREATE_GAME_ADVANCED_OPTIONS);
	int nodes = (*(u32*)(menu + 0x114) + 0x64);
	patchCreateGameMenu_Option(nodes, lastNodes);

	lastSettings = 0;
}
void patchCreateGameMenu(void)
{
	u32 menu = uiGetPointer(UIP_CREATE_GAME);
	if (menu != uiGetActivePointerSlot(0))
		return;

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
	menu = uiGetPointer(UIP_CREATE_GAME_ADVANCED_OPTIONS);
	if (menu != uiGetActivePointerSlot(1))
		return;

	int frag_limit = (*(u32*)(menu + 0x12c) + 0x70);
	patchCreateGameMenu_Option(frag_limit, 50); // Frag Limit = 50
}

/*
 * NAME :		patchAlwaysShowHealth
 * 
 * DESCRIPTION :
 * 				Always shows the players health bar.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchAlwaysShowHealth(void)
{
	Player *player = (Player *)PLAYER_STRUCT;
	u32 healthbar_timer = GetAddress(&vaHealthBarTimerSaveZero);
	u32 old_value = 0xae002514; // sw zero,0x2514(s0)
	if (config.alwaysShowHealth && *(u32*)healthbar_timer == old_value) {
		*(u32*)healthbar_timer = 0;
		player->HudHealthTimer = 3 * GAME_FPS;
	} else if (!config.alwaysShowHealth && *(u32*)healthbar_timer == 0) {
		*(u32*)healthbar_timer = old_value;
	}
}

/*
 * NAME :		patchMapAndScoreboardToggle
 * 
 * DESCRIPTION :
 * 				Lets the player choose how they want to open the
 * 				Scoreboard or Map (via Select, L3 or R3)
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * 
 * DESCRIPTION :
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void flagHandlePickup(Moby* flagMoby, int pIdx)
{
	Player** players = playerGetAll();
	Player* player = players[pIdx];
	if (!player || !flagMoby)
		return;
	
	struct FlagPVars* pvars = (struct FlagPVars*)flagMoby->PVar;
	if (!pvars)
		return;

	// fi flag state isn't 1
	if (flagMoby->State != 1)
		return;

	// flag is currently returning
	if (flagIsReturning(flagMoby))
		return;

	// flag is currently being picked up
	if (flagIsBeingPickedUp(flagMoby))
		return;

	// only allow actions by living players
	if (playerIsDead(player) || playerGetHealth(player) <= 0)
		return;

	// Handle pickup/return
	if (player->mpTeam == pvars->Team) {
		flagReturnToBase(flagMoby, 0, pIdx);
	} else {
		flagPickup(flagMoby, pIdx);
		player->FlagMoby = flagMoby;
	}
	DPRINTF("player %d picked up flag %X at %d\n", player->PlayerId, flagMoby->OClass, gameGetTime());
}

/*
 * NAME :		flagRequestPickup
 * 
 * DESCRIPTION :
 * 			Requests to either pickup or return the given flag.
 * 			If host, this request is automatically passed to the handler.
 * 			If not host, this request is sent over the net to the host.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void flagRequestPickup(Moby* flagMoby, int pIdx)
{
	static int requestCounters[GAME_MAX_PLAYERS] = {0,0,0,0,0,0,0,0,0,0};
	Player** players = playerGetAll();
	Player* player = players[pIdx];
	if (!player || !flagMoby)
		return;
	struct FlagPVars* pvars = (struct FlagPVars*)flagMoby->PVar;
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
 * 
 * DESCRIPTION :
 * 			Reimplements flag pickup logic but runs through our host authoritative logic.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void customFlagLogic(Moby* flagMoby)
{
	VECTOR t;
	int i;
	Player** players = playerGetAll();
	int gameTime = gameGetTime();
	GameOptions* gameOptions = gameGetOptions();

	// if not in game
	if (!isInGame())
		return;

	// if flagMoby doesn't exist
	if (!flagMoby)
		return;

	// if flag pvars don't exist
	struct FlagPVars* pvars = (struct FlagPVars*)flagMoby->PVar;
	if (!pvars)
		return;

	// if flag state doesn't equal 1
	if (flagMoby->State != 1)
		return;

	// if flag is returning
	if (flagIsReturning(flagMoby))
		return;

	// if flag is being picked up
	if (flagIsBeingPickedUp(flagMoby))
		return;

	// return to base if flag has been idle for 40 seconds
	if ((pvars->TimeFlagDropped + (TIME_SECOND * 40)) < gameTime && !flagIsAtBase(flagMoby)) {
		flagReturnToBase(flagMoby, 0, 0xFF);
		return;
	}
	
	// if flag didn't land on safe ground, return flag.
	if(gameConfig.grFlagHotspots) {
		if (!flagIsOnSafeGround(flagMoby) && !flagIsAtBase(flagMoby)) {
			flagReturnToBase(flagMoby, 0, 0xff);
			return;
		}
	}

	// wait 1.5 seconds for last carrier to be able to pick up again
	if (((pvars->TimeFlagDropped + 1500) > gameTime))
		return;

	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		Player* player = players[i];
		if (!player || !player->IsLocal)
			continue;

		// only allow actions by living players
		if (playerIsDead(player) || playerGetHealth(player) <= 0)
			continue;

		// skip player if they've only been alive for < 3 seconds
		if (player->timers.timeAlive <= 180)
			continue;

		// skip player if in vehicle
		if (player->Vehicle && playerDeobfuscate(&player->State, 0, 0) == PLAYER_STATE_VEHICLE)
			continue;

		// skip if player state is in vehicle and critterMode is on
		if (player->Camera && player->Camera->camHeroData.critterMode)
			continue;

		// skip if player is on teleport pad
		if (player->ground.pMoby && player->ground.pMoby->OClass == MOBY_ID_TELEPORT_PAD)
			continue;

		// player must be within 2 units of flag
		vector_subtract(t, flagMoby->Position, player->PlayerPosition);
		float sqrDistance = vector_sqrmag(t);
		if (sqrDistance > (2*2))
			continue;

		// player is on different team than flag and player isn't already holding flag
		if (player->mpTeam != pvars->Team) {
			if (!player->FlagMoby) {
				flagRequestPickup(flagMoby, i);
				return;
			}
		} else {
			// if player is on same team as flag and close enough to return it
			vector_subtract(t, pvars->BasePosition, flagMoby->Position);
			float sqrDistanceToBase = vector_sqrmag(t);
			if (sqrDistanceToBase > 0.1) {
				flagRequestPickup(flagMoby, i);
				return;
			}
		}
	}
}

/*
 * NAME :		onRemoteClientRequestPickUpFlag
 * 
 * DESCRIPTION :
 * 			Handles when a remote client sends the CUSTOM_MSG_ID_FLAG_REQUEST_PICKUP message.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
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
 * 
 * DESCRIPTION :
 * 			Patch CTF Flag update function with our own
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void patchCTFFlag(void)
{
	VECTOR t;
	int i = 0;
	Player** players = playerGetAll();

	if (!isInGame())
		return;

	netInstallCustomMsgHandler(CUSTOM_MSG_ID_FLAG_REQUEST_PICKUP, &onRemoteClientRequestPickUpFlag);

	u32 FlagFunc = GetAddress(&vaFlagUpdate_Func);
	if (FlagFunc){
		*(u32*)FlagFunc = 0x03e00008;
		*(u32*)(FlagFunc + 0x4) = 0x0000102D;
	}

	GuberMoby* gm = guberMobyGetFirst();
	while (gm) {
		if (gm->Moby) {
			switch (gm->Moby->OClass) {
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
 * NAME :		runGameStartMessager
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
void runGameStartMessager(void)
{
	GameSettings * gameSettings = gameGetSettings();
	if (!gameSettings)
		return;

	// in staging
	if (uiGetPointer(UIP_STAGING) == uiGetActivePointerSlot(0))
	{
		// check if game started
		if (!sentGameStart && gameSettings->GameLoadStartTime > 0)
		{
			// check if host
			if (gameAmIHost())
			{
				netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_GAME_LOBBY_STARTED, 0, gameSettings);
			}
			sentGameStart = 1;
		}
	}
	else
	{
		sentGameStart = 0;
	}
}

/*
 * NAME :		runCheckGameMapInstalled
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
	if (mapOverrideResponse < 0)
	{
		for (i = 1; i < GAME_MAX_PLAYERS; ++i)
		{
			if (gs->PlayerClients[i] == clientId && gs->PlayerStates[i] == 6)
			{
		#if UYA_PAL
				((void (*)(u32, u32, u32))0x006c4308)(uiGetPointer(UIP_STAGING), 5, 0);
		#else
				((void (*)(u32, u32, u32))0x006c17f0)(uiGetPointer(UIP_STAGING), 5, 0);
		#endif
				gs->PlayerStates[i] = 0; // unready up
				showNoMapPopup = 1;
				netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_REQUEST_MAP_OVERRIDE, 0, NULL);
			}
			else if (gs->PlayerClients[i] == clientId && gs->PlayerStates[i] == 5)
			{
				showNoMapPopup = 0;
			}
		}
	}
}

/*
 * NAME :		setupPatchConfigInGame
 * 
 * DESCRIPTION :
 * 				Changes the "CONTINUE" start option to "PATCH MENU"
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void setupPatchConfigInGame(void)
{
    // Get Menu address via current map.
    u32 Addr = GetAddress(&vaPauseMenuAddr);
	u32 ConfigEnableFunc = 0x0C000000 | ((u32)&configMenuEnable >> 2);
	if (*(u32*)(Addr + 0x8) != ConfigEnableFunc) {
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
	}
}

/*
 * NAME :		drawFunction
 * 
 * DESCRIPTION :
 * 			Calls the normal draw function.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void drawFunction(void)
{
#ifdef UYA_PAL
    ((void (*)(void))0x0067C9C0)();
#else
	((void (*)(void))0x00679f08)();
#endif
}

/*
 * NAME :		processGameModules
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
void processGameModules()
{
	// Start at the first game module
	GameModule * module = GLOBAL_GAME_MODULES_START;

	// Game settings
	GameSettings * gamesettings = gameGetSettings();

	// Iterate through all the game modules until we hit an empty one
	while (module->GameEntrypoint || module->LobbyEntrypoint)
	{
		// Ensure we have game settings
		if (gamesettings)
		{
			// Check the module is enabled
			if (module->State > GAMEMODULE_OFF)
			{
				// If in game, run game entrypoint
				if (isInGame())
				{
					// Invoke module
					if (module->GameEntrypoint)
						module->GameEntrypoint(module, &config, &gameConfig);
				}
				else if (isInMenus())
				{
					// Invoke lobby module if still active
					if (module->LobbyEntrypoint)
					{
						module->LobbyEntrypoint(module, &config, &gameConfig);
					}
				}
			}
		}
		// If we aren't in a game then try to turn the module off
		// ONLY if it's temporarily enabled
		else if (module->State == GAMEMODULE_TEMP_ON)
		{
			module->State = GAMEMODULE_OFF;
		}
		else if (module->State == GAMEMODULE_ALWAYS_ON)
		{
			// Invoke lobby module if still active
			if (isInMenus() && module->LobbyEntrypoint)
			{
				module->LobbyEntrypoint(module, &config, &gameConfig);
			}
		}

		++module;
	}
}

/*
 * NAME :		onOnlineMenu
 * 
 * DESCRIPTION :
 * 			Called every ui update.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void onOnlineMenu(void)
{
  int i;

	// call normal draw routine
	drawFunction();
	
	lastMenuInvokedTime = gameGetTime();
	if (!hasInitialized)
	{
		// printf("onOnlinemenu - pad enable input\n");
		padEnableInput();
		onConfigInitialize();
		refreshCustomMapList();
		hasInitialized = 1;
	}
	if (hasInitialized == 1 && uiGetPointer(UIP_ONLINE_LOBBY) == uiGetActivePointerSlot(0))
	{
		uiShowOkDialog("System", "Patch has been successfully loaded.");
		hasInitialized = 2;
	}
  
	// map loader
	onMapLoaderOnlineMenu();

	// settings
	onConfigOnlineMenu();

	// draw download data box
	if (dlTotalBytes > 0)
	{
		gfxScreenSpaceBox(0.2, 0.35, 0.6, 0.125, 0x8004223f);
		gfxScreenSpaceBox(0.2, 0.45, 0.6, 0.05, 0x80123251);
		gfxScreenSpaceText(SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.4, 1, 1, 0x8069cbf2, "Downloading...", 11 + (gameGetTime()/240 % 4), 3);

		float w = (float)dlBytesReceived / (float)dlTotalBytes;
		gfxScreenSpaceBox(0.2, 0.45, 0.6 * w, 0.05, 0x8018608f);
	}

	// 
	if (showNoMapPopup)
	{
		if (mapOverrideResponse == -1)
		{
			uiShowOkDialog("Custom Maps", "You have not installed the map modules.");
		}
		else
		{
			char buf[32];
			sprintf(buf, "Please install %s to play.", MapLoaderState.MapName);
			uiShowOkDialog("Custom Maps", buf);
		}
		
		showNoMapPopup = 0;
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
 * 
 * DESCRIPTION :
 * 			Applies all patches and modules.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int main(void)
{
	// Call this first
	uyaPreUpdate();

	// auto enable pad input to prevent freezing when popup shows
	if (isInMenus() && lastMenuInvokedTime > 0 && (gameGetTime() - lastMenuInvokedTime) > TIME_SECOND)
	{
		padEnableInput();
		lastMenuInvokedTime = 0;
	}

	//
  	netInstallCustomMsgHandler(CUSTOM_MSG_ID_SERVER_DOWNLOAD_DATA_REQUEST, &onServerDownloadDataRequest);

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

	// 
	runCameraSpeedPatch();

	// Patches FOV to let it be user selectable.
	patchFov();

	// 
	onConfigUpdate();

	// 
	sendMACAddress();

	if(isInGame())
	{
		// Run Game Rules if in game.
		grGameStart();

		// Patch Flux Niking
		if (gameConfig.grFluxNikingDisabled)
			patchSniperNiking();

		// Patch Flux Wall Sniping
		if (gameConfig.grFluxShotsAlwaysHit)
			patchSniperWallSniping();

		// Patch Dead Jumping/Crouching
		patchDeadJumping();

		// Patch Dead Shooting
		patchDeadShooting();

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

		// Patch Weapon Ordering when Respawning
		patchResurrectWeaponOrdering();

		// Runs FPS Counter
		runFpsCounter();

		// Run Spectate
		if (config.enableSpectate)
			runSpectate();

		// Patches gadget events as they come in.
		// patchGadgetEvents();

		if (config.alwaysShowHealth)
			patchAlwaysShowHealth();

		// Patches the Map and Scoreboard for player toggalability!
		patchMapAndScoreboardToggle();

		// close config menu on transition to lobby
		if (lastGameState != 1)
			configMenuDisable();

		// Updates Start Menu to have "Patch Config" option.
		// Logic for opening menu as well.
		setupPatchConfigInGame();

		// trigger config menu update
		onConfigGameMenu();

		lastGameState = 1;
	}
	else if (isInMenus())
	{
		// Patch various options on Create Game Screen
		patchCreateGameMenu();

		// If in Lobby, run these game rules.
		grLobbyStart();

		// Patch Unkick Bug
		patchUnkick();

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
		if (gameSettings && gameSettings->GameLoadStartTime < 0)
		{
			// if host and just entered staging, send patch game config
			if (gameAmIHost() && !isInStaging)
			{
				// copy over last game config as host
				memcpy(&gameConfig, &gameConfigHostBackup, sizeof(PatchGameConfig_t));
				// Reset SelectedCustomMapId to none
				SelectedCustomMapId = 0;

				// send
				configTrySendGameConfig();
			}

			// if (gameAmIHost()) {
			// 	// set lastNodes option
			// 	#if UYA_PAL
			// 		int getNodesOption = *(u8*)0x0024164f;
			// 	#else
			// 		int getNodesOption = *(u8*)0x002417cf;
			// 	#endif
			// 	 // printf("\nlastsettings: %d", lastSettings);
			// 	lastNodes = (int)getNodesOption;

			// 	// options set, so set lastSettings to 1.
			// 	lastSettings = 1;
			// }

			isInStaging = 1;
		}
		else
		{
			isInStaging = 0;
		}
	}

	// process modules
	processGameModules();

	// Call this last
	uyaPostUpdate();

	return 0;
}
