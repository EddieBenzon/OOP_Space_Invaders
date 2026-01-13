#include "Laser.hpp"

Laser::Laser(Vector2 pos) : Entity(nullptr, pos) {}
void Laser::Draw() const {
	DrawRectangle(position.x, position.y, WIDTH, HEIGHT, SKYBLUE);
}

void Laser::updatePosition() {
	position.y -= SPEED;
}

bool Laser::checkOffScreen() {
	return position.y + HEIGHT < 0;
}

Rectangle Laser::GetRect() const {
	return Rectangle{ position.x, position.y, static_cast<float>(WIDTH), static_cast<float>(HEIGHT) };
};