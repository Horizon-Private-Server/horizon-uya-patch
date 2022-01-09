#include "ui.h"

#define UI_ACTIVE_ID                            (*(int*)0x003434B8)
#define UI_DIALOG_A0                            ((void*)0x01C5C000)

int internal_uiDialog(void *, const char *, const char *, int, int);

int GetActiveUIPointer(int UI)
{
	int UI_POINTERS = 0x01C5C064;
	int Pointer = *(int*)(UI_POINTERS + (UI * 0x4));
	int ActiveUIPointer_1 = *(int*)0x01C5C110;
    int ActiveUIPointer_2 = *(int*)0x01C5C114;
	if (ActiveUIPointer_2 == Pointer)
	{
		return Pointer;
    }
    else if (ActiveUIPointer_1 == Pointer)
    {
        return Pointer;
    }
	else
	{
		return 0;
	}
}

int uiGetActive(void)
{
    return UI_ACTIVE_ID;
}

int uiShowYesNoDialog(const char * title, const char * description)
{
    asm __volatile__ (
        "lui	$at, 0x3F19;"
	    "ori	$at, $at, 0x999A;"
	    "mtc1	$at, $f12;"
    );
    return internal_uiDialog(UI_DIALOG_A0, title, description, 3, 0);
}

int uiShowOkDialog(const char * title, const char * description)
{
    asm __volatile__ (
        "lui	$at, 0x3F19;"
	    "ori	$at, $at, 0x999A;"
	    "mtc1	$at, $f12;"
    );
    return internal_uiDialog(UI_DIALOG_A0, title, description, 4, 0);
}
