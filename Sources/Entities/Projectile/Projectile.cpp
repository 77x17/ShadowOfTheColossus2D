#include "Projectile.hpp"

#include "Constants.hpp"
#include "Normalize.hpp"
#include "TextureManager.hpp"

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

    smoker = Animation(TextureManager::get("smoker"), 64, 64, 11, 14, 0.05f, false);
    
    FROZENCOOLDOWNTIME = 0.55f;
    bounds = smoker.getSprite().getLocalBounds();
    smoker.getSprite().setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    smoker.getSprite().setRotation(angle);
    smoker.getSprite().setScale(1.0f, 1.0f);
}

bool Projectile::isCollision(const sf::FloatRect& rect) const {
    if (lifetime <= 0.0f) {
        return false;
    }
    return hitbox.getGlobalBounds().intersects(rect);
}

void Projectile::update(const float& dt) {
    if (lifetime > 0) {
        lifetime -= dt;
    } 
    if (frozenTimer > 0) {
        frozenTimer -= dt;
    }

    if (lifetime > 0) {
        hitbox.move(velocity * dt * static_cast<float>(std::pow(4, 1.0 - lifetime)));

        sprite.setPosition(hitbox.getPosition()); 
    }
    else if (frozenTimer > 0) {
        smoker.setPosition(hitbox.getPosition());
        smoker.update();
    }
}

void Projectile::draw(sf::RenderTarget& target) const {
    if (lifetime > 0) {
        target.draw(hitbox);

        target.draw(sprite);
    }
    else if (frozenTimer > 0) {
        smoker.draw(target);
    }
}

bool Projectile::isAlive() const {
    return lifetime > 0.0f || frozenTimer > 0.0f;
}

void Projectile::kill() {
    lifetime = 0.0f;

    if (frozenTimer <= 0.0f) {
        frozenTimer = FROZENCOOLDOWNTIME;
    }
}

sf::Vector2f Projectile::getPosition() const {
    return hitbox.getPosition();
}