#include <tamtypes.h>
#include "utils.h"

//--------------------------------------------------------------------------------
int isInEEMemory(void* p)
{
    u32 upper = (u32)p >> 16;
    return upper >= 0x0008 && upper < 0x0200;
}

//--------------------------------------------------------------------------------
int isInScratchpadMemory(void* p)
{
    u32 upper = (u32)p >> 16;
    u32 lower = (u32)p & 0xFFFF;
    return upper == 0x7000 && lower < 0x4000;
}

//--------------------------------------------------------------------------------
int isInIopMemory(void* p)
{
    u32 upper = (u32)p >> 16;
    return upper >= 0x1C00 && upper < 0x1C20;
}

//--------------------------------------------------------------------------------
int ConvertJALtoAddress(int instruction)
{
	return ((instruction & 0x03FFFFFF) << 2);
}