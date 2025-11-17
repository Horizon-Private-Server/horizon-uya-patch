/***************************************************
 * FILENAME :		graphics.h
 * DESCRIPTION :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GRAPHICS_H_
#define _LIBUYA_GRAPHICS_H_

#include <tamtypes.h>
#include "math3d.h"
#include "moby.h"


//--------------------------------------------------------
#if UYA_PAL
#define SCREEN_WIDTH           (512)
#define SCREEN_HEIGHT          (448)
#else
#define SCREEN_WIDTH           (512)
#define SCREEN_HEIGHT          (416)
#endif

typedef enum eSpriteTex {
	SPRITE_CHROME = -27,
	SPRITE_GLASS,
	SPRITE_NEW_GAME_PLUS_1,
	SPRITE_NEW_GAME_PLUS_2,
	SPRITE_HACKER_1,
	SPRITE_NO_CHICKEN,
	SPRITE_WEAPON_LEVEL_UPGRADED,
	SPRITE_RANGER_MISSION_TURRET,
	SPRITE_WEAPON_LEVEL_OMEGA,
	SPRITE_WEAPON_LEVEL_NORMAL,
	SPRITE_OXYGEN_METER_FULL,
	SPRITE_EGG,
	SPRITE_DIALOG_QWARK,
	SPRITE_DIALOG,HELGA,
	SPRITE_DIALOG_SKID,
	SPRITE_DIALOG_AL,
	SPRITE_DIALOG_SKURNCH,
	SPRITE_QUICK_SELECT_R1,
	SPRITE_QUICK_SELECT_R2,
	SPRITE_OXYGEN_METER_REGULAR,
	SPRITE_OXYGEN_METER_LOW,
	SPRITE_WATER_BUBBLE,
	SPRITE_GADGETBOT_ATTACK,
	SPRITE_GADGETBOT_FOLLOW,
	SPRITE_GADGETBOT_ENTER,
	SPRITE_GADGETBOT_HOLD_POSITION,
	SPRITE_GADGETBOT_LIFT_BOT_LIFTDOWN,
	SPRITE_GADGETBOT_LIFT_BOT_LIFTUP,
	SPRITE_GADGETBOT_BRIDGE_BOT_RETRACT,
	SPRITE_GADGETBOT_BRIDGE_BOT_EXTEND,
	SPRITE_GADGETBOT_HAMMER_BOT,
	SPRITE_GADGETBOT_SALUTE,
	SPRITE_ENEMY_DOT_1,
	SPRITE_SHIP_PAINT_JOB,
	SPRITE_ENEMY_DOT_2,
	SPRITE_SHIP_NOSE_1,
	SPRITE_SHIP_NOSE_2,
	SPRITE_SHIP_NOSE_3,
	SPRITE_SHIP_WINGS_1,
	SPRITE_SHIP_WINGS_2,
	SPRITE_SHIP_WINGS_3,
	SPRITE_BOX_BREAKER,
	SPRITE_PLAYER_TURRET_1,
	SPRITE_VIDCOMIC_1,
	SPRITE_VIDCOMIC_2,
	SPRITE_VIDCOMIC_3,
	SPRITE_VIDCOMIC_4,
	SPRITE_VIDCOMIC_5,
	SPRITE_MENU_CORNER,
	SPRITE_BOLT_SP,
	SPRITE_SEWER_CRYSTAL_SP,
	SPRITE_HACKER_2,
	SPRITE_HELIPAK,
	SPRITE_MEMORY_CARD,
	SPRITE_MEMORY_CARD_SAVE,
	SPRITE_HOLOSHIELD_GLOVE,
	SPRITE_WEAPON_INFECTOR,
	SPRITE_WEAPON_N60_STORM_1,
	SPRITE_WEAPON_PLASMA_WHIP,
	SPRITE_WEAPON_RIFT_INDUCER,
	SPRITE_WEAPON_SUCK_CANNON_1,
	SPRITE_WEAPON_AGENTS_OF_DOOM,
	SPRITE_WEAPON_ANNIHILATOR,
	SPRITE_WEAPON_DISC_GUN,
	SPRITE_WEAPON_FLUX_RIFLE_1,
	SPRITE_WEAPON_NITRO_LAUNCHER,
	SPRITE_WEAPON_PLASMA_COIL_1,
	SPRITE_WEAPON_FLUX_RIFLE_2,
	SPRITE_WEAPON_QUAKORAY,
	SPRITE_WEAPON_SHIELD_CHARGER,
	SPRITE_WEAPON_SHOCK_BLASTER,
	SPRITE_WEAPON_SPITTING_HYDRA_1,
	SPRITE_WEAPON_SUCK_CANNON_2,
	SPRITE_WEAPON_SPITTING_HYDRA_2,
	SPRITE_NUMBER_0,
	SPRITE_NUMBER_1,
	SPRITE_NUMBER_2,
	SPRITE_NUMBER_3,
	SPRITE_NUMBER_4,
	SPRITE_NUMBER_5,
	SPRITE_NUMBER_6,
	SPRITE_NUMBER_7,
	SPRITE_NUMBER_8,
	SPRITE_NUMBER_9,
	SPRITE_BOLT,
	SPRITE_CIRCLE,
	SPRITE_SQUARE,
	SPRITE_TRIANGLE,
	SPRITE_CROSS,
	SPRITE_ARROW_SWITCH_TEAM,
	SPRITE_WEAPON_WHOLLOPER_MP,
	SPRITE_WEAPON_MINE_GLOVE_MP,
	SPRITE_WEAPON_N60_STORM_2,
	SPRITE_WEAPON_MINI_ROCKET_TUBE_MP,
	SPRITE_WEAPON_HOLOSHIELD_GLOVE_MP,
	SPRITE_WEAPON_BLITZ_GUN_MP,
	SPRITE_WEAPON_FLUX_RIFLE_3,
	SPRITE_BASE_1,
	SPRITE_NODE_1,
	SPRITE_PLAYER_ARROW_1,
	SPRITE_TURBOSLIDER_MP,
	SPRITE_HOVERSHIP_SP,
	SPRITE_UNK_1,
	SPRITE_CAMERA,
	SPRITE_DRONE_SP,
	SPRITE_SKULL_SP,
	SPRITE_PLAYER_SP,
	SPRITE_DOT,
	SPRITE_SHIELD_SP,
	SPRITE_HEADPHONES_1,
	SPRITE_ARROW,
	SPRITE_AMMO,
	SPRITE_PLAYER_TURRET_2,
	SPRITE_TYHRRANOID,
	SPRITE_WAVEFORM,
	SPRITE_X,
	SPRITE_KILLER_FISH,
	SPRITE_WEAPON_CLANK_ZAPPER,
	SPRITE_BOSS_NINJA,
	SPRITE_BOSS_GLADIATOR,
	SPRITE_CAUTION_SIGN,
	SPRITE_FIST,
	SPRITE_GAS_MASK,
	SPRITE_QWARK_BLASTER,
	SPRITE_MAP_ROTATION,
	SPRITE_STOPWATCH,
	SPRITE_QUASTION_MARK,
	SPRITE_SWINGSHOT,
	SPRITE_NANO_PAK,
	SPRITE_REFRACTOR,
	SPRITE_TYHRRAGUISE,
	SPRITE_WARP_PAD__MP,
	SPRITE_SEWER_CRYSTAL__MP,
	SPRITE_UNK_01,
	SPRITE_DISCO_BALL,
	SPRITE_TITANIUM_BOLT,
	SPRITE_AMMO_PAD,
	SPRITE_BOLD_MULTIPLYER,
	SPRITE_ARROW_LEFT,
	SPRITE_QUESTION_MARK_2,
	SPRITE_WEAPON_BOUNCER,
	SPRITE_WEAPON_MINI_TURRET_GLOVE,
	SPRITE_WEAPON_RYNO,
	SPRITE_WEAPON_PLASMA_COIL_2,
	SPRITE_WEAPON_SHIELD_CHARGER_MP,
	SPRITE_CHARGEBOOTS,
	SPRITE_BOLT_GRABBER,
	SPRITE_HAMMER_BLUE,
	SPRITE_HAMMER_WHITE,
	SPRITE_MAGNETBOOTS,
	SPRITE_PDA,
	SPRITE_THRUSTERPAK,
	SPRITE_MAPOMATIC,
	SPRITE_FONT_SQUARE,
	SPRITE_STAR,
	SPRITE_QUESTION_MARK_3,
	SPRITE_QFORCE_SYMBOL,
	SPRITE_PLAYER_ARROW_2,
	SPRITE_GENERIC_VENDOR,
	SPRITE_GADGETRON_VENTOR,
	SPRITE_ARMOR_VENDOR,
	SPRITE_SHIP_VENDOR,
	SPRITE_SLIM_CAGNITO,
	SPRITE_LOADING,
	SPRITE_PLAYER_MP,
	SPRITE_READY_UP,
	SPRITE_SHIELD_MP,
	SPRITE_HEADPHONES_2,
	SPRITE_TRIANGLE_RIGHT,
	SPRITE_SKULL_MP,
	SPRITE_HOVERSHIP_MP,
	SPRITE_DESTRUCTABLE_BRIDGE,
	SPRITE_CINEMATICS,
	SPRITE_DRONE_MP,
	SPRITE_WEAPON_PUNCHING_GLOVE,
	SPRITE_WEAPON_MINE_GLOVE,
	SPRITE_WEAPON_LAVA_GUN,
	SPRITE_WEAPON_N60_STORM_3,
	SPRITE_FIRST_PERSON_MODE,
	SPRITE_WARP_PAD_2,
	SPRITE_WEAPON_FLUX_RIFLE_4,
	SPRITE_PLAYER_TURRET_3,
	SPRITE_FLAG,
	SPRITE_NODE_MP,
	SPRITE_THIRD_PERSON_MODE,
	SPRITE_WEAPON_SHIELD_CHARGER_2,
	SPRITE_LOCKSTRAFE_MODE,
	SPRITE_WEAPON_GRAVITY_BOMB,
	SPRITE_WEAPON_MINI_ROCKET_TUBE,
	SPRITE_SHEEPINATOR,
	SPRITE_WEAPON_HOLOSHIELD_GLOVE,
	SPRITE_WEAPON_GLITZ_GUN,
	SPRITE_BASE_MP,
	SPRITE_TURBOSLIDER,
	SPRITE_PLAYER_ARROW_3,
	SPRITE_ARMOR_1,
	SPRITE_ARMOR_2,
	SPRITE_ARMOR_3,
	SPRITE_ARMOR_4,
	SPRITE_ARMOR_5,
	SPRITE_BOSS_BIOBLITERATOR,
	SPRITE_BOSS_COURTNEY_GEARS,
	SPRITE_BOSS_TERRIBLE_TWO,
	SPRITE_BOSS_WHIP_CHICK,
	SPRITE_BOSS_CLUNK,
	SPRITE_BOSS_NEFARIOUS,
	SPRITE_BOSS_HYDROTANK,
	SPRITE_BOSS_DAXX_SHIP,
	SPRITE_BOSS_UNK_0,
	SPRITE_BOSS_QUEEN_THYRRANDOI,
	SPRITE_BOSS_CHIMP_QWARK,
} SpriteTex_e;

typedef enum eSpriteTex_Hud {
	SPRITE_HUD_R = 0xed1b,
	SPRITE_HUD_NUMBER_0 = 0xed1c,
	SPRITE_HUD_NUMBER_1 = 0xed1d,
	SPRITE_HUD_NUMBER_2 = 0xed1e,
	SPRITE_HUD_NUMBER_3 = 0xed1f,
	SPRITE_HUD_NUMBER_4 = 0xed20,
	SPRITE_HUD_NUMBER_5 = 0xed21,
	SPRITE_HUD_NUMBER_6 = 0xed22,
	SPRITE_HUD_NUMBER_7 = 0xed23,
	SPRITE_HUD_NUMBER_8 = 0xed24,
	SPRITE_HUD_NUMBER_9 = 0xed25,
	SPRITE_HUD_BOLT = 0xed26,
	SPRITE_HUD_CIRCLE = 0xed27,
	SPRITE_HUD_SQUARE = 0xed28,
	SPRITE_HUD_TRIANGLE = 0xed29,
	SPRITE_HUD_CROSS = 0xed2a,
	SPRITE_HUD_ARROW_SWITCH_TEAM = 0xed2b,
	SPRITE_HUD_WEAPON_WHOLLOPER = 0xed2c,
	SPRITE_HUD_WEAPON_MINE_GLOVE = 0xed2d,
	SPRITE_HUD_WEAPON_N60_STORM = 0xed2e,
	SPRITE_HUD_WEAPON_MINI_ROCKET_TUBE = 0xed2f,
	SPRITE_HUD_WEAPON_HOLOSHIELD_GLOVE = 0xed30,
	SPRITE_HUD_WEAPON_BLITZ_CANNON = 0xed31,
	SPRITE_HUD_WEAPON_FLUX_RIFLE_5 = 0xed32,
	SPRITE_HUD_BASE = 0xed33,
	SPRITE_HUD_NODE = 0xed34,
	SPRITE_HUD_PLAYER_ARROW_4 = 0xed35,
	SPRITE_HUD_TURBOSLIDER = 0xed36,
	SPRITE_HUD_HOVERSHIP = 0xed37,
	SPRITE_HUD_UNK_1 = 0xed38,
	SPRITE_HUD_CAMERA = 0xed39,
	SPRITE_HUD_DRONE = 0xed3a,
	SPRITE_HUD_SKULL = 0xed3b,
	SPRITE_HUD_PLAYER = 0xed3c,
	SPRITE_HUD_DOT = 0xed3d,
	SPRITE_HUD_SHIELD = 0xed3e,
	SPRITE_HUD_HEADPHONES = 0xed3f,
	SPRITE_HUD_ARROW = 0xed40,
	SPRITE_HUD_AMMO = 0xed41,
	SPRITE_HUD_PLAYER_TURRET = 0xed42,
	SPRITE_HUD_TYHRRANOID = 0xed43,
	SPRITE_HUD_WAVEFORM = 0xed44,
	SPRITE_HUD_X = 0xed45,
	SPRITE_HUD_KILLER_FISH = 0xed46,
	SPRITE_HUD_WEAPON_CLANK_ZAPPER = 0xed47,
	SPRITE_HUD_UNK_2 = 0xed48,
	SPRITE_HUD_FIRST_PERSON_MODE = 0xed49,
	SPRITE_HUD_WARP_PAD = 0xed4a,
	SPRITE_HUD_PLAYER_TURRET_2 = 0xed4b,
	SPRITE_HUD_THIRD_PERSON_MODE = 0xed4c,
	SPRITE_HUD_WEAPON_OMNISHIELD = 0xed4d,
	SPRITE_HUD_LOCKSTRAFE_MODE = 0xed4e,
	SPRITE_HUD_WEAPON_GRAVITY_BOMB = 0xed4f,
	SPRITE_HUD_QWARK = 0xed50,
	SPRITE_HUD_R1 = 0xed51,
	SPRITE_HUD_UNK_3 = 0xed52,
	SPRITE_HUD_UNK_4 = 0xed53,
	SPRITE_HUD_UNK_5 = 0xed54,
	SPRITE_HUD_UNK_6 = 0xed55,
	SPRITE_HUD_R2 = 0xed56,
	SPRITE_HUD_UNK_7 = 0xed57,
	SPRITE_HUD_VIDCOMIC_1 = 0xed58,
	SPRITE_HUD_UNK_8 = 0xed59,
	SPRITE_HUD_UNK_9 = 0xed5a,
	SPRITE_HUD_FLAG = 0xed5b,
	SPRITE_HUD_LOADING = 0xed5c,
} SpriteTex_Hud_e;

typedef enum EffectsTex {
	FX_VIDEO_BUFFER_UNK_2 = -8,
	FX_VIDEO_BUFFER_WITH_HUD = -7,
	FX_VIDEO_BUFFER_16x9 = -6,
	FX_VIDEO_BUFFER_UNK_1 = -5,
	FX_VIDEO_BUFFER_4_LOW_RES = -4,
	FX_VIDEO_BUGGER_3_LOW_RES = -3,
	FX_VIDEO_BUGGER_2 = -2,
	FX_VIDEO_BUFFER_1 = -1,
	FX_SMOKE_BLACK = 0,
	FX_LENS_FLARE_1 = 1,
	FX_LENS_FLARE_2 = 2,
	FX_LENS_FLARE_3 = 3,
	FX_GADGETRON = 4,
	FX_CIRCLE_FADED_EDGE_1 = 5,
	FX_LENS_FLARE_4 = 6,
	FX_CIRLCE_NO_FADED_EDGE = 7,
	FX_CIRCLE_OUTLINE_1 = 8,
	FX_FOUR_BLACK_CIRCLES = 9,
	FX_LAVA = 10,
	FX_CIRCLE_FADED_EDGE_2 = 11,
	FX_CIRCLE_ODD = 12,
	FX_RETICLE_1 = 13,
	FX_LINE_WIGGLY = 14,
	FX_LINE_WIDE = 15,
	FX_LINE_SKINNY = 16,
	FX_RETICLE_2 = 17,
	FX_RETICLE_3 = 18,
	FX_STAR = 19,
	FX_LIGHTNING_FADED = 20,
	FX_UNK_1 = 21,
	FX_SQUARE_FLAT_1 = 22,
	FX_BASELIGHT = 23,
	FX_SQUARE_FLAT_2 = 24,
	FX_SQUARE_OUTLINE_SMALL = 25,
	FX_DOTS = 26,
	FX_RETICLE_4 = 27,
	FX_VISIBOMB_HORIZONTAL_LINES = 28,
	FX_UNK_2 = 29,
	FX_SQUARE_WHITE_WITH_TRANSPARENT_DOTS = 30,
	FX_RETICLE_5 = 31,
	FX_RETICLE_BLITZ_CANNON = 32,
	FX_RETICLE_LAVA_GUN = 33,
	FX_RETICLE_6 = 34,
	FX_RETICLE_7 = 35,
	FX_RETICLE_SWINGSHOT = 36,
	FX_RETICLE_8 = 37,
	FX_RETICLE_9 = 38,
	FX_RETICLE_10 = 39,
	FX_RETICLE_11 = 40,
	FX_CIRCLE_FADED_INSIDE = 41,
	FX_CIRCLE_SPIKY = 42,
	FX_LINE_WIDE_NONTRANSPARENT = 43,
	FX_CIRCLE_BURST_NONTRANSPARENT = 44,
	FX_RETICLE_12_NONTRANSPARENT = 45,
	FX_SMOKE_WHITE = 46,
	FX_LIGHTNING_1 = 47,
	FX_LIGHTNING_2 = 48,
	FX_LENS_FLARE_5 = 49,
	FX_UNK_3 = 50,
	FX_RETICLE_QUARTER_1 = 51,
	FX_RETICLE_QUARTER_2 = 52,
	FX_RETICLE_QUARTER_3 = 53,
	FX_CIRCLE_OUTLINE_2 = 54,
	FX_CIRCLE_BURST_1 = 55,
	FX_CIRCLE_BURST_2 = 56,
	FX_EXPLOSION_1 = 57,
	FX_TRIANGLE_SMOKE = 58,
	FX_CIRCLE_FADED_EDGE_3 = 59,
	FX_LIGHTNING_3 = 60,
	FX_CIRCLEOUTLINE_3 = 61,
	FX_SHOT_1 = 62,
	FX_SHOT_2 = 63,
	FX_RETICLE_13 = 64,
	FX_SNOW = 65,
	FX_CIRCLE_QUARTER = 66,
	FX_RIFT_INDUCER_SPIRAL = 67,
	FX_RETICLE_14 = 68,
	FX_TIRE_TRACKS = 69,
	FX_RETICLE_QUARTER_4 = 70,
	FX_CIRCLE_BURST_3 = 71,
	FX_CIRCLE_BURST_FADED = 72,
	FX_CIRCLE_OUTLINE_3 = 73,
	FX_CIRCLE_OUTLINE_4 = 74,
	FX_CIRCLE_SWIRLY = 75,
	FX_CIRCLE_OUTLINE_5 = 76,
	FX_SQUARE_FLAT_2 = 77,
	FX_LEAF_1 = 78,
	FX_LEAF_2 = 79,
	FX_LEAF_3 = 80,
	FX_LEAF_4 = 81,
	FX_LEAF_5 = 82,
	FX_LEAF_6 = 83,
	FX_LEAF_7 = 84,
	FX_CIRCLE_OUTLINE_6 = 85,
	FX_RETICLE_15 = 86,
	FX_RETICLE_16 = 87,
	FX_RETICLE_GRAVITY_BOMB_1 = 88,
	FX_HEALTH_ORB = 89,
	FX_RETICLE_17 = 90,
	FX_RETICLE_18 = 91,
	FX_AL = 92,
	FX_HELGA = 93,
	FX_QWARK = 94,
	FX_SKRUNCH = 95,
	FX_SKID_MCMARX = 96,
	FX_SLIM_COGNITO = 97,
	FX_SASHA = 98,
	FX_MAYOR = 99,

	// Map Determined:
	FX_MAP_100 = 100,
	FX_MAP_101 = 101,
	FX_MAP_102 = 102,
	FX_MAP_103 = 103,
	FX_MAP_104 = 104,
	FX_MAP_105 = 105
} EffectsTex_e;

typedef enum DrawType {
	DRAW_TYPE_NORMAL,
	DRAW_TYPE_BRIGHT,
	DRAW_TYPE_NONTRASPARENT,
} DrawyType_e;

enum TextAlign {
  TEXT_ALIGN_TOPLEFT,
  TEXT_ALIGN_TOPCENTER,
  TEXT_ALIGN_TOPRIGHT,
  TEXT_ALIGN_MIDDLELEFT,
  TEXT_ALIGN_MIDDLECENTER,
  TEXT_ALIGN_MIDDLERIGHT,
  TEXT_ALIGN_BOTTOMLEFT,
  TEXT_ALIGN_BOTTOMCENTER,
  TEXT_ALIGN_BOTTOMRIGHT,
};

enum FontWindowFlags
{
  FONT_WINDOW_FLAGS_NONE = 0,
  FONT_WINDOW_FLAGS_H_ALIGN_CENTER = 1 << 0,
  FONT_WINDOW_FLAGS_V_ALIGN_CENTER = 1 << 1,
  FONT_WINDOW_FLAGS_NO_DRAW = 1 << 2,
  FONT_WINDOW_FLAGS_SUBPIXEL = 1 << 3,
  FONT_WINDOW_FLAGS_NO_SCISSOR = 1 << 4
};

typedef enum FontNames {
	FONT_DEFAULT = 0,
	FONT_BOLD = 0,
	FONT_DEMI = 1,
	FONT_BOOT = 2,
	FONT_NAMES_COUNT = 3
} FontNames;

typedef float POINT[2] __attribute__((__aligned__(8)));

typedef struct RECT
{
    POINT TopLeft;
    POINT TopRight;
    POINT BottomLeft;
    POINT BottomRight;
} RECT;

struct FontWindow
{
  short windowTop;
  short windowBottom;
  short windowLeft;
  short windowRight;
  short textX;
  short textY;
  short maxWidth;
  short maxHeight;
  short lineSpacing;
  short flags;
  short subPixelX;
  short subPixelY;
  short shadowOffsetX;
  short shadowOffsetY;
};

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

typedef struct UV {
	float x; // U: horizontal position
	float y; // V: vertical position
} UV_t;

typedef struct QuadDef { // 0x90
/* 0x00 */ vec4 point[4];
/* 0x40 */ u32 rgba[4];
/* 0x50 */ UV_t uv[4];
/* 0x70 */ u64 clamp;
/* 0x78 */ u64 tex0;
/* 0x80 */ u64 tex1;
/* 0x88 */ u64 alpha;
} QuadDef;

typedef struct ScreenVBEffect { // 0x34
	/* 0x00 */ int enabled;
	/* 0x04 */ u32 BKG_color;
	/* 0x08 */ u64 BKG_alpha;
	/* 0x10 */ int unk_10;
	/* 0x14 */ int A_lines;
	/* 0x18 */ u32 A_color;
	/* 0x1c */ u64 A_alpha;
	/* 0x24 */ int B_lines;
	/* 0x28 */ u32 B_color;
	/* 0x2c */ u64 B_alpha;
} ScreenVBEffect;

typedef struct ScreenInsertEffect { // 0x10
	/* 0x0 */ int enabled;
	/* 0x4 */ u32 color;
	/* 0x8 */ u64 alpha;
} ScreenInsertEffect;

typedef struct ViewContext { // 0x270
	/* 0x000 */ int headerBase[6][4];
	/* 0x060 */ u64 headerTex;
	/* 0x070 */ u64 headerTexNoFog;
	/* 0x080 */ u64 headerGr;
	/* 0x090 */ u64 headerGrNoFog;
	/* 0x0a0 */ float D;
	/* 0x0a4 */ float F;
	/* 0x0a8 */ float xclipratio;
	/* 0x0ac */ float yclipratio;
	/* 0x0b0 */ float xratio;
	/* 0x0b4 */ float yratio;
	/* 0x0b8 */ float xradpad;
	/* 0x0bc */ float yradpad;
	/* 0x0c0 */ MATRIX fMtx;
	/* 0x100 */ MATRIX nfMtx;
	/* 0x140 */ MATRIX hMtx;
	/* 0x180 */ VECTOR hmgScale;
	/* 0x190 */ VECTOR ihmgScale;
	/* 0x1a0 */ VECTOR hvdf;
	/* 0x1b0 */ VECTOR unSquish;
	/* 0x1c0 */ VECTOR guard;
	/* 0x1d0 */ VECTOR iguard;
	/* 0x1e0 */ VECTOR sphereCheckA;
	/* 0x1f0 */ VECTOR sphereCheckB;
	/* 0x200 */ VECTOR sphereCheckC;
	/* 0x210 */ float xpix;
	/* 0x214 */ float ypix;
	/* 0x218 */ float xclip;
	/* 0x21c */ float yclip;
	/* 0x220 */ float fog0;
	/* 0x224 */ float fog1;
	/* 0x228 */ float fogNearDist;
	/* 0x22c */ float fogFarDist;
	/* 0x230 */ float fogMult;
	/* 0x234 */ float fogAdd;
	/* 0x238 */ float fogNearIntensity;
	/* 0x23c */ float fogFarIntensity;
	/* 0x240 */ int scis_l;
	/* 0x244 */ int scis_r;
	/* 0x248 */ int scis_t;
	/* 0x24c */ int scis_b;
	/* 0x250 */ int fogR;
	/* 0x254 */ int fogG;
	/* 0x258 */ int fogB;
	/* 0x25c */ int backR;
	/* 0x260 */ int backG;
	/* 0x264 */ int backB;
	/* 0x268 */ int splitScreenMode;
} ViewContext;

struct DataSource { // 0x8
	/* 0x0 */ int m_state;
	/* 0x4 */ void *__vtable;
};

typedef struct Screen { // 0x50
	/* 0x00 */ int size_x;
	/* 0x04 */ int size_y;
	/* 0x08 */ int center_x;
	/* 0x0c */ int center_y;
	/* 0x10 */ int ofs_x;
	/* 0x14 */ int ofs_y;
	/* 0x18 */ int lim_x;
	/* 0x1c */ int lim_y;
	/* 0x20 */ float oneOverSize_x;
	/* 0x24 */ float oneOverSize_y;
	/* 0x30 */ vec4 vHalfSize;
	/* 0x40 */ vec4 vOneOverSize;
} Screen;

typedef struct ConcretePreLoadedImageBuffer {
	/* 0x00 */ struct DataSource DataSourceImageBuffer;
	/* 0x08 */ unsigned int tex0;
	/* 0x0c */ void *m_memory_buffer;
	/* 0x10 */ void *m_decompressed_buffer;
} ConcretePreLoadedImageBuffer;

typedef struct PartInstance { // 0x40
	/* 0x00 */ char i_class;
	/* 0x01 */ char type;
	/* 0x02 */ char tex;
	/* 0x03 */ char gs_alpha;
	/* 0x04 */ int rgba;
	/* 0x08 */ char rot;
	/* 0x09 */ char draw_dist;
	/* 0x0a */ short int timer;
	/* 0x0c */ float scale;
	/* 0x10 */ VECTOR pos;
	/* 0x20 */ int update[8];
} PartInstance_t;

typedef struct ColorExtTable {
/* 0x00 */ u32 normal; // aka default
/* 0x04 */ u32 blue;
/* 0x08 */ u32 green;
/* 0x0c */  u32 pink;
/* 0x10 */  u32 white;
/* 0x14 */  u32 gray;
/* 0x18 */  u32 black1;
/* 0x1c */  u32 black2;
} ColorExtTable_t;


typedef struct SUB_RETICULE { // 0x40
	/* 0x00 */ VECTOR targetPos;
	/* 0x10 */ float scale;
	/* 0x14 */ int rgba;
	/* 0x18 */ int fxTex;
	/* 0x1c */ float rotAmt;
	/* 0x20 */ Moby *pMoby;
	/* 0x24 */ char type;
	/* 0x25 */ char cameraIndex;
	/* 0x26 */ short int segments;
	/* 0x28 */ float segment_rot;
	/* 0x2c */ int draw_style;
	/* 0x30 */ float retX;
	/* 0x34 */ float retY;
	/* 0x38 */ float retWidth;
	/* 0x3c */ float retHeight;
} SUB_RETICULE;

struct RETICULE { // 0x390
	/* 0x000 */ SUB_RETICULE instance[14];
	/* 0x380 */ int numActive;
	/* 0x384 */ int pad[3];
};

/*
 * NAME :		drawFunction
 * DESCRIPTION :
 * 			Calls the normal draw function.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void drawFunction(void);

/*
 * NAME :		gfxScreenSpaceText
 * DESCRIPTION :
 * 			Draws the text on the screen.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int gfxScreenSpaceText(float x, float y, float scaleX, float scaleY, u32 color, const char * string, int length, int alignment, int font);
int gfxScreenSpaceTextCenter(float x, float y, u32 color, const char * string, int length);
void gfxScreenSpaceTextWindow(struct FontWindow* fontWindow, float scaleX, float scaleY, u32 color, const char * string, int length, u32 shadowColor);

/*
 * NAME :		gfxScreenSpaceBox
 * DESCRIPTION :
 * 			Draws a quad on the screen.
 * NOTES :
 * ARGS : 
 *      rect:           All four corners of quad. Use values 0-1 for x and y position.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void gfxScreenSpaceQuad(RECT * rect, u32 colorTL, u32 colorTR, u32 colorBL, u32 colorBR);

/*
 * NAME :		gfxScreenSpaceBox
 * DESCRIPTION :
 * 			Draws a box on the screen.
 * NOTES :
 * ARGS : 
 *      x:              Screen X position (0-1).
 *      y:              Screen Y position (0-1).
 *      w:              width (0-1).
 *      h:              height (0-1).
 *      color:          color of box.
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void gfxScreenSpaceBox(float x, float y, float w, float h, u32 color);

int gfxGetFontWidth(const char * string, int length, float scale);
void gfxPixelSpaceBox(float x, float y, float w, float h, u32 color);

int gfxGetIsProgressiveScan(void);
void gfxSetIsProgressiveScan(int on);

/*
 * NAME :		gfxOcclusion
 * DESCRIPTION :
 * 			Turn on/off Occlusion (Render All)
 * NOTES :
 * ARGS : 
 *          OnOff: 0 = Show All, 2 = Default
 * RETURN :
 * AUTHOR :			Troy "Metroynome" Pruitt
 */
void gfxOcclusion(int OnOff);

void gfxDoGifPaging(void);
void gfxSetupGifPaging(int);
u64 gfxGetFrameTex(int id);
u64 gfxGetEffectTex(int id);

/*
 * NAME:			gfxSetupEffectTex
 * DESCRIPTION:		Gets Effect Textures info.  (tex0, tex1, clamp, alpha)
 * NOTES:
 * ARGS:
 * RETURN:
 * AUTHOR:			Troy "Metroynome" Pruitt
 */
void gfxSetupEffectTex(QuadDef *quad, int tex, int drawType, int opacity);
void gfxDrawSprite(float x, float y, float w, float h, int tex_x, int tex_y, int tex_w, int tex_h, u64 color, u64 texture);
void gfxDrawEffect(float x, float y, float w, float h, int r, int tex_x, int tex_y, int tex_w, int tex_h, u64 texture, u64 color1, u64 color2, char bSetting1, char bSetting2);
void gfxRegistserDrawFunction(void* callback, Moby* moby);
void gfxDrawBillboardQuad(float scale, float scale2, float theta, VECTOR position, int tex, int color, int drawType);
void gfxDrawQuad(QuadDef quad, MATRIX worldMatrix);
void gfxSetScissor(int xmin, int xmax, int ymin, int ymax);
int gfxConstructEffectTex(int texGsAddr, int palGsAddr, int ulog, int vlog, int format);
int gfxLoadPalToGs(void* ptr, int format);
int gfxLoadTexToGs(void* ptr, int ulog, int vlog, int format);
PartInstance_t * gfxSpawnParticle(VECTOR position, u32 texId, u32 color, char opacity, float rotation);
void gfxHelperAlign(float* pX, float* pY, float w, float h, enum TextAlign alignment);
void gfxHelperDrawSprite_WS(VECTOR worldPosition, float w, float h, int texId, u32 color, enum TextAlign alignment);
void gfxHelperDrawTextWindow(float x, float y, float width, float height, float textOffsetX, float textOffsetY, float scale, u32 color, char* str, int length, enum TextAlign alignment, enum FontWindowFlags flags);
Moby *gfxGetRegisteredDrawMobyList(void);
void *gfxGetRegisteredDrawCalbackList(void);
int gfxGetRegisteredDrawCount(void);
void gfxDrawStrip(int texId, vec3 *pos, int *rbga, struct UV *uv, int unk);
void gfxDrawStripInit(void);
void gfxAddRegister(int register, u64 value);

//
ScreenVBEffect* gfxScreenVBEffect(void);
ScreenInsertEffect* gfxScreenInsertEffect(void);
ViewContext* gfxViewContext(void);
ConcretePreLoadedImageBuffer* gfxGetPreLoadedImageBufferSource(int which);
ColorExtTable_t* gfxColorExtTable(void);
void gfxDrawScreenOverlay(int r, int g, int b, int a);
#endif // _LIBUYA_GRAPHICS_H_
