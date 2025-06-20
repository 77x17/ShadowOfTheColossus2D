#pragma once

#include "Npc.hpp"

class QuestNpc : public Npc {
public:
    QuestNpc(int newID, const sf::FloatRect& newHitbox, const std::string& name, const std::string& spriteName);

};