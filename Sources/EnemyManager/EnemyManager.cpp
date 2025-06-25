#include "EnemyManager.hpp"

#include "Player.hpp"

#include "Constants.hpp"
#include "ItemDatabase.hpp"

void EnemyManager::loadEnemies(const std::unordered_map<std::string, std::vector<sf::FloatRect>>& enemyRects) {
    // Mythic	  = 0.0001  - 1 / 10000
    // Legendary  = 0.001   - 1 / 1000
    // Rare	      = 0.01    - 1 / 100
    // Unique     = 0.03    - 1 / 20
    // Normal     = 0.05    - 1 / 20

    std::vector<std::pair<float, std::shared_ptr<ItemData>>> batInventory;
    batInventory.emplace_back(0.05, ItemDatabase::get("Old Bow"));
    batInventory.emplace_back(0.05, ItemDatabase::get("Old Helmet"));
    batInventory.emplace_back(0.05, ItemDatabase::get("Old Chestplate"));
    batInventory.emplace_back(0.05, ItemDatabase::get("Old Leggings"));
    batInventory.emplace_back(0.05, ItemDatabase::get("Old Boots"));
    batInventory.emplace_back(0.01, ItemDatabase::get("Bat Orb"));
    
    std::vector<std::pair<float, std::shared_ptr<ItemData>>> eyeInventory;
    eyeInventory.emplace_back(0.03, ItemDatabase::get("Wooden Bow"));
    eyeInventory.emplace_back(0.03, ItemDatabase::get("Wooden Helmet"));
    eyeInventory.emplace_back(0.03, ItemDatabase::get("Wooden Chestplate"));
    eyeInventory.emplace_back(0.03, ItemDatabase::get("Wooden Leggings"));
    eyeInventory.emplace_back(0.03, ItemDatabase::get("Wooden Boots"));
    eyeInventory.emplace_back(0.01, ItemDatabase::get("Eye Orb"));

    for (const auto& pair : enemyRects) {
        for (const sf::FloatRect& rect : pair.second) {
            if (pair.first == "Bat Lv.1") {
                enemies.push_back(std::make_unique<Bat>(rect.getPosition(), pair.first,  5.0f, batInventory));
            }
            else if (pair.first == "Vampire Bat Lv.3") {
                enemies.push_back(std::make_unique<VampireBat>(rect.getPosition(), pair.first, 10.0f, batInventory));
            }
            else if (pair.first == "Eye Lv.5") {
                enemies.push_back(std::make_unique<Eye>(rect.getPosition(), pair.first, 20.0f, eyeInventory));
            }
            else if (pair.first == "Slime Lv.5") {
                enemies.push_back(std::make_unique<Slime>(rect.getPosition(), pair.first, 20.0f, eyeInventory));
            }
            else {
                std::cerr << "[Bug] - Main.cpp - loadEnemies()\n";
            }
        }
    }
}

void EnemyManager::summonEnemy(std::unique_ptr<Enemy> enemy) {
    enemies.push_back(std::move(enemy));
}

void EnemyManager::update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects, ItemManager& items) {
    for (auto& enemy : enemies) if (enemy->calculateDistance(player) <= LOADING_DISTANCE) {
        enemy->update(dt, player, collisionRects, items);
    }
    // Enemies despawn
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if ((*it)->isDespawn()) {
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}

const std::vector<std::unique_ptr<Enemy>>& EnemyManager::getEnemies() const {
    return enemies;
}

void EnemyManager::draw(sf::RenderTarget& target, const Player& player) const {
    for (auto& enemy : enemies) if (enemy->calculateDistance(player) <= LOADING_DISTANCE) {
        enemy->draw(target);
    }
}