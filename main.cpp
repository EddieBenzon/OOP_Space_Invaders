#include <raylib.h>
#include "GameManager.hpp"
#include "SpaceShip.hpp"

int main() {
    int windowWidth = 1600;
    int windowHeight = 900;
    InitWindow(windowWidth, windowHeight, "Space Invaders");
    SetTargetFPS(60);

    GameManager game;
    game.SpawnEnemies();
    while (!WindowShouldClose()) {
        game.Update();
        game.Draw();
    }


    CloseWindow();
    return 0;
}
