#include "Player.hpp"

#include <cmath>

Player::Player(float x = 0, float y = 0) {
    state      = 0;
    MOVE_SPEED = 200.0f; 
    size       = sf::Vector2f(TILE_SIZE, TILE_SIZE);
    basePosition    = sf::Vector2f(x, y);
    position        = basePosition;
    movingDirection = sf::Vector2f(0.f, 0.f);

    DYING_TIME         = 1.0f;
    dyingCooldownTimer = 0.0f;
    
    RESPAWN_TIME         = 2.0f;
    respawnCooldownTimer = 0.0f;

    VIEW_LEAP_SPEED = 2.0f;

    dashDirection     = sf::Vector2f(0.f, 0.f); 
    isDashing         = false;                    
    DASH_SPEED        = MOVE_SPEED * 2;          
    DASH_DURATION     = 0.4f;                     
    dashTimer         = 0.0f;                    
    DASH_COOLDOWN     = DASH_DURATION + 0.2f;     
    dashCooldownTimer = 0.0f;                     

    hitbox.setSize(size);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setPosition(x, y);

    loadingBox.setRadius(LOADING_DISTANCE);
    loadingBox.setOutlineColor(sf::Color::Blue);
    loadingBox.setOutlineThickness(1.f);
    loadingBox.setFillColor(sf::Color::Transparent);
    loadingBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - LOADING_DISTANCE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - LOADING_DISTANCE
    );

    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_LEFT)    , TextureManager::get("playerSprite"), 19, 21, 2,  7, 0.5f , true );
    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_RIGHT)   , TextureManager::get("playerSprite"), 19, 21, 2,  7, 0.5f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_UP_LEFT) , TextureManager::get("playerSprite"), 19, 21, 2,  8, 0.5f , true);
    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_UP_RIGHT), TextureManager::get("playerSprite"), 19, 21, 2,  8, 0.5f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_DOWN)    , TextureManager::get("playerSprite"), 19, 21, 2,  7, 0.5f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_LEFT)    , TextureManager::get("playerSprite"), 19, 21, 4,  0, 0.2f , true );
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_RIGHT)   , TextureManager::get("playerSprite"), 19, 21, 4,  0, 0.2f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_UP_LEFT) , TextureManager::get("playerSprite"), 19, 21, 3,  1, 0.2f , true );
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_UP_RIGHT), TextureManager::get("playerSprite"), 19, 21, 3,  1, 0.2f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_DOWN)    , TextureManager::get("playerSprite"), 19, 21, 3,  0, 0.2f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::DASH_LEFT)    , TextureManager::get("playerSprite"), 19, 21, 4,  3, 0.09f, true );
    animationManager.addAnimation(static_cast<int>(PlayerState::DASH_RIGHT)   , TextureManager::get("playerSprite"), 19, 21, 4,  3, 0.09f, false);
    animationManager.addAnimation(static_cast<int>(PlayerState::DYING)        , TextureManager::get("playerSprite"), 19, 21, 4,  9, 0.2f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::DEAD)         , TextureManager::get("playerSprite"), 19, 21, 4, 10, 0.5f , false);

    shadow = Animation(TextureManager::get("playerShadow"), 13, 5, 1, 0, 0.f, false);

    projectiles         = std::vector<Projectile>();
    SHOOT_COOLDOWN      = 0.2f;
    PROJECTILE_SPEED    = MOVE_SPEED * 2;
    PROJECTILE_LIFETIME = 1.0f;
    shootCooldownTimer  = 0.0f;
}

void Player::handleInput(const sf::RenderWindow& window) {
    movingDirection = sf::Vector2f(0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movingDirection.x = -1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movingDirection.x =  1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movingDirection.y =  1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movingDirection.y = -1.f;
    }

    movingDirection = Projectile::normalize(movingDirection);
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && dashCooldownTimer <= 0.f && !isDashing) {
        isDashing         = true;
        dashTimer         = DASH_DURATION;      // Đặt thời gian lướt
        dashCooldownTimer = DASH_COOLDOWN; // Đặt thời gian hồi chiêu

        // Xác định hướng lướt: dựa trên hướng di chuyển hiện tại.
        // Nếu đứng yên, lướt theo hướng đang nhìn.
        if (movingDirection.x != 0 || movingDirection.y != 0) {
            dashDirection = movingDirection;
        } 
        else if (state == static_cast<int>(PlayerState::IDLE_LEFT)     || state == static_cast<int>(PlayerState::WALK_LEFT)) {
            dashDirection = sf::Vector2f(-1.f,  0.f);
        }
        else if (state == static_cast<int>(PlayerState::IDLE_RIGHT)    || state == static_cast<int>(PlayerState::WALK_RIGHT)) {
            dashDirection = sf::Vector2f( 1.f,  0.f);
        }
        else if (state == static_cast<int>(PlayerState::IDLE_UP_LEFT)  || state == static_cast<int>(PlayerState::WALK_UP_LEFT)) {
            dashDirection = sf::Vector2f( 0.f, -1.f);
        }
        else if (state == static_cast<int>(PlayerState::IDLE_UP_RIGHT) || state == static_cast<int>(PlayerState::WALK_UP_RIGHT)) {
            dashDirection = sf::Vector2f( 0.f, -1.f);
        }
        else if (state == static_cast<int>(PlayerState::IDLE_DOWN)     || state == static_cast<int>(PlayerState::WALK_DOWN)) {
            dashDirection = sf::Vector2f( 0.f,  1.f);
        }

        SoundManager::playSound("roll");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootCooldownTimer <= 0.f && !isDashing) {
        shootCooldownTimer = SHOOT_COOLDOWN; 
        
        sf::Vector2f projectileDirection;
        if (movingDirection.x != 0 || movingDirection.y != 0) {
            projectileDirection = movingDirection;
        }
        else if (state == static_cast<int>(PlayerState::IDLE_LEFT)     || state == static_cast<int>(PlayerState::WALK_LEFT)) {
            projectileDirection = sf::Vector2f(-1.f,  0.f);
        }
        else if (state == static_cast<int>(PlayerState::IDLE_RIGHT)    || state == static_cast<int>(PlayerState::WALK_RIGHT)) {
            projectileDirection = sf::Vector2f( 1.f,  0.f);
        }
        else if (state == static_cast<int>(PlayerState::IDLE_UP_LEFT)  || state == static_cast<int>(PlayerState::WALK_UP_LEFT)) {
            projectileDirection = sf::Vector2f( 0.f, -1.f);
        }
        else if (state == static_cast<int>(PlayerState::IDLE_UP_RIGHT) || state == static_cast<int>(PlayerState::WALK_UP_RIGHT)) {
            projectileDirection = sf::Vector2f( 0.f, -1.f);
        }
        else if (state == static_cast<int>(PlayerState::IDLE_DOWN)     || state == static_cast<int>(PlayerState::WALK_DOWN)) {
            projectileDirection = sf::Vector2f( 0.f,  1.f);
        }

        projectiles.emplace_back(
            TextureManager::get("arrow"),
            position + size / 2.f,
            projectileDirection,
            PROJECTILE_SPEED,
            PROJECTILE_LIFETIME
        );

        SoundManager::playSound("arrow");
    }
}

bool Player::isCollisionProjectiles(const sf::FloatRect& rect) {
    for (Projectile& p : projectiles) {
        if (p.isCollision(rect)) {
            p.kill();
            return true;
        }
    }

    return false;
}

bool Player::isCollision(const sf::FloatRect& rect) const {
    return rect.intersects(hitbox.getGlobalBounds());
}

bool Player::isAlive() const {
    return state != static_cast<int>(PlayerState::DYING) && state != static_cast<int>(PlayerState::DEAD);
}

void Player::kill() {
    if (isAlive()) {
        state              = static_cast<int>(PlayerState::DYING);
        dyingCooldownTimer = DYING_TIME;

        SoundManager::playSound("playerHurt");
        
        projectiles.clear();
    }
}

void Player::respawn() {
    if (state == static_cast<int>(PlayerState::DEAD) && respawnCooldownTimer <= 0) {
        position        = basePosition;
        movingDirection = sf::Vector2f(0.f, 0.f);

        hitbox.setPosition(position);
        loadingBox.setPosition(
            hitbox.getPosition().x + hitbox.getSize().x / 2.f,
            hitbox.getPosition().y + hitbox.getSize().y / 2.f
        );
        
        state = 0;
    }
}

sf::FloatRect Player::getHitBox() const {
    return hitbox.getGlobalBounds();
}

void Player::updateTimer(const float &dt) {
    if (dyingCooldownTimer > 0) {
        dyingCooldownTimer -= dt;
    }
    else {
        if (state == static_cast<int>(PlayerState::DYING)) {            
            state                = static_cast<int>(PlayerState::DEAD); 
            respawnCooldownTimer = RESPAWN_TIME;
            
            return;
        }
    }
    if (respawnCooldownTimer > 0) {
        respawnCooldownTimer -= dt;
    }

    if (dashTimer > 0) {
        dashTimer -= dt;
    }
    else {
        isDashing = false;
    }
    if (dashCooldownTimer > 0) {
        dashCooldownTimer -= dt;
    }
    if (shootCooldownTimer > 0) { 
        shootCooldownTimer -= dt;
    }
}

void Player::updatePosition(const float& dt, const std::vector<sf::FloatRect>& collisionRects) {
    sf::Vector2f velocity(0.f, 0.f);
    if (isDashing) {
        // Di chuyển theo hướng dash với tốc độ dash
        velocity.x = dashDirection.x * DASH_SPEED * dt;
        velocity.y = dashDirection.y * DASH_SPEED * dt;
    } else {
        // Di chuyển bình thường nếu không dash
        velocity.x = movingDirection.x * MOVE_SPEED * dt;
        velocity.y = movingDirection.y * MOVE_SPEED * dt;
    }

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

void Player::updateHitbox() {
    hitbox.setPosition(position);
    loadingBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - LOADING_DISTANCE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - LOADING_DISTANCE
    );
}

void Player::updateAnimation() {
    if (!isAlive()) {
        // nothing
    }
    else if (isDashing) {
        state = (movingDirection.x < 0 ? static_cast<int>(PlayerState::DASH_LEFT) : static_cast<int>(PlayerState::DASH_RIGHT));
    }
    else if (movingDirection == sf::Vector2f(0, 0)) {
        if (state == static_cast<int>(PlayerState::WALK_LEFT)) {
            state = static_cast<int>(PlayerState::IDLE_LEFT);
        }
        else if (state == static_cast<int>(PlayerState::WALK_RIGHT)) {
            state = static_cast<int>(PlayerState::IDLE_RIGHT);
        }
        else if (state == static_cast<int>(PlayerState::WALK_UP_LEFT)) {
            state = static_cast<int>(PlayerState::IDLE_UP_LEFT);
        }
        else if (state == static_cast<int>(PlayerState::WALK_UP_RIGHT)) {
            state = static_cast<int>(PlayerState::IDLE_UP_RIGHT);
        }
        else if (state == static_cast<int>(PlayerState::WALK_DOWN)) {
            state = static_cast<int>(PlayerState::IDLE_DOWN);
        }
        else if (state == static_cast<int>(PlayerState::DASH_LEFT)) {
            state = static_cast<int>(PlayerState::IDLE_LEFT);
        }
        else if (state == static_cast<int>(PlayerState::DASH_RIGHT)) {
            state = static_cast<int>(PlayerState::IDLE_RIGHT);
        }
    }
    else if (movingDirection.y < 0) {
        state = (movingDirection.x > 0 ? static_cast<int>(PlayerState::WALK_UP_RIGHT) : static_cast<int>(PlayerState::WALK_UP_LEFT));
    }
    else if (movingDirection.x != 0) {
        state = (movingDirection.x > 0 ? static_cast<int>(PlayerState::WALK_RIGHT) : static_cast<int>(PlayerState::WALK_LEFT));
    }
    else if (movingDirection.y > 0) {
        state = static_cast<int>(PlayerState::WALK_DOWN);
    }

    animationManager.setState(state);
    animationManager.setPosition(position - sf::Vector2f(3, 6));
    animationManager.update();

    shadow.setPosition(position + sf::Vector2f(3, size.y - 5));
}

void Player::updateProjectiles(const float& dt) {
    for (auto& p : projectiles) {
        p.update(dt);
    }

    for (auto it = projectiles.begin(); it != projectiles.end(); /* no increment here */) {
        if (!it->isAlive()) {
            // Erase the element and update the iterator to the next one
            it = projectiles.erase(it);
        } else {
            // The element is alive, so just move to the next one
            ++it;
        }
    }
}

void Player::update(const float& dt, const sf::RenderWindow& window, const std::vector<sf::FloatRect>& collisionRects) {
    updateTimer(dt);

    if (!isAlive()) {
        updateHitbox();

        updateAnimation();
        
        respawn();

        return;
    }
    
    handleInput(window);

    updatePosition(dt, collisionRects);

    updateHitbox();

    updateAnimation();

    updateProjectiles(dt);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(hitbox);
    // window.draw(loadingBox);

    shadow.draw(window);
    
    animationManager.draw(window);

    for (auto& p : projectiles) {
        p.draw(window);
    }
}

sf::Vector2f Player::getPosition() const {
    return position;
}

void Player::updateView(const float& dt, sf::View& view) const {
    sf::Vector2f currentCenter = view.getCenter();
    sf::Vector2f targetCenter  = position;
    sf::Vector2f lerped        = currentCenter + VIEW_LEAP_SPEED * (targetCenter - currentCenter) * dt;
    lerped.x = std::round(lerped.x);
    lerped.y = std::round(lerped.y);
    view.setCenter(lerped);
}