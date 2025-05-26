#include "options.h"

static Options options;

GameState options_update(OptionType type) {
    switch (type) {
        case ENTRY_OPTIONS: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mpos = GetMousePosition();
                if (CheckCollisionPointRec(mpos, options.puzzle_button_rect)) {
                    return GAME_STATE_PUZZLE_BOARD;
                } else if (CheckCollisionPointRec(mpos,
                                                  options.solver_button_rect)) {
                    return GAME_STATE_SOLVER_BOARD;
                }
            }
        } break;
        case IN_GAME_OPTION:
            break;
        default:
            break;
    }
    return GAME_STATE_MAIN_MENU;
}

void entry_options_draw(Rectangle *rect, Font *font) {
    options.type = ENTRY_OPTIONS;

    float font_size = 70 * 0.8;
    Vector2 text_size;
    Rectangle x_rect;
    x_rect.height = 70;
    const char *welcome_text = "Welcome to SUDOKU!";

    text_size = MeasureTextEx(*font, welcome_text, font_size, 1);

    options.welcome_text_pos = (Vector2){
        .x = rect->x + ((rect->width - text_size.x) / 2), .y = rect->y + 30};
    DrawTextEx(*font, welcome_text, options.welcome_text_pos, font_size, 1,
               BLACK);

    text_size = MeasureTextEx(*font, "Play a Puzzle", font_size, 1);
    x_rect.width = text_size.x + 20;
    x_rect.x = rect->x + ((rect->width - x_rect.width) / 2);
    x_rect.y = rect->y + x_rect.height + 100;
    options.puzzle_button_rect = (Rectangle){.x = x_rect.x,
                                             .y = x_rect.y,
                                             .width = x_rect.width,
                                             .height = x_rect.height};
    DrawRectangleRounded(x_rect, 2, 0, BLUE);
    DrawTextEx(*font, "Play a Puzzle",
               (Vector2){.x = x_rect.x + (x_rect.width - text_size.x) / 2,
                         .y = x_rect.y + (x_rect.height - text_size.y) / 2},
               font_size, 1, BLACK);

    text_size = MeasureTextEx(*font, "Puzzle Solver", font_size, 1);
    x_rect.y += x_rect.height + 100;
    x_rect.width = text_size.x + 20;
    x_rect.x = rect->x + ((rect->width - x_rect.width) / 2);
    options.solver_button_rect = (Rectangle){.x = x_rect.x,
                                             .y = x_rect.y,
                                             .width = x_rect.width,
                                             .height = x_rect.height};
    DrawRectangleRounded(x_rect, 2, 0, BLUE);
    DrawTextEx(*font, "Puzzle Solver",
               (Vector2){.x = x_rect.x + (x_rect.width - text_size.x) / 2,
                         .y = x_rect.y + (x_rect.height - text_size.y) / 2},
               font_size, 1, BLACK);

    text_size = MeasureTextEx(*font, "Load Saved Game", font_size, 1);
    x_rect.y += x_rect.height + 100;
    x_rect.width = text_size.x + 20;
    x_rect.x = rect->x + ((rect->width - x_rect.width) / 2);
    options.load_game_button_rect = (Rectangle){.x = x_rect.x,
                                                .y = x_rect.y,
                                                .width = x_rect.width,
                                                .height = x_rect.height};
    DrawRectangleRounded(x_rect, 2, 0, GRAY);
    DrawTextEx(*font, "Load Saved Game",
               (Vector2){.x = x_rect.x + (x_rect.width - text_size.x) / 2,
                         .y = x_rect.y + (x_rect.height - text_size.y) / 2},
               font_size, 1, BLACK);
}

void options_draw(Rectangle *rect, OptionType type, Font *font) {
    switch (type) {
        case ENTRY_OPTIONS:
            entry_options_draw(rect, font);
            break;
        case IN_GAME_OPTION:
            break;
    }
}
