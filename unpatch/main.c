/***************************************************
 * FILENAME :		main.c
 * 
 * DESCRIPTION :
 * 		Unhooks patch file and spawns menu popup telling user patch is downloading.
 * 
 * NOTES :
 * 		Each offset is determined per game region.
 * 		This is to ensure compatibility between versions of UYA PAL/NTSC.
 * 		
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

const int patches[][2] = {
	
};

const int clears[][2] = {
	{ 0x000E0000, 0x0000C000 }, // patch
	{ 0x000CF000, 0x00000800 }, // module definitions
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
	if (connection)
	{
		ClientDownloadDataResponse_t response;
		response.Id = request->Id;
		response.BytesReceived = bytesReceived;
		netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_DOWNLOAD_DATA_RESPONSE, sizeof(ClientDownloadDataResponse_t), &response);
	}

	return sizeof(ServerDownloadDataRequest_t) - sizeof(request->Data) + request->DataSize;
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
 * AUTHOR :			Troy "Agent Moose" Pruitt
 */
void drawFunction(void)
{
#ifdef UYA_PAL
    ((void (*)(void))0x0067C9C0)();
#else
	((void (*)(void))0x00679F08)();
#endif
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
	u32 bgColorDownload = 0x8004223f;
	u32 textColor = 0x8069cbf2;
	u32 barBgColor = 0x80123251;
	u32 barFgColor = 0x8018608f;

	// call normal draw routine
	drawFunction();

	// only show on main menu
	if (uiGetActivePointer(UIP_ONLINE_LOBBY) != 0)
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
 * 
 * DESCRIPTION :
 * 			Entrypoint.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int main (void)
{

	int inGame = isInGame();
	int i;
	const int patchesSize =  sizeof(patches) / (2 * sizeof(int));
	const int clearsSize =  sizeof(clears) / (2 * sizeof(int));

	// unhook patch
	for (i = 0; i < patchesSize; ++i)
	{
		*(u32*)patches[i][0] = (u32)patches[i][1];
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

	if (!inGame)
	{
		// Hook menu loop
		#ifdef UYA_PAL
		*(u32*)0x0057611C = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		#else
		*(u32*)0x005753A4 = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		*(u32*)0x005753DC = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		#endif

		// disable pad on online main menu
		if (uiGetActivePointer(UIP_ONLINE_LOBBY) != 0)
			padDisableInput();
	}

	return 0;
}
