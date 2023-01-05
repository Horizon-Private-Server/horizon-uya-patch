#include "ui.h"
#include "interop.h"

#define UI_ACTIVE_ID                            (*(int*)0x003434B8)
#define UI_DIALOG_A0                            ((void*)0x01C5C000) // NTSC and PAL are the same

int internal_uiDialog(void *, const char *, const char *, int, int, float);

VariableAddress_t vaUiMsgStringFunc = {
	.Lobby = 0x00599618,
	.Bakisi = 0,
	.Hoven = 0,
	.OutpostX12 = 0,
  .KorgonOutpost = 0,
	.Metropolis = 0,
	.BlackwaterCity = 0,
	.CommandCenter = 0,
  .BlackwaterDocks = 0,
  .AquatosSewers = 0,
  .MarcadiaPalace = 0,
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
