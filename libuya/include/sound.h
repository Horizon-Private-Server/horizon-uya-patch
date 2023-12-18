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
#include "spawnpoint.h"

struct SoundInstance { // 0x90
	/* 0x00 */ short int oClass;
	/* 0x02 */ short int mClass;
	/* 0x04 */ void *pUpdate;
	/* 0x08 */ void *pVar;
	/* 0x0c */ float range;
	/* 0x10 */ struct Cuboid cuboid;
};

struct _sound_data { // 0x80
	/* 0x00 */ int handle;
	/* 0x04 */ struct SoundDef *def;
	/* 0x08 */ short int index;
	/* 0x0a */ short int def_index;
	/* 0x0c */ short int volumeMod;
	/* 0x0e */ u16 occlpos;
	/* 0x10 */ char status;
	/* 0x11 */ char loop;
	/* 0x12 */ char priority;
	/* 0x13 */ char unused0;
	/* 0x14 */ int pitch;
	/* 0x18 */ int pitchMod;
	/* 0x1c */ struct Moby *pMoby;
	/* 0x20 */ VECTOR pos;
	/* 0x30 */ VECTOR offset;
	/* 0x40 */ struct SoundInstance *pAmbient;
	/* 0x44 */ int lineEmitter;
	/* 0x48 */ int oClass;
	/* 0x4c */ float pan;
	/* 0x50 */ int delay;
	/* 0x54 */ int flags;
	/* 0x58 */ u16 unique_id;
	/* 0x5a */ u16 pad16_0;
	/* 0x5c */ char occl[36];
};

// Bakisi SoundGlobal: 0x001fe1a0
typedef struct _sound_globals { // 0x1d30
	/* 0x0000 */ int sfxVol;
	/* 0x0004 */ int musicVol;
	/* 0x0008 */ int dialogVol;
	/* 0x000c */ int ambientVol;
	/* 0x0010 */ int shellVol;
	/* 0x0014 */ int mpegVol;
	/* 0x0018 */ int pad32_0[2];
	/* 0x0020 */ int vol_backup[6];
	/* 0x0038 */ int pad32_1[2];
	/* 0x0040 */ char reverb_type;
	/* 0x0041 */ char reverb_delay;
	/* 0x0042 */ char reverb_feedback;
	/* 0x0043 */ char reverb_changed;
	/* 0x0044 */ struct Moby *occl_ignore;
	/* 0x0048 */ int cam_history_pos;
	/* 0x004c */ int load_busy;
	/* 0x0050 */ VECTOR cam_history[4];
	/* 0x0090 */ void *handles[132];
	/* 0x02a0 */ int cam_ofs_frame;
	/* 0x02a4 */ short int kludgeVolState;
	/* 0x02a6 */ short int kludgeVolTimer;
	/* 0x02a8 */ struct SoundInstance *pReverbOwner;
	/* 0x02ac */ int reverb_depth;
	/* 0x02b0 */ int ambientCnt;
	/* 0x02b4 */ struct SoundInstance *pAmbient;
	/* 0x02c0 */ struct _sound_data channels[52];
	/* 0x1cc0 */ VECTOR cam_ofs[6];
	/* 0x1d20 */ u32 volFlags;
	/* 0x1d24 */ int pad32_2[3];
} SoundGlobals;

typedef struct SoundDef { // 0x20
	/* 0x00 */ float minRange;
	/* 0x04 */ float maxRange;
	/* 0x08 */ int minVolume;
	/* 0x0c */ int maxVolume;
	/* 0x10 */ int minPitch;
	/* 0x14 */ int maxPitch;
	/* 0x18 */ char loop;
	/* 0x19 */ char flags;
	/* 0x1a */ short int index;
	/* 0x1c */ int bank_index;
} SoundDef;

void soundHeroPlay(Player* player, short sound, int flags);

void soundMobyPlay(short sound, int flags, Moby* moby);
void soundPlayByOClass(short sound, int flags, Moby* moby, short oClass);
void soundPlay(SoundDef* sound, int flags, Moby* moby, VECTOR position, int volume);

#endif // _LIBUYA_SOUND_H_
