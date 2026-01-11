#pragma once
#include <raylib.h>

class Entity {
protected:
	const Texture2D* image = nullptr;
	Vector2 position{};
public:
	Entity(const Texture2D* s, Vector2 pos);
	virtual ~Entity() = default;
	virtual void Draw() const = 0;
};