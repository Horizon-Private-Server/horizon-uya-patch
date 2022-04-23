/***************************************************
 * FILENAME :		color.h
 * 
 * DESCRIPTION :
 * 		Contains color related function definitions found in Deadlocked.
 * 
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
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

#ifndef _LIBDL_COLOR_H_
#define _LIBDL_COLOR_H_

/*
 * NAME :		colorLerp
 * 
 * DESCRIPTION :
 * 			Interpolates the color from a to b by t.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          a     :           Start color.
 *          b     :           End color.
 *          t     :           Amount to interpolate by.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
u32 colorLerp(u32 a, u32 b, float t);

#endif // _LIBDL_COLOR_H_

