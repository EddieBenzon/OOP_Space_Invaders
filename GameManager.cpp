#include "GameManager.hpp"
#include "Laser.hpp"
#include "Enemy.hpp"
#include "EnemyUFO.hpp"
#include <algorithm>
GameManager::GameManager() :
    shipTexture(LoadTexture("Assets/spaceship scaled.png")),
    spaceship(&shipTexture, Vector2{ (static_cast<float>(GetScreenWidth()) - shipTexture.width) / 2, static_cast<float>(GetScreenHeight()) - shipTexture.height - 70 }),
    obstacleTexture(LoadTexture("Assets/obstacle scaled.png")),
    backgroundTexture(LoadTexture("Assets/space_1.png")),
    enemyTextureOne(LoadTexture("Assets/enemy_1_scaled.png")),
    enemyTextureTwo(LoadTexture("Assets/enemy_2_scaled.png")),
    enemyTextureThree(LoadTexture("Assets/enemy_3_scaled.png")),
    ufoTexture(LoadTexture("Assets/ufo_scaled.png")),
    gameStart(true), playing(false), gameOver(false)
{
    InitAudioDevice();
    menuMusic = LoadMusicStream("Assets/8bit_menu.mp3");
    gameMusic = LoadMusicStream("Assets/16bit_loop.mp3");
    SetMusicVolume(menuMusic, 0.6f);
    SetMusicVolume(gameMusic, 0.6f);
    menuMusic.looping = false;
    gameMusic.looping = true;

    PlayMusicStream(menuMusic);
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
    UnloadMusicStream(menuMusic);
    UnloadMusicStream(gameMusic);
    CloseAudioDevice();
	UnloadTexture(shipTexture);
    UnloadTexture(obstacleTexture);
    UnloadTexture(enemyTextureOne);
    UnloadTexture(enemyTextureTwo);
    UnloadTexture(enemyTextureThree);
    UnloadTexture(backgroundTexture);
    UnloadTexture(ufoTexture);

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
    ClearBackground(BLACK);
    if (gameStart)
    {
        DrawText("KOPILICA INVADERS", 560, 200, 50, WHITE);
        DrawText("Enter name:", 650, 320, 30, WHITE);

        DrawRectangleLines(600, 360, 400, 50, WHITE);
        DrawText(nameBuffer.c_str(), 610, 370, 30, WHITE);

        DrawText("Press ENTER to Play", 610, 450, 25, WHITE);

        EndDrawing();
        return;
    }

    if (gameOver)
    {
        DrawText("GAME OVER", 650, 250, 60, WHITE);
        DrawText(TextFormat("Player: %s", player.getName().c_str()), 650, 350, 30, WHITE);
        DrawText(TextFormat("Score: %d", player.getScore()), 650, 390, 30, WHITE);
        DrawText("Press ENTER to return", 610, 470, 25, WHITE);

        EndDrawing();
        return;
    }

    DrawTexture(backgroundTexture, 0, 0, WHITE);

    DrawText(
        TextFormat("Score: %d", player.getScore()),
        20, 20, 30, WHITE
    );
    DrawText(TextFormat("ufoTimer: %.2f", ufoTimer), 20, 80, 24, WHITE); // test
    DrawText(TextFormat("ufo ptr: %s", (ufo ? "YES" : "NO")), 20, 110, 24, WHITE); // test

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
    if (ufo && ufo->CheckAlive())
        ufo->Draw();
    EndDrawing();
}

void GameManager::Update() {
    UpdateMusicStream(menuMusic);
    
    float deltaT = GetFrameTime();

    if (gameStart)
    {
        UpdateNameBuffer();
        if (IsKeyPressed(KEY_ENTER)) {
            StartNewGame();
            StopMusicStream(menuMusic);
            PlayMusicStream(gameMusic);
        }
        return;
    }


    if (gameOver)
    {
        StopMusicStream(gameMusic);
        if (IsKeyPressed(KEY_ENTER))
        {
            PlayMusicStream(menuMusic);
            nameBuffer.clear();
            SetState(true, false, false);
        }
        return;
    }

    if (playing) {
        UpdateMusicStream(gameMusic);
        HandleInput();
        MoveSwarm(deltaT);
        UpdateUFO(deltaT);

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

        if (enemies.empty())
            SetState(false, false, true);

        return;
    }
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

    if (ufo && ufo->CheckAlive()) {
        for (auto& laser : lasers) {
            if (!laser->CheckAlive()) continue;

            if (CheckCollisionRecs(laser->GetRect(), ufo->GetRect())) {
                ufo->OnKilled(player);
                ufo->Kill();
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

    if (ufo && !ufo->CheckAlive())
        ufo.reset();
}

void GameManager::SetState(bool start, bool play, bool over) {
    gameStart = start;
    playing = play;
    gameOver = over;
}

void GameManager::UpdateNameBuffer() {
    int c = GetCharPressed();
    while (c > 0) {
        if (c >= 32 && c <= 126 && nameBuffer.size() < 12) nameBuffer.push_back(c);

        c = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !nameBuffer.empty())
        nameBuffer.pop_back();
};

void GameManager::StartNewGame() {
    player.setName(nameBuffer.empty() ? "Nameless one" : nameBuffer);
    player.reset();

    lasers.clear();
    enemies.clear();
    SpawnEnemies();
    SetState(false, true, false);
};


float GameManager::RandomFloat(float a, float b)
{
    int r = GetRandomValue(0, 10000);
    float t = (float)r / 10000.0f;
    return a + (b - a) * t;
}

bool GameManager::UFOExists() const
{
    for (const auto& e : enemies)
    {
        if (!e || !e->CheckAlive()) continue;
        if (dynamic_cast<const EnemyUFO*>(e.get()) != nullptr)
            return true;
    }
    return false;
}

void GameManager::SpawnUFO()
{
    if (UFOExists()) return;

    int dir = (GetRandomValue(0, 1) == 0) ? 1 : -1;

    float startX = (dir == 1)
        ? -(float)ufoTexture.width
        : (float)GetScreenWidth() + (float)ufoTexture.width;

    Vector2 pos{ startX, 40.0f };

    ufo = std::make_unique<EnemyUFO>(
        &ufoTexture,
        pos,
        250.0f,
        dir
    );
}

void GameManager::UpdateUFO(float dt)
{
    ufoTimer -= dt;
    if (ufoTimer <= 0.0f) {
        SpawnUFO();
        ufoTimer = GetRandomValue(
            (int)(ufoMinDelay * 1000),
            (int)(ufoMaxDelay * 1000)
        ) / 1000.0f;
    }

    if (ufo && ufo->CheckAlive()) {
        ufo->Update(dt);

        Rectangle r = ufo->GetRect();
        if (r.x + r.width < 0 || r.x > GetScreenWidth())
            ufo->Kill();
    }
}
