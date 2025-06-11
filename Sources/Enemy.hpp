#pragma once

#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Animation.hpp"
#include "Player.hpp"
#include "SoundManager.hpp"

class Enemy {
protected:
    int          state           = 0;
    float        healthPoints    = 0.0f;
    float        MOVE_SPEED      = 125.0f; 
    sf::Vector2f size            = sf::Vector2f(TILE_SIZE, TILE_SIZE);
    sf::Vector2f basePosition    = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f position        = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f movingDirection = sf::Vector2f(0.0f, 0.0f);
    
    float DYING_TIME         = 1.0f;
    float dyingCooldownTimer = 0.0f;

    float RESPAWN_TIME         = 5.0f;
    float respawnCooldownTimer = 0.0f;

    float DETECION_RANGE     = 150.0f;
    float ALERT_LIFETIME     = 0.5f;
    float alertCooldownTimer = 0.0f;

    float RANDOM_TIME          = 5.0f;
    float randomCooldownTimer  = 0.0f;
    float stayingCooldownTimer = 0.0f;

    sf::RectangleShape hitbox;
    sf::CircleShape    detectionBox;
    AnimationManager   animationManager;
    Animation          shadow;
    Animation          alert;

public:
    Enemy(const float& x, const float& y, const float& width, const float& height, const float& hp);

    virtual ~Enemy() = default;

    bool isAlive() const;
    virtual void respawn();

    float calculateDistance(const Player& player) const;

    virtual void updateTimer(const float &dt);
    virtual void attackPlayer(const Player& player);
    void moveRandomly();
    void updateThinking(Player& player);
    void updatePosition(const float& dt, const std::vector<sf::FloatRect>& collisionRects);
    void updateHitbox();
    virtual void updateAnimation() = 0;
    virtual void update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects);

    virtual void draw(sf::RenderWindow& window);
};