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
#include "module.h"
#include "messageid.h"
#include "config.h"
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
const char * patchStr = "PATCH CONFIG";
#if UYA_PAL
const char * regionStr = "PAL:  ";
#else
const char * regionStr = "NTSC: ";
#endif
int lastLodLevel = 2;

extern float _lodScale;
extern void* _correctTieLod;
extern int _correctTieLod_Jump;
extern VariableAddress_t vaPlayerRespawnFunc;
extern MenuElem_ListData_t dataCustomMaps;

PatchConfig_t config __attribute__((section(".config"))) = {
	.enableAutoMaps = 0,
	.disableCameraShake = 0,
	.levelOfDetail = 2,
};

PatchGameConfig_t gameConfig;
PatchGameConfig_t gameConfigHostBackup;

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
	const short MAX_CAMERA_SPEED = 200;

#ifdef UYA_PAL
	VariableAddress_t vaUpdateCameraSpeedIGFunc = {
		.Lobby = 0,
		.Bakisi = 0x004be190,
		.Hoven = 0x004c02a8,
		.OutpostX12 = 0x004b5b80,
    	.KorgonOutpost = 0x004b3318,
		.Metropolis = 0x004b2668,
		.BlackwaterCity = 0x004aff00,
		.CommandCenter = 0x004afef8,
    	.BlackwaterDocks = 0x004b2778,
    	.AquatosSewers = 0x004b1a78,
    	.MarcadiaPalace = 0x004b13f8,
	};
	VariableAddress_t vaDrawCameraSpeedInputIGFunc = {
		.Lobby = 0,
		.Bakisi = 0x004be3a4,
		.Hoven = 0x004c04bc,
		.OutpostX12 = 0x004b5d94,
    	.KorgonOutpost = 0x004b352c,
		.Metropolis = 0x004b287c,
		.BlackwaterCity = 0x004b0114,
		.CommandCenter = 0x004b010c,
    	.BlackwaterDocks = 0x004b298c,
    	.AquatosSewers = 0x004b1c8c,
    	.MarcadiaPalace = 0x004b160c,
	};
#else
	VariableAddress_t vaUpdateCameraSpeedIGFunc = {
		.Lobby = 0,
		.Bakisi = 0x004BBC78,
		.Hoven = 0x004BDCD0,
		.OutpostX12 = 0x004B35E8,
    	.KorgonOutpost = 0x004B0E00,
		.Metropolis = 0x004B0150,
		.BlackwaterCity = 0x004AD968,
		.CommandCenter = 0x004ADB20,
    	.BlackwaterDocks = 0x004B0360,
    	.AquatosSewers = 0x004AF6A0,
 		.MarcadiaPalace = 0x004AEFE0,
	};
	VariableAddress_t vaDrawCameraSpeedInputIGFunc = {
		.Lobby = 0,
		.Bakisi = 0x004BBE8C,
		.Hoven = 0x004BDEE4,
		.OutpostX12 = 0x004B37FC,
    	.KorgonOutpost = 0x004B1014,
		.Metropolis = 0x004B0364,
		.BlackwaterCity = 0x004ADB7C,
		.CommandCenter = 0x004ADD34,
    	.BlackwaterDocks = 0x004B0574,
    	.AquatosSewers = 0x004AF8B4,
    	.MarcadiaPalace = 0x004AF1F4,
	};
#endif

	if (isInMenus())
	{
		// overwrite menu camera controls max cam speed
		// also display speed text next to input
		u32 ui = uiGetPointer(UIP_CONTROLS);
		if (ui && uiGetActivePointer(UIP_EDIT_PROFILE))
		{
			u32 cameraRotationUIPtr = *(u32*)(ui + 0x11C);
			if (cameraRotationUIPtr)
			{
				// max speed
				*(u32*)(cameraRotationUIPtr + 0x7C) = MAX_CAMERA_SPEED;

				// draw %
				if (uiGetActiveSubPointer(UIP_CONTROLS))
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
	VariableAddress_t vaWhoHitMeFunc = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x004f8890,
		.Hoven = 0x004fa9a8,
		.OutpostX12 = 0x004f0280,
		.KorgonOutpost = 0x004eda18,
		.Metropolis = 0x004ecd68,
		.BlackwaterCity = 0x004ea600,
		.CommandCenter = 0x004ea5c8,
		.BlackwaterDocks = 0x004ece48,
		.AquatosSewers = 0x004ec148,
		.MarcadiaPalace = 0x004ebac8,
#else
		.Lobby = 0,
		.Bakisi = 0x004f6110,
		.Hoven = 0x004f8168,
		.OutpostX12 = 0x004eda80,
		.KorgonOutpost = 0x004eb298,
		.Metropolis = 0x004ea5e8,
		.BlackwaterCity = 0x004e7e00,
		.CommandCenter = 0x004e7f88,
		.BlackwaterDocks = 0x004ea7c8,
		.AquatosSewers = 0x004e9b08,
		.MarcadiaPalace = 0x004e9448,
#endif
	};
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
	VariableAddress_t vaWhoHitMeHook = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x004f95d8,
		.Hoven = 0x004fb6f0,
		.OutpostX12 = 0x004f0fc8,
		.KorgonOutpost = 0x004ee760,
		.Metropolis = 0x004edab0,
		.BlackwaterCity = 0x004eb348,
		.CommandCenter = 0x004eb310,
		.BlackwaterDocks = 0x004edb90,
		.AquatosSewers = 0x004ece90,
		.MarcadiaPalace = 0x004ec810,
#else
		.Lobby = 0,
		.Bakisi = 0x004f6e58,
		.Hoven = 0x004f8eb0,
		.OutpostX12 = 0x004ee7C8,
		.KorgonOutpost = 0x004ebfe0,
		.Metropolis = 0x004eb330,
		.BlackwaterCity = 0x004e8b48,
		.CommandCenter = 0x004e8cd0,
		.BlackwaterDocks = 0x004eb510,
		.AquatosSewers = 0x004ea850,
		.MarcadiaPalace = 0x004ea190,
#endif
	};
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
	VariableAddress_t vaShootingFunc = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00495708,
		.Hoven = 0x00497820,
		.OutpostX12 = 0x0048d0f8,
		.KorgonOutpost = 0x0048a7c8,
		.Metropolis = 0x00489be0,
		.BlackwaterCity = 0x00487478,
		.CommandCenter = 0x00487470,
		.BlackwaterDocks = 0x00489cf0,
		.AquatosSewers = 0x00488ff0,
		.MarcadiaPalace = 0x00488970,
#else
		.Lobby = 0,
		.Bakisi = 0x00493718,
		.Hoven = 0x00495770,
		.OutpostX12 = 0x0048b088,
		.KorgonOutpost = 0x004887d8,
		.Metropolis = 0x00487bf0,
		.BlackwaterCity = 0x00485408,
		.CommandCenter = 0x004855c0,
		.BlackwaterDocks = 0x00487e00,
		.AquatosSewers = 0x00487140,
		.MarcadiaPalace = 0x00486a80,
#endif
	};
	// Get player struct from pad
	int pStruct = (*(u32*)((u32)pad + 0x570)) - 0x430c;
	Player * p = (Player*)pStruct;
	// if player health is zero or less, return 0.
	if (p->pNetPlayer->pNetPlayerData->hitPoints <= 0)
		return 0;

	// If not dead, run normal function.
	return ((int (*)(int))GetAddress(&vaShootingFunc))(pad);
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
	VariableAddress_t vaShootingHook = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00532a0c,
		.Hoven = 0x00534b24,
		.OutpostX12 = 0x0052a3fc,
		.KorgonOutpost = 0x00527b94,
		.Metropolis = 0x00526ee4,
		.BlackwaterCity = 0x0052477c,
		.CommandCenter = 0x0052453c,
		.BlackwaterDocks = 0x00526dbc,
		.AquatosSewers = 0x005260bc,
		.MarcadiaPalace = 0x00525a3c,
#else
		.Lobby = 0,
		.Bakisi = 0x0053018c,
		.Hoven = 0x005321e4,
		.OutpostX12 = 0x00527afc,
		.KorgonOutpost = 0x00525314,
		.Metropolis = 0x00524664,
		.BlackwaterCity = 0x00521e7c,
		.CommandCenter = 0x00521dfc,
		.BlackwaterDocks = 0x0052463c,
		.AquatosSewers = 0x0052397c,
		.MarcadiaPalace = 0x005232bc,
#endif
	};
	if (*(u32*)GetAddress(&vaShootingHook) != 0x0C000000 | ((u32)&patchDeadShooting_Hook >> 2))
		*(u32*)GetAddress(&vaShootingHook) = 0x0C000000 | ((u32)&patchDeadShooting_Hook >> 2);
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
	VariableAddress_t vaSniperShotCollLineFixHook = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00408bc0,
		.Hoven = 0x00408528,
		.OutpostX12 = 0x00400420,
		.KorgonOutpost = 0x003ff800,
		.Metropolis = 0x003fe3c0,
		.BlackwaterCity = 0x003fb1a8,
		.CommandCenter = 0x004095b0,
		.BlackwaterDocks = 0x0040b510,
		.AquatosSewers = 0x0040b118,
		.MarcadiaPalace = 0x0040a190,
#else
		.Lobby = 0,
		.Bakisi = 0x00408540,
		.Hoven = 0x00407e28,
		.OutpostX12 = 0x003ffd20,
		.KorgonOutpost = 0x003ff160,
		.Metropolis = 0x003fdd40,
		.BlackwaterCity = 0x003faac8,
		.CommandCenter = 0x00408f18,
		.BlackwaterDocks = 0x0040ae78,
		.AquatosSewers = 0x0040aa80,
		.MarcadiaPalace = 0x00409af8,
#endif
	};

	VariableAddress_t vaSniperShotCreatedHook = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00408508,
		.Hoven = 0x00407e70,
		.OutpostX12 = 0x003ffd68,
		.KorgonOutpost = 0x003ff148,
		.Metropolis = 0x003fdd08,
		.BlackwaterCity = 0x003faaf0,
		.CommandCenter = 0x00408ef8,
		.BlackwaterDocks = 0x0040ae58,
		.AquatosSewers = 0x0040aa60,
		.MarcadiaPalace = 0x00409ad8,
#else
		.Lobby = 0,
		.Bakisi = 0x00407e90,
		.Hoven = 0x00407778,
		.OutpostX12 = 0x003ff670,
		.KorgonOutpost = 0x003feab0,
		.Metropolis = 0x003fd690,
		.BlackwaterCity = 0x003fa418,
		.CommandCenter = 0x00408868,
		.BlackwaterDocks = 0x0040a7c8,
		.AquatosSewers = 0x0040a3d0,
		.MarcadiaPalace = 0x00409448,
#endif
	};

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
	// Function called at vaGetSniperShotDirectionHook
	VariableAddress_t vaGetSniperShotDirection = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x0045d9d0,
		.Hoven = 0x0045f580,
		.OutpostX12 = 0x00456380,
		.KorgonOutpost = 0x00453f10,
		.Metropolis = 0x00453250,
		.BlackwaterCity = 0x00450a80,
		.CommandCenter = 0x00451378,
		.BlackwaterDocks = 0x00453bf8,
		.AquatosSewers = 0x00452ef8,
		.MarcadiaPalace = 0x00452878,
#else
		.Lobby = 0,
		.Bakisi = 0x0045c920,
		.Hoven = 0x0045e410,
		.OutpostX12 = 0x00455250,
		.KorgonOutpost = 0x00452e60,
		.Metropolis = 0x004521a0,
		.BlackwaterCity = 0x0044f950,
		.CommandCenter = 0x00450408,
		.BlackwaterDocks = 0x00452c48,
		.AquatosSewers = 0x00451f88,
		.MarcadiaPalace = 0x004518c8,
#endif
	};

	// call base
	u32 getShotDirectionFunction = GetAddress(&vaGetSniperShotDirection);
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
	VariableAddress_t vaGetSniperShotDirectionHook = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00407394,
		.Hoven = 0x00406cfc,
		.OutpostX12 = 0x003febf4,
		.KorgonOutpost = 0x003fdfd4,
		.Metropolis = 0x003fcb94,
		.BlackwaterCity = 0x003f997c,
		.CommandCenter = 0x00407d84,
		.BlackwaterDocks = 0x00409ce4,
		.AquatosSewers = 0x004098ec,
		.MarcadiaPalace = 0x00408964,
#else
		.Lobby = 0,
		.Bakisi = 0x00406d2c,
		.Hoven = 0x00406614,
		.OutpostX12 = 0x003fe50c,
		.KorgonOutpost = 0x003fd94c,
		.Metropolis = 0x003fc52c,
		.BlackwaterCity = 0x003f92b4,
		.CommandCenter = 0x00407704,
		.BlackwaterDocks = 0x00409664,
		.AquatosSewers = 0x0040926c,
		.MarcadiaPalace = 0x004082e4,
#endif
	};

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
// 	VariableAddress_t vaAllWeaponsUDPtoTCP = {
// #if UYA_PAL
// 		.Lobby = 0,
// 		.Bakisi = 0x00546760,
// 		.Hoven = 0x00548928,
// 		.OutpostX12 = 0x0053e200,
// 		.KorgonOutpost = 0x0053b8e8,
// 		.Metropolis = 0x0053ace8,
// 		.BlackwaterCity = 0x005384d0,
// 		.CommandCenter = 0x00537d28,
// 		.BlackwaterDocks = 0x0053a5a8,
// 		.AquatosSewers = 0x005398a8,
// 		.MarcadiaPalace = 0x00539228,
// #else
// 		.Lobby = 0,
// 		.Bakisi = 0x00543e54,
// 		.Hoven = 0x00545f5c,
// 		.OutpostX12 = 0x0053b874,
// 		.KorgonOutpost = 0x00538fdc,
// 		.Metropolis = 0x005383dc,
// 		.BlackwaterCity = 0x00535b44,
// 		.CommandCenter = 0x00535574,
// 		.BlackwaterDocks = 0x00537db4,
// 		.AquatosSewers = 0x005370f4,
// 		.MarcadiaPalace = 0x00536a34,
// #endif
// 	};

	VariableAddress_t vaFluxUDPtoTCP = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x0040931c,
		.Hoven = 0x00408c84,
		.OutpostX12 = 0x00400b7c,
		.KorgonOutpost = 0x003fff5c,
		.Metropolis = 0x003feb1c,
		.BlackwaterCity = 0x003fb904,
		.CommandCenter = 0x00409d0c,
		.BlackwaterDocks = 0x0040bc6c,
		.AquatosSewers = 0x0040b874,
		.MarcadiaPalace = 0x0040a8ec,
#else
		.Lobby = 0,
		.Bakisi = 0x00408c7c,
		.Hoven = 0x00408564,
		.OutpostX12 = 0x0040045c,
		.KorgonOutpost = 0x003ff89c,
		.Metropolis = 0x003fe47c,
		.BlackwaterCity = 0x003fb204,
		.CommandCenter = 0x00409654,
		.BlackwaterDocks = 0x0040b5b4,
		.AquatosSewers = 0x0040b1bc,
		.MarcadiaPalace = 0x0040a234,
#endif
	};
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
	int GEF = GetAddress(&vaGadgetEventFunc);
	Player * player = (Player*)((u32)message - 0x1a40);
	tNW_GadgetEventMessage * msg = (tNW_GadgetEventMessage*)message;
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
	VariableAddress_t vaLevelOfDetail_Hook = {
	#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x004c9018,
		.Hoven = 0x004cb130,
		.OutpostX12 = 0x004c0a08,
		.KorgonOutpost = 0x004be1a0,
		.Metropolis = 0x004bd4f0,
		.BlackwaterCity = 0x004bad88,
		.CommandCenter = 0x004bad80,
		.BlackwaterDocks = 0x004bd600,
		.AquatosSewers = 0x004bc900,
		.MarcadiaPalace = 0x004bc280,
	#else
		.Lobby = 0,
		.Bakisi = 0x004c68d8,
		.Hoven = 0x004c8930,
		.OutpostX12 = 0x004be248,
		.KorgonOutpost = 0x004bba60,
		.Metropolis = 0x004badb0,
		.BlackwaterCity = 0x004b85c8,
		.CommandCenter = 0x004b8780,
		.BlackwaterDocks = 0x004bafc0,
		.AquatosSewers = 0x004ba300,
		.MarcadiaPalace = 0x004b9c40,
	#endif
	};

	VariableAddress_t vaLevelOfDetail_Shrubs = {
	#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00248e18,
		.Hoven = 0x00249018,
		.OutpostX12 = 0x00248f08,
		.KorgonOutpost = 0x00248d88,
		.Metropolis = 0x00248e08,
		.BlackwaterCity = 0x00248d88,
		.CommandCenter = 0x00248988,
		.BlackwaterDocks = 0x00248a88,
		.AquatosSewers = 0x00248a88,
		.MarcadiaPalace = 0x00248a88,
	#else
		.Lobby = 0,
		.Bakisi = 0x00248f98,
		.Hoven = 0x00249198,
		.OutpostX12 = 0x00249088,
		.KorgonOutpost = 0x00248f08,
		.Metropolis = 0x00248f88,
		.BlackwaterCity = 0x00248f08,
		.CommandCenter = 0x00248b08,
		.BlackwaterDocks = 0x00248c08,
		.AquatosSewers = 0x00248c08,
		.MarcadiaPalace = 0x00248c08,
	#endif
	};

	VariableAddress_t vaLevelOfDetail_Terrain = {
	#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00248ee0,
		.Hoven = 0x002490e0,
		.OutpostX12 = 0x00248fd0,
		.KorgonOutpost = 0x00248e50,
		.Metropolis = 0x00248ed0,
		.BlackwaterCity = 0x00248e50,
		.CommandCenter = 0x00248a50,
		.BlackwaterDocks = 0x00248b50,
		.AquatosSewers = 0x00248b50,
		.MarcadiaPalace = 0x00248b50,
	#else
		.Lobby = 0,
		.Bakisi = 0x00249060,
		.Hoven = 0x00249260,
		.OutpostX12 = 0x00249150,
		.KorgonOutpost = 0x00248fd0,
		.Metropolis = 0x00249050,
		.BlackwaterCity = 0x00248fd0,
		.CommandCenter = 0x00248bd0,
		.BlackwaterDocks = 0x00248cd0,
		.AquatosSewers = 0x00248cd0,
		.MarcadiaPalace = 0x00248cd0,
	#endif
	};

	VariableAddress_t vaLevelOfDetail_Ties = {
	#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x00248f44,
		.Hoven = 0x00249144,
		.OutpostX12 = 0x00249034,
		.KorgonOutpost = 0x00248eb4,
		.Metropolis = 0x00248f34,
		.BlackwaterCity = 0x00248eb4,
		.CommandCenter = 0x00248ab4,
		.BlackwaterDocks = 0x00248bb4,
		.AquatosSewers = 0x00248bb4,
		.MarcadiaPalace = 0x00248bb4,
	#else
		.Lobby = 0,
		.Bakisi = 0x002490c4,
		.Hoven = 0x002492c4,
		.OutpostX12 = 0x002491b4,
		.KorgonOutpost = 0x00249034,
		.Metropolis = 0x002490b4,
		.BlackwaterCity = 0x00249034,
		.CommandCenter = 0x00248c34,
		.BlackwaterDocks = 0x00248d34,
		.AquatosSewers = 0x00248d34,
		.MarcadiaPalace = 0x00248d34,
	#endif
	};

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
	if (uiGetActivePointer(UIP_STAGING) != 0)
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
				((void (*)(u32, u32, u32))0x006c4308)(uiGetActivePointer(UIP_STAGING), 5, 0);
		#else
				((void (*)(u32, u32, u32))0x006c17f0)(uiGetActivePointer(UIP_STAGING), 5, 0);
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

void setupPatchConfigInGame()
{
	VariableAddress_t vaPauseMenuAddr = {
#if UYA_PAL
		.Lobby = 0,
		.Bakisi = 0x003c073c,
		.Hoven = 0x003bfd7c,
		.OutpostX12 = 0x003b7c7c,
		.KorgonOutpost = 0x003b7a3c,
		.Metropolis = 0x003b77bc,
		.BlackwaterCity = 0x003b79fc,
		.CommandCenter = 0x003c7f7c,
		.BlackwaterDocks = 0x003c837c,
		.AquatosSewers = 0x003c8afc,
		.MarcadiaPalace = 0x003c807c,
#else
		.Lobby = 0,
		.Bakisi = 0x003c087c,
		.Hoven = 0x003bfebc,
		.OutpostX12 = 0x003b7dbc,
		.KorgonOutpost = 0x003b7b7c,
		.Metropolis = 0x003b78fc,
		.BlackwaterCity = 0x003b7b3c,
		.CommandCenter = 0x003c80bc,
		.BlackwaterDocks = 0x003c84bc,
		.AquatosSewers = 0x003c8c3c,
		.MarcadiaPalace = 0x003c81bc,
#endif
	};
    // Get Menu address via current map.
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
	// call normal draw routine
	drawFunction();
	
	lastMenuInvokedTime = gameGetTime();
	if (!hasInitialized)
	{
		printf("onOnlinemenu - pad enable input\n");
		padEnableInput();
		onConfigInitialize();
		hasInitialized = 1;
	}
	if (hasInitialized == 1 && uiGetActivePointer(UIP_ONLINE_LOBBY) != 0)
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
			sprintf(buf, "Please install %s to play.", dataCustomMaps.items[(int)gameConfig.customMapId]);
			uiShowOkDialog("Custom Maps", buf);
		}
		
		showNoMapPopup = 0;
	}
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
		DPRINTF("pad enable input\n");
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

	// 
	onConfigUpdate();

	if(isInGame())
	{
		// Run Game Rules if in game.
		grGameStart();

		// Patch Dead Jumping/Crouching
		patchDeadJumping();

		// Patch Dead Shooting
		patchDeadShooting();

		// Patch Kill Stealing
		patchKillStealing();

		// Patch sending weapon shots via UDB to TCP.
		patchWeaponShotLag();

		// Patch Flux Niking
		if (gameConfig.grFluxNikingDisabled)
			patchSniperNiking();

		// Patch Flux Wall Sniping
		if (gameConfig.grFluxShotsAlwaysHit)
			patchSniperWallSniping();

		// Patch Level of Detail
		patchLevelOfDetail();

		// Patch Weapon Ordering when Respawning
		patchResurrectWeaponOrdering();

		// Patches gadget events as they come in.
		// patchGadgetEvents();

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
		// If in Lobby, run these game rules.
		grLobbyStart();

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

				// send
				configTrySendGameConfig();
			}
			
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
