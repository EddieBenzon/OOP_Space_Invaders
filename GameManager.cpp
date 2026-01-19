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
    enemyRedA(LoadTexture("Assets/red1.png")),
    enemyRedB(LoadTexture("Assets/red2.png")),
    enemyGreenA(LoadTexture("Assets/green1.png")),
    enemyGreenB(LoadTexture("Assets/green2.png")),
    enemyBlueA(LoadTexture("Assets/blue1.png")),
    enemyBlueB(LoadTexture("Assets/blue2.png")),
    ufoTexture(LoadTexture("Assets/UFO_Pixel.png")),
    gameStart(true), playing(false), gameOver(false)
{
    InitAudioDevice();

    sfxLaser = LoadSound("Assets/laser_shot.wav");
    sfxEnemyHit = LoadSound("Assets/enemy_hit.wav");
    sfxPlayerHit = LoadSound("Assets/player_hit.wav");
    menuMusic = LoadMusicStream("Assets/8bit_menu.mp3");
    gameMusic = LoadMusicStream("Assets/16bit_loop.mp3");
    gameOverMusic = LoadMusicStream("Assets/game_over.mp3");

    SetMusicVolume(menuMusic, 0.6f);
    SetMusicVolume(gameMusic, 0.6f);
    SetMusicVolume(gameOverMusic, 0.4f);
    SetSoundVolume(sfxLaser, 0.4f);
    SetSoundVolume(sfxEnemyHit, 0.7f);
    SetSoundVolume(sfxPlayerHit, 0.7f);

    menuMusic.looping = false;
    gameMusic.looping = true;
    gameOverMusic.looping = false;

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
    UnloadMusicStream(gameOverMusic);
    UnloadSound(sfxLaser);
    UnloadSound(sfxEnemyHit);
    UnloadSound(sfxPlayerHit);
    CloseAudioDevice();

    UnloadTexture(shipTexture);
    UnloadTexture(obstacleTexture);
    UnloadTexture(enemyRedA);
    UnloadTexture(enemyRedB);
    UnloadTexture(enemyGreenA);
    UnloadTexture(enemyGreenB);
    UnloadTexture(enemyBlueA);
    UnloadTexture(enemyBlueB);
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
    
    if (IsKeyDown(KEY_SPACE) && playerFireTimer <= 0.0f) {
        Vector2 currentPosition = spaceship.getPosition();
        Vector2 newLaser{
        currentPosition.x + spaceship.getWidth()/2.0 - Laser::WIDTH / 2.0,
        currentPosition.y - 20
        };
        lasers.push_back(std::make_unique<Laser>(newLaser));
        PlaySound(sfxLaser);
        playerFireTimer = playerFireCooldown;
    }
}

void GameManager::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    if (gameStart)
    {
        DrawText("SPACE INVADERS", 560, 200, 50, WHITE);
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
    DrawText(TextFormat("Lives: %d", player.getLives()), 20, 80, 24, WHITE);
    DrawText(
        TextFormat("Score: %d", player.getScore()),
        20, 20, 30, WHITE
    );

    for (auto& l : enemyLasers) {
        if (l->CheckAlive())
            l->Draw();
    }


    if (waitingForNextWave) {
        DrawText(
            TextFormat("WAVE %d", currentWave + 1),
            GetScreenWidth() / 2 - 80,
            GetScreenHeight() / 2 - 20,
            40,
            WHITE
        );
    }

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
    
    float deltaT = GetFrameTime();
    for (auto& e : enemies)
        if (e) e->Update(deltaT);
    if (gameStart) {
        UpdateMusicStream(menuMusic);
        UpdateNameBuffer();
        if (IsKeyPressed(KEY_ENTER)) {
            StartNewGame();
            StopMusicStream(menuMusic);
            PlayMusicStream(gameMusic);
        }
        return;
    }


    if (gameOver) {
        UpdateMusicStream(gameOverMusic);
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
        UpdateEnemyFire(deltaT);
        playerFireTimer -= deltaT;
        spaceship.UpdateHitEffect(deltaT);

        for (auto& l : enemyLasers)
            if (l->CheckAlive())
                l->Update(deltaT);

        enemyLasers.erase(std::remove_if(
            enemyLasers.begin(),
            enemyLasers.end(),
            [](const std::unique_ptr<EnemyLaser>& l) {
                return !l || !l->CheckAlive() || l->GetPosition().y > GetScreenHeight();
            }),
            enemyLasers.end()
        );

        if (waitingForNextWave) {
            nextWaveTimer -= deltaT;

            if (nextWaveTimer <= 0.0) {
                currentWave++;
                SpawnEnemies();
                swarmSpeed *= 1.05;
                waitingForNextWave = false;
            }

            return;
        }


        float loseLine = GetScreenHeight() - 300;

        for (auto& e : enemies) {
            if (!e || !e->CheckAlive()) continue;

            if (e->GetPosition().y + e->GetRect().height >= loseLine)
            {
                SetState(false, false, true);
                PlayMusicStream(gameOverMusic);
                break;
            }
        }

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

        if (enemies.empty() && !waitingForNextWave) {
            waitingForNextWave = true;
            nextWaveTimer = nextWaveDelay;
        }

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
            const Texture2D* a = nullptr;
            const Texture2D* b = nullptr;
            int bounty = 0;

            if (i <= 1) { a = &enemyRedA; b = &enemyRedB; bounty = 10; }
            else if (i <= 3) { a = &enemyGreenA; b = &enemyGreenB; bounty = 20; }
            else { a = &enemyBlueA; b = &enemyBlueB; bounty = 30; }

            enemies.emplace_back(std::make_unique<Enemy>(a, b, newPos, bounty));

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
                PlaySound(sfxEnemyHit);
                break;
            }
        }
        for (auto& obs : obstacles) {
            if (!obs || !obs->CheckAlive()) continue;

            if (CheckCollisionRecs(recLaser, obs->GetRect()))
            {
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
                PlaySound(sfxEnemyHit);
                break;
            }
        }
    }

    for (auto& l : enemyLasers) {
        if (!l->CheckAlive()) continue;

        if (CheckCollisionRecs(l->GetRect(), spaceship.GetRect()))
        {
            l->Kill();
            player.loseLife();
            spaceship.StartHitEffect();
            PlaySound(sfxPlayerHit);
            if (player.getLives() <= 0)
            {
                playing = false;
                gameOver = true;

                StopMusicStream(gameMusic);
                PlayMusicStream(gameOverMusic);
            }

            break;
        }
        for (auto& obs : obstacles)
        {
            if (!obs || !obs->CheckAlive()) continue;

            if (CheckCollisionRecs(l->GetRect(), obs->GetRect()))
            {
                l->Kill();
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
        [](const std::unique_ptr<Enemy>& e) {
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
    enemyLasers.clear();
    enemyFireTimer = 1.5;
    player.reset();
    lasers.clear();
    enemies.clear();
    playerFireTimer = 0.0f;
    currentWave = 1;
    waitingForNextWave = false;
    nextWaveTimer = 0.0f;
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


Enemy* GameManager::GetRandomAliveEnemy()
{
    std::vector<Enemy*> alive;

    for (auto& e : enemies)
        if (e && e->CheckAlive())
            alive.push_back(e.get());

    if (alive.empty())
        return nullptr;

    int index = GetRandomValue(0, (int)alive.size() - 1);
    return alive[index];
}

void GameManager::UpdateEnemyFire(float deltaT)
{
    enemyFireTimer -= deltaT;

    if (enemyFireTimer <= 0.0f)
    {
        Enemy* shooter = GetRandomAliveEnemy();
        if (shooter)
        {
            Vector2 pos{
                shooter->GetPosition().x + shooter->GetRect().width / 2.0f - EnemyLaser::WIDTH / 2.0f,
                shooter->GetPosition().y + shooter->GetRect().height
            };

            enemyLasers.push_back(std::make_unique<EnemyLaser>(pos));
        }

        enemyFireTimer = GetRandomValue(
            (int)(enemyFireMin * 1000),
            (int)(enemyFireMax * 1000)
        ) / 1000.0f;
    }
}
