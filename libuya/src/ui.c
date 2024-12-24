#include "ui.h"
#include "player.h"
#include "time.h"
#include "interop.h"

#define UI_DIALOG_A0                            ((void*)0x01C5C000) // NTSC and PAL are the same
#define UI_POINTERS								((u32)0x01c5C064)
#define UI_ACTIVE_MENU_1						(0x01c5c110)
#define UI_ACTIVE_MENU_2						(0x01c5c114)
#define UI_ACTIVE_MENU_3						(0x01c5c118)


int internal_uiDialog(void *, const char *, const char *, int, int, float);
void internal_uiShowPopup(const char *, int, Player * player);
int internal_uiSelectDialog_Simple(void *, const char *, const char *, int);
int internal_uiSelectDialog(void *, const char *, const char **, int, int, int, int);
int internal_uiChangeTeamSkinDialog(void *, void *, void *, int, int, int, int, int);
int internal_uiInvitePlayersDialog(void *, void *, int, int);

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
	internal_uiShowPopup(message, seconds * 100, (!player ? 0 : player->unk_24c9));
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
