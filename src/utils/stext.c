#include "stext.h"

void stext_create(SText *stext, const char *text, float font_size,
                  Rectangle rect) {
    stext->text = text;
    stext->font_size = font_size;

    Vector2 text_size = MeasureTextEx(*resources.font, text, font_size, 1);

    stext->pos = (Vector2){.x = rect.x + (rect.width - text_size.x) / 2,
                           .y = rect.y + (rect.height - text_size.y) / 2};
}

void stext_create_with_text_size(SText *stext, const char *text,
                                 float font_size, Rectangle rect,
                                 Vector2 text_size) {
    stext->text = text;
    stext->font_size = font_size;

    stext->pos = (Vector2){.x = rect.x + (rect.width - text_size.x) / 2,
                           .y = rect.y + (rect.height - text_size.y) / 2};
}

void stext_draw(SText *stext, Color color) {
    DrawTextEx(*resources.font, stext->text, stext->pos, stext->font_size, 1,
               color);
}
