#include <raylib.h>
#include <stdio.h>

int main(void) {
    printf("Hello!\n");

    InitWindow(600, 400, "Sudoku");
    SetTargetFPS(60);

    Font font = LoadFont("res/JetBrainsMonoNerdFont-Bold.ttf");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawTextEx(font, "Welcome to Sudoku!", (Vector2){50, 50}, 30, 1, WHITE);

        EndDrawing();
    }

    UnloadFont(font);

    CloseWindow();
}
