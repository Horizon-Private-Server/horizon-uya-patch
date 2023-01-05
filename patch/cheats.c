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
  };

  u32 weaponPackSpawnFunc = GetAddress(&vaWeaponPackSpawnFunc);
  if (weaponPackSpawnFunc) {
    *(u32*)weaponPackSpawnFunc = 0;
    *(u32*)(weaponPackSpawnFunc - 0x7BF4) = 0;
  }
}
