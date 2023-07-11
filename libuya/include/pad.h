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

struct PAD {
	/*   0 */ u128 pad_buf[16];
	/* 100 */ float analog[16];
	/* 140 */ float hudAnalog[16];
	/* 180 */ unsigned char profile[4];
	/* 184 */ unsigned char vib_profile[4];
	/* 188 */ unsigned char act_direct[4];
	/* 18c */ short int invalidTimer;
	/* 18e */ short int ringIndex;
	/* 190 */ int ringValidSize;
	/* 194 */ int socket;
	/* 198 */ int phase;
	/* 19c */ int state;
	/* 1a0 */ int bits;
	/* 1a4 */ int bitsOn;
	/* 1a8 */ int bitsOff;
	/* 1ac */ int bitsPrev;
	/* 1b0 */ int digitalBits;
	/* 1b4 */ int digitalBitsOn;
	/* 1b8 */ int digitalBitsOff;
	/* 1bc */ int digitalBitsPrev;
	/* 1c0 */ int hudBits;
	/* 1c4 */ int hudBitsOn;
	/* 1c8 */ int hudBitsOff;
	/* 1cc */ int hudDivert;
	/* 1d0 */ int handsOff;
	/* 1d4 */ int handsOffStick;
	/* 1d8 */ int useAnalog;
	/* 1dc */ int term_id;
	/* 1e0 */ int bitsOnRing[30];
	/* 258 */ float analogAngRing[30];
	/* 2d0 */ float analogMagRing[30];
	/* 348 */ int unmaskedBits;
	/* 34c */ int lagIndex;
	/* 350 */ int lagValidSize;
	/* 354 */ int bits_Lagged[7];
	/* 370 */ int digitalBits_Lagged[7];
	/* 38c */ float analog_Lagged[16][7];
	/* 54c */ u8 port;
	/* 54d */ u8 repeatDelay;
	/* 54e */ u8 repeatSpeed;
	/* 54f */ u8 repeatCounter;
	/* 550 */ u8 multi_tap_connected;
	/* 551 */ u8 disconnected;
	/* 552 */ unsigned char act_align[6];
	/* 558 */ u8 slot;
	/* 559 */ char initialized;
	/* 55c */ int rterm_id;
	/* 560 */ int id;
	/* 564 */ int exid;
	/* 568 */ int lagFrames;
	/* 56c */ void* RawPadInputCallback;
	/* 570 */ void* pCallbackData;
	/* 574 */ unsigned char rdata[32];
	/* 594 */ int ipad[10];
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
