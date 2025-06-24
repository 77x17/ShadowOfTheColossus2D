#include "ItemManager.hpp"

#include "Player.hpp"

void ItemManager::addItem(const sf::Vector2f& position, const std::shared_ptr<ItemData>& item) {
    items.emplace_back(position, item);
}

const std::vector<Item>& ItemManager::getItems() {
    return items;
}

void ItemManager::handlePlayerCollision(Player& player) {
    for (auto it = items.begin(); it != items.end(); ) {
        if (it->canPickup() && player.isCollision(it->getHitbox())) {
            if (player.addItem(it->getItem())) {
                it = items.erase(it); 
            }
            else {
                ++it;
            }
        } else {
            ++it; 
        }
    }
}

void ItemManager::update(const float& dt) {
    for (Item& item : items) {
        item.update(dt);
    }
}

void ItemManager::draw(sf::RenderTarget& target) const {
    for (auto& item : items) {
        item.draw(target);
    }
}