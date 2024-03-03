/***************************************************
 * FILENAME :		time.h
 * DESCRIPTION :
 * 		Contains time related constants.
 * NOTES :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_TIME_H_
#define _LIBUYA_TIME_H_

#include <tamtypes.h>

#define TIME_SECOND             (1000)
#define TIME_MINUTE             (TIME_SECOND * 60)
#define SYSTEM_TIME_TICKS_PER_MS  (0x24000)

/*
 * NAME :		timerGetSystemTime
 * DESCRIPTION :
 * 			Returns system time.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
long timerGetSystemTime(void);

long timeDecTimer(int* time);
long timeDecTimerShort(short* time);

#endif // _LIBUYA_TIME_H_
