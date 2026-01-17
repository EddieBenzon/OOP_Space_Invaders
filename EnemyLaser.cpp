#include "EnemyLaser.hpp"

EnemyLaser::EnemyLaser(Vector2 pos)
    : Entity(nullptr, pos) {
}

void EnemyLaser::Update(float deltaT)
{
    position.y += SPEED * deltaT;
}

void EnemyLaser::Draw() const
{
    DrawRectangle(
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(WIDTH),
        static_cast<int>(HEIGHT),
        RED
    );
}

Rectangle EnemyLaser::GetRect() const
{
    return Rectangle{
        position.x,
        position.y,
        WIDTH,
        HEIGHT
    };
}
void EnemyLaser::OnKilled(Player& p) {};