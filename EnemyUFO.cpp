#include "EnemyUFO.hpp"

EnemyUFO::EnemyUFO(const Texture2D* s, Vector2 pos, float speed, int direction)
	: Enemy(s, pos, 700), speed(speed), direction(direction) {}


void EnemyUFO::OnKilled(Player& p) {
	p.addToScore(bounty);
};


void EnemyUFO::Update(float deltaT) {
	position.x += direction * speed * deltaT;
};

void EnemyUFO::Draw() const {
	Enemy::Draw();
};