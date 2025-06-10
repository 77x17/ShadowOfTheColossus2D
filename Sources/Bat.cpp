#include "Bat.hpp"

Bat::Bat(float x = 0, float y = 0) : Enemy(x, y, TILE_SIZE, TILE_SIZE) {
    animationManager.addAnimation(static_cast<int>(BatState::IDLE_LEFT) , TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, true );
    animationManager.addAnimation(static_cast<int>(BatState::IDLE_RIGHT), TextureManager::get("batSprite"), 16, 16, 2, 0, 0.5f, false);
    animationManager.addAnimation(static_cast<int>(BatState::DYING)     , TextureManager::get("batDead")  , 16, 16, 1, 0, 0.5f, false);
    
    shadow = Animation(TextureManager::get("batShadow"), 12,  5, 1, 0, 0.f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.f, false);
}

void Bat::updateAnimation() {
    if (state == static_cast<int>(BatState::DYING) || state == static_cast<int>(BatState::DEAD)) {
        if (dyingCooldownTimer > 0) {
            animationManager.setState((int)BatState::DYING);
            animationManager.setPosition(position);
            animationManager.update();
        }

        return;
    }

    if (movingDirection.x < 0) {
        state = static_cast<int>(BatState::IDLE_LEFT);
    }
    else if (movingDirection.x > 0) {
        state = static_cast<int>(BatState::IDLE_RIGHT);
    }

    animationManager.setState(state, true);
    animationManager.setPosition(position);
    animationManager.update();

    alert .setPosition(position + sf::Vector2f(8, -15));
    shadow.setPosition(position + sf::Vector2f(4, size.y - 8));
}