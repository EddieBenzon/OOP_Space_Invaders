#include "GameManager.hpp"
#include "Laser.hpp"
#include "Enemy.hpp"
#include <algorithm>
GameManager::GameManager() :
    shipTexture(LoadTexture("Assets/spaceship scaled.png")),
    spaceship(&shipTexture, Vector2{ (static_cast<float>(GetScreenWidth()) - shipTexture.width) / 2, static_cast<float>(GetScreenHeight()) - shipTexture.height - 70 }),
    obstacleTexture(LoadTexture("Assets/obstacle scaled.png")),
    backgroundTexture(LoadTexture("Assets/space_1.png")),
    enemyTextureOne(LoadTexture("Assets/enemy_1_scaled.png")),
    enemyTextureTwo(LoadTexture("Assets/enemy_2_scaled.png")),
    enemyTextureThree(LoadTexture("Assets/enemy_3_scaled.png"))

{
    constexpr int obstacleCount = 4;
    const float obstacleY = GetScreenHeight() - 300;

    float margin = 200.0;
    float available = GetScreenWidth() - 2.0 * margin;
    float spacing = available / (obstacleCount - 1);

    for (int i = 0; i < obstacleCount; ++i) {
        Vector2 pos{
            margin + spacing * i - obstacleTexture.width / 2.0,
            obstacleY
        };
        obstacles.push_back(std::make_unique<Obstacle>(&obstacleTexture, pos));
    }
}

GameManager::~GameManager() {
	UnloadTexture(shipTexture);
    UnloadTexture(obstacleTexture);
    UnloadTexture(enemyTextureOne);
    UnloadTexture(enemyTextureTwo);
    UnloadTexture(enemyTextureThree);
    UnloadTexture(backgroundTexture);
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
        Vector2 currentPosition = spaceship.getPosition();
        Vector2 newLaser{
        currentPosition.x + spaceship.getWidth()/2.0 - Laser::WIDTH / 2.0,
        currentPosition.y - 20
        };
        lasers.push_back(std::make_unique<Laser>(newLaser));
    }
}

void GameManager::Draw() {
    BeginDrawing();

    DrawTexture(backgroundTexture, 0, 0, WHITE);

    DrawText(
        TextFormat("Score: %d", player.getScore()),
        20, 20, 30, WHITE
    );
    spaceship.Draw();

    for (auto& obstacle : obstacles) {
        obstacle->Draw();
    }

    for (auto& laser : lasers) {
        laser->Draw();
    }

    for (auto& enemy : enemies) {
        enemy->Draw();
    }
    EndDrawing();
}

void GameManager::Update() {
    float deltaT = GetFrameTime();
    HandleInput();
    MoveSwarm(deltaT);

    for (auto& laser : lasers) {
        if (laser->CheckAlive())
            laser->updatePosition();
    }

    ResolveCollisions();
    CleanUp();
    lasers.erase(std::remove_if(lasers.begin(), lasers.end(),
        [](const std::unique_ptr<Laser>& laser) {
            return laser->checkOffScreen();
        }), lasers.end());

}

void GameManager::SpawnEnemies() {
    enemies.clear();
    constexpr int rows = 5;
    constexpr int columns = 11;

    constexpr float startX = 100.0;
    constexpr float startY = 80.0;

    constexpr float spacingX = 90.0;
    constexpr float spacingY = 80.0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            Vector2 newPos{ startX + j * spacingX, startY + i * spacingY };
            const Texture2D* texPointer = nullptr;
            int bounty = 0;
            if (i <= 1) {
                texPointer = &enemyTextureThree;
                bounty = 30;
            }
            else if ( i > 1 && i <= 3) {
                texPointer = &enemyTextureTwo;
                bounty = 20;
            }
            else {
                texPointer = &enemyTextureOne;
                bounty = 10;
            }

            enemies.emplace_back(std::make_unique<Enemy>(texPointer, newPos, bounty));
        }
    }

};

void GameManager::MoveSwarm(float deltaT) {
    bool anyAlive = false;
    float minX = 1e9f;
    float maxX = -1e9f;

    for (const auto& e : enemies) {
        if (!e) continue;
        
        Rectangle rec = e->GetRect();
        anyAlive = true;
        minX = std::min(minX, rec.x);
        maxX = std::max(maxX, rec.x + rec.width);
    }

    if (!anyAlive) return;

    float deltaX = swarmDirection * swarmSpeed * deltaT;
    bool hitLeft = (minX + deltaX <= swarmPadding);
    bool hitRight = (maxX + deltaX >= (GetScreenWidth() - swarmPadding));

    if (hitLeft || hitRight) {
        swarmDirection *= -1;
        for (auto& e : enemies) {
            if (e) e->Translate(Vector2{ 0.0f, swarmDrop });
        }
    }
    else {
        for (auto& e : enemies) {
            if (e) e->Translate(Vector2{ deltaX, 0.0f });
        }
    }
}

void GameManager::ResolveCollisions() {
    for (auto& laser : lasers) {
        if (!laser->CheckAlive()) continue;

        Rectangle recLaser = laser->GetRect();

        for (auto& enemy : enemies) {
            if (!enemy->CheckAlive()) continue;

            if (CheckCollisionRecs(recLaser, enemy->GetRect())) {
                enemy->OnKilled(player);
                enemy->Kill();
                laser->Kill();
                break;
            }
        }

    }

};

void GameManager::CleanUp() {
    lasers.erase(std::remove_if(lasers.begin(), lasers.end(), [](const std::unique_ptr<Laser>& l) {
        return !l || !l->CheckAlive() || l->checkOffScreen();
        }), lasers.end());

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Entity>& e) {
            return !e || !e->CheckAlive();
        }),
        enemies.end());
}