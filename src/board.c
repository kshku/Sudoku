#include "board.h"

#include <stdbool.h>

typedef enum Mask {
    INVALID_MASK = 1 << 10,
    FIXED_MASK = 1 << 11,
    ALL_MASK = INVALID_MASK | FIXED_MASK
} Mask;

void board_create(Board *board, Font *font, Rectangle *rect) {
    board_create_empty(board);

    board->font = font;
    board->rect = rect;
    board->cell_width = board->rect->width / 9,
    board->cell_height = board->rect->height / 9;
    board->font_size = board->cell_height * 0.8;
}

void board_create_puzzle(Board *board, int n_filled) {
    board_create_empty(board);

    // Sample board
    int sample[9][9] = {
        {5, 0, 1, 6, 0, 7, 9, 0, 0},
        {0, 0, 9, 0, 0, 3, 2, 5, 0},
        {8, 2, 7, 0, 9, 0, 0, 0, 0},
        {9, 0, 2, 0, 5, 1, 3, 7, 0},
        {3, 0, 0, 9, 8, 0, 0, 0, 0},
        {0, 0, 5, 7, 0, 6, 0, 0, 0},
        {4, 0, 6, 0, 7, 5, 0, 3, 2},
        {0, 1, 0, 0, 0, 0, 7, 0, 5},
        {0, 0, 3, 0, 0, 0, 1, 9, 6}
    };

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (sample[i][j]) board->state[i][j] = sample[i][j] | FIXED_MASK;

    // for (int i = 0; i < n_filled; ++i) {
    //     int row = GetRandomValue(0, 8), col = GetRandomValue(0, 8);
    //     int value = GetRandomValue(1, 9);

    //     while (board->state[row][col]) {
    //         row = GetRandomValue(0, 8);
    //         col = GetRandomValue(0, 8);
    //     }

    //     while (!board_is_safe_to_insert(board, value, row, col))
    //         value = GetRandomValue(1, 9);

    //     board->state[row][col] = value | FIXED_MASK;
    // }
}

void board_create_empty(Board *board) {
    // Initialize everything to 0
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j) board->state[i][j] = 0;

    board->row = 0;
    board->col = 0;
}

void board_update(Board *board, bool is_solver) {
    // Change selection based on mouse input
    Vector2 mpos = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)
        && CheckCollisionPointRec(mpos, *board->rect)) {
        board->row = (int)((mpos.y - board->rect->y) / board->cell_height);
        board->col = (int)((mpos.x - board->rect->x) / board->cell_width);
    }

    // Change selection based on key input
    KeyboardKey keys[][4] = {
        {KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN}, /* Arrows */
        {   KEY_A,     KEY_D,  KEY_W,    KEY_S}, /* Gaming keys */
        {   KEY_H,     KEY_L,  KEY_K,    KEY_J}, /* Vim Keys */
    };
    int keys_length = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < keys_length; ++i) {
        if ((IsKeyPressed(keys[i][0]) || IsKeyPressedRepeat(keys[i][0]))
            && board->col > 0)
            --board->col;

        if ((IsKeyPressed(keys[i][1]) || IsKeyPressedRepeat(keys[i][1]))
            && board->col < 8)
            ++board->col;

        if ((IsKeyPressed(keys[i][2]) || IsKeyPressedRepeat(keys[i][2]))
            && board->row > 0)
            --board->row;

        if ((IsKeyPressed(keys[i][3]) || IsKeyPressedRepeat(keys[i][3]))
            && board->row < 8)
            ++board->row;
    }

    // Update the numbers
    // Mask is either INVALID or FIXED
    // If FIXED, we won't modify
    // We don't have to care about invalid, since it will be updated by checking
    // the board
    if (!(board->state[board->row][board->col] & FIXED_MASK) || is_solver) {
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE))
            board->state[board->row][board->col] = 0;

        // We are not going to check for multiple key press in single frame
        // Single key will get repeated for some reason, and if we do, our logic
        // will highlight as invalid
        int key = GetCharPressed();
        if (key >= '0' && key <= '9') {
            int value = key - '0';
            if (is_solver) {
                board->state[board->row][board->col] = value | FIXED_MASK;
            } else {
                if (!board_is_safe_to_insert(board, value, board->row,
                                             board->col))
                    value |= INVALID_MASK;
                board->state[board->row][board->col] = value;
            }
        }
    }

    if (is_solver) board_highlight_invalid(board);
}

void board_draw(Board *board) {
    Vector2 text_size = MeasureTextEx(*board->font, "0", board->font_size, 1);

    for (int i = 0; i < 9; ++i) {
        float cy = board->rect->y + (board->cell_height * i);

        // Draw numbers and highlights
        for (int j = 0; j < 9; ++j) {
            float cx = board->rect->x + (board->cell_width * j);

            // Draw highlight if selected
            if (i == board->row && j == board->col)
                DrawRectangle(cx, cy, board->cell_width, board->cell_height,
                              (Color){.r = 133, .g = 114, .b = 113, .a = 126});

            // If state is not zero, draw the number and highlights
            if (board->state[i][j] & ~ALL_MASK) {
                // Draw highlight if Fixed number
                if (board->state[i][j] & FIXED_MASK)
                    DrawRectangle(
                        cx, cy, board->cell_width, board->cell_height,
                        (Color){
                            130, 130, 130,
                            (i == board->row && j == board->col) ? 126 : 255});

                // Draw highlight if invalid
                if (board->state[i][j] & INVALID_MASK)
                    DrawRectangle(
                        cx, cy, board->cell_width, board->cell_height,
                        (Color){
                            230, 41, 55,
                            (i == board->row && j == board->col) ? 126 : 255});

                // Draw the number
                Vector2 pos = {
                    .x = cx + (board->cell_width - text_size.x) / 2,
                    .y = cy + (board->cell_height - text_size.y) / 2};
                DrawTextCodepoint(*board->font,
                                  ((board->state[i][j] & ~ALL_MASK) + '0'), pos,
                                  board->font_size, BLACK);
            }
        }
    }

    // Draw lines after drawing numbers and highlights to avoid overlapping
    for (int i = 1; i < 9; ++i) {
        // Vertical lines
        Vector2 start = {.x = board->rect->x + (board->cell_width * i),
                         .y = board->rect->y};
        Vector2 end = {.x = start.x, .y = board->rect->height + start.y};
        DrawLineEx(start, end, i % 3 == 0 ? 5 : 3, BLACK);

        // Horizontal lines
        start = (Vector2){.x = board->rect->x,
                          .y = board->rect->y + (board->cell_height * i)};
        end = (Vector2){.x = board->rect->width + start.x, .y = start.y};
        DrawLineEx(start, end, i % 3 == 0 ? 5 : 3, BLACK);
    }
    // Draw boarder of board
    DrawRectangleLinesEx(*board->rect, 6, BLACK);
}

void board_solve(Board *board) {
}

void board_destroy(Board *board) {
}

void board_highlight_invalid(Board *board) {
    board_clear_invalid(board);

    for (int i = 0; i < 9; ++i) {
        int found_row_pos[9] = {0};
        int found_col_pos[9] = {0};
        int found_grid_pos[9][2] = {{0}};
        int grid_i_part = (i / 3) * 3;
        int grid_j_part = (i % 3) * 3;

        for (int j = 0; j < 9; ++j) {
            int row_num = board->state[i][j];
            int col_num = board->state[j][i];

            // i -> grid number
            // j -> cell in grid
            int grid_i, grid_j;
            grid_i = grid_i_part + (j / 3);
            grid_j = grid_j_part + (j % 3);
            int grid_num = board->state[grid_i][grid_j];

            // Extract the number
            row_num &= ~ALL_MASK;
            col_num &= ~ALL_MASK;
            grid_num &= ~ALL_MASK;

            // Check the row
            if (row_num) {
                // To store in zero index based array
                --row_num;

                if (found_row_pos[row_num]) {
                    board->state[i][found_row_pos[row_num] - 1] |= INVALID_MASK;
                    board->state[i][j] |= INVALID_MASK;
                } else {
                    found_row_pos[row_num] = j + 1;
                }
            }

            // Check the column
            if (col_num) {
                // To store in zero index based array
                --col_num;
                if (found_col_pos[col_num]) {
                    board->state[found_col_pos[col_num] - 1][i] |= INVALID_MASK;
                    board->state[j][i] |= INVALID_MASK;
                } else {
                    found_col_pos[col_num] = j + 1;
                }
            }

            // Check the grid
            if (grid_num) {
                // To store in zero index based array
                --grid_num;
                if (found_grid_pos[grid_num][0]) {
                    board->state[found_grid_pos[grid_num][0] - 1]
                                [found_grid_pos[grid_num][1] - 1] |=
                        INVALID_MASK;
                    board->state[grid_i][grid_j] |= INVALID_MASK;
                } else {
                    found_grid_pos[grid_num][0] = grid_i + 1;
                    found_grid_pos[grid_num][1] = grid_j + 1;
                }
            }
        }
    }
}

void board_clear_invalid(Board *board) {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j) board->state[i][j] &= ~INVALID_MASK;
}

bool board_is_safe_to_insert(Board *board, int value, int row, int col) {
    for (int i = 0; i < 9; ++i) {
        // Check row
        if ((board->state[i][col] & ~ALL_MASK) == value) return false;

        // Check column
        if ((board->state[row][i] & ~ALL_MASK) == value) return false;
    }

    // Check grid
    int grid_i_start = (row / 3) * 3;
    int grid_j_start = (col / 3) * 3;
    for (int i = 0; i < 3; ++i) {
        int grid_i = grid_i_start + i;
        for (int j = 0; j < 3; ++j) {
            int grid_j = grid_j_start + j;
            if ((board->state[grid_i][grid_j] & ~ALL_MASK) == value)
                return false;
        }
    }

    return true;
}

typedef enum SolverState {
    SELECT,
    SELECT_PREV,
    SOLVE,
    WAIT,
    CANNOT_SOLVE,
    COMPLETED
} SolverState;

typedef struct Solver {
        bool solving;
        int wait_count;
        bool done;

        SolverState after_wait;
        SolverState state;
} Solver;

static Solver solver = {.solving = false};

void board_solve_anim(Board *board) {
    if (!solver.solving) {
        solver.solving = true;
        board->row = board->col = -1;
        solver.state = SELECT;
        solver.wait_count = 0;
        board_clear_not_fixed(board);
    } else {
        switch (solver.state) {
            case WAIT:
                if (solver.wait_count) --solver.wait_count;
                else solver.state = solver.after_wait;
                break;
            case SELECT:
            case SELECT_PREV:
            reselect:
                if (solver.state == SELECT) {
                    board->col = (board->col + 1) % 9;
                    if (board->col == 0) ++board->row;

                    if (board->row == 9) {
                        board->row = board->col = 0;
                        solver.state = COMPLETED;
                        break;
                    }
                } else {
                    --board->col;
                    if (board->col == -1) {
                        board->col = 8;
                        --board->row;
                    }

                    if (board->row == -1) {
                        board->row = board->col = 0;
                        solver.state = CANNOT_SOLVE;
                        break;
                    }
                }
                if (solver.state == SELECT || solver.state == SELECT_PREV)
                    if (board->state[board->row][board->col] & FIXED_MASK)
                        goto reselect;

                solver.state = SOLVE;
                // solver.state = WAIT;
                // solver.wait_count = 10;
                // solver.after_wait = SOLVE;
                break;
            case SOLVE:
                if ((board->state[board->row][board->col] & ~ALL_MASK) < 9) {
                    int value =
                        (board->state[board->row][board->col] & ~ALL_MASK) + 1;
                    if (!board_is_safe_to_insert(board, value, board->row,
                                                 board->col)) {
                        // solver.after_wait = SOLVE;
                        solver.state = SOLVE;
                        value |= INVALID_MASK;
                    } else {
                        // solver.after_wait = SELECT;
                        solver.state = SELECT;
                    }
                    board->state[board->row][board->col] = value;
                } else {
                    board->state[board->row][board->col] = 0;
                    // solver.after_wait = SELECT_PREV;
                    solver.state = SELECT_PREV;
                }

                // solver.state = WAIT;
                solver.wait_count = 10;
                break;
            case CANNOT_SOLVE:
                TraceLog(LOG_ERROR, "Can't solve!");
            case COMPLETED:
            default:
                break;
        }
    }

    board_draw(board);
}

void board_clear_not_fixed(Board *board) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board->state[i][j] & FIXED_MASK)
                board->state[i][j] &= ~INVALID_MASK;
            else board->state[i][j] = 0;
        }
    }
}
