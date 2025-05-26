#pragma once

#include <raylib.h>

#include "gamestate.h"

typedef enum OptionType {
    OPTIONS_NONE,
    ENTRY_OPTIONS,
    IN_GAME_OPTION
} OptionType;

typedef struct Options {
        OptionType type;
        Vector2 welcome_text_pos;
        Rectangle puzzle_button_rect;
        Rectangle solver_button_rect;
        Rectangle load_game_button_rect;
} Options;

GameState options_update(OptionType type);

void options_draw(Rectangle *rect, OptionType type, Font *font);
