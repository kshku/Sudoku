#pragma once

#include "global.h"
#include "stext.h"

typedef struct SButton {
        SText text;
        Rectangle rect;
} SButton;

void sbutton_create(SButton *sbutton, const char *text, float font_size,
                    Rectangle rect);

void sbutton_draw(SButton *sbutton, Color button_color, Color text_color);
