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
	0 // enableInfiniteHealth
};

// 
int hasInitialized = 0;
int lastMenuInvokedTime = 0;
int lastGameState = 0;
int isInStaging = 0;

// Disable Weapon Pack
int GetLevel(void * GameplayFilePointer)
{
    int WeaponPack_Func = 0;
    switch((u32)GameplayFilePointer)
    {
        // Outpost X12
        case 0x0043A288:
            WeaponPack_Func = 0x004EF540;
            break;
        // Korgon Outpost
        case 0x00437EC8:
            WeaponPack_Func = 0x004ECD58;
            break;
        // Metropolis
        case 0x00437208:
            WeaponPack_Func = 0x004EC0A8;
            break;
        // Blackwater City
        case 0x00434988:
            WeaponPack_Func = 0x004E98C0;
            break;
        // Command Center
        case 0x004357C8:
            WeaponPack_Func = 0x004E9A48;
            break;
        // Blackwater Docks
        case 0x00438008:
            WeaponPack_Func = 0x004EC288;
            break;
        // Aquatos Sewers
        case 0x00437348:
            WeaponPack_Func = 0x004EB5C8;
            break;
        // Marcadia Palace
        case 0x00436C88:
            WeaponPack_Func = 0x004EAF08;
            break;
        // Bakisi Isle
        case 0x00441988:
            WeaponPack_Func = 0x004F7BD0;
            break;
        // Hoven Gorge
        case 0x00443448:
            WeaponPack_Func = 0x004F9C28;
            break;
    }
	return WeaponPack_Func;
}
void DisableWeaponPack(void)
{
	// Grab GameplayFile Pointer.
	// This is where all the functions for the game are kept.
	void * GameplayFilePointer = (void*)(*(u32*)0x01FFFD00);
	// If pointer doesn't equel Online Lobby Pointer, proceed.
	if (GameplayFilePointer != 0x00574F88)
	{
		// Check which Level we are on.  Function returns the address of the Weapon Pack function.
		int WeaponPack = GetLevel(GameplayFilePointer);
		// If WeaponPack function isn't Zero, make it zero.
		if (*(u32*)WeaponPack != 0)
			*(u32*)WeaponPack = 0;
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
	// call normal draw routine
	drawFunction();

	lastMenuInvokedTime = gameGetTime();

	if (!hasInitialized)
	{
		padEnableInput();
		// onConfigInitialize();
		hasInitialized = 1;
	}
	if (hasInitialized == 1 && GetActiveUIPointer(UIP_ONLINE_LOBBY) != 0)
	{
		uiShowOkDialog("System", "Patch has been successfully loaded.");
		hasInitialized = 2;
	}

	if(GetActiveUIPointer(UIP_ONLINE_LOBBY) != 0)
		gfxScreenSpaceText(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.85, 1, 1, 0x80FFFFFF, "\x1f Open Config Menu", -1, 4);
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
	if (lastMenuInvokedTime > 0 && gameGetTime() - lastMenuInvokedTime > TIME_SECOND)
	{
		padEnableInput();
		lastMenuInvokedTime = 0;
	}

	// Hook menu loop
	#ifdef UYA_PAL
	if (*(u32*)0x00576120 == 0)
		*(u32*)0x0057611C = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
	#else
	if (*(u32*)0x005753E0 == 0)
		*(u32*)0x005753DC = 0x0C000000 | ((u32)(&onOnlineMenu) / 4);
	#endif

	// this version of disable weapon pack only works in NTSC.
	#ifdef UYA_NTSC
	DisableWeaponPack();
	#endif

	// Call this last
	uyaPostUpdate();

	return 0;
}
