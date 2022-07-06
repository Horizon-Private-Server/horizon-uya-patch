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
  u32 KorgonBlackwaterCity;
  u32 Metropolis;
  u32 CommandCenter;
  u32 DocksSewersMarcadia;
} VariableAddress_t;

typedef union FixedAddress
{
  u32 Value;
  u32 Lobby;
  u32 Bakisi;
  u32 Hoven;
  u32 OutpostX12;
  u32 KorgonBlackwaterCity;
  u32 Metropolis;
  u32 CommandCenter;
  u32 DocksSewersMarcadia;
} FixedAddress_t;

u32 GetAddress(VariableAddress_t* address);

#endif // _LIBUYA_INTEROP_H_
