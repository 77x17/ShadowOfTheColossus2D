#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "AnimationManager.hpp"
#include "TextureManager.hpp"
#include "Player.hpp"

class Npc {
protected:
    sf::FloatRect hitbox;

    int                TEXT_SIZE;
    float              BACKGROUND_PADDING;
    sf::Text           label;
    sf::RectangleShape labelBackground;
    
    float    FADE_SPEED;
    float    interactTextOpacity;
    float    previousInteractTextOpacity;
    sf::Text interactText;

    AnimationManager   animationManager;
    sf::Sprite         shadow;

    float INTERACT_COOLDOWN = 0.5f;
    float interactCooldownTimer = 0.0f;
public:
    bool collisionWithPlayer;

    Npc(const sf::FloatRect& _hitbox, const std::string& name, const std::string& spriteName);
    
    virtual void update(const float& dt);
    
    void draw(sf::RenderTarget& target) const;

    void drawInteractText(sf::RenderTarget& target) const;
    
    sf::FloatRect getHitbox() const;

    virtual void interactWithPlayer(Player& player) = 0;
};