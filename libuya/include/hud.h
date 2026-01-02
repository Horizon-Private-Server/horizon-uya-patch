#ifndef _LIBUYA_HUD_H_
#define _LIBUYA_HUD_H_

#include <tamtypes.h>
#include "common.h"
#include "math3d.h"
#include "graphics.h"

typedef unsigned int HANDLE_ID;

typedef enum {
    // Radar widgets (0x50000 - 0x500FF)
    HUD_RADAR_ROOT          = 0x50000,
    HUD_RADAR_MAP_CONTAINER = 0x50001,
    HUD_RADAR_MAP_BG        = 0x50006,
    HUD_RADAR_CURSOR_SELECT = 0x50007,
    HUD_RADAR_CURSOR_PLAYER = 0x50008,
    HUD_RADAR_BASE_TEAM1    = 0x50009,
    HUD_RADAR_BASE_TEAM2    = 0x5000A,
    
    HUD_RADAR_NODE_CONTAINER = 0x50002,
    HUD_RADAR_NODE_START     = 0x5000B,
    HUD_RADAR_NODE_END       = 0x50012,
    
    HUD_RADAR_VEHICLE_CONTAINER = 0x50003,
    HUD_RADAR_VEHICLE_START     = 0x5001B,
    HUD_RADAR_VEHICLE_END       = 0x50022,
    
    HUD_RADAR_PLAYER_CONTAINER = 0x50004,
    HUD_RADAR_PLAYER_START     = 0x50013,
    HUD_RADAR_PLAYER_END       = 0x5001A,
    
    HUD_RADAR_TEXT_CONTAINER = 0x50005,
    HUD_RADAR_TEXT_RESPAWN   = 0x50023,
    HUD_RADAR_TEXT_NODE_SEL  = 0x50024,
    
    // Siege/CTF HUD widgets (0x70000 - 0x700FF)
    HUD_SIEGE_ROOT           = 0x70000,
    HUD_SIEGE_MAIN_CONTAINER = 0x70001,  // Main score container
    HUD_SIEGE_FRAME_BG_1     = 0x70002,  // Background frame (blue)
    HUD_SIEGE_FRAME_BG_2     = 0x70003,  // Background frame (darker blue)
    HUD_SIEGE_BAR_RED        = 0x70004,  // Red team bar
    HUD_SIEGE_BAR_BLUE       = 0x70005,  // Blue team bar
    HUD_SIEGE_BAR_BLUE_BG    = 0x70006,  // Blue bar background
    HUD_SIEGE_BAR_RED_BG     = 0x70007,  // Red bar background
    HUD_SIEGE_ICON_1         = 0x70008,
    HUD_SIEGE_BAR_BG_2       = 0x70009,
    HUD_SIEGE_BAR_FG_2       = 0x7000A,
    HUD_SIEGE_NODE_1         = 0x7000B,  // Node indicators start
    HUD_SIEGE_NODE_2         = 0x7000C,
    HUD_SIEGE_NODE_3         = 0x7000D,
    HUD_SIEGE_NODE_4         = 0x7000E,
    HUD_SIEGE_NODE_5         = 0x7000F,
    HUD_SIEGE_NODE_6         = 0x70010,
    HUD_SIEGE_NODE_7         = 0x70011,  // Node indicators end
    HUD_CTF_CONTAINER        = 0x70012,  // CTF-specific container
    HUD_CTF_FRAME_BG_1       = 0x70013,
    HUD_CTF_FRAME_BG_2       = 0x70014,
    HUD_CTF_ICON             = 0x70015,
    HUD_CTF_BAR_BLUE         = 0x70016,
    HUD_CTF_BAR_RED          = 0x70017,
    HUD_CTF_TEXT_1           = 0x70018,
    HUD_CTF_TEXT_2           = 0x70019,
    HUD_SIEGE_FLAG_CONTAINER = 0x7001A,  // Flag/node indicator container
    HUD_SIEGE_FLAG_1         = 0x7001B,
    HUD_SIEGE_FLAG_2         = 0x7001C,
    HUD_SIEGE_FLAG_3         = 0x7001D,
    HUD_SIEGE_FLAG_4         = 0x7001E,
    HUD_SIEGE_FLAG_5         = 0x7001F,
    HUD_SIEGE_FLAG_6         = 0x70020,
    HUD_SIEGE_FLAG_7         = 0x70021,
    HUD_SIEGE_FLAG_8         = 0x70022,
    
    // Health HUD widgets (0x51000 - 0x510FF)
    HUD_HEALTH_ROOT          = 0x51000,
    HUD_HEALTH_BAR_BG        = 0x51001,
    HUD_HEALTH_BAR_FILL      = 0x51002,
    // ... more health widgets
    
    // Weapon HUD widgets (0x52000 - 0x520FF)
    HUD_WEAPON_ROOT          = 0x52000,
    // ... weapon widgets
    
} HudWidgetId_e;

typedef struct HudEntry {
    HANDLE_ID parent;
    HANDLE_ID child;
} HudEntry_t;

typedef enum eEngineState {
    ENGINE_IDLE = 0,
    ENGINE_READY = 1,
    ENGINE_PAUSED = 2
} EngineState_e;

typedef enum eTextAlignment {
	TEXT_ALIGN_LEFT = 0,
	TEXT_ALIGN_CENTER = 1,
	TEXT_ALIGN_RIGHT = 2,
	TEXT_ALIGN_MAX = 3
} TextAlignment_e;

typedef enum WIDGET_TYPE {
	WIDGET_IFRAME = 1,
	WIDGET_TEXT = 2,
	WIDGET_TEXTAREA = 3,
	WIDGET_RECTANGLE = 4,
	WIDGET_HOLLOW_RECTANGLE = 5,
	WIDGET_3D_2D = 6,
	WIDGET_FRAME_CONTAINER = 7
};

struct MapOffsets { // 0x20
/* 0x00 */ float min_x;
/* 0x04 */ float min_y;
/* 0x08 */ float pad_08[2];
/* 0x10 */ float max_x;
/* 0x14 */ float max_y;
/* 0x18 */ float pad_18[2];  // Fixed: was 0x08, should be 0x18
};

typedef struct HudMap {
/* 0x000 */ u32 heroBlipColor;
/* 0x004 */ int pad_004;
/* 0x008 */ u32 enemyBlipColor;
/* 0x00c */ int pad_00c;
/* 0x010 */ u32 defaultNodeColor;
/* 0x014 */ char unk_014[0xc]; 
/* 0x020 */ u32 defaultPlayerCursorColor;
/* 0x024 */ char unk_024[0xc];
/* 0x030 */ struct MapOffsets offset[6];
} HudMap_t;

struct BoundingRectangle {
/* 0x00 */ float left;
/* 0x04 */ float top;
/* 0x08 */ float right;
/* 0x0c */ float bottom;
/* 0x10 */ float translate_x;
/* 0x14 */ float translate_y;
};

typedef struct GuiState {
/* 0x00 */ EngineState_e currentState;
/* 0x04 */ void *init;
/* 0x08 */ void *deinit;
/* 0x0c */ void *update;
/* 0x10 */ void *pre_render;
/* 0x14 */ void *post_render;
} GuiState_t;

typedef struct CanvasData {
/* 0x0000 */ struct BoundingRectangle rect;
/* 0x0018 */ unsigned int timeline_ids[16];
/* 0x0058 */ int *top_layer_frames[35];
/* 0x00e4 */ GuiState_t *guiStateCell[16];
/* 0x0124 */ GuiState_t *guiStateSwapToCell[16];
/* 0x0164 */ GuiState_t *returnState[16];
/* 0x01a4 */ bool returned[16];
/* 0x01b4 */ bool requestedKillThread[16];
/* 0x01c4 */ int currentThread;
/* 0x01c8 */ int canvas_id;
/* 0x01cc */ char hashIndex[0x2008];
/* 0x21d4 */ char hashTimelineIndex[0x188];
/* 0x235c */ char eventQue[0x310];
/* 0x266c */ unsigned int current_channel_id_counter;
/* 0x2670 */ int norender_counter;
/* 0x2674 */ void *ige_data;
/* 0x2678 */ unsigned int data_layer;
} CanvasData_t;

typedef struct Canvas {
/* 0x0 */ CanvasData_t *data;
} Canvas_t;

struct iHeap { // 0x4
/* 0x0 */ int *vtable;
};

struct DataSource {
/* 0x0 */ int state;
/* 0x4 */ int *vtable;
};

typedef struct EngineData { // 0x78  // Fixed: removed duplicate "struct" keyword
/* 0x00 */ struct Canvas *canvases[5];
/* 0x14 */ int current_canvas;
/* 0x18 */ struct iHeap *heaps[2];
/* 0x20 */ struct DataSource *data_source[12];
/* 0x50 */ void *postdraws[4];
/* 0x60 */ int postdraw_counter;
/* 0x64 */ void *predraws[4];
/* 0x74 */ int predraw_counter;
} EngineData_t;

typedef struct Engine {
/* 0x0 */ int lockerId;
/* 0x4 */ EngineData_t *data;
} Engine_t;

typedef struct ConcretePreLoadedImageBuffer {
/* 0x00 */ struct DataSource dataSourceImageBuffer;
/* 0x08 */ unsigned int tex0;
/* 0x0c */ void *memory_buffer;
/* 0x10 */ void *decompressed_buffer;
} ConcretePreLoadedImageBuffer_t;

struct HudWidgetTex { // 0x10
/* 0x0 */ u64 tex;
/* 0x8 */ short unsigned int tex_ofs;
/* 0xa */ short unsigned int pal_ofs;
/* 0xc */ short int u_log;
/* 0xe */ short int v_log;
};

struct HudWidget { // 0x30
/* 0x00 */ short int vert_cnt;
/* 0x02 */ short int poly_cnt;
/* 0x04 */ short int joint_cnt;
/* 0x06 */ short int tex_cnt;
/* 0x08 */ short int frame_cnt;
/* 0x0a */ short int frame_size;
/* 0x0c */ float scale;
/* 0x10 */ short int *verts;
/* 0x14 */ char *polys;
/* 0x18 */ struct HudWidgetTex *tex_defs;
/* 0x1c */ void *tex_data;
/* 0x20 */ void *frames;
/* 0x24 */ int vert_format;
/* 0x28 */ int rgba;
/* 0x2c */ int pad;
};

struct Event { // 0xc
/* 0x0 */ HANDLE_ID targetWidget;
/* 0x4 */ unsigned int eventType;
/* 0x8 */ unsigned int data;
};

struct EventArray { // 0xc0
/* 0x00 */ struct Event m_events[16];
};

struct EventResponseTOC { // 0xc0
/* 0x00 */ struct EventArray *m_array[48];
};

struct fontLetter { // 0x4
/* 0x0 */ unsigned char u;
/* 0x1 */ unsigned char v;
/* 0x2 */ signed char drop;
/* 0x3 */ signed char spacing;
};

struct FreeNode { // 0x4
/* 0x0 */ struct FreeNode *mpNext;
};

struct ConstantBlockSizeMemoryManager { // 0x18
/* 0x00 */ unsigned char *staticBlock;
/* 0x04 */ unsigned int heapSize;
/* 0x08 */ unsigned int allocSize;
/* 0x0c */ unsigned int used;
/* 0x10 */ unsigned int currentlyAllocated;
/* 0x14 */ struct FreeNode *freeList;
};

struct iObject {
/* 0x0 */ u32 refCount;
/* 0x4 */ u8 control_flags;
/* 0x8 */ int *vtable;
};

struct iGraphicsObject { // 0x28
/* 0x00 */ vec4f *positionChannel;
/* 0x04 */ vec4f *scaleChannel;
/* 0x08 */ vec4f *rotationChannel;
/* 0x0c */ vec4f *globalColorChannel;
/* 0x10 */ vec4f *visibilityChannel;
/* 0x14 */ bool bExternalPositionChannel;
/* 0x15 */ bool bExternalScaleChannel;
/* 0x16 */ bool bExternalRotationChannel;
/* 0x17 */ bool bExternalGlobalColorChannel;
/* 0x18 */ bool bExternalVisibilityChannel;
/* 0x1c */ char *myName;
/* 0x20 */ struct ConstantBlockSizeMemoryManager *channelPoolManager;
/* 0x24 */ int *vtable;
};

struct iFrame { // 0x30
/* 0x00 */ struct iObject object;
/* 0x0c */ u32 controlFlags;
/* 0x10 */ vec2f pos;
/* 0x18 */ vec2f scale;
/* 0x20 */ vec2f dropshadow_offset;
/* 0x28 */ u32 dropshadow_color;
/* 0x2c */ u32 color;
};

typedef struct WidgetRectangle { // 0x54
/* 0x00 */ struct iFrame frame;
/* 0x30 */ float rotation;
/* 0x34 */ int unk_34;
/* 0x38 */ u32 color1;
/* 0x3c */ u32 color2;
/* 0x40 */ u32 color3;
/* 0x44 */ u32 color4;
/* 0x48 */ u16 textureId;
/* 0x4a */ u16 currFrame;
/* 0x4c */ char total_fade_frames;
/* 0x4d */ char fade_frame;
/* 0x4e */ char renderstate;
/* 0x4f */ char pad;
/* 0x50 */ struct DataSource *datasource;
} WidgetRectangle_t;

typedef struct WidgetHallowRectangle { // 0x3c
/* 0x00 */ struct iFrame frame;
/* 0x30 */ char unk_30[0x4];
/* 0x34 */ vec2f insets;
} WidgetHallowRectangle_t;

typedef struct WidgetTextGraphic { // 0x40
/* 0x00 */ struct iGraphicsObject object;
/* 0x28 */ struct fontLetter *pFontTable;
/* 0x30 */ u64 font_FX;
/* 0x38 */ char *mExternalString;
/* 0x3c */ TextAlignment_e textAlignment;
} WidgetTextGraphic_t;

typedef struct WidgetText { // 0x64
/* 0x00 */ struct iFrame frame;
/* 0x30 */ int unk_30;
/* 0x34 */ vec2f rot;
/* 0x3c */ int fontTable;
/* 0x40 */ char *pExternalStringMemory;
/* 0x44 */ unsigned int font_FX;
/* 0x48 */ u32 widget_text_control;
/* 0x4c */ float maxWidth;
/* 0x50 */ float minimumScale;
/* 0x54 */ u32 pointSize;
/* 0x58 */ int fontText;
/* 0x5c */ vec2f cached_scale;
} WidgetText_t;

typedef struct WidgetTextAreaData { // 0x54
/* 0x00 */ vec2f rot;
/* 0x08 */ int FontTable;
/* 0x0c */ char *pExternalStringMemory;
/* 0x10 */ char **pExternalStringListHandle;
/* 0x14 */ short int stringListCnt;
/* 0x16 */ short int selectedIdx;
/* 0x18 */ unsigned int font_FX;
/* 0x1c */ bool bAutoScale;
/* 0x1e */ short unsigned int max_width;
/* 0x20 */ float min_scale;
/* 0x24 */ float text_scale;
/* 0x28 */ vec2f selectedLine;
/* 0x30 */ u32 colorSelected;
/* 0x34 */ vec2f Scroll;
/* 0x3c */ vec2f line_spacing;
/* 0x40 */ short unsigned int widget_text_area_control;
/* 0x42 */ unsigned char pointSize;
/* 0x44 */ short int scroll_delay;
/* 0x48 */ float scroll_speed;
/* 0x4c */ short int scroll_repeat;
/* 0x4e */ short int scroll_delay_counter;
/* 0x50 */ char font_text;
/* 0x51 */ char auto_scroll_state;
/* 0x52 */ char did_first_pass;
} WidgetTextAreaData_t;

typedef struct WidgetTextArea { // 0x38
/* 0x00 */ struct iFrame frame;
/* 0x30 */ WidgetTextAreaData_t *data;
/* 0x34 */ bool bTextSizesDirty;
/* 0x38 */ float text_w;
/* 0x3c */ float text_h;
} WidgetTextArea_t;

struct Widget3d_2D { // 0x48
/* 0x00 */ struct iFrame frame;
/* 0x30 */ float rotation;
/* 0x34 */ float widget_frame;
/* 0x38 */ unsigned int widget_id;
/* 0x3c */ void *widget_data_source;
};

typedef struct Widget2D { // 0x20
/* 0x00 */ short int *positions;
/* 0x04 */ short int *uvs;
/* 0x08 */ int *rgbas;
/* 0x0c */ char *polys;
/* 0x10 */ short int vert_cnt;
/* 0x12 */ short int prim_cnt;
/* 0x14 */ char prim_type;
/* 0x15 */ char flags;
/* 0x16 */ short int frame_cnt;
/* 0x18 */ int ipad[2];
} Widget2D_t;

typedef struct Widget3D { // 0x3c
/* 0x00 */  struct iGraphicsObject object;
/* 0x28 */ vec4f *textureChannel;
/* 0x2c */ vec4f *animationFrameChannel;
/* 0x30 */ unsigned int geometryPtr;
/* 0x34 */ struct HudWidget *widget3dData;
/* 0x38 */ bool bExternalTextureChannel;
/* 0x39 */ bool bExternalAnimationChannel;
/* 0x3a */ bool bEnableSplitScreenHack;
} Widget3D_t;

typedef struct WidgetSprite { // 0x30
/* 0x00 */  struct iGraphicsObject object;
/* 0x28 */ vec4f *textureChannel;
/* 0x2c */ bool bExternalTextureChannel;
} WidgetSprite_t;

typedef struct WidgetTextGfx { // 0x40
/* 0x00 */ struct iGraphicsObject object;
/* 0x28 */ struct fontLetter *fontTable;
/* 0x30 */ u64 font_FX;
/* 0x38 */ char *pExternalString;
/* 0x3c */ TextAlignment_e textAlignment;
} WidgetTextGfx_t;

typedef struct WidgetSlider { // 0x3c
/* 0x00 */  struct iGraphicsObject object;
/* 0x28 */ bool hasOutline;
/* 0x2c */ int outlineScale;
/* 0x30 */ unsigned int maxScale;
/* 0x34 */ unsigned int maxValue;
/* 0x38 */ unsigned int sliderEmptyColor;
} WidgetSlider_t;

typedef struct WidgetWheel { // 0x54
/* 0x00 */ struct iFrame frame;
/* 0x40 */ vec2f rot;
/* 0x48 */ void *presenter;
/* 0x4c */ u32 color;
/* 0x50 */ int textureId;
} WidgetWheel_t;

// returns pointer to radar map settings.
HudMap_t *hudGetMapData(void);

Engine_t *hudGetInstance(void);
EngineData_t *hudGetEngineData(void);
int hudGetCurrentCanvasIndex(void);
Canvas_t *hudGetCurrentCanvas(void);
bool hudSetLayer(Canvas_t *canvas, int whichLayer, HANDLE_ID handle_id);
bool hudSetCurrentCanvasLayer(int whichLayer, HANDLE_ID handle_id);
bool hudClearLayer(Canvas_t *canvas, int whichLayer);
bool hudClearCurrentCanvasLayer(int whichLayer);

bool hudAddToContainer(HANDLE_ID container_id, HANDLE_ID frame_id);
bool hudCreateRectangle(float x, float y, float w, float h, HANDLE_ID handle_id, u32 color,int sprite);
bool hudCreateText(float x, float y, float w, float h, HANDLE_ID handle_id, char *pText, u32 color);
bool hudCreateTextArea(float x, float y, float w, float h, float text_scale, HANDLE_ID handle_id, char *pText, u32 color);
bool hudCreateFrame(float x, float y, float w, float h, float art_animation_frame, HANDLE_ID handle_id, int art_data_id, u32 color);

bool hudSetScale(float width, float height, HANDLE_ID handle_id);
bool hudSetPosition(float x, float y, HANDLE_ID handle_id);
bool hudSetColor(HANDLE_ID handle_id, u32 color);
bool hudSetSprite(HANDLE_ID handle_id, SpriteTex_Hud_e sprite);
bool hudSetFlags(HANDLE_ID handle_id, unsigned int flags, bool value);
bool hudSetTextScale(float scale, HANDLE_ID handle_id);
bool hudSetRotation(float rotation, HANDLE_ID handle_id);
bool hudSetDropShadowOffset(float x, float y, HANDLE_ID handle_id);
bool hudSetAligFlags(HANDLE_ID handle_id, unsigned int align_x, unsigned int align_y);

ConcretePreLoadedImageBuffer_t* gfxGetPreLoadedImageBufferSource(int which);

#endif // _LIBUYA_HUD_H_