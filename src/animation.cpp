#include "animation.hpp"

Animation::Animation(const sf::Texture& texture, int frameW, int frameH, int totalF, int frameLayer, float frameT, bool flip = false) 
    : frameWidth(frameW), frameHeight(frameH), currentFrame(0), totalFrames(totalF), layer(frameLayer), frameTime(frameT) {

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, layer * frameHeight, frameWidth, frameHeight));
    if (!flip) {
        sprite.setScale(2.f, 2.f);
        sprite.setOrigin(0, 0);
        
    }
    else {
        sprite.setScale(-2.f, 2.f);
        sprite.setOrigin(frameWidth, 0);
    }
} 

void Animation::update() {
    if (clock.getElapsedTime().asSeconds() > frameTime) {
        currentFrame = (currentFrame + 1) % totalFrames;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, layer * frameHeight, frameWidth, frameHeight));
        clock.restart();
    }
}

void Animation::reset() {
    currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, layer * frameHeight, frameWidth, frameHeight));
    clock.restart();
}

void Animation::setPosition(sf::Vector2f position) {
    sprite.setPosition(position);
}

void Animation::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

AnimationManager::AnimationManager() : currentState(0) {
    // nothing
}

void AnimationManager::addAnimation(int state, const sf::Texture& texture, int frameW, int frameH, int totalF, int frameLayer, float frameT, bool flip = false) {
    // Dùng emplace để xây dựng Animation trực tiếp trong map.
    // Không tạo biến tạm thời, không sao chép.
    animations.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(state),
        std::forward_as_tuple(texture, frameW, frameH, totalF, frameLayer, frameT, flip)
    );
}

void AnimationManager::setState(int state) {
    if (state != currentState) {
        currentState = state;
        animations[currentState].reset();
    }
}

void AnimationManager::setPosition(sf::Vector2f position) {
    animations[currentState].setPosition(position);
}

void AnimationManager::update() {
    animations[currentState].update();
}

void AnimationManager::draw(sf::RenderWindow& window) const {
    animations.at(currentState).draw(window);
}