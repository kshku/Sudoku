#pragma once

#include "global.h"

typedef struct SText {
        const char *text;
        Vector2 pos;
        float font_size;
} SText;

void stext_create(SText *stext, const char *text, float font_size,
                  Rectangle rect);

void stext_create_with_text_size(SText *stext, const char *text,
                                 float font_size, Rectangle rect,
                                 Vector2 text_size);

void stext_draw(SText *stext, Color color);
