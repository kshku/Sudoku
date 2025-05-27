#pragma once

#include <stdatomic.h>

#include "global.h"
#include "utils/stext.h"
#include "utils/sthread.h"
#include "utils/timer.h"

typedef struct LoadingData {
        atomic_uint max_tasks;
        atomic_uint completed_tasks;
        atomic_bool done;
} LoadingData;

typedef struct LoadingScreen {
        SThread thread;
        LoadingData data;
        SText loading_text;
        Rectangle bar_rect;
        Rectangle fill_rect;
        Timer timer;
        long long min_sec;
        float loading_delta;
} LoadingScreen;

void loading_screen_init(LoadingScreen *ls, sthread_func func, const char *text,
                         long long min_sec);

bool loading_screen_update(LoadingScreen *ls);

void loading_screen_draw(LoadingScreen *ls);

void loading_screen_shutdown(LoadingScreen *ls);
