#include <raylib.h>
#include "GameManager.hpp"
#include "SpaceShip.hpp"

int main() {
    int windowWidth = 1300;
    int windowHeight = 900;
    InitWindow(windowWidth, windowHeight, "Space Invaders");
    SetTargetFPS(60);

    GameManager game;
    while (!WindowShouldClose()) {
        game.HandleInput();
        game.Update();
        game.Draw();
    }


    CloseWindow();
    return 0;
}
