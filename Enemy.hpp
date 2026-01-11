#pragma once
#include "Entity.hpp"
#include "Player.hpp"

class Enemy : Entity {
	int bounty;
public:
	Enemy(const Texture2D* s, Vector2 pos, int b); // moraju imat razlicit bounty
	void addScore(Player* player);
};