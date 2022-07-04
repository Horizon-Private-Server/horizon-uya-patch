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
  char customModeId;
  char disableWeaponPacks;
} PatchGameConfig_t;

enum CUSTOM_MAP_ID
{
  CUSTOM_MAP_NONE = 0,
  CUSTOM_MAP_MARAXUS_PRISON,

  // always at the end to indicate how many items there are
  CUSTOM_MAP_COUNT
};

enum CUSTOM_MODE_ID
{
  CUSTOM_MODE_NONE = 0,
  
  // always at the end to indicate how many items there are
  CUSTOM_MODE_COUNT
};

#endif // _CONFIG_H_
