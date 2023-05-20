/***************************************************
 * FILENAME :		game.h
 * 
 * DESCRIPTION :
 * 		Contains game specific offsets and structures for UYA.
 * 
 * NOTES :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GAME_H_
#define _LIBUYA_GAME_H_

#include <tamtypes.h>
#include "common.h"

/*
 * Maximum number of players in a game.
 */
#define GAME_MAX_PLAYERS                (8)

/*
 * NAME :		isInGame
 * 
 * DESCRIPTION :
 * 			Whether the client is currently in game world.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int isInGame(void);

/*
 * NAME :		isInMenus
 * 
 * DESCRIPTION :
 * 			Whether the client is currently in the menus.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int isInMenus(void);

/*
 * NAME :		isSceneLoading
 * 
 * DESCRIPTION :
 * 			Whether the client is currently in a scene transition.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int isSceneLoading(void);

/*
 * NAME :		gameGetTime
 * 
 * DESCRIPTION :
 * 			Gets the current game time in milliseconds.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int gameGetTime(void);

/*
 * NAME :		gameAmIHost
 * 
 * DESCRIPTION :
 * 			Whether the current client is also the game host.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gameAmIHost(void);

/*
 * NAME :		gameGetMyClientId
 * 
 * DESCRIPTION :
 * 			Returns the current client's game/dme id.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gameGetMyClientId(void);

/*
 * NAME :		gameGetMyClientId
 * 
 * DESCRIPTION :
 * 			Returns the currently loaded (or loading) map id.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gameGetCurrentMapId(void);

/*
 * NAME :		gameGetGameModeName
 * 
 * DESCRIPTION :
 * 			Returns a pointer to the given gamemode's name.
 *      Returns NULL if not a valid gamemode.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
char* gameGetGameModeName(int gameModeId);

/*
 * NAME :		gameEnd
 * 
 * DESCRIPTION :
 * 			End game.
 * 
 * NOTES :
 * 
 * ARGS : 
 *      reason :      Why the game ended.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void gameEnd(int reason);

#endif // _LIBUYA_GAME_H_
