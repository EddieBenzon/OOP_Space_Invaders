#include "Enemy.hpp"

Enemy::Enemy(const Texture2D* texA, const Texture2D* texB, Vector2 pos, int bountyValue)
    : Entity(texA, pos), bounty(bountyValue), frameA(texA), frameB(texB)
{
    image = frameA;
}

void Enemy::OnKilled(Player& player) {
	player.addToScore(bounty);
};

void Enemy::Update(float deltaT)
{
    if (!CheckAlive()) return;

    animTimer += deltaT;
    if (animTimer >= animInterval) {
        animTimer -= animInterval;
        usingA = !usingA;
        image = usingA ? frameA : frameB;
    }
}

void Enemy::Draw() const {
	if (image) {
		DrawTexture(*image, position.x, position.y, WHITE);
	}
};
