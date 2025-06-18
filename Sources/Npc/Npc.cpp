#include "Npc.hpp"

#include "Font.hpp"
#include <iostream>

Npc::Npc(int _ID, const sf::FloatRect& _hitbox, const std::string& name, const std::string& spriteName) {
    ID     = _ID;
    hitbox = _hitbox;

    size     = hitbox.getSize();
    position = hitbox.getPosition();

    {
        TEXT_SIZE          = 10;
        BACKGROUND_PADDING = 5.0f;
        label.setFont(Font::font);
        label.setString(name);
        label.setCharacterSize(TEXT_SIZE);
        label.setFillColor(sf::Color::White);
        label.setOutlineThickness(1.0f);
        label.setOutlineColor(sf::Color::Black);
        label.setPosition(position + sf::Vector2f(size.x / 2, 0));

        labelBackground.setFillColor(sf::Color(60, 60, 60, 139));
        labelBackground.setPosition(label.getPosition());
        
        // Adding BACKGROUND_PADDING two side | + BACKGROUND_PADDING / 2
        sf::FloatRect bounds = label.getLocalBounds();
        labelBackground.setSize(bounds.getSize() + sf::Vector2f(BACKGROUND_PADDING, BACKGROUND_PADDING));
        // Adding extra space with sprites
        label.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2 + 10.0f);
        // -2.0f for the font spacing
        labelBackground.setOrigin(label.getOrigin() + sf::Vector2f(BACKGROUND_PADDING / 2, -2.0f + BACKGROUND_PADDING / 2));
    }

    {
        animationManager.addAnimation(0, TextureManager::get(spriteName), 16, 16, 4, 0, 0.5f, false);

        shadow = Animation(TextureManager::get("playerShadow"), 13, 5, 1, 0, 0.f, false);
    }
}

int Npc::getID() const {
    return ID;
}

sf::FloatRect Npc::getHitbox() const {
    return hitbox;
}

void Npc::update() {
    animationManager.setState(0, true);
    animationManager.setPosition(position + sf::Vector2f(0, -4));
    animationManager.update();

    shadow.setPosition(position + sf::Vector2f(0, size.y - 8));
}

void Npc::draw(sf::RenderTarget& target) {
    target.draw(labelBackground);
    target.draw(label);

    // target.draw(hitbox);
    
    shadow.draw(target);

    animationManager.draw(target);
}