#pragma once

#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Animation.hpp"
#include "Player.hpp"
#include "SoundManager.hpp"

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

    float DETECION_RANGE;
    float ALERT_LIFETIME;
    float alertCooldownTimer;

    float RANDOM_TIME;
    float randomCooldownTimer;
    float stayingCooldownTimer;

    sf::RectangleShape hitbox;
    sf::CircleShape    detectionBox;
    AnimationManager   animationManager;
    Animation          shadow;
    Animation          alert;

public:
    Enemy(const float& x, const float& y, const float& width, const float& height, const float& hp, const std::string& nameAndLevel);

    virtual ~Enemy() = default;

    bool isAlive() const;
    virtual void kill();
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