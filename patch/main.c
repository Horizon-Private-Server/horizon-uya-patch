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
#include <libuya/net.h>
#include <libuya/interop.h>
#include <libuya/utils.h>
#include <libuya/player.h>
#include "module.h"
#include "messageid.h"
#include "config.h"
#include "include/config.h"
#include "include/cheats.h"

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

/*
 * Array of game modules.
 */
#define GLOBAL_GAME_MODULES_START			((GameModule*)0x000CF000)

#define EXCEPTION_DISPLAY_ADDR			(0x000C8000)

PatchConfig_t config __attribute__((section(".config"))) = {
	.enableAutoMaps = 0,
	.disableCameraShake = 0,
};

PatchGameConfig_t gameConfig;
PatchGameConfig_t gameConfigHostBackup;

// 
int hasInitialized = 0;
int lastMenuInvokedTime = 0;
int lastGameState = 0;
int sentGameStart = 0;
int isInStaging = 0;
int hasInstalledExceptionHandler = 0;
char mapOverrideResponse = 1;
char showNoMapPopup = 0;
const char * patchStr = "PATCH CONFIG";

extern MenuElem_ListData_t dataCustomMaps;

// downloader
int dlBytesReceived = 0;
int dlTotalBytes = 0;

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
			player->CantMoveTimer = 10;
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
		.KorgonOutpost = 0x003b7a38,
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
