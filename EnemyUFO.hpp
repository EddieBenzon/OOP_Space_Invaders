#pragma once
#include "Enemy.hpp"

class EnemyUFO : public Enemy {
public:
	EnemyUFO(const Texture2D* s, Vector2 pos, int b);
};