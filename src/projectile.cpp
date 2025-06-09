#include "projectile.hpp"

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
    
    shape.setSize(sprite.getLocalBounds().getSize());
    shape.scale(2.f, 2.f);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    shape.setFillColor(sf::Color::Transparent);

    // Center the origin so it positions and rotates from its middle
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(startPos);

    shape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    shape.setPosition(startPos);

    // Calculate velocity from the normalized direction and desired speed
    velocity = normalize(direction) * speed;

    // Optional: Rotate the sprite to face the direction of travel
    float angle = atan2(direction.y, direction.x) * 180 / 3.14159265f;
    sprite.setRotation(angle);
    
    shape.setRotation(angle);
}

bool Projectile::isCollision(const sf::FloatRect& rect) const {
    return shape.getGlobalBounds().intersects(rect);
}

void Projectile::update(float dt) {
    shape.move(velocity * dt * 100.f * static_cast<float>(std::pow(4, 1.0 - lifetime)));
    sprite.setPosition(shape.getPosition());
    lifetime -= dt; // Decrease the remaining lifetime
}

void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    
    window.draw(shape);
}

bool Projectile::isAlive() const {
    return lifetime > 0;
}
