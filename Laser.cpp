#include "Laser.hpp"

Laser::Laser(Vector2 pos) : Entity(nullptr, pos), speed(10){}
void Laser::Draw() const {
	DrawRectangle((int)position.x, (int)position.y, 5, 20, SKYBLUE);
}

void Laser::updatePosition() {
	position.y -= speed;
}

bool Laser::checkOffScreen() {
	return position.y + 20 < 0;
}