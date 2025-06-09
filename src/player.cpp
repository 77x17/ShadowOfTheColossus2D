#include "player.hpp"

Player::Player(float x = 0, float y = 0) : position(x, y) {
    shadow = Animation(TextureManager::get("playerShadow"), 13, 5, 1, 0, 0.f, false);

    shape.setSize(size);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    shape.setFillColor(sf::Color::Transparent);
    shape.setPosition(x, y);

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
        
        sf::Vector2f playerCenter = getPosition();
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
            playerCenter + size / 2.f,
            projectileDirection,
            PROJECTILE_SPEED,
            PROJECTILE_LIFETIME
        );
    }
}

bool Player::isCollisionProjectiles(const sf::FloatRect& rect) const {
    for (auto& p : projectiles) {
        if (p.isCollision(rect)) {
            return true;
        }
    }

    return false;
}

void Player::update(sf::View& view) {
    float dt = deltaClock.restart().asSeconds();
    if (dashCooldownTimer > 0) {
        dashCooldownTimer -= dt;
    }
    if (shootCooldownTimer > 0) { 
        shootCooldownTimer -= dt;
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
    
    if (isDashing) {
        // Di chuyển theo hướng dash với tốc độ dash
        position += dashDirection * DASH_SPEED;

        // Giảm bộ đếm thời gian dash
        dashTimer -= dt;

        // Nếu hết thời gian dash, dừng lại
        if (dashTimer <= 0.f) {
            isDashing = false;
        }    
    } else {
        // Di chuyển bình thường nếu không dash
        position += movingDirection * MOVE_SPEED;
    }

    shape.setPosition(position);

    if (isDashing) {
        state = (movingDirection.x < 0 ? (int)PlayerState::DASH_LEFT : (int)PlayerState::DASH_RIGHT);
    }
    else if (movingDirection == sf::Vector2f(0, 0)) {
        if (state == (int)PlayerState::WALK_RIGHT) {
            state = (int)PlayerState::IDLE_RIGHT;
        }
        else if (state == (int)PlayerState::WALK_LEFT) {
            state = (int)PlayerState::IDLE_LEFT;
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
    window.draw(shape);
    
    shadow.draw(window);
    
    animationManager.draw(window);

    for (auto& p : projectiles) {
        p.draw(window);
    }
}

sf::Vector2f Player::getPosition() const {
    return position;
}