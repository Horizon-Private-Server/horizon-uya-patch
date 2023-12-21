#include "moby.h"

enum TrackNumbers
{
    // Default Online Tracks
    TRACK_ONLINE_LOBBY,
    TRACK_GAME_LOBBY,
    TRACK_VICTORY,
    TRACK_LOSS,
    TRACK_04,
    TRACK_05,
    TRACK_06,
    TRACK_07,
    TRACK_08,
    TRACK_MARCADIA_OPERATION_IRON,
    TRACK_0a,
    TRACK_0b,
    TRACK_0c,
    TRACK_0d,

    // Added Tracks if Single Player Music is activated
};

enum queuelenFlags
{
    QUEUELEN_PLAYING = 0x2,
    QUEUELEN_TRANSITIONING = 0x3,
    QUEUELEN_SWITCHING = 0xA,
};

enum flagsFlags
{
    FLAG_STOP_PLAYING_AFTER,
    FLAG_KEEP_PLAYING_AFTER,
};

enum unpauseFlags
{
    UNPAUSE_STOP_PLAYING_AFTER = 3,
    UNPAUSE_KEEP_PLAYING_AFTER = 4,
    UNPAUSE_PLAY_NEXT = 5,
    UNPAUSE_LOADING = 8, // This only happens at beginning of game.

};

typedef struct music_Playing { // 0x28
	/* 0x00 */ u32 handle;
	/* 0x04 */ short int track;
	/* 0x06 */ short int volume;
	/* 0x08 */ short int flags;
	/* 0x0a */ short int status;
	/* 0x0c */ short int pause;
	/* 0x0e */ short int unpause;
	/* 0x10 */ short int transition;
	/* 0x12 */ short int fade;
	/* 0x14 */ int queuelen;
	/* 0x18 */ int remain;
	/* 0x1c */ struct Moby * pMoby;
	/* 0x20 */ int channel;
	/* 0x24 */ int timer;
} music_Playing;

typedef struct music_Globals { // 0xe4
	/* 0x00 */ struct music_Track * trackPos;
	/* 0x04 */ short int loading;
	/* 0x06 */ char stop;
	/* 0x07 */ char off;
	/* 0x08 */ int lsn;
	/* 0x0c */ int * pCallbackFlag;
	/* 0x10 */ int len;
	/* 0x14 */ void * buff;
	/* 0x18 */ void * load_cb;
	/* 0x1c */ void * load_cb_data;
	/* 0x20 */ int pause;
	/* 0x24 */ int nextspeech_track;
	/* 0x28 */ struct Moby * nextspeech_moby;
	/* 0x2c */ short int transition;
	/* 0x2e */ short int pad0;
	/* 0x30 */ int transition_length;
	/* 0x34 */ int transition_fade_time;
	/* 0x38 */ int transition_delay;
	/* 0x3c */ short int transition_track;
	/* 0x3e */ short int transition_transition;
	/* 0x40 */ char trycount; // sceCdRMode struct
	/* 0x41 */ char spindlctrl;  // sceCdRMode struct
	/* 0x42 */ char datapattern;  // sceCdRMode struct
	/* 0x43 */ char pad;  // sceCdRMode struct
	/* 0x44 */ struct music_Playing play;
	/* 0x6c */ struct music_Playing speech;
	/* 0x94 */ struct music_Playing queued_speech;
	/* 0xbc */ struct music_Playing trans;
} music_Globals;

/*
 * NAME :		musicIsLoaded
 * 
 * DESCRIPTION : Returns music sector value.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int musicIsLoaded(void);

/*
 * NAME :		musicIsLoaded
 * 
 * DESCRIPTION :    Sets music sector value.
 * 
 * NOTES :
 * 
 * ARGS :           sector = new sector value.
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void musicSetSector(int sector);

/*
 * NAME :		musicGetNextTrack
 * 
 * DESCRIPTION : Returns the curent track info.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
music_Playing * musicGetTrackInfo(void);

/*
 * NAME :		musicGetNextTrack
 * 
 * DESCRIPTION : Returns the next track that will play.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int musicGetNextTrack(void);

/*
 * NAME :		musicGetTrackDuration
 * 
 * DESCRIPTION : Returns current track duration
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int musicGetTrackDuration(void);

/*
 * NAME :		musicGetTrackRangeMin
 * 
 * DESCRIPTION : Returns which track to start randomization
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int musicTrackRangeMin(void);

/*
 * NAME :		musicGetTrackRangeMax
 * 
 * DESCRIPTION : Returns the range of how many tracks to randomize through
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int musicTrackRangeMax(void);

void musicPlayTrack(int track, int flags, int volume);
void musicStopTrack(void);
int musicTransitionTrack(int track, int transition, int flags, int volume);
