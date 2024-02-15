#include "string.h"
#include "pad.h"
#include "game.h"
#include "player.h"

#ifdef UYA_PAL

// PAL
#define PAD_POINTER                         ((PadButtonStatus**)0x00241164)
#define P1_PAD                              ((PadButtonStatus*)0x00225800)
#define P2_PAD                              ((PadButtonStatus*)0x00226F00)
#define P3_PAD                              ((PadButtonStatus*)0x00228600)
#define PAD_PROCESS_ADDR                    (*(u32*)0x00686300)
#define PAD_PROCESS_VALUE                   (0x0c1a184e)

#else

// NTSC
#define PAD_POINTER                         ((PadButtonStatus**)0x002412e4)
#define P1_PAD                              ((PadButtonStatus*)0x00225980)
#define P2_PAD                              ((PadButtonStatus*)0x00227080)
#define P3_PAD                              ((PadButtonStatus*)0x00228780)
#define PAD_PROCESS_ADDR                    (*(u32*)0x006837E0)
#define PAD_PROCESS_VALUE                   (0x0C1A0D86)

#endif

// Default value for pad history
const PadHistory DefaultPadHistory = {
    0xFFFF,
    0x7F,
    0x7F,
    0x7F,
    0x7F,
    -1
};

// Local pad history
PadHistory LocalPadHistory[PAD_PORT_MAX * 2] = {
  {0xFFFF,0x7F,0x7F,0x7F,0x7F,0},
  {0xFFFF,0x7F,0x7F,0x7F,0x7F,1},
  {0xFFFF,0x7F,0x7F,0x7F,0x7F,0},
  {0xFFFF,0x7F,0x7F,0x7F,0x7F,1},
};

/*
 * NAME :		padUpdate
 * 
 * DESCRIPTION :
 * 			Processes pad state, registering when a button is pressed and released.
 * 
 * NOTES :
 *          This must be called every tick in order for pad functionality to work.
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void padUpdate(void)
{
    // Update local pad
    memcpy(LocalPadHistory+2, LocalPadHistory+0, 6);
    memcpy(LocalPadHistory+3, LocalPadHistory+1, 6);

    memcpy(LocalPadHistory, &P1_PAD->btns, 6);
    memcpy(LocalPadHistory+1, &P2_PAD->btns, 6);
}

/*
 * NAME :		padGetButton
 * 
 * DESCRIPTION :
 * 			Returns 1 when the user is pressing the given button combination.
 *          Returns negative on failure.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          port:                       Which controller port to read.
 *          buttonMask:                 Buttons to check.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int padGetButton(int port, u16 buttonMask)
{
    switch (port)
    {
        case 0: return (P1_PAD->btns & buttonMask) == 0;
        case 1: return (P2_PAD->btns & buttonMask) == 0;
        default: return -1;
    }
}

/*
 * NAME :		padGetButtonDown
 * 
 * DESCRIPTION :
 * 			Returns 1 during the frame that the user starts pressing the given button combination.
 *          Returns negative on failure.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          port:                       Which controller port to read.
 *          buttonMask:                 Buttons to check.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int padGetButtonDown(int port, u16 buttonMask)
{
    if (port < 0 || port >= PAD_PORT_MAX)
        return -1;

    return (LocalPadHistory[port + 0].btns & buttonMask) == 0 &&
            (LocalPadHistory[port + 2].btns & buttonMask) != 0;
}

/*
 * NAME :		padGetButtonUp
 * 
 * DESCRIPTION :
 * 			Returns 1 during the frame that the user releases the given button combination.
 *          Returns negative on failure.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          port:                       Which controller port to read.
 *          buttonMask:                 Buttons to check.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int padGetButtonUp(int port, u16 buttonMask)
{
    if (port < 0 || port >= PAD_PORT_MAX)
        return -1;

    return (LocalPadHistory[port + 0].btns & buttonMask) != 0 &&
        (LocalPadHistory[port + 2].btns & buttonMask) == 0;
}

/*
 * NAME :		padResetInput
 * 
 * DESCRIPTION :
 * 			Resets the given ports inputs.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          port:                       Which controller port to reset.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void padResetInput(int port)
{
    if (port < 0 || port >= PAD_PORT_MAX)
        return;

    PadButtonStatus * pad = port ? P2_PAD : P1_PAD;

    u64 defaultValue = 0x7F7F7F7FFFFF7900;
    *(u64*)((u32)pad + 0x00) = defaultValue;
    *(u64*)((u32)pad + 0x80) = defaultValue;
}

/*
 * NAME :		padDisableInput
 * 
 * DESCRIPTION :
 * 			Disables the pad input.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void padDisableInput(void)
{
    if (PAD_PROCESS_ADDR == PAD_PROCESS_VALUE)
        PAD_PROCESS_ADDR = 0x24020000;

    if (isInGame()) {
        int i;
        Player ** Players = playerGetAll();
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            Player * player = Players[i];
            if (!player)
                continue;

            if (player->IsLocal)
                player->timers.noInput = 3;
        }
    }
}

/*
 * NAME :		padEnableInput
 * 
 * DESCRIPTION :
 * 			Enables the pad input.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void padEnableInput(void)
{
    if (PAD_PROCESS_ADDR == 0x24020000)
        PAD_PROCESS_ADDR = PAD_PROCESS_VALUE;
}
