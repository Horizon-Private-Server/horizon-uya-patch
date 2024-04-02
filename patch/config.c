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

#define LINE_HEIGHT         (0.05)
#define LINE_HEIGHT_3_2     (0.075)
#define DEFAULT_GAMEMODE    (0)

#if UYA_PAL

#define IS_PROGRESSIVE_SCAN					(*(int*)0x002413a0)
#define IS_WIDESCREEN               (*(char*)0x001a58bc)
#else

#define IS_PROGRESSIVE_SCAN					(*(int*)0x00241520)
#define IS_WIDESCREEN               (*(char*)0x001a5a3c)
#endif

int isConfigMenuActive = 0;
int selectedTabItem = 0;
u32 padPointer = 0;
int SelectedCustomMapId = 0;
int preset = 0;

// Config
extern PatchConfig_t config;

// game config
extern PatchGameConfig_t gameConfig;
extern PatchGameConfig_t gameConfigHostBackup;

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
void rangeActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void gmOverrideListActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);
void labelActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg);

// state handlers
void menuStateAlwaysHiddenHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateAlwaysDisabledHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateAlwaysEnabledHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateEnabledInMenusHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateScavengerHuntEnabledHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuLabelStateHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuLabelStateHandler_BaseDefenses(TabElem_t* tab, MenuElem_t* element, int* state);
void menuLabelStateHandler_CTFandSiege(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_InstallCustomMaps(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_InstalledCustomMaps(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_GameModeOverride(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_BaseDefenses(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_Siege(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_CTF(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_DM(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_CTFandSiege(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_Survivor(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_Default(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_VoteToEndStateHandler(TabElem_t* tab, MenuElem_t* element, int* state);
void menuStateHandler_DisabledInGame(TabElem_t* tab, MenuElem_t* element, int* state);

int menuStateHandler_SelectedMapOverride(MenuElem_ListData_t* listData, char* value);
// int menuStateHandler_SelectedWorldOverride(MenuElem_ListData_t* listData, char* value);
int menuStateHandler_SelectedGameModeOverride(MenuElem_ListData_t* listData, char* value);


// list select handlers
void mapsSelectHandler(TabElem_t* tab, MenuElem_t* element);
void gmResetSelectHandler(TabElem_t* tab, MenuElem_t* element);
void gmRefreshMapsSelectHandler(TabElem_t* tab, MenuElem_t* element);
void voteToEndSelectHandler(TabElem_t* tab, MenuElem_t* element);

#ifdef DEBUG
void downloadPatchSelectHandler(TabElem_t* tab, MenuElem_t* element);
#endif

// tab state handlers
void tabDefaultStateHandler(TabElem_t* tab, int * state);
void tabGameSettingsStateHandler(TabElem_t* tab, int * state);
void tabGameSettingsHelpStateHandler(TabElem_t* tab, int * state);

// navigation functions
void navMenu(TabElem_t* tab, int direction, int loop);
void navTab(int direction);

// extern
int mapsGetInstallationResult(void);
int mapsDownloadingModules(void);
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

// map override list item
MenuElem_ListData_t dataCustomMaps = {
    &SelectedCustomMapId,
    menuStateHandler_SelectedMapOverride,
    1,
    {
      "None",
      [MAX_CUSTOM_MAP_DEFINITIONS+1] NULL,
    }
};

// maps with their own exclusive gamemode
char dataCustomMapsWithExclusiveGameMode[] = {
#if DEBUG
  CUSTOM_MAP_SPLEEF,
#endif
};
const int dataCustomMapsWithExclusiveGameModeCount = sizeof(dataCustomMapsWithExclusiveGameMode)/sizeof(char);

// gamemode override list item
MenuElem_ListData_t dataCustomModes = {
    &gameConfig.customModeId,
    menuStateHandler_SelectedGameModeOverride,
    CUSTOM_MODE_COUNT,
    {
      "None",
#if DEBUG
      "Infected",
      "Juggernaught"
#endif
    }
};

// array of alternative "short" names for game modes
// used when displaying in areas with limited room
// if the entry is NULL then the full name will be used
const char* CustomModeShortNames[] = {
  [CUSTOM_MODE_NONE] NULL,
#if DEBUG
  [CUSTOM_MODE_INFECTED] "Infected",
  [CUSTOM_MODE_JUGGERNAUGHT] NULL,
#endif
};

MenuElem_ListData_t dataWeaponPacks = {
    .value = &gameConfig.grDisableWeaponPacks,
    .stateHandler = NULL,
    .count = 2,
    .items = { "Default", "Off", "On Death" }
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

MenuElem_ListData_t dataSetGattlingTurretHealth = {
    .value = &gameConfig.grSetGattlingTurretHealth,
    .stateHandler = NULL,
    .count = 10,
    .items = { "Default", ".5x", "1.5x", "2x", "3x", "4x", "5x", "6x", "7x", "8x" }
};

MenuElem_ListData_t dataRespawnTimer_Player = {
    .value = &gameConfig.grRespawnTimer_Player,
    .stateHandler = NULL,
    .count = 12,
    .items = { "1.5", "2", "3", "4", "5", "6", "7", "8", "9", "10", "0", "1", }
};

MenuElem_ListData_t dataPlayerSize = {
    .value = &gameConfig.prPlayerSize,
    .stateHandler = NULL,
    .count = 5,
    .items = { "Normal", "Large", "Giant", "Tiny", "Small" }
};

MenuElem_ListData_t dataRadarBlipsDistance = {
    .value = &gameConfig.grRadarBlipsDistance,
    .stateHandler = NULL,
    .count = 3,
    .items = { "Short", "Always", "Off" }
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
    .count = 3,
    .items = { "None", "Competitive", "1v1", }
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

// General
MenuElem_t menuElementsGeneral[] = {
#ifdef DEBUG
  { "Redownload patch", buttonActionHandler, menuStateAlwaysEnabledHandler, downloadPatchSelectHandler },
#endif
  { "Vote to End", buttonActionHandler, menuStateHandler_VoteToEndStateHandler, voteToEndSelectHandler, "Vote to end the game. If a team/player is in the lead they will win." },
  { "Refresh Maps", buttonActionHandler, menuStateEnabledInMenusHandler, gmRefreshMapsSelectHandler, "Refresh the custom map list." },
  // { "Install Custom Maps on Login", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enableAutoMaps },
  { "Participate in Scavenger Hunt", toggleInvertedActionHandler, menuStateScavengerHuntEnabledHandler, &config.disableScavengerHunt, "If you see this option, there is a Horizon scavenger hunt active. Enabling this will spawn random Horizon bolts in game. Collect the most to win the hunt!" },
#if DEBUG
  { "16:9 Widescreen", toggleActionHandler, menuStateAlwaysEnabledHandler, &IS_WIDESCREEN },
  { "Progressive Scan", toggleActionHandler, menuStateAlwaysEnabledHandler, &IS_PROGRESSIVE_SCAN },
#endif
  { "Always Show Health", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.alwaysShowHealth, "Let that health bar hide no longer!  This will make it so the health bar is always visible." },
  { "Camera Pull", toggleInvertedActionHandler, menuStateAlwaysEnabledHandler, &config.aimAssist, "Toggles code that pulls the camera towards nearby targets when aiming." },
  { "Camera Shake", toggleInvertedActionHandler, menuStateAlwaysEnabledHandler, &config.disableCameraShake, "Toggles the camera shake caused by nearby explosions." },
  { "Field of View", rangeActionHandler, menuStateAlwaysEnabledHandler, &dataFieldOfView },
  { "FPS Counter", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enableFpsCounter, "Toggles the in game FPS counter." },
  { "Level of Detail", listActionHandler, menuStateAlwaysEnabledHandler, &dataLevelOfDetail },
  { "Loadout Weapon 1", listActionHandler, menuStateHandler_DisabledInGame, &dataCycleWeapon1, "Main Weapon 1.  Defaults to Gravity Bomb if nothing is selected." },
  { "Loadout Weapon 2", listActionHandler, menuStateHandler_DisabledInGame, &dataCycleWeapon2, "Main Weapon 2.  Defaults to Blitz Cannon if nothing is selected." },
  { "Loadout Weapon 3", listActionHandler, menuStateHandler_DisabledInGame, &dataCycleWeapon3, "Main Weapon 3.  Defaults to Flux Rifle if nothing is selected." },
  { "Spectate", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enableSpectate, "Toggles the custom spectate feature. Use \x13 when dead to spectate.  Some gomemodes may restrict whom you can spectate." },
  { "Quick Select Delay", rangeActionHandler, menuStateAlwaysEnabledHandler, &dataQuickSelectTimeDelay, "Change how short/long you need to hold triangle to open the Quick Select menu." },
  { "Single Player Music", toggleActionHandler, menuStateAlwaysEnabledHandler, &config.enableSingleplayerMusic, "Tired of the same BORING songs in game?!  Spice it up by adding the music tracks from Single Player!" },
  { "Toggle Map Button", listActionHandler, menuStateAlwaysEnabledHandler, &dataMapScore_MapAccess, "The button that you will need to press to show the Map." },
  { "Toggle Scoreboard Button", listActionHandler, menuStateAlwaysEnabledHandler, &dataMapScore_ScoreboardAccess, "The button that you will need to press to show the Scoreboard." },
};

// Game Settings
MenuElem_t menuElementsGameSettings[] = {
  { "Reset", buttonActionHandler, menuStateAlwaysEnabledHandler, gmResetSelectHandler },

  // { "Game Settings", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_HEADER },
  { "Map Override", listActionHandler, menuStateAlwaysEnabledHandler, &dataCustomMaps, "Play on any of the custom maps from the Horizon Map Pack. Visit https://rac-horizon.com to download the map pack." },
  { "Gamemode Override", gmOverrideListActionHandler, menuStateHandler_GameModeOverride, &dataCustomModes, "Change to one of the Horizon Custom Gamemodes." },
  { "Preset", listActionHandler, menuStateAlwaysEnabledHandler, &dataGameConfigPreset, "Select one of the preconfigured game rule presets or manually set the custom game rules below." },

  { "Game Rules", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_HEADER },
  { "Radar Blips", listActionHandler, menuStateHandler_Default, &dataRadarBlipsDistance, "Toggle the radar blips on the map short (default), Always, or Off (won't show any players on map)" },
  { "Respawn Timer", listActionHandler, menuStateHandler_Default, &dataRespawnTimer_Player, "Time in seconds for the player to respawn." },
  { "Respawn Invincibility", toggleActionHandler, menuStateHandler_Default, &gameConfig.grRespawnInvincibility, "Adds a small invincibility timer to players whom have just respawned." },
  { "Penalty Timers", toggleInvertedActionHandler, menuStateHandler_CTFandSiege, &gameConfig.grDisablePenaltyTimers, "Disables longer respawn timers for your team if your teams base turrets have been destroyed." },
  { "Weapon Pack Spawning", listActionHandler, menuStateHandler_Default, &dataWeaponPacks, "Toggle if weapon packs should spawn when a player dies." },
  { "V2s", listActionHandler, menuStateHandler_Default, &dataV2_Setting, "Configures V2 weapon upgrades to be disabled, on (default), or always on (spawn with v2 weapons)." },
  { "Damage Cooldown", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grNoCooldown, "Disables the brief hit invincibility after taking damage." },
  { "Fix Wallsniping", toggleActionHandler, menuStateHandler_Default, &gameConfig.grFluxShotsAlwaysHit, "Forces sniper shots that hit to register on every client. Can result in shots that appear to phase through walls." },
  { "Fix Flux Niking", toggleActionHandler, menuStateHandler_Default, &gameConfig.grFluxNikingDisabled, "Forces sniper shots to always deal the correct amount of damage." },
  { "Auto Respawn", toggleActionHandler, menuStateHandler_DM, &gameConfig.grAutoRespawn, "Automatically respawn players once the in game respawn timer hits zero." },
  { "Vampire Healing", listActionHandler, menuStateHandler_Default, &dataVampire, "Earn a percentage of health for each kill."},
  // { "Health Bars", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grHealthBars, "Draws a healthbar above each player's nametag." },
  { "Health Boxes", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisableHealthBoxes, "Toggle Health Boxes on or off." },
  { "Weapon Crates", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisableWeaponCrates, "Toggle Weapon Crates on or off." },
  { "Ammo Pickups", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisableAmmoPickups, "Toggle Ammo Pickups on or off." },
  { "Health Boxes Respawn Timer", listActionHandler, menuStateHandler_Default, &dataRespawnTimer_HealthBoxes, "Time in seconds for the Health Boxes to respawn." },
  { "Weapon Crates Respawn Timer", listActionHandler, menuStateHandler_Default, &dataRespawnTimer_WeaponCrates, "Time in seconds for the Weapon Crates to respawn." },
  { "Ammo Pickups Respawn Timer", listActionHandler, menuStateHandler_Default, &dataRespawnTimer_AmmoPickups, "Time in seconds for the Ammo Pickups to respawn." },

  { "Drones", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisableDrones, "Toggle Drones on or off." },
  { "Player Turrets", toggleInvertedActionHandler, menuStateHandler_Default, &gameConfig.grDisablePlayerTurrets, "Toggle Player Turrets on or off." },
#if DEBUG
  { "CTF Flag Returns on Bad Ground", toggleActionHandler, menuStateHandler_CTF, &gameConfig.grFlagHotspots, "Returns a dropped flag if it lands on water, lava, or other non-walkable areas." },
#endif

  { "Base/Node Modifications", labelActionHandler, menuLabelStateHandler_CTFandSiege, (void*)LABELTYPE_HEADER },
  { "Gattling Turret Health", listActionHandler, menuStateHandler_BaseDefenses, &dataSetGattlingTurretHealth, "Increase or decrease the amount of health each teams base turrets have." },
  { "Health/Ammo Pads Always Active", toggleActionHandler, menuStateHandler_BaseDefenses, &gameConfig.grBaseHealthPadActive, "Let the Health and Ammo pads in each base always stay active, even if parts of the base are destroyed." },
  { "Bots (Troopers, Ball Bots, ect.)", toggleInvertedActionHandler, menuStateHandler_CTFandSiege, &gameConfig.grNoBaseDefense_Bots, "toggle Troopers (and other bots) on or off." },
  // { "Small Turrets", toggleInvertedActionHandler, menuStateHandler_CTFandSiege, &gameConfig.grNoBaseDefense_SmallTurrets },

  { "Party Rules", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_HEADER },
  { "Chargeboot Forever", toggleActionHandler, menuStateHandler_Default, &gameConfig.prChargebootForever, "Double tap and hold R2 to chargeboot forever." },
  { "Loadout Weapons Only", toggleActionHandler, menuStateHandler_Default, &gameConfig.prLoadoutWeaponsOnly, "Only allow the use of each players Loadout weapons, regardless of what weapons are enabled." },
  // { "Player Size", listActionHandler, menuStateHandler_Default, &dataPlayerSize, "Changes the size of the player model." },
  { "Survivor", toggleActionHandler, menuStateHandler_Survivor, &gameConfig.prSurvivor, "You only have one life!  Once you die, you can't respawn!" },
};

// Game Settings (not in staging)
MenuElem_t menuElementsGameSettingsHelp[] = {
  { "", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_LABEL },
  { "Please create a game to configure", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_LABEL },
  { "the custom game settings.", labelActionHandler, menuLabelStateHandler, (void*)LABELTYPE_LABEL },
};

// tab items
TabElem_t tabElements[] = {
  { "General", tabDefaultStateHandler, menuElementsGeneral, sizeof(menuElementsGeneral)/sizeof(MenuElem_t) },
  { "Game Settings", tabGameSettingsStateHandler, menuElementsGameSettings, sizeof(menuElementsGameSettings)/sizeof(MenuElem_t) },
  { "Game Settings", tabGameSettingsHelpStateHandler, menuElementsGameSettingsHelp, sizeof(menuElementsGameSettingsHelp)/sizeof(MenuElem_t) },
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
int menuStateHandler_SelectedMapOverride(MenuElem_ListData_t* listData, char* value)
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
  if (gm > CUSTOM_MODE_NONE)
  {
    for (i = 0; i < dataCustomMapsWithExclusiveGameModeCount; ++i)
    {
      if (v == dataCustomMapsWithExclusiveGameMode[i])
      {
        *value = CUSTOM_MAP_NONE;
        return 0;
      }
    }
  }

  // success
  return 1;
}

// 
int menuStateHandler_SelectedGameModeOverride(MenuElem_ListData_t* listData, char* value)
{
  if (!value)
    return 0;

  GameSettings* gs = gameGetSettings();
  char v = *value;

  if (gs)
  {
    #if DEBUG
    switch (v)
    {
      case CUSTOM_MODE_INFECTED:
      case CUSTOM_MODE_JUGGERNAUGHT:
      {
        // only allow deathmatch
        if (gs->GameType == GAMERULE_DM)
          return 1;
        
        // otherwise reject custom mode
        *value = CUSTOM_MODE_NONE;
        return 0;
      }
    }
    #endif
  }

  return 1;
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

void menuLabelStateHandler_BaseDefenses(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();
  GameOptions * go = gameGetOptions();
  if (!gs || (!go->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets))
    *state = ELEMENT_HIDDEN;
  else
    *state = ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_Siege(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType != GAMERULE_SIEGE)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_CTF(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType != GAMERULE_CTF)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_DM(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType != GAMERULE_DM)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_CTFandSiege(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType == GAMERULE_DM)
    *state = ELEMENT_HIDDEN;
  else if (preset)
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuLabelStateHandler_CTFandSiege(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();

  if (!gs || gs->GameType == GAMERULE_DM)
    *state = ELEMENT_HIDDEN;
  else
    *state = ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}

void menuStateHandler_Survivor(TabElem_t* tab, MenuElem_t* element, int* state)
{
  GameSettings * gs = gameGetSettings();
  GameOptions * go = gameGetOptions();

  if (!gs || (gs->GameType != GAMERULE_DM) || go->GameFlags.MultiplayerGameFlags.Teams)
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

// 
void gmResetSelectHandler(TabElem_t* tab, MenuElem_t* element)
{
  preset = 0;
  memset(&gameConfig, 0, sizeof(gameConfig));
  SelectedCustomMapId = 0;
}

// 
void gmRefreshMapsSelectHandler(TabElem_t* tab, MenuElem_t* element)
{
  refreshCustomMapList();
  
  // popup
  if (isInMenus())
  {
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
    if (SelectedCustomMapId == dataCustomMapsWithExclusiveGameMode[i])
    {
      *state = ELEMENT_HIDDEN;
      return;
    }
  }

  *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}
void menuStateScavengerHuntEnabledHandler(TabElem_t* tab, MenuElem_t* element, int* state)
{
  if (!scavHuntEnabled)
    *state = ELEMENT_HIDDEN;
  else
    *state = ELEMENT_SELECTABLE | ELEMENT_VISIBLE | ELEMENT_EDITABLE;
}
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
  gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0);

  // draw value
  x = (rect->TopRight[0] * SCREEN_WIDTH) - 5;
  gfxScreenSpaceText(x, y, 1, 1, color, *(char*)element->userdata ? "On" : "Off", -1, 2);
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
  gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0);

  // draw value
  x = (rect->TopRight[0] * SCREEN_WIDTH) - 5;
  gfxScreenSpaceText(x, y, 1, 1, color, *(char*)element->userdata ? "Off" : "On", -1, 2);
}

//------------------------------------------------------------------------------
void drawRangeMenuElement(TabElem_t* tab, MenuElem_t* element, MenuElem_RangeData_t * rangeData, RECT* rect)
{
  char buf[32];

  // get element state
  int state = getMenuElementState(tab, element);

  float x,y,w,v,h;
  float lerp = (state & ELEMENT_EDITABLE) ? 0.0 : 0.5;
  u32 color = colorLerp(colorText, 0, lerp);

  // draw name
  x = (rect->TopLeft[0] * SCREEN_WIDTH) + 5;
  y = (rect->TopLeft[1] * SCREEN_HEIGHT) + 5;
  gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0);

  // draw box
  u32 barColor = colorLerp(RangeBar_IsSelected, 0, lerp);
  v = (float)(*rangeData->value - rangeData->minValue) / (float)(rangeData->maxValue - rangeData->minValue);
  w = (rect->TopRight[0] - rect->TopLeft[0]) * 0.5 * SCREEN_WIDTH;
  x = (rect->TopRight[0] * SCREEN_WIDTH) - w - 5;
  h = (rect->BottomRight[1] - rect->TopRight[1]) * SCREEN_HEIGHT;
  gfxPixelSpaceBox(x, y - 4, w * v, h - 2, barColor);

  // draw name
  sprintf(buf, "%d", *rangeData->value);
  x = (rect->TopRight[0] * SCREEN_WIDTH) - 5;
  gfxScreenSpaceText(x, y, 1, 1, color, buf, -1, 2);
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
  gfxScreenSpaceText(x, y, 1, 1, color, element->name, -1, 0);

  // draw value
  x = (rect->TopRight[0] * SCREEN_WIDTH) - 5;
  gfxScreenSpaceText(x, y, 1, 1, color, listData->items[(int)*listData->value], -1, 2);
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
  gfxScreenSpaceText(x, y, 1, 1, colorLerp(colorText, 0, lerp), element->name, -1, 4);

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
  gfxScreenSpaceText(x, y, 1, 1, colorLerp(colorText, 0, lerp), element->name, -1, 4);
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
    case ACTIONTYPE_VALIDATE:
    {
      if (rangeData->stateHandler != NULL)
        rangeData->stateHandler(rangeData, rangeData->value);
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
    case ACTIONTYPE_VALIDATE:
    {
      if (listData->stateHandler != NULL)
        listData->stateHandler(listData, listData->value);
      break;
    }
  }
}

//------------------------------------------------------------------------------
void gmOverrideListActionHandler(TabElem_t* tab, MenuElem_t* element, int actionType, void * actionArg)
{
  // update name to be based on current gamemode
  GameSettings* gs = gameGetSettings();
  if (gs && actionType == ACTIONTYPE_DRAW) {
    sprintf(element->name, "%s override", gameGetGameModeName(gs->GameType));
  }

  // pass to default list action handler
  listActionHandler(tab, element, actionType, actionArg);
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
      *(char*)element->userdata = !(*(char*)element->userdata);;
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
  gfxScreenSpaceText(0.5 * SCREEN_WIDTH, (frameY + frameTitleH * 0.5) * SCREEN_HEIGHT, 1, 1, colorText, "Patch Config Menu", -1, 4);

  // footer bg
  gfxScreenSpaceBox(frameX, frameY + frameH - frameFooterH, frameW, frameFooterH, colorRed);

  // footer
  gfxScreenSpaceText(((frameX + frameW) * SCREEN_WIDTH) - 5, (frameY + frameH) * SCREEN_HEIGHT - 5, 1, 1, colorText, footerText, -1, 8);

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
      gfxScreenSpaceText((tabX + 2*tabBarPaddingX) * SCREEN_WIDTH, (tabY + (0.5 * tabBarH)) * SCREEN_HEIGHT, 1, 1, color, tab->name, -1, 3);

      // increment X
      tabX += pWidth - tabBarPaddingX;
    }
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
    if (i == tab->selectedMenuItem) {
      state = getMenuElementState(tab, currentElement);
      if (state & ELEMENT_SELECTABLE) {
        RangeBar_IsSelected = colorRangeBarSelected;
        gfxScreenSpaceQuad(&drawRect, colorSelected, colorSelected, colorSelected, colorSelected);

        // draw help
        if (currentElement->help && strlen(currentElement->help) > 0) {

          if (i != helpLastItemIdx) {
            helpLastItemIdx = i;
            helpLastXOffset = 0;
            helpItemCooldown1 = GAME_FPS * 3;
            helpItemCooldown2 = GAME_FPS * 6;
          }

          // draw background
          gfxScreenSpaceBox(frameX, frameY + frameH - 1.0/SCREEN_HEIGHT, frameW, LINE_HEIGHT, colorContentBg);

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
          else { helpItemCooldown1 = GAME_FPS * 3; helpItemCooldown2 = GAME_FPS * 6; helpLastXOffset = 0; }
          gfxScreenSpaceText((frameX + contentPaddingX + helpLastXOffset) * SCREEN_WIDTH, (frameY + frameH) * SCREEN_HEIGHT, 1, 1, colorText, currentElement->help, -1, 0);

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
  if (tab->selectedMenuItem >= menuElementRenderEnd)
    ++tab->menuOffset;
  if (tab->selectedMenuItem < tab->menuOffset)
    tab->menuOffset = tab->selectedMenuItem;

  // get selected element
  if (tab->selectedMenuItem >= menuElementsCount)
    return;

  currentElement = &menuElements[tab->selectedMenuItem];
  state = getMenuElementState(tab, currentElement);

  // find next selectable item if hidden or not selectable
  if ((state & ELEMENT_VISIBLE) == 0 || (state & ELEMENT_SELECTABLE) == 0)
    navMenu(tab, 1, 1);

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
  // nav page up
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
		if (uiGetActivePointer(UIP_ONLINE_LOBBY) != 0 && *(u32*)0x01C5C114 == 0)
		{
			// render message
			// gfxScreenSpaceBox(SCREEN_WIDTH * 0.2, SCREEN_HEIGHT * 0.81, 0.4, 0.3, colorOpenBg);
      float scale = .85;
	  	gfxScreenSpaceText(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.77, scale, scale, 0x80FFFFFF, "Press START to", -1, 4);
			gfxScreenSpaceText(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.80, scale, scale, 0x80FFFFFF, "Open Config Menu", -1, 4);

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
  int newElement = tab->selectedMenuItem + direction;
  MenuElem_t *elem = NULL;
  int state = 0;

  // handle case where tab has no items
  if (tab->elementsCount == 0)
  {
    tab->selectedMenuItem = 0;
    tab->menuOffset = 0;
    return;
  }

  while (newElement != tab->selectedMenuItem)
  {
    if (newElement >= tab->elementsCount)
    {
      if (loop && tab->selectedMenuItem != 0)
        newElement = 0;
      else
        break;
    }
    else if (newElement < 0)
    {
      if (loop && tab->selectedMenuItem != (tab->elementsCount - 1))
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
    tab->selectedMenuItem = newElement;
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
int onSetGameConfig(void * connection, void * data)
{
  // copy it over
  memcpy(&gameConfig, data, sizeof(PatchGameConfig_t));
  DPRINTF("set gameconfig map:%d\n", SelectedCustomMapId);
  return sizeof(PatchGameConfig_t);
}

//------------------------------------------------------------------------------
void onConfigUpdate(void)
{
  int i;

  // in staging, update game info
  GameSettings * gameSettings = gameGetSettings();
  if (gameSettings && gameSettings->GameLoadStartTime < 0 && netGetLobbyServerConnection())
  {
    // 
    char * mapName = mapGetName(gameSettings->GameLevel);
    char * modeName = gameGetGameModeName(gameSettings->GameType);

    // get map override name
    if (SelectedCustomMapId > 0)
      mapName = dataCustomMaps.items[SelectedCustomMapId];
    else if (MapLoaderState.MapName[0])
      mapName = MapLoaderState.MapName;

    // get mode override name
    if (gameConfig.customModeId > 0)
    {
      modeName = (char*)CustomModeShortNames[(int)gameConfig.customModeId];
      if (!modeName)
        modeName = dataCustomModes.items[(int)gameConfig.customModeId];
    }

    // override gamemode name with map if map has exclusive gamemode
    if (SelectedCustomMapId)
    {
      for (i = 0; i < dataCustomMapsWithExclusiveGameModeCount; ++i)
      {
        if (CustomMapDefs[SelectedCustomMapId-1].ForcedCustomModeId != 0)
        {
          modeName = mapName;
          break;
        }
      }
    }

    u32 * stagingUiElements = (u32*)(uiGetActivePointer(UIP_STAGING) + 0x110);
    u32 * stagingDetailsUiElements = (u32*)(uiGetActivePointer(UIP_STAGING_SECONDARY_PLAYER_OPTIONS) + 0x110);

    // update ui strings
    if ((u32)stagingUiElements > 0x100000)
    {
      if (mapName)
        strncpy((char*)(stagingUiElements[1] + 0x14), mapName, 32);
      if (modeName)
        strncpy((char*)(stagingUiElements[2] + 0x14), modeName, 32);
    }
    if ((u32)stagingDetailsUiElements > 0x100000)
    {
      // strncpy((char*)(stagingDetailsUiElements[2] + 0x14), mapName, 32);
      // strncpy((char*)(stagingDetailsUiElements[3] + 0x14), modeName, 32);
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
  //printf("config.c: onConfigOnlineMenu\n");
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
      if (SelectedCustomMapId > 0)
        memcpy(&msg.CustomMap, &CustomMapDefs[SelectedCustomMapId-1], sizeof(msg.CustomMap));
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
      case 1: // Competitive
      {
        gameConfig.grV2s = 0;
        gameConfig.grVampire = 0;
        gameConfig.grNoCooldown = 1;
        gameConfig.grDisableWeaponPacks = 1;
        gameConfig.grDisableHealthBoxes = 0;
        gameConfig.grDisableWeaponCrates = 1;
        gameConfig.grDisableAmmoPickups = 1;
        gameConfig.grDisableDrones = 1;
        gameConfig.grDisablePlayerTurrets = 1;
        // gameConfig.grFluxShotsAlwaysHit = 1;
        // gameConfig.grFluxNikingDisabled = 1;
        break;
      }
      case 2: // 1v1
      {
        gameConfig.grRadarBlipsDistance = 1; // Always
        gameConfig.grRespawnTimer_Player = 10; // 0 Seconds
        gameConfig.grDisableHealthBoxes = 1;
        gameConfig.grDisableWeaponCrates = 1;
        gameConfig.grDisableAmmoPickups = 1;
        gameConfig.grDisableDrones = 1;
        gameConfig.grDisablePlayerTurrets = 1;
        gameConfig.grV2s = 1; // Off
        gameConfig.grNoCooldown = 1;
        gameConfig.grVampire = 4; // 100%
        gameConfig.grDisableWeaponPacks = 1;
        // gameConfig.grFluxShotsAlwaysHit = 1;
        // gameConfig.grFluxNikingDisabled = 1;
        break;
      }
    }

    if (gameConfig.prLoadoutWeaponsOnly) {
      gameConfig.grDisableWeaponPacks = 1;
      gameConfig.grDisableWeaponCrates = 1;
      gameConfig.grDisableAmmoPickups = 1;
    }

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
