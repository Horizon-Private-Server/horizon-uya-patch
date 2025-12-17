#include <libuya/pad.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include <libuya/stdio.h>
#include <libuya/color.h>
#include <libuya/string.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/net.h>
#include <libuya/map.h>
#include "config.h"
#include "messageid.h"
#include "include/config.h"
#include "module.h"

#define LINE_HEIGHT         (0.05)
#define LINE_HEIGHT_3_2     (0.075)
#define DEFAULT_GAMEMODE    (0)
#define THUMBNAIL_SIZE      (9248)
#define TPS                 (60)

int selectedTabItem = 0;
u32 padPointer = 0;
int preset = 0;

int dlBytesReceived = 0;
int dlTotalBytes = 0;
int dlIsActive = 0;

// Config
extern PatchConfig_t config;
extern isConfigMenuActive;

// game config
extern PatchGameConfig_t gameConfig;
extern PatchGameConfig_t gameConfigHostBackup;
extern int redownloadCustomModeBinaries;

// Bot config
struct BotConfig {
  char numToInvite;
  char mode;
  char difficulty;
  char profile;
} botConfig;

// constants
const char footerText[] = "\x14 \x15 TAB     \x10 SELECT     \x12 BACK";

// menu display properties
const u32 colorBlack = 0x80000000;
const u32 colorBg = 0x8018608f;
const u32 colorContentBg = 0x80123251;
const u32 colorTabBg = 0x8018608f;
const u32 colorTabBarBg = 0x8004223f;
const u32 colorTabBarSelectedBg = 0x803880af;
const u32 colorRed = 0x8018608f;
const u32 colorSelected = 0x8018608f;
const u32 colorButtonBg = 0x80303030;
const u32 colorButtonFg = 0x80505050;
const u32 colorText = 0x8069cbf2;
const u32 colorOpenBg = 0x20000000;
const u32 colorRangeBar = 0x8018608f;
const u32 colorRangeBarSelected = 0x80123251;
// The color of rangeBar if selected
u32 RangeBar_IsSelected;

const float frameX = 0.1;
const float frameY = 0.15;
const float frameW = 0.8;
const float frameH = 0.7;
const float frameTitleH = 0.075;
const float frameFooterH = 0.05;
const float contentPaddingX = 0.01;
const float contentPaddingY = 0;
const float tabBarH = 0.075;
const float tabBarPaddingX = 0.005;

//
void configMenuDisable(void);
void configMenuEnable(void);

// action handlers
void buttonActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void toggleActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void toggleInvertedActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void listActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void listVerticalActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void orderedListActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void rangeActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void gmOverrideListActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void labelActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);

// state handlers
void menuStateAlwaysHiddenHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateAlwaysDisabledHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateAlwaysEnabledHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateEnabledInMenusHandler(TabElem_t* tab, MenuElem_t* element, int* state);
#ifdef SCAVENGER_HUNT
void menuStateScavengerHuntEnabledHandler(TabElem_t* tab, MenuElem_t* element, int* state);
#endif
void menuLabelStateHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuLabelStateHandler_BaseDefenses(TabElem_t* tab, MenuElem_t* element, int* state);
void menuLabelStateHandler_CTF(TabElem_t* tab, MenuElem_t* element, int* state);
void menuLabelStateHandler_CTFandSiege(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_InstallCustomMaps(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_InstalledCustomMaps(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_GameModeOverride(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_BaseDefenses(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_Siege(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_CTF(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_DM(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_CTFandSiege(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_KOTH(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_Nodes(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_Survivor(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_Default(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_VoteToEndStateHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_DisabledInGame(TabElem_t* tab, MenuElem_t* element, int* state);

int menuStateHandler_SelectedMapOverride(MenuElem_OrderedListData_t* listData, char* value);
int menuStateHandler_SelectedGameModeOverride(MenuElem_OrderedListData_t* listData, char* value);


// list select handlers
void mapsSelectHandler(TabElem_t* tab, MenuElem_t* element);
void gmResetSelectHandler(TabElem_t* tab, MenuElem_t* element);
void gmRefreshMapsSelectHandler(TabElem_t* tab, MenuElem_t* element);
void voteToEndSelectHandler(TabElem_t* tab, MenuElem_t* element);
void botInviteSelectHandler(TabElem_t* tab, MenuElem_t* element);


#ifdef DEBUG
void downloadPatchSelectHandler(TabElem_t* tab, MenuElem_t* element);
#endif

// tab state handlers
void tabDefaultStateHandler(TabElem_t* tab, int * state);
void tabGameSettingsStateHandler(TabElem_t* tab, int * state);
void tabCustomMapsStateHandler(TabElem_t* tab, int * state);
void tabGameSettingsHelpStateHandler(TabElem_t* tab, int * state);

// navigation functions
void navMenu(TabElem_t* tab, int direction, int loop);
void navTab(int direction);

// extern
int mapsGetInstallationResult(void);
int mapsDownloadingModules(void);
int mapReadCustomMapAuthorDescription(char* mapFilename, char dstAuthor[32], char dstDescription[256]);
int mapReadCustomMapThumbnail(char* mapFilename, char *buf, int bufSize);
void refreshCustomMapList(void);
void sendClientVoteForEnd(void);

extern int scavHuntEnabled;
extern VoteToEndState_t voteToEndState;

MenuElem_ListData_t dataLevelOfDetail = {
    .value = &config.levelOfDetail,
    .stateHandler = NULL,
#if DEBUG
    .count = 4,
#else
    .count = 3,
#endif
    .items = { "Potato", "Low", "Normal", "High" }
};

MenuElem_RangeData_t dataFieldOfView = {
    .value = &config.playerFov,
    .stateHandler = NULL,
    .minValue = -5,
    .maxValue = 5,
};

MenuElem_ListData_t dataHypershotEquipButton = {
  .value = &config.hypershotEquipButton,
  .stateHandler = NULL,
  .count = 8,
  .items = {
    "None",
    "Circle",
    "D-Pad Left",
    "D-Pad Down",
    "D-Pad Right",
    "D-Pad Up",
    "L3",
    "R3"
  }
};

MenuElem_RangeData_t dataQuickSelectTimeDelay = {
    .value = &config.quickSelectTimeDelay,
    .stateHandler = NULL,
    .minValue = 0,
    .maxValue = 30,
};

MenuElem_ListData_t dataCycleWeapon1 = {
    .value = &config.cycleWeapon1,
    .stateHandler = NULL,
    .count = 10,
    .items = {
      "Off",
      "N60 Storm",
      "Blitz Cannon",
      "Flux Rifle",
      "Mini Rocket Tube",
      "Gravity Bomb",
      "Mine Glove",
      "Lava Gun",
      "Morph-O-Ray",
      "Holo Shield Glove"
    }
};

MenuElem_ListData_t dataCycleWeapon2 = {
    .value = &config.cycleWeapon2,
    .stateHandler = NULL,
    .count = 10,
    .items = {
      "Off",
      "N60 Storm",
      "Blitz Cannon",
      "Flux Rifle",
      "Mini Rocket Tube",
      "Gravity Bomb",
      "Mine Glove",
      "Lava Gun",
      "Morph-O-Ray",
      "Holo Shield Glove"
    }
};

MenuElem_ListData_t dataCycleWeapon3 = {
    .value = &config.cycleWeapon3,
    .stateHandler = NULL,
    .count = 10,
    .items = {
      "Off",
      "N60 Storm",
      "Blitz Cannon",
      "Flux Rifle",
      "Mini Rocket Tube",
      "Gravity Bomb",
      "Mine Glove",
      "Lava Gun",
      "Morph-O-Ray",
      "Holo Shield Glove"
    }
};

// map select list
MenuElem_ListData_t dataCustomMaps = {
  .value = &patchStateContainer.CustomMapId,
  .stateHandler = menuStateHandler_SelectedMapOverride,
  .count = 1,
  .rows = 10,
  .items = {
    "None",
    [MAX_CUSTOM_MAP_DEFINITIONS+1] NULL
  }
};

// maps with their own exclusive gamemode
char dataCustomMapsWithExclusiveGameMode[] = {
/*
#if DEBUG
  CUSTOM_MAP_SPLEEF,
#endif
*/
};
const int dataCustomMapsWithExclusiveGameModeCount = sizeof(dataCustomMapsWithExclusiveGameMode)/sizeof(char);

MenuElem_OrderedListData_t dataCustomModes = {
  .value = &gameConfig.customModeId,
  .stateHandler = menuStateHandler_SelectedGameModeOverride,
  .count = 4,
  .items = {
    {CUSTOM_MODE_NONE, "None"},
    // {CUSTOM_MODE_INFECTED, "Infected"},
    // {CUSTOM_MODE_JUGGERNAUGHT, "Juggernaut"},
    {CUSTOM_MODE_MIDFLAG, "MidFlag"},
    {CUSTOM_MODE_DOMINATION, "Domination"},
    {CUSTOM_MODE_KOTH, "King of the Hill"},
  }
};

// array of alternative "short" names for game modes
// used when displaying in areas with limited room
// if the entry is NULL then the full name will be used
const char* CustomModeShortNames[] = {
  [CUSTOM_MODE_NONE] NULL,
  // [CUSTOM_MODE_INFECTED] "Infected",
  // [CUSTOM_MODE_JUGGERNAUT] NULL,
  [CUSTOM_MODE_MIDFLAG] NULL,
  [CUSTOM_MODE_DOMINATION] "Domination",
  [CUSTOM_MODE_KOTH] "KOTH"
};

MenuElem_ListData_t dataKothScoreLimit = {
    .value = &gameConfig.grKothScoreLimit,
    .stateHandler = menuStateHandler_KOTH,
    .count = 14,
    .items = { "Off", "50", "100", "150", "200", "250", "300", "350", "400", "450", "500", "750", "1000", "2000" }
};

MenuElem_ListData_t dataKothHillDuration = {
    .value = &gameConfig.grKothHillDuration,
    .stateHandler = menuStateHandler_KOTH,
    .count = 5,
    .items = { "60", "120", "180", "240", "300" }
};

// KOTH hill size scale options (XY). Order must match KOTH_HILL_SCALE_TABLE in koth/game.c.
static char kothHillSizeIdx = 0; // host edit only; not serialized
MenuElem_ListData_t dataKothHillSize = {
    .value = &kothHillSizeIdx,
    .stateHandler = menuStateHandler_KOTH,
    .count = 7,
    .items = { "1x", "1.5x", "2x", "2.5x", "3x", "3.5x", "4x" }
};

MenuElem_ListData_t dataV2_Setting = {
    .value = &gameConfig.grV2s,
    .stateHandler = NULL,
    .count = 3,
    .items = { "On", "Off", "Always" }
};

MenuElem_ListData_t dataVampire = {
    .value = &gameConfig.grVampire,
    .stateHandler = NULL,
    .count = 5,
    .items = { "Off", "25%", "50%", "75%", "100%" }
};

MenuElem_ListData_t dataSetGatlingTurretHealth = {
    .value = &gameConfig.grSetGatlingTurretHealth,
    .stateHandler = NULL,
    .count = 10,
    .items = { "Default", ".5x", "1.5x", "2x", "3x", "4x", "5x", "6x", "7x", "8x" }
};

MenuElem_ListData_t dataRespawnTimer_Player = {
    .value = &gameConfig.grRespawnTimer_Player,
    .stateHandler = NULL,
    .count = 13,
    .items = { "1.5", "2", "2.5", "3", "4", "5", "6", "7", "8", "9", "10", "0", "1", }
};

MenuElem_ListData_t dataRadarBlipsDistance = {
    .value = &gameConfig.grRadarBlipsDistance,
    .stateHandler = NULL,
    .count = 6,
    .items = { "Short", "2x", "4x", "Always", "Off", ".5x" }
};

MenuElem_ListData_t dataMapScore_MapAccess = {
    .value = &config.mapScoreToggle_MapBtn,
    .stateHandler = NULL,
    .count = 4,
    .items = { "Default", "Select", "L3", "R3" }
};

MenuElem_ListData_t dataMapScore_ScoreboardAccess = {
    .value = &config.mapScoreToggle_ScoreBtn,
    .stateHandler = NULL,
    .count = 4,
    .items = { "Default", "Select", "L3", "R3" }
};

MenuElem_ListData_t dataGameConfigPreset = {
    .value = &preset,
    .stateHandler = NULL,
    .count = 6,
    .items = { "None", "Meta", "Competitive", "1v1", "Bot", "Siege"}
};

MenuElem_ListData_t dataRespawnTimer_HealthBoxes = {
    .value = &gameConfig.grRespawnTimer_HealthBoxes,
    .stateHandler = NULL,
    .count = 7,
    .items = { "30", "0", "5", "10", "15", "20", "25" }
};
MenuElem_ListData_t dataRespawnTimer_WeaponCrates = {
    .value = &gameConfig.grRespawnTimer_WeaponCrates,
    .stateHandler = NULL,
    .count = 7,
    .items = { "30", "0", "5", "10", "15", "20", "25" }
};
MenuElem_ListData_t dataRespawnTimer_AmmoPickups = {
    .value = &gameConfig.grRespawnTimer_AmmoPickups,
    .stateHandler = NULL,
    .count = 7,
    .items = { "30", "0", "5", "10", "15", "20", "25" }
};

MenuElem_ListData_t dataHealthBoxes = {
    .value = &gameConfig.grHealthBoxes,
    .stateHandler = NULL,
    .count = 3,
    .items = { "On", "No Glass Container", "Off" }
};

MenuElem_RangeData_t dataNodeSelectTimer = {
    .value = &gameConfig.grNodeSelectTimer,
    .stateHandler = NULL,
    .minValue = 0,
    .maxValue = 10,
    .appendText = " Seconds",
};

MenuElem_RangeData_t dataSuicidePenaltyTimer = {
    .value = &gameConfig.grSuicidePenaltyTimer,
    .stateHandler = NULL,
    .minValue = 0,
    .maxValue = 10,
    .appendText = " Seconds"
};

MenuElem_ListData_t dataAllNodesTimer = {
    .value = &gameConfig.grAllNodesTimer,
    .stateHandler = NULL,
    .count = 9,
    .items = { "Off", "30 Seconds", "1 Minute", "1.5 Minutes", "2 Minutes", "2.5 Minutes", "3 Minutes", "3.5 Minutes", "4 Minutes" }
};

// Bot options
MenuElem_ListData_t botNumToInvite = {
    .value = &botConfig.numToInvite,
    .stateHandler = NULL,
    .count = 8,
    .items = { "All slots", "1", "2", "3", "4", "5", "6", "7" }
};

MenuElem_ListData_t botMode = {
    .value = &botConfig.mode,
    .stateHandler = NULL,
    .count = 5,
    .items = { "Dynamic", "Static", "Training Passive", "Training Idle", "God" }
};

MenuElem_ListData_t botDifficulty = {
    .value = &botConfig.difficulty,
    .stateHandler = NULL,
    .count = 10,
    .items = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" }
};

MenuElem_ListData_t botProfile = {
    .value = &botConfig.profile,
    .stateHandler = NULL,
    .count = 10,
    .items = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" }
};


// General
MenuElem_t menuElementsGeneral[] = {
#ifdef DEBUG
  { "Redownload patch", buttonActionHandler, menuStateAlwaysEnabledHandler, downloadPatchSelectHandler },
#endif
  { "Vote to End", buttonActionHandler, menuStateHandler_VoteToEndStateHandler, voteToEndSelectHandler, "Vote to end the game. If a team/player is in the lead they will win." },
  { "Refresh Maps", buttonActionHandler, menuStateEnabledInMenusHandler, gmRefreshMapsSelectHandler, "Refresh the custom map list." },
  // { "Install Custom Maps on Login", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enableAutoMaps },
#if SCAVENGER_HUNT
  { "Participate in Scavenger Hunt", toggleInvertedActionHandler, menuStateScavengerHuntEnabledHandler, &config.disableScavengerHunt, "If you see this option, there is a Horizon scavenger hunt active. Enabling this will spawn random Horizon bolts in game. Collect the most to win the hunt!" },
#endif
/*
#if DEBUG
  { "16:9 Widescreen", toggleActionHandler, menuStateAlwaysEnabledHandler, &IS_WIDESCREEN },
  { "Progressive Scan", toggleActionHandler, menuStateAlwaysEnabledHandler, &IS_PROGRESSIVE_SCAN },
#endif
*/
  { "Always Show Health", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.alwaysShowHealth, "Let that health bar hide no longer!  This will make it so the health bar is always visible." },
  { "Enable Team Info", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enableTeamInfo, "Display team health and cycle v2 status on the bottom left of your screen." },
  { "Camera Pull", toggleInvertedActionHandler, menuStateAlwaysEnabledHandler, &config.aimAssist, "Toggles code that pulls the camera towards nearby targets when aiming." },
  { "Camera Shake", toggleInvertedActionHandler, menuStateAlwaysEnabledHandler, &config.disableCameraShake, "Toggles the camera shake caused by nearby explosions." },
  { "Disable D-Pad Movement", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.disableDpadMovement, "Disables the d-pad moving your character in game." },
  // { "Disable Strafing Side-Flips", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.dlStyleFlips, "Disables Strafing Side-Flips." },
  { "Field of View", rangeActionHandler, menuStateAlwaysEnabledHandler, &dataFieldOfView },
  { "FPS Counter", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enableFpsCounter, "Toggles the in game FPS counter." },
  { "Hide Flux Reticle", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.hideFluxReticle, "Hide the Flux's reticle to show how tough you really are!" },
  { "Hypershot Equip Button", listActionHandler, menuStateAlwaysEnabledHandler, &dataHypershotEquipButton, "The button that you will press to take out the hypershot." },
  { "Level of Detail", listActionHandler, menuStateAlwaysEnabledHandler, &dataLevelOfDetail },
  { "Loadout Weapon 1", listActionHandler, menuStateHandler_DisabledInGame, &dataCycleWeapon1, "Main Weapon 1.  Defaults to Gravity Bomb if nothing is selected." },
  { "Loadout Weapon 2", listActionHandler, menuStateHandler_DisabledInGame, &dataCycleWeapon2, "Main Weapon 2.  Defaults to Blitz Cannon if nothing is selected." },
  { "Loadout Weapon 3", listActionHandler, menuStateHandler_DisabledInGame, &dataCycleWeapon3, "Main Weapon 3.  Defaults to Flux Rifle if nothing is selected." },
  // { "Player Sync (Experimental)", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enablePlayerSync, "Synces players movements for a smoother/less laggy experience." },
  // { "Spectate", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enableSpectate, "Toggles the custom spectate feature. Use \x13 when dead to spectate.  Some gomemodes may restrict whom you can spectate." },
  { "Quick Select Delay", rangeActionHandler, menuStateAlwaysEnabledHandler, &dataQuickSelectTimeDelay, "Change how short/long you need to hold triangle to open the Quick Select menu." },
  { "Single Player Music", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enableSingleplayerMusic, "Tired of the same BORING songs in game?!  Spice it up by adding the music tracks from Single Player!" },
  { "Toggle Map Button", listActionHandler, menuStateAlwaysEnabledHandler, &dataMapScore_MapAccess, "The button that you will need to press to show the Map." },
  { "Toggle Scoreboard Button", listActionHandler, menuStateAlwaysEnabledHandler, &dataMapScore_ScoreboardAccess, "The button that you will need to press to show the Scoreboard." },
};

// Game Settings
MenuElem_t menuElementsGameSettings[] = {
  { "Reset", buttonActionHandler, menuStateAlwaysEnabledHandler, gmResetSelectHandler },

  // { "Game Settings", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_HEADER },
  // { "Map Override", listActionHandler, menuStateAlwaysEnabledHandler, &dataCustomMaps, "Play on any of the custom maps from the Horizon Map Pack. Visit https://rac-horizon.com to download the map pack." },
  { "Gamemode Override", gmOverrideListActionHandler, menuStateHandler_GameModeOverride, &dataCustomModes, "Change to one of the Horizon Custom Gamemodes." },
  { "KOTH Points", listActionHandler, menuStateHandler_KOTH, &dataKothScoreLimit, "Points needed to win King of the Hill (0 = disabled, uses timer if set)." },
  { "KOTH Hill Duration", listActionHandler, menuStateHandler_KOTH, &dataKothHillDuration, "How long a hill stays active before rotating to the next." },
  { "KOTH Hill Size", listActionHandler, menuStateHandler_KOTH, &dataKothHillSize, "Scale for KOTH hill radius (XY)." },
  { "Preset", listActionHandler, menuStateAlwaysEnabledHandler, &dataGameConfigPreset, "Select one of the preconfigured game rule presets or manually set the custom game rules below." },

  { "Game Rules", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_HEADER },
  { "Radar Blips", listActionHandler, menuStateHandler_Default, &dataRadarBlipsDistance, "Toggle the radar blips on the map short (default), Always, or Off (won't show any players on map)" },
  { "Respawn Timer", listActionHandler, menuStateHandler_Default, &dataRespawnTimer_Player, "Time in seconds for the player to respawn." },
  { "Respawn Invincibility", toggleActionHandler, menuStateHandler_Default, &gameConfig.grRespawnInvincibility, "Adds a small invincibility timer to players whom have just respawned." },
  { "Penalty Timers", toggleInvertedActionHandler, menuStateHandler_CTFandSiege, &gameConfig.grDisablePenaltyTimers, "Disables longer respawn timers for your team if your teams base turrets have been destroyed." },
  { "V2s", listActionHandler, menuStateHandler_Default, &dataV2_Setting, "Configures V2 weapon upgrades to be disabled, on (default), or always on (spawn with v2 weapons)." },
  { "Damage Cooldown", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grNoCooldown, "Disables the brief hit invincibility after taking damage." },
  { "Destructable Bridges", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDestructableBridges, "turn on/off the ability to destroy bridges." },
  { "Fix Wallsniping", toggleActionHandler, menuStateHandler_Default, &gameConfig.grFluxShotsAlwaysHit, "Forces sniper shots that hit to register on every client. Can result in shots that appear to phase through walls." },
  { "Fix Flux Niking", toggleActionHandler, menuStateHandler_Default, &gameConfig.grFluxNikingDisabled, "Forces sniper shots to always deal the correct amount of damage." },
  { "Weapon Packs", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisableWeaponPacks, "Toggle if weapon packs should spawn when a player dies." },
  { "Health Boxes", listActionHandler, menuStateHandler_Default, &dataHealthBoxes, "Whether health pickups are enabled, or if there is a box enclosure that must be broken first before picking up." },
  // { "Drones", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisableDrones, "Toggle Drones on or off." },
  { "Player Turrets", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisablePlayerTurrets, "Toggle Player Turrets on or off." },
  { "Weapon Crates", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisableWeaponCrates, "Toggle Weapon Crates on or off." },
  { "Ammo Pickups", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisableAmmoPickups, "Toggle Ammo Pickups on or off." },
  { "Health Boxes Respawn Timer", listActionHandler, menuStateHandler_Default, &dataRespawnTimer_HealthBoxes, "Time in seconds for the Health Boxes to respawn." },
  // { "Weapon Crates Respawn Timer", listActionHandler, menuStateHandler_Default, &dataRespawnTimer_WeaponCrates, "Time in seconds for the Weapon Crates to respawn." },
  // { "Ammo Pickups Respawn Timer", listActionHandler, menuStateHandler_Default, &dataRespawnTimer_AmmoPickups, "Time in seconds for the Ammo Pickups to respawn." },
  { "Auto Respawn", toggleActionHandler, menuStateHandler_DM, &gameConfig.grAutoRespawn, "Automatically respawn players once the in game respawn timer hits zero." },
  { "New Player Sync", toggleActionHandler, menuStateHandler_Default, &gameConfig.grNewPlayerSync, "Enable new player sync." },  
  { "Vampire Healing", listActionHandler, menuStateHandler_Default, &dataVampire, "Earn a percentage of health for each kill."},
  // { "Health Bars", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grHealthBars, "Draws a healthbar above each player's nametag." },
#if DEBUG
  { "CTF Flag Returns on Bad Ground", toggleActionHandler, menuStateHandler_CTF, &gameConfig.grFlagHotspots, "Returns a dropped flag if it lands on water, lava, or other non-walkable areas." },
#endif

  { "Siege/CTF Rules", labelActionHandler, menuLabelStateHandler_CTFandSiege, (void*)LABELTYPE_HEADER },
  { "Suicide Penalty", rangeActionHandler, menuStateHandler_CTFandSiege, &dataSuicidePenaltyTimer, "Amount of time added to the respawn time if a player suicided." },
  { "Node Select Timer", rangeActionHandler, menuStateHandler_Nodes, &dataNodeSelectTimer, "Amount of time a player has for choosing a node.  If timer runs out, player is spawned on current selected node." },
  { "All Nodes Countdown", listActionHandler, menuStateHandler_Nodes, &dataAllNodesTimer, "If one team owns all nodes, a countdown stsarts.  Team with all nodes win if countdown reaches zero." },
  { "No Ties in Timed Games", toggleActionHandler, menuStateHandler_Siege, &gameConfig.grSiegeNoTies, "Team with most base damage dealt wins!" },

  { "Base/Node Modifications", labelActionHandler, menuLabelStateHandler_CTFandSiege, (void*)LABELTYPE_HEADER },
  { "Gatling Turret Health", listActionHandler, menuStateHandler_BaseDefenses, &dataSetGatlingTurretHealth, "Increase or decrease the amount of health each teams base turrets have." },
  { "Health/Ammo Pads Always Active", toggleActionHandler, menuStateHandler_BaseDefenses, &gameConfig.grBaseHealthPadActive, "Let the Health and Ammo pads in each base always stay active, even if parts of the base are destroyed." },
  { "Bots (Troopers, Ball Bots, ect.)", toggleInvertedActionHandler, menuStateHandler_CTFandSiege, &gameConfig.grNoBaseDefense_Bots, "toggle Troopers (and other bots) on or off." },
  // { "Small Turrets", toggleInvertedActionHandler, menuStateHandler_CTFandSiege, &gameConfig.grNoBaseDefense_SmallTurrets },

  { "Party Rules", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_HEADER },
  { "Chargeboot Forever", toggleActionHandler, menuStateHandler_Default, &gameConfig.prChargebootForever, "Double tap and hold R2 to chargeboot forever." },
  // { "Force Strafing Side-Flips", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.prDisableDlStyleFlips, "Forces 'Disable Strafing Side-Flips' to be off, no matter the players setting." },
  { "Gravity Bomb->B6 Obliterator", toggleActionHandler, menuStateHandler_Default, &gameConfig.prGravityBombTweakers, "Changes the Gravity Bomb Physics to that of the B6 Obliterator from Ratchet: Deadlocked!" },
  { "Loadout Weapons Only", toggleActionHandler, menuStateHandler_Default, &gameConfig.prLoadoutWeaponsOnly, "Only allow the use of each players Loadout weapons, regardless of what weapons are enabled." },
  { "Survivor", toggleActionHandler, menuStateHandler_Survivor, &gameConfig.prSurvivor, "You only have one life!  Once you die, you can't respawn!" },

  // { "Experimental CTF Rules", labelActionHandler, menuLabelStateHandler_CTF, (void*)LABELTYPE_HEADER },
  // { "CTF: Custom Flag Logic", toggleActionHandler, menuStateHandler_CTF, &gameConfig.grFlagHotspots, "Replaces original CTF code for hopefully less lag." },
};

// Game Settings (not in staging)
MenuElem_t menuElementsGameSettingsHelp[] = {
  { "", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_LABEL },
  { "Please create a game to configure", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_LABEL },
  { "the custom game settings.", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_LABEL },
};

char* mapOverrideSelectedMapThumbnail = NULL;
int mapOverrideSelectedMapHasThumbnail = 0;
char mapOverrideSelectedMapAuthor[32] = {};
char mapOverrideSelectedMapDesc[256] = {};
int mapOverrideSelectedMapTicks = 0;
int mapOverrideLastSelectedMapIdx = 0;
MenuElem_t menuElementsGameSettingsCustomMaps[] = {
  { "Map override", listVerticalActionHandler, menuStateAlwaysEnabledHandler, &dataCustomMaps, "Play on any of the custom maps from the Horizon Map Pack. Visit https://rac-horizon.com to download the map pack." },
};

// Bot Settings
MenuElem_t menuElementsBotSettings[] = {
  { "Invite Bot(s)", buttonActionHandler, menuStateAlwaysEnabledHandler, botInviteSelectHandler },
  { "Number to Invite", listActionHandler, menuStateAlwaysEnabledHandler, &botNumToInvite, "Number of bots to invite" },
  { "Mode", listActionHandler, menuStateAlwaysEnabledHandler, &botMode, "Bot Mode" },
  { "Difficulty (1-10)", listActionHandler, menuStateAlwaysEnabledHandler, &botDifficulty, "Difficulty 1-10. Doesn't apply to training modes" },
  { "Custom Profile", listActionHandler, menuStateAlwaysEnabledHandler, &botProfile, "Bot Profile you want to invite (0=any). Only applies when inviting 1 bot" },
};

// Bot Settings (not in staging)
MenuElem_t menuElementsBotSettingsHelp[] = {
  { "", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_LABEL },
  { "Please create a game to configure", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_LABEL },
  { "bots.", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_LABEL },
};


// tab items
TabElem_t tabElements[] = {
  { "General", tabDefaultStateHandler, menuElementsGeneral, sizeof(menuElementsGeneral)/sizeof(MenuElem_t) },
  { "Game Settings", tabGameSettingsStateHandler, menuElementsGameSettings, sizeof(menuElementsGameSettings)/sizeof(MenuElem_t) },
  { "Game Settings", tabGameSettingsHelpStateHandler, menuElementsGameSettingsHelp, sizeof(menuElementsGameSettingsHelp)/sizeof(MenuElem_t) },
  { "Custom Maps", tabCustomMapsStateHandler, menuElementsGameSettingsCustomMaps, sizeof(menuElementsGameSettingsCustomMaps)/sizeof(MenuElem_t) },
  { "Bots", tabGameSettingsStateHandler, menuElementsBotSettings, sizeof(menuElementsBotSettings)/sizeof(MenuElem_t) },
  { "Bots", tabGameSettingsHelpStateHandler, menuElementsBotSettingsHelp, sizeof(menuElementsBotSettingsHelp)/sizeof(MenuElem_t) },
};

const int tabsCount = sizeof(tabElements)/sizeof(TabElem_t);


//------------------------------------------------------------------------------
//---------------------------- GENERAL SETTINGS TAB ----------------------------
//------------------------------------------------------------------------------
#ifdef DEBUG

// 
void downloadPatchSelectHandler(TabElem_t* tab, MenuElem_t* element)
{
  // close menu
  configMenuDisable();

  // send request
  void * lobbyConnection = netGetLobbyServerConnection();
  if (lobbyConnection)
    netSendCustomAppMessage(lobbyConnection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_REQUEST_PATCH, 0, (void*)element);
}

#endif

//------------------------------------------------------------------------------
//------------------------------ GAME SETTINGS TAB -----------------------------
//------------------------------------------------------------------------------
int menuStateHandler_SelectedMapOverride(MenuElem_OrderedListData_t* listData, char* value)
{
  int i;
  if (!value)
    return 0;

  char gm = gameConfig.customModeId;
  char v = *value;

  // here we can disable certain maps depending on the gamemode
  /*
  switch (gm)
  {
    case CUSTOM_MODE_SURVIVAL:
    {
      if (v >= CUSTOM_MAP_SURVIVAL_START && v <= CUSTOM_MAP_SURVIVAL_END)
        return 1;
      *value = CUSTOM_MAP_SURVIVAL_START;
      return 0;
    }
    case CUSTOM_MODE_PAYLOAD:
    {
      if (v == CUSTOM_MAP_SARATHOS_SP || v == CUSTOM_MAP_DESERT_PRISON)
        return 1;
      *value = CUSTOM_MAP_DESERT_PRISON;
      return 0;
    }
    default:
    {
      if (v < CUSTOM_MAP_SURVIVAL_START)
        return 1;
      
      *value = CUSTOM_MAP_NONE;
      return 0;
    }
  }
  */

  // hide maps with gamemode override
  if (gm > CUSTOM_MODE_NONE) {
    for (i = 0; i < dataCustomMapsWithExclusiveGameModeCount; ++i) {
      if (v == dataCustomMapsWithExclusiveGameMode[i]) {
        *value = CUSTOM_MAP_NONE;
        return 0;
      }
    }
  }

  // success
  return 1;
}

// 
int menuStateHandler_SelectedGameModeOverride(MenuElem_OrderedListData_t* listData, char* value)
{
  if (!value)
    return 0;

  GameSettings* gs = gameGetSettings();
  char v = *value;

  if (gs) {
    switch (v) {
/*
      #ifdef DEBUG
      case CUSTOM_MODE_INFECTED:
      case CUSTOM_MODE_JUGGERNAUGHT: {
        // only allow deathmatch
        if (gs->GameType == GAMETYPE_DM)
          return 1;
        
        // otherwise reject custom mode
        *value = CUSTOM_MODE_NONE;
        return 0;
      }
      #endif
*/
      case CUSTOM_MODE_MIDFLAG: {
        if (gs->GameType == GAMETYPE_CTF)
          return 1;
        
        
        *value = CUSTOM_MODE_NONE;
        return 0; 
      }
      case CUSTOM_MODE_DOMINATION: {
        if (gs->GameType == GAMETYPE_SIEGE || GAMETYPE_CTF)
          return 1;
        
        
        *value = CUSTOM_MODE_NONE;
        return 0; 
      }
    }
  }

  return 1;
}

void menuStateHandler_Siege(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType != GAMETYPE_SIEGE)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_CTF(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType != GAMETYPE_CTF)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_DM(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType != GAMETYPE_DM)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_KOTH(TabElem_t* tab, MenuElem_t* element, int* state)
{
  if (gameConfig.customModeId != CUSTOM_MODE_KOTH)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_CTFandSiege(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType == GAMETYPE_DM)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_BaseDefenses(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();
  GameOptions * go = gameGetOptions();
  if (!gs || (!go->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets))
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_Nodes(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();
  GameOptions * go = gameGetOptions();
  if (!gs || (!go->GameFlags.MultiplayerGameFlags.Nodes))
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_Survivor(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();
  GameOptions * go = gameGetOptions();

  if (!gs || (gs->GameType != GAMETYPE_DM) || go->GameFlags.MultiplayerGameFlags.Teams)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_Default(TabElem_t* tab, MenuElem_t* element, int* state)
{
    if (preset)
      *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
    else
      *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}
// ---------------------------------------
// -------- LABEL STATE HANDLERS ---------
// ---------------------------------------
void menuLabelStateHandler_CTF(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType == GAMETYPE_DM)
    *state = ELEMENT_HIDDEN;
  else
    *state = ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}
void menuLabelStateHandler_CTFandSiege(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType == GAMETYPE_DM)
    *state = ELEMENT_HIDDEN;
  else
    *state = ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuLabelStateHandler_BaseDefenses(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();
  GameOptions * go = gameGetOptions();
  if (!gs || (!go->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets))
    *state = ELEMENT_HIDDEN;
  else
    *state = ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

// 
void gmResetSelectHandler(TabElem_t* tab, MenuElem_t* element)
{
  preset = 0;
  memset(&gameConfig, 0, sizeof(gameConfig));
  patchStateContainer.CustomMapId = 0;
}

// 
void botInviteSelectHandler(TabElem_t* tab, MenuElem_t* element)
{
  preset = 4;
  memset(&gameConfig, 0, sizeof(gameConfig));
  patchStateContainer.CustomMapId = 0;

  void * lobbyConnection = netGetLobbyServerConnection();
  netSendCustomAppMessage(lobbyConnection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_INVITE_BOT, sizeof(botConfig), &botConfig);

  // popup
  if (isInMenus())
  {
    uiShowOkDialog("Bots Invited", "Please Wait up to 60 seconds");
  }
}

// 
void gmRefreshMapsSelectHandler(TabElem_t* tab, MenuElem_t* element)
{
  refreshCustomMapList();

  // popup
  if (isInMenus()) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Found %d maps", CustomMapDefCount);
    uiShowOkDialog("Custom Maps", buf);
  }
}

//
void voteToEndSelectHandler(TabElem_t* tab, MenuElem_t* element)
{
  sendClientVoteForEnd();
  configMenuDisable();
}

// 
void menuStateHandler_VoteToEndStateHandler(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings* gs = gameGetSettings();
  int i = 0;
  
  if (isInGame()) {
    Player* p = playerGetFromSlot(0);
    if (p) {
      *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
      if (!voteToEndState.Votes[p->mpIndex]) *state |= ELEMENT_EDITABLE;
      return;
    }
  }
  
  *state = ELEMENT_HIDDEN;
}

// 
void tabGameSettingsStateHandler(TabElem_t* tab, int * state)
{
  GameSettings * gameSettings = gameGetSettings();
  if (!gameSettings)
  {
    *state = ELEMENT_HIDDEN;
  }

  // if game has started or not the host, disable editing
  else if (gameSettings->GameLoadStartTime > 0 || !gameAmIHost())
  {
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  }
  else
  {
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
  }
}

void tabCustomMapsStateHandler(TabElem_t* tab, int * state)
{
  GameSettings * gameSettings = gameGetSettings();
  // if no game settings, or in game, hide tab.
  if (!gameSettings || gameSettings->GameLoadStartTime > 0) {
    *state = ELEMENT_HIDDEN;
  }
  // if not host, only view.
  else if (!gameAmIHost()) {
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  } else {
    // if host, do all.
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
  }
}

void tabGameSettingsHelpStateHandler(TabElem_t* tab, int * state)
{
  GameSettings * gameSettings = gameGetSettings();
  if (!gameSettings) {
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
  } else {
    *state = ELEMENT_HIDDEN;
  }
}

//------------------------------------------------------------------------------
void tabDefaultStateHandler(TabElem_t* tab, int * state)
{
  *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

//------------------------------------------------------------------------------
void menuStateAlwaysHiddenHandler(TabElem_t* tab, MenuElem_t* element, int* state)
{
  *state = ELEMENT_HIDDEN;
}

// 
void menuStateAlwaysDisabledHandler(TabElem_t* tab, MenuElem_t* element, int* state)
{
  *state = ELEMENT_VISIBLE | ELEMENT_SELECTABLE;
}

// 
void menuStateAlwaysEnabledHandler(TabElem_t* tab, MenuElem_t* element, int* state)
{
  *state = ELEMENT_VISIBLE | ELEMENT_EDITABLE | ELEMENT_SELECTABLE;
}

// 
void menuStateEnabledInMenusHandler(TabElem_t* tab, MenuElem_t* element, int* state)
{
  if (isInMenus()) *state = ELEMENT_VISIBLE | ELEMENT_EDITABLE | ELEMENT_SELECTABLE;
  else *state = ELEMENT_HIDDEN;
}

void menuStateHandler_DisabledInGame(TabElem_t* tab, MenuElem_t* element, int* state)
{
  if (isInMenus()) *state = ELEMENT_VISIBLE | ELEMENT_EDITABLE | ELEMENT_SELECTABLE;
  else *state = ELEMENT_VISIBLE | ELEMENT_SELECTABLE;
}

// 
void menuStateHandler_GameModeOverride(TabElem_t* tab, MenuElem_t* element, int* state)
{
  int i = 0;

  // hide gamemode for maps with exclusive gamemode
  for (i = 0; i < dataCustomMapsWithExclusiveGameModeCount; ++i)
  {
    if (patchStateContainer.CustomMapId == dataCustomMapsWithExclusiveGameMode[i])
    {
      *state = ELEMENT_HIDDEN;
      return;
    }
  }

  *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}
#ifdef SCAVENGER_HUNT
void menuStateScavengerHuntEnabledHandler(TabElem_t* tab, MenuElem_t* element, int* state)
{
  if (!scavHuntEnabled)
    *state = ELEMENT_HIDDEN;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}
#endif
// 
void menuLabelStateHandler(TabElem_t* tab, MenuElem_t* element, int* state)
{
  *state = ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

int getMenuElementState(TabElem_t* tab, MenuElem_t* element)
{
  // get tab and element state
  int tabState = 0, state = 0;
  tab->stateHandler(tab, &tabState);
  element->stateHandler(tab, element, &state);

  return tabState & state;
}

//------------------------------------------------------------------------------
void drawToggleMenuElement(TabElem_t* tab, MenuElem_t* element, RECT* rect)
{
  // get element state
  int state = getMenuElementState(tab, element);

  float x,y;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;
  u32 color = colorLerp(colorText, 0, lerp);

  // draw name
  x = (rect->TopLeft[0] * SCREEN_WIDTH) + 5;
  y = (rect->TopLeft[1] * SCREEN_HEIGHT) + 5;
  gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0, FONT_BOLD);

  // draw value
  x = (rect->TopRight[0] * SCREEN_WIDTH) - 5;
  gfxScreenSpaceText(x, y, 1, 1, color, *(char*)element->userdata ? "On" : "Off", -1, 2, FONT_BOLD);
}

//------------------------------------------------------------------------------
void drawToggleInvertedMenuElement(TabElem_t* tab, MenuElem_t* element, RECT* rect)
{
  // get element state
  int state = getMenuElementState(tab, element);

  float x,y;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;
  u32 color = colorLerp(colorText, 0, lerp);

  // draw name
  x = (rect->TopLeft[0] * SCREEN_WIDTH) + 5;
  y = (rect->TopLeft[1] * SCREEN_HEIGHT) + 5;
  gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0, FONT_BOLD);

  // draw value
  x = (rect->TopRight[0] * SCREEN_WIDTH) - 5;
  gfxScreenSpaceText(x, y, 1, 1, color, *(char*)element->userdata ? "Off" : "On", -1, 2, FONT_BOLD);
}

//------------------------------------------------------------------------------
void drawRangeMenuElement(TabElem_t* tab, MenuElem_t* element, MenuElem_RangeData_t * rangeData, RECT* rect)
{
  char buf[64];

  // get element state
  int state = getMenuElementState(tab, element);

  float x,y,w,v,h;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;
  u32 color = colorLerp(colorText, 0, lerp);

  // draw name
  x = (rect->TopLeft[0] * SCREEN_WIDTH) + 5;
  y = (rect->TopLeft[1] * SCREEN_HEIGHT) + 5;
  gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0, FONT_BOLD);

  // draw box
  // stepValue doesn't work yet.
  // int trueValue = (*rangeData->value * rangeData->stepValue);
  // printf("\nv: %d, s: %d, vs: %d", *rangeData->value, rangeData->stepValue, trueValue);
  u32 barColor = colorLerp(RangeBar_IsSelected, 0, lerp);
  v = (float)(*rangeData->value - rangeData->minValue) / (float)(rangeData->maxValue - rangeData->minValue);
  w = (rect->TopRight[0] - rect->TopLeft[0]) * 0.5 * SCREEN_WIDTH;
  x = (rect->TopRight[0] * SCREEN_WIDTH) - w - 5;
  h = (rect->BottomRight[1] - rect->TopRight[1]) * SCREEN_HEIGHT;
  gfxPixelSpaceBox(x, y - 4, w * v, h - 2, barColor);

  // draw name
  if (strlen(rangeData->appendText) > 0)
    sprintf(buf, "%d%s", *rangeData->value, rangeData->appendText);
  else
    sprintf(buf, "%d", *rangeData->value);

    x = (rect->TopRight[0] * SCREEN_WIDTH) - 5;
  gfxScreenSpaceText(x, y, 1, 1, color, buf, -1, 2, FONT_BOLD);
}

//------------------------------------------------------------------------------
void drawListMenuElement(TabElem_t* tab, MenuElem_t* element, MenuElem_ListData_t * listData, RECT* rect)
{
  // get element state
  int state = getMenuElementState(tab, element);

  float x,y;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;
  u32 color = colorLerp(colorText, 0, lerp);

  // draw name
  x = (rect->TopLeft[0] * SCREEN_WIDTH) + 5;
  y = (rect->TopLeft[1] * SCREEN_HEIGHT) + 5;
  gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0, FONT_BOLD);

  // draw value
  x = (rect->TopRight[0] * SCREEN_WIDTH) - 5;
  gfxScreenSpaceText(x, y, 1, 1, color, listData->items[(int)*listData->value], -1, 2, FONT_BOLD);
}

//------------------------------------------------------------------------------
void drawListVerticalMenuElement(TabElem_t* tab, MenuElem_t* element, MenuElem_ListData_t * listData, int drawIdx, int itemIdx, RECT* rect)
{
  RECT r;
  memcpy(&r, rect, sizeof(r));
  float yOff = drawIdx * LINE_HEIGHT;

  // get element state
  int state = getMenuElementState(tab, element);

  int isSelectedIdx = (int)*listData->value == itemIdx;
  float x,y;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;
  u32 color = colorLerp(colorText, 0, lerp);

  // draw name
  if (drawIdx == 0 && 0) {
    x = (r.TopLeft[0] * SCREEN_WIDTH) + 5;
    y = ((r.TopLeft[1] + r.BottomLeft[1]) * 0.5 * SCREEN_HEIGHT) + 5;
    gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0, FONT_BOLD);
  }

  // draw value
  x = (r.TopRight[0] * SCREEN_WIDTH) - 5;
  y = ((r.TopLeft[1] + yOff) * SCREEN_HEIGHT) + 5;
  gfxScreenSpaceText(x, y, 1, 1, color, listData->items[itemIdx], -1, TEXT_ALIGN_TOPRIGHT, FONT_BOLD);
}

//------------------------------------------------------------------------------
void drawListVerticalMenuElementInfo(TabElem_t* tab, MenuElem_t* element, MenuElem_ListData_t * listData, RECT* rect)
{
  RECT r;
  memcpy(&r, rect, sizeof(r));

  // get element state
  int state = getMenuElementState(tab, element);
  int selIdx = (int)*listData->value;

  float x,y,w,h;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;
  u32 color = colorLerp(colorText, 0, lerp);

  // get info
  if (mapOverrideLastSelectedMapIdx != selIdx) {
    mapOverrideLastSelectedMapIdx = selIdx;
    mapOverrideSelectedMapTicks = 0;

    // read author/description
    if (selIdx <= 0 || !mapReadCustomMapAuthorDescription(CustomMapDefs[selIdx-1].Filename, mapOverrideSelectedMapAuthor, mapOverrideSelectedMapDesc)) {
      mapOverrideSelectedMapAuthor[0] = 0;
      mapOverrideSelectedMapDesc[0] = 0;
    }

    // alloc thumbnail -- only in menus
    if (!mapOverrideSelectedMapThumbnail && isInMenus()) {
      mapOverrideSelectedMapThumbnail = malloc(THUMBNAIL_SIZE);
    }

    // try read thumbnail
    mapOverrideSelectedMapHasThumbnail = 0;
    if (selIdx > 0 && mapOverrideSelectedMapThumbnail && mapReadCustomMapThumbnail(CustomMapDefs[selIdx-1].Filename, mapOverrideSelectedMapThumbnail, THUMBNAIL_SIZE) == THUMBNAIL_SIZE) {
      mapOverrideSelectedMapHasThumbnail = 1;
    }
  }

  // draw info
  u32 infoBgColor = 0x800C2241;
  gfxScreenSpaceQuad(&r, infoBgColor, infoBgColor, infoBgColor, infoBgColor);

  // draw thumb
  if (mapOverrideSelectedMapHasThumbnail && mapOverrideSelectedMapThumbnail) {
    u16 ulog = 7;
    u16 vlog = 6;
    int r1 = gfxLoadPalToGs(mapOverrideSelectedMapThumbnail + 0x020, 0x13);
    int r2 = gfxLoadTexToGs(mapOverrideSelectedMapThumbnail + 0x420, ulog, vlog, 0x13);
    u64 t = gfxConstructEffectTex(r2, r1, ulog, vlog, 0x13);
      
    x = (r.TopLeft[0] * SCREEN_WIDTH) - 0;
    y = ((r.TopLeft[1] + 0) * SCREEN_HEIGHT) + 0;
    w = (r.TopRight[0] - r.TopLeft[0]) * SCREEN_WIDTH;
    h = w * 0.5;

    gfxSetupGifPaging(0);
    gfxDrawSprite(x, y, w, h, 0, 0, 1 << ulog, 1 << vlog, 0x80808080, t);
    gfxDoGifPaging();
  }
  
  // draw description
  if (mapOverrideSelectedMapDesc[0]) {
    mapOverrideSelectedMapTicks += 1;
    w = (r.TopRight[0] - r.TopLeft[0]) * SCREEN_WIDTH;
    h = (r.BottomLeft[1] - r.TopLeft[1] - LINE_HEIGHT) * SCREEN_HEIGHT - w*0.5;
    x = (r.TopLeft[0] * SCREEN_WIDTH) - 0;
    y = ((r.TopLeft[1] + 0) * SCREEN_HEIGHT) + w*0.5;
    gfxSetScissor(x, x + w, y, y + h);
    y -= minf(64, maxf(0, (mapOverrideSelectedMapTicks%(7*TPS) - 3*TPS) * (float)0.5));
    gfxHelperDrawTextWindow(x, y, w, SCREEN_HEIGHT, 5, 5
      , 0.9, color
      , mapOverrideSelectedMapDesc, -1
      , TEXT_ALIGN_TOPLEFT, FONT_WINDOW_FLAGS_NO_SCISSOR);
    gfxSetScissor(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
  }
    
  // draw author
  if (mapOverrideSelectedMapAuthor[0]) {
    x = (r.BottomLeft[0] * SCREEN_WIDTH) + 5;
    y = ((r.BottomLeft[1] + 0) * SCREEN_HEIGHT) - 2;
    char authorBuf[40];
    snprintf(authorBuf, sizeof(authorBuf), "by %s", mapOverrideSelectedMapAuthor);
    gfxScreenSpaceBox(r.BottomLeft[0], r.BottomLeft[1] - LINE_HEIGHT, r.BottomRight[0] - r.BottomLeft[0], LINE_HEIGHT, 0x80102846);
    gfxScreenSpaceText(x, y, 1, 1, color, authorBuf, -1, TEXT_ALIGN_BOTTOMLEFT, FONT_BOLD);
  }
}

//------------------------------------------------------------------------------
void drawOrderedListMenuElement(TabElem_t* tab, MenuElem_t* element, MenuElem_OrderedListData_t * listData, RECT* rect)
{
  // get element state
  int state = getMenuElementState(tab, element);

  int selectedIdx = (int)*listData->value;
  if (selectedIdx < 0)
    selectedIdx = 0;
  
  float x,y;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;
  u32 color = colorLerp(colorText, 0, lerp);

  // draw name
  x = (rect->TopLeft[0] * SCREEN_WIDTH) + 5;
  y = (rect->TopLeft[1] * SCREEN_HEIGHT) + 5;
  gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0, FONT_BOLD);

  // find name
  int i;
  for (i = 0; i < listData->count; ++i) {
    if (listData->items[i].value == selectedIdx) break;
  }

  // invalid
  if (i >= listData->count) return;

  // draw value
  x = (rect->TopRight[0] * SCREEN_WIDTH) - 5;
  gfxScreenSpaceText(x, y, 1, 1, color, listData->items[i].name, -1, 2, FONT_BOLD);
}

//------------------------------------------------------------------------------
void drawButtonMenuElement(TabElem_t* tab, MenuElem_t* element, RECT* rect)
{
  // get element state
  int state = getMenuElementState(tab, element);

  float x,y,b = 0.005;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;
  u32 color;
  RECT rBg = {
    { rect->TopLeft[0] + 0.05, rect->TopLeft[1] },
    { rect->TopRight[0] - 0.05, rect->TopRight[1] },
    { rect->BottomLeft[0] + 0.05, rect->BottomLeft[1] },
    { rect->BottomRight[0] - 0.05, rect->BottomRight[1] },
  };
  RECT rFg = {
    { rBg.TopLeft[0] + b, rBg.TopLeft[1] + b },
    { rBg.TopRight[0] - b, rBg.TopRight[1] + b },
    { rBg.BottomLeft[0] + b, rBg.BottomLeft[1] - b },
    { rBg.BottomRight[0] - b, rBg.BottomRight[1] - b },
  };

  // bg
  color = colorLerp(colorButtonBg, 0, lerp);
	gfxScreenSpaceQuad(&rBg, color, color, color, color);

  // fg
  color = colorLerp(colorButtonFg, 0, lerp);
	gfxScreenSpaceQuad(&rFg, color, color, color, color);

  // draw name
  x = 0.5 * SCREEN_WIDTH;
  y = ((rFg.TopLeft[1] + rFg.BottomLeft[1]) * SCREEN_HEIGHT * 0.5);
  gfxScreenSpaceText(x, y, 1, 1, colorLerp(colorText, 0, lerp), element->name, -1, 4, FONT_BOLD);

  // add some padding
  rect->TopLeft[1] += 0.01;
  rect->TopRight[1] += 0.01;
}

//------------------------------------------------------------------------------
void drawLabelMenuElement(TabElem_t* tab, MenuElem_t* element, RECT* rect)
{
  // get element state
  int state = getMenuElementState(tab, element);

  float x,y;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;

  // draw label
  x = 0.5 * SCREEN_WIDTH;
  y = ((rect->TopLeft[1] + rect->BottomLeft[1]) * SCREEN_HEIGHT * 0.5);
  gfxScreenSpaceText(x, y, 1, 1, colorLerp(colorText, 0, lerp), element->name, -1, 4, FONT_BOLD);
}

void listVerticalInput(TabElem_t* tab)
{
  int i;
  if (!tab)
    return;

  MenuElem_t *currentElement = &tab->elements[tab->selectedMenuItemIdx];
  int state = getMenuElementState(tab, currentElement);

  // nav page down
  if (padGetButtonUp(0, PAD_RIGHT) > 0)
  {
    for (i = 0; i < 5; ++i)
      currentElement->handler(tab, currentElement, ACTIONTYPE_INCREMENT, NULL);
  }
  // nav up
  else if (padGetButtonUp(0, PAD_LEFT) > 0)
  {
    for (i = 0; i < 5; ++i)
      currentElement->handler(tab, currentElement, ACTIONTYPE_DECREMENT, NULL);
  }
  // nav select secondary
  else if (padGetButtonDown(0, PAD_SQUARE) > 0)
  {
    if (state & ELEMENT_EDITABLE)
      currentElement->handler(tab, currentElement, ACTIONTYPE_SELECT_SECONDARY, NULL);
  }
  // nav inc
  else if (padGetButtonUp(0, PAD_DOWN) > 0)
  {
    if (state & ELEMENT_EDITABLE)
      currentElement->handler(tab, currentElement, ACTIONTYPE_INCREMENT, NULL);
  }
  // nav dec
  else if (padGetButtonUp(0, PAD_UP) > 0)
  {
    if (state & ELEMENT_EDITABLE)
      currentElement->handler(tab, currentElement, ACTIONTYPE_DECREMENT, NULL);
  }
}

//------------------------------------------------------------------------------
void buttonActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  // get element state
  int state = getMenuElementState(tab, element);

  // do nothing if hidden
  if ((state & ELEMENT_VISIBLE) == 0)
    return;

  switch (actionType)
  {
    case ACTIONTYPE_SELECT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;

      if (element->userdata)
        ((ButtonSelectHandler)element->userdata)(tab, element);
      break;
    }
    case ACTIONTYPE_GETHEIGHT:
    {
      *(float*)actionArg = LINE_HEIGHT * 2;
      break;
    }
    case ACTIONTYPE_DRAW:
    {
      drawButtonMenuElement(tab, element, (RECT*)actionArg);
      break;
    }
    case ACTIONTYPE_DRAW_HIGHLIGHT:
    {
      gfxScreenSpaceQuad((RECT*)actionArg, colorSelected, colorSelected, colorSelected, colorSelected);
      break;
    }
    case ACTIONTYPE_INPUT:
    {
      tabInput(tab);
      break;
    }
  }
}

//------------------------------------------------------------------------------
void labelActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  // get element state
  int state = getMenuElementState(tab, element);

  // do nothing if hidden
  if ((state & ELEMENT_VISIBLE) == 0)
    return;

  switch (actionType)
  {
    case ACTIONTYPE_GETHEIGHT:
    {
      switch ((int)element->userdata)
      {
        case LABELTYPE_LABEL:
        {
          *(float*)actionArg = LINE_HEIGHT * 0.75;
          break;
        }
        default:
        {
          *(float*)actionArg = LINE_HEIGHT * 2;
          break;
        }
      }
      break;
    }
    case ACTIONTYPE_DRAW:
    {
      drawLabelMenuElement(tab, element, (RECT*)actionArg);
      break;
    }
    case ACTIONTYPE_DRAW_HIGHLIGHT:
    {
      gfxScreenSpaceQuad((RECT*)actionArg, colorSelected, colorSelected, colorSelected, colorSelected);
      break;
    }
    case ACTIONTYPE_INPUT:
    {
      tabInput(tab);
      break;
    }
  }
}

//------------------------------------------------------------------------------
void rangeActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  MenuElem_RangeData_t* rangeData = (MenuElem_RangeData_t*)element->userdata;

  // get element state
  int state = getMenuElementState(tab, element);

  // do nothing if hidden
  if ((state & ELEMENT_VISIBLE) == 0)
    return;

  switch (actionType)
  {
    case ACTIONTYPE_INCREMENT:
    case ACTIONTYPE_SELECT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;
      char newValue = *rangeData->value + 1;
      if (newValue > rangeData->maxValue)
        newValue = rangeData->minValue;

      *rangeData->value = newValue;
      break;
    }
    case ACTIONTYPE_SELECT_SECONDARY:
    {
      *rangeData->value = (rangeData->minValue + rangeData->maxValue) / 2;
      break;
    }
    case ACTIONTYPE_DECREMENT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;
      char newValue = *rangeData->value - 1;
      if (newValue < rangeData->minValue)
        newValue = rangeData->maxValue;

      *rangeData->value = newValue;
      break;
    }
    case ACTIONTYPE_GETHEIGHT:
    {
      *(float*)actionArg = LINE_HEIGHT;
      break;
    }
    case ACTIONTYPE_DRAW:
    {
      drawRangeMenuElement(tab, element, rangeData, (RECT*)actionArg);
      break;
    }
    case ACTIONTYPE_DRAW_HIGHLIGHT:
    {
      gfxScreenSpaceQuad((RECT*)actionArg, colorSelected, colorSelected, colorSelected, colorSelected);
      break;
    }
    case ACTIONTYPE_VALIDATE:
    {
      if (rangeData->stateHandler != NULL)
        rangeData->stateHandler(rangeData, rangeData->value);
      break;
    }
    case ACTIONTYPE_INPUT:
    {
      tabInput(tab);
      break;
    }
  }
}

//------------------------------------------------------------------------------
void listActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  MenuElem_ListData_t* listData = (MenuElem_ListData_t*)element->userdata;
  int itemCount = listData->count;

  // get element state
  int state = getMenuElementState(tab, element);

  // do nothing if hidden
  if ((state & ELEMENT_VISIBLE) == 0)
    return;

  switch (actionType)
  {
    case ACTIONTYPE_INCREMENT:
    case ACTIONTYPE_SELECT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;
      char newValue = *listData->value;

      do
      {
        newValue += 1;
        if (newValue >= itemCount)
          newValue = 0;
        char tValue = newValue;
        if (listData->stateHandler == NULL || listData->stateHandler(listData, &tValue))
          break;
      } while (newValue != *listData->value);

      *listData->value = newValue;
      break;
    }
    case ACTIONTYPE_DECREMENT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;
      char newValue = *listData->value;

      do
      {
        newValue -= 1;
        if (newValue < 0)
          newValue = itemCount - 1;
        char tValue = newValue;
        if (listData->stateHandler == NULL || listData->stateHandler(listData, &tValue))
          break;
      } while (newValue != *listData->value);

      *listData->value = newValue;
      break;
    }
    case ACTIONTYPE_GETHEIGHT:
    {
      *(float*)actionArg = LINE_HEIGHT;
      break;
    }
    case ACTIONTYPE_DRAW:
    {
      drawListMenuElement(tab, element, listData, (RECT*)actionArg);
      break;
    }
    case ACTIONTYPE_DRAW_HIGHLIGHT:
    {
      gfxScreenSpaceQuad((RECT*)actionArg, colorSelected, colorSelected, colorSelected, colorSelected);
      break;
    }
    case ACTIONTYPE_VALIDATE:
    {
      if (listData->stateHandler != NULL)
        listData->stateHandler(listData, listData->value);
      break;
    }
    case ACTIONTYPE_INPUT:
    {
      tabInput(tab);
      break;
    }
  }
}

//------------------------------------------------------------------------------
void orderedListActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  MenuElem_OrderedListData_t* listData = (MenuElem_OrderedListData_t*)element->userdata;
  int itemCount = listData->count;

  // get element state
  int state = getMenuElementState(tab, element);

  // do nothing if hidden
  if ((state & ELEMENT_VISIBLE) == 0)
    return;

  switch (actionType)
  {
    case ACTIONTYPE_INCREMENT:
    case ACTIONTYPE_SELECT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;
      char value = *listData->value;

      int index = 0;
      for (index = 0; index < listData->count; ++index) {
        if (listData->items[index].value == value) {
          break;
        }
      }

      int startIndex = index;
      do
      {
        index += 1;
        if (index >= listData->count)
          index = 0;
        char tValue = listData->items[index].value;
        if (listData->items[index].name && (listData->stateHandler == NULL || listData->stateHandler(listData, &tValue)))
          break;
      } while (index != startIndex);

      *listData->value = listData->items[index].value;
      break;
    }
    case ACTIONTYPE_DECREMENT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;
      char value = *listData->value;

      int index = 0;
      for (index = 0; index < listData->count; ++index) {
        if (listData->items[index].value == value) {
          break;
        }
      }

      int startIndex = index;
      do
      {
        index -= 1;
        if (index < 0)
          index = listData->count - 1;
        char tValue = listData->items[index].value;
        if (listData->items[index].name && (listData->stateHandler == NULL || listData->stateHandler(listData, &tValue)))
          break;
      } while (index != startIndex);

      *listData->value = listData->items[index].value;
      break;
    }
    case ACTIONTYPE_GETHEIGHT:
    {
      *(float*)actionArg = LINE_HEIGHT;
      break;
    }
    case ACTIONTYPE_DRAW:
    {
      drawOrderedListMenuElement(tab, element, listData, (RECT*)actionArg);
      break;
    }
    case ACTIONTYPE_DRAW_HIGHLIGHT:
    {
      gfxScreenSpaceQuad((RECT*)actionArg, colorSelected, colorSelected, colorSelected, colorSelected);
      break;
    }
    case ACTIONTYPE_VALIDATE:
    {
      if (listData->stateHandler != NULL)
        listData->stateHandler(listData, listData->value);
      break;
    }
    case ACTIONTYPE_INPUT:
    {
      tabInput(tab);
      break;
    }
  }
}

int listFindNextValidValue(MenuElem_ListData_t* listData, int currentValue, int direction)
{
  char newValue = currentValue;

  do
  {
    newValue += direction;
    if (newValue < 0) newValue += listData->count;
    if (newValue >= listData->count) newValue = 0;

    char tValue = newValue;
    if (listData->stateHandler == NULL || listData->stateHandler(listData, &tValue))
      break;
  } while (newValue != currentValue);

  return newValue;
}

void listVerticalActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  MenuElem_ListData_t* listData = (MenuElem_ListData_t*)element->userdata;
  int itemCount = listData->count;
  int itemsToDraw = (&tab->elements[tab->selectedMenuItemIdx] == element) ? (listData->rows ? listData->rows : 5) : 1;

  // Initialize custom maps on first access (safe mode)
  // if (listData == &dataCustomMaps && isInMenus()) {
  //   if (CustomMapDefs == NULL)
  //     refreshCustomMapList();
  // }

  // get element state
  int state = getMenuElementState(tab, element);

  // do nothing if hidden
  if ((state & ELEMENT_VISIBLE) == 0)
    return;

  switch (actionType)
  {
    case ACTIONTYPE_INCREMENT:
    case ACTIONTYPE_SELECT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;
      char newValue = *listData->value;

      do
      {
        newValue += 1;
        if (newValue >= itemCount)
          newValue = 0;
        char tValue = newValue;
        if (listData->stateHandler == NULL || listData->stateHandler(listData, &tValue))
          break;
      } while (newValue != *listData->value);

      *listData->value = newValue;
      break;
    }
    case ACTIONTYPE_DECREMENT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;
      char newValue = *listData->value;

      do
      {
        newValue -= 1;
        if (newValue < 0)
          newValue = itemCount - 1;
        char tValue = newValue;
        if (listData->stateHandler == NULL || listData->stateHandler(listData, &tValue))
          break;
      } while (newValue != *listData->value);

      *listData->value = newValue;
      break;
    }
    case ACTIONTYPE_SELECT_SECONDARY:
    {
      *listData->value = 0;
      break;
    }
    case ACTIONTYPE_GETHEIGHT:
    {
      *(float*)actionArg = LINE_HEIGHT * itemsToDraw;
      break;
    }
    case ACTIONTYPE_DRAW:
    {
      int i;
      int itemCount = listData->count;
      int halfToDraw = itemsToDraw / 2;
      int roll = 0;
      int lastIdx = *listData->value;
      RECT rectLeft, rectRight;

      // create map list rect
      memcpy(&rectLeft, (RECT*)actionArg, sizeof(rectLeft));
      rectLeft.TopRight[0] *= 0.5;
      rectLeft.BottomRight[0] *= 0.5;
      
      // create map info rect
      memcpy(&rectRight, (RECT*)actionArg, sizeof(rectRight));
      rectRight.TopLeft[0] = rectLeft.TopRight[0];
      rectRight.BottomLeft[0] = rectLeft.BottomRight[0];

      // draw items up
      for (i = 0; i < halfToDraw; ++i) {
        lastIdx = listFindNextValidValue(listData, lastIdx, -1);
        drawListVerticalMenuElement(tab, element, listData, halfToDraw - i - 1, lastIdx, &rectRight);
      }
      
      // draw selected item
      lastIdx = *listData->value;
      drawListVerticalMenuElement(tab, element, listData, halfToDraw, lastIdx, &rectRight);
      ++i;

      // draw items down
      for (; i < itemsToDraw; ++i) {
        lastIdx = listFindNextValidValue(listData, lastIdx, 1);
        drawListVerticalMenuElement(tab, element, listData, i, lastIdx, &rectRight);
      }

      // draw info box
      drawListVerticalMenuElementInfo(tab, element, listData, &rectLeft);
      break;
    }
    case ACTIONTYPE_DRAW_HIGHLIGHT:
    {
      RECT r;
      memcpy(&r, (RECT*)actionArg, sizeof(r));
      float y = r.TopLeft[1] + (itemsToDraw / 2) * LINE_HEIGHT;

      r.TopLeft[1] = y;
      r.TopRight[1] = y;
      r.BottomLeft[1] = y + LINE_HEIGHT;
      r.BottomRight[1] = y + LINE_HEIGHT;
      r.TopLeft[0] = r.TopRight[0] * 0.5;
      r.BottomLeft[0] = r.BottomRight[0] * 0.5;

      gfxScreenSpaceQuad(&r, colorSelected, colorSelected, colorSelected, colorSelected);
      break;
    }
    case ACTIONTYPE_VALIDATE:
    {
      if (listData->stateHandler != NULL)
        listData->stateHandler(listData, listData->value);
      break;
    }
    case ACTIONTYPE_INPUT:
    {
      listVerticalInput(tab);
      break;
    }
  }
}

//------------------------------------------------------------------------------
void gmOverrideListActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  // update name to be based on current gamemode
  GameSettings* gs = gameGetSettings();
  if (gs && actionType == ACTIONTYPE_DRAW)
    snprintf(element->name, 40, "%s override", gameGetGameModeName(gs->GameType));

  // pass to default list action handler
  orderedListActionHandler(tab, element, actionType, actionArg);
}

//------------------------------------------------------------------------------
void toggleInvertedActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  // get element state
  int state = getMenuElementState(tab, element);

  // do nothing if hidden
  if ((state & ELEMENT_VISIBLE) == 0)
    return;

  switch (actionType)
  {
    case ACTIONTYPE_INCREMENT:
    case ACTIONTYPE_SELECT:
    case ACTIONTYPE_DECREMENT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;

      // toggle
      *(char*)element->userdata = !(*(char*)element->userdata);
      break;
    }
    case ACTIONTYPE_GETHEIGHT:
    {
      *(float*)actionArg = LINE_HEIGHT;
      break;
    }
    case ACTIONTYPE_DRAW:
    {
      drawToggleInvertedMenuElement(tab, element, (RECT*)actionArg);
      break;
    }
    case ACTIONTYPE_DRAW_HIGHLIGHT:
    {
      gfxScreenSpaceQuad((RECT*)actionArg, colorSelected, colorSelected, colorSelected, colorSelected);
      break;
    }
    case ACTIONTYPE_INPUT:
    {
      tabInput(tab);
      break;
    }
  }
}

//------------------------------------------------------------------------------
void toggleActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  // get element state
  int state = getMenuElementState(tab, element);

  // do nothing if hidden
  if ((state & ELEMENT_VISIBLE) == 0)
    return;

  switch (actionType)
  {
    case ACTIONTYPE_INCREMENT:
    case ACTIONTYPE_SELECT:
    case ACTIONTYPE_DECREMENT:
    {
      if ((state & ELEMENT_EDITABLE) == 0)
        break;

      // toggle
      *(char*)element->userdata = !(*(char*)element->userdata);
      break;
    }
    case ACTIONTYPE_GETHEIGHT:
    {
      *(float*)actionArg = LINE_HEIGHT;
      break;
    }
    case ACTIONTYPE_DRAW:
    {
      drawToggleMenuElement(tab, element, (RECT*)actionArg);
      break;
    }
    case ACTIONTYPE_DRAW_HIGHLIGHT:
    {
      gfxScreenSpaceQuad((RECT*)actionArg, colorSelected, colorSelected, colorSelected, colorSelected);
      break;
    }
    case ACTIONTYPE_INPUT:
    {
      tabInput(tab);
      break;
    }
  }
}

//------------------------------------------------------------------------------
void drawFrame(void)
{
  int i;
  TabElem_t * tab = NULL;
  int state = 0;
  float tabX = frameX;
  float tabY = frameY + frameTitleH;

  // bg
  gfxScreenSpaceBox(frameX, frameY, frameW, frameH, colorBg);

  // title bg
  gfxScreenSpaceBox(frameX, frameY, frameW, frameTitleH, colorRed);

  // title
  gfxScreenSpaceText(0.5 * SCREEN_WIDTH, (frameY + frameTitleH * 0.5) * SCREEN_HEIGHT, 1, 1, colorText, "Patch Config Menu", -1, 4, FONT_BOLD);

  // footer bg
  gfxScreenSpaceBox(frameX, frameY + frameH - frameFooterH, frameW, frameFooterH, colorRed);

  // footer
  gfxScreenSpaceText(((frameX + frameW) * SCREEN_WIDTH) - 5, (frameY + frameH) * SCREEN_HEIGHT - 5, 1, 1, colorText, footerText, -1, 8, FONT_BOLD);

  // content bg
  gfxScreenSpaceBox(frameX + contentPaddingX, frameY + frameTitleH + tabBarH + contentPaddingY, frameW - (contentPaddingX*2), frameH - frameTitleH - tabBarH - frameFooterH - (contentPaddingY * 2), colorContentBg);

  // tab bar
  gfxScreenSpaceBox(tabX, tabY, frameW, tabBarH, colorTabBarBg);

  // tabs
  for (i = 0; i < tabsCount; ++i)
  {
    // get tab state
    tab = &tabElements[i];
    tab->stateHandler(tab, &state);

    // skip hidden elements
    if (state & ELEMENT_VISIBLE)
    {
      // get tab title width
      float pWidth = (4 * tabBarPaddingX) + gfxGetFontWidth(tab->name, -1, 1) / (float)SCREEN_WIDTH;

      // get color
      float lerp = state & ELEMENT_EDITABLE ? 0.0 : 0.5;
      u32 color = colorLerp(colorText, 0, lerp);

      // draw bar
      u32 barColor = selectedTabItem == i ? colorTabBarSelectedBg : colorTabBg;
      gfxScreenSpaceBox(tabX + tabBarPaddingX, tabY, pWidth - (2 * tabBarPaddingX), tabBarH, barColor);

      // draw text
      gfxScreenSpaceText((tabX + 2*tabBarPaddingX) * SCREEN_WIDTH, (tabY + (0.5 * tabBarH)) * SCREEN_HEIGHT, 1, 1, color, tab->name, -1, 3, FONT_BOLD);

      // increment X
      tabX += pWidth - tabBarPaddingX;
    }
  }
}

void tabInput(TabElem_t* tab)
{
  int i;
  if (!tab)
    return;

  MenuElem_t *currentElement = &tab->elements[tab->selectedMenuItemIdx];
  int state = getMenuElementState(tab, currentElement);

  // nav down
  if (padGetButtonUp(0, PAD_DOWN) > 0)
  {
    navMenu(tab, 1, 0);
  }
  // nav page down
  if (padGetButtonUp(0, PAD_R2) > 0)
  {
    for (i = 0; i < 10; ++i)
      navMenu(tab, 1, 0);
  }
  // nav up
  else if (padGetButtonUp(0, PAD_UP) > 0)
  {
    navMenu(tab, -1, 0);
  }
  // nav up
  else if (padGetButtonUp(0, PAD_L2) > 0)
  {
    for (i = 0; i < 10; ++i)
      navMenu(tab, -1, 0);
  }
  // nav select
  else if (padGetButtonDown(0, PAD_CROSS) > 0)
  {
    if (state & ELEMENT_EDITABLE)
      currentElement->handler(tab, currentElement, ACTIONTYPE_SELECT, NULL);
  }
  // nav select secondary
  else if (padGetButtonDown(0, PAD_SQUARE) > 0)
  {
    if (state & ELEMENT_EDITABLE)
      currentElement->handler(tab, currentElement, ACTIONTYPE_SELECT_SECONDARY, NULL);
  }
  // nav inc
  else if (padGetButtonUp(0, PAD_RIGHT) > 0)
  {
    if (state & ELEMENT_EDITABLE)
      currentElement->handler(tab, currentElement, ACTIONTYPE_INCREMENT, NULL);
  }
  // nav dec
  else if (padGetButtonUp(0, PAD_LEFT) > 0)
  {
    if (state & ELEMENT_EDITABLE)
      currentElement->handler(tab, currentElement, ACTIONTYPE_DECREMENT, NULL);
  }
}

//------------------------------------------------------------------------------
void drawTab(TabElem_t* tab)
{
  if (!tab)
    return;

  static int helpLastItemIdx = -1;
  static int helpItemCooldown1 = 0;
  static int helpItemCooldown2 = 0;
  static float helpLastXOffset = 0;

  int i = 0, state = 0;
  int menuElementRenderEnd = tab->menuOffset;
  MenuElem_t * menuElements = tab->elements;
	int menuElementsCount = tab->elementsCount;
  MenuElem_t* currentElement;

  float contentX = frameX + contentPaddingX;
  float contentY = frameY + frameTitleH + tabBarH + contentPaddingY;
  float contentW = frameW - (contentPaddingX * 2);
  float contentH = frameH - frameTitleH - tabBarH - frameFooterH - (contentPaddingY * 2);
  RECT drawRect = {
    { contentX, contentY },
    { contentX + contentW, contentY },
    { contentX, contentY },
    { contentX + contentW, contentY }
  };

  // draw items
  for (i = tab->menuOffset; i < menuElementsCount; ++i)
  {
    currentElement = &menuElements[i];
    float itemHeight = 0;
    currentElement->handler(tab, currentElement, ACTIONTYPE_GETHEIGHT, &itemHeight);

    // ensure item is within content bounds
    if ((drawRect.BottomLeft[1] + itemHeight) > (contentY + contentH))
      break;

    // set rect to height
    drawRect.BottomLeft[1] = drawRect.TopLeft[1] + itemHeight;
    drawRect.BottomRight[1] = drawRect.TopRight[1] + itemHeight;

    // draw selection
    if (i == tab->selectedMenuItemIdx) {
      state = getMenuElementState(tab, currentElement);
      if (state & ELEMENT_SELECTABLE) {
        RangeBar_IsSelected = colorRangeBarSelected;
        currentElement->handler(tab, currentElement, ACTIONTYPE_DRAW_HIGHLIGHT, &drawRect);

        // draw help text
        if (currentElement->help && strlen(currentElement->help) > 0) {

          if (i != helpLastItemIdx) {
            helpLastItemIdx = i;
            helpLastXOffset = 0;
            helpItemCooldown1 = 60 * 3;
            helpItemCooldown2 = 60 * 6;
          }

          // draw background
          gfxScreenSpaceBox(frameX, frameY + frameH - 1.0/SCREEN_HEIGHT, frameW, LINE_HEIGHT, 0x80000000);

          // set scissor
          gfxSetScissor(
            frameX * SCREEN_WIDTH,
            (frameX + frameW) * SCREEN_WIDTH,
            (frameY + frameH) * SCREEN_HEIGHT,
            (frameY + frameH + LINE_HEIGHT) * SCREEN_HEIGHT);
          
          // get width
          float w = gfxGetFontWidth(currentElement->help, -1, 1) / (float)SCREEN_WIDTH;
          if (helpItemCooldown1) --helpItemCooldown1;
          else if ((helpLastXOffset + w + contentPaddingX*2) >= frameW) helpLastXOffset -= 0.002;
          else if (helpItemCooldown2) --helpItemCooldown2;
          else { helpItemCooldown1 = 60 * 3; helpItemCooldown2 = 60 * 6; helpLastXOffset = 0; }
          gfxScreenSpaceText((frameX + contentPaddingX + helpLastXOffset) * SCREEN_WIDTH, (frameY + frameH) * SCREEN_HEIGHT, 1, 1, 0x80FFFFFF, currentElement->help, -1, 0, FONT_BOLD);

          // reset scissor
          gfxSetScissor(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
        }
      }
    } else {
      RangeBar_IsSelected = colorRangeBar;
    }

    // draw
    currentElement->handler(tab, currentElement, ACTIONTYPE_DRAW, &drawRect);

    // increment rect
    drawRect.TopLeft[1] += itemHeight;
    drawRect.TopRight[1] += itemHeight;

    menuElementRenderEnd = i + 1;
  }
  
  // draw scroll bar
  if (tab->menuOffset > 0 || menuElementRenderEnd < menuElementsCount)
  {
    float scrollValue = tab->menuOffset / (float)(menuElementsCount - (menuElementRenderEnd-tab->menuOffset));
    float scrollBarHeight = 0.05;
    float contentRectHeight = contentH - scrollBarHeight;

    gfxScreenSpaceBox(contentX + contentW, contentY + (scrollValue * contentRectHeight), 0.01, scrollBarHeight, colorRed);
  }

  // 
  if (tab->selectedMenuItemIdx >= menuElementRenderEnd)
    ++tab->menuOffset;
  if (tab->selectedMenuItemIdx < tab->menuOffset)
    tab->menuOffset = tab->selectedMenuItemIdx;

  // get selected element
  if (tab->selectedMenuItemIdx >= menuElementsCount)
    return;

  currentElement = &menuElements[tab->selectedMenuItemIdx];
  state = getMenuElementState(tab, currentElement);

  // find next selectable item if hidden or not selectable
  if ((state & ELEMENT_VISIBLE) == 0 || (state & ELEMENT_SELECTABLE) == 0)
    navMenu(tab, 1, 1);

  if (currentElement)
    currentElement->handler(tab, currentElement, ACTIONTYPE_INPUT, NULL);
}

//------------------------------------------------------------------------------
void onMenuUpdate(int inGame)
{
  TabElem_t* tab = &tabElements[selectedTabItem];

  if (isConfigMenuActive)
  {
    // prevent pad from affecting menus
    padDisableInput();

		// draw
		if (padGetButtonDown(0, PAD_L3) <= 0)
		{
			// draw frame
			drawFrame();

			// draw tab
			drawTab(tab);
		}

		// nav tab right
		if (padGetButtonUp(0, PAD_R1) > 0)
		{
			navTab(1);
		}
		// nav tab left
		else if (padGetButtonUp(0, PAD_L1) > 0)
		{
			navTab(-1);
		}
		// close
		else if (padGetButtonUp(0, PAD_TRIANGLE) > 0 || padGetButtonUp(0, PAD_START) > 0)
		{
      configMenuDisable();
		}
	}
	else if (!inGame)
  {
    // if in Online Lobby, and SubPointer equals zero (not on find game)
		if (uiGetActiveMenu(UI_MENU_ONLINE_LOBBY, 0) != 0 && *(u32*)0x01C5C114 == 0) {
			// render message
			// gfxScreenSpaceBox(SCREEN_WIDTH * 0.2, SCREEN_HEIGHT * 0.81, 0.4, 0.3, colorOpenBg);
      float scale = .85;
	  	gfxScreenSpaceText(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.77, scale, scale, 0x80FFFFFF, "Press START to", -1, 4, FONT_BOLD);
			gfxScreenSpaceText(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.80, scale, scale, 0x80FFFFFF, "Open Config Menu", -1, 4, FONT_BOLD);
    }
    if (uiGetActiveMenu(UI_MENU_STAGING, 0) > 0 && *(u32*)0x01C5C114 == 0) {
      int isTeams = gameGetOptions()->GameFlags.MultiplayerGameFlags.Teams;
      if (gameAmIHost() && isTeams)
        gfxScreenSpaceText(SCREEN_WIDTH * 0.205, SCREEN_HEIGHT * 0.07, 0.7, .85, 0x8069cbf2, "\x14 RANDOM TEAMS", -1, 4, FONT_BOLD);
    }
		// check for pad input
		if (padGetButtonUp(0, PAD_START) > 0)
		{
			configMenuEnable();
		}
	}
}

//------------------------------------------------------------------------------
void navMenu(TabElem_t* tab, int direction, int loop)
{
  int newElement = tab->selectedMenuItemIdx + direction;
  MenuElem_t *elem = NULL;
  int state = 0;

  // handle case where tab has no items
  if (tab->elementsCount == 0)
  {
    tab->selectedMenuItemIdx = 0;
    tab->menuOffset = 0;
    return;
  }

  while (newElement != tab->selectedMenuItemIdx)
  {
    if (newElement >= tab->elementsCount)
    {
      if (loop && tab->selectedMenuItemIdx != 0)
        newElement = 0;
      else
        break;
    }
    else if (newElement < 0)
    {
      if (loop && tab->selectedMenuItemIdx != (tab->elementsCount - 1))
        newElement = tab->elementsCount - 1;
      else
        break;
    }

    // get newly selected element state
    elem = &tab->elements[newElement];
    elem->stateHandler(tab, elem, &state);

    // skip if hidden
    if ((state & ELEMENT_VISIBLE) == 0 || (state & ELEMENT_SELECTABLE) == 0)
    {
      newElement += direction;
      continue;
    }

    // set new tab
    tab->selectedMenuItemIdx = newElement;
    break;
  }
}

void navTab(int direction)
{
  int newTab = selectedTabItem + direction;
  TabElem_t *tab = NULL;
  int state = 0;

  while (1)
  {
    if (newTab >= tabsCount)
      break;
    if (newTab < 0)
      break;
    
    // get new tab state
    tab = &tabElements[newTab];
    tab->stateHandler(tab, &state);

    // skip if hidden
    if ((state & ELEMENT_VISIBLE) == 0 || (state & ELEMENT_SELECTABLE) == 0)
    {
      newTab += direction;
      continue;
    }

    // set new tab
    selectedTabItem = newTab;
    break;
  }
}

//------------------------------------------------------------------------------
int onServerDownloadDataRequest(void * connection, void * data)
{
	ServerDownloadDataRequest_t* request = (ServerDownloadDataRequest_t*)data;

	// copy bytes to target
  dlIsActive = request->Id;
	dlTotalBytes = request->TotalSize;
	dlBytesReceived += request->DataSize;
	memcpy((void*)request->TargetAddress, request->Data, request->DataSize);
	DPRINTF("DOWNLOAD: %d/%d, writing %d to %08X\n", dlBytesReceived, request->TotalSize, request->DataSize, request->TargetAddress);
  
	// respond
	if (connection && (!request->Chunk || dlBytesReceived >= request->TotalSize)) {
		ClientDownloadDataResponse_t response;
		response.Id = request->Id;
		response.BytesReceived = dlBytesReceived;
		netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_DOWNLOAD_DATA_RESPONSE, sizeof(ClientDownloadDataResponse_t), &response);
	}

  // reset at end
  if (dlBytesReceived >= request->TotalSize) {
    dlTotalBytes = 0;
    dlBytesReceived = 0;
    dlIsActive = 0;
  }

	return sizeof(ServerDownloadDataRequest_t) - sizeof(request->Data) + request->DataSize;
}

//------------------------------------------------------------------------------
int onSetGameConfig(void * connection, void * data)
{
  PatchGameConfig_t config;

  // move to temporary object
  memcpy(&config, data, sizeof(PatchGameConfig_t));

  // check for changes
  redownloadCustomModeBinaries |= config.customModeId != gameConfig.customModeId;

  // copy it over
  memcpy(&gameConfig, data, sizeof(PatchGameConfig_t));

  return sizeof(PatchGameConfig_t);
}

//------------------------------------------------------------------------------
void onConfigUpdate(void)
{
  int i;

  // reset when we lose connection
  void* connection = netGetLobbyServerConnection();
  if (dlTotalBytes > 0 && (!connection || !dlIsActive))
  {
    dlTotalBytes = 0;
    dlBytesReceived = 0;
    dlIsActive = 0;
    DPRINTF("lost connection\n");
  }

  // in staging, update game info
  GameSettings * gameSettings = gameGetSettings();
  if (gameSettings && gameSettings->GameLoadStartTime < 0 && netGetLobbyServerConnection())
  {
    // 
    char * mapName = mapGetName(gameSettings->GameLevel);
    char * modeName = gameGetGameModeName(gameSettings->GameType);

    // get map override name
    if (patchStateContainer.CustomMapId > 0)
      mapName = dataCustomMaps.items[patchStateContainer.CustomMapId];
    else if (MapLoaderState.MapName[0])
      mapName = MapLoaderState.MapName;

    // get mode override name
    if (gameConfig.customModeId > 0) {
      modeName = (char*)CustomModeShortNames[(int)gameConfig.customModeId];
      if (!modeName) {
        for (i = 0; i < dataCustomModes.count; ++i) {
          if (dataCustomModes.items[i].value == (int)gameConfig.customModeId) {
            modeName = dataCustomModes.items[i].name;
            break;
          }
        }
      }
    }

    UiMenu_t* uiStaging = uiGetActiveMenu(UI_MENU_STAGING, 0);
    if (uiStaging > 0) {
      u32 * stagingChild = uiStaging->pChildren;
      u32 mapText = (u32)stagingChild[1] + 0x14;
      u32 modeText = (u32)stagingChild[2] + 0x14;
      if (mapName)
        strncpy((char*)mapText, mapName, 32);
      if (modeName)
        strncpy((char*)modeText, modeName, 32);
    }
  }
}

//------------------------------------------------------------------------------
void onConfigGameMenu(void)
{
  onMenuUpdate(1);
}

//------------------------------------------------------------------------------
void onConfigOnlineMenu(void)
{
  // draw download data box
	if (dlTotalBytes > 0)
	{
    gfxScreenSpaceBox(0.2, 0.35, 0.6, 0.125, colorBlack);
    gfxScreenSpaceBox(0.2, 0.45, 0.6, 0.05, colorContentBg);
    gfxScreenSpaceText(SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.4, 1, 1, colorText, "Downloading...", 11 + (gameGetTime()/240 % 4), 3, FONT_BOLD);

		float w = (float)dlBytesReceived / (float)dlTotalBytes;
		gfxScreenSpaceBox(0.2, 0.45, 0.6 * w, 0.05, colorRed);
	}

  onMenuUpdate(0);

  if (isConfigMenuActive && !netGetLobbyServerConnection()) {
    configMenuDisable();
    padEnableInput();
  }
}

//------------------------------------------------------------------------------
void onConfigInitialize(void)
{
	// install net handlers
  netInstallCustomMsgHook(1);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_SERVER_SET_GAME_CONFIG, &onSetGameConfig);
  netInstallCustomMsgHandler(CUSTOM_MSG_ID_SERVER_DOWNLOAD_DATA_REQUEST, &onServerDownloadDataRequest);


  // reset game configs
  memset(&gameConfigHostBackup, 0, sizeof(gameConfigHostBackup));
  memset(&gameConfig, 0, sizeof(gameConfig));

  // set defaults
#if DEFAULT_GAMEMODE > 0
  gameConfigHostBackup.customModeId = DEFAULT_GAMEMODE;
  gameConfig.customModeId = DEFAULT_GAMEMODE;
#endif
}

//------------------------------------------------------------------------------
void configTrySendGameConfig(void)
{
  int state = 0;
  int i = 0, j = 0;

  // send game config to server for saving if tab is enabled
  tabElements[1].stateHandler(&tabElements[1], &state);
  if (state & ELEMENT_EDITABLE)
  {
    // Generate a per-match seed for custom modes that rely on shared RNG (e.g. KOTH hill order).
    // HACK: Pack KOTH hill size into the high nibble so the module can recover it later (no direct call into koth ELF).
    gameConfig.grSeed = ((kothHillSizeIdx & 0xF) << 28) | ((int)gameGetTime() & 0x0FFFFFFF);

    // validate everything
    for (i = 0; i < tabsCount; ++i)
    {
      TabElem_t* tab = &tabElements[i];
      for (j = 0; j < tab->elementsCount; ++j)
      {
        MenuElem_t* elem = &tab->elements[j];
        if (elem->handler)
          elem->handler(tab, elem, ACTIONTYPE_VALIDATE, NULL);
      }
    }

    // backup
    memcpy(&gameConfigHostBackup, &gameConfig, sizeof(PatchGameConfig_t));

    // send
    void * lobbyConnection = netGetLobbyServerConnection();
    if (lobbyConnection) {
      ClientSetGameConfig_t msg;

      memset(&msg, 0, sizeof(msg));
      if (patchStateContainer.CustomMapId > 0)
        memcpy(&msg.CustomMap, &CustomMapDefs[patchStateContainer.CustomMapId-1], sizeof(msg.CustomMap));
      memcpy(&msg.GameConfig, &gameConfig, sizeof(msg.GameConfig));
      netSendCustomAppMessage(lobbyConnection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_USER_GAME_CONFIG, sizeof(ClientSetGameConfig_t), &msg);
    }
  }
}

//------------------------------------------------------------------------------
void configMenuDisable(void)
{
  if (!isConfigMenuActive)
    return;

  isConfigMenuActive = 0;

  // send config to server for saving
  void * lobbyConnection = netGetLobbyServerConnection();
  if (lobbyConnection)
    netSendCustomAppMessage(lobbyConnection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_USER_CONFIG, sizeof(PatchConfig_t), &config);

  // 
  // update and send gameconfig
  if (gameAmIHost())
  {
    // force game config to preset
        switch (preset)
    {
      case 1: // Meta
      {
        // Game Rules
        gameConfig.grRadarBlipsDistance = 0; // Short
        gameConfig.grRespawnTimer_Player = 0; // 1.5 Seconds
        gameConfig.grRespawnInvincibility = 0; // Off
        gameConfig.grDisablePenaltyTimers = 0; // On
        gameConfig.grDisableWeaponPacks = 1; // Off
        gameConfig.grV2s = 0; // On
        gameConfig.grNoCooldown = 1; // Cooldown disabled
        gameConfig.grFluxShotsAlwaysHit = 1; // Flux always hits
        gameConfig.grFluxNikingDisabled = 1; // Niking disabled
        gameConfig.grAutoRespawn = 0; // Off
        gameConfig.grVampire = 0; // Off
        gameConfig.grHealthBoxes = 1; // No glass container
        gameConfig.grDisableWeaponCrates = 1; // Disabled
        gameConfig.grDisableAmmoPickups = 1; // Disabled
        gameConfig.grRespawnTimer_HealthBoxes = 5; // 20 sec
        gameConfig.grRespawnTimer_WeaponCrates = 0; // 30 sec
        gameConfig.grRespawnTimer_AmmoPickups = 0; // 30 sec
        gameConfig.grNewPlayerSync = 1;
        // gameConfig.grDisableDrones = 0; // Keep drones on. Not working yet
        gameConfig.grDisablePlayerTurrets = 1; // Disabled
        gameConfig.grDestructableBridges = 0; // Enabled
        // Base / Node mods
        gameConfig.grSetGatlingTurretHealth = 1; // .5x
        gameConfig.grBaseHealthPadActive = 0; // Off
        gameConfig.grNoBaseDefense_Bots = 1; // Off
        // Party Rules
        gameConfig.prChargebootForever = 0; // Off 
        gameConfig.prLoadoutWeaponsOnly = 0; // Off
        gameConfig.prSurvivor = 0; // Off
        break;
      }
      case 2: // Competitive
      {
        // Game Rules
        gameConfig.grRadarBlipsDistance = 0; // Short
        gameConfig.grRespawnTimer_Player = 0; // 1.5 Seconds
        gameConfig.grRespawnInvincibility = 0; // Off
        gameConfig.grDisablePenaltyTimers = 0; // On
        gameConfig.grDisableWeaponPacks = 1; // Off
        gameConfig.grV2s = 0; // On
        gameConfig.grNoCooldown = 1; // Cooldown disabled
        gameConfig.grFluxShotsAlwaysHit = 1; // Flux always hits
        gameConfig.grFluxNikingDisabled = 1; // Niking disabled
        gameConfig.grAutoRespawn = 0; // Off
        gameConfig.grVampire = 0; // Off
        gameConfig.grHealthBoxes = 1; // No glass container
        gameConfig.grDisableWeaponCrates = 1; // Disabled
        gameConfig.grDisableAmmoPickups = 1; // Disabled
        gameConfig.grRespawnTimer_HealthBoxes = 5; // 20 sec
        gameConfig.grRespawnTimer_WeaponCrates = 0; // 30 sec
        gameConfig.grRespawnTimer_AmmoPickups = 0; // 30 sec
        gameConfig.grNewPlayerSync = 1;
        // gameConfig.grDisableDrones = 0; // Keep drones on. Not working yet
        gameConfig.grDisablePlayerTurrets = 1; // Disabled
        gameConfig.grDestructableBridges = 0; // Enabled
        // Base / Node mods
        gameConfig.grSetGatlingTurretHealth = 0; // Default
        gameConfig.grBaseHealthPadActive = 0; // Off
        gameConfig.grNoBaseDefense_Bots = 1; // Off
        // Party Rules
        gameConfig.prChargebootForever = 0; // Off 
        gameConfig.prLoadoutWeaponsOnly = 0; // Off
        gameConfig.prSurvivor = 0; // Off
        break;
      }
      case 3: // 1v1
      {
        gameConfig.grRadarBlipsDistance = 1; // Always
        gameConfig.grRespawnTimer_Player = 11; // 0 Seconds
        gameConfig.grHealthBoxes = 2; // Disabled
        gameConfig.grDisableWeaponCrates = 1;
        gameConfig.grDisableAmmoPickups = 1;
        gameConfig.grNewPlayerSync = 1;
        // gameConfig.grDisableDrones = 1;
        gameConfig.grDisablePlayerTurrets = 1;
        gameConfig.grDestructableBridges = 0; // Enabled
        gameConfig.grV2s = 1; // Off
        gameConfig.grNoCooldown = 1;
        gameConfig.grVampire = 4; // 100%
        gameConfig.grDisableWeaponPacks = 1;
        gameConfig.grFluxShotsAlwaysHit = 1; // Flux always hits
        gameConfig.grFluxNikingDisabled = 1; // Niking disabled
        break;
      }
      case 4: // Bot
      {
        // Game Rules
        gameConfig.grRadarBlipsDistance = 0; // Short
        gameConfig.grRespawnTimer_Player = 0; // 1.5 Seconds
        gameConfig.grRespawnInvincibility = 0; // Off
        gameConfig.grDisablePenaltyTimers = 0; // On
        gameConfig.grDisableWeaponPacks = 1; // Off
        gameConfig.grV2s = 0; // On
        gameConfig.grNoCooldown = 1; // Cooldown disabled
        gameConfig.grFluxShotsAlwaysHit = 1; // Flux always hits
        gameConfig.grFluxNikingDisabled = 1; // Niking disabled
        gameConfig.grAutoRespawn = 0; // Off
        gameConfig.grVampire = 0; // Off
        gameConfig.grHealthBoxes = 1; // No glass container
        gameConfig.grDisableWeaponCrates = 1; // Disabled
        gameConfig.grDisableAmmoPickups = 1; // Disabled
        gameConfig.grRespawnTimer_HealthBoxes = 5; // 20 sec
        gameConfig.grRespawnTimer_WeaponCrates = 0; // 30 sec
        gameConfig.grRespawnTimer_AmmoPickups = 0; // 30 sec
        gameConfig.grNewPlayerSync = 0;
        // gameConfig.grDisableDrones = 0; // Keep drones on. Not working yet
        gameConfig.grDisablePlayerTurrets = 1; // Disabled
        gameConfig.grDestructableBridges = 1; // Disabled
        // Base / Node mods
        gameConfig.grSetGatlingTurretHealth = 1; // .5x
        gameConfig.grBaseHealthPadActive = 0; // Off
        gameConfig.grNoBaseDefense_Bots = 1; // Off
        // Party Rules
        gameConfig.prChargebootForever = 0; // Off 
        gameConfig.prLoadoutWeaponsOnly = 0; // Off
        gameConfig.prSurvivor = 0; // Off
        break;
      }
      case 5: // Siege
      {
        // Game Rules
        gameConfig.grRadarBlipsDistance = 0; // Short
        gameConfig.grRespawnTimer_Player = 2; // 2.5 Seconds
        gameConfig.grRespawnInvincibility = 0; // Off
        gameConfig.grDisablePenaltyTimers = 0; // On
        gameConfig.grDisableWeaponPacks = 1; // Off
        gameConfig.grV2s = 0; // On
        gameConfig.grNoCooldown = 1; // Cooldown disabled
        gameConfig.grFluxShotsAlwaysHit = 1; // Flux always hits
        gameConfig.grFluxNikingDisabled = 1; // Niking disabled
        gameConfig.grAutoRespawn = 0; // Off
        gameConfig.grVampire = 0; // Off
        gameConfig.grHealthBoxes = 1; // No glass container
        gameConfig.grDisableWeaponCrates = 1; // Disabled
        gameConfig.grDisableAmmoPickups = 1; // Disabled
        gameConfig.grRespawnTimer_HealthBoxes = 5; // 20 sec
        gameConfig.grRespawnTimer_WeaponCrates = 0; // 30 sec
        gameConfig.grRespawnTimer_AmmoPickups = 0; // 30 sec
        gameConfig.grNewPlayerSync = 0; // Off in case of vehicles
        // gameConfig.grDisableDrones = 0; // Keep drones on. Not working yet
        gameConfig.grDisablePlayerTurrets = 1; // Disabled
        gameConfig.grDestructableBridges = 0; // Enabled
        // Siege-specific settings
        gameConfig.grNodeSelectTimer = 7; // 5 seconds to select a node
        gameConfig.grAllNodesTimer = 2; // 1 minutes countdown when all nodes owned
        gameConfig.grSiegeNoTies = 1; // No ties - team with most base damage wins
        gameConfig.grSuicidePenaltyTimer = 3; // 0 seconds suicide penalty
        // Base / Node mods
        gameConfig.grSetGatlingTurretHealth = 4; // 3x
        gameConfig.grBaseHealthPadActive = 0; // Off
        gameConfig.grNoBaseDefense_Bots = 1; // Off
        // Party Rules
        gameConfig.prChargebootForever = 0; // Off 
        gameConfig.prLoadoutWeaponsOnly = 0; // Off
        gameConfig.prSurvivor = 0; // Off
        break;
      }
    }

    // if (gameConfig.prLoadoutWeaponsOnly) {
    //   gameConfig.grDisableWeaponPacks = 1;
    //   gameConfig.grDisableWeaponCrates = 1;
    //   gameConfig.grDisableAmmoPickups = 1;
    // }

    // send
    configTrySendGameConfig();
  }

  // re-enable pad
  padEnableInput();
}

//------------------------------------------------------------------------------
void configMenuEnable(void)
{
  // enable
  isConfigMenuActive = 1;

  // return to first tab if current is hidden
  int state = 0;
  tabElements[selectedTabItem].stateHandler(&tabElements[selectedTabItem], &state);
  if ((state & ELEMENT_SELECTABLE) == 0 || (state & ELEMENT_VISIBLE) == 0)
    selectedTabItem = 0;
}
