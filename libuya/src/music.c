#include "interop.h"
#include "music.h"

#if UYA_PAL
#define MUSIC_SECTOR                (0x001f8404)
#define TRACK_RANGE_MIN             (0x00243478)
#define TRACK_RANGE_MAX             (0x0024347c)
#define MUSIC_GLOBALS               ((music_Globals*)0x00225660)
#else
#define MUSIC_SECTOR                (0x001F8584)
#define TRACK_RANGE_MIN             (0x002435B8)
#define TRACK_RANGE_MAX             (0x002435BC)
#define MUSIC_GLOBALS               ((music_Globals*)0x002257e0)
#endif


variableAddressCompact_t vaPlayTrackFunc = {
#if UYA_PAL
	.Lobby = 0x005c1ac0,
	.Bakisi = VA_COMPACT_BASE(0x004942b8),
	.Hoven = VA_COMPACT_OFFSET(0x004942b8, 0x004963d0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004942b8, 0x0048bca8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004942b8, 0x00489378),
	.Metropolis = VA_COMPACT_OFFSET(0x004942b8, 0x00488790),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004942b8, 0x00486028),
	.CommandCenter = VA_COMPACT_OFFSET(0x004942b8, 0x00486020),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004942b8, 0x004888a0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004942b8, 0x00487ba0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004942b8, 0x00487520),
#else
	.Lobby = 0x005bfa78,
	.Bakisi = VA_COMPACT_BASE(0x004922c8),
	.Hoven = VA_COMPACT_OFFSET(0x004922c8, 0x00494320),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004922c8, 0x00489c38),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004922c8, 0x00487388),
	.Metropolis = VA_COMPACT_OFFSET(0x004922c8, 0x004867a0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004922c8, 0x00483fb8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004922c8, 0x00484170),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004922c8, 0x004869b0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004922c8, 0x00485cf0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004922c8, 0x00485630),
#endif
};

variableAddressCompact_t vaStopTrackFunc = {
#if UYA_PAL
	.Lobby = 0x005c1e10,
	.Bakisi = VA_COMPACT_BASE(0x00494608),
	.Hoven = VA_COMPACT_OFFSET(0x00494608, 0x00496720),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00494608, 0x0048bff8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00494608, 0x004896c8),
	.Metropolis = VA_COMPACT_OFFSET(0x00494608, 0x00488ae0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00494608, 0x00486378),
	.CommandCenter = VA_COMPACT_OFFSET(0x00494608, 0x00486370),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00494608, 0x00488bf0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00494608, 0x00487ef0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00494608, 0x00487870),
#else
	.Lobby = 0x005bfdc8,
	.Bakisi = VA_COMPACT_BASE(0x00492618),
	.Hoven = VA_COMPACT_OFFSET(0x00492618, 0x00494670),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00492618, 0x00489f88),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00492618, 0x004876d8),
	.Metropolis = VA_COMPACT_OFFSET(0x00492618, 0x00486af0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00492618, 0x00484308),
	.CommandCenter = VA_COMPACT_OFFSET(0x00492618, 0x004844c0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00492618, 0x004844c4),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00492618, 0x00486040),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00492618, 0x00485980),
#endif
};

variableAddressCompact_t vaTransitionTrackFunc = {
#if UYA_PAL
	.Lobby = 0x005c1cb8,
	.Bakisi = VA_COMPACT_BASE(0x004944b0),
	.Hoven = VA_COMPACT_OFFSET(0x004944b0, 0x004965c8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004944b0, 0x0048bea0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004944b0, 0x00489570),
	.Metropolis = VA_COMPACT_OFFSET(0x004944b0, 0x00488988),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004944b0, 0x00486220),
	.CommandCenter = VA_COMPACT_OFFSET(0x004944b0, 0x00486218),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004944b0, 0x00488a98),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004944b0, 0x00487d98),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004944b0, 0x00487718),
#else
	.Lobby = 0x005bfc70,
	.Bakisi = VA_COMPACT_BASE(0x004924c0),
	.Hoven = VA_COMPACT_OFFSET(0x004924c0, 0x00494518),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004924c0, 0x00489e30),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004924c0, 0x00487580),
	.Metropolis = VA_COMPACT_OFFSET(0x004924c0, 0x00486998),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004924c0, 0x004841b0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004924c0, 0x00484368),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004924c0, 0x00486ba8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004924c0, 0x00485ee8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004924c0, 0x00485828),
#endif
};

int musicGetSector(void)
{
    return *(int*)MUSIC_SECTOR;
}

void musicSetSector(int sector)
{
    *(u32*)MUSIC_SECTOR = sector;
}

music_Globals *musicGetGlobals(void)
{
    return MUSIC_GLOBALS;
}

int musicGetNextTrack(void)
{
    return MUSIC_GLOBALS->play.track;
}

int musicGetTrackDuration(void)
{
    return MUSIC_GLOBALS->play.remain;
}

int musicTrackRangeMin(void)
{
    return TRACK_RANGE_MIN;
}

int musicTrackRangeMax(void)
{
    return TRACK_RANGE_MAX;
}

/*
 * NAME : musicGetTrackTable
 * DESCRIPTION : Return the native stereo-track descriptor table for this region.
 * NOTES : Entries contain two eight-byte channel descriptors.
 * ARGS :
 * RETURN : Native music-table address.
 * AUTHOR : Troy "Metroynome" Pruitt
 */
u32* musicGetTrackTable(void)
{
	return (u32*)(MUSIC_SECTOR + 4);
}

variableAddressCompact_t vaQueueTrackFunc = {
#if UYA_PAL
	.Lobby = 0x005c1da0,
	.Bakisi = VA_COMPACT_BASE(0x00494598),
	.Hoven = VA_COMPACT_OFFSET(0x00494598, 0x004966b0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00494598, 0x0048bf88),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00494598, 0x00489658),
	.Metropolis = VA_COMPACT_OFFSET(0x00494598, 0x00488a70),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00494598, 0x00486308),
	.CommandCenter = VA_COMPACT_OFFSET(0x00494598, 0x00486300),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00494598, 0x00488b80),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00494598, 0x00487e80),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00494598, 0x00487800),
#else
	.Lobby = 0x005bfd58,
	.Bakisi = VA_COMPACT_BASE(0x004925a8),
	.Hoven = VA_COMPACT_OFFSET(0x004925a8, 0x00494600),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004925a8, 0x00489f18),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004925a8, 0x00487668),
	.Metropolis = VA_COMPACT_OFFSET(0x004925a8, 0x00486a80),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004925a8, 0x00484298),
	.CommandCenter = VA_COMPACT_OFFSET(0x004925a8, 0x00484450),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004925a8, 0x00486c90),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004925a8, 0x00485fd0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004925a8, 0x00485910),
#endif
};
