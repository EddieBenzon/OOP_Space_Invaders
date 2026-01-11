#pragma once
#include "SpaceShip.hpp"
#include "Entity.hpp"
#include <vector>

class GameManager {
	Texture2D shipTexture;
	SpaceShip spaceship;
	std::vector<Entity> enemies;
	std::vector<Entity> lasers;
public:
	GameManager();
	~GameManager();
	void Draw();
	void Update();
	void HandleInput();
};