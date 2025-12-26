#ifndef _LIBUYA_HUD_H_
#define _LIBUYA_HUD_H_

#include <tamtypes.h>
#include "common.h"

typedef enum eEngineState {
    ENGINE_IDLE = 0,
	ENGINE_READY = 1,
	ENGINE_PAUSED = 2
} EngineState_e;

struct MapOffsets { // 0x20
/* 0x00 */ float min_x;
/* 0x04 */ float min_y;
/* 0x08 */ float pad_08[2];
/* 0x10 */ float max_x;
/* 0x14 */ float max_y;
/* 0x08 */ float pad_18[2];
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
/* 0x0*/ CanvasData_t *data;
};

struct iHeap { // 0x4
/* 0x0 */ int *vtable;
};

struct DataSource {
/* 0x0 */ int state;
/* 0x4 */ int *vtable;
};

struct EngineData { // 0x78
/* 0x00 */ struct Canvas *canvases[5];
/* 0x14 */ int current_canvas;
/* 0x18 */ struct iHeap *heaps[2];
/* 0x20 */ struct DataSource *data_source[12];
/* 0x50 */ void *postdraws[4];
/* 0x60 */ int postdraw_counter;
/* 0x64 */ void *predraws[4];
/* 0x74 */ int predraw_counter;
};

struct Engine {
/* 0x0 */ int lockable;
/* 0x4 */ struct EngineData *data
};

typedef struct ConcretePreLoadedImageBuffer {
/* 0x00 */ struct DataSource dataSourceImageBuffer;
/* 0x08 */ unsigned int tex0;
/* 0x0c */ void *memory_buffer;
/* 0x10 */ void *decompressed_buffer;
} ConcretePreLoadedImageBuffer_t;

// returns pointer to radar map settings.
HudMap_t *hudGetMapData(void);
ConcretePreLoadedImageBuffer_t* gfxGetPreLoadedImageBufferSource(int which);

#endif // _LIBUYA_HUD_H_
