#include "Bat.hpp"

#include <cmath>
#include <random>

Bat::Bat(float x = 0, float y = 0) : basePosition(x, y) {
    position = basePosition;

    shadow = Animation(TextureManager::get("batShadow"), 12,  5, 1, 0, 0.f, false);
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

    animationManager.addAnimation((int)BatState::IDLE_LEFT , TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, true );
    animationManager.addAnimation((int)BatState::IDLE_RIGHT, TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, false);
    animationManager.addAnimation((int)BatState::DYING     , TextureManager::get("batDead")  , 16, 16, 1, 0, 0.5f, false);
}

bool Bat::isAlive() const {
    return lifeState != BatState::DEAD;
}

void Bat::respawn() {
    if (respawnCooldownTimer <= 0) {
        position        = basePosition;
        movingDirection = sf::Vector2f(0.f, 0.f);
        
        hitbox.setPosition(position);
        detectionBox.setPosition(
            hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
            hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
        );

        lifeState = BatState::ALIVE;
    }
}

float Bat::distance(const Player& player) const {
    sf::Vector2f normalizeDirection = player.getPosition() - position;
    return std::sqrt(normalizeDirection.x * normalizeDirection.x + normalizeDirection.y * normalizeDirection.y);
}

void Bat::update(Player& player) {
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

    if (lifeState == BatState::DYING || lifeState == BatState::DEAD) {
        if (dyingCooldownTimer > 0) {
            animationManager.setState((int)BatState::DYING);
            animationManager.setPosition(position);
            animationManager.update();
        }
        else if (lifeState == BatState::DYING) {
            lifeState = BatState::DEAD;

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
        lifeState = BatState::DYING;
        dyingCooldownTimer = DYING_TIME;
        
        SoundManager::playSound("enemyHurt");

        return;
    }

    sf::Vector2f normalizeDirection = player.getPosition() - position;
    float length = std::sqrt(normalizeDirection.x * normalizeDirection.x + normalizeDirection.y * normalizeDirection.y);
    
    if (length <= DETECION_RANGE) {
        if (length != 0)
            normalizeDirection /= length;
    
        if (alertCooldownTimer <= 0 && randomCooldownTimer > 0) {
            alertCooldownTimer  = ALERT_LIFETIME;
            randomCooldownTimer = 0;
        }
        
        movingDirection = normalizeDirection;
    }
    else {
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
    }
    
    position += movingDirection * MOVE_SPEED;

    hitbox.setPosition(position);
    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );

    if (movingDirection.x < 0) {
        state = (int)BatState::IDLE_LEFT;
    }
    else if (movingDirection.x > 0) {
        state = (int)BatState::IDLE_RIGHT;
    }

    animationManager.setState(state, true);
    animationManager.setPosition(position);
    animationManager.update();

    alert .setPosition(position + sf::Vector2f(8, -15));
    shadow.setPosition(position + sf::Vector2f(4, size.y - 8));
}

void Bat::draw(sf::RenderWindow& window) const {
    // window.draw(hitbox);
    // window.draw(detectionBox);
    
    if (alertCooldownTimer > 0) {
        alert.draw(window);
    }
    shadow.draw(window);
    
    animationManager.draw(window);
}

sf::Vector2f Bat::getPosition() const {
    return position;
}