/***************************************************
 * FILENAME :		module.h
 * DESCRIPTION :
 * 		Modules are a way to optimize the limited memory available on the PS2.
 *      Modules are dynamically loaded by the server and dynamically invoked by the patcher.
 *      This header file contains the relevant structues for patch modules.
 * NOTES :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _MODULE_H_
#define _MODULE_H_


#include <tamtypes.h>
#include <libuya/gamesettings.h>
#include "config.h"

// Forward declarations
struct GameModule;

/*
 * NAME :		ModuleStart
 * DESCRIPTION :
 * 			Defines the function pointer for all module entrypoints.
 *          Modules will provide a pointer to their entrypoint that will match
 *          this type.
 * NOTES :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef void (*ModuleStart)(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig);


/*
 * NAME :		GameModuleState
 * DESCRIPTION :
 * 			Contains the different states for a game module.
 *          The state will define how the patcher handles the module.
 * NOTES :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef enum GameModuleState
{
    /*
     * Module is not active.
     */
    GAMEMODULE_OFF,

    /*
     * The module will be invoked when the game/lobby starts and
     * it will be set to 'OFF' when the game/lobby ends.
     */
    GAMEMODULE_TEMP_ON,

    /*
     * The module will always be invoked as long as the player
     * is in a game.
     */
    GAMEMODULE_ALWAYS_ON
} GameModuleState;


/*
 * NAME :		GameModule
 * DESCRIPTION :
 * 			A game module is a dynamically loaded and invoked program.
 *          It contains a state and an entrypoint pointer.
 * NOTES :
 *          Game modules are only executed while the player is in a game.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct GameModule
{
    /*
     * State of the module.
     */
    GameModuleState State;

    /*
     * Entrypoint of module to be invoked when in game.
     */
    ModuleStart GameEntrypoint;

    /*
     * Entrypoint of module to be invoked when in staging or menus.
     */
    ModuleStart LobbyEntrypoint;

    /*
     * Entrypoint of module to be invoked just before the level loads the map but after the map is read from the disc.
     */
    ModuleStart LoadEntrypoint;

} GameModule;

typedef struct UpdateGameStateRequest {
	char TeamsEnabled;
    char PADDING;
    short Version;
	int TeamScores[GAME_MAX_PLAYERS];
	char ClientIds[GAME_MAX_PLAYERS];
	char Teams[GAME_MAX_PLAYERS];
    int Nodes[2]; // Blue Team, Red Team
} UpdateGameStateRequest_t;

typedef struct CustomGameModeStats
{
  u8 Payload[1024 * 6];
} __attribute__((aligned(16))) CustomGameModeStats_t;

typedef struct PatchStateContainer {
    int CustomMapId;
    int UpdateGameState;
    UpdateGameStateRequest_t GameStateUpdate;
    int UpdateCustomGameStats;
    CustomGameModeStats_t CustomGameStats;
    GameSettings GameSettingsAtStart;
    int CustomGameStatsSize;
    int ClientsReadyMask;
    int AllClientsReady;
    int VoteToEndPassed;
} PatchStateContainer_t;

#endif // _MODULE_H_
