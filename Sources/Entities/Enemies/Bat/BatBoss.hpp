#pragma once

#include <SFML/Graphics.hpp>

#include "Enemy.hpp"
#include "Projectile.hpp"

#include <memory>

enum class BatBossState {
    DESPAWN = -3,
    DYING = -2,
    DEAD  = -1,
    IDLE_LEFT,
    IDLE_RIGHT
};

class BatBoss : public Enemy {
private:
    float ATTACK_RANGE;
    
    std::vector<std::unique_ptr<Projectile>> projectiles;
    float SHOOT_COOLDOWN;
    float PROJECTILE_SPEED;
    float PROJECTILE_LIFETIME;
    float shootCooldownTimer;

    float dashCooldownTimer;
    float dashDuration;
    float DASH_DURATION;
    float DASH_COOLDOWN;
    sf::Vector2f dashDirection;

public:
    BatBoss(const sf::Vector2f& position, const std::vector<std::pair<float, std::shared_ptr<ItemData>>>& _inventory);

    void respawn() override;
    
    void followPlayer(const Player& player) override;

    void updateTimer(const float& dt) override;
    void updateAnimation() override;
    void updateProjectiles(const float& dt, Player& player);
    void update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects, ItemManager& items) override;

    void draw(sf::RenderTarget& target) override;

};