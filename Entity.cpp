#include "Entity.hpp"

Entity::Entity(const Texture2D* s, Vector2 pos) :
	image(s), position(pos), isAlive(true) {}

Rectangle Entity::GetRect() const {
	return Rectangle{
		position.x,
		position.y,
		image ? static_cast<float>(image->width) : 0.0f,
		image ? static_cast<float>(image->height) : 0.0f
	};
}

Vector2 Entity::GetPosition() const { return position; }
void Entity::SetPosition(Vector2 p) { position = p; }
void Entity::Translate(Vector2 t) {
	position.x += t.x;
	position.y += t.y;
}

bool Entity::CheckAlive() { return isAlive; }
void Entity::Kill() { isAlive = false; }

