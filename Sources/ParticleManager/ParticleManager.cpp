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

            particles.emplace_back(textures[type], ParticleType::Rain, position, velocity, lifeTime);
            break;
        }
        case ParticleType::Leaf: {
            // velocity = sf::Vector2f(float(rand() % 40 - 20), float(50 + rand() % 50));
            lifeTime = 3.f;
            break;
        }
        case ParticleType::Cloud: {
            cloudParticles.emplace_back(textures[ParticleType::Cloud], ParticleType::Cloud, position, velocity);
            cloudParticles.back().rotate((rand() % 2) * 180);
            cloudParticles.back().flip(rand() % 2, rand() % 2);
            break;
        }
        default: {
            std::cerr << "[Bug] - ParticleManager.cpp - spawn()\n";
            break;
        }
    }
}

void ParticleManager::update(float dt, const sf::View& view) {
    if (leavesSpawnCooldownTimer > 0) {
        leavesSpawnCooldownTimer -= dt;
    }
    if (cloudsSpawnCooldownTimer > 0) {
        cloudsSpawnCooldownTimer -= dt;
    }
    if (rainsSpawnCooldownTimer > 0) {
        rainsSpawnCooldownTimer -= dt;
    }

    if (rainsSpawnCooldownTimer <= 0 && makeRain) {
        sf::Vector2f rainPosition = sf::Vector2f(0.0f, -static_cast<float>(textures[ParticleType::Rain].getSize().y));
        sf::Vector2f rainVelocity = sf::Vector2f(-100.0f, 400.0f);

        spawn(ParticleType::Rain, rainPosition, rainVelocity);

        rainsSpawnCooldownTimer = RAINS_SPAWN_COOLDOWN_TIME;
    }

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

    for (auto& particle : particles) {
        particle.update(dt);
    }
    for (Particle& particle : cloudParticles) {
        particle.update(dt);

        sf::FloatRect viewRect(sf::Vector2f(view.getCenter() - view.getSize()), view.getSize() * 2.0f);
        if (particle.isNotInView(viewRect)) {
            particle.disappear();
        }

        particle.sprite.setColor(sf::Color(255, 255, 255, 255));
    }
    // std::cerr << cloudParticles.size() << '\n';

    cloudParticles.erase(std::remove_if(cloudParticles.begin(), cloudParticles.end(),
        [](const Particle& p) { return p.isDead(); }), cloudParticles.end());
}

void ParticleManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const Particle& particle : cloudParticles) {
        target.draw(particle.sprite, states);
    }
}

void ParticleManager::drawScreen(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& particle : particles) {
        target.draw(particle.sprite, states);
    }
}

void ParticleManager::isCollisionWithCloud(const sf::FloatRect& rect) {
    for (Particle& particle : cloudParticles) {
        if (particle.intersects(rect)) {
            particle.sprite.setColor(sf::Color(255, 255, 255, 150));
        }
    }
}

void ParticleManager::isCollisionWithRain(int regionID) {
    switch (regionID) {
        case 0:
            makeRain = false;
            break;
        case 1:
            makeRain = false;
            break;
        case 2:
            makeRain = true;
            break;
        default:
            makeRain = false;
            break;
    }
}

void ParticleManager::loadTexture() {
    loadTexture(ParticleType::Rain , "Assets/Shaders/rain.png");
    loadTexture(ParticleType::Leaf , "Assets/Shaders/leaf.png");
    loadTexture(ParticleType::Cloud, "Assets/Shaders/cloud.png");
}