#include "board.h"

typedef enum Mask {
    INVALID_MASK = 1 << 10,
    FIXED_MASK = 1 << 11,
    ALL_MASK = INVALID_MASK | FIXED_MASK
} Mask;

bool board_create_puzzel(Board *board) {
    // Temporary
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j) board->state[i][j] = GetRandomValue(0, 9);

    for (int i = 0; i < 15; ++i) {
        board->state[GetRandomValue(0, 9)][GetRandomValue(0, 9)] |= FIXED_MASK;
        board->state[GetRandomValue(0, 9)][GetRandomValue(0, 9)] |=
            INVALID_MASK;
    }
}

bool board_create_empty(Board *board) {
    // Initialize everything to 0
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j) board->state[i][j] = 0;

    return true;
}

void board_draw(Board *board, Font *font, Rectangle *rect) {
    float cw = rect->width / 9, ch = rect->height / 9;
    float font_size = ch * 0.8;
    Vector2 text_size = MeasureTextEx(*font, "0", font_size, 1);

    for (int i = 0; i < 9; ++i) {
        float cx = rect->x + (cw * i);

        // Draw numbers and highlights
        for (int j = 0; j < 9; ++j) {
            float cy = rect->y + (ch * j);
            if (board->state[i][j] & ~ALL_MASK) {
                Rectangle cell = {.x = cx, .y = cy, .width = cw, .height = ch};
                if (board->state[i][j] & INVALID_MASK)
                    DrawRectangle(cell.x, cell.y, cell.width, cell.height, RED);
                if (board->state[i][j] & FIXED_MASK)
                    DrawRectangle(cell.x, cell.y, cell.width, cell.height,
                                  GRAY);

                Vector2 pos = {.x = cx + (cw - text_size.x) / 2,
                               .y = cy + (ch - text_size.y) / 2};
                DrawTextCodepoint(*font,
                                  ((board->state[i][j] & ~ALL_MASK) + '0'), pos,
                                  font_size, BLACK);
            }
        }
    }
    // Draw lines after drawing numbers and highlights to avoid overlapping
    for (int i = 1; i < 9; ++i) {
        // Vertical lines
        Vector2 start = {.x = rect->x + (cw * i), .y = rect->y};
        Vector2 end = {.x = start.x, .y = rect->height + start.y};
        DrawLineEx(start, end, i % 3 == 0 ? 5 : 3, BLACK);

        // Horizontal lines
        start = (Vector2){.x = rect->x, .y = rect->y + (ch * i)};
        end = (Vector2){.x = rect->width + start.x, .y = start.y};
        DrawLineEx(start, end, i % 3 == 0 ? 5 : 3, BLACK);
    }
    // Draw boarder of board
    DrawRectangleLinesEx(*rect, 6, BLACK);
}

void board_solve(Board *board) {
}

void board_destroy(Board *board) {
    (void *)board;
}
