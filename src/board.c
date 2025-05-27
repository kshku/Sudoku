#include "board.h"

#include "loading.h"
#include "utils/sbutton.h"
#include "utils/stext.h"
#include "utils/timer.h"

typedef enum Mask {
    INVALID_MASK = 1 << 10,
    FIXED_MASK = 1 << 11,
    ALL_MASK = INVALID_MASK | FIXED_MASK
} Mask;

typedef struct Board {
        SScene scene;
        int state[9][9];
        Timer timer;
        int row, col;
        Rectangle board_rect;
        Rectangle timer_rect;

        float cell_width, cell_height;
        Vector2 text_size;
        float font_size;

        bool is_solving;
        bool is_paused;

        bool loading;
        LoadingScreen loading_screen;

        SButton resume_button;

        SButton main_menu_button;
        SButton solve_button;
        SButton pause_game_button;
        SButton clear_board_button;
} Board;

static Board board;

static void board_create_puzzle(int n_filled, LoadingData *data);

static void board_create_empty(void);

static bool is_solved(void);

static void board_highlight_invalid(void);

static void board_clear_invalid(void);

static bool board_is_safe_to_insert(int value, int row, int col);

static void board_clear_not_fixed();

static void board_solve_anim();

static void *board_load(void *data, SThread *thread);

static bool board_have_invalid(void);

static void board_draw_buttons(void);

static void board_update_buttons(Vector2 mpos, bool is_pressed);

void board_init(SScene scene) {
    board.row = board.col = 0;
    board.timer_rect = (Rectangle){.x = 0,
                                   .y = 0,
                                   .width = GetScreenWidth(),
                                   .height = GetScreenHeight() / 15};
    board.board_rect = (Rectangle){.x = 50,
                                   .y = 70,
                                   .width = GetScreenWidth() - 100,
                                   .height = GetScreenHeight() - 200};

    board.cell_width = board.board_rect.width / 9;
    board.cell_height = board.board_rect.height / 9;
    board.font_size = board.cell_height * 0.8;

    board.text_size = MeasureTextEx(*resources.font, "0", board.font_size, 1);

    board.is_solving = false;
    board.is_paused = false;

    sbutton_create(&board.resume_button, "Resume", 64, board.board_rect);

    float width_4 = GetScreenWidth() / 6;
    Rectangle buttons_rect = {
        .x = 20, .y = GetScreenHeight() - 100, .width = width_4, .height = 80};

    sbutton_create(&board.main_menu_button, "Main Menu", 40, buttons_rect);

    buttons_rect.x += width_4;
    buttons_rect.width += width_4;

    sbutton_create(&board.solve_button, "Solve Puzzle", 40, buttons_rect);

    buttons_rect.x += width_4;
    buttons_rect.width += width_4;

    sbutton_create(&board.pause_game_button, "Pause Game", 40, buttons_rect);

    buttons_rect.x += width_4;
    buttons_rect.width += width_4;

    sbutton_create(&board.clear_board_button, "Clear Board", 40, buttons_rect);

    board.scene = scene;
    board.loading = true;
    loading_screen_init(&board.loading_screen, board_load,
                        scene == SSCENE_PUZZLE_BOARD ? "Loading Puzzle..."
                                                     : "Loading Solver...",
                        1);
}

static void *board_load(void *data, SThread *thread) {
    (void)(thread);
    LoadingData *loading_data = (LoadingData *)data;
    if (board.scene == SSCENE_PUZZLE_BOARD)
        board_create_puzzle(10, loading_data);
    else board_create_empty();

    loading_data->done = true;

    return NULL;
}

SScene board_update() {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    if (board.is_paused) {
        Vector2 mpos = GetMousePosition();
        bool is_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (CheckCollisionPointRec(mpos, board.resume_button.rect)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (is_pressed) {
                board.is_paused = false;
                timer_resume(&board.timer);
            }
        }

        board_update_buttons(mpos, is_pressed);

        return board.scene;
    }

    if (board.loading) {
        board.loading = !loading_screen_update(&board.loading_screen);
        if (!board.loading) {
            loading_screen_shutdown(&board.loading_screen);
            timer_start(&board.timer);
        }
        return board.scene;
    }

    Vector2 mpos = GetMousePosition();
    bool is_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    // Change selection based on mouse input
    if (!board.is_solving) {
        if (is_pressed) {
            if (CheckCollisionPointRec(mpos, board.board_rect)) {
                board.row =
                    (int)((mpos.y - board.board_rect.y) / board.cell_height);
                board.col =
                    (int)((mpos.x - board.board_rect.x) / board.cell_width);
            }
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
                && board.col > 0)
                --board.col;

            if ((IsKeyPressed(keys[i][1]) || IsKeyPressedRepeat(keys[i][1]))
                && board.col < 8)
                ++board.col;

            if ((IsKeyPressed(keys[i][2]) || IsKeyPressedRepeat(keys[i][2]))
                && board.row > 0)
                --board.row;

            if ((IsKeyPressed(keys[i][3]) || IsKeyPressedRepeat(keys[i][3]))
                && board.row < 8)
                ++board.row;
        }

        // Update the numbers
        // Mask is either INVALID or FIXED
        // If FIXED, we won't modify
        // We don't have to care about invalid, since it will be updated
        // by checking the board
        if (!(board.state[board.row][board.col] & FIXED_MASK)
            || board.scene == SSCENE_SOLVER_BOARD) {
            if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE))
                board.state[board.row][board.col] = 0;

            // We are not going to check for multiple key press in
            // single frame Single key will get repeated for some
            // reason, and if we do, our logic will highlight as invalid
            int key = GetCharPressed();
            if (key >= '0' && key <= '9') {
                int value = key - '0';
                if (board.scene == SSCENE_SOLVER_BOARD) {
                    board.state[board.row][board.col] = value | FIXED_MASK;
                } else {
                    if (!board_is_safe_to_insert(value, board.row, board.col))
                        value |= INVALID_MASK;
                    board.state[board.row][board.col] = value;
                }
            }
        }

    } else {
        board_solve_anim();
    }

    board_update_buttons(mpos, is_pressed);

    if (board.scene == SSCENE_SOLVER_BOARD) board_highlight_invalid();

    if (is_solved()) return SSCENE_GAME_END;

    return board.scene;
}

void board_draw() {
    if (board.loading) {
        loading_screen_draw(&board.loading_screen);
        return;
    }

    if (!board.is_paused) {
        for (int i = 0; i < 9; ++i) {
            float cy = board.board_rect.y + (board.cell_height * i);

            // Draw numbers and highlights
            for (int j = 0; j < 9; ++j) {
                float cx = board.board_rect.x + (board.cell_width * j);

                // Draw highlight if selected
                if (i == board.row && j == board.col)
                    DrawRectangle(
                        cx, cy, board.cell_width, board.cell_height,
                        Fade((Color){.r = 133, .g = 144, .b = 113}, 0.5));

                // If state is not zero, draw the number and highlights
                if (board.state[i][j] & ~ALL_MASK) {
                    // Draw highlight if Fixed number
                    if (board.state[i][j] & FIXED_MASK)
                        DrawRectangle(
                            cx, cy, board.cell_width, board.cell_height,
                            Fade(GRAY,
                                 (i == board.row && j == board.col) ? 0.5 : 1));

                    // Draw highlight if invalid
                    if (board.state[i][j] & INVALID_MASK)
                        DrawRectangle(
                            cx, cy, board.cell_width, board.cell_height,
                            Fade(RED,
                                 (i == board.row && j == board.col) ? 0.5 : 1));

                    // Draw the number
                    Vector2 pos = {
                        .x = cx + (board.cell_width - board.text_size.x) / 2,
                        .y = cy + (board.cell_height - board.text_size.y) / 2};
                    DrawTextCodepoint(*resources.font,
                                      ((board.state[i][j] & ~ALL_MASK) + '0'),
                                      pos, board.font_size, BLACK);
                }
            }
        }
    }

    // Draw lines after drawing numbers and highlights to avoid overlapping
    for (int i = 1; i < 9; ++i) {
        // Vertical lines
        Vector2 start = {.x = board.board_rect.x + (board.cell_width * i),
                         .y = board.board_rect.y};
        Vector2 end = {.x = start.x, .y = board.board_rect.height + start.y};
        DrawLineEx(start, end, i % 3 == 0 ? 5 : 3, BLACK);

        // Horizontal lines
        start = (Vector2){.x = board.board_rect.x,
                          .y = board.board_rect.y + (board.cell_height * i)};
        end = (Vector2){.x = board.board_rect.width + start.x, .y = start.y};
        DrawLineEx(start, end, i % 3 == 0 ? 5 : 3, BLACK);
    }
    // Draw boarder of board
    DrawRectangleLinesEx(board.board_rect, 6, BLACK);

    if (board.is_paused) sbutton_draw(&board.resume_button, GRAY, BLACK);

    switch (board.scene) {
        case SSCENE_PUZZLE_BOARD:
            timer_darw(&board.timer, board.timer_rect,
                       board.is_paused || board.is_solving);
            break;
        default:
            break;
    }

    board_draw_buttons();
}

void board_shutdown(void) {
}

static void board_create_empty(void) {
    // Initialize everything to 0
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j) board.state[i][j] = 0;

    board.row = board.col = 0;
}

static void board_create_puzzle(int n_filled, LoadingData *data) {
    data->max_tasks = 82;
    data->completed_tasks = 0;
    (void)(n_filled);
    board_create_empty();
    data->completed_tasks = 0;
    data->completed_tasks++;

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

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            /*sample[i][j] = 1;*/
            if (sample[i][j]) board.state[i][j] = sample[i][j] | FIXED_MASK;
            data->completed_tasks++;
        }
    }

    // for (int i = 0; i < 81; ++i) {
    //     int row = GetRandomValue(0, 8), col = GetRandomValue(0, 8);
    //     int value = GetRandomValue(1, 9);

    //     while (board.state[row][col]
    //            || !board_is_safe_to_insert(value, row, col)) {
    //         row = GetRandomValue(0, 8);
    //         col = GetRandomValue(0, 8);
    //         value = GetRandomValue(1, 9);
    //     }

    //     board.state[row][col] = value | FIXED_MASK;
    //     data->completed_tasks++;
    // }
}

static bool is_solved(void) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int num = board.state[i][j] & ~FIXED_MASK;
            if (num & INVALID_MASK) return false;
            if (!num) return false;
        }
    }

    return true;
}

static void board_highlight_invalid(void) {
    board_clear_invalid();

    for (int i = 0; i < 9; ++i) {
        int found_row_pos[9] = {0};
        int found_col_pos[9] = {0};
        int found_grid_pos[9][2] = {{0}};
        int grid_i_part = (i / 3) * 3;
        int grid_j_part = (i % 3) * 3;

        for (int j = 0; j < 9; ++j) {
            int row_num = board.state[i][j];
            int col_num = board.state[j][i];

            // i -> grid number
            // j -> cell in grid
            int grid_i, grid_j;
            grid_i = grid_i_part + (j / 3);
            grid_j = grid_j_part + (j % 3);
            int grid_num = board.state[grid_i][grid_j];

            // Extract the number
            row_num &= ~ALL_MASK;
            col_num &= ~ALL_MASK;
            grid_num &= ~ALL_MASK;

            // Check the row
            if (row_num) {
                // To store in zero index based array
                --row_num;

                if (found_row_pos[row_num]) {
                    board.state[i][found_row_pos[row_num] - 1] |= INVALID_MASK;
                    board.state[i][j] |= INVALID_MASK;
                } else {
                    found_row_pos[row_num] = j + 1;
                }
            }

            // Check the column
            if (col_num) {
                // To store in zero index based array
                --col_num;
                if (found_col_pos[col_num]) {
                    board.state[found_col_pos[col_num] - 1][i] |= INVALID_MASK;
                    board.state[j][i] |= INVALID_MASK;
                } else {
                    found_col_pos[col_num] = j + 1;
                }
            }

            // Check the grid
            if (grid_num) {
                // To store in zero index based array
                --grid_num;
                if (found_grid_pos[grid_num][0]) {
                    board.state[found_grid_pos[grid_num][0] - 1]
                               [found_grid_pos[grid_num][1] - 1] |=
                        INVALID_MASK;
                    board.state[grid_i][grid_j] |= INVALID_MASK;
                } else {
                    found_grid_pos[grid_num][0] = grid_i + 1;
                    found_grid_pos[grid_num][1] = grid_j + 1;
                }
            }
        }
    }
}

static void board_clear_invalid(void) {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j) board.state[i][j] &= ~INVALID_MASK;
}

static bool board_is_safe_to_insert(int value, int row, int col) {
    int grid_i_start = (row / 3) * 3;
    int grid_j_start = (col / 3) * 3;
    int grid_i, grid_j;
    for (int i = 0; i < 9; ++i) {
        // Check row
        if ((board.state[i][col] & ~ALL_MASK) == value) return false;

        // Check column
        if ((board.state[row][i] & ~ALL_MASK) == value) return false;

        // Check grid
        grid_i = grid_i_start + i / 3;
        grid_j = grid_j_start + i % 3;
        if ((board.state[grid_i][grid_j] & ~ALL_MASK) == value) return false;
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

static void board_solve_anim() {
    if (!solver.solving) {
        solver.solving = true;
        board.row = board.col = -1;
        solver.state = SELECT;
        solver.wait_count = 0;
        board_clear_not_fixed();
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
                    board.col = (board.col + 1) % 9;
                    if (board.col == 0) ++board.row;

                    if (board.row == 9) {
                        board.row = board.col = 0;
                        solver.state = COMPLETED;
                        break;
                    }
                } else {
                    --board.col;
                    if (board.col == -1) {
                        board.col = 8;
                        --board.row;
                    }

                    if (board.row == -1) {
                        board.row = board.col = 0;
                        solver.state = CANNOT_SOLVE;
                        break;
                    }
                }
                if (solver.state == SELECT || solver.state == SELECT_PREV)
                    if (board.state[board.row][board.col] & FIXED_MASK)
                        goto reselect;

                // solver.state = SOLVE;
                solver.state = WAIT;
                solver.wait_count = 10;
                solver.after_wait = SOLVE;
                break;
            case SOLVE:
                if ((board.state[board.row][board.col] & ~ALL_MASK) < 9) {
                    int value =
                        (board.state[board.row][board.col] & ~ALL_MASK) + 1;
                    if (!board_is_safe_to_insert(value, board.row, board.col)) {
                        solver.after_wait = SOLVE;
                        // solver.state = SOLVE;
                        value |= INVALID_MASK;
                    } else {
                        solver.after_wait = SELECT;
                        // solver.state = SELECT;
                    }
                    board.state[board.row][board.col] = value;
                } else {
                    board.state[board.row][board.col] = 0;
                    solver.after_wait = SELECT_PREV;
                    //     solver.state = SELECT_PREV;
                }

                solver.state = WAIT;
                solver.wait_count = 10;
                break;
            case CANNOT_SOLVE:
                TraceLog(LOG_ERROR, "Can't solve!");
            case COMPLETED:
            default:
                break;
        }
    }

    board_draw();
}

static void board_clear_not_fixed() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board.state[i][j] & FIXED_MASK)
                board.state[i][j] &= ~INVALID_MASK;
            else board.state[i][j] = 0;
        }
    }
}

static bool board_have_invalid(void) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board.state[i][j] & INVALID_MASK) return false;
        }
    }

    return true;
}

static void board_update_buttons(Vector2 mpos, bool is_pressed) {
    if (CheckCollisionPointRec(mpos, board.main_menu_button.rect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (is_pressed) board.scene = SSCENE_MAIN_MENU;
    }

    if (!board.is_solving && !board.is_paused) {
        if (CheckCollisionPointRec(mpos, board.solve_button.rect)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (is_pressed) {
                board.is_solving = true;
                timer_pause(&board.timer);
            }
        }

        if (board.scene == SSCENE_PUZZLE_BOARD) {
            if (CheckCollisionPointRec(mpos, board.pause_game_button.rect)) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                if (is_pressed) {
                    board.is_paused = true;
                    timer_pause(&board.timer);
                }
            }
        }

        if (CheckCollisionPointRec(mpos, board.clear_board_button.rect)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (is_pressed) {
                if (board.scene == SSCENE_PUZZLE_BOARD) board_clear_not_fixed();
                else board_create_empty();
            }
        }
    }
}

static void board_draw_buttons(void) {
    sbutton_draw(&board.main_menu_button, BLUE, BLACK);
    sbutton_draw(&board.clear_board_button, BLUE, BLACK);
    sbutton_draw(&board.solve_button, BLUE, BLACK);
    if (board.scene == SSCENE_PUZZLE_BOARD)
        sbutton_draw(&board.pause_game_button, BLUE, BLACK);
    else sbutton_draw(&board.pause_game_button, GRAY, BLACK);
}
