#pragma once

#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Animation.hpp"
#include "Projectile.hpp"
#include "Player.hpp"
#include "SoundManager.hpp"

enum class EyeState {
    IDLE_LEFT,
    IDLE_RIGHT,

    ALIVE,
    DEAD,
    DYING
};

class Eye {
private:
    // const
    const float MOVE_SPEED = 2.0f; 
    sf::Vector2f size = sf::Vector2f(TILE_SIZE, TILE_SIZE);

    // position
    sf::Vector2f basePosition    = sf::Vector2f(0.f, 0.f);
    sf::Vector2f position        = sf::Vector2f(0.f, 0.f);
    sf::Vector2f movingDirection = sf::Vector2f(0.f, 0.f);
    
    sf::RectangleShape hitbox;
    sf::CircleShape    detectionBox;
    AnimationManager   animationManager;
    Animation          shadow;
    Animation          alert;
    int                state = 0;
    
    sf::Clock   deltaClock;
    const float DETECION_RANGE     = 250.0f;
    const float ALERT_LIFETIME     = 1.0f;
    float       alertCooldownTimer = 0.0f;

    EyeState    lifeState          = EyeState::ALIVE;
    const float DYING_TIME         = 1.0f;
    float       dyingCooldownTimer = 0.0f;

    const float RANDOM_TIME          = 5.0f;
    float       randomCooldownTimer  = 0.0f;
    float       stayingCooldownTimer = 0.0f;

    const float RESPAWN_TIME         = 5.0f;
    float       respawnCooldownTimer = 0.0f;

    Projectile  projectile;
    const float SHOOT_COOLDOWN      = 2.0f;
    const float PROJECTILE_SPEED    = MOVE_SPEED;
    const float PROJECTILE_LIFETIME = 1.0f;
    float shootCooldownTimer        = 0.0f;

public:
    Eye(float x, float y);

    bool isAlive() const;

    void respawn();

    float distance(const Player& player) const;

    void update(Player& player);
    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window) const;

};