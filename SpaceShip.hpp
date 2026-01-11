#pragma once
#include <raylib.h>
#include "Entity.hpp"
class SpaceShip : public Entity{

public:
	SpaceShip(const Texture2D* s, Vector2 pos);
	void Draw() const override;
	void MoveLeft();
	void MoveRight();
	void Fire();
};