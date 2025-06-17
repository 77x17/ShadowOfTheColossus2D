#include "ParticleManager.hpp"

#include <iostream>

bool ParticleManager::loadTexture(ParticleType type, const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        std::cerr << "[Bug] - ParticleManager.cpp - loadTexture()\n";
        return false;
    }
    texture.setSmooth(true);
    textures[type] = std::move(texture);
    return true;
}

void ParticleManager::spawn(ParticleType type, const sf::Vector2f& position, const sf::Vector2f& velocity) {
    if (textures.find(type) == textures.end()) return;

    float lifeTime = 0.0f;
    switch (type) {
        case ParticleType::Rain: {
            // velocity = sf::Vector2f(0.f, 300.f);
            lifeTime = 2.f;
            break;
        }
        case ParticleType::Leaf: {
            // velocity = sf::Vector2f(float(rand() % 40 - 20), float(50 + rand() % 50));
            lifeTime = 3.f;
            break;
        }
        case ParticleType::Cloud: {
            cloudParticles.emplace_back(textures[ParticleType::Cloud], ParticleType::Cloud, position, velocity);
            cloudParticles.back().rotate((rand() % 1) * 180);
            break;
        }
        default: {
            std::cerr << "[Bug] - ParticleManager.cpp - spawn()\n";
            break;
        }
    }

    // particles.emplace_back(textures[type], type, position, velocity, lifeTime);
}

void ParticleManager::update(float dt, const sf::View& view) {
    if (leavesSpawnCooldownTimer > 0) {
        leavesSpawnCooldownTimer -= dt;
    }
    if (cloudsSpawnCooldownTimer > 0) {
        cloudsSpawnCooldownTimer -= dt;
    }

    // particleManager.spawn(ParticleType::Rain , player.getPosition());
    // particleManager.spawn(ParticleType::Leaf , view.getCenter() - view.getSize() / 3.0f);
    if (cloudsSpawnCooldownTimer <= 0) {
        sf::Vector2f cloudPosition;
        sf::Vector2f cloudVelocity;
        float randomHeight;

        cloudPosition = sf::Vector2f(view.getCenter() - view.getSize() / 2.0f);
        cloudPosition.x -= textures[ParticleType::Cloud].getSize().x / 2.0f;

        randomHeight = rand() % static_cast<int>(view.getSize().y - 40.0f) + 20.0f;
        cloudPosition.y += randomHeight;
        cloudVelocity = sf::Vector2f(20.0f, 0.0f);
        spawn(ParticleType::Cloud, cloudPosition, cloudVelocity);
        
        cloudPosition = sf::Vector2f(view.getCenter() + view.getSize() / 2.0f);
        cloudPosition.x += textures[ParticleType::Cloud].getSize().x / 2.0f;
        randomHeight = rand() % static_cast<int>(view.getSize().y - 40.0f) + 20.0f;
        cloudPosition.y -= randomHeight;
        cloudVelocity = sf::Vector2f(-20.0f, 0.0f);
        spawn(ParticleType::Cloud, cloudPosition, cloudVelocity);

        cloudsSpawnCooldownTimer = CLOUDS_SPAWN_COOLDOWN_TIME;
    }

    // for (auto& p : particles) {
    //     p.update(dt);
    // }
    for (Particle& particle : cloudParticles) {
        particle.update(dt);

        sf::FloatRect viewRect(sf::Vector2f(view.getCenter() - view.getSize()), view.getSize() * 2.0f);
        if (particle.isNotInView(viewRect)) {
            particle.disappear();
        }
    }
    // std::cerr << cloudParticles.size() << '\n';

    cloudParticles.erase(std::remove_if(cloudParticles.begin(), cloudParticles.end(),
        [](const Particle& p) { return p.isDead(); }), cloudParticles.end());
}

void ParticleManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // for (const auto& p : particles) {
    //     target.draw(p.sprite, states);
    // }
    for (const Particle& particle : cloudParticles) {
        target.draw(particle.sprite, states);
    }
}

void ParticleManager::drawScreen(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& p : particles) {
        target.draw(p.sprite, states);
    }
}

bool ParticleManager::isCollisionWithCloud(const sf::FloatRect& rect) const {
    for (const Particle& particle : cloudParticles) {
        if (particle.intersects(rect)) {
            return true;
        }
    }
    return false;
}