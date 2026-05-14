#include <tamtypes.h>
#include <libuya/player.h>
#include <libuya/spawnpoint.h>
#include <libuya/moby.h>
#include <libuya/common.h>
#include <libuya/graphics.h>
#include <libuya/math.h>
#include <libuya/math3d.h>

#define HILL_MOBY_OCLASS (0x3000)
#define HILL_MOBY_SIZE (0x200)

#define HILL_VANILLA_SCALE (20.0f)

#define KOTH_HUD_HILL_SPRITE (SPRITE_HUD_BOLT)
#define KOTH_HUD_TIMER_ICON_SPRITE (0x63) // clock_widget
#define KOTH_HUD_CONTAINER_ID (0xacdc0000)
#define KOTH_HUD_FRAME_ID (KOTH_HUD_CONTAINER_ID + 1)

#define HILL_RING_GLOBAL_TEXTURE (FX_TIRE_TRACKS)
#define HILL_RING_GLOBAL_ROTATE_TEXTURE (false)
#define HILL_RING_GLOBAL_SCROLL_SPEED (0.007)
#define HILL_RING_GLOBAL_TRIM_U (0.0f)
#define HILL_RING_GLOBAL_TRIM_V (0.25f)
#define HILL_RING_GLOBAL_MAX_SEGMENTS (64)
#define HILL_RING_GLOBAL_MIN_SEGMENTS (4)

#define HILL_FLOOR_DRAW (true)
#define HILL_FLOOR_OPACITY (0x30)
#define HILL_FLOOR_TEXTURE_CIRCLE (FX_CIRLCE_NO_FADED_EDGE)
#define HILL_FLOOR_TEXTURE_SQUARE (FX_SQUARE_FLAT_1)
#define HILL_FLOOR_HEIGHT_OFFSET (0.0f)

#define HILL_RING_LOWER_DRAW (true)
#define HILL_RING_LOWER_HEIGHT_OFFSET (0.0f)
#define HILL_RING_LOWER_TOP_OPACITY (0x88)
#define HILL_RING_LOWER_BOTTOM_OPACITY (0x10)
#define HILL_RING_LOWER_TOP_STRETCH (0.0f)
#define HILL_RING_LOWER_BOTTOM_STRETCH (0.0f)

#define HILL_RING_UPPER_DRAW (true)
#define HILL_RING_UPPER_HEIGHT_OFFSET (4.0f)
#define HILL_RING_UPPER_TOP_OPACITY (0x00)
#define HILL_RING_UPPER_BOTTOM_OPACITY (0x88)
#define HILL_RING_UPPER_TOP_STRETCH (0.0f)
#define HILL_RING_UPPER_BOTTOM_STRETCH (0.0f)

typedef struct config {
    char grKothScoreLimit;
    char grKothHillDuration;
    char grKothRespawnOutside;
    char grKothRespawnInside;
    char grKothHillSizeIdx;
    char grKothContestedStopsScore;
    char grKothPointStacking;
    char pad;
    int grSeed;
} config_t;

typedef struct kothInfo {
    int gameState;
    bool init;
    bool lastGameUsedMalloc;
    bool radar;
    bool foundCustomMoby;
    int hillScoreLimit;
    Moby *hillMoby;
} kothInfo_t;

typedef struct ringGlobals { // 0x20
/* 0x00 */ short min_segments;
/* 0x02 */ short max_segments;
/* 0x04 */ short texture;
/* 0x06 */ bool rotateTex;
/* 0x07 */ char pad_02;
/* 0x08 */ float trim_u;
/* 0x0c */ float trim_v;
/* 0x10 */ float scrollSpeed;
/* 0x14 */ int pad_14[3];
} ringGlobals_t;

typedef struct floorVars { // 0x10
/* 0x00 */ bool draw;
/* 0x01 */ char opacity;
/* 0x02 */ char pad_02[2];
/* 0x04 */ short circleTex;
/* 0x06 */ short squareTex;
/* 0x08 */ float height_offset;
/* 0x0c */ int pad_0c;
} floorVars_t;

typedef struct ringVars { // 0x10
/* 0x00 */ bool draw;
/* 0x01 */ char pad_01;
/* 0x02 */ char top_opacity;
/* 0x03 */ char bottom_opacity;
/* 0x04 */ float top_stretch;
/* 0x08 */ float bottom_stretch;
/* 0x0c */ float height_offset;
} ringVars_t;

typedef struct hillPvar { // 0x200
/* 0x000 */ int hillCuboidIndex[32];
/* 0x080 */ Cuboid *hillCuboidPtrs[32];
/* 0x100 */ Cuboid *currentCuboid;
/* 0x104 */ int teamTime[8];
/* 0x124 */ Player *playersIn[GAME_MAX_PLAYERS];
/* 0x144 */ bool isCircle;
/* 0x145 */ char pad_145;
/* 0x146 */ char pad_146;
/* 0x147 */ char pad_147;
/* 0x148 */ vec3 *positions;
/* 0x14c */ u32 *colors;
/* 0x150 */ UV_t *uvs;
/* 0x154 */ float opacityFactor;
/* 0x158 */ u32 color;
/* 0x15c */ float scrollTex;
/* 0x160 */ ringGlobals_t globalRing;
/* 0x180 */ floorVars_t floor;
/* 0x190 */ ringVars_t lowerRing;
/* 0x0a0 */ ringVars_t upperRing;
/* 0x1b0 */ char empty[0x50];
} hillPvar_t;

struct KothState {
    int Initialized;
    int GameOver;
    int IsHost;
    int LastConfigSeed;
};

struct KothState State;
kothInfo_t kothInfo;
