#include "draw.h"


LineStatic_t BeamLineStyle = {
    .numSegments = 2,
    .texture = FX_LINE_SKINNY,
    .textureRepeatDistance = 0,
    .lineWidth = 0.9f,
    .billboard = 1,
    .rotateTexture = 1,
    .scrollFrames = 60,
    .textureOffset = 0,
    .pointRandomOffset = 0,
    .widthStart = 1.0f,
    .widthEnd = 1.0f,
};

LineStatic_t SquiglyLineStyle = {
    .numSegments = 8,
    .texture = FX_BASELIGHT,
    .textureRepeatDistance = 0,
    .lineWidth = 2,
    .billboard = 1,
    .rotateTexture = 1,
    .scrollFrames = 1800,
    .textureOffset = 0,
    .pointRandomOffset = 2,
    .widthStart = 1,
    .widthEnd = 3,
};

LineStatic_t LightningLineStyle = {
    .numSegments = 10,
    .texture = FX_BASELIGHT,
    .textureRepeatDistance = 0,
    .lineWidth = 0.8f,
    .billboard = 1,
    .rotateTexture = 1,
    .scrollFrames = 30,
    .textureOffset = 0,
    .pointRandomOffset = 3.0f,
    .widthStart = 0.5f,
    .widthEnd = 0.1f,
};

LineStatic_t* LineStyles[LINE_STYLE_NUM_STYLES] = {
    NULL,
    &BeamLineStyle,
    &SquiglyLineStyle,
    &LightningLineStyle
};

void drawLines(LineEndPoint_t *pEndPoints, int numEndPoints, LineStatic_t *pStyle)
{
    if (numEndPoints < 2) return;
    if (!pStyle) pStyle = &BeamLineStyle;

    // Initialize drawing state
    gfxDrawStripInit();
    gfxAddRegister(8, 0);
    gfxAddRegister(0x14, 0xff9000000260);
    gfxAddRegister(6, gfxGetEffectTex(pStyle->texture));
    gfxAddRegister(0x47, 0x513f1);
    gfxAddRegister(0x42, 0x8000000044);

    // Calculate animated texture offset
    float animatedUVOffset = pStyle->textureOffset;
    
    if (pStyle->scrollFrames != 0) {
        float scrollDirection = (pStyle->scrollFrames < 0) ? 1.0f : -1.0f;
        int absFrames = (pStyle->scrollFrames < 0) ? -pStyle->scrollFrames : pStyle->scrollFrames;
        
        animatedUVOffset += scrollDirection * ((float)(gameGetTime() % absFrames) / (float)absFrames);
    }

    // Count total segments needed
    int totalSegments = 0;
    int i;
    for (i = 0; i < numEndPoints - 1; i++) {
        VECTOR delta;
        vector_subtract(delta, pEndPoints[i + 1].pos, pEndPoints[i].pos);
        float len = vector_length(delta);

        int segs = pStyle->numSegments;
        if (segs <= 0) {
            segs = (int)(len / pStyle->textureRepeatDistance);
            if (segs < 1) segs = 1;
        }
        totalSegments += segs;
    }

    int numPoints = (totalSegments + 1) * 2;
    vec3 stripPos[numPoints];
    int stripColor[numPoints];
    UV_t stripUV[numPoints];

    float distanceAccum = 0.0f;
    int vertexIndex = 0;

    // Build geometry for each line segment
    for (i = 0; i < numEndPoints - 1; i++) {
        // Check if endpoint has a custom style override
        LineStatic_t *segmentStyle = pStyle;
        if (pEndPoints[i].style > LINE_STYLE_NONE && pEndPoints[i].style < LINE_STYLE_NUM_STYLES) {
            segmentStyle = LineStyles[pEndPoints[i].style];
        }

        VECTOR a, b, delta, forward;
        vector_copy(a, pEndPoints[i].pos);
        vector_copy(b, pEndPoints[i + 1].pos);
        vector_subtract(delta, b, a);
        
        float segmentLength = vector_length(delta);
        vector_copy(forward, delta);
        vector_normalize(forward, forward);

        int numSegs = segmentStyle->numSegments;  // Fixed: Use segmentStyle
        if (numSegs <= 0) {
            numSegs = (int)(segmentLength / segmentStyle->textureRepeatDistance);  // Fixed: Use segmentStyle
            if (numSegs < 1) numSegs = 1;
        }

        // Generate vertices along this segment
        int s;
        for (s = 0; s <= numSegs; s++) {
            // Skip duplicate vertex at segment joins
            if (i > 0 && s == 0) continue;

            float t = (float)s / (float)numSegs;
            
            // Calculate position along segment
            VECTOR pos;
            pos[0] = a[0] + delta[0] * t;
            pos[1] = a[1] + delta[1] * t;
            pos[2] = a[2] + delta[2] * t;

            // Calculate perpendicular direction (right vector)
            VECTOR right;

            // Use miter joint at corner vertices
            if (i > 0 && s == 0) {
                // Get directions of previous and current segments
                VECTOR prevDir, currDir;
                vector_subtract(prevDir, pEndPoints[i].pos, pEndPoints[i - 1].pos);
                vector_subtract(currDir, pEndPoints[i + 1].pos, pEndPoints[i].pos);
                vector_normalize(prevDir, prevDir);
                vector_normalize(currDir, currDir);

                // Calculate right vectors for both segments
                VECTOR prevRight, currRight;
                
                if (segmentStyle->billboard) {  // Fixed: Use segmentStyle
                    MATRIX *cameraMtx = &playerGetFromSlot(0)->camera->uMtx;
                    VECTOR camRight = {cameraMtx[0][0], cameraMtx[1][0], cameraMtx[2][0], 0};
                    
                    VECTOR temp;
                    vector_outerproduct(temp, prevDir, camRight);
                    vector_outerproduct(prevRight, temp, prevDir);
                    vector_normalize(prevRight, prevRight);
                    
                    vector_outerproduct(temp, currDir, camRight);
                    vector_outerproduct(currRight, temp, currDir);
                    vector_normalize(currRight, currRight);
                } else {
                    VECTOR up = {0, 1, 0, 0};
                    vector_outerproduct(prevRight, prevDir, up);
                    vector_normalize(prevRight, prevRight);
                    vector_outerproduct(currRight, currDir, up);
                    vector_normalize(currRight, currRight);
                }

                // Calculate miter vector (average of the two rights)
                VECTOR miter;
                miter[0] = prevRight[0] + currRight[0];
                miter[1] = prevRight[1] + currRight[1];
                miter[2] = prevRight[2] + currRight[2];
                miter[3] = 0;
                vector_normalize(miter, miter);

                // Scale miter to maintain consistent width
                float dot = vector_innerproduct(miter, currRight);
                if (dot < 0.25f) dot = 0.25f;
                float miterScale = 1.0f / dot;
                
                vector_scale(right, miter, miterScale);
            } else {
                // Regular vertex - use standard perpendicular
                if (segmentStyle->billboard) {  // Fixed: Use segmentStyle
                    MATRIX *cameraMtx = &playerGetFromSlot(0)->camera->uMtx;
                    right[0] = cameraMtx[0][0];
                    right[1] = cameraMtx[1][0];
                    right[2] = cameraMtx[2][0];
                    right[3] = 0;
                    
                    VECTOR temp;
                    vector_outerproduct(temp, forward, right);
                    vector_outerproduct(right, temp, forward);
                    vector_normalize(right, right);
                } else {
                    VECTOR up = {0, 1, 0, 0};
                    vector_outerproduct(right, forward, up);
                    vector_normalize(right, right);
                }
            }

            // Apply line width with variation along segment
            float widthMultiplier = 1.0f;
            if (segmentStyle->widthStart != 1.0f || segmentStyle->widthEnd != 1.0f) {  // Fixed: Use segmentStyle
                widthMultiplier = segmentStyle->widthStart + (segmentStyle->widthEnd - segmentStyle->widthStart) * t;
            }
            vector_scale(right, right, segmentStyle->lineWidth * widthMultiplier * 0.5f);  // Fixed: Use segmentStyle

            // Create left and right vertices
            stripPos[vertexIndex][0] = pos[0] - right[0];
            stripPos[vertexIndex][1] = pos[1] - right[1];
            stripPos[vertexIndex][2] = pos[2] - right[2];

            stripPos[vertexIndex + 1][0] = pos[0] + right[0];
            stripPos[vertexIndex + 1][1] = pos[1] + right[1];
            stripPos[vertexIndex + 1][2] = pos[2] + right[2];

            // Apply random offset if enabled (but not at endpoints)
            if (segmentStyle->pointRandomOffset > 0.0f && !(i == 0 && s == 0) && !(i == numEndPoints - 2 && s == numSegs)) {  // Fixed: Use segmentStyle
                float angle1 = (float)((vertexIndex * 137) % 360) * (3.14159f / 180.0f);
                float angle2 = (float)((vertexIndex * 211) % 360) * (3.14159f / 180.0f);
                
                VECTOR randomDir;
                randomDir[0] = cosf(angle1) * cosf(angle2);
                randomDir[1] = sinf(angle2);
                randomDir[2] = sinf(angle1) * cosf(angle2);
                randomDir[3] = 0;
                
                vector_scale(randomDir, randomDir, segmentStyle->pointRandomOffset);  // Fixed: Use segmentStyle
                
                stripPos[vertexIndex][0] += randomDir[0];
                stripPos[vertexIndex][1] += randomDir[1];
                stripPos[vertexIndex][2] += randomDir[2];
                
                stripPos[vertexIndex + 1][0] += randomDir[0];
                stripPos[vertexIndex + 1][1] += randomDir[1];
                stripPos[vertexIndex + 1][2] += randomDir[2];
            }

            // Calculate UVs
            float currentDistance = distanceAccum + segmentLength * t;
            float uvCoord;
            
            if (segmentStyle->numSegments > 0) {  // Fixed: Use segmentStyle
                uvCoord = (float)(vertexIndex / 2);
            } else {
                uvCoord = currentDistance / segmentStyle->textureRepeatDistance;  // Fixed: Use segmentStyle
            }

            // Apply UV coordinates with optional 90 degree rotation and animation
            if (segmentStyle->rotateTexture) {  // Fixed: Use segmentStyle
                stripUV[vertexIndex].x = uvCoord + animatedUVOffset;
                stripUV[vertexIndex].y = 0.0f;
                stripUV[vertexIndex + 1].x = uvCoord + animatedUVOffset;
                stripUV[vertexIndex + 1].y = 1.0f;
            } else {
                stripUV[vertexIndex].x = 0.0f;
                stripUV[vertexIndex].y = uvCoord + animatedUVOffset;
                stripUV[vertexIndex + 1].x = 1.0f;
                stripUV[vertexIndex + 1].y = uvCoord + animatedUVOffset;
            }

            // Calculate color (with optional interpolation)
            int color;
            if (pEndPoints[i].lerpColor) {
                color = colorLerp(pEndPoints[i].color, pEndPoints[i + 1].color, t);
            } else {
                color = pEndPoints[i].color;
            }

            // Apply fade at endpoints if enabled
            if (pEndPoints[i].fadeEnds > 0.0f && t < pEndPoints[i].fadeEnds) {
                float fadeT = t / pEndPoints[i].fadeEnds;
                color = colorLerp(color & 0x00FFFFFF, color, fadeT);
            }

            if (pEndPoints[i + 1].fadeEnds > 0.0f && t > (1.0f - pEndPoints[i + 1].fadeEnds)) {
                float fadeT = (t - (1.0f - pEndPoints[i + 1].fadeEnds)) / pEndPoints[i + 1].fadeEnds;
                color = colorLerp(color, color & 0x00FFFFFF, fadeT);
            }

            stripColor[vertexIndex] = color;
            stripColor[vertexIndex + 1] = color;

            vertexIndex += 2;
        }

        distanceAccum += segmentLength;
    }

    gfxDrawStrip(vertexIndex, stripPos, stripColor, stripUV, 1);
}

void drawLine(VECTOR point_a, VECTOR point_b, u32 color, LineStyle_e style)
{
    LineEndPoint_t endpoints[2];
    
    // Setup first endpoint
    vector_copy(endpoints[0].pos, point_a);
    endpoints[0].color = color;
    endpoints[0].lerpColor = 0;
    endpoints[0].fadeEnds = 0.0f;
    endpoints[0].style = 0;
    
    // Setup second endpoint
    vector_copy(endpoints[1].pos, point_b);
    endpoints[1].color = color;
    endpoints[1].lerpColor = 0;
    endpoints[1].fadeEnds = 0.0f;
    endpoints[1].style = 0;
    
    drawLines(endpoints, 2, LineStyles[style]); 
}
