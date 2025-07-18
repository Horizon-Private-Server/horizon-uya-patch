#include <tamtypes.h>

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
#include <libuya/time.h>
#include <libuya/sound.h>
#include "utils.h"
#include "config.h"

void runTimer(TimerVars_t *timer)
{
    int gameTime = gameGetTime();
    char timerBuf[16];

    if (timer->status == -1 || timer->status == 2)
        return;
    
    if (timer->status == 0) {
        timer->startTime = gameTime + (timer->timeValue * TIME_SECOND);
        timer->lastPlayedTickSound = gameTime + (timer->timeStartTicking * TIME_SECOND);
    }

    int timeLeft = (timer->startTime) - (gameTime);
    float timeSecondsLeft = timeLeft / (float)TIME_SECOND;
    float timerScale = timer->timerScale;
    u32 timerColor = timer->colorBase;
    int timeSecondsLeftFloor = (int)timeSecondsLeft;
    float timeSecondsRounded = timeSecondsLeftFloor;
    if ((timeSecondsLeft - timeSecondsRounded) > 0.5)
        timeSecondsRounded += 1;

    if (timeLeft <= 0) {
        timer->status = 2;
    } else {
        // draw subtext
        if (strlen(timer->title) > 0)
            gfxScreenSpaceText(timer->title_x, timer->title_y, timer->titleScale, timer->titleScale, timer->colorBase, timer->title, -1, 4, timer->font);

        // draw timer
        if (timer->bDynamicScaleTime) {
            float t = 1 - fabsf(timeSecondsRounded - timeSecondsLeft);
            float x = powf(t, 15);
            timerScale *= (1.0 + (0.3 * x));
            timerColor = colorLerp(timer->colorBase, timer->colorHigh, x);
        }
        int formatTime = timeLeft * (60.0 / TIME_SECOND);
        if (timer->timeValue >= 60) {
            sprintf(timerBuf, "%02i:%02i:%02i", (formatTime / 60) / 60, (formatTime / 60) % 60, ((formatTime % 60) * 100) / 60);
        } else {
            sprintf(timerBuf, "%02i.%02i", (formatTime / 60) % 60, ((formatTime % 60) * 100) / 60);
        }
        gfxScreenSpaceText(timer->timer_x, timer->timer_y, timerScale, timerScale, timerColor, timerBuf, -1, 4, timer->font);

        // tick timer
        if (timer->tickSound != 0 && timeSecondsLeftFloor < timer->lastPlayedTickSound && timeSecondsLeftFloor < timer->timeStartTicking) {
            timer->lastPlayedTickSound = timeSecondsLeftFloor;
            soundPlay(timer->tickSound, 0, playerGetFromSlot(0)->pMoby, 0, 1024);
        }
        // set timer status
        timer->status = 1;
    }
}
