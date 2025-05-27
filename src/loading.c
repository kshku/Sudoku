#include "loading.h"

static float min(float a, float b);

void loading_screen_init(LoadingScreen *ls, sthread_func func, const char *text,
                         long long min_sec) {
    ls->data =
        (LoadingData){.completed_tasks = 1, .done = false, .max_tasks = 1};

    sthread_create(&ls->thread, func, &ls->data);

    stext_create(&ls->loading_text, text, 48,
                 (Rectangle){.x = 0,
                             .y = 0,
                             .width = GetScreenWidth(),
                             .height = (GetScreenHeight() * 5 / 6)});

    ls->bar_rect.height = 80;
    ls->bar_rect.width = GetScreenWidth() - 100;
    ls->bar_rect.x = 50;
    ls->bar_rect.y = (GetScreenHeight() / 2);

    ls->fill_rect.x = ls->bar_rect.x;
    ls->fill_rect.y = ls->bar_rect.y;
    ls->fill_rect.width =
        (ls->data.completed_tasks * ls->bar_rect.width) / ls->data.max_tasks;
    ls->fill_rect.height = ls->bar_rect.height;

    ls->min_sec = min_sec;

    timer_start(&ls->timer);
}

bool loading_screen_update(LoadingScreen *ls) {
    timer_update(&ls->timer);

    ls->fill_rect.width = min(
        (ls->data.completed_tasks * ls->bar_rect.width) / ls->data.max_tasks,
        (ls->timer.elapsed * ls->bar_rect.width) / ls->min_sec);

    return ls->timer.elapsed > ls->min_sec ? ls->data.done : false;
}

void loading_screen_draw(LoadingScreen *ls) {
    stext_draw(&ls->loading_text, BLACK);

    DrawRectangleRec(ls->bar_rect, BLACK);
    DrawRectangleRec(ls->fill_rect, GREEN);
}

void loading_screen_shutdown(LoadingScreen *ls) {
    sthread_join(&ls->thread, NULL);
}

static float min(float a, float b) {
    return a < b ? a : b;
}
