#include "ui.h"
#include "interop.h"

#define UI_ACTIVE_ID                            (*(int*)0x003434B8)
#define UI_DIALOG_A0                            ((void*)0x01C5C000) // NTSC and PAL are the same

int internal_uiDialog(void *, const char *, const char *, int, int, float);

VariableAddress_t vaUiMsgStringFunc = {
	.Lobby = 0x00599618,
	.Bakisi = 0,
	.CommandCenter = 0,
	.DocksSewersMarcadia = 0,
	.Hoven = 0,
	.KorgonBlackwaterCity = 0,
	.Metropolis = 0,
	.OutpostX12 = 0
};

int GetActiveUIPointer(int UI)
{
	int UI_POINTERS = 0x01C5C064;
	int Pointer = *(int*)(UI_POINTERS + (UI * 0x4));
	if (*(int*)0x01C5C110 == Pointer)
	{
		return Pointer;
    }
    
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
