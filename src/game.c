#include "game.h"

#include <raylib.h>

#include "board.h"

#define NULL ((void *)0)

#define TITLE "SUDOKU"

typedef enum State {
    STATE_START_OPTIOINS,
    STATE_SOLVER_BOARD,
    STATE_PUZZEL_BOARD
} State;

typedef struct Game {
        Font font;
        int width, height, fps;
        Board board;
        Rectangle board_rect;
        State state;
} Game;

static Game game;

bool game_initialize(void) {
    game.width = 900, game.height = 900;
    game.fps = 60;
    game.state = STATE_PUZZEL_BOARD;

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

    // board_create_empty(&game.board);
    board_create_puzzel(&game.board);

    return true;
}

bool game_run(void) {
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(WHITE);

        switch (game.state) {
            case STATE_START_OPTIOINS:
                break;
            case STATE_PUZZEL_BOARD:
            case STATE_SOLVER_BOARD:
                board_draw(&game.board, &game.font, &game.board_rect);
                break;
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
