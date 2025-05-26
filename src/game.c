#include "game.h"

#include "main_menu.h"

typedef struct Game {
        Font font;
        SScene scene;
} Game;

static Game game;

SResources resources;

bool game_initialize(void) {
    game.scene = SSCENE_MAIN_MENU;

    InitWindow(900, 900, APP_NAME);
    SetTargetFPS(60);

    game.font = LoadFontEx("res/JetBrainsMonoNerdFont-Bold.ttf", 64, NULL, 0);

    resources.font = &game.font;

    main_menu_init();

    return true;
}

bool game_run(void) {
    while (!WindowShouldClose()) {
        // Update part
        switch (game.scene) {
            case SSCENE_MAIN_MENU:
                game.scene = main_menu_update();
                break;
            case SSCENE_SOLVER_BOARD:
                break;
            case SSCENE_PUZZLE_BOARD:
                break;
            case SSCENE_GAME_END:
                break;
            default:
                break;
        }

        BeginDrawing();

        ClearBackground(WHITE);

        // Render part
        switch (game.scene) {
            case SSCENE_MAIN_MENU:
                main_menu_draw();
                break;
            case SSCENE_SOLVER_BOARD:
                break;
            case SSCENE_PUZZLE_BOARD:
                break;
            case SSCENE_GAME_END:
                break;
            default:
                break;
        }

        EndDrawing();
    }

    return true;
}

void game_shutdown(void) {
    main_menu_shutdown();

    UnloadFont(game.font);

    CloseWindow();
}

/*
typedef struct Game {
        Font font;
        int width, height, fps;
        Board board;
        Rectangle board_rect;
        Rectangle in_game_options_rect;
        Rectangle entry_options_rect;
        Rectangle timer_rect;
        GameState state;
        bool solve;
        bool completed;
        Timer timer;
} Game;

static Game game;

bool game_initialize(void) {
    game.width = 900, game.height = 900;
    game.fps = 60;
    game.state = GAME_STATE_MAIN_MENU;
    game.solve = false;
    game.completed = false;

    // float left_padding, right_padding, top_padding, bottom_padding;
    // left_padding = top_padding = right_padding = 50;
    // bottom_padding = 150;

    game.board_rect.x = 50;
    game.board_rect.y = 70;
    game.board_rect.width = game.width - 50 - game.board_rect.x;
    game.board_rect.height = game.height - 150 - game.board_rect.y;

    game.entry_options_rect = (Rectangle){.x = 0 + 50,
                                          .y = 0 + 50,
                                          .width = game.width - 50 - 50,
                                          .height = game.height - 50 -
50};

    game.in_game_options_rect = (Rectangle){
        .x = 0 + 50,
        .y = game.board_rect.y + game.board_rect.height + 25,
        .width = game.board_rect.width,
        .height = game.height - (game.board_rect.y +
game.board_rect.height)
                - 50 - 25};

    game.timer_rect = (Rectangle){.x = game.board_rect.x,
                                  .y = 0 + 10,
                                  .width = game.board_rect.width,
                                  .height = 50};

    InitWindow(game.width, game.height, APP_NAME);
    SetTargetFPS(game.fps);

    game.font = LoadFontEx("res/JetBrainsMonoNerdFont-Bold.ttf",
                           (game.board_rect.height / 9) * 0.8, NULL, 0);

    board_create(&game.board, &game.font, &game.board_rect);

    return true;
}

bool game_run(void) {
    while (!WindowShouldClose()) {
        if (game.state != GAME_STATE_MAIN_MENU) {
            game.completed = board_update(
                &game.board, game.state == GAME_STATE_SOLVER_BOARD);
        }

        if (game.state == GAME_STATE_MAIN_MENU) {
            game.state = options_update(ENTRY_OPTIONS);
            switch (game.state) {
                case GAME_STATE_PUZZLE_BOARD:
                    board_create_puzzle(&game.board, 10);
                    timer_start(&game.timer);
                    break;
                case GAME_STATE_SOLVER_BOARD:
                    board_create_empty(&game.board);
                    timer_start(&game.timer);
                    break;
                default:
                    break;
            }
        }

        static bool start_timer = false;
        if (game.solve && !start_timer) {
            start_timer = true;
            timer_start(&game.timer);
        }

        if (IsKeyPressed(KEY_SEMICOLON) && game.state !=
GAME_STATE_MAIN_MENU) game.solve = true;

        BeginDrawing();

        ClearBackground(WHITE);

        // DrawRectangleLinesEx(game.entry_options_rect, 2,
        //                      (Color){128, 254, 89, 255});
        // DrawRectangleLinesEx(game.in_game_options_rect, 2,
        //                      (Color){255, 0, 0, 128});
        // DrawRectangleLinesEx(game.timer_rect, 2, (Color){255, 0, 0,
128});

        if (!game.solve) {
            switch (game.state) {
                case GAME_STATE_MAIN_MENU:
                    options_draw(&game.entry_options_rect,
ENTRY_OPTIONS, &game.font); break; case GAME_STATE_PUZZLE_BOARD: case
GAME_STATE_SOLVER_BOARD: timer_darw(&game.timer, &game.timer_rect,
&game.font); board_draw(&game.board); break; default: break;
            }
        } else {
            timer_darw(&game.timer, &game.timer_rect, &game.font);
            board_solve_anim(&game.board);
        }

        if (game.completed) {
            DrawTextEx(
                game.font, "Completed!",
                (Vector2){.x = (game.width - (10 *
game.font.recs->width)) / 2, .y = (game.height - game.font.recs->height)
/ 2}, game.font.baseSize, 1, GREEN);
        }

        EndDrawing();
    }

    return true;
}

*/
