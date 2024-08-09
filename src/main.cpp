#include "raylib.h"
#include "chessboard.h"

#define SCREEN_WIDTH  (900)
#define SCREEN_HEIGHT (900)

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Window title");
    SetTargetFPS(60);
    InitAudioDevice();
    SetTraceLogLevel(LOG_WARNING);

    ChessBoard board(SCREEN_HEIGHT, true);
    board.printBoard();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        board.printBoard();

        board.updateBoard();


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
