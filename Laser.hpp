#pragma once
#include "Entity.hpp"

class Laser : public Entity {
public:
	static constexpr int SPEED = 10;
	static constexpr int WIDTH = 4;
	static constexpr int HEIGHT = 25;
	Laser(Vector2 pos);
	void Draw() const override;
	void updatePosition();
	bool checkOffScreen();
};