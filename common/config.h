#ifndef _CONFIG_H_
#define _CONFIG_H_

#define PATCH_POINTERS             (*(PatchPointers_t**)0x000CFFC0)

// General Tab
typedef struct PatchConfig {
  char enableAutoMaps;
  char disableCameraShake;
  char levelOfDetail;
  char enableFpsCounter;
  char playerFov;
  char enableSpectate;
  char alwaysShowHealth;
  char mapScoreToggle_MapBtn;
  char mapScoreToggle_ScoreBtn;
  char disableScavengerHunt;
  char enableSingleplayerMusic;
  char quickSelectTimeDelay;
  char aimAssist;
  char cycleWeapon1;
  char cycleWeapon2;
  char cycleWeapon3;
  char hypershotEquipButton;
  char disableDpadMovement;
  char hideFluxReticle;
  char dlStyleFlips;
  char enableTeamInfo;
} PatchConfig_t;

// Game Rules Tab
typedef struct PatchGameConfig {
  // Maps
  char isCustomMap;
  char customModeId;

  // Game Rules
  char grRadarBlipsDistance;
  char grRespawnTimer_Player;
  char grRespawnInvincibility;
  char grDisablePenaltyTimers;
  char grDisableWeaponPacks;
  char grV2s;
  char grNoCooldown;
  char grHealthBars;
  char grHealthBoxes;
  char grDisableWeaponCrates;
  char grDisableAmmoPickups;
  char grRespawnTimer_HealthBoxes;
  char grRespawnTimer_WeaponCrates;
  char grRespawnTimer_AmmoPickups;
  char grAutoRespawn;
  char grSetGatlingTurretHealth;
  char grDisableDrones;
  char grDisablePlayerTurrets;
  char grNoBaseDefense_Bots;
  char grNoBaseDefense_SmallTurrets;
  char grBaseHealthPadActive;
  char grVampire;
  char grFluxShotsAlwaysHit;
  char grFluxNikingDisabled;
  char grFlagHotspots;
  char grDestructableBridges;
  char grSuicidePenaltyTimer;
  char grAllNodesTimer;
  char grNodeSelectTimer;
  char grSiegeNoTies;
  char grKothScoreLimit;
  char grKothHillDuration;
  int grSeed;
  char grNewPlayerSync;

  // Party
  char prSurvivor;
  char prChargebootForever;
  char prLoadoutWeaponsOnly;
  char prGravityBombTweakers;
  char prDisableDlStyleFlips;
} __attribute__((packed)) PatchGameConfig_t;

typedef struct PatchPatches {
  PatchConfig_t config;
  PatchGameConfig_t gameConfig;
  char configStartOption;
  char loadingPopup;
  char killStealing;
  char gadgetEvents;
  char deadShooting;
  char weaponShotLag;
  char resurrectWeaponOrdering;
  char ctfLogic;

  // gameConfig misc.
  char spawnWeaponPackOnDeath;
  char disableRespawning;

  // Misc.
  char uiModifiers;
  char holidays;
  char rankTable;
  char colorsExtTable;
} PatchPatches_t;

typedef struct PatchPointers {
  char ServerTimeMonth;
  char ServerTimeDay;
  char ServerTimeHour;
  char ServerTimeMinute;
  char ServerTimeSecond;
} PatchPointers_t;

typedef struct CustomMapDef {
  int Version;
  int BaseMapId;
  int ForcedCustomModeId;
  char Name[32];
  char Filename[64];
} CustomMapDef_t;

enum CUSTOM_MAP_ID {
  CUSTOM_MAP_NONE = 0,

  // always at the end to indicate how many items there are
  CUSTOM_MAP_COUNT
};

enum CUSTOM_MODE_ID {
  CUSTOM_MODE_NONE = 0,
  CUSTOM_MODE_MIDFLAG = 1,
  // CUSTOM_MODE_INFECTED = 2,
  // CUSTOM_MODE_JUGGERNAUT = 3,
  CUSTOM_MODE_DOMINATION = 4,
  CUSTOM_MODE_KOTH = 5,
  // always at the end to indicate how many items there are
  CUSTOM_MODE_COUNT
};

#endif // _CONFIG_H_
