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
  char alwaysShowHealth;
  char mapScoreToggle_MapBtn;
  char mapScoreToggle_ScoreBtn;

} PatchConfig_t;

// Game Rules Tab
typedef struct PatchGameConfig
{
  // Maps
  char customModeId;

  // Game Rules
  char grRadarBlipsDistance;
  char grRespawnTimer;
  char grDisablePenaltyTimers;
  char grDisableWeaponPacks;
  char grV2s;
  char grNoCooldown;
  char grHealthBars;
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
  char prPlayerSize;
} PatchGameConfig_t;

typedef struct CustomMapDef
{
  int Version;
  int BaseMapId;
  int ForcedCustomModeId;
  char Name[32];
  char Filename[64];
} CustomMapDef_t;

enum CUSTOM_MAP_ID
{

  CUSTOM_MAP_NONE = 0,
  CUSTOM_MAP_MARAXUS_PRISON,
  CUSTOM_MAP_SARATHOS_SWAMP,
  CUSTOM_MAP_SPLEEF,

  // always at the end to indicate how many items there are
  CUSTOM_MAP_COUNT
};


enum CUSTOM_MODE_ID
{
  CUSTOM_MODE_NONE = 0,
  CUSTOM_MODE_INFECTED,
  // CUSTOM_MODE_EXAMPLE = 1,
  
  // always at the end to indicate how many items there are
  CUSTOM_MODE_COUNT
};

#endif // _CONFIG_H_
