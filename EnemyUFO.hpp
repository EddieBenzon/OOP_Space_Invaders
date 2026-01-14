#pragma once
#include "Enemy.hpp"

class EnemyUFO : public Enemy {
	float speed;
	int direction;
public:
	EnemyUFO(const Texture2D* s, Vector2 pos, float speed, int direction);
	void Update(float deltaT);
	void Draw() const override;
	void OnKilled(Player& p) override;
};