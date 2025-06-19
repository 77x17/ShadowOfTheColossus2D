#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include <cmath>

// Struct để lưu thông tin một nguồn sáng
struct LightSource {
    sf::Vector2f position;      // Vị trí world coordinates
    float radius;               // Bán kính ánh sáng
    sf::Vector3f color;         // Màu sắc ánh sáng (RGB)
    float intensity;            // Cường độ sáng (0.0 - 1.0)
    bool isActive;              // Có đang hoạt động không
    int priority;               // Độ ưu tiên (số càng cao càng quan trọng)
    
    // Constructor
    LightSource(const sf::Vector2f& pos = sf::Vector2f(0, 0), 
                float r = 500.0f, 
                const sf::Vector3f& col = sf::Vector3f(1.0f, 1.0f, 1.0f),
                float inten = 1.0f,
                int prio = 0)
        : position(pos), radius(r), color(col), intensity(inten), isActive(true), priority(prio) {}
    
    // Tính khoảng cách đến một điểm
    float distanceTo(const sf::Vector2f& point) const {
        return std::sqrt(std::pow(position.x - point.x, 2) + std::pow(position.y - point.y, 2));
    }
    
    // Kiểm tra có nằm trong view không
    bool isInView(const sf::FloatRect& viewBounds) const {
        // Mở rộng view bounds với radius để đảm bảo ánh sáng ở edge cũng được tính
        sf::FloatRect expandedBounds(
            viewBounds.left - radius,
            viewBounds.top - radius,
            viewBounds.width + 2 * radius,
            viewBounds.height + 2 * radius
        );
        return expandedBounds.contains(position);
    }
};

class SmartLightSource {
private:
    std::vector<std::unique_ptr<LightSource>> lights;
    std::unique_ptr<sf::Shader> lightShader;
    
    // Cấu hình hệ thống
    static constexpr int MAX_LIGHTS = 8;              // Số lượng light tối đa render cùng lúc
    static constexpr float CULLING_DISTANCE = 700.0f; // Khoảng cách tối đa để xem xét
    
    // Shader uniforms cho multiple lights
    mutable std::vector<sf::Vector2f> lightPositions;
    mutable std::vector<float> lightRadii;
    mutable std::vector<sf::Vector3f> lightColors;
    mutable std::vector<float> lightIntensities;
    mutable int activeLightCount;
    
    // View và player info
    sf::Vector2f playerPosition;
    sf::FloatRect currentViewBounds;
    sf::Vector2f viewSize;
    
public:
    SmartLightSource();
    ~SmartLightSource() = default;
    
    // Khởi tạo và quản lý shader
    bool loadShader(const std::string& fragmentPath);
    
    // Quản lý lights
    int addLight(const sf::Vector2f& position, 
                 float radius = 100.0f, 
                 const sf::Vector3f& color = sf::Vector3f(1.0f, 1.0f, 1.0f),
                 float intensity = 1.0f,
                 int priority = 0);
    
    void removeLight(int lightID);
    void updateLight(int lightID, const sf::Vector2f& position);
    void setLightActive(int lightID, bool active);
    void setLightIntensity(int lightID, float intensity);
    void setLightColor(int lightID, const sf::Vector3f& color);
    void setLightPriority(int lightID, int priority);
    
    // Cập nhật hệ thống
    void update(const sf::Vector2f& playerPos, const sf::View& view);
    
    // Render
    sf::Shader* getShader() const;
    bool shouldApplyShader() const;
    
    // Thông tin debug
    int getTotalLights() const { return lights.size(); }
    int getActiveLights() const { return activeLightCount; }
    std::vector<int> getCurrentlyRenderedLights() const;
    
    // Utility functions
    void clearAllLights();
    std::vector<int> getLightsInRange(const sf::Vector2f& center, float range) const;
    
private:
    // Hàm nội bộ để tính toán lights quan trọng nhất
    void calculateImportantLights();
    void updateShaderUniforms() const;
    
    // Hàm so sánh để sắp xếp lights theo độ quan trọng
    struct LightComparator {
        sf::Vector2f playerPos;
        sf::FloatRect viewBounds;
        
        LightComparator(const sf::Vector2f& playerPosition, const sf::FloatRect& bounds)
            : playerPos(playerPosition), viewBounds(bounds) {}
        
        bool operator()(const std::unique_ptr<LightSource>& a, const std::unique_ptr<LightSource>& b) const;
    };
};