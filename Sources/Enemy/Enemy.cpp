#include "Enemy.hpp"
#include "ShaderManager.hpp"

#include <cmath>
#include <random>

Enemy::Enemy(const float& x = 0, const float& y = 0, const float& width = TILE_SIZE, const float& height = TILE_SIZE, const float& hp = 0, const std::string& nameAndLevel = "") {
    state           = 0;
    MOVE_SPEED      = 100.0f; 
    size            = sf::Vector2f(width, height);
    basePosition    = sf::Vector2f(x, y);
    position        = basePosition;
    movingDirection = sf::Vector2f(0.f, 0.f);

    {
        TEXT_SIZE          = 10;
        BACKGROUND_PADDING = 5.0f;
        label.setFont(Font::font);
        label.setString(nameAndLevel);
        label.setCharacterSize(TEXT_SIZE);
        label.setFillColor(sf::Color::White);
        label.setPosition(position + sf::Vector2f(size.x / 2, 0));

        labelBackground.setFillColor(sf::Color(60, 60, 60, 139));
        labelBackground.setPosition(label.getPosition());
        
        // Adding BACKGROUND_PADDING two side | + BACKGROUND_PADDING / 2
        sf::FloatRect bounds = label.getLocalBounds();
        labelBackground.setSize(bounds.getSize() + sf::Vector2f(BACKGROUND_PADDING, BACKGROUND_PADDING));
        // Adding extra space with sprites
        label.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2 + 10.0f);
        // -2.0f for the font spacing
        labelBackground.setOrigin(label.getOrigin() + sf::Vector2f(BACKGROUND_PADDING / 2, -2.0f + BACKGROUND_PADDING / 2));
    }

    {
        HEALTH_POINTS_BAR_WIDTH  = 30.0f;
        HEALTH_POINTS_BAR_HEIGHT = 5.0f;
        maxHealthPoints          = hp;
        healthPoints             = hp;
        healthPointsBar.setFillColor(sf::Color::Red);
        healthPointsBar.setPosition(position + sf::Vector2f(size.x / 2, 0));
        healthPointsBar.setSize(sf::Vector2f(HEALTH_POINTS_BAR_WIDTH, HEALTH_POINTS_BAR_HEIGHT));
        
        healthPointsBarBackground.setFillColor(sf::Color(0, 0, 0, 139));
        healthPointsBarBackground.setPosition(healthPointsBar.getPosition());
        healthPointsBarBackground.setOutlineThickness(1.0f);
        healthPointsBarBackground.setOutlineColor(sf::Color::Black);

        sf::FloatRect bounds = healthPointsBar.getLocalBounds();
        healthPointsBarBackground.setSize(bounds.getSize() + sf::Vector2f(2.0f, 2.0f));
        healthPointsBar.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        healthPointsBarBackground.setOrigin(healthPointsBar.getOrigin() + sf::Vector2f(1.0f, 1.0f));
    }

    DYING_TIME         = 1.0f;
    dyingCooldownTimer = 0.0f;

    RESPAWN_TIME         = 5.0f;
    respawnCooldownTimer = 0.0f;

    INVINCIBLE_TIME         = 1.0f;
    invincibleCooldownTimer = INVINCIBLE_TIME;

    DETECION_RANGE     = 150.0f;
    ALERT_LIFETIME     = 0.5f;
    alertCooldownTimer = 0.0f;

    RANDOM_TIME          = 5.0f;
    randomCooldownTimer  = 0.0f;
    stayingCooldownTimer = 0.0f;
    
    ATTACK_COOLDOWN_TIME = 0.5f;
    attackCooldownTimer  = 0.0f;

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

    KNOCKBACK_STRENGTH     = 200.0f;
    KNOCKBACK_COOLDOWN     = 0.2f;
    knockbackCooldownTimer = 0.0f;
}

bool Enemy::isAlive() const {
    return state != -1 && state != -2;     // DEAD
}

void Enemy::attack(Player& player) {
    if (attackCooldownTimer <= 0) {
        player.hurt(1.0f);
        player.knockback(position);

        attackCooldownTimer = ATTACK_COOLDOWN_TIME;
    }
}

void Enemy::hurt(const float& damage) {
    if (isAlive() && invincibleCooldownTimer <= 0) {
        healthPoints -= damage;

        SoundManager::playSound("enemyHurt");

        if (healthPoints <= 0) {
            kill();
        }
    }
}

void Enemy::knockback(const sf::Vector2f& playerPosition) {
    if (isAlive() && invincibleCooldownTimer <= 0) {
        movingDirection = (position - playerPosition);

        movingDirection = Projectile::normalize(movingDirection);

        knockbackCooldownTimer = KNOCKBACK_COOLDOWN;
    }
}

void Enemy::kill() {
    healthPoints       = 0;

    state              = -2;
    dyingCooldownTimer = DYING_TIME;
    
    SoundManager::playSound("enemyDie");
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

        healthPoints = maxHealthPoints;

        invincibleCooldownTimer = INVINCIBLE_TIME;
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
    if (invincibleCooldownTimer > 0) {
        invincibleCooldownTimer -= dt;
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
    if (attackCooldownTimer > 0) {
        attackCooldownTimer -= dt;
    }
    if (knockbackCooldownTimer > 0) {
        knockbackCooldownTimer -= dt;
    }
}

void Enemy::followPlayer(const Player& player) {
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
    if (knockbackCooldownTimer <= 0) {
        movingDirection = sf::Vector2f(0, 0);
    }
    else {
        // fix here
        return;
    }

    if (calculateDistance(player) <= DETECION_RANGE) {
        if (invincibleCooldownTimer <= 0) {
            if (player.isCollision(hitbox.getGlobalBounds())) {
                attack(player);
            }
        }

        if (player.isAlive()) {
            if (attackCooldownTimer <= 0) {
                followPlayer(player);
            }
            else {
                movingDirection = sf::Vector2f(0.0f, 0.0f);
            }
        }
        else {
            moveRandomly();
        }

        detectionBox.setOutlineColor(sf::Color::Yellow);
    }
    else {
        moveRandomly();

        detectionBox.setOutlineColor(sf::Color::Blue);
    }
}

void Enemy::updatePosition(const float& dt, const std::vector<sf::FloatRect>& collisionRects) {
    sf::Vector2f velocity;

    if (knockbackCooldownTimer > 0) {
        velocity = movingDirection * KNOCKBACK_STRENGTH * dt;
    }
    else {
        velocity = movingDirection * MOVE_SPEED * dt;
    }

    sf::FloatRect nextHitboxRect = sf::FloatRect(position, size);
    if (velocity.x != 0) {
        nextHitboxRect.left += velocity.x;
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextHitboxRect)) {
                nextHitboxRect.left -= velocity.x / 10;
            }
        }
    }
    if (velocity.y != 0) {
        nextHitboxRect.top += velocity.y;
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextHitboxRect)) {
                nextHitboxRect.top -= velocity.y / 10;
            }
        }
    }

    position = nextHitboxRect.getPosition();
}

void Enemy::updateHitbox() {
    hitbox.setPosition(position);

    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2 - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2 - DETECION_RANGE
    );

    label.setPosition(position + sf::Vector2f(size.x / 2, 0));
    labelBackground.setPosition(label.getPosition());

    healthPointsBar.setPosition(position + sf::Vector2f(size.x / 2, 0));
    healthPointsBarBackground.setPosition(healthPointsBar.getPosition());

    healthPointsBar.setSize(sf::Vector2f(HEALTH_POINTS_BAR_WIDTH * (healthPoints / maxHealthPoints), HEALTH_POINTS_BAR_HEIGHT)); 
}

void Enemy::update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects) {
    updateTimer(dt);

    if (!isAlive()) {
        if (state == -2) {
            updateHitbox();

            updateAnimation();
        }
        else if (state == -1) {
            respawn();
        }
        return;
    }

    if (player.isCollisionProjectiles(hitbox.getGlobalBounds())) {
        hurt(1.0f);
        knockback(player.getPosition());

        if (!isAlive()) {
            player.addVictim(label.getString());
        }
    }

    updateThinking(player);

    updatePosition(dt, collisionRects);

    updateHitbox();

    updateAnimation();
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(labelBackground);
    window.draw(label);

    window.draw(healthPointsBarBackground);
    window.draw(healthPointsBar);

    // window.draw(hitbox);
    // window.draw(detectionBox);
    
    shadow.draw(window);

    if (invincibleCooldownTimer > 0) {
        animationManager.draw(window, ShaderManager::get("invincible"));
    }
    else if (knockbackCooldownTimer > 0) {
        animationManager.draw(window, ShaderManager::get("flash"));   
    }
    else {
        animationManager.draw(window);
    }

    if (alertCooldownTimer > 0) {
        alert.draw(window);
    }
}