#include "timer.h"

void timer_start(Timer *timer) {
    timer->start = GetTime();
    timer->elapsed = 0;
}

void timer_update(Timer *timer) {
    timer->elapsed = GetTime() - timer->start;
}

void timer_get_hour_min_sec(long long time, unsigned int *hour,
                            unsigned int *min, unsigned int *sec) {
    *hour = time / 3600;
    time %= 3600;
    *min = time / 60;
    time %= 60;
    *sec = time;
}

void timer_darw(Timer *timer, Rectangle *rect, Font *font) {
    timer_update(timer);
    unsigned int hour, min, sec;
    float font_size = rect->height * 0.8;

    timer_get_hour_min_sec((long long)timer->elapsed, &hour, &min, &sec);

    const char *timer_text = TextFormat("%u:%u:%u", hour, min, sec);

    Vector2 text_size = MeasureTextEx(*font, timer_text, font_size, 1);

    DrawTextEx(*font, timer_text,
               (Vector2){.x = rect->x + ((rect->width - text_size.x) / 2),
                         .y = rect->y + ((rect->height - text_size.y) / 2)},
               font_size, 1, BLACK);
}
