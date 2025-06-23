#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "AllItems.hpp"

class InventorySlot {
public:
    sf::RectangleShape slotBox;
    sf::Text           amountText;
    
    std::shared_ptr<ItemData>* item = nullptr;

    InventorySlot(const float& slotSize);

    bool contains(const sf::Vector2f& point) const;

    void draw(sf::RenderTarget& target) const;

    void setPosition(const sf::Vector2f position);

    void updateAmount();
};

class BagSlot : public InventorySlot {
public:
    BagSlot(const float& slotSize);
};

class EquipSlot : public InventorySlot {
public:
    ItemType type;
    EquipSlot(const float& slotSize);
};