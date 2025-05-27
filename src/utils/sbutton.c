#include "sbutton.h"

void sbutton_create(SButton *sbutton, const char *text, float height,
                    Rectangle rect) {
    Vector2 text_size = MeasureTextEx(*resources.font, text, height * 0.8, 1);
    sbutton->rect.width = text_size.x + 40;
    sbutton->rect.height = height;
    sbutton->rect.x = rect.x + ((rect.width - sbutton->rect.width) / 2);
    sbutton->rect.y = rect.y + ((rect.height - sbutton->rect.height) / 2);

    stext_create_with_text_size(&sbutton->text, text, height * 0.8,
                                sbutton->rect, text_size);
}

void sbutton_draw(SButton *sbutton, Color button_color, Color text_color) {
    DrawRectangleRounded(sbutton->rect, 3, 0, button_color);
    stext_draw(&sbutton->text, text_color);
}
