/***************************************************
 * FILENAME :		ui.h
 * 
 * DESCRIPTION :
 * 		
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_UI_H_
#define _LIBUYA_UI_H_

#include "common.h"
#include <tamtypes.h>

enum UiIds
{
    UI_ID_MULTIPLAYER_MENU = 0xFF,
    UI_ID_DNAS_SELECT = 0x11A,
    UI_ID_ONLINE_SELECT_PROFILE = 0x10E,
    UI_ID_EDIT_PROFILE = 0x123,
    UI_ID_POPUP_YES_NO1 = 0x13B,
    UI_ID_POPUP_YES_NO2 = 0x24d,
    UI_ID_POPUP_OK = 0x270,
    UI_ID_KEYBOARD = 0x21B,
    UI_ID_ONLINE_MAIN_MENU = 0x12C,
    UI_ID_MENU_QUICKPLAY = 0x14F,
    UI_ID_MENU_CREATE_GAME = 0x136,
    UI_ID_CREATE_GAME_WEAPON_OPTIONS = 0x156,
    UI_ID_CREATE_GAME_VEHICLES = 0x14E,
    UI_ID_GAME_LOBBY = 0x23E,
    UI_ID_GAME_LOBBY_GAME_DETAILS = 0x26F,
    UI_ID_GAME_LOBBY_STAGING_ACTIONS = 0x261,
    UI_ID_GAME_LOBBY_CHANGE_TEAM = 0x259,
    UI_ID_GAME_LOBBY_INVITE_PLAYER = 0x261,
    UI_ID_GAME_LOBBY_INVITE_KEYBOARD = 0x323,
    UI_ID_MENU_FIND_GAME = 0x14C,
    UI_ID_FIND_GAME_DETAILS = 0x153,
    UI_ID_MENU_BUDDIES = 0x14b,
    UI_ID_VIEW_PROFILE = 0x122,
    UI_ID_BUDDIES_ADD_IGNORE = 0x230,
    UI_ID_MENU_CLAN = 0x119,
    UI_ID_CREATE_CLAN = 0x1FE,
    UI_ID_MENU_STATS = 0x14C,
};

enum UIPointers
{
    UIP_SELECT_PROFILE = 0x0,
    UIP_ONLINE_AGREEMENT,
    UIP_ANNOUNCEMENTS,
    UIP_IGNORED_PLAYERS,
    UIP_BUDDY_LIST,
    UIP_CLAN_1,
    UIP_CLAN_2,
    UIP_CLAN_3,
    UIP_CLAN_4,
    UIP_CLAN_5,
    UIP_CLAN_6,
    UIP_CLAN_7,
    UIP_CREATE_GAME,
    UIP_CREATE_GAME_ADVANCED_OPTIONS,
    UIP_UNK_POPUP,
    UIP_FIND_GAME,
    UIP_GAME_DETAILS,
    UIP_STAGING_SECONDARY_PLAYER_OPTIONS,
    UIP_END_GAME_DETAILS,
    UIP_KEYBOARD,
    UIP_ONLINE_AGREEMENT_2,
    UIP_LOCAL_PLAY_CREATE_GAME,
    UIP_STAGING_UNK,
    UIP_LOCAL_PLAY_STAGING,
    UIP_ONLINE_LOCAL_EDIT_PROFILE_MENU,
    UIP_ONLINE_LOBBY,
    UIP_PASSWORD_ENTRY,
    UIP_PLAYER_DETAILS,
    UIP_PRIVATE_MESSAGE,
    UIP_EDIT_PROFILE,
    UIP_RECENT_PLAYERS,
    UIP_UNK_1,
    UIP_UNK_2,
    UIP_STAGING ,
    UIP_CHANGE_SKIN_TEAM,
    UIP_STATS,
    UIP_CITY_SELECT,
    UIP_UNK_3,
    UIP_TIPS,
    UIP_IN_GAME_DETAILS,
    UIP_WEAPON_SELECT,
    UIP_UNK_4,
    UIP_CONTROLS,
    UIP_TIPS_2,
    UIP_NULL = 44,
};

/*
 * NAME :		uiGetActive
 * 
 * DESCRIPTION :
 * 			Gets the current ui menu id.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int uiGetActive(void);

/*
 * NAME :		uiShowYesNoDialog
 * 
 * DESCRIPTION :
 * 			Shows a blocking yes no dialog.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 *          Returns 1 if 'Yes' is selected.
 *          Returns 2 if 'No' is selected.
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int uiShowYesNoDialog(const char * title, const char * description);

/*
 * NAME :		uiShowOkDialog
 * 
 * DESCRIPTION :
 * 			Shows a blocking ok dialog.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int uiShowOkDialog(const char * title, const char * description);

/*
 * NAME :		uiShowPopup
 * 
 * DESCRIPTION :
 * 			Shows a popup with the given text to the given local player.
 * 
 * NOTES :
 * 
 * ARGS : 
 *      localPlayerIndex    :               Local player to show popup for.
 *      message             :               Message to show.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void uiShowPopup(int localPlayerIndex, const char * message);

/*
 * NAME :		uiShowHelpPopup
 * 
 * DESCRIPTION :
 * 			Shows a help popup with the given text to the given local player.
 * 
 * NOTES :
 * 
 * ARGS : 
 *      localPlayerIndex    :               Local player to show popup for.
 *      message             :               Message to show.
 *      seconds             :               How many seconds to show the popup for.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void uiShowHelpPopup(int localPlayerIndex, const char * message, int seconds);

/*
 * NAME :		uiMsgString
 * 
 * DESCRIPTION :
 * 			Returns the string associated with the given text id.
 * 
 * NOTES :
 * 
 * ARGS : 
 *      textId              :               Id of the string to return.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
char * uiMsgString(int textId);

u32 uiGetPointer(int UI);
u32 uiGetActivePointer(int UI);
u32 uiGetActiveSubPointer(int UI);

#endif // _LIBUYA_UI_H_
