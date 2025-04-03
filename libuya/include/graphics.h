/***************************************************
 * FILENAME :		graphics.h
 * DESCRIPTION :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GRAPHICS_H_
#define _LIBUYA_GRAPHICS_H_

#include <tamtypes.h>
#include "math3d.h"
#include "moby.h"


//--------------------------------------------------------
#if UYA_PAL
#define SCREEN_WIDTH           (512)
#define SCREEN_HEIGHT          (448)
#else
#define SCREEN_WIDTH           (512)
#define SCREEN_HEIGHT          (416)
#endif

enum Sprite {
	SPRITE_R = 0xed1b,
	SPRITE_NUMBER_0 = 0xed1c,
	SPRITE_NUMBER_1 = 0xed1d,
	SPRITE_NUMBER_2 = 0xed1e,
	SPRITE_NUMBER_3 = 0xed1f,
	SPRITE_NUMBER_4 = 0xed20,
	SPRITE_NUMBER_5 = 0xed21,
	SPRITE_NUMBER_6 = 0xed22,
	SPRITE_NUMBER_7 = 0xed23,
	SPRITE_NUMBER_8 = 0xed24,
	SPRITE_NUMBER_9 = 0xed25,
	SPRITE_BOLT = 0xed26,
	SPRITE_CIRCLE = 0xed27,
	SPRITE_SQUARE = 0xed28,
	SPRITE_TRIANGLE = 0xed29,
	SPRITE_CROSS = 0xed2a,
	SPRITE_ARROW_SWITCH_TEAM = 0xed2b,
	SPRITE_WEAPON_WHOLLOPER = 0xed2c,
	SPRITE_WEAPON_MINE_GLOVE = 0xed2d,
	SPRITE_WEAPON_N60_STORM = 0xed2e,
	SPRITE_WEAPON_MINI_ROCKET_TUBE = 0xed2f,
	SPRITE_WEAPON_HOLOSHIELD_GLOVE = 0xed30,
	SPRITE_WEAPON_BLITZ_CANNON = 0xed31,
	SPRITE_WEAPON_FLUX_RIFLE_MP = 0xed32,
	SPRITE_BASE = 0xed33,
	SPRITE_NODE = 0xed34,
	SPRITE_PLAYER_ARROW = 0xed35,
	SPRITE_TURBOSLIDER = 0xed36,
	SPRITE_HOVERSHIP = 0xed37,
	SPRITE_UNK_1 = 0xed38,
	SPRITE_CAMERA = 0xed39,
	SPRITE_DRONE = 0xed3a,
	SPRITE_SKULL = 0xed3b,
	SPRITE_PLAYER = 0xed3c,
	SPRITE_DOT = 0xed3d,
	SPRITE_SHIELD = 0xed3e,
	SPRITE_HEADPHONES = 0xed3f,
	SPRITE_ARROW = 0xed40,
	SPRITE_AMMO = 0xed41,
	SPRITE_PLAYER_TURRET = 0xed42,
	SPRITE_TYHRRANOID = 0xed43,
	SPRITE_WAVEFORM = 0xed44,
	SPRITE_X = 0xed45,
	SPRITE_KILLER_FISH = 0xed46,
	SPRITE_WEAPON_CLANK_ZAPPER = 0xed47,
	SPRITE_UNK_2 = 0xed48,
	SPRITE_FIRST_PERSON_MODE = 0xed49,
	SPRITE_WARP_PAD = 0xed4a,
	SPRITE_PLAYER_TURRET_2 = 0xed4b,
	SPRITE_THIRD_PERSON_MODE = 0xed4c,
	SPRITE_WEAPON_OMNISHIELD = 0xed4d,
	SPRITE_LOCKSTRAFE_MODE = 0xed4e,
	SPRITE_WEAPON_GRAVITY_BOMB = 0xed4f,
	SPRITE_QWARK = 0xed50,
	SPRITE_R1 = 0xed51,
	SPRITE_UNK_3 = 0xed52,
	SPRITE_UNK_4 = 0xed53,
	SPRITE_UNK_5 = 0xed54,
	SPRITE_UNK_6 = 0xed55,
	SPRITE_R2 = 0xed56,
	SPRITE_UNK_7 = 0xed57,
	SPRITE_VIDCOMIC_1 = 0xed58,
	SPRITE_UNK_8 = 0xed59,
	SPRITE_UNK_9 = 0xed5a,
	SPRITE_FLAG = 0xed5b,
	SPRITE_LOADING = 0xed5c,
};

enum TextAlign {
  TEXT_ALIGN_TOPLEFT,
  TEXT_ALIGN_TOPCENTER,
  TEXT_ALIGN_TOPRIGHT,
  TEXT_ALIGN_MIDDLELEFT,
  TEXT_ALIGN_MIDDLECENTER,
  TEXT_ALIGN_MIDDLERIGHT,
  TEXT_ALIGN_BOTTOMLEFT,
  TEXT_ALIGN_BOTTOMCENTER,
  TEXT_ALIGN_BOTTOMRIGHT,
};

typedef float POINT[2] __attribute__((__aligned__(8)));

typedef struct RECT
{
    POINT TopLeft;
    POINT TopRight;
    POINT BottomLeft;
    POINT BottomRight;
} RECT;

typedef struct CubicLineEndPoint {
	/*   0 */ int iCoreRGBA;
	/*   4 */ int iGlowRGBA;
	/*   8 */ char bFadeEnd;
	/*   9 */ char style;
	/*   a */ unsigned char numEndPoints;
	/*   b */ char bDisabled;
	/*   c */ int iNumSkipPoints;
	/*  10 */ VECTOR vPos;
	/*  20 */ VECTOR vTangent;
	/*  30 */ VECTOR vTangentOccQuat;
} CubicLineEndPoint;

struct UV {
	float x; // U: horizontal position
	float y; // V: vertical position
};

typedef struct QuadDef { // 0x90
/* 0x00 */ vec4 xzyw[4];
/* 0x40 */ u32 rgba[4];
/* 0x50 */ struct UV uv[4];
/* 0x70 */ u64 clamp;
/* 0x78 */ u64 tex0;
/* 0x80 */ u64 tex1;
/* 0x88 */ u64 alpha;
} QuadDef;

typedef struct ScreenVBEffect { // 0x34
	/* 0x00 */ int enabled;
	/* 0x04 */ u32 BKG_color;
	/* 0x08 */ u64 BKG_alpha;
	/* 0x10 */ int unk_10;
	/* 0x14 */ int A_lines;
	/* 0x18 */ u32 A_color;
	/* 0x1c */ u64 A_alpha;
	/* 0x24 */ int B_lines;
	/* 0x28 */ u32 B_color;
	/* 0x2c */ u64 B_alpha;
} ScreenVBEffect;

typedef struct ScreenInsertEffect { // 0x10
	/* 0x0 */ int enabled;
	/* 0x4 */ u32 color;
	/* 0x8 */ u64 alpha;
} ScreenInsertEffect;

typedef struct ViewContext { // 0x270
	/* 0x000 */ int headerBase[6][4];
	/* 0x060 */ u64 headerTex;
	/* 0x070 */ u64 headerTexNoFog;
	/* 0x080 */ u64 headerGr;
	/* 0x090 */ u64 headerGrNoFog;
	/* 0x0a0 */ float D;
	/* 0x0a4 */ float F;
	/* 0x0a8 */ float xclipratio;
	/* 0x0ac */ float yclipratio;
	/* 0x0b0 */ float xratio;
	/* 0x0b4 */ float yratio;
	/* 0x0b8 */ float xradpad;
	/* 0x0bc */ float yradpad;
	/* 0x0c0 */ MATRIX fMtx;
	/* 0x100 */ MATRIX nfMtx;
	/* 0x140 */ MATRIX hMtx;
	/* 0x180 */ VECTOR hmgScale;
	/* 0x190 */ VECTOR ihmgScale;
	/* 0x1a0 */ VECTOR hvdf;
	/* 0x1b0 */ VECTOR unSquish;
	/* 0x1c0 */ VECTOR guard;
	/* 0x1d0 */ VECTOR iguard;
	/* 0x1e0 */ VECTOR sphereCheckA;
	/* 0x1f0 */ VECTOR sphereCheckB;
	/* 0x200 */ VECTOR sphereCheckC;
	/* 0x210 */ float xpix;
	/* 0x214 */ float ypix;
	/* 0x218 */ float xclip;
	/* 0x21c */ float yclip;
	/* 0x220 */ float fog0;
	/* 0x224 */ float fog1;
	/* 0x228 */ float fogNearDist;
	/* 0x22c */ float fogFarDist;
	/* 0x230 */ float fogMult;
	/* 0x234 */ float fogAdd;
	/* 0x238 */ float fogNearIntensity;
	/* 0x23c */ float fogFarIntensity;
	/* 0x240 */ int scis_l;
	/* 0x244 */ int scis_r;
	/* 0x248 */ int scis_t;
	/* 0x24c */ int scis_b;
	/* 0x250 */ int fogR;
	/* 0x254 */ int fogG;
	/* 0x258 */ int fogB;
	/* 0x25c */ int backR;
	/* 0x260 */ int backG;
	/* 0x264 */ int backB;
	/* 0x268 */ int splitScreenMode;
} ViewContext;

struct DataSource { // 0x8
	/* 0x0 */ int m_state;
	/* 0x4 */ void *__vtable;
};

typedef struct ConcretePreLoadedImageBuffer {
	/* 0x00 */ struct DataSource DataSourceImageBuffer;
	/* 0x08 */ unsigned int tex0;
	/* 0x0c */ void *m_memory_buffer;
	/* 0x10 */ void *m_decompressed_buffer;
} ConcretePreLoadedImageBuffer;

typedef struct PartInstance { // 0x40
	/* 0x00 */ char i_class;
	/* 0x01 */ char type;
	/* 0x02 */ char tex;
	/* 0x03 */ char gs_alpha;
	/* 0x04 */ int rgba;
	/* 0x08 */ char rot;
	/* 0x09 */ char draw_dist;
	/* 0x0a */ short int timer;
	/* 0x0c */ float scale;
	/* 0x10 */ VECTOR pos;
	/* 0x20 */ int update[8];
} PartInstance_t;

typedef struct ColorExtTable {
/* 0x00 */ u32 normal; // aka default
/* 0x04 */ u32 blue;
/* 0x08 */ u32 green;
/* 0x0c */  u32 pink;
/* 0x10 */  u32 white;
/* 0x14 */  u32 gray;
/* 0x18 */  u32 black1;
/* 0x1c */  u32 black2;
} ColorExtTable_t;

/*
 * NAME :		drawFunction
 * DESCRIPTION :
 * 			Calls the normal draw function.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void drawFunction(void);

/*
 * NAME :		gfxScreenSpaceText
 * DESCRIPTION :
 * 			Draws the text on the screen.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gfxScreenSpaceText(float x, float y, float scaleX, float scaleY, u32 color, const char * string, int length, int alignment);
int gfxScreenSpaceTextCenter(float x, float y, u32 color, const char * string, int length);

/*
 * NAME :		gfxScreenSpaceBox
 * DESCRIPTION :
 * 			Draws a quad on the screen.
 * NOTES :
 * ARGS : 
 *      rect:           All four corners of quad. Use values 0-1 for x and y position.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void gfxScreenSpaceQuad(RECT * rect, u32 colorTL, u32 colorTR, u32 colorBL, u32 colorBR);

/*
 * NAME :		gfxScreenSpaceBox
 * DESCRIPTION :
 * 			Draws a box on the screen.
 * NOTES :
 * ARGS : 
 *      x:              Screen X position (0-1).
 *      y:              Screen Y position (0-1).
 *      w:              width (0-1).
 *      h:              height (0-1).
 *      color:          color of box.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void gfxScreenSpaceBox(float x, float y, float w, float h, u32 color);

/*
 * NAME :		gfxWorldSpaceToScreenSpace
 * DESCRIPTION :
 * 			Converts world space to screen space pixel coordinates.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gfxWorldSpaceToScreenSpace(VECTOR position, int * x, int * y);

int gfxGetFontWidth(const char * string, int length, float scale);
void gfxPixelSpaceBox(float x, float y, float w, float h, u32 color);

int gfxGetIsProgressiveScan(void);
void gfxSetIsProgressiveScan(int on);

/*
 * NAME :		gfxOcclusion
 * DESCRIPTION :
 * 			Turn on/off Occlusion (Render All)
 * NOTES :
 * ARGS : 
 *          OnOff: 0 = Show All, 2 = Default
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void gfxOcclusion(int OnOff);

void gfxDoGifPaging(void);
void gfxSetupGifPaging(int);
u64 gfxGetFrameTex(int id);
u64 gfxGetEffectTex(int id);
void gfxDrawSprite(float x, float y, float w, float h, int t0, int t1, int texW, int texH, u64 color, u64 texture);
void gfxStickyFX(void* a0, Moby* moby);
void gfxDrawBillboardQuad(float scale, float scale2, float theta, VECTOR position, int tex, int color, int drawType);
void gfxDrawQuad(QuadDef *quadDef, MATRIX worldMatrix);
void gfxSetScissor(int xmin, int xmax, int ymin, int ymax);

//
ScreenVBEffect* gfxScreenVBEffect(void);
ScreenInsertEffect* gfxScreenInsertEffect(void);
ViewContext* gfxViewContext(void);
ConcretePreLoadedImageBuffer* gfxGetPreLoadedImageBufferSource(int which);
ColorExtTable_t* gfxColorExtTable(void);

#endif // _LIBUYA_GRAPHICS_H_
