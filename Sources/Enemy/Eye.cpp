#include "Eye.hpp"

#include <cmath>

Eye::Eye(const float& x = 0, const float& y = 0) : Enemy(x, y, TILE_SIZE, TILE_SIZE, 5.0f, "Eye Lv.5") {
    MOVE_SPEED     = 80.0f; 
    DETECION_RANGE = 250.0f;

    detectionBox.setRadius(DETECION_RANGE);
    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );

    animationManager.addAnimation(static_cast<int>(EyeState::IDLE_LEFT) , TextureManager::get("eyeSprite"), 20, 20, 2, 0, 0.5f, true );
    animationManager.addAnimation(static_cast<int>(EyeState::IDLE_RIGHT), TextureManager::get("eyeSprite"), 20, 20, 2, 0, 0.5f, false);
    animationManager.addAnimation(static_cast<int>(EyeState::DYING)     , TextureManager::get("eyeDead")  , 20, 20, 1, 0, 0.5f, false);

    shadow = Animation(TextureManager::get("eyeShadow"), 12,  5, 1, 0, 0.f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.f, false);
}

void Eye::respawn() {
    if (state == static_cast<int>(EyeState::DEAD) && respawnCooldownTimer <= 0) {
        Enemy::respawn();

        projectile = Projectile();
    }
}

void Eye::kill() {
    Enemy::kill();

    projectile = Projectile();
}

void Eye::updateTimer(const float &dt) {
    Enemy::updateTimer(dt);

    if (shootCooldownTimer > 0) {
        shootCooldownTimer -= dt;
    }
}

void Eye::followPlayer(const Player& player) {
    sf::Vector2f normalizeDirection = player.getPosition() - position;
    float length = std::sqrt(normalizeDirection.x * normalizeDirection.x + normalizeDirection.y * normalizeDirection.y);

    if (length != 0) {
        normalizeDirection /= length;
    }

    if (alertCooldownTimer <= 0 && shootCooldownTimer <= 0) {
        shootCooldownTimer = SHOOT_COOLDOWN / 2.f;
    }

    if (shootCooldownTimer <= 0) {
        projectile = Projectile(
            TextureManager::get("fireball"),
            position + size / 2.f,
            normalizeDirection,
            PROJECTILE_SPEED,
            PROJECTILE_LIFETIME
        );

        shootCooldownTimer = SHOOT_COOLDOWN;
    }
    else if (shootCooldownTimer < 0.2f) {
        alertCooldownTimer = ALERT_LIFETIME;
    }   

    movingDirection = normalizeDirection;

    Enemy::moveRandomly();
}

void Eye::updateAnimation() {
    if (!isAlive()) {
        // nothing
    }
    else if (movingDirection.x < 0) {
        state = static_cast<int>(EyeState::IDLE_LEFT);
    }
    else if (movingDirection.x > 0) {
        state = static_cast<int>(EyeState::IDLE_RIGHT);
    }

    animationManager.setState(state, true);
    animationManager.setPosition(position - sf::Vector2f(4, 4));
    animationManager.update();

    alert .setPosition(position + sf::Vector2f(8, -15));
    shadow.setPosition(position + sf::Vector2f(4, size.y - 8));
}

void Eye::updateProjectiles(const float& dt, Player& player) {
    projectile.update(dt);

    if (projectile.isAlive()) {
        if (projectile.isCollision(player.getHitBox())) {
            player.hurt(1.0f);

            projectile = Projectile();
        }
    }
    else {
        projectile = Projectile();
    }
}   

void Eye::update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects) {
    Enemy::update(dt, player,collisionRects);
    
    if (invincibleCooldownTimer <= 0) {
        updateProjectiles(dt, player);
    }
}

void Eye::draw(sf::RenderWindow& window) {
    Enemy::draw(window);
    
    if (isAlive()) {
        projectile.draw(window);
    }
}