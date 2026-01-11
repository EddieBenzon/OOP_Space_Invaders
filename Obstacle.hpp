#pragma once
#include "Entity.hpp"

class Obstacle : public Entity {
public:
	Obstacle(const Texture2D* s, Vector2 pos);
};