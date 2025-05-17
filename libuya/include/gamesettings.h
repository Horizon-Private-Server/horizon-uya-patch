/***************************************************
 * FILENAME :		gamesettings.h
 * DESCRIPTION :
 * 		Contains lobby specific offsets and structures for Deadlocked.
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GAMESETTINGS_H_
#define _LIBUYA_GAMESETTINGS_H_

#include <tamtypes.h>
#include "common.h"

/*
 * Maximum number of players in a game.
 */
#define GAME_MAX_PLAYERS                (8)

enum eGS_GameTypes {
    GAMETYPE_SIEGE,
    GAMETYPE_CTF,
    GAMETYPE_DM
};

enum eGS_PlayerTypes {
    GS_PLAYER_TYPE_NOBDY = 0,
    GS_PLAYER_TYPE_BUDDY = 1,
    GS_PLAYER_TYPE_CLAN_MEMBER = 2,
    GS_PLAYER_TYPE_BOTH = 3,
    GS_PLAYER_TYPE_BUDDY_USING_UNRESERVED = 4,
    GS_PLAYER_TYPE_CLAN_USING_UNRESERVED = 5,
    GS_PLAYER_TYPE_BOTH_USING_UNRESERVED = 6,
    GS_PLAYER_TYPE_BOTH_USING_BUDDY = 7,
    GS_PLAYER_TYPE_BOTH_USING_CLAN = 8,
};

enum eGS_PlayerStates {
    GS_PLAYER_STATE_UNREADY = 0,
    GS_PLAYER_STATE_SETUP = 1,
    GS_PLAYER_STATE_MAP_NONE = 2,
    GS_PLAYER_STATE_MAP_VERSION_ERROR = 3,
    GS_PLAYER_STATE_UNUSED = 4,
    GS_PLAYER_STATE_KICK = 5,
    GS_PLAYER_STATE_READY = 6,
    GS_PLAYER_STATE_GAME_START = 7
};

/*
 * NAME :		GameSettings
 * DESCRIPTION :
 * 			Contains the lobby's game settings.
 * NOTES :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct GameSettings {
 /* 0x000 */ char PlayerNames[GAME_MAX_PLAYERS][16];
 /* 0x080 */ char PlayerClanTags[GAME_MAX_PLAYERS][8];
 /* 0x0c0 */ short PlayerSkins[GAME_MAX_PLAYERS];
 /* 0x0d0 */ short PlayerTeams[GAME_MAX_PLAYERS];
 /* 0x0e0 */ short PlayerClients[GAME_MAX_PLAYERS];
 /* 0x0f0 */ short PlayerStates[GAME_MAX_PLAYERS];
 /* 0x100 */ short PlayerTypes[GAME_MAX_PLAYERS];
 /* 0x110 */ float PlayerRanks[GAME_MAX_PLAYERS];
 /* 0x130 */ float PlayerRankDeviations[GAME_MAX_PLAYERS];
 /* 0x150 */ int PlayerAccountIds[GAME_MAX_PLAYERS];
 /* 0x170 */ int GameStartTime;
 /* 0x174 */ int GameLoadStartTime;
 /* 0x178 */ int PlayerCount;
 /* 0x17c */ int GameLevel;
 /* 0x180 */ int GameType;
 /* 0x184 */ int UNK_OFFSET_184;
 /* 0x188 */ char GameRules[0x17];

    // the following have not been found, if they are there.
    // short PlayerHeadset;
    // char TeamSpawnPointIds[GAME_MAX_PLAYERS];
    // u32 SpawnSeed;

} GameSettings;

typedef struct GameOptions {
    union {
        struct {
/* 0x00 */  char Turboslider : 8;
/* 0x01 */  char Hovership : 8;
/* 0x02 */  char Vehicle_02 : 8;
/* 0x03 */  char Vehicle_03 : 8;
/* 0x04 */  char UNK_04 : 8;
/* 0x05 */  char BaseDefense_Bots : 8;
/* 0x06 */  char UNK_06 : 8;
/* 0x07 */  char Nodes : 8;
/* 0x08 */  char BaseDefense_BaseAmmoHealth : 8;
/* 0x09 */  char UNK_09 : 8;
/* 0x0a */  char UNK_0A : 8;
/* 0x0b */  char BaseDefense_SmallTurrets : 8;
/* 0x0c */  char BaseDefense_GatlinTurrets : 8;
/* 0x0d */  char BaseTeleporter : 8;
/* 0x0e */  char ChaosModeAttritionMode : 8;
/* 0x0f */  char FragLimit : 8;
/* 0x10 */  char CapLimit : 8;
/* 0x11 */  char Weapons : 8;
/* 0x12 */  char Chargeboots : 8;
/* 0x13 */  char UnlimitedAmmo : 8;
/* 0x14 */  char SpawnWithWeapons : 8;
/* 0x15 */  char Teams : 8;
/* 0x16 */  char Headsets : 8;
/* 0x17 */  char PlayerNames : 8;
        } MultiplayerGameFlags;
    } GameFlags;
} GameOptions;

/*
 * NAME :		gameGetSettings
 * DESCRIPTION :
 * 			Returns a pointer to the active tNW_GameSettings object.
 * NOTES :
 * ARGS : 
 * RETURN :
 *          Returns NULL if no lobby.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
GameSettings * gameGetSettings(void);

GameOptions * gameGetOptions(void);

void gameSetClientState(int cliendId, int state);
void gameSetClientTeam(int cliendId, int team);
void gameSetClientSkin(int cliendId, int skin);
void gameSetClientName(int cliendId, char* name);

#endif // _LIBUYA_GAMESETTINGS_H_
