#include "NaturalEffects.hpp"

#include <iostream>

NaturalEffects::NaturalEffects() {
    shader = std::make_unique<sf::Shader>();
}

void NaturalEffects::load(const std::string& path) {
    if (!shader->loadFromFile(path, sf::Shader::Fragment)) {
        std::cerr << "[Bug] - NaturalEffects.cpp - load() " << path << "\n"; 
    }
}

void NaturalEffects::update(float dt, int regionID, const sf::Vector2f& lightNorm, const float& aspectRatio) {
    if (!shader) {
        std::cerr << "[Bug] - NaturalEffects.cpp - update()\n";
        return; 
    }

    // Reset tất cả về 0 trước
    targetDarkness    = 0.0f;
    targetFogDensity  = 0.0f;
    targetLightRadius = 0.5f;
    targetClearRadius = 0.2f;
    
    switch (regionID) {
        case 0:
            targetFogDensity = 0.2f;
            targetFogColor = sf::Vector3f(0.8f, 0.8f, 0.85f);
            targetClearRadius = 0.5f;
            
            break;
        case 1:
            targetDarkness = 0.6f;
            targetLightRadius = 0.4f;

            targetFogDensity = 0.4f;
            targetFogColor = sf::Vector3f(0.8f, 0.8f, 0.85f);
            targetClearRadius = 0.25f;
            
            break;
        case 2: // Vùng tối nặng
            targetDarkness = 0.8f;
            targetLightRadius = 0.3f;

            targetFogDensity = 0.4f;
            targetFogColor = sf::Vector3f(0.8f, 0.8f, 0.85f);
            targetClearRadius = 0.25f;
            
            break;
        case 4: // Vùng sương mù dày
            targetFogDensity = 0.7f;
            targetFogColor = sf::Vector3f(0.6f, 0.6f, 0.65f);
            targetClearRadius = 0.15f;
            
            break;
        case 5: // Vùng sương mù độc hại
            targetFogDensity = 0.8f;
            targetFogColor = sf::Vector3f(0.6f, 0.8f, 0.6f);
            targetClearRadius = 0.1f;
            break;
            
        case 6: // Vùng kết hợp tối + sương mù
            targetDarkness = 0.5f;
            targetLightRadius = 0.35f;
            targetFogDensity = 0.3f;
            targetFogColor = sf::Vector3f(0.7f, 0.7f, 0.8f);
            targetClearRadius = 0.2f;
            break;
    }
    
    // Cập nhật mượt mà tất cả giá trị
    currentDarkness    += (targetDarkness - currentDarkness) * FADE_SPEED * dt;
    currentLightRadius += (targetLightRadius - currentLightRadius) * FADE_SPEED * dt;
    
    currentFogDensity  += (targetFogDensity - currentFogDensity) * FADE_SPEED * dt;
    currentClearRadius += (targetClearRadius - currentClearRadius) * FADE_SPEED * dt;
    
    currentFogColor.x += (targetFogColor.x - currentFogColor.x) * FADE_SPEED * dt;
    currentFogColor.y += (targetFogColor.y - currentFogColor.y) * FADE_SPEED * dt;
    currentFogColor.z += (targetFogColor.z - currentFogColor.z) * FADE_SPEED * dt;

    shader->setUniform("lightPosition", lightNorm);
    shader->setUniform("aspectRatio", aspectRatio);

    // Darkness uniforms
    shader->setUniform("darkness"   , currentDarkness);
    shader->setUniform("lightRadius", currentLightRadius);

    // Fog uniforms
    shader->setUniform("fogDensity" , currentFogDensity);
    shader->setUniform("clearRadius", currentClearRadius);
    shader->setUniform("fogColor"   , currentFogColor);
}

sf::Shader* NaturalEffects::get() {
    if (shader) {
        return shader.get();
    } else {
        std::cerr << "[Bug] - NaturalEffects.cpp get()\n";
    }

    return nullptr;
}

bool NaturalEffects::shouldApplyShader() const { 
    return shader && currentDarkness > 0.01f || currentFogDensity > 0.01f; 
}

void NaturalEffects::setFadeSpeed(float speed) { FADE_SPEED = speed; }