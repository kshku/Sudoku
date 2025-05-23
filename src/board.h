#pragma once

#include <raylib.h>
#include <stdbool.h>

typedef struct Board {
        unsigned int state[9][9];
} Board;

bool board_create_puzzel(Board *board);

bool board_create_empty(Board *board);

void board_draw(Board *board, Font *font, Rectangle *rect);

void board_solve(Board *board);

void board_destroy(Board *board);
