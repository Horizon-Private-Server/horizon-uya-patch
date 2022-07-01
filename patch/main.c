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
#include <libuya/pad.h>
#include <libuya/stdio.h>
#include <libuya/uya.h>
#include <libuya/math3d.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include <libuya/time.h>
#include "module.h"
#include "config.h"

void onConfigOnlineMenu(void);
void onConfigGameMenu(void);
void configMenuEnable(void);
void configMenuDisable(void);
int GetActiveUIPointer(int);

/*
 * Array of game modules.
 */
#define GLOBAL_GAME_MODULES_START			((GameModule*)0x000CF000)

#define EXCEPTION_DISPLAY_ADDR			(0x000BA000)

// config
PatchConfig_t config __attribute__((section(".config"))) = {
	0
};

// 
int hasInitialized = 0;
int lastMenuInvokedTime = 0;
int lastGameState = 0;
int isInStaging = 0;

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
	//printf("onOnlineMenu - run draw function\n");
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
	// call normal draw routine
	drawFunction();

	lastMenuInvokedTime = gameGetTime();
	if (!hasInitialized)
	{
		printf("onOnlinemenu - pad enable input\n");
		padEnableInput();
		// onConfigInitialize();
		hasInitialized = 1;
	}
	if (hasInitialized == 1 && GetActiveUIPointer(UIP_ONLINE_LOBBY) != 0)
	{
		uiShowOkDialog("System", "Patch has been successfully loaded.");
		hasInitialized = 2;
	}

	// settings
	onConfigOnlineMenu();
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
	//printf("begining of main\n");
	// Call this first
	uyaPreUpdate();

	// auto enable pad input to prevent freezing when popup shows
	if (lastMenuInvokedTime > 0 && gameGetTime() - lastMenuInvokedTime > TIME_SECOND)
	{
		//printf("pad enable input\n");
		padEnableInput();
		lastMenuInvokedTime = 0;
	}

	// void * GameplayFilePointer = (void*)(*(u32*)0x01FFFD00);
	if(gameIsIn())
	{
		// In game stuff
	}
	else
	{
		// Not In game stuff
		// Hook menu loop
#ifdef UYA_PAL
		if (*(u32*)0x00576120 == 0)
			*(u32*)0x0057611C = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
#else
		//printf("patching main menu\n");
		if (*(u32*)0x005753E0 == 0)
			*(u32*)0x005753DC = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
#endif
	}

	// Call this last

	//printf("uyaPostUpdate\n");
	uyaPostUpdate();

	return 0;
}
