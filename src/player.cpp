#include "player.hpp"

Player::Player(int _x = 0, int _y = 0) 
    : x(_x), y(_y), vx(0), vy(0), shadow(TextureManager::get("player_shadow"), 13, 5, 1, 0, 0.f, false) {
    shape.setSize(sf::Vector2f(WIDTH, HEIGHT));
    shape.setFillColor(sf::Color::Cyan);
    shape.setPosition(x, y);

    animationManager.addAnimation(PlayerState::IDLE_LEFT    , TextureManager::get("player_sprite"), 19, 21, 2, 7, 0.5f , true );
    animationManager.addAnimation(PlayerState::IDLE_RIGHT   , TextureManager::get("player_sprite"), 19, 21, 2, 7, 0.5f , false);
    animationManager.addAnimation(PlayerState::IDLE_UP_LEFT , TextureManager::get("player_sprite"), 19, 21, 2, 8, 0.5f , true);
    animationManager.addAnimation(PlayerState::IDLE_UP_RIGHT, TextureManager::get("player_sprite"), 19, 21, 2, 8, 0.5f , false);
    animationManager.addAnimation(PlayerState::IDLE_DOWN    , TextureManager::get("player_sprite"), 19, 21, 2, 7, 0.5f , false);
    animationManager.addAnimation(PlayerState::WALK_LEFT    , TextureManager::get("player_sprite"), 19, 21, 4, 0, 0.2f , true );
    animationManager.addAnimation(PlayerState::WALK_RIGHT   , TextureManager::get("player_sprite"), 19, 21, 4, 0, 0.2f , false);
    animationManager.addAnimation(PlayerState::WALK_UP_LEFT , TextureManager::get("player_sprite"), 19, 21, 3, 1, 0.2f , true );
    animationManager.addAnimation(PlayerState::WALK_UP_RIGHT, TextureManager::get("player_sprite"), 19, 21, 3, 1, 0.2f , false);
    animationManager.addAnimation(PlayerState::WALK_DOWN    , TextureManager::get("player_sprite"), 19, 21, 3, 0, 0.2f , false);
    animationManager.addAnimation(PlayerState::DASH         , TextureManager::get("player_sprite"), 19, 21, 4, 3, 0.09f, false);
}

void Player::handleInput(const sf::RenderWindow& window) {
    vx = 0.f;
    vy = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        vx = -1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        vx =  1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        vy =  1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        vy = -1.f;
    }

    sf::Vector2f normalizeVector = Projectile::normalize(sf::Vector2f(vx, vy));
    vx = normalizeVector.x;
    vy = normalizeVector.y;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && dashCooldownTimer <= 0.f && !isDashing) {
        isDashing         = true;
        dashTimer         = DASH_DURATION;      // Đặt thời gian lướt
        dashCooldownTimer = DASH_COOLDOWN; // Đặt thời gian hồi chiêu

        // Xác định hướng lướt: dựa trên hướng di chuyển hiện tại.
        // Nếu đứng yên, lướt theo hướng đang nhìn.
        if (vx != 0 || vy != 0) {
            dashDirection = sf::Vector2f(vx, vy);
        } 
        else if (state == PlayerState::IDLE_RIGHT    || state == PlayerState::WALK_RIGHT) {
            dashDirection = {1.f, 0.f};
        }
        else if (state == PlayerState::IDLE_LEFT     || state == PlayerState::WALK_LEFT) {
            dashDirection = {-1.f, 0.f};
        }
        else if (state == PlayerState::IDLE_UP_LEFT  || state == PlayerState::WALK_UP_LEFT) {
            dashDirection = {0.f, -1.f};
        }
        else if (state == PlayerState::IDLE_UP_RIGHT || state == PlayerState::WALK_UP_RIGHT) {
            dashDirection = {0.f, -1.f};
        }
        else if (state == PlayerState::IDLE_DOWN     || state == PlayerState::WALK_DOWN) {
            dashDirection = {0.f, 1.f};
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootCooldownTimer <= 0.f && !isDashing) {
        shootCooldownTimer = SHOOT_COOLDOWN; 
        
        sf::Vector2f playerCenter = getPosition();
        sf::Vector2f direction;
        if (vx != 0 || vy != 0) {
            direction = sf::Vector2f(vx, vy);
        } else if (state == PlayerState::IDLE_RIGHT  || state == PlayerState::WALK_RIGHT) {
            direction = {1.f, 0.f};
        }
        else if (state == PlayerState::IDLE_LEFT     || state == PlayerState::WALK_LEFT) {
            direction = {-1.f, 0.f};
        }
        else if (state == PlayerState::IDLE_UP_LEFT  || state == PlayerState::WALK_UP_LEFT) {
            direction = {0.f, -1.f};
        }
        else if (state == PlayerState::IDLE_UP_RIGHT || state == PlayerState::WALK_UP_RIGHT) {
            direction = {0.f, -1.f};
        }
        else if (state == PlayerState::IDLE_DOWN     || state == PlayerState::WALK_DOWN) {
            direction = {0.f, 1.f};
        }

        projectiles.emplace_back(
            TextureManager::get("arrow"),
            playerCenter,
            direction,
            PROJECTILE_SPEED,
            PROJECTILE_LIFETIME
        );
    }
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
        x += dashDirection.x * DASH_SPEED * dt * 50;
        y += dashDirection.y * DASH_SPEED * dt * 50;

        // Giảm bộ đếm thời gian dash
        dashTimer -= dt;

        // Nếu hết thời gian dash, dừng lại
        if (dashTimer <= 0.f) {
            isDashing = false;
        }    
    } else {
        // Di chuyển bình thường nếu không dash
        x += vx * MOVE_SPEED * dt * 100;
        y += vy * MOVE_SPEED * dt * 100;
    }

    shape.setPosition(x, y);

    if (isDashing) {
        state = PlayerState::DASH;
    }
    else if (vx == 0 && vy == 0) {
        if (state == PlayerState::WALK_RIGHT) {
            state = PlayerState::IDLE_RIGHT;
        }
        else if (state == PlayerState::WALK_LEFT) {
            state = PlayerState::IDLE_LEFT;
        }
        else if (state == PlayerState::WALK_UP_LEFT) {
            state = PlayerState::IDLE_UP_LEFT;
        }
        else if (state == PlayerState::WALK_UP_RIGHT) {
            state = PlayerState::IDLE_UP_RIGHT;
        }
        else if (state == PlayerState::WALK_DOWN) {
            state = PlayerState::IDLE_DOWN;
        }
        else if (state == PlayerState::DASH) {
            state = PlayerState::IDLE_RIGHT;
        }
    }
    else if (vy < 0) {
        state = (vx > 0 ? PlayerState::WALK_UP_RIGHT : PlayerState::WALK_UP_LEFT);
    }
    else if (vx != 0) {
        state = (vx > 0 ? PlayerState::WALK_RIGHT : PlayerState::WALK_LEFT);
    }
    else if (vy > 0) {
        state = PlayerState::WALK_DOWN;
    }

    animationManager.setState(state);
    animationManager.setPosition(sf::Vector2f(x - 3, y - 6));
    animationManager.update();

    shadow.setPosition(sf::Vector2f(x + 3, y + HEIGHT - 5));

    sf::Vector2f currentCenter = view.getCenter();
    sf::Vector2f targetCenter  = sf::Vector2f(x, y);
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
    return sf::Vector2f(x, y);
}