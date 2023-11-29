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
#include "player.h"

enum UIPointers
{
    UIP_SELECT_PROFILE = 0,
    UIP_ONLINE_AGREEMENT = 1,
    UIP_ANNOUNCEMENTS = 2,
    UIP_IGNORED_PLAYERS = 3,
    UIP_BUDDY_LIST = 4,
    UIP_CLAN_1 = 5,
    UIP_CLAN_2 = 6,
    UIP_CLAN_3 = 7,
    UIP_CLAN_4 = 8,
    UIP_CLAN_5 = 9,
    UIP_CLAN_6 = 10,
    UIP_CLAN_7 = 11,
    UIP_CREATE_GAME = 12,
    UIP_CREATE_GAME_ADVANCED_OPTIONS = 13,
    UIP_UNK_POPUP = 14,
    UIP_FIND_GAME = 15,
    UIP_GAME_DETAILS = 16,
    UIP_STAGING_SECONDARY_PLAYER_OPTIONS = 17,
    UIP_END_GAME_DETAILS = 18,
    UIP_KEYBOARD = 19,
    UIP_ONLINE_AGREEMENT_2 = 20,
    UIP_LOCAL_PLAY_CREATE_GAME = 21,
    UIP_STAGING_UNK = 22,
    UIP_LOCAL_PLAY_STAGING = 23,
    UIP_ONLINE_LOCAL_EDIT_PROFILE_MENU = 24,
    UIP_ONLINE_LOBBY = 25,
    UIP_PASSWORD_ENTRY = 26,
    UIP_PLAYER_DETAILS = 27,
    UIP_PRIVATE_MESSAGE = 28,
    UIP_EDIT_PROFILE = 29,
    UIP_RECENT_PLAYERS = 30,
    UIP_CITY = 31,
    UIP_UNK_2 = 32,
    UIP_STAGING = 33,
    UIP_CHANGE_SKIN_TEAM = 34,
    UIP_STATS = 35,
    UIP_CITY_SELECT = 36,
    UIP_UNK_3 = 37,
    UIP_TIPS = 38,
    UIP_IN_GAME_DETAILS = 39,
    UIP_WEAPON_SELECT = 40,
    UIP_SELECT_BOX = 41,
    UIP_CONTROLS = 42,
    UIP_TIPS_2 = 43,
    UIP_NULL = 44,
};

typedef struct FontWindow { // 0x1c
	/* 0x00 */ short int win_top;
	/* 0x02 */ short int win_bot;
	/* 0x04 */ short int win_left;
	/* 0x06 */ short int win_right;
	/* 0x08 */ short int text_x;
	/* 0x0a */ short int text_y;
	/* 0x0c */ short int max_width;
	/* 0x0e */ short int max_height;
	/* 0x10 */ short int line_spacing;
	/* 0x12 */ short int flags;
	/* 0x14 */ short int sub_pixel_x;
	/* 0x16 */ short int sub_pixel_y;
	/* 0x18 */ short int drop_shadow_offset_x;
	/* 0x1a */ short int drop_shadow_offset_y;
} FontWindow;

enum UiElementType
{
    UI_ELEMENT_MENU = -1,
    UI_ELEMENT_BUTTON = 0,
    UI_ELEMENT_NUMBER_SELECT = 8,
    UI_ELEMENT_TEXT = 10,
};

typedef struct UiElement
{
    enum UiElementType Type;
    int State;
    int LastState;
    struct UiElement* Parent;
} UiElement_t;

typedef struct UiTextElement
{
    UiElement_t Element;
    char PAD_10[0x48];
    void* VTable;
    unsigned int Id;
    char Text[200];
    /* 138 */
} UiTextElement_t;

typedef struct UiButtonElement
{
    UiElement_t Element;
    int SelectId;
    int Id;
    char Text[64];
    void* VTable;
    int PAD_5C;
    /* 60 */
} UiButtonElement_t;

typedef struct UiMenu
{
    UiElement_t Element;
    char PAD_10[0x48];
    void* VTable;
    char PAD_5C[0x40];
    int Id;
    int ReturnToMenuId;
    int LastMenuId;
    char PAD_A8[0x04];
    int ChildCount;
    UiElement_t* Children[64];
} UiMenu_t;

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
int uiShowInputDialog(const char * title, char * value, int maxLength);
int uiShowSelectDialog(const char * option1, const char * option2);

/*
 * NAME :		uiShowPopup
 * 
 * DESCRIPTION :
 * 			Shows a popup with the given text to the given local player.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          message : Mesasge to show
 *          seconds : how many seconds to show the popup (-1 for default time)
 *          player  : The local player to show the message too.
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void uiShowPopup(Player * player, const char * message, int seconds);

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

__LIBUYA_GETTER__ UiMenu_t* uiGetPointer(int id);
UiMenu_t* uiGetActivePointer();
UiMenu_t* uiGetActivePointerSlot(int slot);

#endif // _LIBUYA_UI_H_
