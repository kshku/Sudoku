#include "game.h"

#include <raylib.h>

#include "board.h"

#define NULL ((void *)0)

#define TITLE "SUDOKU"

typedef enum State {
    STATE_START_OPTIOINS,
    STATE_SOLVER_BOARD,
    STATE_PUZZLE_BOARD
} State;

typedef struct Game {
        Font font;
        int width, height, fps;
        Board board;
        Rectangle board_rect;
        State state;
        bool solve;
        bool board_created;
} Game;

static Game game;

void draw_options(void);

bool game_initialize(void) {
    game.width = 900, game.height = 900;
    game.fps = 60;
    game.board_created = false;
    game.state = STATE_PUZZLE_BOARD;
    game.solve = false;

    float left_padding, right_padding, top_padding, bottom_padding;
    left_padding = top_padding = right_padding = 50;
    bottom_padding = 150;

    game.board_rect =
        (Rectangle){.x = 0 + left_padding,
                    .y = 0 + top_padding,
                    .width = game.width - right_padding - left_padding,
                    .height = game.height - bottom_padding - top_padding};

    InitWindow(game.width, game.height, TITLE);
    SetTargetFPS(game.fps);

    game.font = LoadFontEx("res/JetBrainsMonoNerdFont-Bold.ttf",
                           (game.board_rect.height / 9) * 0.8, NULL, 0);

    board_create(&game.board, &game.font, &game.board_rect);

    return true;
}

bool game_run(void) {
    while (!WindowShouldClose()) {
        if (!game.board_created) {
            switch (game.state) {
                case STATE_PUZZLE_BOARD:
                    board_create_puzzle(&game.board, 10);
                    break;
                case STATE_SOLVER_BOARD:
                    board_create_empty(&game.board);
                    break;
            }
            game.board_created = true;
        }

        if (!game.solve)
            board_update(&game.board, game.state == STATE_SOLVER_BOARD);

        if (IsKeyPressed(KEY_SEMICOLON)) game.solve = true;

        BeginDrawing();

        ClearBackground(WHITE);

        if (!game.solve) {
            switch (game.state) {
                case STATE_START_OPTIOINS:
                    draw_options();
                    break;
                case STATE_PUZZLE_BOARD:
                case STATE_SOLVER_BOARD:
                    board_draw(&game.board);
                    break;
            }
        } else {
            board_solve_anim(&game.board);
        }

        EndDrawing();
    }

    return true;
}

void game_shutdown(void) {
    board_destroy(&game.board);

    UnloadFont(game.font);

    CloseWindow();
}

void draw_options(void) {
}
