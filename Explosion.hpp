#pragma once
#include "Entity.hpp"
#include <vector>

class Explosion : public Entity {
    std::vector<Rectangle> frames;
    int frameIndex = 0;
    float timer = 0.0;
    float frameInterval = 0.06;
    float scale = 1.0; 

public:
    Explosion(const Texture2D* sheet,
        Vector2 pos,
        std::vector<Rectangle> frames,
        float frameInterval = 0.06f,
        float scale = 1.0f);

    void Update(float deltaT) override;
    void Draw() const override;
    void OnKilled(Player& p) override;
};
