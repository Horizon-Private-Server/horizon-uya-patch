#include <tamtypes.h>
#include "interop.h"
#include "moby.h"
#include "collision.h"

#define COLL_MOBY_LIST ((Moby**)GetAddress(&vaCollMobyList))
#define COLLOUTPUT ((CollOutput*)GetAddress(&vaCollOutput))

variableAddressCompact_t vaCollOutput = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x0025b980,
	.Hoven = VA_COMPACT_OFFSET(0x0025b980, 0x0025bac0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0025b980, 0x0025b9c0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0025b980, 0x0025b840),
	.Metropolis = VA_COMPACT_OFFSET(0x0025b980, 0x0025b880),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0025b980, 0x0025b840),
	.CommandCenter = VA_COMPACT_OFFSET(0x0025b980, 0x0025b400),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0025b980, 0x0025b540),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0025b980, 0x0025b580),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0025b980, 0x0025b500),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x0025bb00,
	.Hoven = VA_COMPACT_OFFSET(0x0025bb00, 0x0025bc40),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0025bb00, 0x0025bb40),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0025bb00, 0x0025b9c0),
	.Metropolis = VA_COMPACT_OFFSET(0x0025bb00, 0x0025ba00),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0025bb00, 0x0025b9c0),
	.CommandCenter = VA_COMPACT_OFFSET(0x0025bb00, 0x0025b580),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0025bb00, 0x0025b6c0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0025bb00, 0x0025b700),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0025bb00, 0x0025b680),
#endif
};

variableAddressCompact_t vaCollMobyList = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x0025fa00,
	.Hoven = VA_COMPACT_OFFSET(0x0025fa00, 0x0025fb40),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0025fa00, 0x0025fa40),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0025fa00, 0x0025f8c0),
	.Metropolis = VA_COMPACT_OFFSET(0x0025fa00, 0x0025f900),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0025fa00, 0x0025f8c0),
	.CommandCenter = VA_COMPACT_OFFSET(0x0025fa00, 0x0025f480),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0025fa00, 0x0025f5c0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0025fa00, 0x0025f600),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0025fa00, 0x0025f580),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x0025fb80,
	.Hoven = VA_COMPACT_OFFSET(0x0025fb80, 0x0025fcc0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0025fb80, 0x0025fbc0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0025fb80, 0x0025fa40),
	.Metropolis = VA_COMPACT_OFFSET(0x0025fb80, 0x0025fa80),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0025fb80, 0x0025fa40),
	.CommandCenter = VA_COMPACT_OFFSET(0x0025fb80, 0x0025f600),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0025fb80, 0x0025f740),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0025fb80, 0x0025f780),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0025fb80, 0x0025f700),
#endif
};

variableAddressCompact_t vaCollLineFix = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x0044d138,
	.Hoven = VA_COMPACT_OFFSET(0x0044d138, 0x0044ecb8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044d138, 0x00445ab8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044d138, 0x00443678),
	.Metropolis = VA_COMPACT_OFFSET(0x0044d138, 0x004429b8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044d138, 0x004401b8),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044d138, 0x00440e38),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044d138, 0x004436b8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044d138, 0x004429b8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044d138, 0x00442338),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x0044c290,
	.Hoven = VA_COMPACT_OFFSET(0x0044c290, 0x0044dd50),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044c290, 0x00444b90),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044c290, 0x004427d0),
	.Metropolis = VA_COMPACT_OFFSET(0x0044c290, 0x00441b10),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044c290, 0x0043f290),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044c290, 0x004400d0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044c290, 0x00442910),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044c290, 0x00441c50),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044c290, 0x00441590),
#endif
};

variableAddressCompact_t vaCollHotspot = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00451420,
	.Hoven = VA_COMPACT_OFFSET(0x00451420, 0x00452fa0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00451420, 0x00449da0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00451420, 0x00447960),
	.Metropolis = VA_COMPACT_OFFSET(0x00451420, 0x00446ca0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00451420, 0x004444a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00451420, 0x00445120),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00451420, 0x004479a0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00451420, 0x00446ca0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00451420, 0x00446620),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00450578,
	.Hoven = VA_COMPACT_OFFSET(0x00450578, 0x00452038),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00450578, 0x00448e78),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00450578, 0x00446ab8),
	.Metropolis = VA_COMPACT_OFFSET(0x00450578, 0x00445df8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00450578, 0x00443578),
	.CommandCenter = VA_COMPACT_OFFSET(0x00450578, 0x004443b8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00450578, 0x00446bf8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00450578, 0x00445f38),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00450578, 0x00445878),
#endif
};

variableAddressCompact_t vaCollMobysSphereFix = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x00582fa8),
	.Bakisi = 0x0044fec0,
	.Hoven = VA_COMPACT_OFFSET(0x0044fec0, 0x00451a40),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044fec0, 0x00448840),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044fec0, 0x00446400),
	.Metropolis = VA_COMPACT_OFFSET(0x0044fec0, 0x00445740),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044fec0, 0x00442f40),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044fec0, 0x00443bc0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044fec0, 0x00446440),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044fec0, 0x00445740),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044fec0, 0x004450c0),
#else
	.Lobby = VA_COMPACT_LOBBY(0x00582000),
	.Bakisi = 0x0044f018,
	.Hoven = VA_COMPACT_OFFSET(0x0044f018, 0x00450ad8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044f018, 0x00447918),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044f018, 0x00445558),
	.Metropolis = VA_COMPACT_OFFSET(0x0044f018, 0x00444898),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044f018, 0x00442018),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044f018, 0x00442e58),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044f018, 0x00445698),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044f018, 0x004449d8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044f018, 0x00444318),
#endif
};

variableAddressCompact_t vaCollMovingSphere = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x005b6490),
	.Bakisi = 0x00482fc8,
	.Hoven = VA_COMPACT_OFFSET(0x00482fc8, 0x00484b88),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00482fc8, 0x0047b988),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00482fc8, 0x00479508),
	.Metropolis = VA_COMPACT_OFFSET(0x00482fc8, 0x00478848),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00482fc8, 0x00475e00),
	.CommandCenter = VA_COMPACT_OFFSET(0x00482fc8, 0x00476948),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00482fc8, 0x004791c8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00482fc8, 0x004784c8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00482fc8, 0x00477e48),
#else
	.Lobby = VA_COMPACT_LOBBY(0x005b4558),
	.Bakisi = 0x004811d0,
	.Hoven = VA_COMPACT_OFFSET(0x004811d0, 0x00482cd0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004811d0, 0x00479b10),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004811d0, 0x00477710),
	.Metropolis = VA_COMPACT_OFFSET(0x004811d0, 0x00476a50),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004811d0, 0x00473f88),
	.CommandCenter = VA_COMPACT_OFFSET(0x004811d0, 0x00474c90),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004811d0, 0x004774d0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004811d0, 0x00476810),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004811d0, 0x00476150),
#endif
};

variableAddressCompact_t vaCollHeroSphere = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x005838d0),
	.Bakisi = 0x004507e8,
	.Hoven = VA_COMPACT_OFFSET(0x004507e8, 0x00452368),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004507e8, 0x00449168),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004507e8, 0x00446d28),
	.Metropolis = VA_COMPACT_OFFSET(0x004507e8, 0x00446068),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004507e8, 0x00443868),
	.CommandCenter = VA_COMPACT_OFFSET(0x004507e8, 0x004444e8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004507e8, 0x00446d68),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004507e8, 0x00446068),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004507e8, 0x004459e8),
#else
	.Lobby = VA_COMPACT_LOBBY(0x00582928),
	.Bakisi = 0x0044f940,
	.Hoven = VA_COMPACT_OFFSET(0x0044f940, 0x00451400),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044f940, 0x00448240),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044f940, 0x00445e80),
	.Metropolis = VA_COMPACT_OFFSET(0x0044f940, 0x004451c0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044f940, 0x00442940),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044f940, 0x00443780),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044f940, 0x00445fc0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044f940, 0x00445300),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044f940, 0x00444c40),
#endif
};

variableAddressCompact_t vaCollsphereFix = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x00581270),
	.Bakisi = 0x0044e188,
	.Hoven = VA_COMPACT_OFFSET(0x0044e188, 0x0044fd08),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044e188, 0x00446b08),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044e188, 0x004446c8),
	.Metropolis = VA_COMPACT_OFFSET(0x0044e188, 0x00443a08),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044e188, 0x00441208),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044e188, 0x00441e88),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044e188, 0x00444708),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044e188, 0x00443a08),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044e188, 0x00443388),
#else
	.Lobby = VA_COMPACT_LOBBY(0x005802c8),
	.Bakisi = 0x0044d2e0,
	.Hoven = VA_COMPACT_OFFSET(0x0044d2e0, 0x0044eda0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044d2e0, 0x00445be0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044d2e0, 0x00443820),
	.Metropolis = VA_COMPACT_OFFSET(0x0044d2e0, 0x00442b60),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044d2e0, 0x004402e0),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044d2e0, 0x00441120),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044d2e0, 0x00443960),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044d2e0, 0x00442ca0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044d2e0, 0x004425e0),
#endif
};

variableAddressCompact_t vaCollLineList = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x00583e18),
	.Bakisi = 0x00450d30,
	.Hoven = VA_COMPACT_OFFSET(0x00450d30, 0x004528b0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00450d30, 0x004496b0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00450d30, 0x00447270),
	.Metropolis = VA_COMPACT_OFFSET(0x00450d30, 0x004465b0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00450d30, 0x00443db0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00450d30, 0x00444a30),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00450d30, 0x004472b0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00450d30, 0x004465b0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00450d30, 0x00445f30),
#else
	.Lobby = VA_COMPACT_LOBBY(0x00582e70),
	.Bakisi = 0x0044fe88,
	.Hoven = VA_COMPACT_OFFSET(0x0044fe88, 0x00451948),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044fe88, 0x00448788),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044fe88, 0x004463c8),
	.Metropolis = VA_COMPACT_OFFSET(0x0044fe88, 0x00445708),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044fe88, 0x00442e88),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044fe88, 0x00443cc8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044fe88, 0x00446508),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044fe88, 0x00445848),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044fe88, 0x00445188),
#endif
};

Moby** CollMobysSphere_Fix_GetHitMobies(void)
{
  return COLL_MOBY_LIST;
}

CollOutput* collGetOutput(void)
{
	return COLLOUTPUT;
}

Moby* CollLine_Fix_GetHitMoby(void)
{
	return COLLOUTPUT->pMoby;
}

float* CollLine_Fix_GetHitPosition(void)
{
	return COLLOUTPUT->ip;
}

float* CollLine_Fix_GetHitNormal(void)
{
	return COLLOUTPUT->normal;
}

int CollLine_Fix_GetHitCollisionId(void)
{
	return COLLOUTPUT->poly;
}

int CollSoundSurface(void)
{
	int poly = COLLOUTPUT->poly;
    if (poly < 0)
    	return -1;

    return (poly >> 5) & 0x3;
}
