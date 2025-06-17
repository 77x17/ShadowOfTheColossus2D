#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

enum class ParticleType {
    Rain,
    Leaf,
    Cloud
};

struct Particle {
    ParticleType type;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float lifetime;
    sf::FloatRect bounds;
    
    Particle(const sf::Texture& texture, ParticleType _type, const sf::Vector2f& pos, const sf::Vector2f& vel, float life = 1.0f)
        : type(_type), velocity(vel), lifetime(life) {
        sprite.setTexture(texture);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
        sprite.setPosition(pos);

        // Giảm kích thước bounds xuống 50% mỗi chiều (bỏ 1/4 mỗi bên)
        bounds = sprite.getGlobalBounds();
        float shrinkX = bounds.width * 0.25f;
        float shrinkY = bounds.height * 0.25f;
        bounds.left   += shrinkX;
        bounds.top    += shrinkY;
        bounds.width  -= 2 * shrinkX;
        bounds.height -= 2 * shrinkY;
    }

    void update(float dt) {
        if (type != ParticleType::Cloud) {
            lifetime -= dt;
        }
        
        sprite.move(velocity * dt);
        
        bounds.left += velocity.x * dt;
        bounds.top  += velocity.y * dt;
    }

    void disappear() {
        lifetime = 0;
    }
    
    bool isNotInView(const sf::FloatRect& rect) const {
        return !rect.intersects(sprite.getGlobalBounds());
    }

    bool intersects(const sf::FloatRect& rect) const {
        return rect.intersects(bounds);
        // return rect.intersects(sprite.getGlobalBounds());
    }

    bool isDead() const {
        return lifetime <= 0.f;
    }

    void rotate(const float& rotation) {
        sprite.setRotation(rotation);
    }

    void flip(const float& rotation) {
        sprite.setRotation(rotation);
    }
};

class ParticleManager : public sf::Drawable {
private:
    std::vector<Particle> particles;
    std::vector<Particle> cloudParticles;
    std::unordered_map<ParticleType, sf::Texture> textures;

    float LEAVES_SPAWN_COOLDOWN_TIME = 10.0f;
    float CLOUDS_SPAWN_COOLDOWN_TIME = 15.0f;

    float leavesSpawnCooldownTimer   = 0.0f;
    float cloudsSpawnCooldownTimer   = 0.0f;

public:
    ParticleManager() {}

    bool loadTexture(ParticleType type, const std::string& filename);

    void spawn(ParticleType type, const sf::Vector2f& position, const sf::Vector2f& velocity);

    void update(float dt, const sf::View& view);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void drawScreen(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    bool isCollisionWithCloud(const sf::FloatRect& rect) const;
};
