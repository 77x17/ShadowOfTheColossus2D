#include "Animation.hpp"

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
    if (totalFrames == 0) {
        std::cerr << "Bug nay lam toi mat 5 tieng cuoc doi\n";
        return;
    }
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

void Animation::draw(sf::RenderTarget& target, sf::Shader* shader) const {
    target.draw(sprite, shader);
}

void Animation::drawWithShader(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

sf::Sprite Animation::getSprite() const {
    return sprite;
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
    if (state != currentState && animations.find(state) != animations.end()) {
        currentState = state;
        animations[currentState].reset();
    }
}

void AnimationManager::setState(int state, bool notReset) {
    if (state != currentState) {
        currentState = state;
    }
}

void AnimationManager::setPosition(sf::Vector2f position) {
    animations[currentState].setPosition(position);
}

void AnimationManager::update() {
    if (animations.find(currentState) != animations.end()) {
        animations[currentState].update();
    }
}

void AnimationManager::draw(sf::RenderTarget& target, sf::Shader* shader) const {
    animations.at(currentState).draw(target, shader);
}

void AnimationManager::drawWithShader(sf::RenderTarget& target,  sf::RenderStates states) const {
    animations.at(currentState).drawWithShader(target, states);
}

sf::Sprite AnimationManager::getSprite() const {
    return animations.at(currentState).getSprite();
}