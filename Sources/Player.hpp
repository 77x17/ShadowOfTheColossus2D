#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Constants.hpp"
#include "Animation.hpp"
#include "Projectile.hpp"
#include "SoundManager.hpp"

enum class PlayerState {
    DYING = -2,
    DEAD  = -1,
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
    DASH_RIGHT
};

class Player {
private:
    // const
    int          state;
    float        MOVE_SPEED;
    sf::Vector2f size;
    sf::Vector2f basePosition;
    sf::Vector2f position;
    sf::Vector2f movingDirection;

    float DYING_TIME;
    float dyingCooldownTimer;
    
    float RESPAWN_TIME;
    float respawnCooldownTimer;

    float VIEW_LEAP_SPEED;

    // Biến trạng thái cho Dash
    sf::Vector2f dashDirection; // Hướng lướt
    bool  isDashing;            // Có đang lướt không?
    float DASH_SPEED;           // Tốc độ khi lướt
    float DASH_DURATION;        // Lướt trong bao nhiêu giây
    float dashTimer;            // Đếm thời gian lướt
    float DASH_COOLDOWN;        // Hồi chiêu bao nhiêu giây
    float dashCooldownTimer;    // Đếm thời gian hồi chiêu

    sf::RectangleShape hitbox;
    sf::CircleShape    loadingBox;
    AnimationManager   animationManager;
    Animation          shadow;

    std::vector<Projectile> projectiles;
    float SHOOT_COOLDOWN;
    float PROJECTILE_SPEED;
    float PROJECTILE_LIFETIME;
    float shootCooldownTimer;

public:
    Player(float x, float y);

    void handleInput(const sf::RenderWindow& window);
    bool isCollisionProjectiles(const sf::FloatRect& rect);
    bool isCollision(const sf::FloatRect& rect) const;

    bool isAlive() const;
    void kill();
    void respawn();

    sf::FloatRect getHitBox() const;

    void updateTimer(const float &dt);
    void updatePosition(const float& dt, const std::vector<sf::FloatRect>& collisionRects);
    void updateHitbox();
    void updateAnimation();
    void updateProjectiles(const float& dt);
    void update(const float& dt, const sf::RenderWindow& window, const std::vector<sf::FloatRect>& collisionRects);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    void updateView(const float& dt, sf::View& view) const;
};