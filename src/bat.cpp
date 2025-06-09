#include "bat.hpp"

#include <cmath>

Bat::Bat(float x = 0, float y = 0) : basePosition(x, y) {
    position = basePosition;
    shadow = Animation(TextureManager::get("batShadow"), 12,  5, 1, 0, 0.f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.f, false);

    shape.setSize(size);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    shape.setFillColor(sf::Color::Transparent);
    shape.setPosition(x, y);

    animationManager.addAnimation((int)BatState::IDLE_LEFT , TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, true );
    animationManager.addAnimation((int)BatState::IDLE_RIGHT, TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, false);
    animationManager.addAnimation((int)BatState::DYING, TextureManager::get("batDead"), 16, 16, 2, 0, 0.5f, false);
}

bool Bat::isAlive() const {
    return lifeState != BatState::DEAD;
}

void Bat::respawn() {
    position        = basePosition;
    movingDirection = sf::Vector2f(0.f, 0.f);
    lifeState       = BatState::ALIVE;
}

void Bat::update(Player& player) {
    if (lifeState == BatState::DYING || lifeState == BatState::DEAD) {
        float dt = deltaClock.restart().asSeconds();
        if (dyingCooldownTimer > 0) {
            dyingCooldownTimer -= dt;
            
            animationManager.setState((int)BatState::DYING);
            animationManager.setPosition(position);
            animationManager.update();
        }
        else {
            lifeState = BatState::DEAD;
        }

        return;
    }

    if (player.isCollisionProjectiles(shape.getGlobalBounds())) {
        lifeState = BatState::DYING;
        dyingCooldownTimer = DYING_TIME;
        
        return;
    }

    // if (player.isCollision(shape.getGlobalBounds())) {
    //     player.kill();
    // }

    float dt = deltaClock.restart().asSeconds();
    if (alertCooldownTimer > 0) {
        alertCooldownTimer -= dt;
    }

    sf::Vector2f normalizeDirection = player.getPosition() - position;
    float length = std::sqrt(normalizeDirection.x * normalizeDirection.x + normalizeDirection.y * normalizeDirection.y);
    
    if (length <= DETECION_RANGE) {
        if (length != 0)
            normalizeDirection /= length;
    
        if (movingDirection == sf::Vector2f(0.f, 0.f)) {
            alertCooldownTimer = ALERT_LIFETIME;
        }
        movingDirection = normalizeDirection;
    }
    else {
        movingDirection = sf::Vector2f(0.f, 0.f);
    }
    
    position += movingDirection * MOVE_SPEED;

    shape.setPosition(position);

    if (movingDirection.x < 0) {
        state = (int)BatState::IDLE_LEFT;
    }
    else if (movingDirection.x > 0) {
        state = (int)BatState::IDLE_RIGHT;
    }

    animationManager.setState(state);
    animationManager.setPosition(position);
    animationManager.update();

    alert .setPosition(position + sf::Vector2f(8, -15));
    shadow.setPosition(position + sf::Vector2f(4, size.y - 8));
}

void Bat::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    
    if (alertCooldownTimer > 0) {
        alert.draw(window);
    }
    shadow.draw(window);
    
    animationManager.draw(window);
}

sf::Vector2f Bat::getPosition() const {
    return position;
}