#pragma once
#include "Entity.hpp"
#include "Player.hpp"

class Enemy : public Entity {
	int bounty;
public:
	Enemy(const Texture2D* s, Vector2 pos, int b);
	void Draw() const override;
	void OnKilled(Player& p) override;
};