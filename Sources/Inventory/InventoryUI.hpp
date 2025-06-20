#pragma once

#include "InventorySlot.hpp"

#include <vector>

#include "Player.hpp"

class InventoryUI {
private:
    bool visible = false;

    const float slotSize     = 32.0f;
    const float textSize     = 12.0f;
    const float slotPadding  = 4.0f;
    const float groupPadding = 25.0f;
    const int   cols = 10, rows = 4;

    sf::RectangleShape containBox;

    sf::Text inventoryText;
    std::vector<BagSlot>* bagSlots = nullptr;
    
    sf::Text equipmentText;
    std::vector<sf::Text> equipLabels;
    std::vector<EquipSlot>* equipSlots = nullptr;
    
    int previousDraggedItemID = -1;
    std::shared_ptr<ItemData> draggedItem = nullptr;
    sf::Vector2f dreggedPos;

    sf::Text itemInformationText;
    sf::RectangleShape bagItemInformationBox;
    sf::RectangleShape equipItemInformationBox;
    sf::Text selectedBagItemInfomation;
    sf::Text selectedEquipItemInfomation;

    sf::Text totalStatsText;
    float totalDamage;
    float totalHealth;
    sf::RectangleShape equipmentStatsBox;
    sf::Text equipmentStats;

    sf::Text hoveredItemInfo;

public:
    InventoryUI(const sf::Vector2f& windowSize, Player& player);

    void updatePosition(const sf::Vector2f& windowSize);

    void toggle() {
        visible = !visible;
    }
    
    void updateHover(const sf::Vector2f& mousePos);
    void updateDrag(const sf::Vector2f& mousePos);
    void handleClick(const sf::Vector2f& mousePos);
    void handleRelease(const sf::Vector2f& mousePos, Player& player, std::vector<Item>& items);
    void draw(sf::RenderTarget& target);
    
    bool isVisible() const;
    
    void updateStats(Player &player);
};