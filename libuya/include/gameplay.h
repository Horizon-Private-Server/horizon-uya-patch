/***************************************************
 * FILENAME :		gameplay.h
 * 
 * DESCRIPTION :
 * 		
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GAMEPLAY_H_
#define _LIBUYA_GAMEPLAY_H_

#include <tamtypes.h>

// credits to chaoticgd for labels
// https://github.com/chaoticgd/wrench
typedef struct GameplayHeaderDef
{
  /* 00 */ u32 PropertiesOffset;                        // offset to death height, fog density/color, etc
  /* 04 */ u32 CamerasOffset;                           // 
  /* 09 */ u32 SoundInstancesOffset;                    // 
  /* 0C */ u32 USEnglishTranslationsOffset;             // 
  /* 10 */ u32 UKEnglishTranslationsOffset;             // 
  /* 14 */ u32 FrenchTranslationsOffset;                // 
  /* 18 */ u32 GermanTranslationsOffset;                // 
  /* 1C */ u32 SpanishTranslationsOffset;               // 
  /* 20 */ u32 ItalianTranslationsOffset;               // 
  /* 24 */ u32 JapaneseTranslationsOffset;              // 
  /* 28 */ u32 KoreanTranslationsOffset;                // 
  /* 2C */ u32 MobyClassesOffset;                       // 
  /* 30 */ u32 MobyInstancesOffset;                     // GameplayMobyHeaderDef_t
  /* 34 */ u32 MobyGroupsOffset;                        // 
  /* 38 */ u32 GlobalPVarOffset;                        // 
  /* 3C */ u32 PVarPointerScratchpadTableOffset;        // 
  /* 40 */ u32 PVarTableOffset;                         // GameplayPVarDef_t
  /* 44 */ u32 PVarDataOffset;                          // 
  /* 48 */ u32 PVarPointerRewireTableOffset;            // 
  /* 4C */ u32 CuboidsOffset;                           // GameplayCuboidHeaderDef_t
  /* 50 */ u32 SpheresOffset;                           // 
  /* 54 */ u32 CylindersOffset;                         // 
  /* 58 */ u32 UNK58Offset;                             // 
  /* 5C */ u32 PathsOffset;                             // 
  /* 60 */ u32 GrindPathsOffset;                        // 
  /* 64 */ u32 UNK64Offset;                             // 
  /* 68 */ u32 PAD68;                                   // 
  /* 6C */ u32 UNK6COffset;                             // 
  /* 70 */ u32 UNK70Offset;                             // 
  /* 74 */ u32 AreasOffset;                             // 
} GameplayHeaderDef_t;

/*
 *
 */
typedef struct GameplayPVarDef
{
  int Offset;
  int Size;
} GameplayPVarDef_t;

/*
 *
 */
typedef struct GameplayMobyDef
{
	u32 Size;
	char Mission;
	int UID;
	int Bolts;
	int OClass;
	float Scale;
	int DrawDistance;
	int UpdateDistance;
	short UNK_20;
	short UNK_22;
	short UNK_24;
	short UNK_26;
	float PosX;
	float PosY;
	float PosZ;
	float RotX;
	float RotY;
	float RotZ;
	int Group;
	int IsRooted;
	float RootedDistance;
	short UNK_4C;
	short UNK_4E;
	int PVarIndex;
	short UNK_54;
	short UNK_56;
	int UNK_58;
	int Red;
	int Green;
	int Blue;
	int Light;
	int UNK_6C;
} GameplayMobyDef_t;

/*
 *
 */
typedef struct GameplayMobyHeaderDef
{
  int StaticCount;
  int DynamicCount;
  int pad[2];
  struct GameplayMobyDef MobyInstances[0];
} GameplayMobyHeaderDef_t;

/*
 *
 */
typedef struct GameplayCuboidDef
{
  MATRIX M0;
  MATRIX M1;
} GameplayCuboidDef_t;

/*
 *
 */
typedef struct GameplayCuboidHeaderDef
{
  int Count;
  int pad[3];
  struct GameplayCuboidDef CuboidInstances[0];
} GameplayCuboidHeaderDef_t;

#endif // _LIBUYA_GAMEPLAY_H_
