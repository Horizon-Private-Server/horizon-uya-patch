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

#include <libuya/time.h>
#include <libuya/string.h>
#include <libuya/stdio.h>
#include "module.h"

const int patches[][2] = {
	
};

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
	const int size =  sizeof(patches) / (2 * sizeof(int));

	// unhook patch
	for (i = 0; i < size; ++i)
	{
		*(u32*)patches[i][0] = (u32)patches[i][1];
	}

	return 0;
}
