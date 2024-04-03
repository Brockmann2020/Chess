#include "raylib.h"
#include "boardmanager.h"

#define SCREEN_WIDTH  (900)
#define SCREEN_HEIGHT (900)

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Window title");
    SetTargetFPS(60);

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BoardManager board(SCREEN_HEIGHT, true);

        board.printBoard();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
