#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "constants.hpp"
#include "animation.hpp"
#include "projectile.hpp"

class Player {
private:
    // const
    const float MOVE_SPEED = 3.0f; 
    const float WIDTH      = TILE_SIZE;
    const float HEIGHT     = TILE_SIZE;

    // position
    float  x,  y;
    float vx, vy;
    
    sf::RectangleShape shape;
    AnimationManager   animationManager;
    Animation          shadow;
    PlayerState state = PlayerState::IDLE_RIGHT;

    // Biến trạng thái cho Dash
    const float DASH_SPEED    = MOVE_SPEED * 3;         // Tốc độ khi lướt
    const float DASH_DURATION = 0.4f;                   // Lướt trong bao nhiêu giây
    const float DASH_COOLDOWN = DASH_DURATION + 0.2f;   // Hồi chiêu bao nhiêu giây
    sf::Vector2f dashDirection;                         // Hướng lướt

    bool isDashing          = false;                    // Có đang lướt không?
    float dashTimer         = 0.0f;                     // Đếm thời gian lướt
    float dashCooldownTimer = 0.0f;                     // Đếm thời gian hồi chiêu
    sf::Clock deltaClock;

    std::vector<Projectile> projectiles;
    const float SHOOT_COOLDOWN      = 0.2f;
    const float PROJECTILE_SPEED    = 400.f;
    const float PROJECTILE_LIFETIME = 1.0f;
    float shootCooldownTimer        = 0.0f;

public:
    Player(int _x, int _y);

    void handleInput(const sf::RenderWindow& window);
    void update(sf::View& view);
    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window) const;

};