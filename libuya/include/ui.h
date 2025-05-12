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

typedef enum uiPadButtons {
    UI_PAD_NONE = 0,
    UI_PAD_DPAD_UP = 1,
    UI_PAD_DPAD_DOWN = 2,
    UI_PAD_DPAD_LEFT = 3,
    UI_PAD_DPAD_RIGHT = 4,
    UI_PAD_TRIANGLE = 5,
    UI_PAD_CIRCLE = 6,
    UI_PAD_CROSS = 7,
    UI_PAD_SQUARE = 8,
    UI_PAD_SELECT = 9,
    UI_PAD_START = 10,
    UI_PAD_L1 = 11,
    UI_PAD_L2 = 12,
    UI_PAD_L3 = 13,
    UI_PAD_R1 = 14,
    UI_PAD_R2 = 15,
    UI_PAD_R3 = 16,
    UI_PAD_TOTAL = UI_PAD_R3
} uiPadButtons_e;

enum UiKeyboard {
    KEY_DESCIPTION = 0,
    KEY_TEXT_INPUT = 1,
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
};

enum UiElementType {
    UI_ELEMENT_MENU = -1,
    UI_ELEMENT_TEXT = 1,
    UI_ELEMENT_BOOL_SELECT = 2,
    UI_ELEEMNT_STRING_SELECT = 3,
    UI_ELEMENT_IMAGE = 4,
    UI_ELEMENT_LIST = 5,
    UI_ELEMENT_RANGE_SELECT = 6,
    UI_ELEMENT_TEXT_INFO = 7,
    UI_ELEMENT_TEXT_INPUT = 8,
    UI_ELEMENT_SPRITE = 12,
};

typedef struct UiVTable {
/* 0x00 */ int *unk_00[0x14];
/* 0x50 */ void (*setColor)(int ui, u32 color);
} UiVTable_t;

typedef struct UiElementText { // 0x6c
/* 0x00 */ enum UiElementType type;
/* 0x04 */ int state;
/* 0x08 */ int lastState;
/* 0x0c */ int unk_0c;
/* 0x10 */ struct UiElementText* pParent;
/* 0x14 */ char title[56];
/* 0x4c */ int padding;
/* 0x50 */ float selectorBoxSize[4];
/* 0x60 */ UiVTable_t* vTable;
/* 0x64 */ int unk_64;
/* 0x68 */ int unk_68;
} UiElementText_t;

typedef struct UiElementBoolSelect { // 0x74
/* 0x00 */ enum UiElementType type;
/* 0x04 */ int state;
/* 0x08 */ int lastState;
/* 0x0c */ int unk_0c;
/* 0x10 */ struct UiElementBoolSelect* pParent;
/* 0x14 */ char title[56];
/* 0x4c */ int padding;
/* 0x50 */ float selectorBoxSize[4];
/* 0x60 */ UiVTable_t* vTable;/* 0x64 */ int selectedIndex;
/* 0x68 */ int unk_68;
/* 0x6c */ int unk_6c;
/* 0x70 */ int unk_70;
} UiElementBoolSelect_t;

typedef struct UiElementStringSelect { // 0x1494
/* 0x0000 */ enum UiElementType type;
/* 0x0004 */ int state;
/* 0x0008 */ int lastState;
/* 0x000c */ int unk_0c;
/* 0x0010 */ struct UiElementStringSelect* pParent;
/* 0x0014 */ char text[56];
/* 0x004c */ int padding;
/* 0x0050 */ float selectorBoxSize[4];
/* 0x0060 */ UiVTable_t* vTable;
/* 0x0064 */ int itemCount;
/* 0x0068 */ char items[64][80];
/* 0x1468 */ int selectedIndex;
/* 0x146c */ int selectedIndex2;
/* 0x1470 */ char unk_1470[0x24];
} UiElementStringSelect_t;

typedef struct UiElementImage { // 0x84
/* 0x00 */ enum UiElementType type;
/* 0x04 */ int state;
/* 0x08 */ int lastState;
/* 0x0c */ int unk_0c;
/* 0x10 */ struct UiElementImage* pParent;
/* 0x14 */ char title[56];
/* 0x4c */ int padding;
/* 0x50 */ float selectorBoxSize[4];
/* 0x60 */ UiVTable_t* vTable;
/* 0x64 */ char unk_64[0x18];
/* 0x7c */ int imageId;
/* 0x80 */ int unk_80;
} UiElementImage_t;

typedef struct UiElementList { // 105a4
/* 0x00000 */ enum UiElementType type;
/* 0x00004 */ int state;
/* 0x00008 */ int lastState;
/* 0x0000c */ int unk_0c;
/* 0x00010 */ struct UiElementList* pParent;
/* 0x00014 */ char text[56];
/* 0x0004c */ int padding;
/* 0x00050 */ float selectorBoxSize[4];
/* 0x00060 */ UiVTable_t* vTable;/* 0x00064 */ int itemCount;
/* 0x00068 */ char unk_00064[0x1053c];
} UiElementList_t;

typedef struct UiElementRangeSelect { // 0xb8
/* 0x00 */ enum UiElementType type;
/* 0x04 */ int state;
/* 0x08 */ int lastState;
/* 0x0c */ int unk_0c;
/* 0x10 */ struct UiElementRangeSelect* pParent;
/* 0x14 */ char title[56];
/* 0x4c */ int padding;
/* 0x50 */ float selectorBoxSize[4];
/* 0x60 */ UiVTable_t* vTable;
/* 0x64 */ int selectedIndex;
/* 0x68 */ int selectedIndex2;
/* 0x6c */ int rangeMin;
/* 0x70 */ int rangeMax;
/* 0x74 */ int rangeStep;
/* 0x78 */ char text[40];
/* 0xa0 */ char unk_a0[0x18];
} UiElementRangeSelect_t;

typedef struct UiElementTextInfo { // 0xd0
/* 0x00 */ enum UiElementType type;
/* 0x04 */ int state;
/* 0x08 */ int lastState;
/* 0x0c */ int unk_0c;
/* 0x10 */ struct UiElementTextInfo* pParent;
/* 0x14 */ char title[56];
/* 0x4c */ int padding;
/* 0x50 */ float selectorBoxSize[4];
/* 0x60 */ UiVTable_t* vTable;
/* 0x64 */ char text[80];
/* 0xb4 */ int unk_b4;
/* 0xb8 */ int unk_b8;
/* 0xbc */ int unk_bc;
/* 0xc0 */ int bShowText;
/* 0xc4 */ int unk_c4;
/* 0xc8 */ u32 color;
/* 0xcc */ int pad;
} UiElementTextInfo_t;

typedef struct UiElementTextInput { // 0x4a8
/* 0x00 */ enum UiElementType type;
/* 0x04 */ int state;
/* 0x08 */ int lastState;
/* 0x0c */ int unk_0c;
/* 0x10 */ struct UiElementTextInput* pParent;
/* 0x14 */ char title[56];
/* 0x4c */ int padding;
/* 0x50 */ float selectorBoxSize[4];
/* 0x60 */ UiVTable_t* vTable;
/* 0x064 */ char textInput[16];
/* 0x074*/ char unk_74[0x434];
} UiElementTextInput_t;

typedef struct UiElementSprite { // 0x84
/* 0x00 */ enum UiElementType type;
/* 0x04 */ int state;
/* 0x08 */ int lastState;
/* 0x0c */ int unk_0c;
/* 0x10 */ struct UiElementSprite* pParent;
/* 0x14 */ char title[56];
/* 0x4c */ int padding;
/* 0x50 */ float selectorBoxSize[4];
/* 0x60 */ UiVTable_t* vTable;
/* 0x64 */ int unk_64;
/* 0x68 */ int sprite;
/* 0x6c */ u32 spriteColor;
/* 0x70 */ int unk_70[5];
} UiElementSprite_t;

typedef struct UiMenu {
/* 0x000 */ char unk_000[0x58];
/* 0x058 */ float shadow[2];
/* 0x060 */ UiVTable_t* vTable;
/* 0x064 */ char unk_064[0xa4];
/* 0x108 */ int returnToMenuId;
/* 0x10c */ int childCount;
/* 0x110 */ UiElementText_t* pChildren[96];
/* 0x290 */ int selectedIndex;
/* 0x294 */ void* pLastChild;
/* 0x298 */ int menuSize; // Fromst pFirstChild to pLastChild
/* 0x29c */ int startItem;
/* 0x2a0 */ float unk_2a0;
/* 0x2a4 */ void* pFirstChild;
/* 0x2a8 */ int unk_2a8;
/* 0x2ac */ int unk_2ac;
/* 0x2b0 */ char itemValues;
} UiMenu_t;

typedef struct UiOnlineLobbyElements {
// do stuff here later
} UiOnlineLobbyElements_t;

typedef struct UiCreateGameElements {
/* 0x00 */ UiElementTextInput_t* name;
/* 0x04 */ UiElementStringSelect_t* map;
/* 0x08 */ UiElementStringSelect_t* mode;
/* 0x0c */ UiElementRangeSelect_t* players;
/* 0x10 */ UiElementRangeSelect_t* buddySlots;
/* 0x14 */ UiElementRangeSelect_t* clanSlots;
/* 0x18 */ UiElementRangeSelect_t* localPlayers;
/* 0x1c */ UiElementRangeSelect_t* time;
/* 0x20 */ UiElementBoolSelect_t* usePassword;
/* 0x24 */ UiElementText_t* password;
/* 0x28 */ UiElementText_t* weapons;
/* 0x2c */ UiElementText_t* advancedOptions;
/* 0x30 */ UiElementImage_t* mapScreenshot;
/* 0x34 */ UiElementImage_t* miniMap;
} UiCreateGameElements_t;

typedef struct UiWeaponsElements { // 0x24
/* 0x00 */ UiElementSprite_t* fluxRifleSprite;
/* 0x04 */ UiElementSprite_t* n60StormSprite;
/* 0x08 */ UiElementSprite_t* blitzGunSprite;
/* 0x0c */ UiElementSprite_t* miniRocketTubeSprite;
/* 0x10 */ UiElementSprite_t* gravityBombSprite;
/* 0x14 */ UiElementSprite_t* mineGloveSprite;
/* 0x18 */ UiElementSprite_t* morphORaySprite;
/* 0x1c */ UiElementSprite_t* lavaGunSprite;
/* 0x20 */ UiElementText_t* descriptionText;
} UiWeaponsElements_t;

typedef struct UiAdvancedOptionsElements { // 0x30
/* 0x00 */ UiElementBoolSelect_t* vehicles;
/* 0x04 */ UiElementBoolSelect_t* nodes;
/* 0x08 */ UiElementBoolSelect_t* names;
/* 0x0c */ UiElementBoolSelect_t* baseDefenses;
/* 0x10 */ UiElementBoolSelect_t* unlimitedAmmo;
/* 0x14 */ UiElementBoolSelect_t* spawnWithWeapons;
/* 0x18 */ UiElementBoolSelect_t* chargeboots;
/* 0x1c */ UiElementRangeSelect_t* fragLimit;
/* 0x20 */ UiElementStringSelect_t* teams;
/* 0x24 */ UiElementStringSelect_t* ctfMode;
/* 0x28 */ UiElementRangeSelect_t* ctfCaps;
/* 0x2c */ UiElementStringSelect_t* siegeRules;
} UiAdvancedOptionsElements_t;

typedef struct UiStagingElements { // 0x180
/* 0x000 */ UiElementText_t* nameText;
/* 0x004 */ UiElementText_t* mapText;
/* 0x008 */ UiElementText_t* modeText;
/* 0x00c */ UiElementText_t* inviteText;
/* 0x010 */ UiElementText_t* startText;
/* 0x014 */ UiElementText_t* detailsText;
/* 0x018 */ UiElementImage_t* screenshotImage;
/* 0x01c */ UiElementText_t* playerHeadingText;
/* 0x020 */ UiElementText_t* skillHeadingText;
/* 0x024 */ UiElementSprite_t* voiceHeadingSprite;
/* 0x028 */ UiElementText_t* teamHeadingText;
/* 0x02c */ UiElementText_t* skinHeadingText;
/* 0x030 */ UiElementText_t* readyHeadingText;
/* 0x034 */ UiElementSprite_t* changeTeamRequestHeadingSprite;
/* 0x038 */ UiElementSprite_t* changeTeamRequestSprite;
/* 0x03c */ UiElementText_t* playerNameText[8];
/* 0x05c */ UiElementSprite_t* playerSkillOneSprite[8];
/* 0x07c */ UiElementSprite_t* playerSkillTwoSprite[8];
/* 0x09c */ UiElementSprite_t* playerSkillthreeSprite[8];
/* 0x0bc */ UiElementSprite_t* playerSkillFourSprite[8];
/* 0x0dc */ UiElementSprite_t* voiceSprite[8];
/* 0x0fc */ UiElementSprite_t* teamBackgroundSprite[8];
/* 0x11c */ UiElementSprite_t* teamForegroundSprite[8];
/* 0x13c */ UiElementText_t* skinText[8];
/* 0x15c */ UiElementSprite_t* readySprite[8];
} UiStagingElements_t;

typedef struct UiClanDetailsElements {
UiElementText_t* clanMembersText;
UiElementList_t* clanMembersList;
UiElementText_t* clanStatsText;
UiElementTextInfo_t* status;
UiElementTextInfo_t* overallRank;
UiElementTextInfo_t* winsLosses;
UiElementTextInfo_t* killsDeaths;
UiElementTextInfo_t* clanTagPreview;
UiElementTextInfo_t* challenges;
UiElementText_t* clanRoom;
UiElementText_t* invites;
UiElementText_t* clanMessage;
UiElementText_t* challengeClan;
UiElementText_t* challengeCancel;
UiElementText_t* challengeRoom;
UiElementText_t* clanTag;
UiElementText_t* leaveClan
} UiClanDetailsElements_t;

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

UiMenu_t* uiGetMenu(int UI);
UiMenu_t* uiGetActiveMenu(int UI, int whichMenu);

#endif // _LIBUYA_UI_H_
