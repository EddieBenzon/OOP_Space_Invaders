#include "Enemy.hpp"

Enemy::Enemy(const Texture2D* s, Vector2 pos, int b) : Entity(s, pos), bounty(b) {}

void Enemy::addScore(Player* player) {
	player->addToScore(bounty);
};

void Enemy::Draw() const {
	if (image) {
		DrawTexture(*image, position.x, position.y, WHITE);
	}
};
