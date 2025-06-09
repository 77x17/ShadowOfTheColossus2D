#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "constants.hpp"
#include "animation.hpp"
#include "projectile.hpp"
#include "player.hpp"

enum class BatState {
    IDLE_LEFT,
    IDLE_RIGHT,
    ALIVE,
    DEAD,
    DYING
};

class Bat {
private:
    // const
    const float MOVE_SPEED = 3.0f; 
    sf::Vector2f size = sf::Vector2f(TILE_SIZE, TILE_SIZE);

    // position
    sf::Vector2f basePosition    = sf::Vector2f(0.f, 0.f);
    sf::Vector2f position        = sf::Vector2f(0.f, 0.f);
    sf::Vector2f movingDirection = sf::Vector2f(0.f, 0.f);
    
    sf::RectangleShape shape;
    AnimationManager   animationManager;
    Animation          shadow;
    Animation          alert;
    int                state = 0;
    
    sf::Clock   deltaClock;
    const float DETECION_RANGE     = 200.0f;
    const float ALERT_LIFETIME     = 1.0f;
    float       alertCooldownTimer = 0.0f;

    BatState    lifeState          = BatState::ALIVE;
    const float DYING_TIME         = 1.0f;
    float       dyingCooldownTimer = 0.0f;

public:
    Bat(float x, float y);

    bool isAlive() const;

    void respawn();

    void update(Player& player);
    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window) const;

};