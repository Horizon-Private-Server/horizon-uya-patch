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
#define SCREEN_WIDTH           (512)
#define SCREEN_HEIGHT          (416)

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
 * AUTHOR :			Troy "Agent Moose" Pruitt
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

void gfxScreenSpacePIF(RECT * rect);

int gfxGetFontWidth(const char * string, int length, float scale);

#endif // _LIBUYA_GRAPHICS_H_
