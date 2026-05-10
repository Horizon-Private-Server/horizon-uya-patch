#include <tamtypes.h>

#include <libuya/stdio.h>
#include <libuya/string.h>
#include <libuya/player.h>
#include <libuya/utils.h>
#include <libuya/game.h>
#include <libuya/pad.h>
#include <libuya/uya.h>
#include <libuya/weapon.h>
#include <libuya/interop.h>
#include <libuya/moby.h>
#include <libuya/graphics.h>
#include <libuya/gamesettings.h>
#include <libuya/spawnpoint.h>
#include <libuya/team.h>
#include <libuya/ui.h>
#include <libuya/time.h>
#include <libuya/camera.h>
#include <libuya/gameplay.h>
#include <libuya/map.h>
#include <libuya/collision.h>
#include <libuya/guber.h>
#include <libuya/sound.h>
#include <libuya/music.h>
#include <libuya/hud.h>
#include "include/koth.h"
#include "module.h"
#include "config.h"
#include "messageid.h"

void getHillCuboids(hillPvar_t *this, bool isCustomMap, bool foundCustomMoby);
extern int isCustomMap;

config_t kothConfig = {
    .grKothScoreLimit = 1, // 30s
    .grKothHillDuration = 0, // 60s
    .grKothRespawnOutside = 0, // 40f
    .grKothRespawnInside = 0, // 40f
    .grKothHillSizeIdx = 0, // 1x
    .grKothContestedStopsScore = 0,
    .grKothPointStacking = 0,
    .pad = 0,
    .grSeed = 8675309,
};

VariableAddress_t vaResurrectSpawnDistance = {
#if UYA_PAL
    .Lobby = 0x00671760,
    .Bakisi = 0x00545108,
    .Hoven = 0x005472d0,
    .OutpostX12 = 0x0053cba8,
    .KorgonOutpost = 0x0053a290,
    .Metropolis = 0x00539690,
    .BlackwaterCity = 0x00536e78,
    .CommandCenter = 0x005366d0,
    .BlackwaterDocks = 0x00538f50,
    .AquatosSewers = 0x00538250,
    .MarcadiaPalace = 0x00537bd0,
#else
    .Lobby = 0x0066ee10,
    .Bakisi = 0x005427f8,
    .Hoven = 0x00544900,
    .OutpostX12 = 0x0053a218,
    .KorgonOutpost = 0x00537980,
    .Metropolis = 0x00536d80,
    .BlackwaterCity = 0x005344e8,
    .CommandCenter = 0x00533f18,
    .BlackwaterDocks = 0x00536758,
    .AquatosSewers = 0x00535a98,
    .MarcadiaPalace = 0x005353d8,
#endif
};

// TIME_UP gameEnd callsites (Team modes)
static VariableAddress_t vaGameTimerEndHookDmTeams = {
#if UYA_PAL
    .Lobby = 0x0067063c,
    .Bakisi = 0x00543fe4,
    .Hoven = 0x005461ac,
    .OutpostX12 = 0x0053ba84,
    .KorgonOutpost = 0x0053916c,
    .Metropolis = 0x0053856c,
    .BlackwaterCity = 0x00535d54,
    .CommandCenter = 0x005355ac,
    .BlackwaterDocks = 0x00537e2c,
    .AquatosSewers = 0x0053712c,
    .MarcadiaPalace = 0x00536aac,
#else
    .Lobby = 0x0066dcec,
    .Bakisi = 0x005416d4,
    .Hoven = 0x005437dc,
    .OutpostX12 = 0x005390f4,
    .KorgonOutpost = 0x0053685c,
    .Metropolis = 0x00535c5c,
    .BlackwaterCity = 0x005333c4,
    .CommandCenter = 0x00532df4,
    .BlackwaterDocks = 0x00535634,
    .AquatosSewers = 0x00534974,
    .MarcadiaPalace = 0x005342b4,
#endif
};

// TIME_UP gameEnd callsites (Free-For-All / standard DM). Unknown maps remain 0.
static VariableAddress_t vaGameTimerEndHookDmNoTeams = {
#if UYA_PAL
    .Lobby = 0x0067063c,
    .Bakisi = 0x00543fe4,
    .Hoven = 0x005461ac,
    .OutpostX12 = 0x0053ba84,
    .KorgonOutpost = 0x0053916c,
    .Metropolis = 0x0053856c,
    .BlackwaterCity = 0x00535d54,
    .CommandCenter = 0x005355ac,
    .BlackwaterDocks = 0x00537e2c,
    .AquatosSewers = 0x0053712c,
    .MarcadiaPalace = 0x00536aac,
#else
    .Lobby = 0x0066dcec,
    .Bakisi = 0x005416d4,
    .Hoven = 0x005437dc,
    .OutpostX12 = 0x005390f4,
    .KorgonOutpost = 0x0053685c,
    .Metropolis = 0x00535c5c,
    .BlackwaterCity = 0x005333c4,
    .CommandCenter = 0x00532df4,
    .BlackwaterDocks = 0x00535634,
    .AquatosSewers = 0x00534974,
    .MarcadiaPalace = 0x005342b4,
#endif
};

void hillMovementUpdates(Moby *this)
{
    if (!this->pVar)
        return;
    
        hillPvar_t *pvars = (hillPvar_t*)this->pVar;
    pvars->opacityFactor = 1.0f;
}

int hillCheckIfInside(Cuboid *cube, VECTOR playerPos, char bIsCircle)
{
    MATRIX rotMatrix;
    matrix_unit(rotMatrix);
    matrix_rotate_x(rotMatrix, rotMatrix, cube->rot[0]);
    matrix_rotate_y(rotMatrix, rotMatrix, cube->rot[1]);
    matrix_rotate_z(rotMatrix, rotMatrix, cube->rot[2]);
    
    // get imatrix
    MATRIX invMatrix;
    matrix_unit(invMatrix);
    matrix_inverse(invMatrix, rotMatrix);
    memcpy(&cube->imatrix, invMatrix, sizeof(mtx3));

    VECTOR delta;
    vector_subtract(delta, playerPos, (VECTOR){cube->pos[0], cube->pos[1], cube->pos[2], 0});
    vector_apply(delta, delta, invMatrix);
    VECTOR xAxis = {cube->matrix.v0[0], cube->matrix.v1[0], cube->matrix.v2[0], 0};
    VECTOR zAxis = {cube->matrix.v0[1], cube->matrix.v1[1], cube->matrix.v2[1], 0};
    VECTOR yAxis = {cube->matrix.v0[2], cube->matrix.v1[2], cube->matrix.v2[2], 0};

    float halfWidth = vector_length(xAxis) * .5;
    float halfDepth = vector_length(zAxis) * .5;
    float yHeight = vector_length(yAxis);
    
    if (delta[2] < -1.25 || delta[2] > yHeight + 6) {
        return 0;
    }
    if (bIsCircle) {
        float radius = halfWidth;
        float distSq = delta[0] * delta[0] + delta[1] * delta[1];
        return (distSq <= radius * radius);
    } else {
        int x = delta[0] < -halfWidth || delta[0] > halfWidth;
        int z = delta[1] < -halfDepth || delta[1] > halfDepth;
        return (x || z) ? 0 : 1;
    }
}

void hillPlayerUpdates(Moby *this)
{
    if (!this->pVar)
        return;

    hillPvar_t *pvar = (hillPvar_t*)this->pVar;
    GameSettings *gs = gameGetSettings();
    int playerCount = gs->PlayerCount;
    int i;
    for (i = 0; i < playerCount; ++i) {
        Player *player = playerGetFromSlot(i);
        if (!player || playerIsDead(player))
            continue;

        int in = hillCheckIfInside(pvar->currentCuboid, player->playerPosition, pvar->isCircle);
        if (in) {
            pvar->color = TEAM_COLORS[player->mpTeam];
        } else {
            pvar->color = 0x00ffffff;
        }

    }
}

void hill_drawShape(Moby *this)
{
    hillPvar_t *pvar = (hillPvar_t*)this->pVar;
    if (!pvar->currentCuboid) return;
    Cuboid *cube = (Cuboid*)pvar->currentCuboid;
    if (!vector_length(cube->pos)) return;

    int arraySize = (pvar->globalRing.max_segments + 1) * 2;
    if (!pvar->positions || !pvar->colors || !pvar->uvs) {
        pvar->positions = malloc(sizeof(vec3[2][arraySize]));
        pvar->colors = malloc(sizeof(int[2][arraySize]));
        pvar->uvs = malloc(sizeof(UV_t[2][arraySize]));
    }

    vec3 (*positions)[arraySize] = pvar->positions;
    int (*colors)[arraySize] = pvar->colors;
    UV_t (*uvs)[arraySize] = pvar->uvs;

    pvar->isCircle = (vector_length(cube->matrix.v2) > 1.0001) ? true : false;
    bool isCircle = pvar->isCircle;
    u32 baseColor = pvar->color;
    int i, edge, s;
    
    /* Prepare arrays for strip vertices */
    int cachedSegments = -1;
    int cachedIsCircle = -1;

    /* Setup rotation matrix from cube */
    MATRIX rotMatrix;
    matrix_unit(rotMatrix);
    matrix_rotate_x(rotMatrix, rotMatrix, cube->rot[0]);
    matrix_rotate_y(rotMatrix, rotMatrix, cube->rot[1]);
    matrix_rotate_z(rotMatrix, rotMatrix, cube->rot[2]);
    
    /* Extract axes from cube matrix */
    VECTOR center, xAxis, zAxis, yAxis, halfX, halfZ;
    vector_copy(center, cube->pos);
    vector_copy(xAxis, (VECTOR){cube->matrix.v0[0], cube->matrix.v1[0], cube->matrix.v2[0], 0});
    vector_copy(zAxis, (VECTOR){cube->matrix.v0[1], cube->matrix.v1[1], cube->matrix.v2[1], 0});
    vector_copy(yAxis, (VECTOR){cube->matrix.v0[2], cube->matrix.v1[2], cube->matrix.v2[2], 0});
    
    /* Apply rotation to axes */
    vector_apply(xAxis, xAxis, rotMatrix);
    vector_apply(zAxis, zAxis, rotMatrix);
    vector_apply(yAxis, yAxis, rotMatrix);
    
    vector_scale(halfX, xAxis, 0.5f);
    vector_scale(halfZ, zAxis, 0.5f);
    // vector_normalize(yAxis, yAxis);
    
    float fRadius = vector_length(halfX);
    
    /* ===== Calculate segment count ===== */
    int segmentSize = 2;
    int segments;
    
    if (isCircle) {
        segments = clamp((2 * MATH_PI * fRadius) / segmentSize, pvar->globalRing.min_segments, pvar->globalRing.max_segments);
    } else {
        /* For square, calculate total segments around perimeter */
        float sideLenX = vector_length(xAxis);
        float sideLenZ = vector_length(zAxis);
        float perimeter = (sideLenX + sideLenZ) * 2.0f;
        segments = clamp((int)(perimeter / segmentSize), pvar->globalRing.min_segments, pvar->globalRing.max_segments);
    }
    
    /* ===== Setup rendering ===== */
    int floorTex = isCircle ? pvar->floor.circleTex : pvar->floor.squareTex;
    int ringTex = pvar->globalRing.texture;
    
    /* UV trimming to remove transparent edges */
    float uMin = pvar->globalRing.trim_u;
    float uMax = 1.0f - pvar->globalRing.trim_u;
    float vMin = pvar->globalRing.trim_v;
    float vMax = 1.0f - pvar->globalRing.trim_v;
    float uRange = uMax - uMin;
    float vRange = vMax - vMin;
    
    /* Initialize strip drawing */
    gfxDrawStripInit();
    gfxAddRegister(8, 0);
    gfxAddRegister(0x14, 0xff9000000260);
    gfxAddRegister(6, gfxGetEffectTex(ringTex));
    gfxAddRegister(0x47, 0x513f1);
    gfxAddRegister(0x42, 0x8000000044);

    /* === Setup shape-specific parameters === */
    VECTOR corners[4], vRadius;
    int segmentsPerEdge[4];
    float thetaStep;
    
    if (isCircle) {
        thetaStep = (2.0f * MATH_PI) / segments;
    } else {
        /* Calculate corners */
        float hx0 = halfX[0], hx1 = halfX[1], hx2 = halfX[2];
        float hz0 = halfZ[0], hz1 = halfZ[1], hz2 = halfZ[2];
        
        vector_copy(corners[0], (VECTOR){center[0]-hx0-hz0, center[1]-hx1-hz1, center[2]-hx2-hz2, 0});
        vector_copy(corners[1], (VECTOR){center[0]+hx0-hz0, center[1]+hx1-hz1, center[2]+hx2-hz2, 0});
        vector_copy(corners[2], (VECTOR){center[0]+hx0+hz0, center[1]+hx1+hz1, center[2]+hx2+hz2, 0});
        vector_copy(corners[3], (VECTOR){center[0]-hx0+hz0, center[1]-hx1+hz1, center[2]-hx2+hz2, 0});
        
        /* Calculate segments per edge proportionally */
        float sideLenX = vector_length(xAxis);
        float sideLenZ = vector_length(zAxis);
        float perimeter = (sideLenX + sideLenZ) * 2.0f;
        
        segmentsPerEdge[0] = segmentsPerEdge[2] = (int)((sideLenX / perimeter) * segments);
        segmentsPerEdge[1] = segmentsPerEdge[3] = (int)((sideLenZ / perimeter) * segments);
        
        /* Ensure we use all segments */
        segmentsPerEdge[0] += segments - (segmentsPerEdge[0] + segmentsPerEdge[1] + segmentsPerEdge[2] + segmentsPerEdge[3]);
    }
    
    int numSegments = (segments + 1) * 2;
    VECTOR tempRight, tempUp;
    
    /* === Generate positions once (only if not cached) === */
    if (cachedSegments != segments || cachedIsCircle != isCircle) {
        if (isCircle) vector_copy(vRadius, halfX);
        
        for (i = 0; i <= segments; i++) {
            VECTOR pos;
            
            if (isCircle) {
                /* Calculate position on circle */
                vector_add(pos, center, vRadius);
                
                /* Calculate tangent for quad orientation */
                vector_outerproduct(tempRight, yAxis, vRadius);
                vector_normalize(tempRight, tempRight);
                vector_copy(tempUp, yAxis);
                
                /* Rotate radius for next segment */
                vector_rodrigues(vRadius, vRadius, yAxis, thetaStep);
            } else {
                /* Determine which edge we're on */
                int accumulatedSegs = 0;
                int currentEdge = 0;
                int localS = i;
                
                for (edge = 0; edge < 4; edge++) {
                    if (i <= accumulatedSegs + segmentsPerEdge[edge]) {
                        currentEdge = edge;
                        localS = i - accumulatedSegs;
                        break;
                    }
                    accumulatedSegs += segmentsPerEdge[edge];
                }
                
                VECTOR p0, p1, edgeDir;
                vector_copy(p0, corners[currentEdge]);
                vector_copy(p1, corners[(currentEdge + 1) & 3]);
                
                vector_subtract(edgeDir, p1, p0);
                vector_normalize(edgeDir, edgeDir);
                
                float t = (float)localS / segmentsPerEdge[currentEdge];
                vector_lerp(pos, p0, p1, t);
                
                /* For square, vertices are just offset in Z direction */
                vector_copy(tempRight, (VECTOR){0, 0, 0, 0});
                vector_copy(tempUp, (VECTOR){0, 0, 0, 0});
            }
            
            int idx = i * 2;
            
            VECTOR offsetPos;
            // offset upper ring
            vector_add(offsetPos, pos, (VECTOR){0, 0, pvar->lowerRing.height_offset, 0});
            /* === Lower Ring Bottom === */
            positions[0][idx][0] = pos[0] + tempRight[0] - tempUp[0];
            positions[0][idx][1] = pos[1] + tempRight[1] - tempUp[1];
            positions[0][idx][2] = pos[2] + tempRight[2] - tempUp[2] + pvar->lowerRing.bottom_stretch;
            /* === Lower Ring Top === */
            positions[0][idx + 1][0] = pos[0] + tempRight[0] + tempUp[0];
            positions[0][idx + 1][1] = pos[1] + tempRight[1] + tempUp[1];
            positions[0][idx + 1][2] = pos[2] + tempRight[2] + tempUp[2] + pvar->lowerRing.top_stretch;
            
            // offset upper ring
            vector_add(offsetPos, pos, (VECTOR){0, 0, pvar->upperRing.height_offset, 0});
            /* === Upper Ring Botton === */
            positions[1][idx][0] = offsetPos[0] + tempRight[0] + tempUp[0];
            positions[1][idx][1] = offsetPos[1] + tempRight[1] + tempUp[1];
            positions[1][idx][2] = offsetPos[2] + tempRight[2] + tempUp[2] + pvar->upperRing.bottom_stretch;
            /* === Upper Ring Top === */
            positions[1][idx + 1][0] = offsetPos[0] + tempRight[0] - tempUp[0];
            positions[1][idx + 1][1] = offsetPos[1] + tempRight[1] - tempUp[1];
            positions[1][idx + 1][2] = offsetPos[2] + tempRight[2] - tempUp[2] + pvar->upperRing.top_stretch;
        }
        
        cachedSegments = segments;
        cachedIsCircle = isCircle;
    }
    
    /* === Calculate distance-based opacity === */
    Player *player = playerGetFromSlot(0);
    VECTOR delta;
    vector_subtract(delta, center, player->playerPosition);
    float distance = vector_length(delta);
    
    float opacityFactor = pvar->opacityFactor;
    // if (distance > 52.0f) {
    //     opacityFactor = 1.0f - clamp((distance - 52.0f) / 12.0f, 0.0f, 1.0f);
    // }
    
    /* === Update colors and UVs every frame === */
    float trimmedU = uMin + (pvar->scrollTex - floorf(pvar->scrollTex)) * uRange;
    
    for (i = 0; i <= segments; i++) {
        float trimmedV = vMin + (((float)i / segmentSize) - floorf((float)i / segmentSize)) * vRange;
        int idx = i * 2;

        colors[1][idx] = ((pvar->upperRing.top_opacity * (int)opacityFactor) << 24) | baseColor;
        colors[1][idx + 1] = ((pvar->upperRing.bottom_opacity * (int)opacityFactor) << 24) | baseColor;
        colors[0][idx + 1] = ((pvar->lowerRing.top_opacity * (int)opacityFactor) << 24) | baseColor;
        colors[0][idx] = ((pvar->lowerRing.bottom_opacity * (int)opacityFactor) << 24) | baseColor;

        uvs[0][idx].x = uvs[1][idx].x = trimmedU;
        uvs[0][idx].y = uvs[0][idx + 1].y = uvs[1][idx].y = uvs[1][idx + 1].y = trimmedV;
        uvs[0][idx + 1].x = trimmedU - 0.3f;
        uvs[1][idx + 1].x = trimmedU + 0.3f;
    }
    
    /* === Draw lower, then upper ring === */
    if (pvar->lowerRing.draw)
        gfxDrawStrip(numSegments, positions[0], colors[0], uvs[0], 1);
    if (pvar->upperRing.draw)
        gfxDrawStrip(numSegments, positions[1], colors[1], uvs[1], 1);
    
    /* Animate texture scroll */
    pvar->scrollTex += pvar->globalRing.scrollSpeed;
    if (pvar->scrollTex >= 1.0f) pvar->scrollTex -= 1.0f;
    
    /* ===== Draw floor quad ===== */
    if (!pvar->floor.draw)
        return;

    QuadDef floorQuad;
    gfxSetupEffectTex(&floorQuad, floorTex, DRAW_TYPE_NORMAL, pvar->floor.opacity);
    
    /* Offset floor down slightly */
    VECTOR offset = {0, 0, pvar->floor.height_offset, 0};
    VECTOR rotatedOffset;
    vector_apply(rotatedOffset, offset, rotMatrix);
    
    /* Create floor corners */
    VECTOR floorCorners[4];
    if (pvar->isCircle) {
        vector_copy(floorCorners[0], (VECTOR){center[0] - fRadius, center[1] - fRadius, center[2], 0});
        vector_copy(floorCorners[1], (VECTOR){center[0] + fRadius, center[1] - fRadius, center[2], 0});
        vector_copy(floorCorners[2], (VECTOR){center[0] + fRadius, center[1] + fRadius, center[2], 0});
        vector_copy(floorCorners[3], (VECTOR){center[0] - fRadius, center[1] + fRadius, center[2], 0});
    } else {
        vector_copy(floorCorners[0], (VECTOR){center[0] - halfX[0] - halfZ[0], center[1] - halfX[1] - halfZ[1], center[2] - halfX[2] - halfZ[2], 0});
        vector_copy(floorCorners[1], (VECTOR){center[0] + halfX[0] - halfZ[0], center[1] + halfX[1] - halfZ[1], center[2] + halfX[2] - halfZ[2], 0});
        vector_copy(floorCorners[2], (VECTOR){center[0] + halfX[0] + halfZ[0], center[1] + halfX[1] + halfZ[1], center[2] + halfX[2] + halfZ[2], 0});
        vector_copy(floorCorners[3], (VECTOR){center[0] - halfX[0] + halfZ[0], center[1] - halfX[1] + halfZ[1], center[2] - halfX[2] + halfZ[2], 0});
    }
    
    /* Apply offset to all corners */
    for (i = 0; i < 4; i++) {
        vector_add(floorCorners[i], floorCorners[i], rotatedOffset);
    }
    
    /* Setup floor quad vertices */
    u32 floorColor = (pvar->floor.opacity * (int)opacityFactor << 24) | baseColor;
    
    vector_copy(floorQuad.point[0], floorCorners[1]);
    vector_copy(floorQuad.point[1], floorCorners[0]);
    vector_copy(floorQuad.point[2], floorCorners[2]);
    vector_copy(floorQuad.point[3], floorCorners[3]);
    
    floorQuad.point[0][3] = floorQuad.point[1][3] = floorQuad.point[2][3] = floorQuad.point[3][3] = 1.0f;
    
    floorQuad.rgba[0] = floorQuad.rgba[1] = floorQuad.rgba[2] = floorQuad.rgba[3] = floorColor;

    floorQuad.uv[0] = (UV_t){0, 0};
    floorQuad.uv[1] = (UV_t){0, 1};
    floorQuad.uv[2] = (UV_t){1, 0};
    floorQuad.uv[3] = (UV_t){1, 1};
    
    gfxDrawQuad(floorQuad, NULL);
}

void hill_update(Moby *moby)
{
    hillPvar_t *pvars = (hillPvar_t*)moby->pVar;
    if (!pvars) return;
    if (!pvars->hillCuboidPtrs[0] && !pvars->hillCuboidIndex[0]) {
        return;
    }

    // this is where the random hill part goes.
    if (!pvars->currentCuboid)
        pvars->currentCuboid = pvars->hillCuboidPtrs[0];
    // move hill if vectors don't match.
    // float dX = moby->position[0] == pvars->currentCuboid->pos[0];
    // float dZ = moby->position[1] == pvars->currentCuboid->pos[1];
    // float dY = moby->position[2] == pvars->currentCuboid->pos[2];
    // if (!dX || !dY || !dZ)
    vector_copy(moby->position, pvars->currentCuboid->pos);
    
    gfxRegisterDrawFunction(&hill_drawShape, moby);

    // handle hill movement
    hillMovementUpdates(moby);

    // handle players
    hillPlayerUpdates(moby);

    // handle auto destruct
    // if (pvars->DestroyAtTime && gameGetTime() > pvars->DestroyAtTime) {
    //     scavHuntHBoltDestroy(moby);
    // }
}

Moby *getHillMoby(void)
{
	Moby* moby = mobyListGetStart();
	Moby* mobyEnd = mobyListGetEnd();
	while (moby < mobyEnd) {
		if (moby->oClass == HILL_MOBY_OCLASS) {
            kothInfo.foundCustomMoby = true;
            return moby;
		}
		++moby;
	}
    moby = mobySpawn(HILL_MOBY_OCLASS, HILL_MOBY_SIZE);
    return !moby ? 0 : moby;
}

void hill_setupMoby(void)
{
    Moby *moby = getHillMoby();
    if (!moby)
        return;

    hillPvar_t *pvars = (hillPvar_t*)moby->pVar;
    // setup hill cuboid list.
    getHillCuboids(pvars, isCustomMap, kothInfo.foundCustomMoby);

    moby->pUpdate = &hill_update;
    moby->updateDist = 0xff;
    moby->drawDist = 0x7fff;

    // update hill pvar with our things
    pvars->globalRing.max_segments = HILL_RING_GLOBAL_MAX_SEGMENTS;
    pvars->globalRing.min_segments = HILL_RING_GLOBAL_MIN_SEGMENTS;
    pvars->globalRing.rotateTex = HILL_RING_GLOBAL_ROTATE_TEXTURE;
    pvars->globalRing.scrollSpeed = HILL_RING_GLOBAL_SCROLL_SPEED;
    pvars->globalRing.texture = HILL_RING_GLOBAL_TEXTURE;
    pvars->globalRing.trim_u = HILL_RING_GLOBAL_TRIM_U;
    pvars->globalRing.trim_v = HILL_RING_GLOBAL_TRIM_V;

    pvars->floor.draw = HILL_FLOOR_DRAW;
    pvars->floor.circleTex = HILL_FLOOR_TEXTURE_CIRCLE;
    pvars->floor.squareTex = HILL_FLOOR_TEXTURE_SQUARE;
    pvars->floor.opacity = HILL_FLOOR_OPACITY;

    pvars->lowerRing.draw = HILL_RING_LOWER_DRAW;
    pvars->lowerRing.height_offset = HILL_RING_LOWER_HEIGHT_OFFSET;
    pvars->lowerRing.top_opacity = HILL_RING_LOWER_TOP_OPACITY;
    pvars->lowerRing.bottom_opacity = HILL_RING_LOWER_BOTTOM_OPACITY;
    pvars->lowerRing.top_stretch = HILL_RING_LOWER_TOP_STRETCH;
    pvars->lowerRing.bottom_stretch = HILL_RING_LOWER_BOTTOM_STRETCH;

    pvars->upperRing.draw = HILL_RING_UPPER_DRAW;
    pvars->upperRing.height_offset = HILL_RING_UPPER_HEIGHT_OFFSET;
    pvars->upperRing.top_opacity = HILL_RING_UPPER_TOP_OPACITY;
    pvars->upperRing.bottom_opacity = HILL_RING_UPPER_BOTTOM_OPACITY;
    pvars->upperRing.top_stretch = HILL_RING_UPPER_TOP_STRETCH;
    pvars->upperRing.bottom_stretch = HILL_RING_UPPER_BOTTOM_STRETCH;

    kothInfo.hillMoby = moby;
}

int kothUseTeams(void)
{
    GameOptions *go = gameGetOptions();
    return go && go->GameFlags.MultiplayerGameFlags.Teams;
}

int kothValidPlayerIdx(int idx)
{
    return idx >= 0 && idx < GAME_MAX_PLAYERS;
}

int kothFindLeader(int *outScore, int *outTie)
{
    GameSettings *gs = gameGetSettings();
    if (!gs)
        return -1;

    int useTeams = kothUseTeams();
    int leaderIdx = -1;
    int leaderScore = 0;
    int isTie = 0;
    GameData *gd = gameGetData();
    if (!kothInfo.hillMoby)
        return -1;
    hillPvar_t *hill = (hillPvar_t*)kothInfo.hillMoby->pVar;

    if (useTeams) {
        int teamScores[GAME_MAX_PLAYERS];
        char teamSeen[GAME_MAX_PLAYERS];
        int teamKills[GAME_MAX_PLAYERS];

        Player **players = playerGetAll();
        int i;
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            Player *p = players[i];
            if (!p)
                continue;
            int playerIdx = p->mpIndex;
            if (!kothValidPlayerIdx(playerIdx))
                continue;
            int team = gs->PlayerTeams[playerIdx];
            if (team < 0 || team >= GAME_MAX_PLAYERS)
                continue;
            teamSeen[team] = 1;
            teamScores[team] += hill->teamTime[playerIdx];
            teamKills[team] += gd ? gd->playerStats.frag[playerIdx].kills : 0;
        }

        int t;
        for (t = 0; t < GAME_MAX_PLAYERS; ++t) {
            if (!teamSeen[t])
                continue;
            int score = teamScores[t];
            if (leaderIdx < 0 || score > leaderScore) {
                leaderIdx = t;
                leaderScore = score;
                isTie = 0;
            } else if (score == leaderScore) {
                // tie-break on kills between tied top teams only
                if (teamKills[t] > teamKills[leaderIdx]) {
                    leaderIdx = t;
                    leaderScore = score;
                    isTie = 0;
                } else if (teamKills[t] == teamKills[leaderIdx]) {
                    isTie = 1;
                } else {
                    isTie = 1;
                }
            }
        }
    } else {
        Player **players = playerGetAll();
        char seen[GAME_MAX_PLAYERS];
        int i;

        memset(seen, 0, sizeof(seen));
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            Player *p = players[i];
            if (!p)
                continue;

            int idx = p->mpIndex;
            if (idx < 0 || idx >= GAME_MAX_PLAYERS)
                continue;
            if (seen[idx])
                continue;
            if (!gs->PlayerNames[idx][0])
                continue;

            seen[idx] = 1;
            int score = hill->teamTime[idx];
            int kills = gd ? gd->playerStats.frag[idx].kills : 0;
            if (leaderIdx < 0 || score > leaderScore) {
                leaderIdx = idx;
                leaderScore = score;
                isTie = 0;
            } else if (score == leaderScore) {
                // tie-break on kills between tied top players only
                int leaderKills = gd ? gd->playerStats.frag[leaderIdx].kills : 0;
                if (kills > leaderKills) {
                    leaderIdx = idx;
                    leaderScore = score;
                    isTie = 0;
                } else if (kills == leaderKills) {
                    isTie = 1;
                } else {
                    isTie = 1;
                }
            }
        }
    }

    if (outScore)
        *outScore = leaderScore;
    if (outTie)
        *outTie = isTie;
    return leaderIdx;
}

int kothSetWinnerFields(int winnerIdx, int reason, int endGame)
{
    GameSettings *gs = gameGetSettings();
    GameOptions *go = gameGetOptions();
    GameData *gd = gameGetData();
    if (!gs || !go || !gd)
        return 0;

    int useTeams = go->GameFlags.MultiplayerGameFlags.Teams;
    int winnerId = winnerIdx;
    if (useTeams) {
        if (winnerId < 0 || winnerId >= GAME_MAX_PLAYERS)
            return 0;
    } else {
        if (winnerId < 0 || winnerId >= GAME_MAX_PLAYERS)
            return 0;
    }
    if (winnerId < 0)
        return 0;

    gd->gameEndReason = reason;
    gameSetWinner(winnerId, useTeams);
    if (endGame)
        gameEnd(reason);
    kothInfo.gameState = 3;
    return 1;
}

void kothClearMalloc(void)
{
    // check if malloc was used last game.  if so, free before anything else.
    Moby *m = (Moby*)kothInfo.hillMoby;
    if (kothInfo.lastGameUsedMalloc == 1 && m != 0) {
        hillPvar_t *n = (hillPvar_t*)m->pVar;
        if (!n)
            return;

        int i;
        for (i = 0; i < sizeof(n->hillCuboidPtrs); i++) {
            if (n->hillCuboidPtrs[i])
                free(n->hillCuboidPtrs[i]);
        }
    }
}

void kothDeclareWinner(int winnerIdx, int reason)
{
    if (!kothSetWinnerFields(winnerIdx, reason, 1))
        return;
}

void runGameEnd(int reason)
{
    int forwardReason = reason;

    if (kothInfo.gameState < 2) {
        kothInfo.gameState = 2;
        if (reason == GAME_END_TIME_UP) {
            int leaderScore = 0;
            int isTie = 0;
            int leaderIdx = kothFindLeader(&leaderScore, &isTie);
            if (leaderIdx >= 0 && !isTie) {
                // Mirror score-limit path to avoid base TIME_UP overriding the winner.
                kothSetWinnerFields(leaderIdx, GAME_END_TEAM_WIN, 0);
                forwardReason = GAME_END_TEAM_WIN;
            }
        }
    }
    // clear hills if malloc.  this must be done before loading back into staging.
    kothClearMalloc();
    // Forward to original gameEnd.
    gameEnd(forwardReason);
}

void kothHooksInit(void)
{
    GameOptions *go = gameGetOptions();
    VariableAddress_t *hookTable = go && go->GameFlags.MultiplayerGameFlags.Teams ? &vaGameTimerEndHookDmTeams : &vaGameTimerEndHookDmNoTeams;
    u32 hook = GetAddress(hookTable);
    if (!hook && hookTable != &vaGameTimerEndHookDmTeams)
        hook = GetAddress(&vaGameTimerEndHookDmTeams);
    if (hook)
        HOOK_JAL(hook, &runGameEnd);
}

// int radarSetVisible(int visible)
// {
//     int hasFrame = hudSetFlags(KOTH_HUD_FRAME_ID, 1, visible);
//     int hasContainer = hudSetFlags(KOTH_HUD_CONTAINER_ID, 1, visible);
//     return hasFrame || hasContainer;
// }

// void radarUpdate(void)
// {
//     float x, y;
//     int id = KOTH_HUD_FRAME_ID;
//     int a = kothGetActiveHillIndex();
//     if (a < 0 || a >= hillCount) {
//         radarSetVisible(0);
//         return;
//     }

//     if (!kothVectorIsSane(hills[a].position)) {
//         radarSetVisible(0);
//         return;
//     }

//     radarSetVisible(1);
//     gfxWStoMapSpace(hills[a].position, &x, &y);
//     if (!kothFloatIsSane(x) || !kothFloatIsSane(y)) {
//         radarSetVisible(0);
//         return;
//     }
//     x = clampf(x, 0.0f, 1.0f);
//     y = clampf(y, 0.0f, 1.0f);
//     hudSetPosition(x, y, id);
//     // Alpha overlay tinted by current hill occupant team (if any) and blinked with hill warning.
//     float fadeScale = kothGetBlinkScale();
//     int alpha = (int)(0x80 * fadeScale);
//     if (alpha > 0xFF) alpha = 0xFF;
//     hudSetColor(id, (alpha << 24) | (kothGetActiveHillColor() & 0x00FFFFFF));
// }


// void radarRun(void)
// {
//     radarUdate();

//     // Reuse the HUD widget once it exists. Recreating and re-adding it every
//     // tick can corrupt the HUD tree and manifests like a soft freeze.
//     if (kothInfo.radar) {
//         if (radarSetVisible(1))
//             return;

//         // HUD state was torn down underneath us (map load/reset), so rebuild it.
//         kothInfo.radar = false;
//     }

//     // Create container first
//     hudCreateContainer(KOTH_HUD_CONTAINER_ID);
//     // Make container visible
//     hudSetFlags(KOTH_HUD_CONTAINER_ID, 1, true);
//     // Add container to radar root (0x50000) so it renders
//     hudAddToContainer(HUD_RADAR_ROOT, KOTH_HUD_CONTAINER_ID);
//     // create rectangle widget
//     if (hudCreateRectangle(0.05, 0.05, 0.05, 0.05, KOTH_HUD_FRAME_ID, 0x80ffffff, KOTH_HUD_HILL_SPRITE)) {
//         // add rectrangle frame to main container
//         hudAddToContainer(KOTH_HUD_CONTAINER_ID, KOTH_HUD_FRAME_ID);
//     }
//     hudSetFlags(KOTH_HUD_FRAME_ID, 1, true);

//     kothInfo.radar = true;
// }

int scoreboardHillLimit(void)
{
    const int SCORE_LIMIT_OPTIONS[] = {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 750, 1000, 2000};
    // int idx = kothConfig ? (int)kothConfig->grKothScoreLimit : 0;
    int idx = (int)kothConfig.grKothScoreLimit;
    if (idx < 0 || idx >= (int)COUNT_OF(SCORE_LIMIT_OPTIONS))
        return SCORE_LIMIT_OPTIONS[0];

    return SCORE_LIMIT_OPTIONS[idx];
}

void scoreboardRun(void)
{
    if (kothInfo.hillScoreLimit > 0)
        kothInfo.hillScoreLimit = scoreboardHillLimit();

    hillPvar_t *pvars = (hillPvar_t*)kothInfo.hillMoby->pVar;
    scoreboard(kothInfo.hillScoreLimit, pvars->teamTime);
}

void koth(void)
{
    // setup hill
    if (!kothInfo.hillMoby) {
        hill_setupMoby();
        printf("\nkothInfo: %08x", &kothInfo);
    } else {
        hill_update(kothInfo.hillMoby);
        printf("\nkothInfo (hill updated): %08x", &kothInfo);
    }

    // create and run radar icon.
    // radarRun();
    // run scoreboard
    // scoreboardRun();

    // if (kothRespawnDistanceIsCustom())
    //     kothUpdateRespawnDistanceForLocals();

    
}

void kothInit(PatchGameConfig_t* gameConfig)
{
	// hook messages
	// netHookMessages();

	memset(&kothInfo, 0, sizeof(kothInfo_t));
    kothInfo.hillScoreLimit = -1;

	void * connection = netGetLobbyServerConnection();
	// if online, install messages
    // if (connection) {
    //     netInstallCustomMsgHandler(CUSTOM_MSG_ID_KOTH_SCORE_UPDATE, &kothOnReceiveScore);
    //     netInstallCustomMsgHandler(CUSTOM_MSG_ID_KOTH_HILL_SYNC, &kothOnReceiveHillSync);
    // }

    // Disable base-game frag limit so KOTH doesn't end on kills.
    // GameOptions *go = gameGetOptions();
    // if (go)
    //     go->GameFlags.MultiplayerGameFlags.FragLimit = 0;

    // init hooks
    // kothHooksInit();


	// give a 1 second delay before finalizing the initialization.
	// this helps prevent the slow loaders from desyncing
	static int startDelay = 60 * 2;
	if (startDelay > 0) {
		--startDelay;
		return;
	}

	// finalize initialization
	State.Initialized = 1;
}
