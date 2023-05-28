#include "ui.h"
#include "interop.h"

#define UI_ACTIVE_ID                            (*(int*)0x003434B8)
#define UI_DIALOG_A0                            ((void*)0x01C5C000) // NTSC and PAL are the same

int internal_uiDialog(void *, const char *, const char *, int, int, float);

#ifdef UYA_PAL
VariableAddress_t vaUiMsgStringFunc = {
	.Lobby = 0x0059a970,
	.Bakisi = 0x00467388,
	.Hoven = 0x00468f10,
	.OutpostX12 = 0x0045fd10,
	.KorgonOutpost = 0x0045d8a0,
	.Metropolis = 0x0045cbe0,
	.BlackwaterCity = 0x0045a410,
	.CommandCenter = 0x0045ad08,
	.BlackwaterDocks = 0x0045a410,
	.AquatosSewers = 0x0045c888,
	.MarcadiaPalace = 0x0045c208,
};
#else
VariableAddress_t vaUiMsgStringFunc = {
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
};
#endif

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
