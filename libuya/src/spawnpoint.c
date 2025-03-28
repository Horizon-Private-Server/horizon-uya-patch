#include <tamtypes.h>
#include "string.h"
#include "spawnpoint.h"
#include "game.h"
#include "interop.h"

#define SPAWNPOINTS             ((SpawnPoint*)(*(u32*)GetAddress(&vaSpawnPointsPtr)))
#define SPAWNPOINTS_COUNT       ((SpawnPoint*)(*(u32*)((u32)GetAddress(&vaSpawnPointsPtr) + 0x4)))


VariableAddress_t vaSpawnPointsPtr = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00248500,
	.Hoven = 0x00248700,
	.OutpostX12 = 0x00248600,
    .KorgonOutpost = 0x00248480,
	.Metropolis = 0x00248500,
	.BlackwaterCity = 0x00248480,
	.CommandCenter = 0x00248080,
    .BlackwaterDocks = 0x00248180,
    .AquatosSewers = 0x00248180,
    .MarcadiaPalace = 0x00248180,
#else
	.Lobby = 0,
	.Bakisi = 0x00248680                                     ,
	.Hoven = 0x00248880,
	.OutpostX12 = 0x00248780,
    .KorgonOutpost = 0x00248600,
	.Metropolis = 0x00248680,
	.BlackwaterCity = 0x00248600,
	.CommandCenter = 0x00248200,
    .BlackwaterDocks = 0x00248300,
    .AquatosSewers = 0x00248300,
    .MarcadiaPalace = 0x00248300,
#endif
};

VariableAddress_t vaGetRandomSpawnPointFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00544df0,
	.Hoven = 0x00546fb8,
	.OutpostX12 = 0x0053c890,
    .KorgonOutpost = 0x00539f78,
	.Metropolis = 0x00539378,
	.BlackwaterCity = 0x00536b60,
	.CommandCenter = 0x005363b8,
    .BlackwaterDocks = 0x00538c38,
    .AquatosSewers = 0x00537f38,
    .MarcadiaPalace = 0x005378b8,
#else
	.Lobby = 0,
	.Bakisi = 0x005424e0,
	.Hoven = 0x005445e8,
	.OutpostX12 = 0x00539f00,
    .KorgonOutpost = 0x00537668,
	.Metropolis = 0x00536a68,
	.BlackwaterCity = 0x005341d0,
	.CommandCenter = 0x00533c00,
    .BlackwaterDocks = 0x00536440,
    .AquatosSewers = 0x00535780,
    .MarcadiaPalace = 0x005350c0,
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
  for (i = 0; i < 64; ++i)
  {
    if (gameData->DeathMatchGameData->resurrectionPts[i] == index)
      return 1;
  }

  return 0;
}

SpawnPoint * spawnPointGet(int index)
{
    SpawnPoint * spawnPoints = SPAWNPOINTS;
    if (!spawnPoints)
        return NULL;

    return &spawnPoints[index];
}

void spawnPointSet(SpawnPoint * sp, int index)
{
    SpawnPoint * spawnPoints = SPAWNPOINTS;
    if (!spawnPoints)
        return;

    memcpy(&spawnPoints[index], sp, sizeof(SpawnPoint));
}
