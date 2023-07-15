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

int spawnPointGetCount(void)
{
    return SPAWNPOINTS_COUNT;
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
