/***************************************************
 * FILENAME :		gamesettings.h
 * 
 * DESCRIPTION :
 * 		Contains lobby specific offsets and structures for Deadlocked.
 * 
 * NOTES :
 * 		Each offset is determined per app id.
 * 		This is to ensure compatibility between versions of Deadlocked/Gladiator.
 * 		
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

enum GameRuleIds
{
    GAMERULE_SEIGE,
    GAMERULE_CTF,
    GAMERULE_DM
};

/*
 * NAME :		GameSettings
 * 
 * DESCRIPTION :
 * 			Contains the lobby's game settings.
 * 
 * NOTES :
 * 
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct GameSettings
{
    char PlayerNames[GAME_MAX_PLAYERS][16];
    char PlayerClanTags[GAME_MAX_PLAYERS][8];
    char PlayerSkins[GAME_MAX_PLAYERS];
    char PlayerTeams[GAME_MAX_PLAYERS];
    char PlayerClients[GAME_MAX_PLAYERS];
    char PlayerStates[GAME_MAX_PLAYERS];
    char UNK_OFFSET_100[16];
    float PlayerRanks[GAME_MAX_PLAYERS];
    float PlayerRankDeviations[GAME_MAX_PLAYERS];
    int PlayerAccountIds[GAME_MAX_PLAYERS];
    int GameStartTime;
    int GameLoadStartTime;
    int PlayerCount;
    int GameLevel;
    int GameType;
    int UNK_OFFSET_184;
    char GameRules[0x17];

    // the following have not been found, if they are there.
    // short PlayerHeadset;
    // char TeamSpawnPointIds[GAME_MAX_PLAYERS];
    // u32 SpawnSeed;

} GameSettings;

typedef struct GameOptions
{
    union {
        struct {
            char Vehicle_00 : 8;
            char Vehicle_01 : 8;
            char Vehicle_02 : 8;
            char Vehicle_03 : 8;
            char Nodes : 8;
            char UNK_05 : 8;
            char UNK_06 : 8;
            char UNK_07 : 8;
            char UNK_08 : 8;
            char UNK_09 : 8;
            char UNK_0A : 8;
            char UNK_0B : 8;
            char FragLimit : 8;
            char ChaosModeAttritionMode : 8;
            char UNK_0E : 8;
            char BaseDefenses : 8;
            char UnlimitedAmmo : 8;
            char SpawnWithChargeboots : 8;
            char Weapons : 8;
            char CapsToWin : 8;
            char PlayerNames : 8;
            char UNK_15 : 8;
            char Teams : 8;
            char SpawnWithWeapons : 8;
        } MultiplayerGameFlags;
    } GameFlags;
} GameOptions;

#endif // _LIBUYA_GAMESETTINGS_H_
