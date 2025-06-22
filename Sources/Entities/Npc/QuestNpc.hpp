#pragma once

#include "Npc.hpp"

class QuestNpc : public Npc {
private:
    int      ID;
    
public:
    QuestNpc(int newID, const sf::FloatRect& newHitbox, const std::string& name, const std::string& spriteName);

    void update(const float& dt) override;

    void interactWithPlayer(Player& player) override;
};