#ifndef _PATCH_UTILS_
#define _PATCH_UTILS_

#include <libuya/graphics.h>
#include <libuya/math.h>
#include <libuya/math3d.h>
#include <libuya/time.h>

typedef enum TimerStatus {
    TIMER_STATUS_DISABLED = -1,
    TIMER_STATUS_RESET = 0,
    TIMER_STATUS_ACTIVE = 1,
    TIMER_STATUS_COMPLETE = 2
} TimerStatus;

typedef struct TimerVars {
    float timer_x;
    float timer_y;
    float timerScale;
    float title_x;
    float title_y;
    float titleScale;
    char title[32];
    u32 colorBase;
    u32 colorHigh;
    int font;
    int timeValue;
    int timeStartTicking;
    SoundDef *tickSound;
    int startTime;
    int lastPlayedTickSound;
    short bDynamicScaleTime;
    short status;
} TimerVars_t;

void runTimer(TimerVars_t *timer);

#endif // _PATCH_UTILS_