#include "Player.hpp"

#include <cmath>

Player::Player(float x = 0, float y = 0) : basePosition(x, y) {
    position = basePosition;

    shadow = Animation(TextureManager::get("playerShadow"), 13, 5, 1, 0, 0.f, false);

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

    animationManager.addAnimation((int)PlayerState::IDLE_LEFT    , TextureManager::get("playerSprite"), 19, 21, 2, 7, 0.5f , true );
    animationManager.addAnimation((int)PlayerState::IDLE_RIGHT   , TextureManager::get("playerSprite"), 19, 21, 2, 7, 0.5f , false);
    animationManager.addAnimation((int)PlayerState::IDLE_UP_LEFT , TextureManager::get("playerSprite"), 19, 21, 2, 8, 0.5f , true);
    animationManager.addAnimation((int)PlayerState::IDLE_UP_RIGHT, TextureManager::get("playerSprite"), 19, 21, 2, 8, 0.5f , false);
    animationManager.addAnimation((int)PlayerState::IDLE_DOWN    , TextureManager::get("playerSprite"), 19, 21, 2, 7, 0.5f , false);
    animationManager.addAnimation((int)PlayerState::WALK_LEFT    , TextureManager::get("playerSprite"), 19, 21, 4, 0, 0.2f , true );
    animationManager.addAnimation((int)PlayerState::WALK_RIGHT   , TextureManager::get("playerSprite"), 19, 21, 4, 0, 0.2f , false);
    animationManager.addAnimation((int)PlayerState::WALK_UP_LEFT , TextureManager::get("playerSprite"), 19, 21, 3, 1, 0.2f , true );
    animationManager.addAnimation((int)PlayerState::WALK_UP_RIGHT, TextureManager::get("playerSprite"), 19, 21, 3, 1, 0.2f , false);
    animationManager.addAnimation((int)PlayerState::WALK_DOWN    , TextureManager::get("playerSprite"), 19, 21, 3, 0, 0.2f , false);
    animationManager.addAnimation((int)PlayerState::DASH_LEFT    , TextureManager::get("playerSprite"), 19, 21, 4, 3, 0.09f, true );
    animationManager.addAnimation((int)PlayerState::DASH_RIGHT   , TextureManager::get("playerSprite"), 19, 21, 4, 3, 0.09f, false);
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
        else if (state == (int)PlayerState::IDLE_LEFT     || state == (int)PlayerState::WALK_LEFT) {
            dashDirection = sf::Vector2f(-1.f,  0.f);
        }
        else if (state == (int)PlayerState::IDLE_RIGHT    || state == (int)PlayerState::WALK_RIGHT) {
            dashDirection = sf::Vector2f( 1.f,  0.f);
        }
        else if (state == (int)PlayerState::IDLE_UP_LEFT  || state == (int)PlayerState::WALK_UP_LEFT) {
            dashDirection = sf::Vector2f( 0.f, -1.f);
        }
        else if (state == (int)PlayerState::IDLE_UP_RIGHT || state == (int)PlayerState::WALK_UP_RIGHT) {
            dashDirection = sf::Vector2f( 0.f, -1.f);
        }
        else if (state == (int)PlayerState::IDLE_DOWN     || state == (int)PlayerState::WALK_DOWN) {
            dashDirection = sf::Vector2f( 0.f,  1.f);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootCooldownTimer <= 0.f && !isDashing) {
        shootCooldownTimer = SHOOT_COOLDOWN; 
        
        sf::Vector2f projectileDirection;
        if (movingDirection.x != 0 || movingDirection.y != 0) {
            projectileDirection = movingDirection;
        }
        else if (state == (int)PlayerState::IDLE_LEFT     || state == (int)PlayerState::WALK_LEFT) {
            projectileDirection = sf::Vector2f(-1.f,  0.f);
        }
        else if (state == (int)PlayerState::IDLE_RIGHT    || state == (int)PlayerState::WALK_RIGHT) {
            projectileDirection = sf::Vector2f( 1.f,  0.f);
        }
        else if (state == (int)PlayerState::IDLE_UP_LEFT  || state == (int)PlayerState::WALK_UP_LEFT) {
            projectileDirection = sf::Vector2f( 0.f, -1.f);
        }
        else if (state == (int)PlayerState::IDLE_UP_RIGHT || state == (int)PlayerState::WALK_UP_RIGHT) {
            projectileDirection = sf::Vector2f( 0.f, -1.f);
        }
        else if (state == (int)PlayerState::IDLE_DOWN     || state == (int)PlayerState::WALK_DOWN) {
            projectileDirection = sf::Vector2f( 0.f,  1.f);
        }

        projectiles.emplace_back(
            TextureManager::get("arrow"),
            getPosition() + size / 2.f,
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
    return lifeState != PlayerState::DEAD;
}

void Player::kill() {
    lifeState = PlayerState::DEAD;

    SoundManager::playSound("playerHurt");
}

void Player::respawn() {
    position        = basePosition;
    movingDirection = sf::Vector2f(0.f, 0.f);

    hitbox.setPosition(position);
    loadingBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f
    );
    
    lifeState = PlayerState::ALIVE;
}

sf::FloatRect Player::getHitBox() const {
    return hitbox.getGlobalBounds();
}

void Player::update(sf::View& view, const std::vector<sf::FloatRect>& collisionRects) {
    if (lifeState == PlayerState::DEAD) {
        return;
    }

    float dt = deltaClock.restart().asSeconds();
    if (dashCooldownTimer > 0) {
        dashCooldownTimer -= dt;
    }
    if (shootCooldownTimer > 0) { 
        shootCooldownTimer -= dt;
    }
    if (dashTimer > 0) {
        dashTimer -= dt;
    }
    else {
        isDashing = false;
    }

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

    sf::Vector2f velocity(0.f, 0.f);
    if (isDashing) {
        // Di chuyển theo hướng dash với tốc độ dash
        velocity.x = dashDirection.x * DASH_SPEED;
        velocity.y = dashDirection.y * DASH_SPEED;
    } else {
        // Di chuyển bình thường nếu không dash
        velocity.x = movingDirection.x * MOVE_SPEED;
        velocity.y = movingDirection.y * MOVE_SPEED;
    }

    sf::FloatRect nextPlayerRect_x = sf::FloatRect(position + velocity, size);
    sf::FloatRect nextPlayerRect_y = sf::FloatRect(position + velocity, size);

    if (velocity.x != 0.0f) {
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextPlayerRect_x)) {
                if (velocity.x > 0.0f) {
                    nextPlayerRect_x.left = rect.left - nextPlayerRect_x.width - 0.1f;
                }
                else if (velocity.x < 0.0f) {
                    nextPlayerRect_x.left = rect.left + rect.width + 0.1f;
                }
                else {
                    float a = rect.left - nextPlayerRect_x.width - 0.1f - nextPlayerRect_x.left;
                    float b = rect.left + rect.width + 0.1f - nextPlayerRect_x.left;
                    nextPlayerRect_x.left = a < b ? rect.left - nextPlayerRect_x.width - 0.1f : rect.left + rect.width + 0.1f;
                }
            }
        }
    }
    if (velocity.y != 0.0f) {
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextPlayerRect_x)) {
                if (velocity.y > 0.0f) {
                    nextPlayerRect_x.top = rect.top - nextPlayerRect_x.height - 0.1f;
                }
                else if (velocity.y < 0.0f) {
                    nextPlayerRect_x.top = rect.top + rect.height + 0.1f;
                }
                else {
                    float a = rect.top - nextPlayerRect_x.height - 0.1f - nextPlayerRect_x.top;
                    float b = rect.top + rect.height + 0.1f - nextPlayerRect_x.top;
                    nextPlayerRect_x.top = a < b ? rect.top - nextPlayerRect_x.height - 0.1f : rect.top + rect.height + 0.1f;
                }
            }
        }
    }
    if (velocity.y != 0.0f) {
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextPlayerRect_y)) {
                if (velocity.y > 0.0f) {
                    nextPlayerRect_y.top = rect.top - nextPlayerRect_y.height - 0.1f;
                }
                else if (velocity.y < 0.0f) {
                    nextPlayerRect_y.top = rect.top + rect.height + 0.1f;
                }
                else {
                    float a = rect.top - nextPlayerRect_y.height - 0.1f - nextPlayerRect_y.top;
                    float b = rect.top + rect.height + 0.1f - nextPlayerRect_y.top;
                    nextPlayerRect_y.top = a < b ? rect.top - nextPlayerRect_y.height - 0.1f : rect.top + rect.height + 0.1f;
                }
            }
        }
    }
    if (velocity.x != 0.0f) {
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextPlayerRect_y)) {
                if (velocity.x > 0.0f) {
                    nextPlayerRect_y.left = rect.left - nextPlayerRect_y.width - 0.1f;
                }
                else if (velocity.x < 0.0f) {
                    nextPlayerRect_y.left = rect.left + rect.width + 0.1f;
                }
                else {
                    float a = rect.left - nextPlayerRect_y.width - 0.1f - nextPlayerRect_y.left;
                    float b = rect.left + rect.width + 0.1f - nextPlayerRect_y.left;
                    nextPlayerRect_y.left = a < b ? rect.left - nextPlayerRect_y.width - 0.1f : rect.left + rect.width + 0.1f;
                }
            }
        }
    }
    
    float distance_x = std::abs(position.x - nextPlayerRect_x.left);
    float distance_y = std::abs(position.y - nextPlayerRect_y.top);

    if (distance_x != 0 || distance_y != 0) {
        std::cerr << distance_x << ' ' << distance_y << '\n';
    }

    position = distance_x < distance_y ? nextPlayerRect_x.getPosition() : nextPlayerRect_y.getPosition();
    position = nextPlayerRect_x.getPosition();
    hitbox.setPosition(position);
    loadingBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - LOADING_DISTANCE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - LOADING_DISTANCE
    );

    if (isDashing) {
        state = (movingDirection.x < 0 ? (int)PlayerState::DASH_LEFT : (int)PlayerState::DASH_RIGHT);
    }
    else if (movingDirection == sf::Vector2f(0, 0)) {
        if (state == (int)PlayerState::WALK_LEFT) {
            state = (int)PlayerState::IDLE_LEFT;
        }
        else if (state == (int)PlayerState::WALK_RIGHT) {
            state = (int)PlayerState::IDLE_RIGHT;
        }
        else if (state == (int)PlayerState::WALK_UP_LEFT) {
            state = (int)PlayerState::IDLE_UP_LEFT;
        }
        else if (state == (int)PlayerState::WALK_UP_RIGHT) {
            state = (int)PlayerState::IDLE_UP_RIGHT;
        }
        else if (state == (int)PlayerState::WALK_DOWN) {
            state = (int)PlayerState::IDLE_DOWN;
        }
        else if (state == (int)PlayerState::DASH_LEFT) {
            state = (int)PlayerState::IDLE_LEFT;
        }
        else if (state == (int)PlayerState::DASH_RIGHT) {
            state = (int)PlayerState::IDLE_RIGHT;
        }
    }
    else if (movingDirection.y < 0) {
        state = (movingDirection.x > 0 ? (int)PlayerState::WALK_UP_RIGHT : (int)PlayerState::WALK_UP_LEFT);
    }
    else if (movingDirection.x != 0) {
        state = (movingDirection.x > 0 ? (int)PlayerState::WALK_RIGHT : (int)PlayerState::WALK_LEFT);
    }
    else if (movingDirection.y > 0) {
        state = (int)PlayerState::WALK_DOWN;
    }

    animationManager.setState(state);
    animationManager.setPosition(position - sf::Vector2f(3, 6));
    animationManager.update();

    shadow.setPosition(position + sf::Vector2f(3, size.y - 5));

    sf::Vector2f currentCenter = view.getCenter();
    sf::Vector2f targetCenter  = position;
    sf::Vector2f lerped        = currentCenter + 0.1f * (targetCenter - currentCenter);

    view.setCenter(lerped);
}

void Player::draw(sf::RenderWindow& window) const {
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