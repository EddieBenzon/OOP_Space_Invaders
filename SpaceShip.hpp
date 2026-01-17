#pragma once
#include <raylib.h>
#include <vector>
#include "Entity.hpp"
class SpaceShip : public Entity{
	bool hitEffectActive = false;
	float hitEffectTimer = 0.0;

	static constexpr float HIT_EFFECT_DURATION = 1.0;

public:
	SpaceShip(const Texture2D* s, Vector2 pos);
	void Draw() const override;
	void MoveLeft();
	void MoveRight();
	Vector2 getPosition() const;
	int getWidth() const;
	void OnKilled(Player& p) override;
	void StartHitEffect();
	void UpdateHitEffect(float dt);
};