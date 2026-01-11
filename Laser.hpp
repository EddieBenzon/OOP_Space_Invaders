#pragma once
#include "Entity.hpp"

class Laser : public Entity {
	int speed;
public:
	Laser(Vector2 pos);
	void Draw() const override;
	void updatePosition();
	bool checkOffScreen();
};