/***************************************************
 * FILENAME :		main.c
 * 
 * DESCRIPTION :
 * 		Manages and applies all Deadlocked patches.
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
#include <libuya/pad.h>
#include <libuya/stdio.h>
#include <libuya/uya.h>
#include <libuya/math3d.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include "module.h"

// void onConfigOnlineMenu(void);
// void onConfigGameMenu(void);
// void configMenuEnable(void);
// void configMenuDisable(void);

/*
 * Array of game modules.
 */
#define GLOBAL_GAME_MODULES_START			((GameModule*)0x000CF000)

#define EXCEPTION_DISPLAY_ADDR			(0x000BA000)

// config
// PatchConfig_t config __attribute__((section(".config"))) = {
// 	0
// };

// 
int hasInitialized = 0;

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
	((void (*)(void))0x00679F08)();

	if (!hasInitialized)
	{
		printf("Init 1\n");
		// padEnableInput();
		// onConfigInitialize();
		hasInitialized = 1;
	}
	if (hasInitialized == 1/* && GetActiveUIPointer(UIP_ONLINE_LOBBY) != 0 */)
	{
		printf("Init 2\n");
		// uiShowOkDialog("System", "Patch has been successfully loaded.");
		hasInitialized = 2;
	}
	gfxScreenSpaceText(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.855, 1, 1, 0x80FFFFFF, "\x18 + \x19 Open Mod Menu", -1, 4);
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
int main (void)
{
	// Call this first
	uyaPreUpdate();

	if (*(u32*)0x005753E0 == 0)
		*(u32*)0x005753DC = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);

	// Call this last
	uyaPostUpdate();

	return 0;
}
