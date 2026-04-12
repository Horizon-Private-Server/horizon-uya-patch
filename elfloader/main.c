/***************************************************
 * FILENAME :		main.c
 * 
 * DESCRIPTION :
 * 		Unhooks patch file and spawns menu popup telling user patch is downloading.
 * 
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>

#include <libuya/uya.h>
#include <libuya/player.h>
#include <libuya/pad.h>
#include <libuya/time.h>
#include <libuya/net.h>
#include "module.h"
#include "messageid.h"
#include <libuya/game.h>
#include <libuya/string.h>
#include <libuya/stdio.h>
#include <libuya/gamesettings.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>

// int selfDestruct __attribute__((section(".config"))) = 0;

int bytesReceived = 0;
int totalBytes = 0;

int loadelf (u32 loadFromAddress, u32 size);
int doBootElf = 0;
ServerResponseBootElf_t bootElf = {
  .BootElfId = 0,
  .Address = 0,
  .Size = 0
};

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
    response.Stop = 0;
    netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_DOWNLOAD_DATA_RESPONSE, sizeof(ClientDownloadDataResponse_t), &response);
  }

  return sizeof(ServerDownloadDataRequest_t) - sizeof(request->Data) + request->DataSize;
}

/*
 * NAME :		onBootElfResponse
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
int onBootElfResponse(void * connection, void * data)
{
  memcpy(&bootElf, data, sizeof(ServerResponseBootElf_t));

  DPRINTF("[elfloader] boot elf response id:%d addr:%08X size:%08X\n", bootElf.BootElfId, bootElf.Address, bootElf.Size);
  doBootElf = 1;

  return sizeof(ServerResponseBootElf_t);
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
#ifdef UYA_PAL
  ((void (*)(void))0x0067C9C0)();
#else
	((void (*)(void))0x00679F08)();
#endif

  if (bootElf.Address && bootElf.Size && doBootElf == 1) {
    doBootElf = 2;
    DPRINTF("[elfloader] before loadelf\n");
    loadelf(bootElf.Address, bootElf.Size);
  }

  // only show on main menu
	if (uiGetActiveMenu(UI_MENU_ONLINE_LOBBY, 0) != 0)
    return;

  gfxScreenSpaceBox(0.2, 0.35, 0.6, 0.125, bgColorDownload);
  gfxScreenSpaceBox(0.2, 0.45, 0.6, 0.05, barBgColor);
  gfxScreenSpaceText(SCREEN_WIDTH * 0.35, SCREEN_HEIGHT * 0.4, 1, 1, textColor, "Downloading elf...", 17 + (gameGetTime()/240 % 4), 3, FONT_BOLD);

  if (totalBytes > 0)
  {
    float w = (float)bytesReceived / (float)totalBytes;
    gfxScreenSpaceBox(0.2, 0.45, 0.6 * w, 0.05, barFgColor);
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
  int i;
  static int initialized = 0;

  if (!initialized) {
    // reset global net callbacks table ptr
    // *(u32*)0x00211E64 = 0;

    initialized = 1;
    DPRINTF("\n[elfloader]: main init\n");
  }
  
  if (doBootElf == 2) {
    gfxScreenSpaceBox(0.2, 0.35, 0.6, 0.125, 0x80000000);
    gfxScreenSpaceText(SCREEN_WIDTH * 0.26, SCREEN_HEIGHT * 0.41, 1, 1, 0x80C0C0C0, "Loading... this may take awhile...", 31 + (gameGetTime()/240 % 4), 3, FONT_BOLD);
  }
  
  // // just clear if selfDestruct is true
  // if (selfDestruct) {
  //   return;
  // }

  // 
  netInstallCustomMsgHandler(CUSTOM_MSG_ID_SERVER_DOWNLOAD_DATA_REQUEST, &onServerDownloadDataRequest);
  netInstallCustomMsgHandler(CUSTOM_MSG_ID_SERVER_RESPONSE_BOOT_ELF, &onBootElfResponse);

  // Hook menu loop
  #ifdef UYA_PAL
  *(u32*)0x005760E4 = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
  *(u32*)0x0057611C = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
  #else
  *(u32*)0x005753A4 = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
  *(u32*)0x005753DC = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
  #endif

  // disable pad on online main menu
	if (uiGetActiveMenu(UI_MENU_ONLINE_LOBBY, 0) != 0)
    padDisableInput();

  return 0;
}
