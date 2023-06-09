/***************************************************
 * FILENAME :		cheats.c
 * 
 * DESCRIPTION :
 * 		Contains simple cheats/game settings for UYA.
 * 
 * NOTES :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>

#include <libuya/string.h>
#include <libuya/player.h>
#include <libuya/utils.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/pad.h>
#include <libuya/stdio.h>
#include <libuya/uya.h>
#include <libuya/math3d.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include <libuya/time.h>
#include <libuya/interop.h>
#include <libuya/net.h>
#include "module.h"
#include "messageid.h"
#include "config.h"
#include "include/config.h"


/*
 * NAME :		disableWeaponPacks
 * 
 * DESCRIPTION :
 * 
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void disableWeaponPacks(void)
{
  VariableAddress_t vaWeaponPackSpawnFunc = {
#if UYA_PAL
    .Lobby = 0,
	  .Bakisi = 0x004FA350,
	  .Hoven = 0x004FC468,
	  .OutpostX12 = 0x004F1D40,
    .KorgonOutpost = 0x004EF4D8,
	  .Metropolis = 0x004EE828,
	  .BlackwaterCity = 0x004EC0C0,
	  .CommandCenter = 0x004EC088,
    .BlackwaterDocks = 0x004EE908,
    .AquatosSewers = 0x004EDC08,
    .MarcadiaPalace = 0x004ED588,
#else
    .Lobby = 0,
	  .Bakisi = 0x004F7BD0,
	  .Hoven = 0x004F9C28,
	  .OutpostX12 = 0x004EF540,
    .KorgonOutpost = 0x004ECD58,
	  .Metropolis = 0x004EC0A8,
	  .BlackwaterCity = 0x004E98C0,
	  .CommandCenter = 0x004E9A48,
    .BlackwaterDocks = 0x004EC288,
    .AquatosSewers = 0x004EB5C8,
    .MarcadiaPalace = 0x004EAF08,
#endif
  };

  u32 weaponPackSpawnFunc = GetAddress(&vaWeaponPackSpawnFunc);
  if (weaponPackSpawnFunc) {
    *(u32*)weaponPackSpawnFunc = 0;
    *(u32*)(weaponPackSpawnFunc - 0x7BF4) = 0;
  }
}

/*
 * NAME :		spawnWeaponPackOnDeath
 * 
 * DESCRIPTION :
 * 
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Agent Moose" Pruitt
 */

char SpawnedPack = 0;
void SpawnPack(u32 a0, u32 a1)
{
  VariableAddress_t vaRespawnFunc = {
#if UYA_PAL
    .Lobby = 0,
	  .Bakisi = 0x0051a940,
	  .Hoven = 0x0051ca58,
	  .OutpostX12 = 0x00512330,
    .KorgonOutpost = 0x0050fac8,
	  .Metropolis = 0x0050ee18,
	  .BlackwaterCity = 0x0050c6b0,
	  .CommandCenter = 0x0050c470,
    .BlackwaterDocks = 0x0050ecf0,
    .AquatosSewers = 0x0050dff0,
    .MarcadiaPalace = 0x0050d970,
#else
    .Lobby = 0,
    .Bakisi = 0x00518138,
    .Hoven = 0x0051a190,
    .OutpostX12 = 0x0050faa8,
    .KorgonOutpost = 0x0050d2c0,
    .Metropolis = 0x0050c610,
    .BlackwaterCity = 0x00509e28,
    .CommandCenter = 0x00509da8,
    .BlackwaterDocks = 0x0050c5e8,
    .AquatosSewers = 0x0050b928,
    .MarcadiaPalace = 0x0050b268,
#endif
  };

  VariableAddress_t vaSpawnWeaponPackFunc = {
#if UYA_PAL
    .Lobby = 0,
	  .Bakisi = 0x004fb188,
	  .Hoven = 0x004fd2a0,
	  .OutpostX12 = 0x004f2b78,
    .KorgonOutpost = 0x004f0310,
	  .Metropolis = 0x004ef660,
	  .BlackwaterCity = 0x004ecef8,
	  .CommandCenter = 0x004ecec0,
    .BlackwaterDocks = 0x004ef740,
    .AquatosSewers = 0x004eea40,
    .MarcadiaPalace = 0x004ee3c0,
#else
    .Lobby = 0,
    .Bakisi = 0x004f8a08,
    .Hoven = 0x004faa60,
    .OutpostX12 = 0x004f0378,
    .KorgonOutpost = 0x004edb90,
    .Metropolis = 0x004ecee0,
    .BlackwaterCity = 0x004ea6f8,
    .CommandCenter = 0x004ea880,
    .BlackwaterDocks = 0x004ed0c0,
    .AquatosSewers = 0x004ec400,
    .MarcadiaPalace = 0x004ebd40,
#endif
  };
  // Spawn Pack if Health <= zero and if not spawned already.
  if (*(u32*)(TNW_PLAYERDATA + 0x44) <= 0 && SpawnedPack == 0)
  {
    SpawnedPack = 1;

    // Run normal function
    ((void (*)(u32, u32))GetAddress(&vaRespawnFunc))(a0, a1);

    // Spawn Pack
    ((void (*)(u32))GetAddress(&vaSpawnWeaponPackFunc))(PLAYER_STRUCT);
  }
}

void spawnWeaponPackOnDeath(void)
{
  VariableAddress_t vaRespawnPlayerHook = {
#if UYA_PAL
    .Lobby = 0,
	  .Bakisi = 0x0052a09c,
	  .Hoven = 0x0052c1b4,
	  .OutpostX12 = 0x00521a8c,
    .KorgonOutpost = 0x0051f224,
	  .Metropolis = 0x0051e574,
	  .BlackwaterCity = 0x0051be0c,
	  .CommandCenter = 0x0051bbcc,
    .BlackwaterDocks = 0x0051e44c,
    .AquatosSewers = 0x0051d74c,
    .MarcadiaPalace = 0x0051d0cc,
#else
    .Lobby = 0,
    .Bakisi = 0x0052781c,
    .Hoven = 0x00529874,
    .OutpostX12 = 0x0051f18c,
    .KorgonOutpost = 0x0051c9a4,
    .Metropolis = 0x0051bd08,
    .BlackwaterCity = 0x0051950c,
    .CommandCenter = 0x0051948c,
    .BlackwaterDocks = 0x0051bccc,
    .AquatosSewers = 0x0051b00c,
    .MarcadiaPalace = 0x0051a94c,
#endif
  };

  // Disable normal Weapon Pack spawns
  disableWeaponPacks();

  // if Health is greater than zero and pack has spawned
  if (*(u32*)(TNW_PLAYERDATA + 0x44) > 0 && SpawnedPack == 1)
    SpawnedPack = 0;

  // Hook SpawnPack (On Outpost x12 address: 0x0051F18C)
  HOOK_JAL(GetAddress(&vaRespawnPlayerHook), &SpawnPack);
}

/*
 * NAME :		disableV2s
 * 
 * DESCRIPTION :
 * 
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Agent Moose" Pruitt
 */
void disableV2s(void)
{
    VariableAddress_t vaWeaponMeterAddress = {
#ifdef UYA_PAL
        .Lobby = 0,
        .Bakisi = 0x004fb6d0,
        .Hoven = 0x004fd7e8,
        .OutpostX12 = 0x004f30c0,
        .KorgonOutpost = 0x004f0858,
        .Metropolis = 0x004efba8,
        .BlackwaterCity = 0x004ed440,
        .CommandCenter = 0x004ed408,
        .BlackwaterDocks = 0x004efc88,
        .AquatosSewers = 0x004eef88,
        .MarcadiaPalace = 0x004ee908,
#else
        .Lobby = 0,
        .Bakisi = 0x004f8f50,
        .Hoven = 0x004fafa8,
        .OutpostX12 = 0x004f08c0,
        .KorgonOutpost = 0x004ee0d8,
        .Metropolis = 0x004ed428,
        .BlackwaterCity = 0x004eac40,
        .CommandCenter = 0x004eadc8,
        .BlackwaterDocks = 0x004ed608,
        .AquatosSewers = 0x004ec948,
        .MarcadiaPalace = 0x004ec288,
#endif
    };
    u32 addr = GetAddress(&vaWeaponMeterAddress);
    if (*(u32*)addr == 0xA1220000) // sw v0, 0x0(t0)
    {
        // Meter Kills: 0 to 2:
        *(u32*)addr = 0xA1200000; // sw zero, 0x0(t0)
        // Meter Kills: 3:
        *(u32*)(addr + 0x10) = 0xA1200000; // sw zero, 0x0(t0)
         // jal to fully upgrade to v2
        *(u32*)(addr + 0x16C) = 0;
    }
}
