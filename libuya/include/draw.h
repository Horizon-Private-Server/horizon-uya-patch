#ifndef _LIBUYA_DRAW_H_
#define _LIBUYA_DRAW_H_

#include <tamtypes.h>
#include "common.h"
#include "math3d.h"

typedef enum LineStyle {
    LINE_STYLE_NONE = 0,
    LINE_STYLE_BEAM = 1,
    LINE_STYLE_SQUIGLY = 2,
    LINE_STYLE_LIGHTNING = 3,
    LINE_STYLE_NUM_STYLES = 4
} LineStyle_e;

typedef struct LineStatic {
    /* 0x00 */ int numSegments;
    /* 0x04 */ int texture;
    /* 0x08 */ float textureOffset;
    /* 0x0c */ float textureRepeatDistance;
    /* 0x10 */ bool billboard;
    /* 0x11 */ bool rotateTexture;
    /* 0x12 */ char pad[2];
    /* 0x14 */ float lineWidth;
    /* 0x18 */ float widthStart;
    /* 0x1c */ float widthEnd;
    /* 0x20 */ int scrollFrames;
    /* 0x24 */ float pointRandomOffset;
} LineStatic_t;

typedef struct LineEndPoint {
    /* 0x00 */ VECTOR pos;
    /* 0x10 */ int color;
    /* 0x14 */ bool lerpColor;
    /* 0x15 */ char pad[2];
    /* 0x17 */ LineStyle_e style;  // Fixed: was LineStyles_e
    /* 0x18 */ float fadeEnds;
} LineEndPoint_t;

extern LineStatic_t BeamLineStyle;
extern LineStatic_t SquiglyLineStyle;
extern LineStatic_t LightningLineStyle;
extern LineStatic_t* LineStyles[LINE_STYLE_NUM_STYLES];

void drawLines(LineEndPoint_t *pEndPoints, int numEndPoints, LineStatic_t *pStyle);
void drawLine(VECTOR point_a, VECTOR point_b, u32 color, LineStyle_e style);

#endif