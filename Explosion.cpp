#include "Explosion.hpp"

Explosion::Explosion(const Texture2D* sheet, Vector2 pos, std::vector<Rectangle> f, float interval, float s)
    : Entity(sheet, pos), frames(std::move(f)), frameInterval(interval), scale(s)
{
}

void Explosion::Update(float dt)
{
    if (!CheckAlive()) return;

    timer += dt;
    if (timer >= frameInterval) {
        timer -= frameInterval;
        frameIndex++;

        if (frameIndex >= (int)frames.size()) {
            Kill();
        }
    }
}

void Explosion::Draw() const
{
    if (!CheckAlive() || !image) return;
    if (frameIndex < 0 || frameIndex >= (int)frames.size()) return;

    Rectangle src = frames[frameIndex];

    Rectangle dst{
        position.x,
        position.y,
        src.width * scale,
        src.height * scale
    };

    DrawTexturePro(*image, src, dst, Vector2{ 0,0 }, 0.0f, WHITE);
}

void Explosion::OnKilled(Player& p) { (void)p; }
