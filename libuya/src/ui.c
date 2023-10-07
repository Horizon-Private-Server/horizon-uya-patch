#include "ui.h"
#include "player.h"
#include "time.h"
#include "interop.h"

#define UI_ACTIVE_ID                            (*(int*)0x003434B8)
#define UI_DIALOG_A0                            ((void*)0x01C5C000) // NTSC and PAL are the same

int internal_uiDialog(void *, const char *, const char *, int, int, float);
void internal_uiShowPopup(const char *, int, Player * player);

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

u32 uiGetPointer(int UI)
{
	return *(u32*)(0x01C5C064 + UI*4);
}

u32 uiGetActivePointer(int UI)
{
	u32 UI_POINTERS = 0x01C5C064;
	u32 Pointer = *(u32*)(UI_POINTERS + (UI * 0x4));
	if (*(u32*)0x01C5C110 == Pointer)
		return Pointer;
    
	return 0;
}

u32 uiGetActiveSubPointer(int UI)
{
	u32 UI_POINTERS = 0x01C5C064;
	u32 Pointer = *(u32*)(UI_POINTERS + (UI * 0x4));
	if (*(u32*)0x01C5C114 == Pointer)
		return Pointer;
    
	return 0;
}

int uiGetActive(void)
{
    return UI_ACTIVE_ID;
}

int uiShowYesNoDialog(const char * title, const char * description)
{
    return internal_uiDialog(UI_DIALOG_A0, title, description, 3, 0, 0.6);
}

int uiShowOkDialog(const char * title, const char * description)
{
    return internal_uiDialog(UI_DIALOG_A0, title, description, 4, 0, 0.6);
}

void uiShowPopup(Player * player, const char * message, int seconds)
{
	internal_uiShowPopup(message, seconds * 100, (!player ? 0 : player->unk_24c9));
}
