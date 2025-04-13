/***************************************************
 * FILENAME :		main.c
 * DESCRIPTION :
 * 		Unhooks patch file and spawns menu popup telling user patch is downloading.
 * NOTES :
 * 		Each offset is determined per game region.
 * 		This is to ensure compatibility between versions of UYA PAL/NTSC.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>

#include "module.h"
#include "messageid.h"
#include <libuya/uya.h>
#include <libuya/game.h>
#include <libuya/time.h>
#include <libuya/string.h>
#include <libuya/stdio.h>
#include <libuya/ui.h>
#include <libuya/net.h>
#include <libuya/graphics.h>
#include <libuya/pad.h>
#include <libuya/player.h>
#include <libuya/gamesettings.h>

/*
	-1 = Both
	0 = Not In Game
	1 = In Game
*/
const int patches[][3] = {
#if UYA_PAL
	// patch
	{ 0, 0x005760E4, 0x0c19f270 }, // menu loop hook 1
	{ 0, 0x0057611C, 0x0c19f270 }, // menu loop hook 2
	{ -1, 0x002405e8, 0x00000000 }, // DME_CALLBACK_TABLE
	{ 0, 0x0047cfec, 0x0069a630 }, // patchCreateGame
	{ 0, 0x0047d124, 0x0069ce50 }, // patchAdvancedOptions
	{ 0, 0x0047e9ac, 0x006c1730 }, // patchStaging
	{ 0, 0x0047c834, 0x00690de8 }, // patchBuddies
	{ 0, 0x0047e44c, 0x006b48c8 }, // patchPlayerDetails
	{ 0, 0x0047eab4, 0x006c5cc8 }, // patchStats
	{ 0, 0x0047dbac, 0x006aaa08 }, // patchKeyboard

	// maploader
	{ 0, 0x005a41c8, 0x0c170a90 }, // LOAD_LEVEL_READ_WAD_HOOK
	{ 0, 0x00194f8c, 0x0c0653b6 }, // LOAD_LEVEL_READ_LEVEL_TOC_HOOK
	{ 0, 0x005a4070, 0x0c06528e }, // LOAD_LEVEL_CD_SYNC_HOOK
	{ 0, 0x006787a8, 0x03e00008 }, // LOAD_LEVEL_TRANSITION_MENU_LOAD_HOOK
	{ 0, 0x00193340, 0x0c064bda }, // LEVEL_EXIT_FUNCTION_HOOK
#else
	// patch
	{ 0, 0x005753A4, 0x0c19e7c2 }, // menu loop hook 1
	{ 0, 0x005753DC, 0x0c19e7c2 }, // menu loop hook 2
	{ -1, 0x00240768, 0x00000000 }, // DME_CALLBACK_TABLE
	{ 0, 0x0047d0ac, 0x00697e20 }, // patchCreateGame
	{ 0, 0x0047d1e4, 0x0069a640 }, // patchAdvancedOptions
	{ 0, 0x0047ea6c, 0x006bec18 }, // patchStaging
	{ 0, 0x0047c8f4, 0x0068e6f8 }, // patchBuddies
	{ 0, 0x0047e50c, 0x006b1f60 }, // patchPlayerDetails
	{ 0, 0x0047eb74, 0x006c31b8 }, // patchStats
	{ 0, 0x0047dc6c, 0x006a80b8 }, // patchKeyboard

	// maploader
	{ 0, 0x005a2560, 0x0c17027e }, // LOAD_LEVEL_READ_WAD_HOOK
	{ 0, 0x0019507c, 0x0c0653f2 }, // LOAD_LEVEL_READ_LEVEL_TOC_HOOK
	{ 0, 0x005a2408, 0x0c0652ca }, // LOAD_LEVEL_CD_SYNC_HOOK
	{ 0, 0x00675dc0, 0x03e00008 }, // LOAD_LEVEL_TRANSITION_MENU_LOAD_HOOK
	{ 0, 0x00193430, 0x0c064c16 }, // LEVEL_EXIT_FUNCTION_HOOK
#endif
	// patch (PAL and NTSC are same)
	{ -1, 0x00153248, 0x03E00008 }, // GET_MEDIUS_APP_HANDLER_HOOK
	{ -1, 0x0013cae0, 0x0c0474e0 }, // ScePadRead_memcpy Hook
};

const int clears[][2] = {
	{ 0x000d0000, 0x00010000 }, // usb modules
	{ 0x000e0000, 0x00015000 }, // patch
	{ 0x000f5000, 0x0000a000 }, // game mode
	{ 0x000cf000, 0x00000800 }, // module definitions
	{ 0x000cffd0, 0x00000020 }, // patch hash
	{ 0x000cffc0, 0x00000010 }, // patch pointers
};

int hasClearedMemory = 0;
int bytesReceived = 0;
int totalBytes = 0;

int onServerDownloadDataRequest(void * connection, void * data)
{
	ServerDownloadDataRequest_t* request = (ServerDownloadDataRequest_t*)data;

	// copy bytes to target
	totalBytes = request->TotalSize;
	bytesReceived += request->DataSize;
	memcpy((void*)request->TargetAddress, request->Data, request->DataSize);
	DPRINTF("DOWNLOAD: {%d} %d/%d, writing %d to %08X\n", request->Id, bytesReceived, request->TotalSize, request->DataSize, request->TargetAddress);

	// respond
	if (connection && (!request->Chunk || bytesReceived >= request->TotalSize))
	{
		ClientDownloadDataResponse_t response;
		response.Id = request->Id;
		response.BytesReceived = bytesReceived;
		netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_DOWNLOAD_DATA_RESPONSE, sizeof(ClientDownloadDataResponse_t), &response);
	}

	return sizeof(ServerDownloadDataRequest_t) - sizeof(request->Data) + request->DataSize;
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
	u32 bgColorDownload = 0x8004223f;
	u32 textColor = 0x8069cbf2;
	u32 barBgColor = 0x80123251;
	u32 barFgColor = 0x8018608f;

	// call normal draw routine
#ifdef UYA_PAL
    ((void (*)(void))0x0067C9C0)();
#else
	((void (*)(void))0x00679F08)();
#endif

	// only show on main menu
	if (uiGetActiveMenu(UI_MENU_ONLINE_LOBBY, 0) != 0)
	{
    gfxScreenSpaceBox(0.2, 0.35, 0.6, 0.125, bgColorDownload);
    gfxScreenSpaceBox(0.2, 0.45, 0.6, 0.05, barBgColor);
    gfxScreenSpaceText(SCREEN_WIDTH * 0.35, SCREEN_HEIGHT * 0.4, 1, 1, textColor, "Downloading patch...", 17 + (gameGetTime()/240 % 4), 3);

    if (totalBytes > 0)
    {
      float w = (float)bytesReceived / (float)totalBytes;
      gfxScreenSpaceBox(0.2, 0.45, 0.6 * w, 0.05, barFgColor);
    }
  }
}

/*
 * NAME :		main
 * DESCRIPTION :
 * 			Entrypoint.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int main (void)
{

	int inGame = isInGame();
	int i;
	const int patchesSize =  sizeof(patches) / (3 * sizeof(int));
	const int clearsSize =  sizeof(clears) / (2 * sizeof(int));

	// state
	// 0 = menus
	// 1 = in game
	// 2 = loading scene
	int state = isInGame() ? 1 : (isInMenus() ? 0 : 2);

	// unhook patch
	for (i = 0; i < patchesSize; ++i)
	{
    int context = patches[i][0];
    if (context < 0 || context == state)
      *(u32*)patches[i][1] = (u32)patches[i][2];
	}

	// clear memory
	if (!hasClearedMemory)
	{
		hasClearedMemory = 1;
		for (i = 0; i < clearsSize; ++i)
		{
			memset((void*)clears[i][0], 0, clears[i][1]);
		}
	}

	// 
	netInstallCustomMsgHook(1);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_SERVER_DOWNLOAD_DATA_REQUEST, &onServerDownloadDataRequest);

	if (state == 0 && uiGetActiveMenu(UI_MENU_ONLINE_LOBBY, 0) != 0)
	{
		// Hook menu loop
		#ifdef UYA_PAL
		*(u32*)0x005760E4 = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		*(u32*)0x0057611C = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		#else
		*(u32*)0x005753A4 = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		*(u32*)0x005753DC = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		#endif

		// disable pad on online main menu
		// if (uiGetActiveMenu(UI_MENU_ONLINE_LOBBY, 0) != 0)
		padDisableInput();
	}

	return 0;
}
