#include "Bat.hpp"

#include "Constants.hpp"
#include "TextureManager.hpp"

Bat::Bat(const sf::Vector2f& position, const std::vector<std::pair<float, std::shared_ptr<ItemData>>>& _inventory) 
: Enemy(position, sf::Vector2f(TILE_SIZE, TILE_SIZE), 5.0f, "Bat Lv.1", _inventory) {
    DETECION_RANGE = 150.0f;

    detectionBox.setRadius(DETECION_RANGE);
    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );

    animationManager.addAnimation(static_cast<int>(BatState::IDLE_LEFT) , TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, true );
    animationManager.addAnimation(static_cast<int>(BatState::IDLE_RIGHT), TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, false);
    animationManager.addAnimation(static_cast<int>(BatState::DYING)     , TextureManager::get("batDead")  , 16, 16, 1, 0, 0.5f, false);
    
    shadow = Animation(TextureManager::get("batShadow"), 12,  5, 1, 0, 0.0f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.0f, false);

    ATTACK_RANGE = 60.0f;
}

void Bat::followPlayer(const Player& player) {
    Enemy::followPlayer(player);
    
    if (calculateDistance(player) <= ATTACK_RANGE) {    
        movingDirection   *= 1.5f;
    }
}

void Bat::updateTimer(const float& dt) {
    Enemy::updateTimer(dt);
}

void Bat::updateAnimation() {
    if (!isAlive() || knockbackCooldownTimer > 0) {
        // nothing
    }
    else if (movingDirection.x < 0) {
        state = static_cast<int>(BatState::IDLE_LEFT);
    }
    else if (movingDirection.x > 0) {
        state = static_cast<int>(BatState::IDLE_RIGHT);
    }

    animationManager.setState(state, true);
    animationManager.setPosition(hitbox.getPosition());
    animationManager.update();

    alert .setPosition(hitbox.getPosition() + sf::Vector2f(8, -15));
    shadow.setPosition(hitbox.getPosition() + sf::Vector2f(4, hitbox.getSize().y - 8));
}