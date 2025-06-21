#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "ItemData.hpp"

class InventorySlot {
public:
    sf::RectangleShape slotBox;
    std::shared_ptr<ItemData>* item = nullptr;

    bool contains(const sf::Vector2f& point) const;

    void draw(sf::RenderTarget& target) const;

};

class BagSlot : public InventorySlot {
public:
    void init(const float& slotSize) {
        slotBox.setSize(sf::Vector2f(slotSize, slotSize));
        slotBox.setFillColor(sf::Color(100, 100, 100));
        slotBox.setOutlineThickness(1.f);
        slotBox.setOutlineColor(sf::Color::White);
    }
};

class EquipSlot : public InventorySlot {
public:
    void init(const float& slotSize) {
        slotBox.setSize(sf::Vector2f(slotSize, slotSize));
        slotBox.setFillColor(sf::Color(80, 80, 80));
        slotBox.setOutlineThickness(1.f);
        slotBox.setOutlineColor(sf::Color::Yellow);
    }

    ItemType type;
};