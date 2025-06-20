#include "Projectile.hpp"

#include "Constants.hpp"
#include "Normalize.hpp"

#include <cmath>
#include <iostream>

Projectile::Projectile(const sf::Texture& texture, 
                       sf::Vector2f       startPos, 
                       sf::Vector2f       direction, 
                       float              speed, 
                       float              life)
: lifetime(life) {
    sprite.setTexture(texture);
    sprite.scale(2.f, 2.f);
    
    hitbox.setSize(sprite.getLocalBounds().getSize());
    hitbox.scale(2.f, 2.f);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);
    hitbox.setFillColor(sf::Color::Transparent);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(startPos);

    hitbox.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    hitbox.setPosition(startPos);

    velocity = Normalize::normalize(direction) * speed;

    float angle = atan2(direction.y, direction.x) * 180 / 3.14159265f;
    sprite.setRotation(angle);
    
    hitbox.setRotation(angle);
}

bool Projectile::isCollision(const sf::FloatRect& rect) const {
    if (!isAlive()) {
        return false;
    }
    return hitbox.getGlobalBounds().intersects(rect);
}

void Projectile::update(const float& dt) {
    if (lifetime > 0) {
        lifetime -= dt;
    } 

    if (isAlive()) {
        hitbox.move(velocity * dt * static_cast<float>(std::pow(4, 1.0 - lifetime)));

        sprite.setPosition(hitbox.getPosition()); 
    }
}

void Projectile::draw(sf::RenderTarget& target) const {
    if (isAlive()) {
        // target.draw(hitbox);

        target.draw(sprite);
    }
}

bool Projectile::isAlive() const {
    return lifetime > 0.0f;
}

void Projectile::kill() {
    lifetime = 0.0f;
}

sf::Vector2f Projectile::getPosition() const {
    return hitbox.getPosition();
}