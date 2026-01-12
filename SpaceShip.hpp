#pragma once
#include <raylib.h>
#include <vector>
#include "Entity.hpp"
class SpaceShip : public Entity{

public:
	SpaceShip(const Texture2D* s, Vector2 pos);
	void Draw() const override;
	void MoveLeft();
	void MoveRight();
	Vector2 getPosition() const;
	int getWidth() const;
};