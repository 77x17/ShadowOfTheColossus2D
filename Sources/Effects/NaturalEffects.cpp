#include "NaturalEffects.hpp"

#include <iostream>

#include "Clock.hpp"
#include "SmartLightSource.hpp"

NaturalEffects::NaturalEffects() {
    shader = std::make_unique<sf::Shader>();
    smartLights = std::make_unique<SmartLightSource>();
}

void NaturalEffects::load(const std::string& path) {
    if (!shader->loadFromFile(path, sf::Shader::Fragment)) {
        std::cerr << "[Bug] - NaturalEffects.cpp - load() " << path << "\n"; 
    }
}

bool NaturalEffects::loadSmartLightingShader(const std::string& smartLightingPath) {
    return smartLights->loadShader(smartLightingPath);
}

void NaturalEffects::update(float dt, int regionID, const sf::Vector2f& lightNorm, const float& aspectRatio, const Clock& gameClock) {
    if (!shader) {
        std::cerr << "[Bug] - NaturalEffects.cpp - update()\n";
        return; 
    }

    // Reset tất cả về 0 trước
    targetDarkness     = 0.0f;   // 0 = sáng hoàn toàn, 1 = tối đen
    targetLightRadius  = 0.3f;   // bán kính ánh sáng (0.0 - 1.0)
    
    targetFogDensity   = 0.0f;   // 0 = không có sương mù, 1 = sương mù dày đặc 
    targetLightRadius  = 0.5f;   // bán kính vùng không có sương mù (0.0 - 1.0)
    targetClearRadius  = 0.2f;   // màu sắc sương mù 

    targetTintColor    = sf::Vector3f(1.0f, 1.0f, 1.0f);
    targetTintStrength = 0.0f;
    
    std::string timeOfDay = gameClock.getTimeOfDay();
    if (timeOfDay == "Morning") {
        targetDarkness    = 0.0f;
        targetLightRadius = 0.3f;

        targetTintColor    = sf::Vector3f(1.2f, 1.1f, 0.9f); // smooth yellow
        targetTintStrength = 0.2f;
    }
    else if (timeOfDay == "Noon") {
        targetDarkness    = 0.0f;
        targetLightRadius = 0.3f;

        targetTintColor    = sf::Vector3f(1.0f, 1.0f, 1.0f); // clear
        targetTintStrength = 0.0f;
    }
    else if (timeOfDay == "Afternoon") {
        targetDarkness    = 0.05f;
        targetLightRadius = 0.3f;

        targetTintColor    = sf::Vector3f(1.4f, 0.8f, 0.5f); // orange
        targetTintStrength = 0.3f;
    }
    else if (timeOfDay == "Evening") {
        targetDarkness    = 0.3f;
        targetLightRadius = 0.3f;

        targetTintColor    = sf::Vector3f(1.0f, 0.6f, 0.4f); // cam đỏ buổi tối
        targetTintStrength = 0.2f;
    }
    else if (timeOfDay == "Night") {
        targetDarkness    = 0.6f;
        targetLightRadius = 0.3f;

        targetTintColor    = sf::Vector3f(0.7f, 0.8f, 1.2f); // xanh đêm nhẹ
        targetTintStrength = 0.3f;
    }

    switch (regionID) {
        case 0:
            targetFogDensity = 0.1f;
            targetFogColor = sf::Vector3f(0.8f, 0.8f, 0.85f);
            targetClearRadius = 1.0f;
            
            break;
        case 1:
            targetFogDensity = 0.4f;
            targetFogColor = sf::Vector3f(0.8f, 0.8f, 0.85f);
            targetClearRadius = 0.5f;
            
            break;
        case 2: 
            targetFogDensity = 0.5f;
            targetFogColor = sf::Vector3f(0.8f, 0.8f, 0.85f);
            targetClearRadius = 0.4f;
            
            break;
        case 4:
            targetFogDensity = 0.7f;
            targetFogColor = sf::Vector3f(0.6f, 0.6f, 0.65f);
            targetClearRadius = 0.15f;
            
            break;
        case 5:
            targetFogDensity = 0.8f;
            targetFogColor = sf::Vector3f(0.6f, 0.8f, 0.6f);
            targetClearRadius = 0.1f;
            break;
            
        case 6:
            targetFogDensity = 0.3f;
            targetFogColor = sf::Vector3f(0.7f, 0.7f, 0.8f);
            targetClearRadius = 0.2f;
            break;
    }
    
    // Interpolate values smoothly
    interpolateValues(dt);

    // Update shader uniforms
    updateShaderUniforms(lightNorm, aspectRatio);
}

void NaturalEffects::updateSmartLighting(const sf::Vector2f& playerPos, const sf::View& view) {
    if (smartLights) {
        smartLights->update(playerPos, view);
    }
}

void NaturalEffects::interpolateValues(float dt) {
    currentDarkness    += (targetDarkness    - currentDarkness)    * FADE_SPEED * dt;
    currentLightRadius += (targetLightRadius - currentLightRadius) * FADE_SPEED * dt;
    
    currentFogDensity  += (targetFogDensity  - currentFogDensity)  * FADE_SPEED * dt;
    currentClearRadius += (targetClearRadius - currentClearRadius) * FADE_SPEED * dt;
    
    currentFogColor.x += (targetFogColor.x - currentFogColor.x) * FADE_SPEED * dt;
    currentFogColor.y += (targetFogColor.y - currentFogColor.y) * FADE_SPEED * dt;
    currentFogColor.z += (targetFogColor.z - currentFogColor.z) * FADE_SPEED * dt;

    currentTintColor.x  += (targetTintColor.x  - currentTintColor.x)  * FADE_SPEED * dt;
    currentTintColor.y  += (targetTintColor.y  - currentTintColor.y)  * FADE_SPEED * dt;
    currentTintColor.z  += (targetTintColor.z  - currentTintColor.z)  * FADE_SPEED * dt;
    currentTintStrength += (targetTintStrength - currentTintStrength) * FADE_SPEED * dt;
}

void NaturalEffects::updateShaderUniforms(const sf::Vector2f& lightNorm, const float& aspectRatio) {
    // Update old shader uniforms
    if (shader) {
        shader->setUniform("lightPosition", lightNorm);
        shader->setUniform("aspectRatio", aspectRatio);
        shader->setUniform("darkness"   , currentDarkness);
        shader->setUniform("lightRadius", currentLightRadius);
        shader->setUniform("fogDensity" , currentFogDensity);
        shader->setUniform("clearRadius", currentClearRadius);
        shader->setUniform("fogColor"   , currentFogColor);
        shader->setUniform("tintColor"   , currentTintColor);
        shader->setUniform("tintStrength", currentTintStrength);
    }
    
    // Update smart lighting shader uniforms
    if (smartLights && smartLights->getShader()) {
        sf::Shader* smartShader = smartLights->getShader();
        smartShader->setUniform("darkness", currentDarkness);
        smartShader->setUniform("fogDensity", currentFogDensity);
        smartShader->setUniform("clearRadius", currentClearRadius);
        smartShader->setUniform("fogColor", currentFogColor);
        smartShader->setUniform("tintColor", currentTintColor);
        smartShader->setUniform("tintStrength", currentTintStrength);
    }
}

sf::Shader* NaturalEffects::get() {
    if (shader) {
        return shader.get();
    } else {
        std::cerr << "[Bug] - NaturalEffects.cpp get()\n";
    }

    return nullptr;
}

sf::Shader* NaturalEffects::getSmartLightingShader() {
    return smartLights ? smartLights->getShader() : nullptr;
}

bool NaturalEffects::shouldApplyShader() const { 
    return shader && (currentDarkness > 0.01f || currentFogDensity > 0.01f || currentTintStrength > 0.01f); 
}

bool NaturalEffects::shouldApplySmartLighting() const {
    return smartLights && smartLights->shouldApplyShader();
}

// Light management functions
int NaturalEffects::addLight(const sf::Vector2f& position, float radius, 
                            const sf::Vector3f& color, float intensity, int priority) {
    return smartLights ? smartLights->addLight(position, radius, color, intensity, priority) : -1;
}

void NaturalEffects::removeLight(int lightID) {
    if (smartLights) smartLights->removeLight(lightID);
}

void NaturalEffects::updateLightPosition(int lightID, const sf::Vector2f& position) {
    if (smartLights) smartLights->updateLight(lightID, position);
}

void NaturalEffects::setLightActive(int lightID, bool active) {
    if (smartLights) smartLights->setLightActive(lightID, active);
}

void NaturalEffects::setLightIntensity(int lightID, float intensity) {
    if (smartLights) smartLights->setLightIntensity(lightID, intensity);
}

void NaturalEffects::setLightColor(int lightID, const sf::Vector3f& color) {
    if (smartLights) smartLights->setLightColor(lightID, color);
}

void NaturalEffects::setLightPriority(int lightID, int priority) {
    if (smartLights) smartLights->setLightPriority(lightID, priority);
}

void NaturalEffects::setFadeSpeed(float speed) { 
    FADE_SPEED = speed; 
}

void NaturalEffects::clearAllLights() {
    if (smartLights) smartLights->clearAllLights();
}

std::vector<int> NaturalEffects::getLightsInRange(const sf::Vector2f& center, float range) const {
    return smartLights ? smartLights->getLightsInRange(center, range) : std::vector<int>();
}

int NaturalEffects::getTotalLights() const {
    return smartLights ? smartLights->getTotalLights() : 0;
}

int NaturalEffects::getActiveLights() const {
    return smartLights ? smartLights->getActiveLights() : 0;
}

std::vector<int> NaturalEffects::getCurrentlyRenderedLights() const {
    return smartLights ? smartLights->getCurrentlyRenderedLights() : std::vector<int>();
}
