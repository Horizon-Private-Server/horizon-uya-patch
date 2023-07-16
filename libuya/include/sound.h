/***************************************************
 * FILENAME :		sound.h
 * 
 * DESCRIPTION :
 * 		Contains sound specific offsets and structures for Deadlocked.
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_SOUND_H_
#define _LIBUYA_SOUND_H_

#include "common.h"
#include "moby.h"
#include "player.h"

void soundHeroPlay(Player * player, short sound, int flags);

void soundMobyPlay(short sound, int flags, Moby * moby);

void soundPlay(short sound, int flags, Moby * moby, VECTOR a3, int volume);

#endif // _LIBUYA_SOUND_H_
