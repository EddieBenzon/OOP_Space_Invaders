#include "SpaceShip.hpp"
#include <raylib.h>

SpaceShip::SpaceShip(const Texture2D* s, Vector2 pos) :
Entity(s, pos) {}

void SpaceShip::Draw() const {
	if (image) {
		DrawTexture(*image, (int)position.x, (int)position.y, WHITE);
	}
}

void SpaceShip::MoveLeft() {
	position.x -= 6;
}

void SpaceShip::MoveRight() {
	position.x += 6;
}


Vector2 SpaceShip::getPosition() const {
	return position;
};

int SpaceShip::getWidth() const {
	return image ? (float)image->width : 0;
}