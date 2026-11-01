#include "ui.h"
#include "player.h"
#include "time.h"
#include "interop.h"
#include "game.h"
#include "graphics.h"
#include <string.h>

/* Native HelpMsgs entry, using the 32-bit EE ABI. */
typedef struct uiTipMessage {
	/* 0x00 */ const char *text;
	/* 0x04 */ int id;
	/* 0x08 */ int speech;
	/* 0x0c */ short alternate;
	/* 0x0e */ short icon;
} uiTipMessage_t;

/* The native tip renderer owns 0x4c bytes of state. Keep a private copy so
 * custom tips do not alter tutorial history or consume a queued game tip. */
static u32 tipState[0x4c / 4];
static uiTipMessage_t tipMessage;
static u32 tipInitializeAddress;
static u32 tipRenderAddress;
static u32 *tipNativeState;
static uiTipMessage_t **tipNativeMessages;
static int tipFrames;
static int tipElapsed;

int internal_uiDialog(void *, const char *, const char *, int, int, float);
void internal_uiShowPopup(const char *, int, Player * player);
int internal_uiSelectDialog_Simple(void *, const char *, const char *, int);
int internal_uiSelectDialog(void *, const char *, const char **, int, int, int, int);
int internal_uiChangeTeamSkinDialog(void *, void *, void *, int, int, int, int, int);
int internal_uiInvitePlayersDialog(void *, void *, int, int);

int internal_uiPlaySound(int soundId, int flags, void* moby);

VariableAddress_t vaUiSetImage = {
#if UYA_PAL
	.Lobby = 0x006814b0,
#else
	.Lobby = 0x0067e990,
#endif
};

VariableAddress_t vaUiPlaySound = {
#if UYA_PAL
	.Lobby = 0x005f3548,
	.Bakisi = 0x004c5be8,
	.Hoven = 0x004c7d00,
	.OutpostX12 = 0x004bd5d8,
	.KorgonOutpost = 0x004bad70,
	.Metropolis = 0x004ba0c0,
	.BlackwaterCity = 0x004b7958,
	.CommandCenter = 0x004b7950,
	.BlackwaterDocks = 0x004ba1d0,
	.AquatosSewers = 0x004b94d0,
	.MarcadiaPalace = 0x004b8e50,
#else
	.Lobby = 0x005f0e48,
	.Bakisi = 0x004c3540,
	.Hoven = 0x004c5598,
	.OutpostX12 = 0x004baeb0,
	.KorgonOutpost = 0x004b86c8,
	.Metropolis = 0x004b7a18,
	.BlackwaterCity = 0x004b5230,
	.CommandCenter = 0x004b53e8,
	.BlackwaterDocks = 0x004b7c28,
	.AquatosSewers = 0x004b6f68,
	.MarcadiaPalace = 0x004b68a8,
#endif
};

VariableAddress_t vaUiMsgStringFunc = {
#ifdef UYA_PAL
	.Lobby = 0x0059a970,
	.Bakisi = 0x00467360,
	.Hoven = 0x00468f10,
	.OutpostX12 = 0x0045fd10,
	.KorgonOutpost = 0x0045d8a0,
	.Metropolis = 0x0045cbe0,
	.BlackwaterCity = 0x0045a410,
	.CommandCenter = 0x0045ad08,
	.BlackwaterDocks = 0x0045d588,
	.AquatosSewers = 0x0045c888,
	.MarcadiaPalace = 0x0045c208,
#else
	.Lobby = 0x00599618,
	.Bakisi = 0x00466108,
	.Hoven = 0x00467bf8,
	.OutpostX12 = 0x0045ea38,
	.KorgonOutpost = 0x0045c648,
	.Metropolis = 0x0045B988, 
	.BlackwaterCity = 0x00459138,
	.CommandCenter = 0x00459bf0,
	.BlackwaterDocks = 0x0045c430,
	.AquatosSewers = 0x0045b770,
	.MarcadiaPalace = 0x0045b0b0,
#endif
};

VariableAddress_t vaUIShowPopup = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00475ab0,
	.Hoven = 0x00477660,
	.OutpostX12 = 0x0046e460,
    .KorgonOutpost = 0x0046bff0,
	.Metropolis = 0x0046b330,
	.BlackwaterCity = 0x00468b60,
	.CommandCenter = 0x00469420,
    .BlackwaterDocks = 0x0046bca0,
    .AquatosSewers = 0x0046afa0,
    .MarcadiaPalace = 0x0046a920,
#else
	.Lobby = 0,
	.Bakisi = 0x00473d98,
	.Hoven = 0x00475888,
	.OutpostX12 = 0x0046c6c8,
    .KorgonOutpost = 0x0046a2d8,
	.Metropolis = 0x00469618,
	.BlackwaterCity = 0x00466dc8,
	.CommandCenter = 0x00467848,
    .BlackwaterDocks = 0x0046a088,
    .AquatosSewers = 0x004693c8,
    .MarcadiaPalace = 0x00468d08,
#endif
};

UiMenu_t* uiGetMenu(int UI)
{
	return *(u32*)(UI_POINTERS + (UI * 4));
}

UiMenu_t* uiGetActiveMenu(int UI, int whichMenu)
{
	u32 pointer = *(u32*)((u32)UI_ACTIVE_MENU_1 + (whichMenu * 4));
	if (pointer == uiGetMenu(UI))
		return pointer;
    
	return 0;
}

int uiShowYesNoDialog(const char * title, const char * description)
{
    return internal_uiDialog(UI_DIALOG_A0, title, description, 3, 0, 0.6);
}

int uiShowOkDialog(const char * title, const char * description)
{
    return internal_uiDialog(UI_DIALOG_A0, title, description, 4, 0, 0.6);
}

int uiShowSelectDialog_Simple(const char * option1, const char * option2)
{
    return internal_uiSelectDialog_Simple(UI_DIALOG_A0, option1, option2, 0);
}

int uiShowInputDialog(const char * title, char * value, int maxLength)
{
    return internal_uiInputDialog(UI_DIALOG_A0, title, value, 0, maxLength, 0, 0, 0, 0);
}

void uiShowPopup(Player * player, const char * message, int seconds)
{
	internal_uiShowPopup(message, seconds * 100, player->isLocal2);
}

int uiShowTipPopup(const char *message, int seconds)
{
	u32 initialize;
	u32 render;
	u32 state;
	u32 messages;

	if (!isInGame() || !message || seconds < 1 || seconds > 3600)
		return 0;

	/* Help initialization and drawing follow uiMsgString in the retail help
	 * implementation. Validate their instructions before decoding globals;
	 * an overlay with a different layout is unsupported, never guessed. */
	initialize = GetAddress(&vaUiMsgStringFunc);
	if (!initialize)
		return 0;
	initialize += 0xc8;
	render = initialize + 0x9b0;
	if (*(u32 *)initialize != 0x27bdffa0 ||
		(*(u32 *)(initialize + 0x0c) & 0xffff0000) != 0x3c040000 ||
		(*(u32 *)(initialize + 0x2c) & 0xffff0000) != 0x24900000 ||
		*(u32 *)(initialize + 0x58) != 0x8e020020 ||
		(*(u32 *)(initialize + 0x60) & 0xffff0000) != 0x3c040000 ||
		(*(u32 *)(initialize + 0x64) & 0xffff0000) != 0x8c840000 ||
		*(u32 *)render != 0x27bdff70 ||
		(*(u32 *)(render + 0x04) & 0xffff0000) != 0x3c030000 ||
		(*(u32 *)(render + 0x18) & 0xffff0000) != 0x24630000 ||
		(*(u32 *)(render + 0x280) & 0xffff0000) != 0x3c040000 ||
		(*(u32 *)(render + 0x284) & 0xffff0000) != 0x8c840000)
		return 0;

	state = (*(u32 *)(initialize + 0x0c) << 16) + *(s16 *)(initialize + 0x2c);
	messages = (*(u32 *)(initialize + 0x60) << 16) + *(s16 *)(initialize + 0x64);
	if (state != ((*(u32 *)(render + 0x04) << 16) + *(s16 *)(render + 0x18)) ||
		messages != ((*(u32 *)(render + 0x280) << 16) + *(s16 *)(render + 0x284)) ||
		state < 0x00100000 || state > 0x01ffffb4 ||
		messages < 0x00100000 || messages > 0x01fffffc || (state & 3) || (messages & 3))
		return 0;

	tipInitializeAddress = initialize;
	tipRenderAddress = render;
	tipNativeState = (u32 *)state;
	tipNativeMessages = (uiTipMessage_t **)messages;
	tipMessage.text = message;
	tipMessage.id = -1;
	tipMessage.speech = -1;
	tipMessage.alternate = -1;
	tipMessage.icon = 0;
	memset(tipState, 0, sizeof(tipState));
	tipFrames = seconds * GAME_FPS;
	tipElapsed = 0;
	return 1;
}

void uiUpdateTipPopup(void)
{
	u32 savedState[0x4c / 4];
	uiTipMessage_t *savedMessages;

	if (!tipFrames)
		return;
	if (!isInGame() || GetAddress(&vaUiMsgStringFunc) + 0xc8 != tipInitializeAddress) {
		tipFrames = 0;
		return;
	}
	/* Let native tutorial tips finish before drawing ours. All substitutions
	 * below are confined to these synchronous game-thread rendering calls. */
	if (tipNativeState[0] != 0)
		return;
	memcpy(savedState, tipNativeState, sizeof(savedState));
	savedMessages = *tipNativeMessages;
	memcpy(tipNativeState, tipState, sizeof(tipState));
	*tipNativeMessages = &tipMessage;
	if (!tipElapsed) {
		((void (*)(void))tipInitializeAddress)();
		tipNativeState[0x30 / 4] = 1;
	}
	if (tipElapsed < 8) {
		tipNativeState[0] = 3;
		tipNativeState[1] = tipElapsed;
	} else if (tipElapsed < 12) {
		tipNativeState[0] = 4;
		tipNativeState[1] = tipElapsed - 8;
	} else if (tipFrames <= 8) {
		tipNativeState[0] = 7;
		tipNativeState[1] = 8 - tipFrames;
	} else {
		tipNativeState[0] = 5;
		tipNativeState[1] = tipElapsed - 12;
	}
	((void (*)(void))tipRenderAddress)();
	memcpy(tipState, tipNativeState, sizeof(tipState));
	*tipNativeMessages = savedMessages;
	memcpy(tipNativeState, savedState, sizeof(savedState));
	++tipElapsed;
	--tipFrames;
}

int uiShowSelectDialog(const char * title, const char * items[], int itemCount, int selectedIndex)
{
    return internal_uiSelectDialog(UI_DIALOG_A0, title, items, itemCount, selectedIndex, 0, 0);
}

int uiShowChangeTeamSkinDialog(void * team, void * skin, int numTeams, int bChangeTeams, int bUnlockDan, int bUnlockNefarious)
{
	int controllerPort = 0; // Player 1 has opened menu
	internal_uiChangeTeamSkinDialog(UI_DIALOG_A0, &team, &skin, controllerPort, numTeams, bChangeTeams, bUnlockDan, bUnlockNefarious);
}

int uiShowInvitePlayersDialog(void)
{
	internal_uiInvitePlayersDialog(UI_DIALOG_A0, 0x002496a0, 3, 0);
}

void uiRefresh(void)
{
	if (GAME_NET_INFO->m_GuiSetRefreshCallback != 0)
		((void (*)(void))GAME_NET_INFO->m_GuiSetRefreshCallback)();
}

void uiSetBusy(void)
{
	if (GAME_NET_INFO->m_GuiSetBusyCallback != 0)
		((void (*)(void))GAME_NET_INFO->m_GuiSetBusyCallback)();
}

/*
 * NAME : uiProgressDialog
 * DESCRIPTION : Draw a centered, wrapping progress popup in the patch menu's colors.
 * NOTES : Call each frame; input and operation state remain with the caller.
 * ARGS : text, completed units, total units (zero or negative when unknown).
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
void uiProgressDialog(const char* text, int completed, int total)
{
	if (!text)
		return;
	float portion = total > 0 ? (float)completed / total : 0.25f;
	if (portion < 0)
		portion = 0;
	if (portion > 1)
		portion = 1;
	float barX = 0.3f;
	if (total <= 0)
		barX += (gameGetTime() % 1000) * 0.0003f;

	gfxScreenSpaceBox(0, 0, 1, 1, 0x50000000);
	gfxScreenSpaceBox(0.24f, 0.36f, 0.52f, 0.25f, 0x8018608f);
	gfxScreenSpaceBox(0.25f, 0.37f, 0.5f, 0.23f, 0x80123251);
	struct FontWindow window = {
		.windowLeft = SCREEN_WIDTH * 0.28f,
		.windowRight = SCREEN_WIDTH * 0.72f,
		.windowTop = SCREEN_HEIGHT * 0.385f,
		.windowBottom = SCREEN_HEIGHT * 0.49f,
		.textX = SCREEN_WIDTH * 0.5f,
		.textY = SCREEN_HEIGHT * 0.4375f,
		.lineSpacing = 16,
		.flags = FONT_WINDOW_FLAGS_H_ALIGN_CENTER | FONT_WINDOW_FLAGS_V_ALIGN_CENTER,
		.shadowOffsetX = 1,
		.shadowOffsetY = 1
	};
	gfxScreenSpaceTextWindow(&window, 1, 1, 0x8069cbf2, text, -1, 0x80000000);
	gfxScreenSpaceBox(0.3f, 0.52f, 0.4f, 0.025f, 0x8004223f);
	gfxScreenSpaceBox(barX, 0.52f, 0.4f * portion, 0.025f, 0x803880af);
}

/*
 * NAME : uiPlaySound
 * DESCRIPTION : Play a native menu sound.
 * NOTES :
 * ARGS : soundId, native sound flags.
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
void uiPlaySound(int soundId, int flags)
{
	internal_uiPlaySound(soundId, flags, NULL);
}
