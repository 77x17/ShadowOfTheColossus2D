#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <iostream>

enum class PlayerState {
    IDLE_LEFT,
    IDLE_RIGHT,
    IDLE_UP_LEFT,
    IDLE_UP_RIGHT,
    IDLE_DOWN,
    WALK_LEFT,
    WALK_RIGHT,
    WALK_UP_LEFT,
    WALK_UP_RIGHT,
    WALK_DOWN,
    DASH
};

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
    std::unordered_map<PlayerState, Animation> animations;
    PlayerState currentState;
public:
    AnimationManager();

    void addAnimation(PlayerState state, const sf::Texture& texture, int frameW, int frameH, int totalF, int frameLayer, float frameT, bool flip);
    void setState(PlayerState state);
    void setPosition(sf::Vector2f position);
    void update();

    void draw(sf::RenderWindow& window) const;
};