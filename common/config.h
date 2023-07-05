#ifndef _CONFIG_H_
#define _CONFIG_H_

typedef struct PatchConfig
{
  // In Game
  char enableInfiniteHealthMoonjump;
} PatchConfig_t;

typedef struct PatchGameConfig
{
  char customMapId;
  // char customWorld;
  char customModeId;
  char disableWeaponPacks;
  char disableV2s;
  char disableHealthBoxes;
  char autoRespawn;
  char setGattlingTurretHealth;
} PatchGameConfig_t;

enum CUSTOM_MAP_ID
{
  CUSTOM_MAP_NONE = 0,
  CUSTOM_MAP_MARAXUS_PRISON,
  CUSTOM_MAP_SPLEEF,

  // always at the end to indicate how many items there are
  CUSTOM_MAP_COUNT
};

// enum CUSTOM_WORLD_ID
// {
//   CUSTOM_WORLD_NONE = 0,
//   CUSTOM_WORLD_YES,

//   // always at the end to indicate how many items there are
//   CUSTOM_WORLD_COUNT
// };

enum CUSTOM_MODE_ID
{
  CUSTOM_MODE_NONE = 0,
  CUSTOM_MODE_EXAMPLE = 1,
  
  // always at the end to indicate how many items there are
  CUSTOM_MODE_COUNT
};

#endif // _CONFIG_H_
