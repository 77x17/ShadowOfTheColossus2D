#pragma once

#include "Enemy.hpp"

class Player;

#include "Bat.hpp"
#include "Eye.hpp"

#include "BatBoss.hpp"

class EnemyManager {
private:
    std::vector<std::unique_ptr<Enemy>> enemies;

public:
    void loadEnemies(const std::unordered_map<std::string, std::vector<sf::FloatRect>>& enemyRects);

    void summonEnemy(std::unique_ptr<Enemy> enemy);

    void update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects, ItemManager& items);

    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;

    void draw(sf::RenderTarget& target, const Player& player) const;
};