#include "Projectile.hpp"

#include <cmath>

// Helper function to normalize a vector (make its length 1)
sf::Vector2f Projectile::normalize(const sf::Vector2f& source) {
    float length = std::sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0) {
        return sf::Vector2f(source.x / length, source.y / length);
    }
    // Return the original vector if its length is 0 to avoid division by zero.
    return source;
}

Projectile::Projectile(const sf::Texture& texture, sf::Vector2f startPos, sf::Vector2f direction, float speed, float life)
    : lifetime(life) {
        
    sprite.setTexture(texture);
    sprite.scale(2.f, 2.f);
    
    hitbox.setSize(sprite.getLocalBounds().getSize());
    hitbox.scale(2.f, 2.f);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);
    hitbox.setFillColor(sf::Color::Transparent);

    // Center the origin so it positions and rotates from its middle
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(startPos);

    hitbox.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    hitbox.setPosition(startPos);

    // Calculate velocity from the normalized direction and desired speed
    velocity = normalize(direction) * speed;

    // Optional: Rotate the sprite to face the direction of travel
    float angle = atan2(direction.y, direction.x) * 180 / 3.14159265f;
    sprite.setRotation(angle);
    
    hitbox.setRotation(angle);
}

bool Projectile::isCollision(const sf::FloatRect& rect) const {
    return hitbox.getGlobalBounds().intersects(rect);
}

void Projectile::update(float dt) {
    hitbox.move(velocity * dt * static_cast<float>(std::pow(4, 1.0 - lifetime)));
    sprite.setPosition(hitbox.getPosition());
    lifetime -= dt; // Decrease the remaining lifetime
}

void Projectile::draw(sf::RenderWindow& window) const {
    if (isAlive()) {
        // window.draw(hitbox);

        window.draw(sprite);
    }
}

bool Projectile::isAlive() const {
    return lifetime > 0;
}

void Projectile::kill() {
    lifetime = 0;
}

sf::Vector2f Projectile::getPosition() const {
    return hitbox.getPosition();
}