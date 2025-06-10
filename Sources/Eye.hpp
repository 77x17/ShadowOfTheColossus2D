#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.hpp"
#include "Projectile.hpp"

enum class EyeState {
    DYING = -2,
    DEAD  = -1,
    IDLE_LEFT,
    IDLE_RIGHT
};

class Eye : public Enemy {
private:
    Projectile projectile;
    float      SHOOT_COOLDOWN      = 2.0f;
    float      PROJECTILE_SPEED    = MOVE_SPEED * 2;
    float      PROJECTILE_LIFETIME = 1.0f;
    float      shootCooldownTimer  = 0.0f;

public:
    Eye(float x, float y);

    void respawn() override;

    void updateTimer(const float &dt) override;
    void attackPlayer(const Player& player) override;
    void updateAnimation() override;
    void update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects) override;

    void draw(sf::RenderWindow& window) const override;

};