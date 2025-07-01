#pragma once

#include <SFML/Graphics.hpp>

#include "AnimationManager.hpp"

class Projectile {
private:
    sf::RectangleShape hitbox;
    sf::Sprite         sprite;
    sf::Vector2f       velocity;
    float              lifetime;

    float              FROZENCOOLDOWNTIME;
    float              frozenTimer = 0.0f;

    Animation          smoker;

public:
    Projectile() = default;

    Projectile(const sf::Texture& texture, 
               sf::Vector2f       startPos, 
               sf::Vector2f       direction, 
               float              speed, 
               float              life);

    bool isCollision(const sf::FloatRect& rect) const;

    void update(const float& dt);
    void draw(sf::RenderTarget& target) const;

    bool isAlive() const;  
    void kill();

    sf::Vector2f getPosition() const;

};