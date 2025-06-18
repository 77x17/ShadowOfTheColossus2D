#pragma once

#include "InventorySlot.hpp"

#include <vector>

class InventoryUI {
private:
    bool visible = false;

    std::vector<BagSlot> bagSlots;
    std::vector<EquipSlot> equipSlots;

    int previousDraggedItemID = -1;
    Item* draggedItem         = nullptr;
    sf::Vector2f dreggedPos;

    sf::RectangleShape containBox;
    std::vector<sf::Text> equipLabels;

public:
    InventoryUI(const sf::Vector2f& windowSize);

    void toggle() {
        visible = !visible;
    }

    bool addItem(Item* newItem);
    
    void update(sf::Vector2f mousePos);
    void handleClick(const sf::Vector2f& mousePos);
    void handleRelease(const sf::Vector2f& mousePos);
    void draw(sf::RenderTarget& target);
    
    bool isVisible() const;

};