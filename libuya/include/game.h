/***************************************************
 * FILENAME :		game.h
 * DESCRIPTION :
 * 		Contains game specific offsets and structures for UYA.
 * NOTES :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GAME_H_
#define _LIBUYA_GAME_H_

#include <tamtypes.h>
#include "moby.h"
#include "common.h"

/*
 * Maximum number of players in a game.
 */
#define GAME_MAX_PLAYERS                    (8)
#define GAME_MAX_LOCALS                     (2)
/*
 * Games FPS
*/
#if UYA_PAL
#define GAME_FPS                            (50.0)
#else
#define GAME_FPS                            (60.0)
#endif

typedef struct FragCount {
/* 0x0 */ short kills;
/* 0x2 */ short deaths;
} FragCount;

typedef struct PlayerStats { // 0x60
/* 0x00 */ FragCount frag[GAME_MAX_PLAYERS];
/* 0x20 */ char nodesCaptured[GAME_MAX_PLAYERS];
/* 0x28 */ char nodesSaved[GAME_MAX_PLAYERS];
/* 0x30 */ char flagsCaptured[GAME_MAX_PLAYERS];
/* 0x38 */ char flagsSaved[GAME_MAX_PLAYERS];
/* 0x40 */ float baseDamage[GAME_MAX_PLAYERS];
} PlayerStats;

//=================   =For DL:
// typedef struct PlayerGameStats
// {
//     short WeaponKills[GAME_MAX_PLAYERS][7];
//     short WeaponDeaths[GAME_MAX_PLAYERS][7];
//     short WeaponShots[GAME_MAX_PLAYERS][7];
//     short WeaponShotsHitBy[GAME_MAX_PLAYERS][7];
//     float VehicleTime[GAME_MAX_PLAYERS];
//     short VehicleWeaponKills[GAME_MAX_PLAYERS];
//     short VehicleWeaponDeaths[GAME_MAX_PLAYERS];
//     short VehicleRoadKills[GAME_MAX_PLAYERS];
//     short VehicleRoadDeaths[GAME_MAX_PLAYERS];
//     short VehicleShotsFired[GAME_MAX_PLAYERS];
//     short VehicleShotsHit[GAME_MAX_PLAYERS];
//     short Kills[GAME_MAX_PLAYERS];
//     short Deaths[GAME_MAX_PLAYERS];
//     short Suicides[GAME_MAX_PLAYERS];
//     short MultiKills[GAME_MAX_PLAYERS];
//     short SniperKills[GAME_MAX_PLAYERS];
//     short WrenchKills[GAME_MAX_PLAYERS];
//     char ConquestNodesCaptured[GAME_MAX_PLAYERS];
//     char ConquestNodeSaves[GAME_MAX_PLAYERS];
//     char ConquestDefensiveKills[GAME_MAX_PLAYERS];
//     char ConquestPoints[GAME_MAX_PLAYERS];
//     char CtfFlagsCaptures[GAME_MAX_PLAYERS];
//     char CtfFlagsSaved[GAME_MAX_PLAYERS];
//     float KingHillHoldTime[GAME_MAX_PLAYERS];
//     float InternalKingHillHoldTime[GAME_MAX_PLAYERS];
//     float JuggernautTime[GAME_MAX_PLAYERS];
//     short Squats[GAME_MAX_PLAYERS];
//     short VehicleSquats[GAME_MAX_PLAYERS];
//     short TicketScore[GAME_MAX_PLAYERS];
// } PlayerGameStats;

//--------------------------------------------------------
typedef struct TeamStats {
    short TeamTicketScore[GAME_MAX_PLAYERS];
    char TeamUpgradesLevel1[GAME_MAX_PLAYERS];
    char TeamUpgradesLevel2[GAME_MAX_PLAYERS];
    char TeamUpgradesLevel3[GAME_MAX_PLAYERS];
    float TeamCaptureTimer[GAME_MAX_PLAYERS];
    short TeamCaptureTimerSettings[GAME_MAX_PLAYERS];
    char NumNodesOwned[GAME_MAX_PLAYERS];
    float PercentNodesCaptured[GAME_MAX_PLAYERS];
    float NodeHoldTime[GAME_MAX_PLAYERS];
    char FlagCaptureCounts[GAME_MAX_PLAYERS];
} TeamStats;

typedef struct LocalPlayerYourBaseGameData { // 0x1b0
/* 0x000 */ int team1_StartBase;
/* 0x004 */ int team2_StartBase;
/* 0x008 */ int unk_08[6];
/* 0x020 */ int nodeResurrectionPts[8];
/* 0x040 */ int unk_SpawnPts[8];
/* 0x060 */ int unk_BaseMobySpawnPts[8];
/* 0x080 */ int unk_team1;
/* 0x084 */ int unk_team2;
/* 0x088 */ int team1_BaseComputerTeam;
/* 0x08c */ int team2_BaseComputerTeam;
/* 0x090 */ int unk_090[6];
/* 0x0a8 */ int nodeTeam[8];
/* 0x0c8 */ int unk_0c8[8];
/* 0x0e8 */ int baseHealth[8];
/* 0x108 */ float totalHudHealth[8];
/* 0x128 */ float prevHudHealth[8];
/* 0x148 */ float hudHealth[8];
/* 0x168 */ int unk_168[18];
} LocalPlayerYourBaseGameData;

typedef struct CTFGameData {
/* 0x00 */ int blueFlagCuboid;
/* 0x04 */ int redflagCuboid;
/* 0x08 */ int blueTeamCaptures;
/* 0x0c */ int redTeamCaptures;
/* 0x10 */ int blueTeamSaves;
/* 0x14 */ int redTeamSaves;
} CTFGameData;

typedef struct DeathMatchGameData {
    int resurrectionPts[64];
    int pad1;
    int randomSpawn;
    int smartSpawnPts;
    int pad[13];
} DeathMatchGameData;

typedef struct GameData {
/* 0x000 */ int timeEnd;
/* 0x004 */ int timeStart;
/* 0x008 */ int gameState;
/* 0x00c */ int numTeams;
/* 0x010 */ int winningTeam;
/* 0x014 */ int winningPlayer;
/* 0x018 */ PlayerStats playerStats;
/* 0x078 */ char unk_078[0x194];
/* 0x20c */ LocalPlayerYourBaseGameData *allYourBaseGameData;
/* 0x210 */ CTFGameData *CTFGameData;
/* 0x214 */ DeathMatchGameData *DeathMatchGameData;
} GameData;

//=================   =For DL:
// typedef struct GameData
// {
//     int TimeEnd;
//     int TimeStart;
//     int GameState;
//     int NumTeams;
//     int WinningTeam;
//     int WinningPlayer;
//     int BaseHoldTime;
//     int FragDisplayCount;
//     char FragMsg[0x3C];
//     int GameEndReceived;
//     int GameEndReason;
//     int GameIsOver;
//     int NumNodes;
//     int NumStartPlayers;
//     int NumStartTeams;
//     int MyTotalSquats;
//     int MyTotalTimeSquatted;
//     int MyTotalGangSquats;
//     int TeamCaptain[GAME_MAX_PLAYERS];
//     PlayerGameStats PlayerStats;
//     TeamGameStats TeamStats;
//     LocalPlayerYourBaseGameData * AllYourBaseGameData;
//     CTFGameData * CtfGameData;
//     DeathMatchGameData * DeathmatchGameData;
// } GameData;

typedef struct GameLocalSettings { // 0xc4
	/* 0x00 */ int PalMode;
	/* 0x04 */ char HelpVoiceOn;
	/* 0x05 */ char HelpTextOn;
	/* 0x06 */ char SubtitlesActive;
	/* 0x08 */ int Stereo;
	/* 0x0c */ int MusicVolume;
	/* 0x10 */ int EffectsVolume;
    /* 0x14 */ char unk_14[0x90];
	/* 0xa4 */ unsigned char FirstPersonModeOn[8];
    /* 0xac */ char _was_NTSCProgessive;
    /* 0xad */ char Wide;
	/* 0xae */ char ControllerVibrationOn[8];
	/* 0xb6 */ char QuickSelectPauseOn;
	/* 0xb7 */ char Language;
	/* 0xb8 */ char AuxSetting2;
	/* 0xb9 */ char AuxSetting3;
	/* 0xba */ char AuxSetting4;
	/* 0xbb */ char AutoSaveOn;
} GameLocalSettings;

struct tNW_Info {
	/* 0x0000 */ char unk_0000[0x18];
	/* 0x0018 */ int myClientIndex;
	/* 0x001c */ int numLocalPlayers;
	/* 0x0020 */ char unk_0020[0x8];
	/* 0x0028 */ int myTeam;
	/* 0x002c */ int mySkin;
	/* 0x0030 */ char unk_0030[0x18];
	/* 0x0044 */ int myAccountId;
	/* 0x0048 */ int unk_00048;
	/* 0x004c */ char myClanName[0x21];
	/* 0x006d */ char isClanLeader;
	/* 0x006e */ char myUserName[0xf];
	/* 0x007d */ char pad[0x3];
	/* 0x0080 */ int myAccountCityId;
	/* 0x0084 */ char myLicenseAccepted;
	/* 0x0085 */ char unk_0085[0x49];
	/* 0x00ce */ char gameName[64];
	/* 0x0108 */ char gamePassword[32];
	/* 0x0138 */ int gameTime;
	/* 0x0178 */ char isSessionMaster;
	/* 0x0179 */ char isNewSessionMaster;
	/* 0x017a */ char pad_017a[2];
	/* 0x017c */ int sessionMasterClientIndex;
	/* 0x0180 */ char unk_0180[0x11f4];
	/* 0x1374 */ int gameWorldId;
} tNW_Info_t;

typedef struct GameLocalSettings { // 0xc4
	/* 0x00 */ int PalMode;
	/* 0x04 */ char HelpVoiceOn;
	/* 0x05 */ char HelpTextOn;
	/* 0x06 */ char SubtitlesActive;
	/* 0x08 */ int Stereo;
	/* 0x0c */ int MusicVolume;
	/* 0x10 */ int EffectsVolume;
    /* 0x14 */ char unk_14[0x90];
	/* 0xa4 */ unsigned char FirstPersonModeOn[8];
    /* 0xac */ char _was_NTSCProgessive;
    /* 0xad */ char Wide;
	/* 0xae */ char ControllerVibrationOn[8];
	/* 0xb6 */ char QuickSelectPauseOn;
	/* 0xb7 */ char Language;
	/* 0xb8 */ char AuxSetting2;
	/* 0xb9 */ char AuxSetting3;
	/* 0xba */ char AuxSetting4;
	/* 0xbb */ char AutoSaveOn;
} GameLocalSettings;

struct tNW_Info {
	/* 0x0000 */ char unk_0000[0x18];
	/* 0x0018 */ int myClientIndex;
	/* 0x001c */ int numLocalPlayers;
	/* 0x0020 */ char unk_0020[0x8];
	/* 0x0028 */ int myTeam;
	/* 0x002c */ int mySkin;
	/* 0x0030 */ char unk_0030[0x18];
	/* 0x0044 */ int myAccountId;
	/* 0x0048 */ int unk_00048;
	/* 0x004c */ char myClanName[0x21];
	/* 0x006d */ char isClanLeader;
	/* 0x006e */ char myUserName[0xf];
	/* 0x007d */ char pad[0x3];
	/* 0x0080 */ int myAccountCityId;
	/* 0x0084 */ char myLicenseAccepted;
	/* 0x0085 */ char unk_0085[0x49];
	/* 0x00ce */ char gameName[64];
	/* 0x0108 */ char gamePassword[32];
	/* 0x0138 */ int gameTime;
	/* 0x0178 */ char isSessionMaster;
	/* 0x0179 */ char isNewSessionMaster;
	/* 0x017a */ char pad_017a[2];
	/* 0x017c */ int sessionMasterClientIndex;
	/* 0x0180 */ char unk_0180[0x11f4];
	/* 0x1374 */ int gameWorldId;
} tNW_Info_t;

/*
 * NAME :		isInGame
 * DESCRIPTION :
 * 			Whether the client is currently in game world.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int isInGame(void);

/*
 * NAME :		isInMenus
 * DESCRIPTION :
 * 			Whether the client is currently in the menus.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int isInMenus(void);

/*
 * NAME :		isSceneLoading
 * DESCRIPTION :
 * 			Whether the client is currently in a scene transition.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int isSceneLoading(void);

/*
 * NAME :		gameHasEnded
 * DESCRIPTION :
 * 			Whether the game has ended and/or is ending.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int gameHasEnded(void);

/*
 * NAME :		gameGetFinishedExitTime
 * DESCRIPTION :
 * 			Gets the time when to leave after the game has ended.
 * NOTES :
 * ARGS : 
 * RETURN :
 *          Returns 0 if the game has not ended.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int gameGetFinishedExitTime(void);

/*
 * NAME :		gameGetTime
 * DESCRIPTION :
 * 			Gets the current game time in milliseconds.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int gameGetTime(void);

/*
 * NAME :		gameIsHost
 * DESCRIPTION :
 * 			Whether the given host id is the host of the game.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gameIsHost(int hostId);

/*
 * NAME :		gameAmIHost
 * DESCRIPTION :
 * 			Whether the current client is also the game host.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gameAmIHost(void);

/*
 * NAME :		gameGetHostId
 * DESCRIPTION :
 * 			Returns the current game's host's client/dme id.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int gameGetHostId(void);

/*
 * NAME :		gameGetMyClientId
 * DESCRIPTION :
 * 			Returns the current client's game/dme id.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int gameGetMyClientId(void);

/*
 * NAME :		gameGetMyClientId
 * DESCRIPTION :
 * 			Returns the currently loaded (or loading) map id.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gameGetCurrentMapId(void);

/*
 * NAME :		gameGetGameModeName
 * DESCRIPTION :
 * 			Returns a pointer to the given gamemode's name.
 *          Returns NULL if not a valid gamemode.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
char* gameGetGameModeName(int gameModeId);

/*
 * NAME :		gameEnd
 * DESCRIPTION :
 * 			End game.
 * NOTES :
 * ARGS : 
 *      reason :      Why the game ended.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void gameEnd(int reason);

/*
 * NAME :		gameGetDeathHeight
 * DESCRIPTION :
 * 			Gets the level's death height.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
__LIBUYA_GETTER__ float gameGetDeathHeight(void);

/*
 * NAME :		gameSetDeathHeight
 * DESCRIPTION :
 * 			Sets the level's death height.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
__LIBUYA_SETTER__ void gameSetDeathHeight(float height);

__LIBUYA_SETTER__ void gameGetQuickSandhHeight(void);
__LIBUYA_SETTER__ void gameSetQuickSandHeight(float height);

/*
 * NAME :		gameGetData
 * DESCRIPTION :
 * 			Gets the game data.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ GameData* gameGetData(void);

/*
 * NAME :		gameGetGSFrame
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int gameGetGSFrame(void);

/*
 * NAME :		gameGetWorldId
 * DESCRIPTION :
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
int gameGetWorldId(void);

// Gets local (PS2) settings
GameLocalSettings *gameGetLocalSettings(void);


#endif // _LIBUYA_GAME_H_
