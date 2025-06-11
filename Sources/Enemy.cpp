#include "Enemy.hpp"

#include <cmath>
#include <random>

Enemy::Enemy(const float& x = 0, const float& y = 0, const float& width = TILE_SIZE, const float& height = TILE_SIZE, const float& hp = 0) {
    state           = 0;
    healthPoints    = hp;
    MOVE_SPEED      = 125.0f; 
    size            = sf::Vector2f(width, height);
    basePosition    = sf::Vector2f(x, y);
    position        = basePosition;
    movingDirection = sf::Vector2f(0.f, 0.f);
    
    DYING_TIME         = 1.0f;
    dyingCooldownTimer = 0.0f;

    RESPAWN_TIME         = 5.0f;
    respawnCooldownTimer = 0.0f;

    DETECION_RANGE     = 150.0f;
    ALERT_LIFETIME     = 0.5f;
    alertCooldownTimer = 0.0f;

    RANDOM_TIME          = 5.0f;
    randomCooldownTimer  = 0.0f;
    stayingCooldownTimer = 0.0f;

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
}

bool Enemy::isAlive() const {
    return state != -1 && state != -2;     // DEAD
}

void Enemy::kill() {
    state              = -2;
    dyingCooldownTimer = DYING_TIME;
    
    SoundManager::playSound("enemyHurt");
}

void Enemy::respawn() {
    if (state == -1 && respawnCooldownTimer <= 0) {
        position        = basePosition;
        movingDirection = sf::Vector2f(0.f, 0.f);
        
        hitbox.setPosition(position);
        detectionBox.setPosition(
            hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
            hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
        );

        state = 0;          // ALIVE
    }
}

float Enemy::calculateDistance(const Player& player) const {
    sf::Vector2f distanceFomular = player.getPosition() - position;
    return std::sqrt(distanceFomular.x * distanceFomular.x + distanceFomular.y * distanceFomular.y);
}

void Enemy::updateTimer(const float &dt) {
    if (dyingCooldownTimer > 0) {
        dyingCooldownTimer -= dt;
    }
    else {
        if (state == -2) {                          // dying and end dyingCooldown
            state                = -1;              // dead
            respawnCooldownTimer = RESPAWN_TIME;
        }
    }
    if (respawnCooldownTimer > 0) {
        respawnCooldownTimer -= dt;
    }

    if (alertCooldownTimer > 0) {
        alertCooldownTimer -= dt;
    }
    if (randomCooldownTimer > 0) {
        randomCooldownTimer -= dt;
    }
    if (stayingCooldownTimer > 0) {
        stayingCooldownTimer -= dt;
    }
}

void Enemy::attackPlayer(const Player& player) {
    sf::Vector2f normalizeDirection = player.getPosition() - position;
    float length = std::sqrt(normalizeDirection.x * normalizeDirection.x + normalizeDirection.y * normalizeDirection.y);

    if (length != 0) {
        normalizeDirection /= length;
    }

    if (alertCooldownTimer <= 0 && randomCooldownTimer > 0) {
        alertCooldownTimer  = ALERT_LIFETIME;
        randomCooldownTimer = 0;
    }
    
    movingDirection = normalizeDirection;
}

void Enemy::moveRandomly() {
    // randomCooldownTimer được kích hoạt khi quái vật không tìm thấy người chơi trong phạm vi
    if (randomCooldownTimer > 0) {
        // stayingCooldownTimer được chỉnh để bằng 3/4 thời gian của ramdomCooldownTimer
        // mục đích để quái di chuyển 1/4 và 3/4 thời gian còn lại đứng yên tại chỗ
        if (stayingCooldownTimer <= 0) {
            movingDirection = sf::Vector2f(rand() % 3 - 1.f, rand() % 3 - 1.f);

            // normalize
            float length = std::sqrt(movingDirection.x * movingDirection.x + movingDirection.y * movingDirection.y);
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

void Enemy::updateThinking(Player& player) {
    if (calculateDistance(player) <= DETECION_RANGE) {
        if (player.isCollision(hitbox.getGlobalBounds())) {
            player.kill();

            return;
        }

        attackPlayer(player);

        detectionBox.setOutlineColor(sf::Color::Yellow);
    }
    else {
        moveRandomly();

        detectionBox.setOutlineColor(sf::Color::Blue);
    }
}

void Enemy::updatePosition(const float& dt, const std::vector<sf::FloatRect>& collisionRects) {
    sf::Vector2f velocity = movingDirection * MOVE_SPEED * dt;

    sf::FloatRect nextHitboxRect = sf::FloatRect(position, size);
    if (velocity.x != 0) {
        nextHitboxRect.left += velocity.x;
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextHitboxRect)) {
                nextHitboxRect.left -= velocity.x / 10.0f;
            }
        }
    }
    if (velocity.y != 0) {
        nextHitboxRect.top += velocity.y;
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextHitboxRect)) {
                nextHitboxRect.top -= velocity.y / 10.0f;
            }
        }
    }

    position = nextHitboxRect.getPosition();
}

void Enemy::updateHitbox() {
    hitbox.setPosition(position);

    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );
}

void Enemy::update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects) {
    updateTimer(dt);

    if (!isAlive()) {
        if (state == -2) {
            updateAnimation();
        }
        else if (state == -1) {
            respawn();
        }
        return;
    }

    if (player.isCollisionProjectiles(hitbox.getGlobalBounds())) {
        kill();

        return;
    }

    updateThinking(player);

    updatePosition(dt, collisionRects);

    updateHitbox();

    updateAnimation();
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(hitbox);
    window.draw(detectionBox);
    
    shadow.draw(window);
    animationManager.draw(window);
    if (alertCooldownTimer > 0) {
        alert.draw(window);
    }
}