#include <tamtypes.h>
#include "interop.h"
#include "moby.h"
#include "collision.h"

#define COLL_MOBY_LIST ((Moby**)GetAddress(&vaCollMobyList))
#define COLLOUTPUT ((CollOutput*)GetAddress(&vaCollOutput))

VariableAddress_t vaCollOutput = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0025b980,
	.Hoven = 0x0025bac0,
	.OutpostX12 = 0x0025b9c0,
	.KorgonOutpost = 0x0025b840,
	.Metropolis = 0x0025b880,
	.BlackwaterCity = 0x0025b840,
	.CommandCenter = 0x0025b400,
	.BlackwaterDocks = 0x0025b540,
	.AquatosSewers = 0x0025b580,
	.MarcadiaPalace = 0x0025b500,
#else
	.Lobby = 0,
	.Bakisi = 0x0025bb00,
	.Hoven = 0x0025bc40,
	.OutpostX12 = 0x0025bb40,
	.KorgonOutpost = 0x0025b9c0,
	.Metropolis = 0x0025ba00,
	.BlackwaterCity = 0x0025b9c0,
	.CommandCenter = 0x0025b580,
	.BlackwaterDocks = 0x0025b6c0,
	.AquatosSewers = 0x0025b700,
	.MarcadiaPalace = 0x0025b680,
#endif
};

VariableAddress_t vaCollMobyList = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0025fa00,
	.Hoven = 0x0025fb40,
	.OutpostX12 = 0x0025fa40,
	.KorgonOutpost = 0x0025f8c0,
	.Metropolis = 0x0025f900,
	.BlackwaterCity = 0x0025f8c0,
	.CommandCenter = 0x0025f480,
	.BlackwaterDocks = 0x0025f5c0,
	.AquatosSewers = 0x0025f600,
	.MarcadiaPalace = 0x0025f580,
#else
	.Lobby = 0,
	.Bakisi = 0x0025fb80,
	.Hoven = 0x0025fcc0,
	.OutpostX12 = 0x0025fbc0,
	.KorgonOutpost = 0x0025fa40,
	.Metropolis = 0x0025fa80,
	.BlackwaterCity = 0x0025fa40,
	.CommandCenter = 0x0025f600,
	.BlackwaterDocks = 0x0025f740,
	.AquatosSewers = 0x0025f780,
	.MarcadiaPalace = 0x0025f700,
#endif
};

VariableAddress_t vaCollLineFix = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0044d138,
	.Hoven = 0x0044ecb8,
	.OutpostX12 = 0x00445ab8,
	.KorgonOutpost = 0x00443678,
	.Metropolis = 0x004429b8,
	.BlackwaterCity = 0x004401b8,
	.CommandCenter = 0x00440e38,
	.BlackwaterDocks = 0x004436b8,
	.AquatosSewers = 0x004429b8,
	.MarcadiaPalace = 0x00442338,
#else
	.Lobby = 0,
	.Bakisi = 0x0044c290,
	.Hoven = 0x0044dd50,
	.OutpostX12 = 0x00444b90,
	.KorgonOutpost = 0x004427d0,
	.Metropolis = 0x00441b10,
	.BlackwaterCity = 0x0043f290,
	.CommandCenter = 0x004400d0,
	.BlackwaterDocks = 0x00442910,
	.AquatosSewers = 0x00441c50,
	.MarcadiaPalace = 0x00441590,
#endif
};

VariableAddress_t vaCollHotspot = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00451420,
	.Hoven = 0x00452fa0,
	.OutpostX12 = 0x00449da0,
	.KorgonOutpost = 0x00447960,
	.Metropolis = 0x00446ca0,
	.BlackwaterCity = 0x004444a0,
	.CommandCenter = 0x00445120,
	.BlackwaterDocks = 0x004479a0,
	.AquatosSewers = 0x00446ca0,
	.MarcadiaPalace = 0x00446620,
#else
	.Lobby = 0,
	.Bakisi = 0x00450578,
	.Hoven = 0x00452038,
	.OutpostX12 = 0x00448e78,
	.KorgonOutpost = 0x00446ab8,
	.Metropolis = 0x00445df8,
	.BlackwaterCity = 0x00443578,
	.CommandCenter = 0x004443b8,
	.BlackwaterDocks = 0x00446bf8,
	.AquatosSewers = 0x00445f38,
	.MarcadiaPalace = 0x00445878,
#endif
};

VariableAddress_t vaCollMobysSphereFix = {
#if UYA_PAL
	.Lobby = 0x00582fa8,
	.Bakisi = 0x0044fec0,
	.Hoven = 0x00451a40,
	.OutpostX12 = 0x00448840,
	.KorgonOutpost = 0x00446400,
	.Metropolis = 0x00445740,
	.BlackwaterCity = 0x00442f40,
	.CommandCenter = 0x00443bc0,
	.BlackwaterDocks = 0x00446440,
	.AquatosSewers = 0x00445740,
	.MarcadiaPalace = 0x004450c0,
#else
	.Lobby = 0x00582000,
	.Bakisi = 0x0044f018,
	.Hoven = 0x00450ad8,
	.OutpostX12 = 0x00447918,
	.KorgonOutpost = 0x00445558,
	.Metropolis = 0x00444898,
	.BlackwaterCity = 0x00442018,
	.CommandCenter = 0x00442e58,
	.BlackwaterDocks = 0x00445698,
	.AquatosSewers = 0x004449d8,
	.MarcadiaPalace = 0x00444318,
#endif
};

VariableAddress_t vaCollMovingSphere = {
#if UYA_PAL
	.Lobby = 0x005b6490,
	.Bakisi = 0x00482fc8,
	.Hoven = 0x00484b88,
	.OutpostX12 = 0x0047b988,
	.KorgonOutpost = 0x00479508,
	.Metropolis = 0x00478848,
	.BlackwaterCity = 0x00475e00,
	.CommandCenter = 0x00476948,
	.BlackwaterDocks = 0x004791c8,
	.AquatosSewers = 0x004784c8,
	.MarcadiaPalace = 0x00477e48,
#else
	.Lobby = 0x005b4558,
	.Bakisi = 0x004811d0,
	.Hoven = 0x00482cd0,
	.OutpostX12 = 0x00479b10,
	.KorgonOutpost = 0x00477710,
	.Metropolis = 0x00476a50,
	.BlackwaterCity = 0x00473f88,
	.CommandCenter = 0x00474c90,
	.BlackwaterDocks = 0x004774d0,
	.AquatosSewers = 0x00476810,
	.MarcadiaPalace = 0x00476150,
#endif
};

VariableAddress_t vaCollHeroSphere = {
#if UYA_PAL
	.Lobby = 0x005838d0,
	.Bakisi = 0x004507e8,
	.Hoven = 0x00452368,
	.OutpostX12 = 0x00449168,
	.KorgonOutpost = 0x00446d28,
	.Metropolis = 0x00446068,
	.BlackwaterCity = 0x00443868,
	.CommandCenter = 0x004444e8,
	.BlackwaterDocks = 0x00446d68,
	.AquatosSewers = 0x00446068,
	.MarcadiaPalace = 0x004459e8,
#else
	.Lobby = 0x00582928,
	.Bakisi = 0x0044f940,
	.Hoven = 0x00451400,
	.OutpostX12 = 0x00448240,
	.KorgonOutpost = 0x00445e80,
	.Metropolis = 0x004451c0,
	.BlackwaterCity = 0x00442940,
	.CommandCenter = 0x00443780,
	.BlackwaterDocks = 0x00445fc0,
	.AquatosSewers = 0x00445300,
	.MarcadiaPalace = 0x00444c40,
#endif
};

VariableAddress_t vaCollsphereFix = {
#if UYA_PAL
	.Lobby = 0x00581270,
	.Bakisi = 0x0044e188,
	.Hoven = 0x0044fd08,
	.OutpostX12 = 0x00446b08,
	.KorgonOutpost = 0x004446c8,
	.Metropolis = 0x00443a08,
	.BlackwaterCity = 0x00441208,
	.CommandCenter = 0x00441e88,
	.BlackwaterDocks = 0x00444708,
	.AquatosSewers = 0x00443a08,
	.MarcadiaPalace = 0x00443388,
#else
	.Lobby = 0x005802c8,
	.Bakisi = 0x0044d2e0,
	.Hoven = 0x0044eda0,
	.OutpostX12 = 0x00445be0,
	.KorgonOutpost = 0x00443820,
	.Metropolis = 0x00442b60,
	.BlackwaterCity = 0x004402e0,
	.CommandCenter = 0x00441120,
	.BlackwaterDocks = 0x00443960,
	.AquatosSewers = 0x00442ca0,
	.MarcadiaPalace = 0x004425e0,
#endif
};

VariableAddress_t vaCollLineList = {
#if UYA_PAL
	.Lobby = 0x00583e18,
	.Bakisi = 0x00450d30,
	.Hoven = 0x004528b0,
	.OutpostX12 = 0x004496b0,
	.KorgonOutpost = 0x00447270,
	.Metropolis = 0x004465b0,
	.BlackwaterCity = 0x00443db0,
	.CommandCenter = 0x00444a30,
	.BlackwaterDocks = 0x004472b0,
	.AquatosSewers = 0x004465b0,
	.MarcadiaPalace = 0x00445f30,
#else
	.Lobby = 0x00582e70,
	.Bakisi = 0x0044fe88,
	.Hoven = 0x00451948,
	.OutpostX12 = 0x00448788,
	.KorgonOutpost = 0x004463c8,
	.Metropolis = 0x00445708,
	.BlackwaterCity = 0x00442e88,
	.CommandCenter = 0x00443cc8,
	.BlackwaterDocks = 0x00446508,
	.AquatosSewers = 0x00445848,
	.MarcadiaPalace = 0x00445188,
#endif
};

Moby** CollMobysSphere_Fix_GetHitMobies(void)
{
  return COLL_MOBY_LIST;
}

CollOutput* colGetOutput(void)
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
