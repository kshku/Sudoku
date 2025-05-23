#pragma once

#include <raylib.h>

typedef struct Board {
        int state[9][9];
        int selected_row, selected_col;
        Font *font;
        Rectangle *rect;
        float cell_width, cell_height;
        float font_size;
} Board;

void board_create(Board *board, Font *font, Rectangle *rect);

void board_create_puzzle(Board *board);

void board_create_empty(Board *board);

void board_update(Board *board);

void board_draw(Board *board);

void board_solve(Board *board);

void board_destroy(Board *board);
