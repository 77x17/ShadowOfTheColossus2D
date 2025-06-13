#pragma once

#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Animation.hpp"
#include "Player.hpp"
#include "SoundManager.hpp"
#include "Font.hpp"

class Enemy {
protected:
    int          state;
    float        MOVE_SPEED; 
    sf::Vector2f size;
    sf::Vector2f basePosition;
    sf::Vector2f position;
    sf::Vector2f movingDirection;

    int                TEXT_SIZE;
    float              BACKGROUND_PADDING;
    sf::Text           label;
    sf::RectangleShape labelBackground;

    float              HEALTH_POINTS_BAR_WIDTH;
    float              HEALTH_POINTS_BAR_HEIGHT;
    float              maxHealthPoints;
    float              healthPoints;
    sf::RectangleShape healthPointsBar;
    sf::RectangleShape healthPointsBarBackground;

    float DYING_TIME;
    float dyingCooldownTimer;

    float RESPAWN_TIME;
    float respawnCooldownTimer;
    
    float INVINCIBLE_TIME;
    float invincibleCooldownTimer;

    float DETECION_RANGE;
    float ALERT_LIFETIME;
    float alertCooldownTimer;

    float RANDOM_TIME;
    float randomCooldownTimer;
    float stayingCooldownTimer;

    float ATTACK_COOLDOWN_TIME;
    float attackCooldownTimer;

    sf::RectangleShape hitbox;
    sf::CircleShape    detectionBox;
    AnimationManager   animationManager;
    Animation          shadow;
    Animation          alert;

    float KNOCKBACK_STRENGTH;
    float KNOCKBACK_COOLDOWN;
    float knockbackCooldownTimer;

public:
    Enemy(const float& x, const float& y, const float& width, const float& height, const float& hp, const std::string& nameAndLevel);

    virtual ~Enemy() = default;

    bool isAlive() const;
    void attack(Player& player);
    void hurt(const float& damage);
    void knockback(const sf::Vector2f& playerPosition);
    virtual void kill();
    virtual void respawn();

    float calculateDistance(const Player& player) const;

    virtual void updateTimer(const float &dt);
    virtual void followPlayer(const Player& player);
    void moveRandomly();
    void updateThinking(Player& player);
    void updatePosition(const float& dt, const std::vector<sf::FloatRect>& collisionRects);
    void updateHitbox();
    virtual void updateAnimation() = 0;
    virtual void update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects);

    virtual void draw(sf::RenderWindow& window);
};