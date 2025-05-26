#include "main_menu.h"

#include "utils/sbutton.h"
#include "utils/stext.h"

typedef struct MainMenu {
        SText welcome_text;
        SButton puzzle_button;
        SButton solver_button;
        SButton load_button;
} MainMenu;

static MainMenu main_menu;

void main_menu_init(void) {
    stext_create(&main_menu.welcome_text, "Welcome to Sudoku!", 64,
                 (Rectangle){.x = 0,
                             .y = 0,
                             .width = GetScreenWidth(),
                             .height = GetScreenHeight() * 3 / 10});

    sbutton_create(&main_menu.puzzle_button, "Play a Puzzle", 48,
                   (Rectangle){.x = 0,
                               .y = 0,
                               .width = GetScreenWidth(),
                               .height = GetScreenHeight() * 6 / 10});

    sbutton_create(&main_menu.solver_button, "Sudoku Solver", 48,
                   (Rectangle){.x = 0,
                               .y = 0,
                               .width = GetScreenWidth(),
                               .height = GetScreenHeight() * 9 / 10});

    sbutton_create(&main_menu.load_button, "Load Saved Game", 48,
                   (Rectangle){.x = 0,
                               .y = 0,
                               .width = GetScreenWidth(),
                               .height = GetScreenHeight() * 12 / 10});
}

SScene main_menu_update(void) {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    Vector2 mpos = GetMousePosition();

    if (CheckCollisionPointRec(mpos, main_menu.puzzle_button.rect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            return SSCENE_PUZZLE_BOARD;
        }
    }

    if (CheckCollisionPointRec(mpos, main_menu.solver_button.rect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            return SSCENE_SOLVER_BOARD;
        }
    }

    return SSCENE_MAIN_MENU;
}

void main_menu_draw(void) {
    stext_draw(&main_menu.welcome_text, BLACK);
    sbutton_draw(&main_menu.puzzle_button, BLUE, BLACK);
    sbutton_draw(&main_menu.solver_button, BLUE, BLACK);
    sbutton_draw(&main_menu.load_button, GRAY, BLACK);
}

void main_menu_shutdown(void) {
}
