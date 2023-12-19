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
/* 0x00 */ u32 PropertiesOffset;                  // offset to death height, fog density/color, etc
/* 0x04 */ u32 LightsOffset;
/* 0x08 */ u32 CamerasOffset;
/* 0x0C */ u32 SoundInstancesOffset;
/* 0x10 */ u32 USEnglishTranslationsOffset;
/* 0x14 */ u32 UKEnglishTranslationsOffset;
/* 0x18 */ u32 FrenchTranslationsOffset;
/* 0x1C */ u32 GermanTranslationsOffset;
/* 0x20 */ u32 SpanishTranslationsOffset;
/* 0x24 */ u32 ItalianTranslationsOffset;
/* 0x28 */ u32 JapaneseTranslationsOffset;
/* 0x2C */ u32 KoreanTranslationsOffset;
/* 0x30 */ u32 TieClassesOffset;
/* 0x34 */ u32 TieInstancesOffset;
/* 0x38 */ u32 TieGroupsOffset;
/* 0x3C */ u32 ShrubClassesOffset;
/* 0x40 */ u32 ShrubInstancesOffset;
/* 0x44 */ u32 ShrubGroupsOffset;
/* 0x48 */ u32 MobyClassesOffset;
/* 0x4C */ u32 MobyInstancesOffset;               // GameplayMobyHeaderDef_t
/* 0x50 */ u32 MobyGroupsOffset;
/* 0x54 */ u32 GlobalPVarOffset;
/* 0x58 */ u32 PVarPointerScratchpadTableOffset;
/* 0x5C */ u32 PVarTableOffset;                   // GameplayPVarDef_t
/* 0x60 */ u32 PVarDataOffset;					  // Actual PVar Data
/* 0x64 */ u32 PVarPointerRewireTableOffset;
/* 0x68 */ u32 CuboidsOffset;                     // GameplayCuboidHeaderDef_t
/* 0x6C */ u32 SpheresOffset;
/* 0x70 */ u32 CylindersOffset;
/* 0x74 */ u32 PillsOffset;
/* 0x78 */ u32 PathsOffset;
/* 0x7C */ u32 GrindPathsOffset;
/* 0x80 */ u32 PointLightsOffset;
/* 0x84 */ u32 EnvTransitionsOffset;
/* 0x88 */ u32 CameraCollisionGridsOffset;
/* 0x8C */ u32 EnvSamplePointsOffset;
/* 0x90 */ u32 OcclusionOffset;
/* 0x94 */ u32 TieAmbientRGBAsOffset;
/* 0x98 */ u32 AreasOffset;
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
typedef struct DL_GameplayMobyDef
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
	float PosZ;
	float PosY;
	float RotX;
	float RotZ;
	float RotY;
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
} DL_GameplayMobyDef_t;

typedef struct GameplayMobyDef // 0x88
{
/* 0x00 */ u32 Size;
           char unk_04[0x24];
/* 0x28 */ int OClass;
/* 0x2C */ float Scale;
           int unk_30[4];
/* 0x40 */ float PosX;
/* 0x44 */ float PosZ;
/* 0x48 */ float PosY;
/* 0x4C */ float RotX;
/* 0x50 */ float RotZ;
/* 0x54 */ float RotY;
		   char unk_58[0x10];
/* 0x68 */ int PVarIndex;
           char unk_6c[0x1c];
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
