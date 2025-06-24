#include "Npc.hpp"

#include "Font.hpp"
#include "TextureManager.hpp"

#include <iostream>

Npc::Npc(const sf::FloatRect& newHitbox, 
         const std::string&   name, 
         const std::string&   spriteName) 
: hitbox(newHitbox) {
    // --- [Begin] - Nametag --- 
    TEXT_SIZE          = 10;
    BACKGROUND_PADDING = 5.0f;
    label.setFont(Font::font);
    label.setString(name);
    label.setCharacterSize(TEXT_SIZE);
    label.setFillColor(sf::Color::White);
    label.setOutlineThickness(1.0f);
    label.setOutlineColor(sf::Color::Black);
    label.setPosition(hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x / 2, 0));

    labelBackground.setFillColor(sf::Color(60, 60, 60, 139));
    labelBackground.setPosition(label.getPosition());
    
    sf::FloatRect bounds = label.getLocalBounds();
    labelBackground.setSize(bounds.getSize() + sf::Vector2f(BACKGROUND_PADDING, BACKGROUND_PADDING));
    label.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2 + 10.0f);
    labelBackground.setOrigin(label.getOrigin() + sf::Vector2f(BACKGROUND_PADDING / 2, -2.0f + BACKGROUND_PADDING / 2));
    // --- [End] - Nametag --- 
    
    // --- [Begin] - Interact text --- 
    FADE_SPEED                  = 5.0f;
    interactTextOpacity         = 0.0f;
    previousInteractTextOpacity = 0.0f;
    
    interactText.setFont(Font::font);
    interactText.setCharacterSize(12.5f);
    interactText.setOutlineThickness(2.0f);
    interactText.setFillColor(sf::Color(255, 255, 255, interactTextOpacity));
    interactText.setOutlineColor(sf::Color(0, 0, 0, interactTextOpacity));
    interactText.setString("Press [F] to talk");
    interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width / 2, 
                           interactText.getLocalBounds().top  + interactText.getLocalBounds().height / 2);
    interactText.setPosition(hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x / 2, -hitbox.getSize().y));
    // --- [End] ---

    // --- [Begin] - Animation --- 
    animationManager.addAnimation(0, TextureManager::get(spriteName), 16, 16, 4, 0, 0.5f, false);
    
    shadow.setTexture(TextureManager::get("playerShadow"));
    // --- [Begin] - Animation --- 

    collisionWithPlayer = false;
}

sf::FloatRect Npc::getHitbox() const {
    return hitbox;
}

void Npc::update(const float& dt) {
    animationManager.setState(0, true);
    animationManager.setPosition(hitbox.getPosition() + sf::Vector2f(0, -4));
    animationManager.update();

    shadow.setPosition(hitbox.getPosition() + sf::Vector2f(0, hitbox.getPosition().y - 8));

    if (interactCooldownTimer > 0) {
        interactCooldownTimer -= dt;
    }
}

void Npc::draw(sf::RenderTarget& target) const {
    target.draw(labelBackground);
    target.draw(label);

    // sf::RectangleShape hitboxShape;
    // hitboxShape.setPosition(hitbox.getPosition());
    // hitboxShape.setSize(hitbox.getSize());
    // hitboxShape.setOutlineColor(sf::Color::Green);
    // hitboxShape.setOutlineThickness(1.f);
    // hitboxShape.setFillColor(sf::Color::Transparent);
    // target.draw(hitboxShape);
    
    target.draw(shadow);

    animationManager.draw(target);
}

void Npc::drawInteractText(sf::RenderTarget& target) const {
    target.draw(interactText);
}
