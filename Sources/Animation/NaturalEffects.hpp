#pragma once
#include <SFML/Graphics.hpp>

#include "Clock.hpp"

#include <memory>

class NaturalEffects {
private:
    std::unique_ptr<sf::Shader> shader;
    // Darkness properties
    float currentDarkness = 0.0f;
    float targetDarkness  = 0.0f;
    float currentLightRadius = 0.5f;
    float targetLightRadius  = 0.5f;
    
    // Fog properties
    float currentFogDensity = 0.0f;
    float targetFogDensity  = 0.0f;
    float currentClearRadius = 0.2f;
    float targetClearRadius  = 0.2f;
    sf::Vector3f currentFogColor = sf::Vector3f(0.7f, 0.7f, 0.7f);
    sf::Vector3f targetFogColor  = sf::Vector3f(0.7f, 0.7f, 0.7f);
    
    float currentTintStrength = 0.0f;
    float targetTintStrength = 0.0f;
    sf::Vector3f currentTintColor;
    sf::Vector3f targetTintColor;

    float FADE_SPEED = 0.5f;

public:
    NaturalEffects();
    
    void load(const std::string& path);

    void update(float dt, int regionID, const sf::Vector2f& lightNorm, const float& aspectRatio, const Clock& gameClock);
    
    sf::Shader* get();

    bool shouldApplyShader() const;
    
    void setFadeSpeed(float speed);
};