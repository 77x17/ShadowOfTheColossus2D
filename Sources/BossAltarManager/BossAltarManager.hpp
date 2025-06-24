#pragma once

#include "BossAltar.hpp"

#include <vector>
#include <unordered_map>

class Player;
class EnemyManager;

class BossAltarManager {
private:
    std::vector<BossAltar> bossAltars;

public:
    void loadBossAltars(const std::unordered_map<int, sf::FloatRect>& bossAltarRects);

    void handlePlayerInteraction(Player& player);

    void update(const float& dt, EnemyManager& enemies);

    void drawInteractText(sf::RenderTarget& target) const;
};