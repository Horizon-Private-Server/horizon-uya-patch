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

#include <libuya/uya.h>
#include <libuya/game.h>
#include <libuya/time.h>
#include <libuya/string.h>
#include <libuya/stdio.h>
#include "module.h"
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include <libuya/pad.h>
#include <libuya/player.h>
#include <libuya/gamesettings.h>

const int patches[][2] = {
	
};

const int clears[][2] = {
	{ 0x000D0000, 0x00008000 }, // patch
	{ 0x000C8000, 0x00007000 }, // gamerules
	{ 0x000CF000, 0x00001000 }, // module definitions
};
int hasClearedMemory = 0;

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
	u32 bgColorDownload = 0x70000000;
	u32 downloadColor = 0x80808080;

	// call normal draw routine
	drawFunction();

	// only show on main menu
	if (GetActiveUIPointer(UIP_ONLINE_LOBBY) == 0)
		return;

	// render background
	gfxScreenSpaceBox(0.2, 0.35, 0.6, 0.3, bgColorDownload);

	// flash color
	int gameTime = ((gameGetTime()/100) % 15);
	if (gameTime > 7)
		gameTime = 15 - gameTime;
	downloadColor += 0x101010 * gameTime;

	// render text
	gfxScreenSpaceText(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5, 1, 1, downloadColor, "Downloading patch, please wait...", -1, 4);
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

	int inGame = gameIsIn();
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

	if (!inGame)
	{
		// Hook menu loop
		#ifdef UYA_PAL
		*(u32*)0x0057611C = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		#else
		*(u32*)0x005753DC = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
		#endif

		// disable pad on online main menu
		if (GetActiveUIPointer(UIP_ONLINE_LOBBY) != 0)
			padDisableInput();
	}

	return 0;
}
