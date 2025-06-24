#pragma once

#include "Npc.hpp"

#include "QuestNpc.hpp"
#include "MerchantNpc.hpp"

#include "TileMap.hpp"
#include "Player.hpp"

class NpcManager {
private:
    std::vector<std::unique_ptr<Npc>> npcs;

public:
    void loadNpcs(const TileMap& map);

    void handlePlayerInteraction(Player& player);

    void update(const float& dt);

    void draw(sf::RenderTarget& target) const;

    void drawInteractText(sf::RenderTarget& target) const;

    bool isInteractWithMerchant(bool inventoryVisible);
};