#pragma once
#include "Entity.hpp"
#include "Player.hpp"
class Enemy : public Entity {
protected:
    int bounty;

    const Texture2D* frameA = nullptr;
    const Texture2D* frameB = nullptr;

    float animTimer = 0.0;
    static constexpr float animInterval = 1.15;
    bool usingA = true;

public:
    Enemy(const Texture2D* texA, const Texture2D* texB, Vector2 pos, int bountyValue);

    void Update(float deltaT) override;
    void Draw() const override;
    void OnKilled(Player& p) override;
};
