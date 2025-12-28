#ifndef _LIBUYA_HUD_H_
#define _LIBUYA_HUD_H_

#include <tamtypes.h>
#include "common.h"
#include "math3d.h"
#include "graphics.h"

typedef unsigned int HANDLE_ID;

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
	WIDGET_3D = 6,
	WIDGIET_FRAME_CONTAINER = 7
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

struct Canvas {
/* 0x0 */ CanvasData_t *data;
};

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

struct iFrame {
/* 0x00 */ struct iObject object;
/* 0x0c */ u32 controlFlags;
/* 0x10 */ vec2f pos;
/* 0x18 */ vec2f scale;
/* 0x20 */ vec2f dropshadow_offset;
/* 0x28 */ float alpha;
/* 0x2c */ float rot;
/* 0x30 */ int unk_30;
/* 0x34 */ u32 dropshadow_color;
/* 0x38 */ u32 color;
};

typedef struct WidgetRectangle { // 60
/* 0x00 */ struct iFrame frame;
/* 0x40 */ float rotation;
/* 0x44 */ u32 color1;
/* 0x48 */ u32 color2;
/* 0x4c */ u32 color3;
/* 0x50 */ u16 textureId;
/* 0x52 */ u16 frameNum;
/* 0x54 */ char total_fade_frames;
/* 0x55 */ char fade_frame;
/* 0x56 */ char renderstate;
/* 0x57 */ char pad;
/* 0x58 */ struct DataSource *datasource;
} WidgetRectangle_t;

typedef struct WidgetHallowRectangle { // 0x48
/* 0x00 */ struct iFrame frame;
/* 0x40 */ vec2f insets;
} WidgetHallowRectangle_t;

typedef struct WidgetTextGraphic { // 0x40
/* 0x00 */ struct iGraphicsObject object;
/* 0x28 */ struct fontLetter *pFontTable;
/* 0x30 */ u64 font_FX;
/* 0x38 */ char *mExternalString;
/* 0x3c */ TextAlignment_e textAlignment;
} WidgetTextGraphic_t;

typedef struct WidgetTextFrame { // 0x70
/* 0x00 */ struct iFrame frame;
/* 0x40 */ vec2f rot;
/* 0x48 */ int fontTable;
/* 0x4c */ char *pExternalStringMemory;
/* 0x50 */ unsigned int font_FX;
/* 0x54 */ unsigned char widget_text_control;
/* 0x58 */ float maxWidth;
/* 0x5c */ float minimumScale;
/* 0x60 */ unsigned char pointSize;
/* 0x64 */ int fontText;
/* 0x68 */ vec2f cached_scale;
} WidgetTextFrame_t;

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

typedef struct WidgetTextArea { // 0x50
/* 0x00 */ struct iFrame frame;
/* 0x40 */ WidgetTextAreaData_t *data;
/* 0x44 */ bool bTextSizesDirty;
/* 0x48 */ float text_w;
/* 0x4c */ float text_h;
} WidgetTextArea_t;

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
int hudGetCurrentCanvas(void);

bool hudAddToContainer(unsigned int container_id, unsigned int frame_id);
bool hudCreateRectangle(float x, float y, float w, float h, unsigned int handle_id, u32 color,int sprite);
bool hudCreateText(float x, float y, float w, float h, unsigned int handle_id, char *pText, u32 color);
bool hudCreateTextArea(float x, float y, float w, float h, float text_scale, unsigned int handle_id, char *pText, u32 color);

bool hudSetScale(float width, float height, unsigned int handle_id);
bool hudSetPosition(float x, float y, unsigned int handle_id);
bool hudSetColor(unsigned int handle_id, u32 color);
bool hudSetSprite(unsigned int handle_id, SpriteTex_Hud_e sprite);
bool hudSetFlags(unsigned int handle_id, unsigned int flags, bool value);
bool hudSetTextScale(float scale, unsigned int handle_id);

ConcretePreLoadedImageBuffer_t* gfxGetPreLoadedImageBufferSource(int which);

#endif // _LIBUYA_HUD_H_