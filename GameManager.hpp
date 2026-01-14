#pragma once
#include "SpaceShip.hpp"
#include "Entity.hpp"
#include "Laser.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"
#include "EnemyUFO.hpp"
#include <vector>
#include <memory>

class GameManager {
	bool gameStart, playing, gameOver;
	Player player;
	std::string nameBuffer;
	Texture2D backgroundTexture;
	Texture2D shipTexture;
	Texture2D obstacleTexture;
	Texture2D enemyTextureOne;
	Texture2D enemyTextureTwo;
	Texture2D enemyTextureThree;
	Texture2D ufoTexture;
	SpaceShip spaceship;
	std::vector<std::unique_ptr<Entity>> enemies;
	std::vector<std::unique_ptr<Laser>> lasers;
	std::vector<std::unique_ptr<Obstacle>> obstacles;
	std::unique_ptr<EnemyUFO> ufo;

	Music menuMusic;
	Music gameMusic;


	float swarmDirection = 1.0;
	float swarmSpeed = 70.0;     
	float swarmDrop = 18.0;     
	float swarmPadding = 20.0;

	float ufoTimer = 6.0;
	float ufoMinDelay = 8.0;
	float ufoMaxDelay = 15.0;

	void UpdateUFO(float dt);
	void SpawnUFO();
	float RandomFloat(float a, float b);
	bool UFOExists() const;


	void SetState(bool start, bool play, bool over);

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