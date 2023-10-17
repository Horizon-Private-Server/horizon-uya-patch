#ifndef _CONFIG_H_
#define _CONFIG_H_

// General Tab
typedef struct PatchConfig
{
  char enableAutoMaps;
  char disableCameraShake;
  char levelOfDetail;
  char enableFpsCounter;
  char playerFov;
  char enableSpectate;
} PatchConfig_t;

// Game Rules Tab
typedef struct PatchGameConfig
{
  // Maps
  char customMapId;
  // char customWorld;
  char customModeId;

  // Game Rules
  char grRespawnTimer;
  char grDisablePenaltyTimers;
  char grDisableWeaponPacks;
  char grV2s;
  char grNoCooldown;
  char grDisableHealthBoxes;
  char grDisableWeaponCrates;
  char grDisableAmmoPickups;
  char grAutoRespawn;
  char grSetGattlingTurretHealth;
  char grDisableDrones;
  char grDisablePlayerTurrets;
  char grNoBaseDefense_Bots;
  char grNoBaseDefense_SmallTurrets;
  char grBaseHealthPadActive;
  char grVampire;
  char grFluxShotsAlwaysHit;
  char grFluxNikingDisabled;

  // Party
  char prSurvivor;
  char prChargebootForever;
} PatchGameConfig_t;

enum CUSTOM_MAP_ID
{
#if TEST
  CUSTOM_MAP_TEST = -1,
#endif
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
