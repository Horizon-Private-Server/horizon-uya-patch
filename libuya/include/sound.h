/***************************************************
 * FILENAME :		sound.h
 * DESCRIPTION :
 * 		Contains sound specific offsets and structures for Deadlocked.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
#ifndef _LIBUYA_SOUND_H_
#define _LIBUYA_SOUND_H_

#include "math3d.h"
#include "common.h"
#include "moby.h"
#include "player.h"
#include "spawnpoint.h"

#if UYA_PAL
#define SOUND_GLOBALS ((SoundGlobals*)0x001fe020)
#else
#define SOUND_GLOBALS ((SoundGlobals*)0x001fe1a0)
#endif

// All sounds were played in Bank 0
typedef enum SOUND_IDS {
	SOUND_ID_MENU_HELP_MESSAGE = 0,
	SOUND_ID_BOLTS = 1,
	SOUND_ID_MENU_CURSOR_MOVE = 2,
	SOUND_ID_MENU_SELECT_OKAY = 3,
	SOUND_ID_MENU_SELECT_DENIED = 4,
	SOUND_ID_WEAPON_UPGRADED = 5,
	SOUND_ID_MENU_OPEN = 7,
	SOUND_ID_MENU_SUBMENU_OPEN = 8,
	SOUND_ID_MENU_CLOSE = 9,
	SOUND_ID_MENU_POPUP_OPEN = 10,
	SOUND_ID_MENU_POPUP_CLOSE = 11,
	SOUND_ID_MENU_POPUP_SELECT = 13,
	SOUND_ID_DRONES_GOT = 145,
	SOUND_ID_DRONES_EXPLODE = 146,
	SOUND_ID_DRONES_SHOT = 148,
	SOUND_ID_BLITZ_SHOT = 172,
	SOUND_ID_GRAVITY_BOMB_SHOT = 183,
	SOUND_ID_FLUX_RIFLE_SHOT = 189,
	SOUND_ID_HEALTH_PAD_GOT = 246,
	SOUND_ID_AMMO_PAD_GOT = 247,
	SOUND_ID_OMNI_SHIELD_EQUIP = 255,
	SOUND_ID_OMNI_SHIELD_UNEQUIP = 256,
	SOUND_ID_OMNI_SHIELD_IDLE = 257,
	SOUND_ID_OMNI_SHIELD_ATTACK = 258,
	SOUND_ID_CTF_FLAG_PICKUP = 263,
	SOUND_ID_CTF_FLAG_DROP = 264,
	SOUND_ID_CTF_FLAG_RETURN = 265,
	SOUND_ID_CTF_FLAG_CAPTURED = 267,
	SOUND_ID_VEHICLE_RESPAWN = 270
	// nothing seems to be passed 270
} SOUND_IDS;

struct SoundInstance { // 0x90
	/* 0x00 */ short int oClass;
	/* 0x02 */ short int mClass;
	/* 0x04 */ void *pUpdate;
	/* 0x08 */ void *pVar;
	/* 0x0c */ float range;
	/* 0x10 */ struct Cuboid cuboid;
};

typedef struct _sound_data { // 0x80
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
} SoundData;

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
/* 0x0090 */ void *functions[10];
/* 0x00b8 */ SoundData channels[52];
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
