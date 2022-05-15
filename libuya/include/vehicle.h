/***************************************************
 * FILENAME :		vehicle.h
 * 
 * DESCRIPTION :
 * 		Contains vehicle specific offsets and structures for Deadlocked.
 * 
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBDL_VEHICLE_H_
#define _LIBDL_VEHICLE_H_

#include "player.h"

/*
 * NAME :		Vehicle
 * 
 * DESCRIPTION :
 * 			Contains the vehicle data.
 * 
 * NOTES :
 *          This is very large and documentation is incomplete.
 *          Much of the padding is to skip over area's not yet understood.
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct Vehicle
{
    // Health and vehicle speed are located somewhere in here.
    // I was able to do a quick test and found both, didn't document though.
    char UNK0[0x33C];
} Vehicle;

#endif // _LIBDL_VEHICLE_H_
