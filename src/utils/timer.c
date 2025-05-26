#include "timer.h"

void timer_start(Timer *timer) {
    timer->start = (long long)GetTime();
    timer->elapsed = 0;
}

void timer_update(Timer *timer) {
    timer->elapsed = ((long long)GetTime()) - timer->start;
}

void timer_get_hour_min_sec(long long time, unsigned int *hour,
                            unsigned int *min, unsigned int *sec) {
    *hour = time / 3600;
    time %= 3600;
    *min = time / 60;
    time %= 60;
    *sec = time;
}

void timer_darw(Timer *timer, Rectangle rect) {
    timer_update(timer);
    unsigned int hour, min, sec;
    float font_size = rect.height * 0.8;

    timer_get_hour_min_sec(timer->elapsed, &hour, &min, &sec);

    const char *timer_text = TextFormat("%u:%u:%u", hour, min, sec);

    Vector2 text_size =
        MeasureTextEx(*resources.font, timer_text, font_size, 1);

    DrawTextEx(*resources.font, timer_text,
               (Vector2){.x = rect.x + ((rect.width - text_size.x) / 2),
                         .y = rect.y + ((rect.height - text_size.y) / 2)},
               font_size, 1, BLACK);
}
