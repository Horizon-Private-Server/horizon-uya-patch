#ifndef _LIBUYA_DRAW_H_
#define _LIBUYA_DRAW_H_

#include <tamtypes.h>

extern LineStatic_t BeamLineStyle;
extern LineStatic_t SquiglyLineStyle;
extern LineStatic_t LightningLineStyle;
extern LineStatic_t* LineStyles[LINE_STYLE_NUM_STYLES];

void drawLines(LineEndPoint_t *pEndPoints, int numEndPoints, LineStatic_t *pStyle);
void drawLine(VECTOR point_a, VECTOR point_b, u32 color, LineStyle_e style);

#endif
