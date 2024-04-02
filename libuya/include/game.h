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

//--------------------------------------------------------
typedef struct DeathMatchKD {
    short Kills;
    short Deaths;
} DeathMatchKD;

typedef struct PlayerGameStats { // 0x40
/*0x00*/ DeathMatchKD DeathMatch[GAME_MAX_PLAYERS];
/*0x20*/ char NodesCaptured[GAME_MAX_PLAYERS];
/*0x28*/ char NodesSaved[GAME_MAX_PLAYERS];
/*0x30*/ char FlagsCaptured[GAME_MAX_PLAYERS];
/*0x38*/ char FlagsSaved[GAME_MAX_PLAYERS];
} PlayerGameStats;

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
typedef struct TeamGameStats
{
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
} TeamGameStats;

//--------------------------------------------------------
typedef struct DeathMatchGameData
{
    int ResurrectionPts[64];
    int Pad1;
    int RandomSpawn;
    int SmartSpawnPts;
    int pad[13];
} DeathMatchGameData;

//--------------------------------------------------------
typedef struct CTFGameData
{
    int Unk_00[2];
    short TeamCaptures;
    short TeamSaves;
} CTFGameData;

//--------------------------------------------------------
typedef struct LocalPlayerYourBaseGameData
{
    int Team1_SpawnPts[3];
    int Team2_SpawnPts[3];
    int NodeResurrectionPts[8];
    Moby* BaseMobys[8];
    Moby* NodeMobys[8];
    int Team1_StartBase;
    int Team2_StartBase;
    int BaseTeam[8];
    int NodeTeam[8];
    int OrigBaseTeam[8];
    int BaseHealth[8];
    float TotalHudHealth[8];
    float PrevHudHealth[8];
    float HudHealth[8];
    int NumBases;
    int WinScore;
    int Team3_SpawnPts[3];
    int Team4_SpawnPts[3];
    Moby* HomeNodeMobys[2];
    int pad[8];
} LocalPlayerYourBaseGameData;

//--------------------------------------------------------
typedef struct ScoreboardItem
{
    int TeamId;
    int UNK;
    int Value;
} ScoreboardItem;

//--------------------------------------------------------
typedef struct GameData {
/* 0x000 */ int TimeEnd;
/* 0x004 */ int TimeStart;
/* 0x008 */ int GameState;
/* 0x00c */ int NumTeams;
/* 0x010 */ int WinningTeam;
/* 0x014 */ int WinningPlayer;
/* 0x018 */ PlayerGameStats PlayerStats;
/* 0x058 */ char unk_058[0x1b4];
/* 0x20c */ void* unk_ptr_20c;
/* 0x210 */ CTFGameData* CTFGameData;
/* 0x214 */ DeathMatchGameData* DeathMatchGameData;
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
//--------------------------------------------------------

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

#endif // _LIBUYA_GAME_H_
