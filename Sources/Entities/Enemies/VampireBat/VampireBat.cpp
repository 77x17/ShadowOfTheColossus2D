#include "VampireBat.hpp"

#include "Constants.hpp"
#include "TextureManager.hpp"

VampireBat::VampireBat(const sf::Vector2f& position, 
                       const std::string&  nameAndLevel,
                       const float&        hp, 
                       const float&        damage,
                       const float&        exp,
                       const std::vector<std::pair<float, std::shared_ptr<ItemData>>>& _inventory)
: Enemy(position, sf::Vector2f(TILE_SIZE, TILE_SIZE), nameAndLevel, hp, damage, exp, _inventory) {
    DETECION_RANGE = 150.0f;

    detectionBox.setRadius(DETECION_RANGE);
    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );

    animationManager.addAnimation(static_cast<int>(VampireBatState::IDLE_LEFT) , TextureManager::get("vampireBat"), 16, 16, 4, 0, 0.25f, true );
    animationManager.addAnimation(static_cast<int>(VampireBatState::IDLE_RIGHT), TextureManager::get("vampireBat"), 16, 16, 4, 0, 0.25f, false);
    animationManager.addAnimation(static_cast<int>(VampireBatState::DYING)     , TextureManager::get("batDead")   , 16, 16, 1, 0, 0.5f, false);
    
    shadow = Animation(TextureManager::get("batShadow"), 12,  5, 1, 0, 0.0f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.0f, false);

    ATTACK_RANGE = 60.0f;
}

void VampireBat::followPlayer(const Player& player) {
    Enemy::followPlayer(player);
    
    if (calculateDistance(player) <= ATTACK_RANGE) {    
        movingDirection   *= 1.5f;
    }
}

void VampireBat::updateTimer(const float& dt) {
    Enemy::updateTimer(dt);
}

void VampireBat::updateAnimation() {
    if (!isAlive() || knockbackCooldownTimer > 0) {
        // nothing
    }
    else if (movingDirection.x < 0) {
        state = static_cast<int>(VampireBatState::IDLE_LEFT);
    }
    else if (movingDirection.x > 0) {
        state = static_cast<int>(VampireBatState::IDLE_RIGHT);
    }

    animationManager.setState(state, true);
    animationManager.setPosition(hitbox.getPosition());
    animationManager.update();

    alert .setPosition(hitbox.getPosition() + sf::Vector2f(7.0f, -20.0f));
    shadow.setPosition(hitbox.getPosition() + sf::Vector2f(5.0f, hitbox.getSize().y - 5.0f));
}