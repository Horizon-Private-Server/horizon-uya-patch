/***************************************************
 * FILENAME :		pad.h
 * 
 * DESCRIPTION :
 * 		Contains pad specific offsets and structures for UYA.
 * 
 * NOTES :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

#ifndef _LIBUYA_PAD_H
#define _LIBUYA_PAD_H

#include <tamtypes.h>
#include "common.h"

#define PAD_PORT_MAX        2

#define PAD_LEFT            0x0080
#define PAD_DOWN            0x0040
#define PAD_RIGHT           0x0020
#define PAD_UP              0x0010
#define PAD_START           0x0008
#define PAD_R3              0x0004
#define PAD_L3              0x0002
#define PAD_SELECT          0x0001
#define PAD_SQUARE          0x8000
#define PAD_CROSS           0x4000
#define PAD_CIRCLE          0x2000
#define PAD_TRIANGLE        0x1000
#define PAD_R1              0x0800
#define PAD_L1              0x0400
#define PAD_R2              0x0200
#define PAD_L2              0x0100

struct PAD { // 0x5c0
	/* 0x000 */ u128 pad_buf[16];
	/* 0x100 */ float analog[16];
	/* 0x140 */ float hudAnalog[16];
	/* 0x180 */ unsigned char profile[4];
	/* 0x184 */ unsigned char vib_profile[4];
	/* 0x188 */ unsigned char act_direct[4];
	/* 0x18c */ short int invalidTimer;
	/* 0x18e */ short int ringIndex;
	/* 0x190 */ int ringValidSize;
	/* 0x194 */ int socket;
	/* 0x198 */ int phase;
	/* 0x19c */ int state;
	/* 0x1a0 */ int bits;
	/* 0x1a4 */ int bitsOn;
	/* 0x1a8 */ int bitsOff;
	/* 0x1ac */ int bitsPrev;
	/* 0x1b0 */ int digitalBits;
	/* 0x1b4 */ int digitalBitsOn;
	/* 0x1b8 */ int digitalBitsOff;
	/* 0x1bc */ int digitalBitsPrev;
	/* 0x1c0 */ int hudBits;
	/* 0x1c4 */ int hudBitsOn;
	/* 0x1c8 */ int hudBitsOff;
	/* 0x1cc */ int hudDivert;
	/* 0x1d0 */ int handsOff;
	/* 0x1d4 */ int handsOffStick;
	/* 0x1d8 */ int useAnalog;
	/* 0x1dc */ int term_id;
	/* 0x1e0 */ int bitsOnRing[30];
	/* 0x258 */ float analogAngRing[30];
	/* 0x2d0 */ float analogMagRing[30];
	/* 0x348 */ int unmaskedBits;
	/* 0x34c */ int lagIndex;
	/* 0x350 */ int lagValidSize;
	/* 0x354 */ int bits_Lagged[7];
	/* 0x370 */ int digitalBits_Lagged[7];
	/* 0x38c */ float analog_Lagged[7][16];
	/* 0x54c */ u8 port;
	/* 0x54d */ u8 repeatDelay;
	/* 0x54e */ u8 repeatSpeed;
	/* 0x54f */ u8 repeatCounter;
	/* 0x550 */ u8 multi_tap_connected;
	/* 0x551 */ u8 disconnected;
	/* 0x552 */ unsigned char act_align[6];
	/* 0x558 */ u8 slot;
	/* 0x559 */ char initialized;
	/* 0x55c */ int rterm_id;
	/* 0x560 */ int id;
	/* 0x564 */ int exid;
	/* 0x568 */ int lagFrames;
	/* 0x56c */ void *RawPadInputCallback;
	/* 0x570 */ void *pCallbackData;
	/* 0x574 */ unsigned char rdata[32];
	/* 0x594 */ int ipad[10];
};

typedef struct padButtonStatus
{
    unsigned char ok;
    unsigned char mode;
    unsigned short btns;
    // joysticks
    unsigned char rjoy_h;
    unsigned char rjoy_v;
    unsigned char ljoy_h;
    unsigned char ljoy_v;
    // pressure mode
    unsigned char right_p;
    unsigned char left_p;
    unsigned char up_p;
    unsigned char down_p;
    unsigned char triangle_p;
    unsigned char circle_p;
    unsigned char cross_p;
    unsigned char square_p;
    unsigned char l1_p;
    unsigned char r1_p;
    unsigned char l2_p;
    unsigned char r2_p;
} __attribute__((packed)) PadButtonStatus;

typedef struct PadHistory
{
    u16 btns;
    u8 rjoy_h;
    u8 rjoy_v;
    u8 ljoy_h;
    u8 ljoy_v;
    short id;
} PadHistory;

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
__LIBUYA_GETTER__ int padGetButton(int port, u16 buttonMask);

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
__LIBUYA_GETTER__ int padGetButtonDown(int port, u16 buttonMask);

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
__LIBUYA_GETTER__ int padGetButtonUp(int port, u16 buttonMask);

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
__LIBUYA_SETTER__ void padResetInput(int port);

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
void padDisableInput(void);

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
void padEnableInput(void);

#endif // _LIBUYA_PAD_H
