#pragma once

#include <raylib.h>
#include <stdbool.h>

#ifndef NULL
    #define NULL ((void *)0)
#endif

typedef enum SScene {
    SSCENE_MAIN_MENU,
    SSCENE_SOLVER_BOARD,
    SSCENE_PUZZLE_BOARD,
    SSCENE_GAME_END
} SScene;

typedef struct SResources {
        Font *font;
} SResources;

extern SResources resources;
