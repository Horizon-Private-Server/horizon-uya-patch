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
#include <libuya/map.h>
#include <libuya/hud.h>
#include "messageid.h"
#include "config.h"
#include "include/koth.h"

#define KOTH_ENABLE_HILL_SYNC 1
#define KOTH_FADE_WARNING 1
#define KOTH_SIEGE_USE_BOLT_CRANK 1
#define KOTH_CUBOID_SCALE 1.0f
#define KOTH_RING_HEIGHT_SCALE 1.0f
#define KOTH_SIZE_OPTIONS 7

#define KOTH_HUD_SPRITE (SPRITE_HUD_BOLT)
#define KOTH_HUD_CONTAINER_ID (0xacdc0000)
#define KOTH_HUD_FRAME_ID (KOTH_HUD_CONTAINER_ID + 1)


// Set to 1 to enable verbose KOTH debugging at compile time.
#define KOTH_DEBUG 0
#if KOTH_DEBUG
#define KOTH_LOG(fmt, args...) printf(fmt, ##args)
#else
#define KOTH_LOG(fmt, args...) ((void)0)
#endif
#define KOTH_LOGVEC(label, v) KOTH_LOG(label ": %d,%d,%d,%d\n", (int)(v)[0], (int)(v)[1], (int)(v)[2], (int)(v)[3])

#ifndef TEAM_MAX
#define TEAM_MAX 8
#endif

//TODO add height limits for scoring, currently unlimited. 
#define KOTH_MAX_HILLS         (8)
#define KOTH_RING_RADIUS       (10.0f)
#define KOTH_RING_HEIGHT       (1.0f)
#define KOTH_RING_ALPHA_SCALE  (1.0f)
#define KOTH_SCORE_TICK_MS     (TIME_SECOND)
#define KOTH_HILL_ACTIVE_MS    (TIME_SECOND * 60)
#define KOTH_NAME_MAX_LEN      (7)
#define KOTH_OCLASS_CUSTOM     (0x3000) // custom hill moby oclass
#define KOTH_MAX_CUSTOM_CUBOIDS (32)

#ifndef KOTH_RING_WALL_FX
#define KOTH_RING_WALL_FX FX_VISIBOMB_HORIZONTAL_LINES
//#define KOTH_RING_WALL_FX FX_TIRE_TRACKS
#endif

//TODO 1.1 is best at default scaling, but is a bit much if scaled 4x. Using less but may want to dynamically adjust 
#define KOTH_SCORE_MARGIN_CIRCLE 1.0f // TODO: restore small margin (was 1.07f) if boundary leniency is desired

// Toggle how the active hill is selected each rotation window
// Define KOTH_RANDOM_ORDER to cycle hills in a deterministic randomized order without replacement.
// Leave undefined to rotate in fixed index order.
#define KOTH_RANDOM_ORDER

// Exposed from koth/main.c for map source selection.
extern int isCustomMap;

typedef struct KothHill {
    VECTOR position;
    Moby *moby;
    Moby *drawMoby;
    float scroll;
    float radiusX;
    float radiusY;
    Cuboid cuboid;
    int isCircle;
    int drawAtMidpoint; // 1 = center of cuboid, 0 = base of cuboid
    // Derived footprint (currently uniform circle; keep both axes for future ellipse support).
    float footprintRx;
    float footprintRy;
    VECTOR axisX;          // normalized local X in world space
    VECTOR axisY;          // normalized local Y in world space
    VECTOR axisUp;         // normalized local Z in world space
    float halfHeight;      // half-height derived from cuboid up axis length
    int axesComputed;      // 1 if axes are valid
    int preferRotYaw;      // 1 to prefer cuboid.rot.z for yaw (custom hills)
#ifdef KOTH_RANDOM_ORDER
    int orderIdx;
#endif
} KothHill_t;

static KothHill_t hills[KOTH_MAX_HILLS];
static int hillCount = 0;
static int initialized = 0;
static int gameEndHookInstalled = 0;
static int gameEndHandled = 0;
static int handlerInstalled = 0;
static int nextScoreTickTime = 0;
static int gameOverTriggered = 0;
static int hillCycleStartTime = 0;
static int lastTimeStart = -1;
#if KOTH_ENABLE_HILL_SYNC
static int lastActiveHillIdx = -1;
#endif
static int hillSizeIdx = 0; // synced hill size index
static const float KOTH_HILL_SCALE_TABLE[KOTH_SIZE_OPTIONS] = {1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f};
static float hillScale = 1.0f; // default scale (idx 0)
static int kothContestedStopsScore = 0;
static float kothScrollSpeedMultiplier = 1.0f;
static float kothHillAlphaScale = 1.0f;
static float kothScrollStep = 0.007f;
static int kothAlphaNearBase = 0x80;
static int kothAlphaFarBase = 0x50;
static int kothAlphaFloorBase = 0x40;
static int kothRingWallFx = KOTH_RING_WALL_FX;
static int kothUserCfgPollTimeMs = -TIME_SECOND;
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

static void kothApplyScaleToHill(KothHill_t *hill)
{
    if (!hill)
        return;
    float baseX = hill->radiusX;
    float baseY = hill->radiusY;
    if (baseX <= 0) baseX = KOTH_RING_RADIUS;
    if (baseY <= 0) baseY = KOTH_RING_RADIUS;

    if (hill->isCircle) {
        // Circle uses the larger axis for a uniform footprint.
        float base = baseX;
        if (baseY > base)
            base = baseY;
        float scaled = base * hillScale;
        if (scaled <= 0)
            scaled = KOTH_RING_RADIUS * hillScale;
        hill->footprintRx = scaled;
        hill->footprintRy = scaled;
    } else {
        // Rectangle: keep independent axes, ignore rotation.
        float scaledX = baseX * hillScale;
        float scaledY = baseY * hillScale;
        if (scaledX <= 0) scaledX = KOTH_RING_RADIUS * hillScale;
        if (scaledY <= 0) scaledY = KOTH_RING_RADIUS * hillScale;
        hill->footprintRx = scaledX;
        hill->footprintRy = scaledY;
    }
}

static void kothOnHillSizeUpdated(void)
{
    if (hillScale <= 0)
        hillScale = 1.0f;

    int i;
    for (i = 0; i < hillCount; ++i) {
        kothApplyScaleToHill(&hills[i]);
    }
}
static int kothScores[GAME_MAX_PLAYERS];
static int lastBroadcastScore[GAME_MAX_PLAYERS];
static PatchGameConfig_t *kothConfig = NULL;
static int lastSeed = 0;
static int kothInitLogged = 0;

typedef struct KothCustomHillPVar {
    int cuboidRefs[KOTH_MAX_CUSTOM_CUBOIDS];
} KothCustomHillPVar_t;

typedef struct KothVanillaHillSet {
    const Cuboid *cuboids;
    int count;
} KothVanillaHillSet_t;

// Vanilla hill cuboids (indexed by MAP_ID_* - MAP_ID_BAKISI).
static const Cuboid VANILLA_HILLS_BAKISI[] = {
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {373.6029f, 414.3022f, 203.8417f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {519.58356f, 398.7586f, 201.38f, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {35, 0, 0, 0},
            {0, 35, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {409.012f, 567.944f, 201.250f, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {18, 0, 0, 0},
            {0, 18, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {281.9309f, 429.3058f, 201.3012f, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {325.4687f, 298.8129f, 201.1440f, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {567.0633f, 259.2868f, 202.3434f, 1},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

static const Cuboid VANILLA_HILLS_HOVEN[] = {
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {352.0844f, 334.7871f, 67.0f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {339.51f, 240.437f, 66.4f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {260.38f, 251.191f, 63.68f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {172.397f, 251.956f, 67.25f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {166.521f, 164.01f, 66.20f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

static const Cuboid VANILLA_HILLS_OUTPOST[] = {
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {327.71f, 257.880f, 101.35f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {35, 0, 0, 0},
            {0, 35, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {478.67f, 191.556f, 102.10f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {509.34f, 248.783f, 101.1f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {389.9045f, 236.870f, 102.10f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {373.82f, 285.54f, 101.1f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

static const Cuboid VANILLA_HILLS_KORGON[] = {
    {
        .matrix = {
            {33, 0, 0, 0},
            {0, 33, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {359.3894f, 427.6866f, 97.0f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {322.8834f, 337.7596f, 97.1f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {395.0782f, 298.7676f, 97.0844f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {27, 0, 0, 0},
            {0, 27, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {424.8894f, 335.7746f, 97.1f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {215.4736f, 338.2934f, 97.1f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {283.20f, 291.9809f, 97.3f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

static const Cuboid VANILLA_HILLS_METRO[] = {
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 11, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {755.4391f, 339.3310f, 338.8f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0.8125f, 0}
    },
    {
        .matrix = {
            {66.915f, 0, 0, 0},
            {0, 66.915f, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {797.5479f, 296.7849f, 336.597f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {66.915f, 0, 0, 0},
            {0, 66.915f, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {712.8783f, 381.7458f, 336.597f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {18, 0, 0, 0},
            {0, 18, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {773.9068f, 445.8956f, 336.4594f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {18, 0, 0, 0},
            {0, 18, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {737.2782f, 231.7361f, 336.4694f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {707.1735f, 290.8173f, 329.2778f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {803.4766f, 387.5046f, 329.2790f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

static const Cuboid VANILLA_HILLS_BWCITY[] = {
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {226.9902f, 366.6711f, 81.6625f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {25, 0, 0, 0},
            {0, 25, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {209.0136f, 171.8414f, 81.6825f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {219.9686f, 235.8673f, 87.4906f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {219.6191f, 297.8083f, 87.4906f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

static const Cuboid VANILLA_HILLS_COMMAND[] = {
    {
        .matrix = {
            {40, 0, 0, 0},
            {0, 40, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {330.1714f, 365.327f, 35.0198f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

static const Cuboid VANILLA_HILLS_BWDOCKS[] = {
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {211.7765f, 191.4453f, 100.4812f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {261.8332f, 191.4044f, 100.7406f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {261.8378f, 263.7712f, 98.3135f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {30, 0, 0, 0},
            {0, 30, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {212.0f, 263.6f, 106.3031f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

static const Cuboid VANILLA_HILLS_AQUATOS[] = {
    {
        .matrix = {
            {16, 0, 0, 0},
            {0, 16, 0, 0},
            {0, 0, 1, 0}
        },
        .pos = {400.9128f, 336.8955f, 941.5f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
};

static const Cuboid VANILLA_HILLS_MARCADIA[] = {
    {
        .matrix = {
            {10, 0, 0, 0},
            {0, 10, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {538.2250f, 843.2510f, 116.1824f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {20, 0, 0, 0},
            {0, 20, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {502.108f, 864.6436f, 117.0f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {478.471f, 869.456f, 117.1281f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {450.248f, 889.508f, 117.0f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    },
    {
        .matrix = {
            {15, 0, 0, 0},
            {0, 15, 0, 0},
            {0, 0, 2, 0}
        },
        .pos = {433.1663f, 862.41f, 116.9276f, 0},
        .imatrix = 0,
        .rot = {0, 0, 0, 0}
    }
};

static const KothVanillaHillSet_t KOTH_VANILLA_HILLS[] = {
    { VANILLA_HILLS_BAKISI,   (int)(sizeof(VANILLA_HILLS_BAKISI) / sizeof(VANILLA_HILLS_BAKISI[0])) },
    { VANILLA_HILLS_HOVEN,    (int)(sizeof(VANILLA_HILLS_HOVEN) / sizeof(VANILLA_HILLS_HOVEN[0])) },
    { VANILLA_HILLS_OUTPOST,  (int)(sizeof(VANILLA_HILLS_OUTPOST) / sizeof(VANILLA_HILLS_OUTPOST[0])) },
    { VANILLA_HILLS_KORGON,   (int)(sizeof(VANILLA_HILLS_KORGON) / sizeof(VANILLA_HILLS_KORGON[0])) },
    { VANILLA_HILLS_METRO,    (int)(sizeof(VANILLA_HILLS_METRO) / sizeof(VANILLA_HILLS_METRO[0])) },
    { VANILLA_HILLS_BWCITY,   (int)(sizeof(VANILLA_HILLS_BWCITY) / sizeof(VANILLA_HILLS_BWCITY[0])) },
    { VANILLA_HILLS_COMMAND,  (int)(sizeof(VANILLA_HILLS_COMMAND) / sizeof(VANILLA_HILLS_COMMAND[0])) },
    { VANILLA_HILLS_BWDOCKS,  (int)(sizeof(VANILLA_HILLS_BWDOCKS) / sizeof(VANILLA_HILLS_BWDOCKS[0])) },
    { VANILLA_HILLS_AQUATOS,  (int)(sizeof(VANILLA_HILLS_AQUATOS) / sizeof(VANILLA_HILLS_AQUATOS[0])) },
    { VANILLA_HILLS_MARCADIA, (int)(sizeof(VANILLA_HILLS_MARCADIA) / sizeof(VANILLA_HILLS_MARCADIA[0])) },
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

static float clampf(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

static int clampi(int value, int min, int max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Derive yaw for a hill using its cuboid basis; optionally prefer rot.z (for custom maps).
static void kothComputeHillAxes(KothHill_t *hill)
{
    if (!hill)
        return;

    VECTOR x = {1,0,0,0};
    VECTOR y = {0,1,0,0};
    VECTOR z = {0,0,1,0};

    vector_copy(x, hill->cuboid.matrix.v0);
    vector_copy(y, hill->cuboid.matrix.v1);
    vector_copy(z, hill->cuboid.matrix.v2);

    float lenX = vector_length(x);
    float lenY = vector_length(y);
    float lenZ = vector_length(z);
    if (lenX < 0.0001f) {
        vector_copy(x, hill->cuboid.imatrix.v0);
        lenX = vector_length(x);
    }
    if (lenY < 0.0001f) {
        vector_copy(y, hill->cuboid.imatrix.v1);
        lenY = vector_length(y);
    }
    if (lenZ < 0.0001f) {
        vector_copy(z, hill->cuboid.imatrix.v2);
        lenZ = vector_length(z);
    }

    // Preserve raw length for height/anchoring from the up axis.
    float rawHeight = lenZ;

    if (lenX > 0.0001f) vector_scale(x, x, 1.0f / lenX); else vector_copy(x, (VECTOR){1,0,0,0});
    if (lenY > 0.0001f) vector_scale(y, y, 1.0f / lenY); else vector_copy(y, (VECTOR){0,1,0,0});
    if (lenZ > 0.0001f) vector_scale(z, z, 1.0f / lenZ); else vector_copy(z, (VECTOR){0,0,1,0});

    vector_copy(hill->axisX, x);
    vector_copy(hill->axisY, y);
    vector_copy(hill->axisUp, z);
    if (rawHeight < 2.0f)
        rawHeight = 2.0f; // clamp minimum raw height so scaled halfHeight doesn't get too small
    hill->halfHeight = (rawHeight > 0.0001f) ? (rawHeight * KOTH_RING_HEIGHT_SCALE * 0.5f) : 0.0f;
    hill->axesComputed = 1;
}

static void kothGetHillYaw(const KothHill_t *hill, float *cosYaw, float *sinYaw)
{
    float c = 1.0f, s = 0.0f;
    if (hill) {
        if (!hill->axesComputed)
            kothComputeHillAxes((KothHill_t*)hill);

        // Derive yaw from the X axis projected onto the XY plane.
        float x = hill->axisX[0];
        float y = hill->axisX[1];
        if (x != 0.0f || y != 0.0f) {
            float ang = atan2f(y, x);
            c = cosf(ang);
            s = sinf(ang);
        }
    }
    if (cosYaw) *cosYaw = c;
    if (sinYaw) *sinYaw = s;
}

#if 0 // unused helper kept for reference
static void kothBuildSyntheticCuboid(KothHill_t *hill, VECTOR pos, float radius)
{
    if (!hill)
        return;
    float r = radius > 0 ? radius : KOTH_RING_RADIUS;
    vector_copy(hill->cuboid.matrix.v0, (VECTOR){r * 2, 0, 0, 0});
    vector_copy(hill->cuboid.matrix.v1, (VECTOR){0, r * 2, 0, 0});
    vector_copy(hill->cuboid.matrix.v2, (VECTOR){0, 0, 1, 0});
    vector_copy(hill->cuboid.pos, pos);
    vector_copy(hill->cuboid.rot, (VECTOR){0,0,0,0});
    MATRIX tmp;
    matrix_unit(tmp);
    memcpy(&hill->cuboid.imatrix, tmp, sizeof(mtx3));
    hill->isCircle = 1;
}
#endif

#if 0 // verbose cuboid logger (disabled)
static void kothLogCuboid(const char *label, KothHill_t *hill)
{
#if KOTH_DEBUG
    if (!hill) return;
    KOTH_LOG("%s radii baseX=%d baseY=%d footprintX=%d footprintY=%d isCircle=%d\n",
             label,
             (int)hill->radiusX, (int)hill->radiusY,
             (int)hill->footprintRx, (int)hill->footprintRy,
             hill->isCircle);
    KOTH_LOGVEC("[KOTH][DBG] cuboid v0", hill->cuboid.matrix.v0);
    KOTH_LOGVEC("[KOTH][DBG] cuboid v1", hill->cuboid.matrix.v1);
    KOTH_LOGVEC("[KOTH][DBG] cuboid v2", hill->cuboid.matrix.v2);
    KOTH_LOGVEC("[KOTH][DBG] cuboid pos", hill->cuboid.pos);
    KOTH_LOGVEC("[KOTH][DBG] cuboid rot", hill->cuboid.rot);
#endif
}
#endif

static void kothEnsureCycleStart(void);
static int kothGetActiveHillIndex(void);
static int kothUseTeams(void);
#if KOTH_ENABLE_HILL_SYNC
static void kothBroadcastHillIndex(int hillIdx);
#endif
static void kothOnHillSizeUpdated(void);
static int kothRespawnDistanceIsCustom(void);
static float kothGetInsideRespawnDistance(void);
static float kothGetOutsideRespawnDistance(void);
static float kothSelectRespawnDistance(Player *player);
static void kothUpdateRespawnDistance(Player *player);
static void kothUpdateRespawnDistanceForLocals(void);
static int kothHillIsContested(void);
static u32 kothGetActiveHillColor(void);
static float kothGetBlinkScale(void);

static float kothMapScrollSpeed(char value)
{
    static const float options[] = {1.0f, 0.5f, 0.25f, 0.0f};
    int idx = value;
    if (idx < 0 || idx >= (int)(sizeof(options) / sizeof(options[0])))
        idx = 0;
    return options[idx];
}

static float kothMapHillAlphaScale(char value)
{
    static const float options[] = {1.0f, 0.75f, 0.5f, 0.25f, 0.10f};
    int idx = value;
    if (idx < 0 || idx >= (int)(sizeof(options) / sizeof(options[0])))
        idx = 0;
    return options[idx];
}

void kothSetUserConfig(PatchConfig_t *config)
{
    int now = gameGetTime();
    if (now - kothUserCfgPollTimeMs < (TIME_SECOND >> 1))
        return;
    kothUserCfgPollTimeMs = now;

    kothScrollSpeedMultiplier = kothMapScrollSpeed(config ? config->kothScrollSpeed : 0);
    kothScrollStep = 0.007f * kothScrollSpeedMultiplier;

    kothHillAlphaScale = kothMapHillAlphaScale(config ? config->kothHillTransparency : 0);
    kothAlphaNearBase = (int)(0x80 * kothHillAlphaScale);
    kothAlphaFarBase = (int)(0x50 * kothHillAlphaScale);
    kothAlphaFloorBase = (int)(0x40 * kothHillAlphaScale);
    if (kothAlphaNearBase > 0xFF) kothAlphaNearBase = 0xFF;
    if (kothAlphaFarBase > 0xFF) kothAlphaFarBase = 0xFF;
    if (kothAlphaFloorBase > 0xFF) kothAlphaFloorBase = 0xFF;

    // Clamp FX id; fall back to default wall effect on invalid values.
    int fxId = config ? (int)config->kothHillFxId : KOTH_RING_WALL_FX;
    if (fxId < 0 || fxId > 105)
        fxId = KOTH_RING_WALL_FX;
    kothRingWallFx = fxId;
}

void kothSetConfig(PatchGameConfig_t *config)
{
    kothConfig = config;
    int seedRaw = config ? config->grSeed : 0;
    // Hill size is now sent explicitly.
    int sizeIdx = config ? config->grKothHillSizeIdx : 0;
    if (sizeIdx < 0 || sizeIdx >= KOTH_SIZE_OPTIONS)
        sizeIdx = 0;
    hillSizeIdx = sizeIdx;
    hillScale = KOTH_HILL_SCALE_TABLE[hillSizeIdx];
    // Seed no longer carries size; keep lower bits for randomness if needed.
    lastSeed = seedRaw & 0x0FFFFFFF;
    kothContestedStopsScore = config ? config->grKothContestedStopsScore : 0;
    kothOnHillSizeUpdated();
#if KOTH_DEBUG
    KOTH_LOG("[KOTH][DBG] config set sizeIdx=%d scale=%d seedLow=0x%X\n", hillSizeIdx, (int)hillScale, lastSeed);
#endif
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

static void scanHillsOnce(void)
{
    if (initialized)
        return;

    memset(hills, 0, sizeof(hills));
    hillCount = 0;
    int source = 0; // 1=custom, 2=siege, 3=vanilla

    // On vanilla maps, prefer the hard-coded hill set.
    if (!isCustomMap) {
        GameSettings *gs = gameGetSettings();
        int mapId = gs ? gs->GameLevel : GAME_MAP_ID;
        if (mapId >= MAP_ID_BAKISI && mapId <= MAP_ID_MARCADIA) {
            int idx = mapId - MAP_ID_BAKISI;
            const KothVanillaHillSet_t *set = &KOTH_VANILLA_HILLS[idx];
            int c;
            for (c = 0; c < set->count && hillCount < KOTH_MAX_HILLS; ++c) {
                const Cuboid *cub = &set->cuboids[c];
                if (!cub)
                    continue;
                float rx = vector_length(cub->matrix.v0) * 0.5f * KOTH_CUBOID_SCALE;
                float ry = vector_length(cub->matrix.v1) * 0.5f * KOTH_CUBOID_SCALE;
                if (rx <= 0 || ry <= 0)
                    continue;

                vector_copy(hills[hillCount].position, cub->pos);
                hills[hillCount].position[3] = 1;
                    vector_copy(hills[hillCount].cuboid.matrix.v0, cub->matrix.v0);
                    vector_copy(hills[hillCount].cuboid.matrix.v1, cub->matrix.v1);
                    vector_copy(hills[hillCount].cuboid.matrix.v2, cub->matrix.v2);
                    vector_copy(hills[hillCount].cuboid.pos, cub->pos);
                    vector_copy(hills[hillCount].cuboid.rot, cub->rot);
                    hills[hillCount].cuboid.imatrix = cub->imatrix;
                    hills[hillCount].radiusX = rx;
                    hills[hillCount].radiusY = ry;
                    float diff = rx - ry;
                    if (diff < 0) diff = -diff;
                    hills[hillCount].isCircle = diff < 0.1f;
                    kothApplyScaleToHill(&hills[hillCount]);
                    hills[hillCount].moby = NULL;
                    hills[hillCount].preferRotYaw = 0;
                    kothComputeHillAxes(&hills[hillCount]);
                    hills[hillCount].scroll = 0;
                    hills[hillCount].drawAtMidpoint = 0;
                    hills[hillCount].drawMoby = mobySpawn(0x1c0d, 0);
                    if (hills[hillCount].drawMoby) {
                        vector_copy(hills[hillCount].drawMoby->position, hills[hillCount].position);
                        hills[hillCount].drawMoby->updateDist = -1;
                        hills[hillCount].drawMoby->drawn = 1;
                    hills[hillCount].drawMoby->opacity = 0;
                    hills[hillCount].drawMoby->drawDist = 0x00;
                    hills[hillCount].drawMoby->pUpdate = NULL; // set later
                }
#ifdef KOTH_DEBUG
                KOTH_LOG("[KOTH][DBG] hill[%d] source=vanilla mapId=%d pos=(%d,%d,%d)\n",
                         hillCount, mapId,
                         (int)hills[hillCount].position[0], (int)hills[hillCount].position[1], (int)hills[hillCount].position[2]);
#endif
                ++hillCount;
            }
            if (hillCount > 0)
                source = 3;
        }
    }

    Moby* moby = mobyListGetStart();
    Moby* mobyEnd = mobyListGetEnd();
    int foundCustom = 0;
    if (source != 3) {
    while (moby < mobyEnd && hillCount < KOTH_MAX_HILLS) {
        if (moby->oClass == KOTH_OCLASS_CUSTOM) {
            // One custom hill moby defines multiple cuboids; emit a hill per valid cuboid. (OCLASS 3000) 
//TODO we may want to scale the hill based off of the cuboid size (and other transformations?) 
            if (moby->pVar) {
                KothCustomHillPVar_t *hp = (KothCustomHillPVar_t *)moby->pVar;
                int spCount = spawnPointGetCount();
                int c;
                for (c = 0; c < KOTH_MAX_CUSTOM_CUBOIDS && hillCount < KOTH_MAX_HILLS; ++c) {
                    int idx = hp->cuboidRefs[c];
                    if (idx < 0 || idx >= spCount)
                        continue;
                    Cuboid *cub = spawnPointGet(idx);
                    if (!cub)
                        continue;

                    foundCustom = 1;
                    vector_copy(hills[hillCount].position, cub->pos);
                    hills[hillCount].position[3] = 1;
                    // Copy full cuboid so we can derive draw/score inputs uniformly.
                    vector_copy(hills[hillCount].cuboid.matrix.v0, cub->matrix.v0);
                    vector_copy(hills[hillCount].cuboid.matrix.v1, cub->matrix.v1);
                    vector_copy(hills[hillCount].cuboid.matrix.v2, cub->matrix.v2);
                    vector_copy(hills[hillCount].cuboid.pos, cub->pos);
                    vector_copy(hills[hillCount].cuboid.rot, cub->rot);
                    hills[hillCount].cuboid.imatrix = cub->imatrix;
                    hills[hillCount].isCircle = 0;

                    // Derive radii from cuboid scale (basis vector lengths).
                    // Custom maps author scale as full width; do not halve here.
                    hills[hillCount].radiusX = vector_length(cub->matrix.v0) * KOTH_CUBOID_SCALE;
                    hills[hillCount].radiusY = vector_length(cub->matrix.v1) * KOTH_CUBOID_SCALE;
                    if (hills[hillCount].radiusX <= 0) hills[hillCount].radiusX = KOTH_RING_RADIUS;
                    if (hills[hillCount].radiusY <= 0) hills[hillCount].radiusY = KOTH_RING_RADIUS;
                    // Choose circle when axes are effectively equal.
                    float diff = hills[hillCount].radiusX - hills[hillCount].radiusY;
                    if (diff < 0) diff = -diff;
                    hills[hillCount].isCircle = diff < 0.1f;
                    kothApplyScaleToHill(&hills[hillCount]);
                    hills[hillCount].moby = moby;
                    hills[hillCount].preferRotYaw = 1;
                    kothComputeHillAxes(&hills[hillCount]);
                    hills[hillCount].scroll = 0;
                    hills[hillCount].drawAtMidpoint = 0;
                    if (hills[hillCount].radiusX <= 0) hills[hillCount].radiusX = KOTH_RING_RADIUS;
                    if (hills[hillCount].radiusY <= 0) hills[hillCount].radiusY = KOTH_RING_RADIUS;
                    hills[hillCount].drawMoby = mobySpawn(0x1c0d, 0);
                    if (hills[hillCount].drawMoby) {
                        vector_copy(hills[hillCount].drawMoby->position, hills[hillCount].position);
                        hills[hillCount].drawMoby->updateDist = -1;
                        hills[hillCount].drawMoby->drawn = 1;
                        hills[hillCount].drawMoby->opacity = 0;
                        hills[hillCount].drawMoby->drawDist = 0x00;
                        hills[hillCount].drawMoby->pUpdate = NULL; // set later
#ifdef KOTH_DEBUG
                    } else {
                        KOTH_LOG("[KOTH][DBG] failed to spawn draw moby for custom hill cuboid=%d\n", idx);
#endif
                    }
#ifdef KOTH_DEBUG
                    KOTH_LOG("[KOTH][DBG] hill[%d] source=custom mobyIdx=0x%04X pos=(%d,%d,%d) cuboid=%d\n",
                             hillCount, moby->oClass,
                             (int)hills[hillCount].position[0], (int)hills[hillCount].position[1], (int)hills[hillCount].position[2], idx);
#endif
                    ++hillCount;
                }
            }
        }
        ++moby;
    }
    }

    // If no custom hills found, fall back to siege nodes.
    if (source != 3 && !foundCustom) {
        moby = mobyListGetStart();
        mobyEnd = mobyListGetEnd();
        while (moby < mobyEnd && hillCount < KOTH_MAX_HILLS) {
            if (moby->oClass == MOBY_ID_SIEGE_NODE) {
#if KOTH_SIEGE_USE_BOLT_CRANK
                // Prefer the bolt crank attached to this node (pVar + 0x3c) for positioning.
                int usedCrank = 0;
                if (moby->pVar) {
                    int crankIdx = *(int*)(moby->pVar + 0x3c);
                    Moby *listStart = mobyListGetStart();
                    Moby *listEnd = mobyListGetEnd();
                    Moby *crank = listStart + crankIdx;
                    if (crank >= listStart && crank < listEnd && crank->oClass == MOBY_ID_BOLT_CRANK) {
                        vector_copy(hills[hillCount].position, crank->position);
                        hills[hillCount].position[3] = 1;
                        usedCrank = 1;
#ifdef KOTH_DEBUG
                        KOTH_LOG("[KOTH][DBG] siege hill using bolt crank idx=%d pos=(%d,%d,%d)\n",
                                 crankIdx, (int)hills[hillCount].position[0], (int)hills[hillCount].position[1], (int)hills[hillCount].position[2]);
#endif
                    }
                }
                if (!usedCrank) {
                    vector_copy(hills[hillCount].position, moby->position);
                    hills[hillCount].position[3] = 1;
#ifdef KOTH_DEBUG
                    KOTH_LOG("[KOTH][DBG] siege hill using node pos=(%d,%d,%d)\n",
                             (int)hills[hillCount].position[0], (int)hills[hillCount].position[1], (int)hills[hillCount].position[2]);
#endif
                }
#else
                vector_copy(hills[hillCount].position, moby->position);
                hills[hillCount].position[3] = 1;
#endif
                hills[hillCount].isCircle = 1;
                hills[hillCount].radiusX = KOTH_RING_RADIUS;
                hills[hillCount].radiusY = KOTH_RING_RADIUS;
                kothApplyScaleToHill(&hills[hillCount]);
                hills[hillCount].preferRotYaw = 0;
                kothComputeHillAxes(&hills[hillCount]);
                hills[hillCount].drawAtMidpoint = 0;
#ifdef KOTH_DEBUG
                KOTH_LOG("[KOTH][DBG] hill[%d] source=siege pos=(%d,%d,%d) radius=%d\n",
                         hillCount,
                         (int)hills[hillCount].position[0], (int)hills[hillCount].position[1], (int)hills[hillCount].position[2],
                         (int)hills[hillCount].radiusX);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid v0", hills[hillCount].cuboid.matrix.v0);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid v1", hills[hillCount].cuboid.matrix.v1);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid v2", hills[hillCount].cuboid.matrix.v2);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid pos", hills[hillCount].cuboid.pos);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid rot", hills[hillCount].cuboid.rot);
#endif
                hills[hillCount].moby = moby;
                hills[hillCount].scroll = 0;
                hills[hillCount].drawMoby = mobySpawn(0x1c0d, 0);
                if (hills[hillCount].drawMoby) {
                    vector_copy(hills[hillCount].drawMoby->position, hills[hillCount].position);
                    hills[hillCount].drawMoby->updateDist = -1;
                    hills[hillCount].drawMoby->drawn = 1;
                    hills[hillCount].drawMoby->opacity = 0;
                    hills[hillCount].drawMoby->drawDist = 0x00;
                    hills[hillCount].drawMoby->pUpdate = NULL; // set later
#ifdef KOTH_DEBUG
                } else {
                    KOTH_LOG("[KOTH][DBG] failed to spawn draw moby for siege hill moby=0x%04X\n", moby->oClass);
#endif
                }
#ifdef KOTH_DEBUG
                KOTH_LOG("[KOTH][DBG] hill[%d] source=siege pos=(%d,%d,%d) radius=%d\n",
                         hillCount, (int)hills[hillCount].position[0], (int)hills[hillCount].position[1], (int)hills[hillCount].position[2],
                         (int)hills[hillCount].radiusX);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid v0", hills[hillCount].cuboid.matrix.v0);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid v1", hills[hillCount].cuboid.matrix.v1);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid v2", hills[hillCount].cuboid.matrix.v2);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid pos", hills[hillCount].cuboid.pos);
                KOTH_LOGVEC("[KOTH][DBG] siege cuboid rot", hills[hillCount].cuboid.rot);
#endif
                ++hillCount;
            }
            ++moby;
        }
    }
    if (source == 0) {
        if (foundCustom)
            source = 1;
        else if (hillCount > 0)
            source = 2;
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
#ifdef KOTH_DEBUG
    const char *sourceStr = "unknown";
    if (source == 1) sourceStr = "custom";
    else if (source == 2) sourceStr = "siege";
    else if (source == 3) sourceStr = "vanilla";
    KOTH_LOG("[KOTH][DBG] hills initialized count=%d source=%s\n", hillCount, sourceStr);
#endif
}

static int playerInsideHill(Player *player)
{
    if (!player)
        return 0;

    int activeIdx = kothGetActiveHillIndex();
    if (activeIdx < 0 || activeIdx >= hillCount)
        return 0;

    // Build normalized axes from the cuboid basis (fallback to inverse if needed).
    if (!hills[activeIdx].axesComputed)
        kothComputeHillAxes(&hills[activeIdx]);

    float rx = hills[activeIdx].footprintRx;
    float ry = hills[activeIdx].footprintRy;
    if (rx <= 0) rx = KOTH_RING_RADIUS * hillScale;
    if (ry <= 0) ry = KOTH_RING_RADIUS * hillScale;
    float hz = hills[activeIdx].halfHeight * 2.0f;
    if (hz <= 0)
        hz = (KOTH_RING_HEIGHT * KOTH_RING_HEIGHT_SCALE * 0.5f);
    float ringHeight = hz * 2.0f;
    float baseNudge = 0.0f;

    // Anchor scoring to the same base plane as rendering.
    VECTOR baseCenter;
    vector_copy(baseCenter, hills[activeIdx].position);
    VECTOR upOffset;
    vector_scale(upOffset, hills[activeIdx].axisUp, hz + baseNudge);
    vector_subtract(baseCenter, baseCenter, upOffset);

    VECTOR delta;
    vector_subtract(delta, player->playerPosition, baseCenter);

    float projX = vector_innerproduct_unscaled(delta, hills[activeIdx].axisX);
    float projY = vector_innerproduct_unscaled(delta, hills[activeIdx].axisY);
    float projZ = vector_innerproduct_unscaled(delta, hills[activeIdx].axisUp);
    if (projZ < -0.1f || projZ > ringHeight)
        return 0;

    if (hills[activeIdx].isCircle) {
        float radius = rx * KOTH_SCORE_MARGIN_CIRCLE;
        float sqrDist = (projX * projX) + (projY * projY);
        return sqrDist <= (radius * radius);
    }

    float dx = fabsf(projX);
    float dy = fabsf(projY);
    return dx <= rx && dy <= ry;
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

static void kothAwardHillScoreForLocals(Player **players)
{
    int i;
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player *p = players[i];
        if (!p || playerIsDead(p) || p->vehicle || !p->isLocal)
            continue;

        if (playerInsideHill(p)) {
            ++kothScores[p->mpIndex];
            if (kothScores[p->mpIndex] != lastBroadcastScore[p->mpIndex])
                broadcastScore(p->mpIndex);
        }
    }
}

static int kothHillIsContested(void)
{
    int contested = 0;
    int occupantCount = 0;
    int occupantTeam = -1;
    Player **players = playerGetAll();
    int i;

    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player *p = players[i];
        if (!p || playerIsDead(p) || p->vehicle)
            continue;
        if (!playerInsideHill(p))
            continue;

        ++occupantCount;
        if (!kothUseTeams()) {
            if (occupantCount > 1) {
                contested = 1;
                break;
            }
        } else {
            int team = p->mpTeam;
            if (occupantTeam < 0)
                occupantTeam = team;
            else if (team != occupantTeam) {
                contested = 1;
                break;
            }
        }
    }

    return contested;
}

static void updateScores(void)
{
    Player **players = playerGetAll();
    int i;

    // Fast path: legacy behavior when contested scoring is disabled.
    if (!kothContestedStopsScore) {
        kothAwardHillScoreForLocals(players);
        return;
    }

    // Contested mode: block scoring if more than one team (or more than one body in FFA) occupies the hill.
    if (kothHillIsContested())
        return;

    // Award points to locals only when uncontested.
    kothAwardHillScoreForLocals(players);
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
        TIME_SECOND * 90,
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

static float kothGetBlinkScale(void)
{
#if KOTH_FADE_WARNING
    // Fade/pulse as we approach hill rotation.
    float fadeScale = 1.0f;
    const int warnMs = 10 * TIME_SECOND;
    int duration = kothGetHillDurationMs();
    if (duration > 0 && hillCount > 0) {
        kothEnsureCycleStart();
        if (hillCycleStartTime > 0) {
            int elapsed = gameGetTime() - hillCycleStartTime;
            if (elapsed < 0)
                elapsed = 0;
            int cycElapsed = elapsed % duration;
            int timeLeft = duration - cycElapsed;
            if (timeLeft < warnMs) {
                int seconds = timeLeft / TIME_SECOND;
                int phase = seconds & 1; // toggle each second
                fadeScale = phase ? 1.0f : 0.0f;
            }
        }
    }
    return fadeScale;
#else
    return 1.0f;
#endif
}

static void drawHillAt(const KothHill_t *hill, VECTOR center, u32 color, float *scroll, float radiusX, float radiusZ, int isCircle, float cosYaw, float sinYaw, int drawAtMidpoint)
{
    const int MAX_SEGMENTS = 64;
    const int MIN_SEGMENTS = 8;
#define KOTH_USE_STRIP 1
    if (radiusX <= 0) radiusX = KOTH_RING_RADIUS;
    if (radiusZ <= 0) radiusZ = KOTH_RING_RADIUS;
    VECTOR tempRight, tempUp, halfX, halfZ, vRadius;

    // Build oriented axes from the cuboid basis if available.
    VECTOR axisX = {1,0,0,0};
    VECTOR axisZ = {0,1,0,0};
    VECTOR axisUp = {0,0,1,0};
    if (hill) {
    vector_copy(axisX, hill->cuboid.matrix.v0);
    vector_copy(axisZ, hill->cuboid.matrix.v1);
    vector_copy(axisUp, hill->cuboid.matrix.v2);
    }
    float lenX = vector_length(axisX);
    float lenZ = vector_length(axisZ);
    float lenUp = vector_length(axisUp);
    if (lenX > 0.0001f) vector_scale(axisX, axisX, 1.0f / lenX); else vector_copy(axisX, (VECTOR){1,0,0,0});
    if (lenZ > 0.0001f) vector_scale(axisZ, axisZ, 1.0f / lenZ); else vector_copy(axisZ, (VECTOR){0,1,0,0});
    if (lenUp > 0.0001f) vector_scale(axisUp, axisUp, 1.0f / lenUp); else vector_copy(axisUp, (VECTOR){0,0,1,0});

    vector_scale(halfX, axisX, radiusX);
    vector_scale(halfZ, axisZ, radiusZ);
    float fRadius = vector_length(halfX);

    // Use cuboid-derived half-height; apply a modest scale to better span base-to-top while anchored.
    float halfHeight = hill ? (hill->halfHeight * 2.0f) : 0.0f;
    if (halfHeight <= 0)
        halfHeight = (KOTH_RING_HEIGHT * KOTH_RING_HEIGHT_SCALE * 0.5f);
    float ringHeight = halfHeight * 2.0f;
    float baseNudge = 0.0f;
    VECTOR baseCenter, topCenter;
    if (drawAtMidpoint) {
        VECTOR upOffset;
        vector_scale(upOffset, axisUp, halfHeight);
        vector_subtract(baseCenter, center, upOffset);
        vector_add(topCenter, center, upOffset);
    } else {
        VECTOR upOffset;
        vector_scale(upOffset, axisUp, halfHeight);
        vector_subtract(baseCenter, center, upOffset);
        vector_add(topCenter, baseCenter, upOffset);
        vector_add(topCenter, topCenter, upOffset);
    }
    VECTOR upVec;
    vector_subtract(upVec, topCenter, baseCenter); // full height vector
    // TODO: if floor Z-fighting occurs, consider reintroducing a small offset (previously -5% of ringHeight).
    float floorOffsetZ = 0.0f;

    float segmentSize = 1.0f;
    int segments = (int)((2 * MATH_PI * fRadius) / segmentSize);
    segments = (int)clampf((float)segments, (float)MIN_SEGMENTS, (float)MAX_SEGMENTS);
    float thetaStep = 2 * MATH_PI / (float)segments;

    // vec3 positions[(MAX_SEGMENTS + 1) * 2];
    // u32 colors[(MAX_SEGMENTS + 1) * 2];
    // UV_t uvs[(MAX_SEGMENTS + 1) * 2];
    // // Paranoia: clear buffers so unused slots can't leak stale data if segment count changes.
    // memset(positions, 0, sizeof(positions));
    // memset(colors, 0, sizeof(colors));
    // memset(uvs, 0, sizeof(uvs));

    float fadeScale = kothGetBlinkScale();

    int alphaNear = (int)(kothAlphaNearBase * KOTH_RING_ALPHA_SCALE);
    if (alphaNear > 0xFF) alphaNear = 0xFF;
    int alphaFar = (int)(kothAlphaFarBase * KOTH_RING_ALPHA_SCALE);
    if (alphaFar > 0xFF) alphaFar = 0xFF;
    alphaNear = (int)(alphaNear * fadeScale);
    alphaFar = (int)(alphaFar * fadeScale);
    if (alphaNear > 0xFF) alphaNear = 0xFF;
    if (alphaFar > 0xFF) alphaFar = 0xFF;

    u32 baseRgb = color & 0x00FFFFFF;

#if KOTH_USE_STRIP
    if (isCircle) {
        // Quad-per-segment along the arc (manual rotation instead of Rodrigues).
        vector_copy(vRadius, halfX);
        float cosStep = cosf(thetaStep);
        float sinStep = sinf(thetaStep);
        QuadDef quad;
        gfxSetupEffectTex(&quad, kothRingWallFx, 0, 0x80);
        // u = 0 at current angle, u = 1 at next; v scrolls vertically.
        quad.uv[0] = (UV_t){0, -*scroll};
        quad.uv[1] = (UV_t){0, 1.0f - *scroll};
        quad.uv[2] = (UV_t){1, -*scroll};
        quad.uv[3] = (UV_t){1, 1.0f - *scroll};
        // Fade top vs. bottom.
        quad.rgba[0] = quad.rgba[2] = (alphaNear << 24) | baseRgb;
        quad.rgba[1] = quad.rgba[3] = (alphaFar << 24) | baseRgb;

        int i;
        for (i = 0; i < segments; ++i) {
            VECTOR nextRadius = {
                (vRadius[0] * cosStep) - (vRadius[1] * sinStep),
                (vRadius[0] * sinStep) + (vRadius[1] * cosStep),
                vRadius[2],
                0
            };

            VECTOR currCenter, nextCenter;
            vector_add(currCenter, baseCenter, vRadius);
            vector_add(nextCenter, baseCenter, nextRadius);

            // top current
            vector_add(quad.point[0], currCenter, upVec);
            quad.point[0][3] = 1;
            // bottom current
            vector_copy(quad.point[1], currCenter);
            quad.point[1][3] = 1;
            // top next
            vector_add(quad.point[2], nextCenter, upVec);
            quad.point[2][3] = 1;
            // bottom next
            vector_copy(quad.point[3], nextCenter);
            quad.point[3][3] = 1;

            gfxDrawQuad(quad, NULL);
            vector_copy(vRadius, nextRadius);
        }

        *scroll += kothScrollStep;
    } else {
        // Oriented rectangle walls using cuboid axes.
        VECTOR corners[4];
        vector_add(corners[0], baseCenter, (VECTOR){ -halfX[0] - halfZ[0], -halfX[1] - halfZ[1], -halfX[2] - halfZ[2], 0});
        vector_add(corners[1], baseCenter, (VECTOR){  halfX[0] - halfZ[0],  halfX[1] - halfZ[1],  halfX[2] - halfZ[2], 0});
        vector_add(corners[2], baseCenter, (VECTOR){  halfX[0] + halfZ[0],  halfX[1] + halfZ[1],  halfX[2] + halfZ[2], 0});
        vector_add(corners[3], baseCenter, (VECTOR){ -halfX[0] + halfZ[0], -halfX[1] + halfZ[1], -halfX[2] + halfZ[2], 0});

        VECTOR topCorners[4], bottomCorners[4];
        int i;
        for (i = 0; i < 4; ++i) {
            vector_add(topCorners[i], corners[i], upVec);
            topCorners[i][3] = 1;
            vector_copy(bottomCorners[i], corners[i]);
            bottomCorners[i][3] = 1;
        }

        QuadDef wall;
        gfxSetupEffectTex(&wall, kothRingWallFx, 0, 0x80);
        wall.rgba[0] = wall.rgba[1] = (alphaNear << 24) | baseRgb;
        wall.rgba[2] = wall.rgba[3] = (alphaFar << 24) | baseRgb;

        int faces[4][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};
        for (i = 0; i < 4; ++i) {
            int a = faces[i][0];
            int b = faces[i][1];
            // Order points to keep consistent winding for both triangles.
            vector_copy(wall.point[0], topCorners[a]);
            vector_copy(wall.point[1], bottomCorners[a]);
            vector_copy(wall.point[2], topCorners[b]);
            vector_copy(wall.point[3], bottomCorners[b]);
            // Vertical scroll to match circle path (u across, v advances; keep top points sharing v).
            wall.uv[0].x = 0;            wall.uv[0].y = *scroll;         // top A
            wall.uv[1].x = 0;            wall.uv[1].y = *scroll + 1.0f;  // bottom A
            wall.uv[2].x = 1.0f;         wall.uv[2].y = *scroll;         // top B
            wall.uv[3].x = 1.0f;         wall.uv[3].y = *scroll + 1.0f;  // bottom B

            gfxDrawQuad(wall, NULL);
        }
        *scroll += kothScrollStep;
    }

#endif

    // floor quad
    VECTOR corners[4];
        vector_add(corners[0], baseCenter, (VECTOR){ -halfX[0] - halfZ[0], -halfX[1] - halfZ[1], -halfX[2] - halfZ[2], 0});
        vector_add(corners[1], baseCenter, (VECTOR){  halfX[0] - halfZ[0],  halfX[1] - halfZ[1],  halfX[2] - halfZ[2], 0});
        vector_add(corners[2], baseCenter, (VECTOR){  halfX[0] + halfZ[0],  halfX[1] + halfZ[1],  halfX[2] + halfZ[2], 0});
        vector_add(corners[3], baseCenter, (VECTOR){ -halfX[0] + halfZ[0], -halfX[1] + halfZ[1], -halfX[2] + halfZ[2], 0});
    int c;
    for (c = 0; c < 4; ++c) {
        corners[c][0] += axisUp[0] * floorOffsetZ;
        corners[c][1] += axisUp[1] * floorOffsetZ;
        corners[c][2] += axisUp[2] * floorOffsetZ;
        corners[c][3] = 0;
    }

    QuadDef floorQuad;
    // gfxSetupEffectTex(&floorQuad, isCircle ? FX_CIRLCE_NO_FADED_EDGE : FX_SQUARE_FLAT_1, 0, 0x80);
    gfxSetupEffectTex(&floorQuad, FX_CIRLCE_NO_FADED_EDGE, 0, 0x80);
    floorQuad.uv[0] = (UV_t){0, 0};
    floorQuad.uv[1] = (UV_t){0, 1};
    floorQuad.uv[2] = (UV_t){1, 0};
    floorQuad.uv[3] = (UV_t){1, 1};
    int floorAlpha = kothAlphaFloorBase;
    // Optional: tweak floor tint to push greener (uncomment to apply).
    // baseRgb = (baseRgb & 0x00FF00FF) | (((baseRgb >> 16) & 0xFF) << 8); // example tweak
    floorAlpha = (int)(floorAlpha * fadeScale);
    if (floorAlpha > 0xFF) floorAlpha = 0xFF;
    floorQuad.rgba[0] = floorQuad.rgba[1] = floorQuad.rgba[2] = floorQuad.rgba[3] = (floorAlpha << 24) | baseRgb;
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
    u32 baseColor = kothGetActiveHillColor(); // default white; blended toward hill occupants
    KothHill_t *hill = kothFindHill(moby);
    float *scroll = hill ? &hill->scroll : NULL;
    float fallbackScroll = 0;
    if (!scroll)
        scroll = &fallbackScroll;
    float cosYaw = 1.0f, sinYaw = 0.0f;
    if (hill)
        kothGetHillYaw(hill, &cosYaw, &sinYaw);
    float radiusX = hill ? hill->footprintRx : (KOTH_RING_RADIUS * hillScale);
    float radiusZ = hill ? hill->footprintRy : (KOTH_RING_RADIUS * hillScale);
    drawHillAt(hill, moby->position, baseColor, scroll, radiusX, radiusZ, hill ? hill->isCircle : 1, cosYaw, sinYaw, hill ? hill->drawAtMidpoint : 1);
}

static void hillUpdate(Moby *moby)
{
    gfxRegisterDrawFunction(&drawHill, moby);
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
            float cosYaw = 1.0f, sinYaw = 0.0f;
            kothGetHillYaw(&hills[activeIdx], &cosYaw, &sinYaw);
            drawHillAt(&hills[activeIdx], hills[activeIdx].position, kothGetActiveHillColor(), &hills[activeIdx].scroll,
                       hills[activeIdx].footprintRx, hills[activeIdx].footprintRy, hills[activeIdx].isCircle, cosYaw, sinYaw, hills[activeIdx].drawAtMidpoint);
        }
#ifdef KOTH_DEBUG
    } else {
        KOTH_LOG("[KOTH][DBG] drawHills skip activeIdx=%d hillCount=%d\n", activeIdx, hillCount);
#endif
    }
}

void scoreboard(int maxScore, int* scores)
{
    GameSettings *s = gameGetSettings();
    GameOptions *o = gameGetOptions();
    if (!s || !o || !scores)
        return;

    const int opacity = 0x60;
    const u32 bgColor = 0x18608f;
    const u32 textColor = 0x69cbf2;
    const float bgScorebarLerp = 0.25f;

    // Layout constants (top-right, matched to uya-cheats test HUD).
    const float anchorX = 0.8105f;
    const float anchorY = 0.03f;
    const float width = 0.1621f;
    const float height = 0.05055f;
    const float padding = 0.0025f;
    const float scoreBarW = (width * 0.5f) - (padding * 3.0f);
    const float scoreBarH = (height * 0.3333f) * 2.0f;
    const float scoreBarX = anchorX + (width * 0.5f) + padding;
    const float textX = anchorX + (width * 0.5f);

    typedef struct { int team; int score; } TeamScore;
    TeamScore teamRows[TEAM_MAX];
    TeamScore playerRows[GAME_MAX_PLAYERS];
    TeamScore *sortedScores = NULL;
    int maxRows = 0;

    int numRows = 0;
    if (o->GameFlags.MultiplayerGameFlags.Teams) {
        sortedScores = teamRows;
        maxRows = TEAM_MAX;
        // Accumulate scores per team only for teams that have players.
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
            int team = p->mpTeam;
            if (team < 0 || team >= TEAM_MAX)
                continue;
            teamSeen[team] = 1;
            teamScores[team] += scores[p->mpIndex];
        }

        int t;
        for (t = 0; t < TEAM_MAX && numRows < maxRows; ++t) {
            if (!teamSeen[t])
                continue;
            sortedScores[numRows].team = t;
            sortedScores[numRows].score = teamScores[t];
            ++numRows;
        }
    } else {
        sortedScores = playerRows;
        maxRows = GAME_MAX_PLAYERS;
        // FFA: one row per active player.
        char seen[GAME_MAX_PLAYERS];
        memset(seen, 0, sizeof(seen));
        Player **players = playerGetAll();
        int i;
        for (i = 0; i < GAME_MAX_PLAYERS && numRows < maxRows; ++i) {
            Player *p = players[i];
            if (!p)
                continue;
            int idx = p->mpIndex;
            if (idx < 0 || idx >= GAME_MAX_PLAYERS)
                continue;
            if (seen[idx])
                continue;
            if (!s->PlayerNames[idx][0])
                continue;
            seen[idx] = 1;
            sortedScores[numRows].team = idx;
            sortedScores[numRows].score = scores[idx];
            ++numRows;
        }
    }

    if (numRows <= 0)
        return;

    // Sort rows by score descending (stable enough for small N).
    int i, j;
    for (i = 0; i < numRows - 1; ++i) {
        int maxIdx = i;
        for (j = i + 1; j < numRows; ++j) {
            if (sortedScores[j].score > sortedScores[maxIdx].score)
                maxIdx = j;
        }
        if (maxIdx != i) {
            TeamScore tmp = sortedScores[i];
            sortedScores[i] = sortedScores[maxIdx];
            sortedScores[maxIdx] = tmp;
        }
    }

    int topScore = sortedScores[0].score;
    if (maxScore <= 0)
        maxScore = topScore;
    if (maxScore <= 0)
        maxScore = 1;

    // Draw rows.
    // Flush state once before HUD drawing to reduce GS/GIF bleed risk.
    gfxDoGifPaging();
    char buf[32];
    for (i = 0; i < numRows; ++i) {
        int currentScore = sortedScores[i].score;
        int currentTeam = sortedScores[i].team;

        float rowYnorm = anchorY + i * (height + padding);
        float scoreBarYnorm = rowYnorm + (height - scoreBarH) * 0.5f;
        float textYnorm = rowYnorm + (height * 0.5f) - 0.005f;
        float fill = clampf((float)currentScore / (float)maxScore, 0.0f, 1.0f);
        //float fill = 0.5f; 
        //KOTH_LOG("[KOTH][DBG] scoreboard row=%d/%d team=%d score=%d maxScore=%d fillNum=%d fillRaw=%d\n",
        //         i, numRows, currentTeam, currentScore, maxScore, (int)(fill * 1000), currentScore ? (int)((currentScore * 1000) / maxScore) : 0);

        float textScale = currentScore > 9999 ? 0.5f : (currentScore > 999 ? 0.75f : 1.0f);
        //float textScale = 1.0f; 


        //// Log inputs for each draw to trace bad state (ints for PCSX2 log visibility).
        //KOTH_LOG("[KOTH][DBG] scoreboard row=%d pos=%d,%d size=%d,%d fill=%d scoreBarY=%d textY=%d textScale=%d\\n",
        //         i,
        //         (int)(anchorX * 1000), (int)(rowYnorm * 1000),
        //         (int)(width * 1000), (int)(height * 1000),
        //         (int)(fill * 1000),
        //         (int)(scoreBarYnorm * 1000),
        //         (int)(textYnorm * 1000),
        //         (int)(textScale * 1000));

        // Normalized screen-space boxes to align with font pipeline.
        gfxScreenSpaceBox(anchorX, rowYnorm, width, height, (opacity << 24) | bgColor);
        u32 teamColor = (currentTeam >= 0 && currentTeam < TEAM_MAX) ? TEAM_COLORS[currentTeam] : 0x00FFFFFF;
        // Solid color for bg (no lerp) to avoid NaN/inf.
        // Slightly lighter background for the bar track.
        int trackOpacity = opacity >> 1;
        gfxScreenSpaceBox(scoreBarX, scoreBarYnorm, scoreBarW, scoreBarH, (trackOpacity << 24) | (teamColor & 0x00FFFFFF));
        gfxScreenSpaceBox(scoreBarX, scoreBarYnorm, scoreBarW * fill, scoreBarH, (opacity << 24) | teamColor);

        snprintf(buf, sizeof(buf), "%d", currentScore);
        gfxScreenSpaceText(textX * SCREEN_WIDTH, textYnorm * SCREEN_HEIGHT, textScale, textScale, (opacity << 24) | textColor, buf, -1, TEXT_ALIGN_MIDDLERIGHT, FONT_BOLD);

    }

}

static void drawHud(void)
{
    scoreboard(kothGetScoreLimit(), kothScores);
    radarUpdate();
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

    // Size index comes from padding[0]; default to 0 if out of range.
    int incomingSizeIdx = msg->Padding[0];
    if (incomingSizeIdx < 0 || incomingSizeIdx >= KOTH_SIZE_OPTIONS)
        incomingSizeIdx = 0;
    if (incomingSizeIdx != hillSizeIdx) {
        hillSizeIdx = incomingSizeIdx;
        hillScale = KOTH_HILL_SCALE_TABLE[hillSizeIdx];
        kothOnHillSizeUpdated();
#ifdef KOTH_DEBUG
        KOTH_LOG("[KOTH][DBG] hill size updated from sync idx=%d scale=%d\n", hillSizeIdx, (int)hillScale);
#endif
    }

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
    kothInitLogged = 0;
    hillSizeIdx = 0;
    hillScale = KOTH_HILL_SCALE_TABLE[hillSizeIdx];
#ifdef KOTH_RANDOM_ORDER
    hillOrderCount = 0;
    ++hillOrderShuffleNonce;
#endif
    memset(hills, 0, sizeof(hills));
    memset(kothScores, 0, sizeof(kothScores));
    memset(lastBroadcastScore, 0, sizeof(lastBroadcastScore));
}

void radarInit(void)
{
    // Create container first
    hudCreateContainer(KOTH_HUD_CONTAINER_ID);
    // Make container visible
    hudSetFlags(KOTH_HUD_CONTAINER_ID, 1, true);
    // Add container to radar root (0x50000) so it renders
    hudAddToContainer(HUD_RADAR_ROOT, KOTH_HUD_CONTAINER_ID);
    // create rectangle widget
    if (hudCreateRectangle(0.05, 0.05, 0.05, 0.05, KOTH_HUD_FRAME_ID, 0x80ffffff, KOTH_HUD_SPRITE)) {
        // add rectrangle frame to main container
        hudAddToContainer(KOTH_HUD_CONTAINER_ID, KOTH_HUD_FRAME_ID);
        // set visibility
        hudSetFlags(KOTH_HUD_FRAME_ID, 1, true);
    }
}

void radarUpdate(void)
{
    float x, y;
    int id = KOTH_HUD_FRAME_ID;
    int a = kothGetActiveHillIndex();
    if (a < 0 || a >= hillCount)
        return;

    Moby *draw = hills[a].drawMoby;
    if (!draw)
        return;

    gfxWStoMapSpace(draw->position, &x, &y);
    hudSetPosition(x, y, id);
    // Alpha overlay tinted by current hill occupant team (if any) and blinked with hill warning.
    float fadeScale = kothGetBlinkScale();
    int alpha = (int)(0x80 * fadeScale);
    if (alpha > 0xFF) alpha = 0xFF;
    hudSetColor(id, (alpha << 24) | (kothGetActiveHillColor() & 0x00FFFFFF));
}

static float kothDecodeRespawnValue(char idx)
{
    static const float OPTIONS[] = {40.0f, 60.0f, 80.0f, 120.0f, 500.0f, 10.0f, 20.0f, 30.0f};
    int count = (int)(sizeof(OPTIONS) / sizeof(OPTIONS[0]));
    if (idx < 0 || idx >= count)
        return OPTIONS[0];
    return OPTIONS[(int)idx];
}

static float kothGetOutsideRespawnDistance(void)
{
    // Default to index 0 (40.0f) if config is missing.
    return kothDecodeRespawnValue(kothConfig ? kothConfig->grKothRespawnOutside : 0);
}

static float kothGetInsideRespawnDistance(void)
{
    // Default to index 0 (40.0f) if config is missing.
    return kothDecodeRespawnValue(kothConfig ? kothConfig->grKothRespawnInside : 0);
}

static int kothRespawnDistanceIsCustom(void)
{
    // Skip touching the spawn distance when both options are left at default 40.
    return !(kothGetOutsideRespawnDistance() == 40.0f && kothGetInsideRespawnDistance() == 40.0f);
}

static void setSpawnDistance(float distance)
{
    u32 addr = GetAddress(&vaResurrectSpawnDistance);
    if (!addr)
        return;

    if (distance < 0)
        distance = 0;

    union {
        float f;
        u32 u;
    } bits = { distance };

    // The target is a LUI immediate loading the high 16 bits of the float constant.
    u16 s = (u16)(bits.u >> 16);
    POKE_U16(addr, s);

#if KOTH_DEBUG
    u16 confirm = *(u16*)addr;
    if (confirm != s) {
        KOTH_LOG("[KOTH][DBG] spawn distance poke mismatch want=%u got=%u addr=0x%08X\n", s, confirm, addr);
    }
#endif
}

static int kothTeamHasPlayerInHill(int team)
{
    if (team < 0)
        return 0;

    Player **players = playerGetAll();
    int i;
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player *p = players[i];
        if (!p || playerIsDead(p) || p->vehicle)
            continue;
        if (p->mpTeam != team)
            continue;
        if (playerInsideHill(p))
            return 1;
    }

    return 0;
}

static float kothSelectRespawnDistance(Player *player)
{
    float outsideDist = kothGetOutsideRespawnDistance();

    if (!player)
        return outsideDist;

    if (!kothUseTeams())
        return outsideDist;

    if (kothTeamHasPlayerInHill(player->mpTeam))
        return kothGetInsideRespawnDistance();

    return outsideDist;
}

static void kothUpdateRespawnDistance(Player *player)
{
    static char lastPrintWaiting[GAME_MAX_PLAYERS];
    static int lastPrintDistance[GAME_MAX_PLAYERS];

    if (!player)
        return;

    float distance = kothSelectRespawnDistance(player);
    if (distance < 0)
        distance = 0;
    int idx = player->mpIndex;
    int waiting = player->timers.resurrectWait != 0;
    int distInt = (int)distance;

    if (idx >= 0 && idx < GAME_MAX_PLAYERS) {
        if (waiting) {
            if (!lastPrintWaiting[idx] || lastPrintDistance[idx] != distInt) {
                printf("KOTH respawn distance %d\n", distInt);
                lastPrintDistance[idx] = distInt;
            }
        } else {
            lastPrintWaiting[idx] = 0;
        }
        lastPrintWaiting[idx] = waiting;
    }

    setSpawnDistance(distance);
}

static void kothUpdateRespawnDistanceForLocals(void)
{
    if (!kothRespawnDistanceIsCustom())
        return;

    Player **players = playerGetAll();
    int i;
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player *p = players[i];
        if (!p || !p->isLocal)
            continue;
        if (!p->timers.resurrectWait)
            continue;
        kothUpdateRespawnDistance(p);
    }
}

static u32 kothGetActiveHillColor(void)
{
    // Default white; tint/blend to the colors of anyone occupying the hill.
    const u32 defaultColor = 0x00FFFFFF;
    static u32 cachedColor = defaultColor;
    static int lastPollTime = 0;

    int now = gameGetTime();
    // Update at most twice per second to reduce per-frame scanning cost.
    // Use shift instead of divide for ~half-second cadence.
    if (now - lastPollTime < (TIME_SECOND >> 1))
        return cachedColor;
    lastPollTime = now;

    if (!kothUseTeams())
        ; // still allow blending based on player index colors below

    // If the hill is contested and the feature is enabled, force white.
    if (kothContestedStopsScore && kothHillIsContested())
        return (cachedColor = defaultColor);

    int activeIdx = kothGetActiveHillIndex();
    if (activeIdx < 0)
        return (cachedColor = defaultColor);

    int colorCount = 0;
    int accumR = 0, accumG = 0, accumB = 0;
    int teamsMode = kothUseTeams();
    Player **players = playerGetAll();
    int i;
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player *p = players[i];
        if (!p || playerIsDead(p) || p->vehicle)
            continue;
        if (!playerInsideHill(p))
            continue;
        int colorIdx = teamsMode ? p->mpTeam : p->mpIndex;
        if (colorIdx < 0)
            colorIdx = 0;
        // Reuse TEAM_COLORS palette even in FFA for variety.
        u32 color = TEAM_COLORS[colorIdx % TEAM_MAX];
        accumR += (color >> 16) & 0xFF;
        accumG += (color >> 8) & 0xFF;
        accumB += color & 0xFF;
        ++colorCount;
    }

    if (colorCount <= 0)
        return (cachedColor = defaultColor);

    u32 blended = ((accumR / colorCount) << 16) | ((accumG / colorCount) << 8) | (accumB / colorCount);
    return (cachedColor = blended);
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

    radarInit();

#if KOTH_DEBUG
    // Log once per init call (not every frame) when we actually set up hills/handlers.
    if (!kothInitLogged) {
        KOTH_LOG("[KOTH][DBG] init complete handler=%d gameEndHook=%d hills=%d seed=%d scoreLimit=%d hillMs=%d\n",
                 handlerInstalled, gameEndHookInstalled, hillCount, (kothConfig ? kothConfig->grSeed : -1),
                 kothGetScoreLimit(), kothGetHillDurationMs());
        kothInitLogged = 1;
    }
#endif
}

void kothTick(void)
{
    if (!isInGame())
        return;

    // Detect a new match via timeStart and fully reset state (scores, hills, seed).
    {
        GameData *gd = gameGetData();
        int timeStart = gd ? gd->timeStart : -1;
        int seedNowRaw = kothConfig ? kothConfig->grSeed : 0;
        int seedNow = seedNowRaw & 0x0FFFFFFF;
        if ((timeStart > 0 && timeStart != lastTimeStart) || seedNow != lastSeed) {
            kothReset();
            // Reapply latest config so size/seed persist across match resets.
            kothSetConfig(kothConfig);
            lastTimeStart = timeStart;
            lastSeed = seedNow;
        }
    }

    kothInit();
    if (kothRespawnDistanceIsCustom())
        kothUpdateRespawnDistanceForLocals();
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
    msg.Padding[0] = (char)hillSizeIdx;
    // Send elapsed time on current hill so clients can reconstruct cycle start on their own clock.
    msg.ElapsedMs = gameGetTime() - hillCycleStartTime;
    netBroadcastCustomAppMessage(connection, CUSTOM_MSG_ID_KOTH_HILL_SYNC, sizeof(msg), &msg);
}
#endif
