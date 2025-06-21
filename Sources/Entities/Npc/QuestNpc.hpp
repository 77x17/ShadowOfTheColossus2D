#pragma once

#include "Npc.hpp"

class QuestNpc : public Npc {
private:
    int      ID;
    float    FADE_SPEED;
    float    interactTextOpacity;
    float    previousInteractTextOpacity;
    sf::Text interactText;
    
public:
    QuestNpc(int newID, const sf::FloatRect& newHitbox, const std::string& name, const std::string& spriteName);

    void update(const float& dt) override;

    void drawInteractQuest(sf::RenderTarget& target) const;

    void interactWithPlayer(Player& player) override;
};