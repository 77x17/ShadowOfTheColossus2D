#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Clock;
class SmartLightSource;

class NaturalEffects {
private:
    std::unique_ptr<sf::Shader> shader;
    std::unique_ptr<SmartLightSource> smartLights;

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
    ~NaturalEffects() = default;
    
    void load(const std::string& path);
    bool loadSmartLightingShader(const std::string& smartLightingPath);

    void update(float dt, int regionID, const sf::Vector2f& lightNorm, const float& aspectRatio, const Clock& gameClock);
    void updateSmartLighting(const sf::Vector2f& playerPos, const sf::View& view);
    
    sf::Shader* get();
    sf::Shader* getSmartLightingShader();

    bool shouldApplyShader() const;
    bool shouldApplySmartLighting() const;

    // Quản lý lights
    int addLight(const sf::Vector2f& position, float radius = 1000.0f, 
                 const sf::Vector3f& color = sf::Vector3f(1.0f, 1.0f, 1.0f),
                 float intensity = 1.0f, int priority = 0);
    void removeLight(int lightID);
    void updateLightPosition(int lightID, const sf::Vector2f& position);
    void setLightActive(int lightID, bool active);
    void setLightIntensity(int lightID, float intensity);
    void setLightColor(int lightID, const sf::Vector3f& color);
    void setLightPriority(int lightID, int priority);
    
    // Utility
    void setFadeSpeed(float speed);
    void clearAllLights();
    std::vector<int> getLightsInRange(const sf::Vector2f& center, float range) const;

    // Debug info
    int getTotalLights() const;
    int getActiveLights() const;
    std::vector<int> getCurrentlyRenderedLights() const;

private:
    void interpolateValues(float dt);
    void updateShaderUniforms(const sf::Vector2f& lightNorm, const float& aspectRatio);

};