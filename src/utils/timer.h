#pragma once

#include <assert.h>

#include "global.h"

typedef struct Timer {
        long long start;
        long long elapsed;
        long long paused;
        static_assert(sizeof(long long) >= sizeof(double),
                      "Expected sizeof(long long) >= 8");
} Timer;

void timer_start(Timer *timer);

void timer_update(Timer *timer);

void timer_pause(Timer *timer);

void timer_resume(Timer *timer);

void timer_get_hour_min_sec(long long time, unsigned int *hour,
                            unsigned int *min, unsigned int *sec);

void timer_darw(Timer *timer, Rectangle rect, bool paused);
