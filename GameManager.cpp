#include "GameManager.hpp"
#include "Laser.hpp"

GameManager::GameManager() : 
	shipTexture(LoadTexture("Assets/spaceship scaled.png")),
    spaceship(&shipTexture, Vector2{ ((float)GetScreenWidth() - shipTexture.width) / 2, (float)GetScreenHeight() - shipTexture.height - 30 }) {
}
GameManager::~GameManager() {
	UnloadTexture(shipTexture);
}

void GameManager::HandleInput() {
    if (IsKeyDown(KEY_LEFT)) {
        if (spaceship.getPosition().x <= 0) return;
        spaceship.MoveLeft();
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        if (spaceship.getPosition().x >= GetScreenWidth() - spaceship.getWidth()) return;
        spaceship.MoveRight();
    } 
    
    if (IsKeyPressed(KEY_SPACE)) {
        spaceship.Fire();
        Vector2 currentPosition = spaceship.getPosition();
        Vector2 newLaser{
        currentPosition.x + spaceship.getWidth() / 2.0f - 2.0f,
        currentPosition.y - 20
        };
        lasers.push_back(std::make_unique<Laser>(newLaser));
    }
}

void GameManager::Draw() {
    BeginDrawing();

    ClearBackground(BLACK);
    spaceship.Draw();
    for (auto& laser : lasers) {
        laser->Draw();
    }


    EndDrawing();
}

void GameManager::Update() {
    for (auto& laser : lasers) {
        laser->updatePosition();
    }

    lasers.erase(std::remove_if(lasers.begin(), lasers.end(),
        [](const std::unique_ptr<Laser>& laser) {
            return laser->checkOffScreen();
        }), lasers.end());

}