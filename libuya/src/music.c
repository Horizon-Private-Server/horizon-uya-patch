#include "interop.h"
#include "music.h"

#if UYA_PAL
#define MUSIC_SECTOR                (0x001f8404)
#define TRACK_RANGE_MIN             (0x00243478)
#define TRACK_RANGE_MAX             (0x0024347c)
#define CURRENT_TRACK               ((music_Playing*)0x002256a4)
#else
#define MUSIC_SECTOR                (0x001F8584)
#define TRACK_RANGE_MIN             (0x002435B8)
#define TRACK_RANGE_MAX             (0x002435BC)
#define CURRENT_TRACK               ((music_Playing*)0x00225824)
#endif

VariableAddress_t vaPlayTrackFunc = {
#if UYA_PAL
	.Lobby = 0x005c1a60,
	.Bakisi = 0x00494258,
	.Hoven = 0x00496370,
	.OutpostX12 = 0x0048bc48,
    .KorgonOutpost = 0x00489318,
	.Metropolis = 0x00488730,
	.BlackwaterCity = 0x00485fc8,
	.CommandCenter = 0x00485fc0,
    .BlackwaterDocks = 0x00488840,
    .AquatosSewers = 0x00487b40,
    .MarcadiaPalace = 0x004874c0,
#else
	.Lobby = 0x005bfa18,
	.Bakisi = 0x00492268,
	.Hoven = 0x004942c0,
	.OutpostX12 = 0x00489bd8,
    .KorgonOutpost = 0x00487328,
	.Metropolis = 0x00486740,
	.BlackwaterCity = 0x00483f58,
	.CommandCenter = 0x00484110,
    .BlackwaterDocks = 0x00486950,
    .AquatosSewers = 0x00485c90,
    .MarcadiaPalace = 0x004855d0,
#endif
};

VariableAddress_t vaStopTrackFunc = {
#if UYA_PAL
	.Lobby = 0x005c1e10,
	.Bakisi = 0x00494608,
	.Hoven = 0x00496720,
	.OutpostX12 = 0x0048bff8,
    .KorgonOutpost = 0x004896c8,
	.Metropolis = 0x00488ae0,
	.BlackwaterCity = 0x00486378,
	.CommandCenter = 0x00486370,
    .BlackwaterDocks = 0x00488bf0,
    .AquatosSewers = 0x00487ef0,
    .MarcadiaPalace = 0x00487870,
#else
	.Lobby = 0x005bfdc8,
	.Bakisi = 0x00492618,
	.Hoven = 0x00494670,
	.OutpostX12 = 0x00489f88,
    .KorgonOutpost = 0x004876d8,
	.Metropolis = 0x00486af0,
	.BlackwaterCity = 0x00484308,
	.CommandCenter = 0x004844c0,
    .BlackwaterDocks = 0x004844c4,
    .AquatosSewers = 0x00486040,
    .MarcadiaPalace = 0x00485980,
#endif
};

VariableAddress_t vaTransitionTrackFunc = {
#if UYA_PAL
	.Lobby = 0x005c1cb8,
	.Bakisi = 0x004944b0,
	.Hoven = 0x004965c8,
	.OutpostX12 = 0x0048bea0,
    .KorgonOutpost = 0x00489570,
	.Metropolis = 0x00488988,
	.BlackwaterCity = 0x00486220,
	.CommandCenter = 0x00486218,
    .BlackwaterDocks = 0x00488a98,
    .AquatosSewers = 0x00487d98,
    .MarcadiaPalace = 0x00487718,
#else
	.Lobby = 0x005bfc70,
	.Bakisi = 0x004924c0,
	.Hoven = 0x00494518,
	.OutpostX12 = 0x00489e30,
    .KorgonOutpost = 0x00487580,
	.Metropolis = 0x00486998,
	.BlackwaterCity = 0x004841b0,
	.CommandCenter = 0x00484368,
    .BlackwaterDocks = 0x00486ba8,
    .AquatosSewers = 0x00485ee8,
    .MarcadiaPalace = 0x00485828,
#endif
};

int musicIsLoaded(void)
{
    return *(int*)MUSIC_SECTOR;
}

void musicSetSector(int sector)
{
    *(u32*)MUSIC_SECTOR = sector;
}

int musicGetCurrentTrack(void)
{
    static int CurrentTrack;
    static int NextTrack;
    if (CURRENT_TRACK->unpause == UNPAUSE_LOADING && CurrentTrack == 0 && NextTrack == 0 && CURRENT_TRACK->track != -1)
    {
        // Set CurrentTrack to Track.  This will make it so we know which was is currently playing.
        // The game automatically sets the track variable to the next track to play after the music starts.
        CurrentTrack = CURRENT_TRACK->track;
    }
    else if ((CURRENT_TRACK->unpause == UNPAUSE_KEEP_PLAYING_AFTER || CURRENT_TRACK->unpause == UNPAUSE_STOP_PLAYING_AFTER) && NextTrack == 0)
    {
        // Set NextTrack to Track value.
        NextTrack = CURRENT_TRACK->track;
    }
    // If NextTrack does not equal the Track, that means that the song has switched.
    // We need to move the NextTrack value into the CurrentTrack value, because it is now
    // playing that track.  Then we set the NextTrack to the Track value.
    else if (NextTrack != CURRENT_TRACK->track)
    {
        CurrentTrack = NextTrack;
        NextTrack = CURRENT_TRACK->track;
    }
    return CurrentTrack;
}

int musicGetNextTrack(void)
{
    return CURRENT_TRACK->track;
}

int musicGetTrackDuration(void)
{
    return CURRENT_TRACK->remain;
}

int musicTrackRangeMin(void)
{
    return TRACK_RANGE_MIN;
}

int musicTrackRangeMax(void)
{
    return TRACK_RANGE_MAX;
}
