#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <iostream>

class TextureManager {
private:
    // Dùng map để lưu trữ texture, với key là một ID (string) và value là đối tượng texture
    static std::unordered_map<std::string, sf::Texture> textures;

public:
    // Tải một texture từ file và lưu bằng một ID
    static void load(const std::string& id, const std::string& path);

    // Lấy một tham chiếu hằng (const reference) đến texture đã được tải
    static const sf::Texture& get(const std::string& id);
};

class Animation {
private:
    sf::Sprite sprite;

    int frameWidth;
    int frameHeight;
    int currentFrame;
    int totalFrames;
    int layer;

    float     frameTime;
    sf::Clock clock;

public:
    Animation() = default;
    Animation(const sf::Texture& texture, int frameW, int frameH, int totalF, int frameLayer, float frameT, bool flip);

    void update();
    void reset();
    void setPosition(sf::Vector2f position);

    void draw(sf::RenderWindow& window) const;
};

class AnimationManager {
private:
    std::unordered_map<int, Animation> animations;
    int currentState;
public:
    AnimationManager();

    void addAnimation(int state, const sf::Texture& texture, int frameW, int frameH, int totalF, int frameLayer, float frameT, bool flip);
    void setState(int state);
    void setState(int state, bool notReset);
    void setPosition(sf::Vector2f position);
    void update();

    void draw(sf::RenderWindow& window) const;
};