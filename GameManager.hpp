#pragma once
#include "SpaceShip.hpp"
#include "Entity.hpp"
#include "Laser.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"
#include <vector>
#include <memory>

class GameManager {
	Player player;
	Texture2D backgroundTexture;
	Texture2D shipTexture;
	Texture2D obstacleTexture;
	Texture2D enemyTextureOne;
	Texture2D enemyTextureTwo;
	Texture2D enemyTextureThree;
	SpaceShip spaceship;
	std::vector<std::unique_ptr<Entity>> enemies;
	std::vector<std::unique_ptr<Laser>> lasers;
	std::vector<std::unique_ptr<Obstacle>> obstacles;

	float swarmDirection = 1.0;
	float swarmSpeed = 70.0;     
	float swarmDrop = 18.0;     
	float swarmPadding = 20.0;

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
};