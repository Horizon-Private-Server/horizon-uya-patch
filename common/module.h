/***************************************************
 * FILENAME :		module.h
 * 
 * DESCRIPTION :
 * 		Modules are a way to optimize the limited memory available on the PS2.
 *      Modules are dynamically loaded by the server and dynamically invoked by the patcher.
 *      This header file contains the relevant structues for patch modules.
 * 
 * NOTES :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _MODULE_H_
#define _MODULE_H_


#include <tamtypes.h>

// Forward declarations
struct GameModule;

/*
 * Size of game module arg buffer.
 */
#define GAME_MODULE_ARG_SIZE            (8)

/*
 * NAME :		ModuleStart
 * 
 * DESCRIPTION :
 * 			Defines the function pointer for all module entrypoints.
 *          Modules will provide a pointer to their entrypoint that will match
 *          this type.
 * 
 * NOTES :
 * 
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef void (*ModuleStart)(struct GameModule * module);


/*
 * NAME :		GameModuleState
 * 
 * DESCRIPTION :
 * 			Contains the different states for a game module.
 *          The state will define how the patcher handles the module.
 * 
 * NOTES :
 * 
 * 
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
 * 
 * DESCRIPTION :
 * 			A game module is a dynamically loaded and invoked program.
 *          It contains a state and an entrypoint pointer.
 * 
 * NOTES :
 *          Game modules are only executed while the player is in a game.
 * 
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
     * Buffer containing module specific arguments given by the server.
     */
    char Argbuffer[GAME_MODULE_ARG_SIZE];

} GameModule;



#endif // _MODULE_H_
