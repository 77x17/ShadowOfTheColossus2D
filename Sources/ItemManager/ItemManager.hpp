#pragma once

#include "Item.hpp"

class Player;

class ItemManager {
private:
    std::vector<Item> items;

public:
    void addItem(const sf::Vector2f& position, const std::shared_ptr<ItemData>& item);

    const std::vector<Item>& getItems();

    void handlePlayerCollision(Player& player);

    void update(const float& dt);

    void draw(sf::RenderTarget& target) const;
};