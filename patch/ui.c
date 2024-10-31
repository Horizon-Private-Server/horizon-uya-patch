#include <tamtypes.h>
#include <libuya/string.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/pad.h>
#include <libuya/stdio.h>
#include <libuya/uya.h>

#ifdef UYA_PAL
#define STAGING_BASE_FUNC (0x006c1730)
#else
#define STAGING_BASE_FUNC (0x006bec18)
#endif

typedef enum uiPadButtons {
    UI_PAD_NONE = 0,
    UI_PAD_DPAD_UP,
    UI_PAD_DPAD_DOWN,
    UI_PAD_DPAD_LEFT,
    UI_PAD_DPAD_RIGHT,
    UI_PAD_TRIANGLE,
    UI_PAD_CIRCLE,
    UI_PAD_CROSS,
    UI_PAD_SQUARE,
    UI_PAD_SELECT,
    UI_PAD_START,
    UI_PAD_L1,
    UI_PAD_L2,
    UI_PAD_L3,
    UI_PAD_R1,
    UI_PAD_R2,
    UI_PAD_R3,
    UI_PAD_TOTAL = UI_PAD_R3
} uiPadButtons_t;

typedef int (*uiVTable_Func)(void * ui, int pad);
uiVTable_Func stagingFunc = (uiVTable_Func)STAGING_BASE_FUNC;

int patchStaging(void * ui, int pad)
{	
	// call game function we're replacing
	int result = stagingFunc(ui, pad);

	// if (pad != UI_PAD_NONE)
	// 	DPRINTF("\nbutton: %d, ui: %08x", pad, ui);

	// Patch Unkick (Still no worky)
	// int i;
	// GameSettings * gs = gameGetSettings();
	// if (gs) {
	// 	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
	// 		// Check ClientID and if they are kicked and not viewing the kicked popup
	// 		if (gs->PlayerClients[i] == gameGetMyClientId() && gs->PlayerStates[i] == 5) {
	// 			((int (*)(void *, int, int, short))0x006c0c600)(ui, 1, 0, 0x1600);
	// 			return 1;
	// 		}
	// 	}
	// }

	return result;
}