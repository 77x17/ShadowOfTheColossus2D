#pragma once

#include <SFML/Graphics.hpp>

class Projectile {
private:
    sf::RectangleShape hitbox;
    sf::Sprite         sprite;
    sf::Vector2f       velocity;
    float              lifetime;
public:
    static sf::Vector2f normalize(const sf::Vector2f& source);
    
    Projectile(const sf::Texture& texture, sf::Vector2f startPos, sf::Vector2f direction, float speed, float life);

    bool isCollision(const sf::FloatRect& rect) const;

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    bool isAlive() const;

};