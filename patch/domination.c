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
#include <libuya/guber.h>
#include <libuya/sound.h>

#define MAX_SEGMENTS (64)
#define MIN_SEGMENTS (8)
#define BASE_RADIUS (10.0f)
#define CAPTURE_STEP (0.001f)

static inline int playerIsLocal(Player *player)
{
    return player && player->isLocal;
}

static float clamp01(float value)
{
    if (value < 0.0f) return 0.0f;
    if (value > 1.0f) return 1.0f;
    return value;
}

static void approachFloat(float *value, float target, float step)
{
    if (*value < target) {
        *value += step;
        if (*value > target)
            *value = target;
    } else if (*value > target) {
        *value -= step;
        if (*value < target)
            *value = target;
    }
}

static u32 lerpColor(u32 a, u32 b, float t)
{
    t = clamp01(t);
    int aA = (a >> 24) & 0xFF;
    int aR = (a >> 16) & 0xFF;
    int aG = (a >> 8) & 0xFF;
    int aB = a & 0xFF;

    int bA = (b >> 24) & 0xFF;
    int bR = (b >> 16) & 0xFF;
    int bG = (b >> 8) & 0xFF;
    int bB = b & 0xFF;

    int rA = aA + (int)((bA - aA) * t);
    int rR = aR + (int)((bR - aR) * t);
    int rG = aG + (int)((bG - aG) * t);
    int rB = aB + (int)((bB - aB) * t);

    return (rA << 24) | (rR << 16) | (rG << 8) | (rB & 0xFF);
}

// if local player is blue, invert
static u32 getBoltCrankTextColor(float percent01, int invert)
{
    const u32 redColor = 0x00FF5050;
    const u32 whiteColor = 0x00FFFFFF;
    const u32 blueColor = 0x003060FF;

    if (invert) {
        percent01 = clamp01(percent01);
        if (percent01 <= 0.5f) {
            float localT = percent01 / 0.5f;
            return lerpColor(redColor, whiteColor, localT);
        }

        float localT = (percent01 - 0.5f) / 0.5f;
        return lerpColor(whiteColor, blueColor, localT);
    } else {
        percent01 = clamp01(percent01);
        if (percent01 <= 0.5f) {
            float localT = percent01 / 0.5f;
            return lerpColor(blueColor, whiteColor, localT);
        }

        float localT = (percent01 - 0.5f) / 0.5f;
        return lerpColor(whiteColor, redColor, localT);  
    }
}

typedef struct DominationBase {
	int state;
    float bias;
    Moby *node;
	Moby *boltCrank;
    Player *players[8];
	int color;
    float scrolling;
    float boltCrankPercent;
    int localPlayerInside;
    int localPlayerColor;
} DominationBase_t;

typedef struct DominationInfo {
	int gameState;
	float baseRaddius;
    int baseCount;
    Moby *bases[8];
} DominationInfo_t;
DominationInfo_t domInfo;

Moby *spawnBaseMobies(Moby *node, Moby *boltCrank);

void vector_rodrigues(VECTOR output, VECTOR v, VECTOR axis, float angle)
{
    VECTOR k, v_cross, term1, term2, term3;
    float cosTheta = cosf(angle);
    float sinTheta = sinf(angle);

    // normalize axis into k
    vector_normalize(k, axis);

    // term1 = v * cos(theta)
    vector_scale(term1, v, cosTheta);

    // term2 = (k x v) * sin(theta)
    vector_outerproduct(v_cross, k, v);  // cross product
    vector_scale(term2, v_cross, sinTheta);

    // term3 = k * (k . v) * (1 - cos(theta))
    float dot = vector_innerproduct(k, v);
    vector_scale(term3, k, dot * (1.0f - cosTheta));

    // output = term1 + term2 + term3
    vector_add(output, term1, term2);
    vector_add(output, output, term3);

    // preserve homogeneous component
    output[3] = v[3];
}

void getBases(void)
{
	Moby* moby = mobyListGetStart();
	Moby* mobyEnd = mobyListGetEnd();
	int i = 0;
	while (moby < mobyEnd) {
		if (moby->oClass == MOBY_ID_SIEGE_NODE) {
			// grab associated bolt crank from bridge peice
			int boltCrankInstanceNum = *(int*)(moby->pVar + 0x3c);
			Moby* list = mobyListGetStart();
			Moby* boltCrank = list + boltCrankInstanceNum;
			if (boltCrank->oClass == MOBY_ID_BOLT_CRANK) {
				Moby *base = spawnBaseMobies(moby, boltCrank);
                domInfo.bases[i] = (Moby*)base;
                ++domInfo.baseCount;
                printf("\nn: %d, base: %08x", domInfo.baseCount, &domInfo.bases[i]);
            }
			++i;
		}
		++moby;
	}
}

void drawBase(Moby *base)
{
    DominationBase_t *pvar = base->pVar;
	float scrollQuad = pvar->scrolling;
    u32 baseColor = getBoltCrankTextColor(clamp01(pvar->boltCrankPercent * 0.01f), pvar->localPlayerColor);
    
    int i, k, j, s;
    QuadDef quad[3];
    // get texture info (tex0, tex1, clamp, alpha)
    //gfxSetupEffectTex(&quad[0], FX_TIRE_TRACKS + 1, 0, 0x80);
    gfxSetupEffectTex(&quad[0], FX_RETICLE_5, 0, 0x80);
    gfxSetupEffectTex(&quad[2], FX_CIRLCE_NO_FADED_EDGE, 0, 0x80);

    quad[0].uv[0] = (UV_t){0, 0}; // bottom left (-, -)
    quad[0].uv[1] = (UV_t){0, 1}; // top left (-, +)
    quad[0].uv[2] = (UV_t){1, 0}; // bottom right (+, -)
    quad[0].uv[3] = (UV_t){1, 1}; // top right (+, +)

    // copy quad 0 to quad 2
    memcpy(quad[2].uv, &quad[0].uv, sizeof(quad[0].uv));

    // modify top and bottom level UVs Y.  (uv is turned 90 degrees)
    float uvOffset = 0; // .04;
    quad[0].uv[0].y += uvOffset;
    quad[0].uv[1].y -= uvOffset;
    quad[0].uv[2].y += uvOffset;
    quad[0].uv[3].y -= uvOffset;

    // copy quad 0 uv to quad 1;
    quad[1] = quad[0];

    // set seperate rgbas
    quad[0].rgba[0] = quad[0].rgba[1] = (0x00 << 24) | baseColor;
    quad[0].rgba[2] = quad[0].rgba[3] = (0x20 << 24) | baseColor;
    quad[1].rgba[0] = quad[1].rgba[1] = (0x30 << 24) | baseColor;
    quad[1].rgba[2] = quad[1].rgba[3] = (0x10 << 24) | baseColor;
    quad[2].rgba[0] = quad[2].rgba[1] = quad[2].rgba[2] = quad[2].rgba[3] = (0x30 << 24) | baseColor;

    VECTOR center, tempCenter, tempRight, tempUp, halfX, halfZ, vRadius;
    vector_copy(center, base->position);
    VECTOR xAxis = {domInfo.baseRaddius, 0, 0, 0};
    VECTOR zAxis = {0, domInfo.baseRaddius, 0, 0};
    VECTOR yAxis = {0, 0, 2, 0};
    
    vector_scale(halfX, xAxis, .5);
    vector_scale(halfZ, zAxis, .5);
    float fRadius = vector_length(halfX);
    int signs[4][2] = {{1, -1}, {-1, -1}, {1, 1}, {-1, 1}};
    vector_normalize(yAxis, yAxis);

    // get tangent
    vector_outerproduct(tempRight, yAxis, halfX);
    vector_normalize(tempRight, tempRight);

    // scale x, y of texture
    vector_scale(tempRight, tempRight, 1);
    vector_scale(tempUp, yAxis, 1);

    float segmentSize = 1;
    int segments = (int)((2 * MATH_PI * fRadius) / segmentSize);
    float thetaStep = 2 * MATH_PI / clamp((float)segments, MIN_SEGMENTS, MAX_SEGMENTS);

    for (k = 0; k < 2; ++k) {
		// draw top and botom quad
		// copy vRadius into r
		vector_copy(vRadius, halfX);
		for (i = 0; i < segments; ++i) {
			vector_add(tempCenter, center, vRadius);
			// offset quad[1] by 3
			tempCenter[2] += k * 2;
			// create vector for each point.
			for (j = 0; j < 4; ++j) {
				quad[k].point[j][0] = tempCenter[0] + signs[j][0] * tempRight[0] + signs[j][1] * tempUp[0];
				quad[k].point[j][1] = tempCenter[1] + signs[j][0] * tempRight[1] + signs[j][1] * tempUp[1];
				quad[k].point[j][2] = tempCenter[2] + signs[j][0] * tempRight[2] + signs[j][1] * tempUp[2];
				quad[k].point[j][3] = 1;
			}

			quad[k].uv[0].x = quad[k].uv[1].x = 0 - scrollQuad;
			quad[k].uv[2].x = quad[k].uv[3].x = 1 - scrollQuad;
			
			// maybe for later:  put points in array.
			// quadPos[k][i] = quad[k].point;
			
			gfxDrawQuad(quad[k], NULL);

			// rotate radius and tangent
			vector_rodrigues(vRadius, vRadius, yAxis, thetaStep);
			vector_outerproduct(tempRight, yAxis, vRadius);
			vector_normalize(tempRight, tempRight);
		}
    }
    // scroll quad to animate
    pvar->scrolling += .007f;

    // draw floor quad
    VECTOR offset = {0, 0, -1, 0};
    VECTOR corners[4];

	vector_copy(corners[0], (VECTOR){center[0] - fRadius, center[1] - fRadius, center[2], 0});
    vector_copy(corners[1], (VECTOR){center[0] + fRadius, center[1] - fRadius, center[2], 0});
    vector_copy(corners[2], (VECTOR){center[0] + fRadius, center[1] + fRadius, center[2], 0});
    vector_copy(corners[3], (VECTOR){center[0] - fRadius, center[1] + fRadius, center[2], 0});

    vector_copy(quad[2].point[0], corners[1]);
    vector_copy(quad[2].point[1], corners[0]);
    vector_copy(quad[2].point[2], corners[2]);
    vector_copy(quad[2].point[3], corners[3]);

    gfxDrawQuad(quad[2], NULL);

    // show capture progress when a local player is inside the base radius
    if (pvar->localPlayerInside) {
        char text[32];
        int percentRounded = (int)(pvar->boltCrankPercent);
        if (percentRounded < 0)
            percentRounded = 0;
        if (percentRounded > 100)
            percentRounded = 100;

        snprintf(text, sizeof(text), "Node capture progress: %d%%", percentRounded);
        float percent01 = pvar->boltCrankPercent * 0.01f;
        u32 textColor = (0x80 << 24) | getBoltCrankTextColor(percent01, pvar->localPlayerColor);
        gfxScreenSpaceText(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85f, 1, 1, textColor, text, -1, TEXT_ALIGN_MIDDLECENTER, FONT_BOLD);
    }
}

int baseCheckIfInside(VECTOR basePos, VECTOR playerPos)
{
    VECTOR delta;
    vector_subtract(delta, playerPos, basePos);

    // check Y axis
    if (delta[2] < -1.25 || delta[2] > basePos[2] + 6) {
        return 0;
    }
    // check radius
    float radius = domInfo.baseRaddius / 2;
    float distSq = delta[0] * delta[0] + delta[1] * delta[1];

    return (distSq <= radius * radius);
}

void basePlayerUpdate(Moby *this)
{
    DominationBase_t *pvar = (DominationBase_t*)this->pVar;
    GameSettings *gs = gameGetSettings();
    int i, j;
    int localPlayerInside = 0;
    int localPlayerColor = -1;
    Player** players = playerGetAll();
    
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player *player = players[i];
        if (!player || playerIsDead(player))
            continue;

        int in = baseCheckIfInside(this->position, player->playerPosition);
        if (in) {
            if (playerIsLocal(player)) {
                localPlayerInside = 1;
                localPlayerColor = player->mpTeam;
            }

            // Check if player is already in the array
            int alreadyIn = 0;
            for (j = 0; j < 8; ++j) {
                if (pvar->players[j] == player) {
                    alreadyIn = 1;
                    break;
                }
            }
            
            // Add player if not already in array
            if (!alreadyIn) {
                for (j = 0; j < 8; ++j) {
                    if (!pvar->players[j]) {
                        pvar->players[j] = player;
                        break;
                    }
                }
            }
        } else {
            // Remove player from array if they left the radius
            for (j = 0; j < 8; ++j) {
                if (pvar->players[j] == player) {
                    pvar->players[j] = NULL;
                    break;
                }
            }
        }
    }

    pvar->localPlayerInside = localPlayerInside;
    pvar->localPlayerColor = localPlayerColor;
    
    // Set color based on first player in array
    // pvar->color = 0x00ffffff; // Default white
    // for (j = 0; j < 8; ++j) {
    //     if (pvar->players[j]) {
    //         pvar->color = TEAM_COLORS[pvar->players[j]->mpTeam];
    //         break;
    //     }
    // }

    // set color based on base owner team
    // pvar->color = 0x00ffffff;
    // if (pvar->owner > -1)
    //     pvar->color = TEAM_COLORS[pvar->owner];

    // cache capture percentage for HUD drawing
    pvar->boltCrankPercent = 0;
    if (pvar->boltCrank && pvar->boltCrank->pVar) {
        M6695_BoltCrank_t *boltVars = (M6695_BoltCrank_t*)pvar->boltCrank->pVar;
        if (boltVars) {
            float normalized = clamp01(boltVars->bias);
            if (localPlayerColor == 0) {
                // Invert the normalized value
                pvar->boltCrankPercent = (1.0f - normalized) * 100.0f;
            } else if (localPlayerColor == 1) {
                // Use the normalized value directly
                pvar->boltCrankPercent = normalized * 100.0f;
            } else {
                pvar->boltCrankPercent = normalized * 100.0f;
            }
        }
    }
}

void baseHandleCapture(Moby* this)
{
    int capturingTeam = -1;
    int isContested = 0;
    int capturingCount = 0;
    int defendingCount = 0;
    Player *capturingPlayer[8] = {0};  // Initialize to NULL
    Player *defendingPlayer[8] = {0};  // Initialize to NULL
    int i;
    DominationBase_t *pvars = (DominationBase_t*)this->pVar;

    // Safety check
    if (!pvars || !pvars->boltCrank || !pvars->boltCrank->pVar)
        return;
    M6695_BoltCrank_t *boltVars = (M6695_BoltCrank_t*)pvars->boltCrank->pVar;

    // Get first capturing player and team
    for (i = 0; i < 8; ++i) {
        if (pvars->players[i] && !playerIsDead(pvars->players[i])) {
            if (capturingTeam == -1) {
                // First player found - they become the capturing team
                capturingTeam = pvars->players[i]->mpTeam;
                capturingPlayer[capturingCount] = pvars->players[i];
                ++capturingCount;
            } else if (pvars->players[i]->mpTeam == capturingTeam) {
                // Same team as capturing team
                capturingPlayer[capturingCount] = pvars->players[i];
                ++capturingCount;
            } else {
                // Different team - it's contested!
                defendingPlayer[defendingCount] = pvars->players[i];
                ++defendingCount;
                isContested = 1;
            }
        }
    }

    float targetBias = 0.5f;
    float step = CAPTURE_STEP;
    if (!isContested && capturingTeam != -1) {
        if (capturingCount > 1) {
            step *= capturingCount;
            if (step > 0.05f)
                step = 0.05f;
        }

        if (capturingTeam == TEAM_BLUE) {
            targetBias = 0.0f;
        } else if (capturingTeam == TEAM_RED) {
            targetBias = 1.0f;
        } else {
            targetBias = 0.5f;
        }
        approachFloat(&boltVars->bias, targetBias, step);
        pvars->bias = clamp01(boltVars->bias);
    } else {
        pvars->state = 5;
        step = 0.005f;
    }

    //approachFloat(&boltVars->bias, targetBias, step);
    //boltVars->bias = clamp01(boltVars->bias);
    //*(float*)(pvars->boltCrank->pVar) = boltVars->bias;
}

void updateBase(Moby* this)
{
    DominationBase_t *pvars = (DominationBase_t*)this->pVar;
    if (!pvars) return;

    // turn collision off and move bolt crank to y: 0
	pvars->boltCrank->collData = 0;
	pvars->boltCrank->position[2] = 0;

    // draw base
    gfxRegistserDrawFunction(&drawBase, (Moby*)this);

    // handle players and base color
    basePlayerUpdate(this);

    // handle capture
    baseHandleCapture(this);
}

Moby *spawnBaseMobies(Moby *node, Moby *boltCrank)
{
    Moby *moby = mobySpawn(0x1c0d, sizeof(DominationBase_t));
    if (!moby) return NULL;

    moby->pUpdate = &updateBase;
    vector_copy(moby->position, boltCrank->position);
    moby->updateDist = -1;
    moby->drawn = 1;
    moby->opacity = 0x00;
    moby->drawDist = 0x00;

    // set pvars
    DominationBase_t *base = (DominationBase_t*)moby->pVar;
    memset(base, 0, sizeof(DominationBase_t));
    base->node = (Moby*)node;
    base->boltCrank = (Moby*)boltCrank;
    base->color = 0x00ffffff;
    base->bias = 0.5f;
    base->boltCrankPercent = 50.0f;


    return moby;
}

void domination(void)
{
	if (!isInGame()) {
        domInfo.gameState = 0;
		return;
    }

	if (domInfo.gameState == 0) {
		getBases();
		domInfo.gameState = 1;
		domInfo.baseRaddius = BASE_RADIUS;
	}
}
