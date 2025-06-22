#pragma once

#include "InventorySlot.hpp"

#include <vector>

#include "Player.hpp"

class MerchantUI {
private:
    bool visible = false;

    const float slotSize     = 32.0f;
    const float textSize     = 12.0f;
    const float slotPadding  = 4.0f;
    const float groupPadding = 25.0f;
    const int   cols = 10, rows = 4;

    sf::RectangleShape containBox;

    sf::Text inventoryText;
    std::vector<BagSlot> playerBagSlots;

    sf::Text merchantInventoryText;
    std::vector<std::shared_ptr<ItemData>> merchantInventory;
    std::vector<BagSlot> merchantBagSlots;
    
    int previousDraggedItemID = -1;
    std::shared_ptr<ItemData> draggedItem = nullptr;
    sf::Vector2f dreggedPos;

    sf::RectangleShape hoveredItemInfoBox;
    sf::Text hoveredItemInfo;
    sf::Text hoveredItemName;
    sf::Text hoveredItemRarity;

public:
    MerchantUI(const sf::Vector2f& windowSize, Player& player);

    void updatePosition(const sf::Vector2f& windowSize);

    void setVisible(bool m_visible) {
        visible = m_visible;
    }
    
    void updateHover(const sf::Vector2f& mousePos);
    void updateDrag(const sf::Vector2f& mousePos);
    void handleClick(const sf::Vector2f& mousePos);
    void handleRelease(const sf::Vector2f& mousePos, Player& player, std::vector<Item>& items);
    void draw(sf::RenderTarget& target);
    
    bool isVisible() const;
    bool isDrag() const;
    
    void isPayment(Player& player);
};