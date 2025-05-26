#pragma once

#include <raylib.h>

#include "global.h"

typedef struct Board {
        int state[9][9];
        int row, col;
        Font *font;
        Rectangle *rect;
        float cell_width, cell_height;
        float font_size;
} Board;

void board_create(Board *board, Font *font, Rectangle *rect);

void board_clear_not_fixed(Board *board);

void board_create_puzzle(Board *board, int n_filled);

void board_create_empty(Board *board);

bool board_update(Board *board, bool is_solver);

void board_draw(Board *board);

void board_solve_anim(Board *board);

void board_destroy(Board *board);

void board_highlight_invalid(Board *board);

bool board_is_safe_to_insert(Board *board, int value, int row, int col);

void board_clear_invalid(Board *board);
