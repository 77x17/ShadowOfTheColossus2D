#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Player.hpp"

#include "EnemyManager.hpp"

class BossAltar {
private:
    int ID;
    sf::FloatRect hitbox;

    std::vector<std::pair<std::string, int>> requiredItems;

    float INTERACT_COOLDOWN     = 1.0f;
    float interactCooldownTimer = 0.0f;

    float    FADE_SPEED;
    float    interactTextOpacity;
    float    previousInteractTextOpacity;
    sf::Text interactText;

    bool suitableForSummonBoss = false;
public:
    bool collisionWithPlayer = false;
    
    BossAltar(const int& m_ID, const sf::FloatRect& m_hitbox);
    
    void update(const float& dt);
    
    void draw(sf::RenderTarget& target) const;
    
    void drawInteractText(sf::RenderTarget& target) const;
    
    sf::FloatRect getHitbox() const;

    void interactWithPlayer(Player& player);

    bool isSuitableForSummonBoss() const;

    void summonBoss(EnemyManager& enemies);

};