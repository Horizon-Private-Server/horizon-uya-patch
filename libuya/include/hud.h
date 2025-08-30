#ifndef _LIBUYA_HUD_H_
#define _LIBUYA_HUD_H_

#include <tamtypes.h>
#include "math3d.h"

typedef struct HudRadar_PlayerData {
/* 0x000 */ u32 heroBlipColor;
/* 0x004 */ int pad_004;
/* 0x008 */ u32 enemyBlipColor;
/* 0x00c */ int pad_00c;
/* 0x010 */ u32 defaultNodeColor;
/* 0x014 */ char unk_014[0xc]; 
/* 0x020 */ u32 defaultPlayerCursorColor;
/* 0x030 */ vec2 mapCenter;
/* 0x040 */ vec2 screenCenter;
} HudRadar_PlayerData_t;

// returns pointer to radar map settings.
HudRadar_PlayerData_t *hudGetMapSettings(void);

#endif // _LIBUYA_HUD_H_
