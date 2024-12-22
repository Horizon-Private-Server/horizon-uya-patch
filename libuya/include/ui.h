/***************************************************
 * FILENAME :		ui.h
 * DESCRIPTION :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_UI_H_
#define _LIBUYA_UI_H_

#include "common.h"
#include <tamtypes.h>
#include "player.h"

enum UiMenuIds {
    UI_MENU_SELECT_PROFILE = 0,
    UI_MENU_ONLINE_AGREEMENT = 1,
    UI_MENU_ANNOUNCEMENTS = 2,
    UI_MENU_IGNORED_PLAYERS = 3,
    UI_MENU_BUDDY_LIST = 4,
    UI_MENU_CLAN_1 = 5,
    UI_MENU_CLAN_2 = 6,
    UI_MENU_CLAN_3 = 7,
    UI_MENU_CLAN_4 = 8,
    UI_MENU_CLAN_DETAILS = 9,
    UI_MENU_CLAN_6 = 10,
    UI_MENU_CLAN_7 = 11,
    UI_MENU_CREATE_GAME = 12,
    UI_MENU_CREATE_GAME_ADVANCED_OPTIONS = 13,
    UI_MENU_UNK_POPUP = 14,
    UI_MENU_FIND_GAME = 15,
    UI_MENU_GAME_DETAILS = 16,
    UI_MENU_STAGING_INVITE_PLAYERS = 17,
    UI_MENU_END_GAME_DETAILS = 18,
    UI_MENU_KEYBOARD = 19,
    UI_MENU_ONLINE_AGREEMENT_2 = 20,
    UI_MENU_LOCAL_PLAY_CREATE_GAME = 21,
    UI_MENU_STAGING_UNK = 22,
    UI_MENU_LOCAL_PLAY_STAGING = 23,
    UI_MENU_ONLINE_LOCAL_EDIT_PROFILE_MENU = 24,
    UI_MENU_ONLINE_LOBBY = 25,
    UI_MENU_PASSWORD_ENTRY = 26,
    UI_MENU_PLAYER_DETAILS = 27,
    UI_MENU_PRIVATE_MESSAGE = 28,
    UI_MENU_EDIT_PROFILE = 29,
    UI_MENU_RECENT_PLAYERS = 30,
    UI_MENU_CITY = 31,
    UI_MENU_UNK_2 = 32,
    UI_MENU_STAGING = 33,
    UI_MENU_CHANGE_SKIN_TEAM = 34,
    UI_MENU_STATS = 35,
    UI_MENU_CITY_SELECT = 36,
    UI_MENU_UNK_3 = 37,
    UI_MENU_TIPS = 38,
    UI_MENU_IN_GAME_DETAILS = 39,
    UI_MENU_WEAPON_SELECT = 40,
    UI_MENU_UNK_4 = 41,
    UI_MENU_CONTROLS = 42,
    UI_MENU_TIPS_2 = 43,
    UI_MENU_NULL = 44,
};

enum UiElementType {
    UI_ELEMENT_MENU = -1,
    UI_ELEMENT_BUTTON = 0,
    UI_ELEMENT_NUMBER_SELECT = 8,
    UI_ELEMENT_TEXT = 10,
};

enum KeyboardKeys {
    KEY_DESCIPTION = 0,
    KEY_UNK_1 = 1,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_BACKSLASH,
    KEY_SEMICOLON,
    KEY_APOSTROPHIE,
    KEY_BRACKET_OPEN,
    KEY_BRACKET_CLOSED,
    KEY_MINUS,
    KEY_EQUALS,
    KEY_SHIFT,
    KEY_CAPS,
    KEY_SPACE,
    KEY_DONE,
    KEY_DEL,
    KEY_CANCEL,
    KEY_ALT
} KeyboardKeys_t;

typedef struct UiElement { // 0x6c
/* 0x00 */ enum UiElementType type;
/* 0x04 */ int state;
/* 0x08 */ int lastState;
/* 0x00 */ int unk_0c;
/* 0x10 */ struct UiElement* pParent;
/* 0x14 */ char text[56];
/* 0x4c */ int pad;
/* 0x50 */ float boxSize[4];
/* 0x60 */ void * vtable;
/* 0x64 */ int unk_64;
/* 0x68 */ int unk_68;
} UiElement_t;

typedef struct UiMenu {
/* 0x000 */ char unk_00[0x60];
/* 0x060 */ void* vTable;
/* 0x064 */ char unk_064[0xa4];
/* 0x108 */ int returnToMenuId;
/* 0x10c */ int childCount;
/* 0x110 */ UiElement_t* pChildren[96];
/* 0x290 */ int selectedIndex;
} UiMenu_t;

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

/*
 * NAME :		uiGetActive
 * DESCRIPTION :
 * 			Gets the current ui menu id.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int uiGetActive(void);

/*
 * NAME :		uiShowYesNoDialog
 * DESCRIPTION :
 * 			Shows a blocking yes no dialog.
 * NOTES :
 * ARGS : 
 * RETURN :
 *          Returns 1 if 'Yes' is selected.
 *          Returns 2 if 'No' is selected.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int uiShowYesNoDialog(const char * title, const char * description);

/*
 * NAME :		uiShowOkDialog
 * DESCRIPTION :
 * 			Shows a blocking ok dialog.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int uiShowOkDialog(const char * title, const char * description);
int uiShowInputDialog(const char * title, char * value, int maxLength);
int uiShowSelectDialog_Simple(const char * option1, const char * option2);
int uiShowSelectDialog(const char * title, const char * items[], int itemCount, int selectedIndex);
int uiShowChangeTeamSkinDialog(void * team, void * skin, int numTeams, int bChangeTeams, int bUnlockDan, int bUnlockNefarious);
int uiShowInvitePlayersDialog(void);

/*
 * NAME :		uiShowPopup
 * DESCRIPTION :
 * 			Shows a popup with the given text to the given local player.
 * NOTES :
 * ARGS : 
 *          message : Mesasge to show
 *          seconds : how many seconds to show the popup (-1 for default time)
 *          player  : The local player to show the message too.
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void uiShowPopup(Player * player, const char * message, int seconds);

/*
 * NAME :		uiShowHelpPopup
 * DESCRIPTION :
 * 			Shows a help popup with the given text to the given local player.
 * NOTES :
 * ARGS : 
 *      localPlayerIndex    :               Local player to show popup for.
 *      message             :               Message to show.
 *      seconds             :               How many seconds to show the popup for.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void uiShowHelpPopup(int localPlayerIndex, const char * message, int seconds);

/*
 * NAME :		uiMsgString
 * DESCRIPTION :
 * 			Returns the string associated with the given text id.
 * NOTES :
 * ARGS : 
 *      textId              :               Id of the string to return.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
char * uiMsgString(int textId);

u32 uiGetPointer(int UI);
u32 uiGetActiveMenu(int UI);
u32 uiGetActiveSubMenu(int UI);

#endif // _LIBUYA_UI_H_
