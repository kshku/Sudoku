#pragma once

#include <raylib.h>

#include "global.h"

typedef struct Timer {
        double start;
        double elapsed;
} Timer;

void timer_start(Timer *timer);

void timer_update(Timer *timer);

void timer_darw(Timer *timer, Rectangle *rect, Font *font);
