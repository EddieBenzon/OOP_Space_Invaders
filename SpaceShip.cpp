#include "SpaceShip.hpp"
#include <raylib.h>

SpaceShip::SpaceShip(const Texture2D* s, Vector2 pos) :
Entity(s, pos) {}

void SpaceShip::Draw() const {
    if (!image) return;

    Color tint = WHITE;

    if (hitEffectActive)
    {
        float phase = fmod(hitEffectTimer * 10.0, 2.0);
        if (phase < 1.0)
            tint = Color{ 255, 255, 255, 60 };
    }

    DrawTexture(*image, position.x, position.y, tint);
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

void SpaceShip::OnKilled(Player& p) {};

void SpaceShip::StartHitEffect()
{
    hitEffectActive = true;
    hitEffectTimer = HIT_EFFECT_DURATION;
}

void SpaceShip::UpdateHitEffect(float dt)
{
    if (!hitEffectActive) return;

    hitEffectTimer -= dt;
    if (hitEffectTimer <= 0.0)
    {
        hitEffectActive = false;
        hitEffectTimer = 0.0;
    }
}
