/***************************************************
 * FILENAME :		interop.h
 * DESCRIPTION :
 * 		Provides tools to define and reference/invoke addresses in memory by map.
 * NOTES :
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

/* The first word tags the layout: BASE selects the fixed map base,
 * CUSTOM_BASE selects an explicit Base followed by offsets for every map.
 * Lobby/MainMenu, when present, is an ordinary address at byte 4.
 * Full tables keep their original layout. Rebuild unpublished compact users.
 * Offsets are signed words biased by 0x8000; zero means unavailable.
 */
#define VA_COMPACT_BASE(address) ((u32)(address) | (u32)0x80000000)
#define VA_COMPACT_CUSTOM_BASE(address) ((u32)(address) | (u32)0xc0000000)
#define VA_COMPACT_OFFSET(base, address) ((address) == 0 ? 0 : \
	(u16)(((s32)(address) - (s32)(base)) / 4 + 0x8000 + \
	0 * sizeof(char[((address) == 0 || ((s32)(address) - (s32)(base)) % 4 == 0 && \
	((s32)(address) - (s32)(base)) / 4 >= -32767 && \
	((s32)(address) - (s32)(base)) / 4 <= 32767) ? 1 : -1])))

typedef struct variableAddressCompact {
	/* 0x00 */ u32 Bakisi;
	/* 0x04 */ u32 Lobby;
	/* 0x08 */ u16 Hoven;
	/* 0x0a */ u16 OutpostX12;
	/* 0x0c */ u16 KorgonOutpost;
	/* 0x0e */ u16 Metropolis;
	/* 0x10 */ u16 BlackwaterCity;
	/* 0x12 */ u16 CommandCenter;
	/* 0x14 */ u16 BlackwaterDocks;
	/* 0x16 */ u16 AquatosSewers;
	/* 0x18 */ u16 MarcadiaPalace;
	/* 0x1a: two bytes of EE alignment padding. */
} variableAddressCompact_t;

typedef struct variableAddressCompactBase {
	/* 0x00 */ u32 Base;
	/* 0x04 */ u32 Lobby;
	/* 0x08 */ u16 Bakisi;
	/* 0x0a */ u16 Hoven;
	/* 0x0c */ u16 OutpostX12;
	/* 0x0e */ u16 KorgonOutpost;
	/* 0x10 */ u16 Metropolis;
	/* 0x12 */ u16 BlackwaterCity;
	/* 0x14 */ u16 CommandCenter;
	/* 0x16 */ u16 BlackwaterDocks;
	/* 0x18 */ u16 AquatosSewers;
	/* 0x1a */ u16 MarcadiaPalace;
} variableAddressCompactBase_t;

/* Full, fixed-base compact and custom-base compact tables are accepted. */
u32 GetAddress(const void* address);
u32 GetAddressImmediate(const void* address);

#endif // _LIBUYA_INTEROP_H_
