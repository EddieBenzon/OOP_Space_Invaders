#include "Obstacle.hpp"

Obstacle::Obstacle(const Texture2D* s, Vector2 pos) : Entity(s, pos) {}

void Obstacle::Draw() const {
	if (image) {
		DrawTexture(*image, (int)position.x, (int)position.y, WHITE);
	}
}

void Obstacle::OnKilled(Player& p) {};