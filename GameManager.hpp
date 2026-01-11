#pragma once
#include "SpaceShip.hpp"
#include "Entity.hpp"
#include "Laser.hpp"
#include <vector>
#include <memory>

class GameManager {
	Texture2D shipTexture;
	SpaceShip spaceship;
	std::vector<std::unique_ptr<Entity>> enemies;
	std::vector<std::unique_ptr<Laser>> lasers;
public:
	GameManager();
	~GameManager();
	void Update();
	void HandleInput();
	void Draw();
};