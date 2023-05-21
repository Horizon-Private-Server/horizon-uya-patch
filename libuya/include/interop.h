/***************************************************
 * FILENAME :		interop.h
 * 
 * DESCRIPTION :
 * 		Provides tools to define and reference/invoke addresses in memory by map.
 * 
 * NOTES :
 * 
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_INTEROP_H_
#define _LIBUYA_INTEROP_H_

#include <tamtypes.h>

typedef struct VariableAddress
{
  u32 Lobby;
  u32 Bakisi;
  u32 Hoven;
  u32 OutpostX12;
  u32 KorgonOutpost;
  u32 Metropolis;
  u32 BlackwaterCity;
  u32 CommandCenter;
  u32 BlackwaterDocks;
  u32 AquatosSewers;
  u32 MarcadiaPalace;
} VariableAddress_t;

u32 GetAddress(VariableAddress_t* address);
u32 GetAddressImmediate(VariableAddress_t* address);

#endif // _LIBUYA_INTEROP_H_
