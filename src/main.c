#include <raylib.h>

#include "game.h"

int main(void) {
    if (!game_initialize()) return -1;

    if (!game_run()) return -1;

    game_shutdown();
}
