/***************************************************
 * FILENAME :		graphics.h
 * 
 * DESCRIPTION :
 * 		
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GRAPHICS_H_
#define _LIBUYA_GRAPHICS_H_

#include <tamtypes.h>
#include "math3d.h"


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
typedef enum FontAlignment {
	TOP_LEFT = 0,
	TOP_CENTER = 1,
	TOP_RIGHT = 2,
	CENTER_LEFT = 3,
	CENTER_CENTER = 4,
	CENTER_RIGHT = 5,
	BOTTOM_LEFT = 6,
	BOTTOM_CENTER = 7,
	BOTTOM_RIGHT = 8
} FontAlignment;

/*
 * NAME :		drawFunction
 * 
 * DESCRIPTION :
 * 			Calls the normal draw function.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void drawFunction(void);

/*
 * NAME :		gfxScreenSpaceText
 * 
 * DESCRIPTION :
 * 			Draws the text on the screen.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gfxScreenSpaceText(float x, float y, float scaleX, float scaleY, u32 color, const char * string, int length, int alignment);
int gfxScreenSpaceTextCenter(float x, float y, u32 color, const char * string, int length);

/*
 * NAME :		gfxScreenSpaceBox
 * 
 * DESCRIPTION :
 * 			Draws a quad on the screen.
 * 
 * NOTES :
 * 
 * ARGS : 
 *      rect:           All four corners of quad. Use values 0-1 for x and y position.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void gfxScreenSpaceQuad(RECT * rect, u32 colorTL, u32 colorTR, u32 colorBL, u32 colorBR);

/*
 * NAME :		gfxScreenSpaceBox
 * 
 * DESCRIPTION :
 * 			Draws a box on the screen.
 * 
 * NOTES :
 * 
 * ARGS : 
 *      x:              Screen X position (0-1).
 *      y:              Screen Y position (0-1).
 *      w:              width (0-1).
 *      h:              height (0-1).
 *      color:          color of box.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void gfxScreenSpaceBox(float x, float y, float w, float h, u32 color);

/*
 * NAME :		gfxWorldSpaceToScreenSpace
 * 
 * DESCRIPTION :
 * 			Converts world space to screen space pixel coordinates.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gfxWorldSpaceToScreenSpace(VECTOR position, int * x, int * y);

int gfxGetFontWidth(const char * string, int length, float scale);
void gfxPixelSpaceBox(float x, float y, float w, float h, u32 color);

int gfxGetIsProgressiveScan(void);
void gfxSetIsProgressiveScan(int on);

/*
 * NAME :		gfxOcclusion
 * 
 * DESCRIPTION :
 * 			Turn on/off Occlusion (Render All)
 * 
 * NOTES :
 * 
 * ARGS : 
 *          OnOff: 0 = Show All, 2 = Default
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void gfxOcclusion(int OnOff);

void gfxDoGifPaging(void);
void gfxSetupGifPaging(int);
u64 gfxGetFrameTex(int id);
u64 gfxGetEffectTex(int id, int);
void gfxDrawSprite(float x, float y, float w, float h, int t0, int t1, int texW, int texH, u64 color, u64 texture);

#endif // _LIBUYA_GRAPHICS_H_
