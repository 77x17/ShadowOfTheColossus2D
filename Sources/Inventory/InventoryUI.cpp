#include "InventoryUI.hpp"

#include <iostream>

#include "Font.hpp"

InventoryUI::InventoryUI(const sf::Vector2f& windowSize) {
    containBox.setSize(sf::Vector2f(600.f, 400.f));
    containBox.setFillColor(sf::Color(30, 30, 30, 220));
    containBox.setOutlineThickness(2.f);
    containBox.setOutlineColor(sf::Color::White);
    containBox.setOrigin(containBox.getLocalBounds().width / 2.0f, containBox.getLocalBounds().height / 2.0f);
    containBox.setPosition(windowSize / 2.0f); // Vị trí ban đầu

    const float slotSize    = 32.0f;
    const float slotPadding = 4.0f;
    const int   cols = 10, rows = 4;
    sf::Vector2f startPos = containBox.getGlobalBounds().getPosition() + sf::Vector2f(20.0f, 20.0f);

    BagSlot bagSlot(slotSize);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            bagSlot.slotBox.setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), y * (slotSize + slotPadding)));
            bagSlots.push_back(bagSlot);
        }
    }

    // ===== TẠO EQUIP SLOTS (8 ô dọc) =====
    equipLabels.assign(8, sf::Text());
    std::string equipSlotLabels[] = { "Helmet", "Chestplate", "Leggings", "Boots", "Ring", "Amulet", "Shield", "Weapon" };
    ItemType equipSlotTypes[] = { ItemType::Helmet, ItemType::Chestplate, ItemType::Leggings, ItemType::Boots, ItemType::Ring, ItemType::Amulet, ItemType::Shield, ItemType::Weapon };
    for (int i = 0; i < 8; ++i) {
        equipLabels[i].setFont(Font::font);
        equipLabels[i].setCharacterSize(12.0f);
        equipLabels[i].setFillColor(sf::Color::White);
        equipLabels[i].setOutlineThickness(1.0f);
        equipLabels[i].setOutlineColor(sf::Color::Black);
        equipLabels[i].setString(equipSlotLabels[i]);
    }

    sf::Vector2f equipStart = startPos + sf::Vector2f(cols * (slotSize + slotPadding) + 20.0f, 0.0f);
    
    EquipSlot equipSlot(slotSize); 
    for (int i = 0; i < 4; ++i) {
        equipSlot.slotBox.setPosition(equipStart + sf::Vector2f(0.f, i * (slotSize + slotPadding)));
        equipSlot.type = equipSlotTypes[i];
        equipSlots.push_back(equipSlot);

        equipLabels[i].setOrigin(0.0f, equipLabels[i].getLocalBounds().top);
        equipLabels[i].setPosition(equipStart + sf::Vector2f((slotSize + slotPadding), i * (slotSize + slotPadding)));
    }

    for (int i = 4; i < 8; ++i) {
        equipSlot.slotBox.setPosition(equipStart + sf::Vector2f(4 * (slotSize + slotPadding), (i - 4) * (slotSize + slotPadding)));
        equipSlot.type = equipSlotTypes[i];
        equipSlots.push_back(equipSlot);

        equipLabels[i].setOrigin(equipLabels[i].getLocalBounds().left + equipLabels[i].getLocalBounds().width,
                                 equipLabels[i].getLocalBounds().top + equipLabels[i].getLocalBounds().height);
        equipLabels[i].setPosition(equipStart + sf::Vector2f(4 * (slotSize + slotPadding) - slotPadding, (i - 4) * (slotSize + slotPadding) + slotSize));
    }
}

bool InventoryUI::addItem(Item* newItem) {
    for (auto& slot : bagSlots) {
        if (!slot.item) {
            slot.item = newItem;
            return true;
        }
    }
    return false; // Hết chỗ
}

void InventoryUI::update(sf::Vector2f mousePos) {
    if (!visible) return;

    if (draggedItem) {
        dreggedPos = mousePos;
    }
}

void InventoryUI::handleClick(const sf::Vector2f& mousePos) {
    previousDraggedItemID = -1;

    for (auto& slot : bagSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if (!draggedItem && slot.item) {
                draggedItem = slot.item;
                slot.item = nullptr;
                return;
            }
        }
    }

    for (auto& slot : equipSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if (!draggedItem && slot.item) {
                draggedItem = slot.item;
                slot.item = nullptr;
                return;
            }
        }
    }
}

void InventoryUI::handleRelease(const sf::Vector2f& mousePos) {
    for (auto& slot : bagSlots) {
        if (slot.contains(mousePos)) {
            if (draggedItem && !slot.item) {
                slot.item = draggedItem;
                draggedItem = nullptr;
                return;
            }
        }
    }

    for (auto& slot : equipSlots) {
        if (slot.contains(mousePos)) {
            if (draggedItem && !slot.item && slot.type == draggedItem->type) {
                slot.item = draggedItem;
                draggedItem = nullptr;
                return;
            }
        }
    }

    if (previousDraggedItemID >= static_cast<int>(bagSlots.size())) {
        previousDraggedItemID -= bagSlots.size();

        for (auto& slot : equipSlots) {
            if (previousDraggedItemID == 0) {
                if (draggedItem && !slot.item && slot.type == draggedItem->type) {
                    slot.item = draggedItem;
                    draggedItem = nullptr;
                    return;
                }
                else if (draggedItem) {
                    std::cerr << "[Bug] - InventoryUI.cpp - handleRelease()\n";
                }
            }

            --previousDraggedItemID;
        }
    } 
    else {
        for (auto& slot : bagSlots) {
            if (previousDraggedItemID == 0) {
                if (draggedItem && !slot.item) {
                    slot.item = draggedItem;
                    draggedItem = nullptr;
                    return;
                }
                else if (draggedItem) {
                    std::cerr << "[Bug] - InventoryUI.cpp - handleRelease()\n";
                }
            }

            --previousDraggedItemID;
        }
    }
}

void InventoryUI::draw(sf::RenderTarget& target) {
    if (!visible) return;

    target.draw(containBox);

    for (const auto& slot : bagSlots) { slot.draw(target); }
    for (const auto& slot : equipSlots) { slot.draw(target); }
    for (const auto& label : equipLabels) { target.draw(label); }

    if (draggedItem) {
        sf::Sprite sprite = draggedItem->sprite;
        sprite.setPosition(dreggedPos);
        target.draw(sprite);
    }
}

bool InventoryUI::isVisible() const {
    return visible;
}