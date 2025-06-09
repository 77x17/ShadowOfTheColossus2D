#include "Eye.hpp"

#include <cmath>
#include <random>

Eye::Eye(float x = 0, float y = 0) : basePosition(x, y) {
    position = basePosition;

    shadow = Animation(TextureManager::get("eyeShadow"), 12,  5, 1, 0, 0.f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.f, false);

    hitbox.setSize(size);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setPosition(x, y);

    detectionBox.setRadius(DETECION_RANGE);
    detectionBox.setOutlineColor(sf::Color::Blue);
    detectionBox.setOutlineThickness(1.f);
    detectionBox.setFillColor(sf::Color::Transparent);
    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );

    animationManager.addAnimation((int)EyeState::IDLE_LEFT , TextureManager::get("eyeSprite"), 20, 20, 2, 0, 0.5f, true );
    animationManager.addAnimation((int)EyeState::IDLE_RIGHT, TextureManager::get("eyeSprite"), 20, 20, 2, 0, 0.5f, false);
    animationManager.addAnimation((int)EyeState::DYING     , TextureManager::get("eyeDead")  , 20, 20, 2, 0, 0.5f, false);
}

bool Eye::isAlive() const {
    return lifeState != EyeState::DEAD;
}

void Eye::respawn() {
    if (respawnCooldownTimer <= 0) {
        position        = basePosition;
        movingDirection = sf::Vector2f(0.f, 0.f);
        
        hitbox.setPosition(position);
        detectionBox.setPosition(
            hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
            hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
        );

        lifeState = EyeState::ALIVE;

        projectile = Projectile();
    }
}

float Eye::distance(const Player& player) const {
    sf::Vector2f normalizeDirection = player.getPosition() - position;
    return std::sqrt(normalizeDirection.x * normalizeDirection.x + normalizeDirection.y * normalizeDirection.y);
}

void Eye::update(Player& player) {
    float dt = deltaClock.restart().asSeconds();
    if (alertCooldownTimer > 0) {
        alertCooldownTimer -= dt;
    }
    if (dyingCooldownTimer > 0) {
        dyingCooldownTimer -= dt;
    }
    if (respawnCooldownTimer > 0) {
        respawnCooldownTimer -= dt;
    }
    if (shootCooldownTimer > 0) {
        shootCooldownTimer -= dt;
    }

    if (lifeState == EyeState::DYING || lifeState == EyeState::DEAD) {
        if (dyingCooldownTimer > 0) {
            animationManager.setState((int)EyeState::DYING);
            animationManager.setPosition(position - sf::Vector2f(4, 4));
            animationManager.update();
        }
        else if (lifeState == EyeState::DYING) {
            lifeState = EyeState::DEAD;

            respawnCooldownTimer = RESPAWN_TIME;
        }

        return;
    }
    else {
        if (player.isCollision(hitbox.getGlobalBounds())) {
            player.kill();
        }
    }

    if (player.isCollisionProjectiles(hitbox.getGlobalBounds())) {
        lifeState = EyeState::DYING;
        dyingCooldownTimer = DYING_TIME;
        
        SoundManager::playSound("enemyHurt");

        return;
    }

    sf::Vector2f normalizeDirection = player.getPosition() - position;
    float length = std::sqrt(normalizeDirection.x * normalizeDirection.x + normalizeDirection.y * normalizeDirection.y);
    
    if (length <= DETECION_RANGE) {
        if (length != 0)
            normalizeDirection /= length;

        if (alertCooldownTimer <= 0 && shootCooldownTimer <= 0) {
            shootCooldownTimer = SHOOT_COOLDOWN / 2.f;
        }

        if (shootCooldownTimer <= 0) {
            projectile = Projectile(
                TextureManager::get("fireball"),
                getPosition() + size / 2.f,
                normalizeDirection,
                PROJECTILE_SPEED,
                PROJECTILE_LIFETIME
            );

            shootCooldownTimer = SHOOT_COOLDOWN;
        }
        else if (shootCooldownTimer < 0.2f) {
            alertCooldownTimer = ALERT_LIFETIME;
        }   
        
        // movingDirection = normalizeDirection;
    }
    // else {
        // randomCooldownTimer được kích hoạt khi quái vật không tìm thấy người chơi trong phạm vi
        // stayingCooldownTimer được chỉnh để bằng 3/4 thời gian của ramdomCooldownTimer
        // mục đích để quái di chuyển 1/4 và 3/4 thời gian còn lại đứng yên tại chỗ
        if (randomCooldownTimer > 0) {
            randomCooldownTimer -= dt;
            
            if (stayingCooldownTimer > 0) {
                stayingCooldownTimer -= dt;
            }
            else {
                movingDirection = sf::Vector2f(rand() % 3 - 1.f, rand() % 3 - 1.f);

                // normalize
                length = std::sqrt(movingDirection.x * movingDirection.x + movingDirection.y * movingDirection.y);
                if (length != 0) {
                    movingDirection /= length * 3;
                }
                else {
                    movingDirection = sf::Vector2f(0.f, 0.f);
                }

                stayingCooldownTimer = randomCooldownTimer;
            }
        }
        else {
            randomCooldownTimer  = RANDOM_TIME;
            stayingCooldownTimer = 3 * RANDOM_TIME / 4;

            movingDirection = sf::Vector2f(0.f, 0.f);
        }
    // }

    if (projectile.isAlive()) { 
        projectile.update(dt);
        
        if (projectile.isCollision(player.getHitBox())) {
            player.kill();
        }
    }
    else {
        projectile = Projectile();
    }

    position += movingDirection * MOVE_SPEED;

    hitbox.setPosition(position);
    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );

    if (movingDirection.x < 0) {
        state = (int)EyeState::IDLE_LEFT;
    }
    else if (movingDirection.x > 0) {
        state = (int)EyeState::IDLE_RIGHT;
    }

    animationManager.setState(state, true);
    animationManager.setPosition(position - sf::Vector2f(4, 4));
    animationManager.update();

    alert .setPosition(position + sf::Vector2f(8, -15));
    shadow.setPosition(position + sf::Vector2f(4, size.y - 8));
}

void Eye::draw(sf::RenderWindow& window) const {
    window.draw(hitbox);
    window.draw(detectionBox);
    
    if (alertCooldownTimer > 0) {
        alert.draw(window);
    }
    shadow.draw(window);
    
    animationManager.draw(window);

    projectile.draw(window);
}

sf::Vector2f Eye::getPosition() const {
    return position;
}