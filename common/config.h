#ifndef _CONFIG_H_
#define _CONFIG_H_

// General Tab
typedef struct PatchConfig
{
  char enableAutoMaps;
  char disableCameraShake;
} PatchConfig_t;

// Game Rules Tab
typedef struct PatchGameConfig
{
  // Maps
  char customMapId;
  // char customWorld;
  char customModeId;

  // Game Rules
  char grDisableWeaponPacks;
  char grDisableV2s;
  char grDisableHealthBoxes;
  char grAutoRespawn;
  char grSetGattlingTurretHealth;
  char grVampire;

  // Party
  char prSurvivor;
  char prChargebootForever;
} PatchGameConfig_t;

enum CUSTOM_MAP_ID
{
  CUSTOM_MAP_NONE = 0,
  CUSTOM_MAP_MARAXUS_PRISON,
  CUSTOM_MAP_SARATHOS_SWAMP,

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
