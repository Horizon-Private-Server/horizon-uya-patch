#include <tamtypes.h>
#include "string.h"
#include "spawnpoint.h"
#include "game.h"
#include "interop.h"

#define SPAWNPOINTS             ((Cuboid*)(*(u32*)GetAddress(&vaSpawnPointsPtr)))
#define SPAWNPOINTS_COUNT       ((Cuboid*)(*(u32*)((u32)GetAddress(&vaSpawnPointsPtr) + 0x4)))
#define SPLINES                 ((Paths*)*(*(u32*)((u32)GetAddress(&vaSpawnPointsPtr) + 0x38)))

variableAddressCompact_t vaSpawnPointsPtr = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00248500),
	.Hoven = VA_COMPACT_OFFSET(0x00248500, 0x00248700),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00248500, 0x00248600),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00248500, 0x00248480),
	.Metropolis = VA_COMPACT_OFFSET(0x00248500, 0x00248500),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00248500, 0x00248480),
	.CommandCenter = VA_COMPACT_OFFSET(0x00248500, 0x00248080),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00248500, 0x00248180),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00248500, 0x00248180),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00248500, 0x00248180),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00248680),
	.Hoven = VA_COMPACT_OFFSET(0x00248680, 0x00248880),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00248680, 0x00248780),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00248680, 0x00248600),
	.Metropolis = VA_COMPACT_OFFSET(0x00248680, 0x00248680),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00248680, 0x00248600),
	.CommandCenter = VA_COMPACT_OFFSET(0x00248680, 0x00248200),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00248680, 0x00248300),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00248680, 0x00248300),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00248680, 0x00248300),
#endif
};

variableAddressCompact_t vaGetRandomSpawnPointFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00544df0),
	.Hoven = VA_COMPACT_OFFSET(0x00544df0, 0x00546fb8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00544df0, 0x0053c890),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00544df0, 0x00539f78),
	.Metropolis = VA_COMPACT_OFFSET(0x00544df0, 0x00539378),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00544df0, 0x00536b60),
	.CommandCenter = VA_COMPACT_OFFSET(0x00544df0, 0x005363b8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00544df0, 0x00538c38),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00544df0, 0x00537f38),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00544df0, 0x005378b8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005424e0),
	.Hoven = VA_COMPACT_OFFSET(0x005424e0, 0x005445e8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005424e0, 0x00539f00),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005424e0, 0x00537668),
	.Metropolis = VA_COMPACT_OFFSET(0x005424e0, 0x00536a68),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005424e0, 0x005341d0),
	.CommandCenter = VA_COMPACT_OFFSET(0x005424e0, 0x00533c00),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005424e0, 0x00536440),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005424e0, 0x00535780),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005424e0, 0x005350c0),
#endif
};

int spawnPointGetCount(void)
{
    return SPAWNPOINTS_COUNT;
}

int spawnPointIsPlayer(int index)
{
  int i;
  GameData* gameData = gameGetData();
  for (i = 0; i < 64; ++i) {
    if (gameData->DeathMatchGameData->resurrectionPts[i] == index)
      return 1;
  }

  return 0;
}

Cuboid * spawnPointGet(int index)
{
   Cuboid *spawnPoints = SPAWNPOINTS;
    if (!spawnPoints)
        return NULL;

    return &spawnPoints[index];
}

void spawnPointSet(Cuboid *sp, int index)
{
    Cuboid *spawnPoints = SPAWNPOINTS;
    if (!spawnPoints)
        return;

    memcpy(&spawnPoints[index], sp, sizeof(Cuboid));
}
