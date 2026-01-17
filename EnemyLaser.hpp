#pragma once
#include "Entity.hpp"

class EnemyLaser : public Entity {
public:
    static constexpr float SPEED = 300.0;
    static constexpr float WIDTH = 4.0;
    static constexpr float HEIGHT = 20.0;

    EnemyLaser(Vector2 pos);

    void Update(float deltaT);
    void Draw() const override;
    Rectangle GetRect() const;
    void OnKilled(Player& p) override;
};
