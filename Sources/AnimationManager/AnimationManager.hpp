#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <iostream>

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

    void draw(sf::RenderTarget& target, sf::Shader* shader = nullptr) const;
    void drawWithShader(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    sf::Sprite& getSprite();
    const sf::Sprite& getSpriteConst() const;
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

    void draw(sf::RenderTarget& target, sf::Shader* shader = nullptr) const;
    void drawWithShader(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    sf::Sprite getSprite() const;
};