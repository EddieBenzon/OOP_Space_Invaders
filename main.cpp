#include <raylib.h>
#include "GameManager.hpp"


int main() {
    int windowWidth = 1300;
    int windowHeight = 900;
    InitWindow(windowWidth, windowHeight, "Space Invaders");
    SetTargetFPS(60);

    GameManager game;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("IS IT WORKING?", 200, 280, 48, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
