#pragma once
#include <raylib.h>

class Player;

class Entity {
protected:
	const Texture2D* image = nullptr;
	Vector2 position{};
	bool isAlive;
public:
	Entity(const Texture2D* s, Vector2 pos);
	virtual ~Entity() = default;
	virtual void Draw() const = 0;

	Rectangle GetRect() const;
	Vector2 GetPosition() const;
	void SetPosition(Vector2 p);
	void Translate(Vector2 t);
	bool CheckAlive();
	void Kill();
	virtual void OnKilled(Player& p) = 0;
};