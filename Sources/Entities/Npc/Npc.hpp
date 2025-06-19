#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "AnimationManager.hpp"
#include "TextureManager.hpp"

class Npc {
private:
    int           ID;
    sf::FloatRect hitbox;

    int                TEXT_SIZE;
    float              BACKGROUND_PADDING;
    sf::Text           label;
    sf::RectangleShape labelBackground;
    
    AnimationManager   animationManager;
    Animation          shadow;

public:
    Npc(int                  _id, 
        const sf::FloatRect& _hitbox, 
        const std::string&   name, 
        const std::string&   spriteName);

    int getID() const;
    
    void update();
    
    void draw(sf::RenderTarget& target);
    
    sf::FloatRect getHitbox() const;
} ;