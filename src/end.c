#include "end.h"

#include "utils/sbutton.h"
#include "utils/stext.h"

typedef struct End {
        unsigned int hour, min, sec;
        SText complete_text;
        SText message;
        SButton main_menu_button;
        SButton new_puzzle_button;
} End;

static End end;

void end_init(unsigned int hour, unsigned int min, unsigned int sec) {
    end.hour = hour, end.min = min, end.sec = sec;

    stext_create(&end.complete_text, "Completed!!!", 64,
                 (Rectangle){.x = 0,
                             .y = 0,
                             .width = GetScreenWidth(),
                             .height = GetScreenHeight() / 2});

    stext_create(&end.message,
                 TextFormat("Time taken: %d:%d:%d", hour, min, sec), 48,
                 (Rectangle){.x = 0,
                             .y = 0,
                             .width = GetScreenWidth(),
                             .height = GetScreenHeight() * 3 / 4});

    sbutton_create(&end.main_menu_button, "Main Menu", 40,
                   (Rectangle){.x = 0,
                               .y = GetScreenHeight() / 2,
                               .width = GetScreenWidth(),
                               .height = GetScreenHeight() / 3});

    sbutton_create(&end.new_puzzle_button, "New Puzzle", 40,
                   (Rectangle){.x = 0,
                               .y = GetScreenHeight() / 2,
                               .width = GetScreenWidth(),
                               .height = GetScreenHeight() * 2 / 3});
}

SScene end_update(void) {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    Vector2 mpos = GetMousePosition();
    bool is_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (CheckCollisionPointRec(mpos, end.main_menu_button.rect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (is_pressed) return SSCENE_MAIN_MENU;
    }

    if (CheckCollisionPointRec(mpos, end.new_puzzle_button.rect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (is_pressed) return SSCENE_PUZZLE_BOARD;
    }

    return SSCENE_GAME_END;
}

void end_draw(void) {
    end.message.text =
        TextFormat("Time taken: %d:%d:%d", end.hour, end.min, end.sec);
    stext_draw(&end.message, BLACK);

    stext_draw(&end.complete_text, BLACK);

    sbutton_draw(&end.main_menu_button, BLUE, BLACK);

    sbutton_draw(&end.new_puzzle_button, BLUE, BLACK);
}
