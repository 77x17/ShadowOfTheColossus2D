#include "SmartLightSource.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

SmartLightSource::SmartLightSource() 
    : activeLightCount(0), playerPosition(0, 0), viewSize(800, 600) {
    
    lightShader = std::make_unique<sf::Shader>();
    
    // Khởi tạo arrays cho shader uniforms
    lightPositions.resize(MAX_LIGHTS);
    lightRadii.resize(MAX_LIGHTS);
    lightColors.resize(MAX_LIGHTS);
    lightIntensities.resize(MAX_LIGHTS);
}

bool SmartLightSource::loadShader(const std::string& fragmentPath) {
    if (!lightShader->loadFromFile(fragmentPath, sf::Shader::Fragment)) {
        std::cerr << "[Error] - SmartLightSource: Failed to load shader from " << fragmentPath << std::endl;
        return false;
    }
    return true;
}

int SmartLightSource::addLight(const sf::Vector2f& position, float radius, 
                               const sf::Vector3f& color, float intensity, int priority) {
    auto light = std::make_unique<LightSource>(position, radius, color, intensity, priority);
    lights.push_back(std::move(light));
    return static_cast<int>(lights.size()) - 1; // Trả về ID của light mới
}

void SmartLightSource::removeLight(int lightID) {
    if (lightID >= 0 && lightID < static_cast<int>(lights.size())) {
        lights.erase(lights.begin() + lightID);
    }
}

void SmartLightSource::updateLight(int lightID, const sf::Vector2f& position) {
    if (lightID >= 0 && lightID < static_cast<int>(lights.size()) && lights[lightID]) {
        lights[lightID]->position = position;
    }
}

void SmartLightSource::setLightActive(int lightID, bool active) {
    if (lightID >= 0 && lightID < static_cast<int>(lights.size()) && lights[lightID]) {
        lights[lightID]->isActive = active;
    }
}

void SmartLightSource::setLightIntensity(int lightID, float intensity) {
    if (lightID >= 0 && lightID < static_cast<int>(lights.size()) && lights[lightID]) {
        lights[lightID]->intensity = std::clamp(intensity, 0.0f, 1.0f);
    }
}

void SmartLightSource::setLightColor(int lightID, const sf::Vector3f& color) {
    if (lightID >= 0 && lightID < static_cast<int>(lights.size()) && lights[lightID]) {
        lights[lightID]->color = color;
    }
}

void SmartLightSource::setLightPriority(int lightID, int priority) {
    if (lightID >= 0 && lightID < static_cast<int>(lights.size()) && lights[lightID]) {
        lights[lightID]->priority = priority;
    }
}

void SmartLightSource::update(const sf::Vector2f& playerPos, const sf::View& view) {
    playerPosition = playerPos;
    
    // Cập nhật view bounds
    sf::Vector2f viewCenter = view.getCenter();
    viewSize = view.getSize();
    currentViewBounds = sf::FloatRect(
        viewCenter.x - viewSize.x / 2,
        viewCenter.y - viewSize.y / 2,
        viewSize.x,
        viewSize.y
    );
    
    // Tính toán lights quan trọng nhất
    calculateImportantLights();
    
    // Cập nhật shader uniforms
    updateShaderUniforms();
}

void SmartLightSource::calculateImportantLights() {
    // Tạo danh sách lights hợp lệ (active và trong tầm)
    std::vector<int> candidateLights;
    
    for (int i = 0; i < static_cast<int>(lights.size()); ++i) {
        if (!lights[i] || !lights[i]->isActive) continue;
        
        // Kiểm tra khoảng cách đến player
        float distToPlayer = lights[i]->distanceTo(playerPosition);
        if (distToPlayer > CULLING_DISTANCE) continue;
        
        // Kiểm tra có trong view không (với buffer)
        if (!lights[i]->isInView(currentViewBounds)) continue;
        
        candidateLights.push_back(i);
    }
    
    // Sắp xếp theo độ quan trọng
    std::sort(candidateLights.begin(), candidateLights.end(), 
              [this](int a, int b) {
                  const auto& lightA = lights[a];
                  const auto& lightB = lights[b];
                  
                  // Ưu tiên theo priority trước
                  if (lightA->priority != lightB->priority) {
                      return lightA->priority > lightB->priority;
                  }
                  
                  // Sau đó theo khoảng cách đến player
                  float distA = lightA->distanceTo(playerPosition);
                  float distB = lightB->distanceTo(playerPosition);
                  
                  return distA < distB;
              });
    
    // Chọn tối đa MAX_LIGHTS
    activeLightCount = std::min((int)candidateLights.size(), MAX_LIGHTS);
    
    // Cập nhật arrays cho shader
    for (int i = 0; i < activeLightCount; ++i) {
        int lightIndex = candidateLights[i];
        const auto& light = lights[lightIndex];
        
        // Chuyển đổi world coordinates sang normalized coordinates [0,1]
        lightPositions[i] = sf::Vector2f(
            (light->position.x - currentViewBounds.left) / currentViewBounds.width,
            (light->position.y - currentViewBounds.top) / currentViewBounds.height
        );
        
        // Chuyển đổi radius sang normalized scale
        lightRadii[i] = light->radius / std::max(currentViewBounds.width, currentViewBounds.height);
        
        lightColors[i] = light->color;
        lightIntensities[i] = light->intensity;
    }
    
    // Clear unused slots
    for (int i = activeLightCount; i < MAX_LIGHTS; ++i) {
        lightPositions[i] = sf::Vector2f(0, 0);
        lightRadii[i] = 0.0f;
        lightColors[i] = sf::Vector3f(0, 0, 0);
        lightIntensities[i] = 0.0f;
    }
}

void SmartLightSource::updateShaderUniforms() const {
    if (!lightShader) return;
    
    // Aspect ratio
    float aspectRatio = viewSize.x / viewSize.y;
    lightShader->setUniform("aspectRatio", aspectRatio);
    
    // Số lượng lights active
    lightShader->setUniform("numLights", activeLightCount);
    
    // Light arrays - SFML yêu cầu truyền từng element
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        std::string indexStr = std::to_string(i);
        
        lightShader->setUniform("lightPositions[" + indexStr + "]", lightPositions[i]);
        lightShader->setUniform("lightRadii[" + indexStr + "]", lightRadii[i]);
        lightShader->setUniform("lightColors[" + indexStr + "]", lightColors[i]);
        lightShader->setUniform("lightIntensities[" + indexStr + "]", lightIntensities[i]);
    }
}

sf::Shader* SmartLightSource::getShader() const {
    return lightShader.get();
}

bool SmartLightSource::shouldApplyShader() const {
    return lightShader && activeLightCount > 0;
}

std::vector<int> SmartLightSource::getCurrentlyRenderedLights() const {
    std::vector<int> result;
    
    // Tìm lại lights đang được render dựa trên logic tương tự
    std::vector<int> candidateLights;
    
    for (int i = 0; i < static_cast<int>(lights.size()); ++i) {
        if (!lights[i] || !lights[i]->isActive) continue;
        
        float distToPlayer = lights[i]->distanceTo(playerPosition);
        if (distToPlayer > CULLING_DISTANCE) continue;
        
        if (!lights[i]->isInView(currentViewBounds)) continue;
        
        candidateLights.push_back(i);
    }
    
    std::sort(candidateLights.begin(), candidateLights.end(), 
              [this](int a, int b) {
                  const auto& lightA = lights[a];
                  const auto& lightB = lights[b];
                  
                  if (lightA->priority != lightB->priority) {
                      return lightA->priority > lightB->priority;
                  }
                  
                  float distA = lightA->distanceTo(playerPosition);
                  float distB = lightB->distanceTo(playerPosition);
                  
                  return distA < distB;
              });
    
    int count = std::min(static_cast<int>(candidateLights.size()), MAX_LIGHTS);
    for (int i = 0; i < count; ++i) {
        result.push_back(candidateLights[i]);
    }
    
    return result;
}

void SmartLightSource::clearAllLights() {
    lights.clear();
    activeLightCount = 0;
}

std::vector<int> SmartLightSource::getLightsInRange(const sf::Vector2f& center, float range) const {
    std::vector<int> result;
    
    for (int i = 0; i < static_cast<int>(lights.size()); ++i) {
        if (lights[i] && lights[i]->isActive) {
            float distance = lights[i]->distanceTo(center);
            if (distance <= range) {
                result.push_back(i);
            }
        }
    }
    
    return result;
}