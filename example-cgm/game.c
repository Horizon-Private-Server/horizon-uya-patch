/***************************************************
 * FILENAME :		game.c
 * DESCRIPTION :
 * 		PAYLOAD.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>
#include <string.h>
#include <libuya/stdio.h>

#include "config.h"
#include "include/game.h"

//--------------------------------------------------------------------------
void gameTick(void)
{
	// run custom game logic here
	DPRINTF("custom game mode!! %d\n", gameGetTime());
}

//--------------------------------------------------------------------------
void initialize(PatchGameConfig_t* gameConfig)
{
	// hook messages
	netHookMessages();

	// give a 1 second delay before finalizing the initialization.
	// this helps prevent the slow loaders from desyncing
	static int startDelay = 60 * 1;
	if (startDelay > 0) {
		--startDelay;
		return;
	}

	// finalize initialization
	State.Initialized = 1;
}
