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

//--------------------------------------------------------
// typedef void (*gfxDrawFuncDef)(Moby*);

//--------------------------------------------------------
typedef float POINT[2] __attribute__((__aligned__(8)));

//--------------------------------------------------------
typedef struct RECT
{
    POINT TopLeft;
    POINT TopRight;
    POINT BottomLeft;
    POINT BottomRight;
} RECT;

//--------------------------------------------------------
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

//--------------------------------------------------------
struct UV
{
	float U;
	float V;
};

//--------------------------------------------------------
struct QuadDef
{
	VECTOR VertexPositions[4];
	u32 VertexColors[4];
	struct UV VertexUVs[4];
	u64 Clamp;
	u64 Tex0;
	u64 Tex1;
	u64 Alpha;
};

//--------------------------------------------------------
enum TextAlign
{
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
u64 gfxGetEffectTex(int id, int);
void gfxDrawSprite(float x, float y, float w, float h, int t0, int t1, int texW, int texH, u64 color, u64 texture);
void gfxStickyFX(void* a0, Moby* moby);
void gfxDrawBillboardQuad(float scale, float scale2, float theta, VECTOR position, int tex, int color, int drawType);
void gfxDrawQuad(struct QuadDef * quadDef, MATRIX worldMatrix, u32 a3);
void gfxSetScissor(int xmin, int xmax, int ymin, int ymax);

//
ScreenVBEffect* gfxScreenVBEffect(void);
ScreenInsertEffect* gfxScreenInsertEffect(void);
ViewContext* gfxViewContext(void);
ConcretePreLoadedImageBuffer* gfxGetPreLoadedImageBufferSource(int which);
ColorExtTable_t* gfxColorExtTable(void);

#endif // _LIBUYA_GRAPHICS_H_
