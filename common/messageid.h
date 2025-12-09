
/***************************************************
 * FILENAME :		messageid.h
 * DESCRIPTION :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _MESSAGEID_H_
#define _MESSAGEID_H_
#include "config.h"

/*
 * NAME :		CustomMessageId
 * DESCRIPTION :
 * 			Contains the different message ids.
 * NOTES :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
enum CustomMessageId
{
    UNUSED = 0,

    /*
     * Sent to the client when the server initiates a data download.
     */
    CUSTOM_MSG_ID_SERVER_DOWNLOAD_DATA_REQUEST = 1,

    /*
     * Sent from the client to the server when the client receives a data request message.
     */
    CUSTOM_MSG_ID_CLIENT_DOWNLOAD_DATA_RESPONSE = 2,

    /*
     * Sent to the server when the client wants the map loader irx modules.
     */
    CUSTOM_MSG_ID_CLIENT_REQUEST_MAP_IRX_MODULES = 3,

    /*
     * Sent after the server has sent all the map loader irx modules.
     */
    CUSTOM_MSG_ID_SERVER_SENT_MAP_IRX_MODULES = 4,

    /*
     * Info on custom map.
     */
    CUSTOM_MSG_ID_SET_MAP_OVERRIDE = 5,

    /*
     * Sent in response to CUSTOM_MSG_ID_SET_MAP_OVERRIDE by the client.
     */
    CUSTOM_MSG_ID_SET_MAP_OVERRIDE_RESPONSE = 6,

    /*
     * Client requests server to sent current map override info.
     */
    CUSTOM_MSG_ID_REQUEST_MAP_OVERRIDE = 7,

    /*
     * Sent from the client to the server when the client has updated their patch config.
     */
    CUSTOM_MSG_ID_CLIENT_USER_CONFIG = 8,

    /*
     * Sent from the client to the server when the client has updated their patch game config.
     */
    CUSTOM_MSG_ID_CLIENT_USER_GAME_CONFIG = 9,

    /*
     * Sent from the server to the client when the host has changed and is propogating the patch game config.
     */
    CUSTOM_MSG_ID_SERVER_SET_GAME_CONFIG = 10,

    /*
     * Sent by the host when a game lobby has started.
     */
    CUSTOM_MSG_ID_GAME_LOBBY_STARTED = 11,

    /*
     * Sent by the host when a game lobby has ended.
     */
    CUSTOM_MSG_ID_GAME_LOBBY_ENDED = 12,

    /*
     * Sent from the client to the server when the client wants to redownload the patch.
     */
    CUSTOM_MSG_ID_CLIENT_REQUEST_PATCH = 13,

    /*
     * Sent from the client to the server with the client's machine id (MAC address).
     */
    CUSTOM_MSG_ID_CLIENT_SET_MACHINE_ID = 14,

    /*
     * Sent from a client to the host when they pick up the flag.
     */
    CUSTOM_MSG_ID_FLAG_REQUEST_PICKUP = 15,

    /*
     * Sent by the client to the server when the client has collected a horizon bolt.
     */
    CUSTOM_MSG_ID_CLIENT_PICKED_UP_HORIZON_BOLT = 16,

    /*
     * Sent by the client to the server when the client requests the current scavenger hunt settings.
     */
    CUSTOM_MSG_ID_CLIENT_REQUEST_SCAVENGER_HUNT_SETTINGS = 17,

    /*
     * Sent by the server to the client containing the current scavenger hunt settings.
     */
    CUSTOM_MSG_ID_SERVER_RESPONSE_SCAVENGER_HUNT_SETTINGS = 18,

    /*
     * Sent by the client to the server when the client requests MONTH and DAY (no year).
     */
    CUSTOM_MSG_ID_CLIENT_REQUEST_DATE_SETTINGS = 19,

    /*
     * Sent by the server to the client containing the current MONTH and DAY (no year).
     */
    CUSTOM_MSG_ID_CLIENT_RESPONSE_DATE_SETTINGS = 20,

    /*
     * Sent to the server when a player votes to end
     */
    CUSTOM_MSG_ID_PLAYER_VOTED_TO_END = 21,

    /*
     * sent to the client to update voes to end state
     */
    CUSTOM_MSG_ID_VOTE_TO_END_STATE_UPDATED = 22,

    /*
     * Sent to the client to invite bots
     */
    CUSTOM_MSG_ID_INVITE_BOT = 23,

    /*
     * Sent from the server to the host when the server wants the host to update all the teams
     */
    CUSTOM_MSG_ID_SERVER_REQUEST_TEAM_CHANGE = 24,

    /*
     * Sent to server for game details/state
     */
    CUSTOM_MSG_ID_CLIENT_SET_GAME_STATE = 25,

    /*
     * Sent to server for game details/state
     */
    CUSTOM_MSG_PLAYER_SYNC_STATE_UPDATE = 27,
    /*
     * Sent every second to sync KOTH personal score.
     */
    CUSTOM_MSG_ID_KOTH_SCORE_UPDATE = 28,

    /*
     * Start of custom message ids reserved for custom game modes.
     */
    CUSTOM_MSG_ID_GAME_MODE_START = 100,
};

typedef struct ServerDownloadDataRequest
{
    int Id;
    int TargetAddress;
    int TotalSize;
    int DataOffset;
    short Chunk;
    short DataSize;
    char Data[2048];
} ServerDownloadDataRequest_t;

typedef struct ClientDownloadDataResponse
{
    int Id;
    int BytesReceived;
} ClientDownloadDataResponse_t;

typedef struct ClientSetGameConfig
{
  PatchGameConfig_t GameConfig;
  CustomMapDef_t CustomMap;
} ClientSetGameConfig_t;

typedef struct ClientRequestPickUpFlag
{
    int GameTime;
    int PlayerId;
    u32 FlagUID;
} ClientRequestPickUpFlag_t;

typedef struct ScavengerHuntSettingsResponse
{
  int Enabled;
  float SpawnFactor;
} ScavengerHuntSettingsResponse_t;

typedef struct DateResponse {
  char Month;
  char Day;
  char Hour;
  char Minute;
  char Second;
} DateResponse_t;

typedef struct KothScoreUpdate {
  char PlayerIdx;
  short Score;
  char Padding;
} KothScoreUpdate_t;

#endif // _MESSAGEID_H_
