#include "QuestNpc.hpp"

QuestNpc::QuestNpc(int newID, const sf::FloatRect& newHitbox, const std::string& name, const std::string& spriteName) 
: Npc(newID, newHitbox, name, spriteName) {}