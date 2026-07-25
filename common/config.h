#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <libuya/gamesettings.h>

#define PATCH_INTEROP (*(PatchInterop_t**)0x000cffc0)

struct CustomMapDef;

typedef void (*SendCustomCommandToClientFunc_t)(int id, int size, void * data);
typedef void (*SetSpectateFunc_t)(int localPlayerIndex, int spectatePlayerOrDisable);
typedef int (*GetCustomMapDefCountFunc_t)(void);
typedef struct CustomMapDef* (*GetCustomMapDefFunc_t)(int index);
typedef int (*ReadCustomMapExtraDataFunc_t)(char* mapFilename, void* buffer, int bufferSize, int customModeId);
typedef void (*RefreshCustomMapDefsFunc_t)(void);
typedef void (*HopToCustomMapFunc_t)(struct CustomMapDef* def);
typedef int (*ReadExtraData_f)(void* dst, int len);


enum CUSTOM_MODE_ID {
	CUSTOM_MODE_NONE = 0,
	CUSTOM_MODE_MIDFLAG = 1,
	// CUSTOM_MODE_INFECTED = 2,
	CUSTOM_MODE_JUGGERNAUT = 3,
	CUSTOM_MODE_DOMINATION = 4,
	CUSTOM_MODE_KOTH = 5,
	// always at the end to indicate how many items there are
	CUSTOM_MODE_COUNT
};

enum CUSTOM_MAP_ID {
	CUSTOM_MAP_NONE = 0,

	// always at the end to indicate how many items there are
	CUSTOM_MAP_COUNT
};

enum CLIENT_TYPE {
	CLIENT_TYPE_NORMAL = 0,
	CLIENT_TYPE_HZN = 1,
	CLIENT_TYPE_PCSX2 = 2
};

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
	char preferredGameServer;
	char kothScrollSpeed;
	char kothHillTransparency;
	char kothHillFxId;
	char playerSyncRate;
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
	char grKothRespawnOutside;
	char grKothRespawnInside;
	char grKothHillSizeIdx;
	char grKothContestedStopsScore;
	char grKothPointStacking;
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
	char swingshotGunBug;

	// gameConfig misc.
	char spawnWeaponPackOnDeath;
	char disableRespawning;

	// Misc.
	char uiModifiers;
	char holidays;
	char rankTable;
	char colorsExtTable;
} PatchPatches_t;

typedef struct CustomMapDef {
	int Version;
	int BaseMapId;
	int ForcedCustomModeId;
	char Name[32];
	char Filename[64];
} CustomMapDef_t;

typedef struct UpdateGameStateRequest {
	char TeamsEnabled;
    char PADDING;
    short Version;
	int TeamScores[GAME_MAX_PLAYERS];
	char ClientIds[GAME_MAX_PLAYERS];
	char Teams[GAME_MAX_PLAYERS];
    int Nodes[2]; // Blue Team, Red Team
} UpdateGameStateRequest_t;

typedef struct CustomGameModeStats {
	u8 pad[4];
} __attribute__((aligned(16))) CustomGameModeStats_t;

typedef struct PatchStateContainer {
	PatchConfig_t* config;
	PatchGameConfig_t* gameConfig;
	int customMapId;
	int selectedCustomMapChanged;
	int updateGameState;
	UpdateGameStateRequest_t gameStateUpdate;
	int updateCustomGameStats;
	CustomGameModeStats_t customGameStats;
	GameSettings gameSettingsAtStart;
	int customGameStatsSize;
	int clientsReadyMask;
	int allClientsReady;
	int voteToEndPassed;
	ReadExtraData_f readExtraDataFunc;
} PatchStateContainer_t;

typedef struct PatchInterop {
	char client;
	char month;
	char day;
	char pad0;
	PatchConfig_t* config;
	PatchGameConfig_t* gameConfig;
	char* mapLoaderFilename;
	GetCustomMapDefCountFunc_t getCustomMapDefCount;
	GetCustomMapDefFunc_t getCustomMapDef;
	ReadCustomMapExtraDataFunc_t readCustomMapExtraData;
	RefreshCustomMapDefsFunc_t refreshCustomMapDefs;
	PatchStateContainer_t* patchStateContainer;
	int* clientLatency;
} PatchInterop_t;

#endif // _CONFIG_H_
