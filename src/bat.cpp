#include "bat.hpp"

#include <cmath>

Bat::Bat(float x = 0, float y = 0) : position(x, y), movingDirection(0, 0) {
    shadow = Animation(TextureManager::get("batShadow"), 12,  5, 1, 0, 0.f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.f, false);

    shape.setSize(size);
    shape.setFillColor(sf::Color::Cyan);
    shape.setPosition(x, y);

    animationManager.addAnimation((int)BatState::IDLE_LEFT , TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, true );
    animationManager.addAnimation((int)BatState::IDLE_RIGHT, TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, false);
}

void Bat::update(const Player& player) {
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

    state = (movingDirection.x < 0 ? (int)BatState::IDLE_LEFT : (int)BatState::IDLE_RIGHT);

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