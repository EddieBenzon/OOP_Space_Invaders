#pragma once
#include "SpaceShip.hpp"
#include "Entity.hpp"
#include "Laser.hpp"
#include "EnemyLaser.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"
#include "EnemyUFO.hpp"
#include "Explosion.hpp"
#include <vector>
#include <memory>


class GameManager {
	bool gameStart, playing, gameOver;
	Player player;
	std::string nameBuffer;
	Texture2D backgroundTexture;
	Texture2D shipTexture;
	Texture2D obstacleTexture;
	Texture2D enemyRedA, enemyRedB;
	Texture2D enemyGreenA, enemyGreenB;
	Texture2D enemyBlueA, enemyBlueB;
	Texture2D ufoTexture;
	Texture2D explosionSheet;
	SpaceShip spaceship;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::vector<std::unique_ptr<Laser>> lasers;
	std::vector<std::unique_ptr<Obstacle>> obstacles;
	std::vector<std::unique_ptr<EnemyLaser>> enemyLasers;
	std::unique_ptr<EnemyUFO> ufo;
	std::vector<std::unique_ptr<Explosion>> explosions;


	Sound sfxLaser;
	Sound sfxEnemyHit;
	Sound sfxPlayerHit;
	Music menuMusic;
	Music gameMusic;
	Music gameOverMusic;

	float enemyFireTimer = 1.5;
	float enemyFireMin = 0.8;
	float enemyFireMax = 2.0;

	int currentWave = 1;
	bool waitingForNextWave = false;
	float nextWaveTimer = 0.0;

	const float nextWaveDelay = 2.0;

	float swarmDirection = 1.0;
	float swarmSpeed = 70.0;     
	float swarmDrop = 18.0;     
	float swarmPadding = 20.0;

	float ufoTimer = 10.0;
	float ufoMinDelay = 8.0;
	float ufoMaxDelay = 15.0;

	void UpdateUFO(float deltaT);
	void SpawnUFO();
	float RandomFloat(float a, float b);
	bool UFOExists() const;


	void SetState(bool start, bool play, bool over);
	Enemy* GetRandomAliveEnemy();
	void UpdateEnemyFire(float deltaT);

	float playerFireCooldown = 0.45;  
	float playerFireTimer = 0.0;      


public:
	GameManager();
	~GameManager();
	void Update();
	void HandleInput();
	void SpawnEnemies();
	void Draw();
	void MoveSwarm(float deltaT);
	void ResolveCollisions();
	void CleanUp();
	void UpdateNameBuffer();
	void StartNewGame();
};