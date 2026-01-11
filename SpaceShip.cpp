#include "SpaceShip.hpp"
#include <raylib.h>
#include <iostream>
SpaceShip::SpaceShip(const Texture2D* s, Vector2 pos) :
Entity(s, pos) {}

void SpaceShip::Draw() const {
	if (image) {
		DrawTexture(*image, (int)position.x, (int)position.y, WHITE);
	}
}

void SpaceShip::MoveLeft() {
	position.x -= 7;
}

void SpaceShip::MoveRight() {
	position.x += 7;
}

void SpaceShip::Fire() {
	std::cout << "Firing weapon!" << std::endl;
	
}

Vector2 SpaceShip::getPosition() const {
	return position;
};

int SpaceShip::getWidth() const {
	return image ? (float)image->width : 0;
}