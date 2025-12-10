#include <tamtypes.h>
#include <string.h>

#include <libuya/stdio.h>
#include <libuya/string.h>
#include <libuya/player.h>
#include <libuya/utils.h>
#include <libuya/game.h>
#include <libuya/interop.h>
#include <libuya/moby.h>
#include <libuya/graphics.h>
#include <libuya/gamesettings.h>
#include <libuya/spawnpoint.h>
#include <libuya/ui.h>
#include <libuya/time.h>
#include <libuya/net.h>
#include <libuya/team.h>
#include <libuya/math.h>
#include "messageid.h"
#include "config.h"
#include "include/koth.h"

#define KOTH_ENABLE_HILL_SYNC 1

#define KOTH_DEBUG
#ifdef KOTH_DEBUG
#define KOTH_LOG(fmt, args...) printf(fmt, ##args)
#else
#define KOTH_LOG(fmt, args...) ((void)0)
#endif

#ifndef TEAM_MAX
#define TEAM_MAX 8
#endif

#define KOTH_MAX_HILLS         (8)
#define KOTH_RING_RADIUS       (10.0f)
#define KOTH_RING_HEIGHT       (1.0f)
#define KOTH_RING_ALPHA_SCALE  (1.0f)
#define KOTH_SCORE_TICK_MS     (TIME_SECOND)
#define KOTH_HILL_ACTIVE_MS    (TIME_SECOND * 60)
#define KOTH_NAME_MAX_LEN      (7)

// Toggle how the active hill is selected each rotation window
// Define KOTH_RANDOM_ORDER to cycle hills in a deterministic randomized order without replacement.
// Leave undefined to rotate in fixed index order.
#define KOTH_RANDOM_ORDER

typedef struct KothHill {
    VECTOR position;
    Moby *moby;
    Moby *drawMoby;
    float scroll;
#ifdef KOTH_RANDOM_ORDER
    int orderIdx;
#endif
} KothHill_t;

static KothHill_t hills[KOTH_MAX_HILLS];
static int hillCount = 0;
static int initialized = 0;
static int gameEndHookInstalled = 0;
static int gameEndHandled = 0;
static char kothTitle[32];
static int handlerInstalled = 0;
static int nextScoreTickTime = 0;
static int gameOverTriggered = 0;
static int hillCycleStartTime = 0;
static int lastTimeStart = -1;
#if KOTH_ENABLE_HILL_SYNC
static int lastActiveHillIdx = -1;
#endif
#ifdef KOTH_RANDOM_ORDER
static int hillOrder[KOTH_MAX_HILLS];
static int hillOrderCount = 0;
static u32 hillOrderSeed = 0;
static u32 hillOrderShuffleNonce = 0;

// simple LCG for deterministic shuffling
static u32 kothRngNext(u32 *state)
{
    *state = (*state * 1664525u + 1013904223u);
    return *state;
}

static int kothRandRange(u32 *state, int min, int max)
{
    if (max <= min)
        return min;
    u32 r = kothRngNext(state);
    return min + (int)(r % (u32)(max - min));
}
#endif
static int kothScores[GAME_MAX_PLAYERS];
static int lastBroadcastScore[GAME_MAX_PLAYERS];
static PatchGameConfig_t *kothConfig = NULL;
static int lastSeed = 0;

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

static float clampf(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

static int kothGetActiveHillIndex(void);
static int kothUseTeams(void);
#if KOTH_ENABLE_HILL_SYNC
static void kothBroadcastHillIndex(int hillIdx);
#endif

void kothSetConfig(PatchGameConfig_t *config)
{
    kothConfig = config;
    lastSeed = config ? config->grSeed : 0;
}

static void kothCopyName(char *dst, const char *src)
{
    int i = 0;
    for (i = 0; i < KOTH_NAME_MAX_LEN && src[i]; ++i)
        dst[i] = src[i];
    dst[i] = 0;
}

static int kothUseTeams(void)
{
    GameOptions *go = gameGetOptions();
    return go && go->GameFlags.MultiplayerGameFlags.Teams;
}

static int kothGetActiveHillIndex(void);

static void vector_rodrigues(VECTOR output, VECTOR v, VECTOR axis, float angle)
{
    VECTOR k, v_cross, term1, term2, term3;
    float cosTheta = cosf(angle);
    float sinTheta = sinf(angle);

    vector_normalize(k, axis);
    vector_scale(term1, v, cosTheta);
    vector_outerproduct(v_cross, k, v);
    vector_scale(term2, v_cross, sinTheta);
    float dot = vector_innerproduct(k, v);
    vector_scale(term3, k, dot * (1.0f - cosTheta));
    vector_add(output, term1, term2);
    vector_add(output, output, term3);
    output[3] = v[3];
}

static void scanHillsOnce(void)
{
    if (initialized)
        return;

    memset(hills, 0, sizeof(hills));
    hillCount = 0;

    Moby* moby = mobyListGetStart();
    Moby* mobyEnd = mobyListGetEnd();
    while (moby < mobyEnd && hillCount < KOTH_MAX_HILLS) {
        if (moby->oClass == MOBY_ID_SIEGE_NODE) {
            vector_copy(hills[hillCount].position, moby->position);
            hills[hillCount].position[3] = 1;
            hills[hillCount].moby = moby;
            hills[hillCount].scroll = 0;
            hills[hillCount].drawMoby = mobySpawn(0x1c0d, 0);
            if (hills[hillCount].drawMoby) {
                vector_copy(hills[hillCount].drawMoby->position, moby->position);
                hills[hillCount].drawMoby->updateDist = -1;
                hills[hillCount].drawMoby->drawn = 1;
                hills[hillCount].drawMoby->opacity = 0;
                hills[hillCount].drawMoby->drawDist = 0x00;
                hills[hillCount].drawMoby->pUpdate = NULL; // set later
            }
            ++hillCount;
        }
        ++moby;
    }

    // If we didn't find any hills yet (e.g., moby list not populated), try again on the next tick.
    if (hillCount <= 0)
        return;

#ifdef KOTH_RANDOM_ORDER
    hillOrderCount = hillCount;
    int i;
    for (i = 0; i < hillCount; ++i)
        hillOrder[i] = i;
    // deterministic Fisher-Yates shuffle
    hillOrderSeed = kothConfig ? (u32)kothConfig->grSeed : 1;
    if (!hillOrderSeed)
        hillOrderSeed = 1;
    for (i = hillCount - 1; i > 0; --i) {
        int j = kothRandRange(&hillOrderSeed, 0, i + 1);
        int tmp = hillOrder[i];
        hillOrder[i] = hillOrder[j];
        hillOrder[j] = tmp;
    }
#endif

    initialized = 1;
}

static int playerInsideHill(Player *player)
{
    if (!player)
        return 0;

    int activeIdx = kothGetActiveHillIndex();
    if (activeIdx < 0 || activeIdx >= hillCount)
        return 0;

    VECTOR delta;
    vector_subtract(delta, player->playerPosition, hills[activeIdx].position);
    // ignore height to keep circle flat on ground
    delta[2] = 0;
    float sqrDist = vector_sqrmag(delta);
    return sqrDist <= (KOTH_RING_RADIUS * KOTH_RING_RADIUS);
}

static void broadcastScore(int playerIdx)
{
    void *connection = netGetDmeServerConnection();
    if (!connection)
        return;

    KothScoreUpdate_t msg;
    msg.PlayerIdx = (char)playerIdx;
    msg.Score = (short)kothScores[playerIdx];
    msg.Padding = 0;

    netBroadcastCustomAppMessage(connection, CUSTOM_MSG_ID_KOTH_SCORE_UPDATE, sizeof(msg), &msg);
    lastBroadcastScore[playerIdx] = kothScores[playerIdx];
}

static void updateScores(void)
{
    Player **players = playerGetAll();
    int i;
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player *p = players[i];
        if (!p || playerIsDead(p) || p->vehicle)
            continue;

        if (playerInsideHill(p) && p->isLocal) {
            ++kothScores[p->mpIndex];
            if (kothScores[p->mpIndex] != lastBroadcastScore[p->mpIndex])
                broadcastScore(p->mpIndex);
        }
    }
}

static int kothGetScoreLimit(void)
{
    static const int SCORE_LIMIT_OPTIONS[] = {
        0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 750, 1000, 2000
    };

    int idx = kothConfig ? (int)kothConfig->grKothScoreLimit : 0;
    if (idx < 0 || idx >= (int)COUNT_OF(SCORE_LIMIT_OPTIONS))
        return SCORE_LIMIT_OPTIONS[0];

    return SCORE_LIMIT_OPTIONS[idx];
}

static int kothGetHillDurationMs(void)
{
    static const int HILL_DURATION_OPTIONS_MS[] = {
        KOTH_HILL_ACTIVE_MS,        // 60
        TIME_SECOND * 120,
        TIME_SECOND * 180,
        TIME_SECOND * 240,
        TIME_SECOND * 300
    };

    int idx = kothConfig ? (int)kothConfig->grKothHillDuration : 0;
    if (idx < 0 || idx >= (int)COUNT_OF(HILL_DURATION_OPTIONS_MS))
        idx = 0;

    return HILL_DURATION_OPTIONS_MS[idx];
}

static int kothFindLeader(int *outScore, int *outTie)
{
    GameSettings *gs = gameGetSettings();
    if (!gs)
        return -1;

    int useTeams = kothUseTeams();
    int leaderIdx = -1;
    int leaderScore = 0;
    int isTie = 0;
    GameData *gd = gameGetData();

    if (useTeams) {
        int teamScores[TEAM_MAX];
        char teamSeen[TEAM_MAX];
        memset(teamScores, 0, sizeof(teamScores));
        memset(teamSeen, 0, sizeof(teamSeen));
        int teamKills[TEAM_MAX];
        memset(teamKills, 0, sizeof(teamKills));

        Player **players = playerGetAll();
        int i;
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            Player *p = players[i];
            if (!p)
                continue;
            int team = gs->PlayerTeams[p->mpIndex];
            if (team < 0 || team >= TEAM_MAX)
                continue;
            teamSeen[team] = 1;
            teamScores[team] += kothScores[p->mpIndex];
            teamKills[team] += gd ? gd->playerStats.frag[p->mpIndex].kills : 0;
        }

        int t;
        for (t = 0; t < TEAM_MAX; ++t) {
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
            int score = kothScores[idx];
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

static int kothSetWinnerFields(int winnerIdx, int reason, int endGame)
{
    GameSettings *gs = gameGetSettings();
    GameOptions *go = gameGetOptions();
    GameData *gd = gameGetData();
    if (!gs || !go || !gd)
        return 0;

    int useTeams = go->GameFlags.MultiplayerGameFlags.Teams;
    int winnerId = winnerIdx;
    if (useTeams) {
        if (winnerId < 0 || winnerId >= TEAM_MAX)
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
    gameOverTriggered = 1;
    return 1;
}

static void kothDeclareWinner(int winnerIdx, int reason)
{
    if (!kothSetWinnerFields(winnerIdx, reason, 1))
        return;
}

static void runGameEnd(int reason)
{
    /*
     * Hooked gameEnd callsite: run custom logic once, then forward to the real gameEnd.
     */
    int forwardReason = reason;
#ifdef KOTH_DEBUG
    GameData *gd = gameGetData();
    int timeNow = gameGetTime();
    int timeStart = gd ? gd->timeStart : -1;
    int timeEnd = gd ? gd->timeEnd : -1;
    KOTH_LOG("[KOTH][DBG] runGameEnd reason=%d timeNow=%d timeStart=%d timeEnd=%d handled=%d\n", reason, timeNow, timeStart, timeEnd, gameEndHandled);
#endif

    if (!gameEndHandled) {
        gameEndHandled = 1;
        if (reason == GAME_END_TIME_UP) {
            int leaderScore = 0;
            int isTie = 0;
            int leaderIdx = kothFindLeader(&leaderScore, &isTie);
            KOTH_LOG("[KOTH][DBG] runGameEnd TIME_UP leaderIdx=%d leaderScore=%d isTie=%d\n", leaderIdx, leaderScore, isTie);
            if (leaderIdx >= 0 && !isTie) {
                // Mirror score-limit path to avoid base TIME_UP overriding the winner.
                kothSetWinnerFields(leaderIdx, GAME_END_TEAM_WIN, 0);
                KOTH_LOG("[KOTH][DBG] runGameEnd set winner idx=%d reason=%d (TEAM_WIN)\n", leaderIdx, GAME_END_TEAM_WIN);
                forwardReason = GAME_END_TEAM_WIN;
            }
        }
    }

    // Forward to original gameEnd.
    gameEnd(forwardReason);
}

static void kothCheckVictory(void)
{
    if (gameOverTriggered)
        return;
    if (!gameAmIHost())
        return;

    GameData *gd = gameGetData();
    if (!gd)
        return;

    int leaderScore = 0;
    int isTie = 0;
    int leaderIdx = kothFindLeader(&leaderScore, &isTie);

    int timeStart = gd ? gd->timeStart : 0;
    int timeEndRaw = gd ? gd->timeEnd : 0;
    // Treat timeEnd as a duration from timeStart; base game appears to keep duration rather than absolute end.
    int resolvedEnd = (timeStart > 0 && timeEndRaw > 0) ? (timeStart + timeEndRaw) : -1;
    int timerActive = resolvedEnd > timeStart;

    if (timerActive) {
        int now = gameGetTime();
        int timeLeft = resolvedEnd - now;
        static int lastTimerDebugSec = -1;
        int nowSec = now / 1000;
        if (timeLeft <= 2000 && nowSec != lastTimerDebugSec) {
#ifdef KOTH_DEBUG
            KOTH_LOG("[KOTH][DBG] timer expire check timeNow=%d timeStart=%d timeEndRaw=%d resolvedEnd=%d timeLeft=%d leaderIdx=%d leaderScore=%d isTie=%d\n", now, timeStart, timeEndRaw, resolvedEnd, timeLeft, leaderIdx, leaderScore, isTie);
            lastTimerDebugSec = nowSec;
            // Dump scoreboard snapshot to verify host view of scores and teams.
            GameSettings *gsDbg = gameGetSettings();
            if (gsDbg) {
                int p;
                for (p = 0; p < GAME_MAX_PLAYERS; ++p) {
                    if (!gsDbg->PlayerNames[p][0])
                        continue;
                    int team = gsDbg->PlayerTeams[p];
                    char dbgName[KOTH_NAME_MAX_LEN + 1];
                    kothCopyName(dbgName, gsDbg->PlayerNames[p]);
                    KOTH_LOG("[KOTH][DBG] scoreboard idx=%d team=%d name=%s score=%d\n", p, team, dbgName, kothScores[p]);
                }
            }
            if (!isTie && leaderIdx >= 0) {
                KOTH_LOG("[KOTH][DBG] timer expire would set winner idx=%d reason=%d (TEAM_WIN)\n", leaderIdx, GAME_END_TEAM_WIN);
            } else if (leaderIdx < 0) {
                KOTH_LOG("[KOTH][DBG] timer expired but no leader found\n");
            } else {
                // TODO: decide tie behavior (end game without winner vs. wait for hook)
            }
#endif
        }
        if (timeLeft <= 0)
            return;
    }

    if (leaderIdx < 0)
        return;

    int scoreLimit = kothGetScoreLimit();
    if (scoreLimit > 0 && leaderScore >= scoreLimit && !isTie) {
        kothDeclareWinner(leaderIdx, GAME_END_TEAM_WIN);
        return;
    }
}

static void drawHillAt(VECTOR center, u32 color, float *scroll)
{
    const int MAX_SEGMENTS = 64;
    const int MIN_SEGMENTS = 8;
    QuadDef quad[3];
    gfxSetupEffectTex(&quad[0], FX_VISIBOMB_HORIZONTAL_LINES, 0, 0x80);
    gfxSetupEffectTex(&quad[2], FX_CIRLCE_NO_FADED_EDGE, 0, 0x80);

    quad[0].uv[0] = (UV_t){0, 0};
    quad[0].uv[1] = (UV_t){0, 1};
    quad[0].uv[2] = (UV_t){1, 0};
    quad[0].uv[3] = (UV_t){1, 1};
    quad[1] = quad[0];
    quad[2] = quad[0];

    float uvOffset = 0;
    quad[0].uv[0].y += uvOffset;
    quad[0].uv[1].y -= uvOffset;
    quad[0].uv[2].y += uvOffset;
    quad[0].uv[3].y -= uvOffset;
    quad[1] = quad[0];

    int alphaOuterNear = (int)(0x00 * KOTH_RING_ALPHA_SCALE) & 0xFF;
    int alphaOuterFar = (int)(0x30 * KOTH_RING_ALPHA_SCALE);
    if (alphaOuterFar > 0xFF) alphaOuterFar = 0xFF;
    int alphaMidNear = (int)(0x50 * KOTH_RING_ALPHA_SCALE);
    if (alphaMidNear > 0xFF) alphaMidNear = 0xFF;
    int alphaMidFar = (int)(0x20 * KOTH_RING_ALPHA_SCALE);
    if (alphaMidFar > 0xFF) alphaMidFar = 0xFF;
    int alphaCenter = (int)(0x30 * KOTH_RING_ALPHA_SCALE);
    if (alphaCenter > 0xFF) alphaCenter = 0xFF;

    u32 baseRgb = color & 0x00FFFFFF;
    quad[0].rgba[0] = quad[0].rgba[1] = (alphaOuterNear << 24) | baseRgb;
    quad[0].rgba[2] = quad[0].rgba[3] = (alphaOuterFar << 24) | baseRgb;
    quad[1].rgba[0] = quad[1].rgba[1] = (alphaMidNear << 24) | baseRgb;
    quad[1].rgba[2] = quad[1].rgba[3] = (alphaMidFar << 24) | baseRgb;
    quad[2].rgba[0] = quad[2].rgba[1] = quad[2].rgba[2] = quad[2].rgba[3] = (alphaCenter << 24) | baseRgb;

    VECTOR xAxis = {KOTH_RING_RADIUS * 2, 0, 0, 0};
    VECTOR zAxis = {0, KOTH_RING_RADIUS * 2, 0, 0};
    VECTOR yAxis = {0, 0, KOTH_RING_HEIGHT, 0};
    VECTOR tempRight, tempUp, halfX, halfZ, vRadius, tempCenter;
    vector_scale(halfX, xAxis, .5);
    vector_scale(halfZ, zAxis, .5);
    float fRadius = vector_length(halfX);
    int signs[4][2] = {{1, -1}, {-1, -1}, {1, 1}, {-1, 1}};
    vector_normalize(yAxis, yAxis);

    vector_outerproduct(tempRight, yAxis, halfX);
    vector_normalize(tempRight, tempRight);
    vector_scale(tempRight, tempRight, 1);
    vector_scale(tempUp, yAxis, KOTH_RING_HEIGHT * 0.5f);

    float segmentSize = 1;
    int segments = (int)((2 * MATH_PI * fRadius) / segmentSize);
    float thetaStep = 2 * MATH_PI / clampf((float)segments, (float)MIN_SEGMENTS, (float)MAX_SEGMENTS);

    int k, i;
    for (k = 0; k < 2; ++k) {
        vector_copy(vRadius, halfX);
        for (i = 0; i < segments; ++i) {
            vector_add(tempCenter, center, vRadius);
            tempCenter[2] += k * KOTH_RING_HEIGHT;
            int j;
            for (j = 0; j < 4; ++j) {
                quad[k].point[j][0] = tempCenter[0] + signs[j][0] * tempRight[0] + signs[j][1] * tempUp[0];
                quad[k].point[j][1] = tempCenter[1] + signs[j][0] * tempRight[1] + signs[j][1] * tempUp[1];
                quad[k].point[j][2] = tempCenter[2] + signs[j][0] * tempRight[2] + signs[j][1] * tempUp[2];
                quad[k].point[j][3] = 1;
            }

            // Scroll texture vertically to keep the ring graphic oriented upward.
            quad[k].uv[0].y = quad[k].uv[1].y = 0 - *scroll;
            quad[k].uv[2].y = quad[k].uv[3].y = 1 - *scroll;

            gfxDrawQuad(quad[k], NULL);
            vector_rodrigues(vRadius, vRadius, yAxis, thetaStep);
            vector_outerproduct(tempRight, yAxis, vRadius);
            vector_normalize(tempRight, tempRight);
        }
    }
    *scroll += .007f;

    // floor quad
    VECTOR corners[4];
    vector_copy(corners[0], (VECTOR){center[0] - fRadius, center[1] - fRadius, center[2], 0});
    vector_copy(corners[1], (VECTOR){center[0] + fRadius, center[1] - fRadius, center[2], 0});
    vector_copy(corners[2], (VECTOR){center[0] + fRadius, center[1] + fRadius, center[2], 0});
    vector_copy(corners[3], (VECTOR){center[0] - fRadius, center[1] + fRadius, center[2], 0});

    QuadDef floorQuad;
    gfxSetupEffectTex(&floorQuad, FX_CIRLCE_NO_FADED_EDGE, 0, 0x80);
    floorQuad.uv[0] = (UV_t){0, 0};
    floorQuad.uv[1] = (UV_t){0, 1};
    floorQuad.uv[2] = (UV_t){1, 0};
    floorQuad.uv[3] = (UV_t){1, 1};
    floorQuad.rgba[0] = floorQuad.rgba[1] = floorQuad.rgba[2] = floorQuad.rgba[3] = (0x20 << 24) | baseRgb;
    vector_copy(floorQuad.point[0], corners[1]);
    vector_copy(floorQuad.point[1], corners[0]);
    vector_copy(floorQuad.point[2], corners[2]);
    vector_copy(floorQuad.point[3], corners[3]);
    gfxDrawQuad(floorQuad, NULL);
}

static KothHill_t *kothFindHill(Moby *moby)
{
    int i;
    for (i = 0; i < hillCount; ++i) {
        if (hills[i].moby == moby || hills[i].drawMoby == moby)
            return &hills[i];
    }
    return NULL;
}

static void kothEnsureCycleStart(void)
{
    if (hillCycleStartTime == 0 && hillCount > 0) {
        GameData *gd = gameGetData();
        if (gd && gd->timeStart > 0)
            hillCycleStartTime = gd->timeStart;
        else
            hillCycleStartTime = gameGetTime();
    }
}

static int kothGetActiveHillIndex(void)
{
    if (hillCount <= 0)
        return -1;

    kothEnsureCycleStart();

    int duration = kothGetHillDurationMs();
    if (duration <= 0)
        return 0;

    int elapsed = gameGetTime() - hillCycleStartTime;
    if (elapsed < 0)
        elapsed = 0;

    int idx = (elapsed / duration) % hillCount;
#ifdef KOTH_RANDOM_ORDER
    if (hillOrderCount <= 0 || idx >= hillOrderCount)
        return idx;
    return hillOrder[idx];
#else
    return idx;
#endif
}

static void drawHill(Moby *moby)
{
    u32 baseColor = 0x0080FF00; // green tint for hill marker
    KothHill_t *hill = kothFindHill(moby);
    float *scroll = hill ? &hill->scroll : NULL;
    float fallbackScroll = 0;
    if (!scroll)
        scroll = &fallbackScroll;
    drawHillAt(moby->position, baseColor, scroll);
}

static void hillUpdate(Moby *moby)
{
    gfxRegistserDrawFunction(&drawHill, moby);
}

static void drawHills(void)
{
    int activeIdx = kothGetActiveHillIndex();
#if KOTH_ENABLE_HILL_SYNC
    if (activeIdx != lastActiveHillIdx && gameAmIHost()) {
        kothBroadcastHillIndex(activeIdx);
        lastActiveHillIdx = activeIdx;
    }
#endif
    int i;
    for (i = 0; i < hillCount; ++i) {
        if (hills[i].drawMoby) {
            // Clear draw hooks on all hills; we’ll enable only the active one.
            hills[i].drawMoby->pUpdate = NULL;
            hills[i].drawMoby->drawn = 0;
            hills[i].drawMoby->opacity = 0;
        }
    }

    if (activeIdx >= 0 && activeIdx < hillCount) {
        if (hills[activeIdx].drawMoby) {
            hills[activeIdx].drawMoby->pUpdate = &hillUpdate;
            hills[activeIdx].drawMoby->drawn = 1;
        } else {
            drawHillAt(hills[activeIdx].position, 0x0080FF00, &hills[activeIdx].scroll);
        }
    }
}

static void drawHud(void)
{
    GameSettings *gs = gameGetSettings();
    if (!gs)
        return;

    int useTeams = kothUseTeams();

    typedef struct {
        int idx;
        int score;
    } Entry;
    Entry entries[GAME_MAX_PLAYERS > TEAM_MAX ? GAME_MAX_PLAYERS : TEAM_MAX];
    int count = 0;

    if (useTeams) {
        int teamScores[TEAM_MAX];
        char teamSeen[TEAM_MAX];
        memset(teamScores, 0, sizeof(teamScores));
        memset(teamSeen, 0, sizeof(teamSeen));

        Player **players = playerGetAll();
        int i;
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            Player *p = players[i];
            if (!p)
                continue;
            int team = gs->PlayerTeams[p->mpIndex];
            if (team < 0 || team >= TEAM_MAX)
                continue;
            teamSeen[team] = 1;
            teamScores[team] += kothScores[p->mpIndex];
        }

        int t;
        for (t = 0; t < TEAM_MAX; ++t) {
            if (!teamSeen[t])
                continue;
            entries[count].idx = t;
            entries[count].score = teamScores[t];
            ++count;
        }
    } else {
        char seen[GAME_MAX_PLAYERS];
        memset(seen, 0, sizeof(seen));
        Player **players = playerGetAll();
        int i;
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
            entries[count].idx = idx;
            entries[count].score = kothScores[idx];
            ++count;
        }
    }

    // sort desc by score (small N)
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        int i;
        for (i = 0; i < count - 1; ++i) {
            if (entries[i].score < entries[i + 1].score) {
                Entry tmp = entries[i];
                entries[i] = entries[i + 1];
                entries[i + 1] = tmp;
                swapped = 1;
            }
        }
    }

    float startX = 20;
    float startY = SCREEN_HEIGHT * 0.35f;
    float lineH = 16.0f;
    gfxScreenSpaceText(startX, startY - lineH, 1, 1, 0x80FFFFFF, kothTitle, -1, TEXT_ALIGN_TOPLEFT, FONT_BOLD);

    int i;
    for (i = 0; i < count; ++i) {
        int idx = entries[i].idx;
        char line[64];
        u32 color = 0x80FFFFFF;
        if (useTeams) {
            static const char *teamNames[TEAM_MAX] = {
                "Blue", "Red", "Green", "Orange", "Yellow", "Purple", "Aqua", "Pink"
            };
            const char *name = (idx >= 0 && idx < TEAM_MAX) ? teamNames[idx] : "Team";
            snprintf(line, sizeof(line), "%s: %d", name, entries[i].score);
            color = (0x80 << 24) | ((idx >= 0 && idx < TEAM_MAX) ? TEAM_COLORS[idx] : 0x00FFFFFF);
        } else {
            char nameBuf[KOTH_NAME_MAX_LEN + 1];
            kothCopyName(nameBuf, gs->PlayerNames[idx]);
            snprintf(line, sizeof(line), "%s: %d", nameBuf, entries[i].score);
            int team = gs->PlayerTeams[idx];
            color = (0x80 << 24) | ((team >= 0 && team < 8) ? TEAM_COLORS[team] : 0x00FFFFFF);
        }
        gfxScreenSpaceText(startX, startY + (i * lineH), 1, 1, color, line, -1, TEXT_ALIGN_TOPLEFT, FONT_DEFAULT);
    }
}

int kothOnReceiveScore(void *connection, void *data)
{
    KothScoreUpdate_t *msg = (KothScoreUpdate_t*)data;
    int idx = msg->PlayerIdx;
    if (idx < 0 || idx >= GAME_MAX_PLAYERS)
        return sizeof(KothScoreUpdate_t);

    kothScores[idx] = msg->Score;
    return sizeof(KothScoreUpdate_t);
}

#if KOTH_ENABLE_HILL_SYNC
static int kothOnReceiveHillSync(void *connection, void *data)
{
    KothHillSync_t *msg = (KothHillSync_t*)data;
    int idx = msg->HillIdx;
    if (idx < 0 || idx >= hillCount)
        return sizeof(KothHillSync_t);

    // Always snap to host's hill index/time; host is authoritative to recover from drift (ahead or behind).
    // Use elapsed time so we don't depend on host absolute clock.
    int elapsed = msg->ElapsedMs;
    if (elapsed < 0)
        elapsed = 0;
    hillCycleStartTime = gameGetTime() - elapsed;
    if (hillCycleStartTime < 0)
        hillCycleStartTime = 0;
    lastActiveHillIdx = idx;
    KOTH_LOG("[KOTH][DBG] hill sync received idx=%d localIdx=%d seed=%d timeStart=%d elapsed=%d", idx, kothGetActiveHillIndex(), (kothConfig ? kothConfig->grSeed : -1), gameGetData() ? gameGetData()->timeStart : -1, elapsed);
    return sizeof(KothHillSync_t);
}
#endif

void kothReset(void)
{
    // Clean up any spawned draw mobies to clear registered draw callbacks.
    int i;
    for (i = 0; i < hillCount; ++i) {
        if (hills[i].drawMoby) {
            mobyDestroy(hills[i].drawMoby);
            hills[i].drawMoby = NULL;
        }
    }

    initialized = 0;
    handlerInstalled = 0;
    gameEndHookInstalled = 0;
    hillCount = 0;
    nextScoreTickTime = 0;
    gameOverTriggered = 0;
    gameEndHandled = 0;
    hillCycleStartTime = 0;
    lastTimeStart = -1;
#if KOTH_ENABLE_HILL_SYNC
    lastActiveHillIdx = -1;
#endif
    lastSeed = 0;
#ifdef KOTH_RANDOM_ORDER
    hillOrderCount = 0;
    ++hillOrderShuffleNonce;
#endif
    memset(hills, 0, sizeof(hills));
    memset(kothScores, 0, sizeof(kothScores));
    memset(lastBroadcastScore, 0, sizeof(lastBroadcastScore));
}

void kothInit(void)
{
    if (!handlerInstalled) {
        netInstallCustomMsgHandler(CUSTOM_MSG_ID_KOTH_SCORE_UPDATE, &kothOnReceiveScore);
#if KOTH_ENABLE_HILL_SYNC
        netInstallCustomMsgHandler(CUSTOM_MSG_ID_KOTH_HILL_SYNC, &kothOnReceiveHillSync);
#endif
        handlerInstalled = 1;
    }

    // Disable base-game frag limit so KOTH doesn't end on kills.
    GameOptions *go = gameGetOptions();
    if (go) {
        go->GameFlags.MultiplayerGameFlags.FragLimit = 0;
    }

    // Cache header text for HUD.
    int scoreLimit = kothGetScoreLimit();
    if (scoreLimit > 0)
        snprintf(kothTitle, sizeof(kothTitle), "%d", scoreLimit);
    else
        snprintf(kothTitle, sizeof(kothTitle), "No limit");

    if (!gameEndHookInstalled) {
        // Choose callsite table based on teams flag; fallback to teams table if FFA entry missing.
        VariableAddress_t *hookTable = go && go->GameFlags.MultiplayerGameFlags.Teams ? &vaGameTimerEndHookDmTeams : &vaGameTimerEndHookDmNoTeams;
        u32 hook = GetAddress(hookTable);
        if (!hook && hookTable != &vaGameTimerEndHookDmTeams)
            hook = GetAddress(&vaGameTimerEndHookDmTeams);
        if (hook) {
            HOOK_JAL(hook, &runGameEnd);
            gameEndHookInstalled = 1;
        }
    }

    scanHillsOnce();
}

void kothTick(void)
{
    if (!isInGame())
        return;

    // Detect a new match via timeStart and fully reset state (scores, hills, seed).
    {
        GameData *gd = gameGetData();
        int timeStart = gd ? gd->timeStart : -1;
        int seedNow = kothConfig ? kothConfig->grSeed : 0;
        if ((timeStart > 0 && timeStart != lastTimeStart) || seedNow != lastSeed) {
            kothReset();
            lastTimeStart = timeStart;
            lastSeed = seedNow;
        }
    }

    kothInit();
    drawHills();
    drawHud();

    int gameTime = gameGetTime();
    if (nextScoreTickTime == 0)
        nextScoreTickTime = gameTime + KOTH_SCORE_TICK_MS;

    if (gameTime >= nextScoreTickTime) {
        updateScores();
#ifdef KOTH_DEBUG
        // KOTHDBUG: trace timer every tick (host only) to see countdown activity
        if (gameAmIHost()) {
            GameData *gd = gameGetData();
            int hasGd = gd != NULL;
            int startSet = gd && (gd->timeStart > 0);
            int endSet = gd && (gd->timeEnd > 0);
            // Treat timeEnd as a duration in ms from timeStart (base game may store duration, not absolute end).
            int resolvedEnd = (startSet && endSet) ? (gd->timeStart + gd->timeEnd) : -1;
            int endAfterStart = resolvedEnd > (startSet ? gd->timeStart : 0);
            int timerActive = hasGd && startSet && endAfterStart;
            int timeStart = gd ? gd->timeStart : -1;
            int timeEnd = gd ? gd->timeEnd : -1;
            int timeLeft = timerActive ? (resolvedEnd - gameTime) : -1;
            KOTH_LOG("[KOTH][DBG] tick timerActive=%d timeNow=%d hasGd=%d startSet=%d endSet=%d endAfterStart=%d timeStart=%d timeEnd=%d resolvedEnd=%d timeLeft=%d seed=%d hillIdx=%d\n",
                     timerActive, gameTime, hasGd, startSet, endSet, endAfterStart, timeStart, timeEnd, resolvedEnd, timeLeft,
                     (kothConfig ? kothConfig->grSeed : -1), kothGetActiveHillIndex());
        }
#endif
        nextScoreTickTime = gameTime + KOTH_SCORE_TICK_MS;
    }

    kothCheckVictory();
}

#if KOTH_ENABLE_HILL_SYNC
static void kothBroadcastHillIndex(int hillIdx)
{
    void *connection = netGetDmeServerConnection();
    if (!connection || hillIdx < 0 || hillIdx >= hillCount)
        return;

    KothHillSync_t msg;
    msg.HillIdx = (char)hillIdx;
    memset(msg.Padding, 0, sizeof(msg.Padding));
    // Send elapsed time on current hill so clients can reconstruct cycle start on their own clock.
    msg.ElapsedMs = gameGetTime() - hillCycleStartTime;
    netBroadcastCustomAppMessage(connection, CUSTOM_MSG_ID_KOTH_HILL_SYNC, sizeof(msg), &msg);
}
#endif
