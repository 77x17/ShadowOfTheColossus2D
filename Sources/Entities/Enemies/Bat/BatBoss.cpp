#include "BatBoss.hpp"

#include "TextureManager.hpp"

BatBoss::BatBoss(const sf::Vector2f& position, const std::vector<std::pair<float, std::shared_ptr<ItemData>>>& _inventory) 
: Enemy(position, sf::Vector2f(TILE_SIZE, TILE_SIZE) * 2.0f, 100.0f, "Bat Boss Lv.10", _inventory) {
    // --- [Begin] - Configuration ---
    MOVE_SPEED = 100.0f; 

    DETECION_RANGE = 350.0f;
    KNOCKBACK_COOLDOWN = 0.05f;
    KNOCKBACK_STRENGTH = 50.0f;

    ATTACK_RANGE = 100.0f;

    damagePerAttack = 5.0f;
    expAmount       = 50.0f;
    // --- [End] ---

    // --- [Begin] - Animation ---
    detectionBox.setRadius(DETECION_RANGE);
    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );

    animationManager.addAnimation(static_cast<int>(BatBossState::IDLE_LEFT) , TextureManager::get("batBoss"), 50, 50, 4, 0, 0.5f, false);
    animationManager.addAnimation(static_cast<int>(BatBossState::IDLE_RIGHT), TextureManager::get("batBoss"), 50, 50, 4, 0, 0.5f, true);
    animationManager.addAnimation(static_cast<int>(BatBossState::DYING)     , TextureManager::get("batBossDead"), 50, 50, 1, 0, 0.5f, false);
    
    shadow = Animation(TextureManager::get("batShadow"), 12,  5, 1, 0, 0.0f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.0f, false);
    // --- [End] ---
}

void BatBoss::followPlayer(const Player& player) {
    Enemy::followPlayer(player);
    
    if (calculateDistance(player) <= ATTACK_RANGE) {    
        movingDirection   *= 1.5f;
    }
}

void BatBoss::updateTimer(const float& dt) {
    Enemy::updateTimer(dt);
}

void BatBoss::updateAnimation() {
    if (!isAlive() || knockbackCooldownTimer > 0) {
        // nothing
    }
    else if (movingDirection.x < 0) {
        state = static_cast<int>(BatBossState::IDLE_LEFT);
    }
    else if (movingDirection.x > 0) {
        state = static_cast<int>(BatBossState::IDLE_RIGHT);
    }

    animationManager.setState(state, true);
    animationManager.setPosition(hitbox.getPosition() - sf::Vector2f(18.5f, 20.0f));
    animationManager.update();

    alert .setPosition(hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x / 2.0f - alert.getSprite().getLocalBounds().width / 2.0f, -hitbox.getSize().y / 2.0f));
    shadow.setPosition(hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x / 2.0f - 10.0f, hitbox.getSize().y));
}