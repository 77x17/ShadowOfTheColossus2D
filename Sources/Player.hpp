#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Constants.hpp"
#include "Animation.hpp"
#include "Projectile.hpp"
#include "SoundManager.hpp"

enum class PlayerState {
    IDLE_LEFT,
    IDLE_RIGHT,
    IDLE_UP_LEFT,
    IDLE_UP_RIGHT,
    IDLE_DOWN,
    WALK_LEFT,
    WALK_RIGHT,
    WALK_UP_LEFT,
    WALK_UP_RIGHT,
    WALK_DOWN,
    DASH_LEFT,
    DASH_RIGHT,

    ALIVE,
    DEAD,
    DYING
};

class Player {
private:
    // const
    const float MOVE_SPEED = 5.0f; 
    sf::Vector2f size = sf::Vector2f(TILE_SIZE, TILE_SIZE);

    // position
    sf::Vector2f basePosition    = sf::Vector2f(0.f, 0.f);
    sf::Vector2f position        = sf::Vector2f(0.f, 0.f);
    sf::Vector2f movingDirection = sf::Vector2f(0.f, 0.f);
    
    sf::RectangleShape hitbox;
    sf::CircleShape    loadingBox;
    AnimationManager   animationManager;
    Animation          shadow;
    int                state = 0;

    // Biến trạng thái cho Dash
    const float DASH_SPEED    = MOVE_SPEED * 2;         // Tốc độ khi lướt
    const float DASH_DURATION = 0.4f;                   // Lướt trong bao nhiêu giây
    const float DASH_COOLDOWN = DASH_DURATION + 0.2f;   // Hồi chiêu bao nhiêu giây
    sf::Vector2f dashDirection;                         // Hướng lướt

    sf::Clock deltaClock;
    bool isDashing          = false;                    // Có đang lướt không?
    float dashTimer         = 0.0f;                     // Đếm thời gian lướt
    float dashCooldownTimer = 0.0f;                     // Đếm thời gian hồi chiêu

    std::vector<Projectile> projectiles;
    const float SHOOT_COOLDOWN      = 0.2f;
    const float PROJECTILE_SPEED    = MOVE_SPEED;
    const float PROJECTILE_LIFETIME = 1.0f;
    float shootCooldownTimer        = 0.0f;

    PlayerState lifeState = PlayerState::ALIVE;

public:
    Player(float x, float y);

    void handleInput(const sf::RenderWindow& window);
    bool isCollisionProjectiles(const sf::FloatRect& rect) const;
    bool isCollision(const sf::FloatRect& rect) const;

    bool isAlive() const;
    void kill();
    void respawn();

    sf::FloatRect getHitBox() const;

    void update(sf::View& view);
    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window) const;

};