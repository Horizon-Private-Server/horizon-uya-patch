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
#include "net.h"

#if UYA_PAL
#define GAME_FPS                            (50.0)
#define GAME_LOCAL_SETTINGS                 ((GameLocalSettings*)0x001a5810)
#define GAME_NET_INFO                       ((tNW_Info_t*)0x01a5cc0)
#else
#define GAME_FPS                            (60.0)
#define GAME_LOCAL_SETTINGS                 ((GameLocalSettings*)0x001a5990)
#define GAME_NET_INFO                       ((tNW_Info_t*)0x001a5e40)
#endif

#define GAME_MAX_PLAYERS                    (8)
#define GAME_MAX_LOCALS                     (2)

#define GAME_TIME                           (GAME_NET_INFO->netFrameTime)
#define GAME_CLIENT_ID						(GAME_NET_INFO->myClientIndex)
#define GAME_HOST_ID						(GAME_NET_INFO->sessionMasterClientIndex)
#define GAME_WORLD_ID						(GAME_NET_INFO->gameWorldId)

typedef enum GameEndReason {
	GAME_END_TIME_UP = 1,
	GAME_END_TEAM_WIN = 4,
} eGameEndReason;

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
/* 0x040 */ int baseLightIndex[8]; // aka: moby instance index
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
/* 0x078 */ char unk_078[0x174];
/* 0x1ec */ int gameEndReason;
/* 0x1f0 */ int gameIsOver;
/* 0x1f4 */ char unk_1f4[0x18];
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

typedef struct tNW_PlayerInfoStats { // 0x34
	/* 0x00 */ unsigned int crc;
	/* 0x04 */ int rank[6];
	/* 0x1c */ int padding;
	/* 0x20 */ char headsetAttached;
	/* 0x21 */ char isClanLeader;
	/* 0x22 */ char hasLocalPlayer;
	/* 0x23 */ char pad;
	/* 0x24 */ int clanID;
	/* 0x28 */ int totalGamesWithCheaters;
	/* 0x2c */ int consecutiveGamesWithCheaters;
	/* 0x30 */ int lastGameHadCheater;
} tNW_PlayerInfoStats_t;

typedef struct tNW_Info {
/* 0x0000 */ char unk_0000[0x10];
/* 0x0010 */ int numLocalPlayers;
/* 0x0014 */ void* myConnectionIndex; // NetTypeConnectionInfo* myConnectionIndex
/* 0x0018 */ int myClientIndex;
/* 0x001c */ unsigned int uniqueIdCnt;
/* 0x0020 */ int unk_0020;
/* 0x0024 */ int gameLevel;
/* 0x0028 */ int myTeam;
/* 0x002c */ int mySkin;
/* 0x0030 */ char mySessionKey[0x14];
/* 0x0044 */ int myAccountId;
/* 0x0048 */ int unk_00048;
/* 0x004c */ char myClanName[0x21];
/* 0x006d */ char isClanLeader;
/* 0x006e */ char myUserName[0xf];
/* 0x007d */ char pad[0x3];
/* 0x0080 */ int myAccountCityId;
/* 0x0084 */ char myLicenseAccepted;
/* 0x0085 */ char unk_0085[0x13];
/* 0x0098 */ tNW_PlayerInfoStats_t myStats;
/* 0x00cc */ char gameName[64];
/* 0x0108 */ char gamePassword[24];
/* 0x0124 */ enum eNW_STATE state;
/* 0x0125 */ char unk_0125[0xf];
/* 0x0134 */ int unk_0134;
/* 0x0138 */ int netFrameTime; // aka: gameTime
/* 0x013c */ int numClients;
/* 0x0140 */ int numPlayers;
/* 0x0144 */ int maxPlayers;
/* 0x0148 */ int maxBuddySlots;
/* 0x014c */ int maxClanSlots;
/* 0x0150 */ int maxBuddySlots_2;
/* 0x0154 */ int maxClanSlots_2;
/* 0x0158 */ char clientIndex[8];
/* 0x0160 */ char unk_0160[0x18];
/* 0x0178 */ char isSessionMaster;
/* 0x0179 */ char isNewSessionMaster;
/* 0x017a */ char pad_017a[2];
/* 0x017c */ int sessionMasterClientIndex;
/* 0x0180 */ int onlyLocalPlayers;
/* 0x0184 */ int originalyOnlyLocalPlayers;
/* 0x0188 */ char INukedTheMicroCode;
/* 0x0189 */ char unk_0189[0x3];
/* 0x018c */ char m_bPeer2PeerGame;
/* 0x018d */ char unk_018d[0x3];
/* 0x0190 */ char m_bUseEncryption;
/* 0x0191 */ char m_bZeroAggregation;
/* 0x0192 */ char myIP[0x20];
/* 0x01b2 */ char unk_01b2[0x132];
/* 0x02dc */ void* m_GuiSetBusyCallback;
/* 0x02e0 */ void* m_GuiSetRefreshCallback;
/* 0x02e4 */ int m_LastMediusError;
/* 0x02e8 */ int m_LastMGCLError;
/* 0x02ec */ int m_LastNetError;
/* 0x02f0 */ int m_LastMediusNetError;
/* 0x02f4 */ char unk_02f4[0x198];
/* 0x048c */ int m_bForcePlayerReport;
/* 0x0490 */ int m_LastPlayerReportTime;
/* 0x0494 */ int m_bForceWorldReport;
/* 0x0498 */ int m_LastWorldReportTime;
/* 0x049c */ char unk_049c[0x14];
/* 0x04b0 */ int gameSequenceNumber;
/* 0x04b4 */ char unk_04b4[0x148];
/* 0x05fc */ int maxPlayers_2;
/* 0x0600 */ char unk_0600[0x30];
/* 0x0630 */ int m_bUpdateClanStatsPending;
/* 0x0634 */ int m_UpdateClanStatsStatus;
/* 0x0638 */ int m_bAccountUpdateStatsPending;
/* 0x063c */ int m_AccountUpdateStatsStatus;
/* 0x0640 */ int unk_0640;
/* 0x0644 */ int m_JoinResponse;
/* 0x0648 */ int superCheat;
/* 0x064c */ int unk_064c;
/* 0x0650 */ int sendMessageToPlayerClient[8];
/* 0x0670 */ int sendMessageData[8];
/* 0x0690 */ int Unk___ClientSettings_1; // gets modified when redy/unready happens
/* 0x0694 */ int unk___ClientSettings_2;
/* 0x0698 */ int receivedMessagePlayerIdx;
/* 0x069c */ char unk_069c[0x20];
/* 0x06bc */ char m_bSessionMasterLagging;
/* 0x06bd */ char clientIndexRemapper[8];
/* 0x06c5 */ char unk_06c5[0x11b];
/* 0x07c0 */ int m_LastUdpPacketReceived[8];
/* 0x07e0 */ int m_LastUdpPacketReceivedFromAnyone;
/* 0x07e4 */ char m_RussiaHouseTransportFlagHack;
/* 0x07e5 */ char unk_07e5[0x8];
/* 0x07ed */ char bNewSessionMaster;
/* 0x07ee */ char unk_07ee[0x90];
/* 0x0870 */ int unk_0870[2]; // found in FUN_00197688
/* 0x0878 */ int unk_0878[2]; // found in FUN_00197688
/* 0x0880 */ int unk_0880[2]; // found in FUN_00197688
/* 0x0888 */ int unk_0888[2]; // found in FUN_00197688
/* 0x088c */ char unk_088c[0x3ff];
/* 0x0c8b */ char unk_0c8b; // found in FUN_00197688
/* 0x0c8c */ char unk_0c8c;
/* 0x0c8d */ char unk_0c8d; // found in FUN_00197688
/* 0x0c8e */ char unk_0c8e[0x116];
/* 0x0da4 */ float deviation[3];
/* 0x0db0 */ float rank[3];
/* 0x0dbc */ char unk_0dbc[0x51d];
/* 0x12d9 */ char m_bMGCL_Connected;
/* 0x12da */ char unk_12da[0x9b];
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
