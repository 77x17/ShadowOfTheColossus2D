#include "InventoryUI.hpp"

#include <iostream>

#include "Font.hpp"

InventoryUI::InventoryUI(const sf::Vector2f& windowSize) {
    containBox.setSize(sf::Vector2f(600.0f, 348.0f));   // (slotSize + slotPadding) * 15 + 3 * 20 | (slotSize + slotPadding) * 8 + 3 * 20
    containBox.setFillColor(sf::Color(30, 30, 30, 220));
    containBox.setOutlineThickness(2.f);
    containBox.setOutlineColor(sf::Color::White);
    containBox.setOrigin(containBox.getLocalBounds().width / 2.0f, containBox.getLocalBounds().height / 2.0f);
    containBox.setPosition(windowSize / 2.0f); // Vị trí ban đầu
    
    sf::Vector2f startPos = containBox.getGlobalBounds().getPosition() + sf::Vector2f(groupPadding, groupPadding);
    inventoryText.setFont(Font::font);
    inventoryText.setCharacterSize(textSize);
    inventoryText.setFillColor(sf::Color::White);
    inventoryText.setString("Inventory");
    inventoryText.setOrigin(inventoryText.getLocalBounds().left, inventoryText.getLocalBounds().top + inventoryText.getLocalBounds().height);
    inventoryText.setPosition(startPos - sf::Vector2f(0.0f, slotPadding));
    BagSlot bagSlot(slotSize);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            bagSlot.slotBox.setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), y * (slotSize + slotPadding)));
            bagSlots.push_back(bagSlot);
        }
    }

    // ===== TẠO EQUIP SLOTS (8 ô dọc) =====
    std::string equipSlotLabels[] = { "Helmet", "Chestplate", "Leggings", "Boots", "Ring", "Amulet", "Shield", "Weapon" };
    equipLabels.assign(8, sf::Text());
    for (int i = 0; i < 8; ++i) {
        equipLabels[i].setFont(Font::font);
        equipLabels[i].setCharacterSize(textSize);
        equipLabels[i].setFillColor(sf::Color::White);
        equipLabels[i].setString(equipSlotLabels[i]);
    }
    
    ItemType equipSlotTypes[] = { ItemType::Helmet, ItemType::Chestplate, ItemType::Leggings, ItemType::Boots, ItemType::Ring, ItemType::Amulet, ItemType::Shield, ItemType::Weapon };
    sf::Vector2f equipStart = startPos + sf::Vector2f((cols - 1) * (slotSize + slotPadding) + slotSize - slotPadding + groupPadding, 0.0f);
    equipmentText.setFont(Font::font);
    equipmentText.setCharacterSize(textSize);
    equipmentText.setFillColor(sf::Color::White);
    equipmentText.setString("Equipment");
    equipmentText.setOrigin(equipmentText.getLocalBounds().left, equipmentText.getLocalBounds().top + equipmentText.getLocalBounds().height);
    equipmentText.setPosition(equipStart - sf::Vector2f(0.0f, slotPadding));
    EquipSlot equipSlot(slotSize); 
    for (int i = 0; i < 4; ++i) {
        equipSlot.slotBox.setPosition(equipStart + sf::Vector2f(0.0f, i * (slotSize + slotPadding)));
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

    itemInformationText.setFont(Font::font);
    itemInformationText.setCharacterSize(textSize);
    itemInformationText.setFillColor(sf::Color::White);
    itemInformationText.setString("Item information");
    itemInformationText.setOrigin(itemInformationText.getLocalBounds().left, itemInformationText.getLocalBounds().top + itemInformationText.getLocalBounds().height);
    itemInformationText.setPosition(startPos + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - 2 * slotPadding + groupPadding));

    bagItemInformationBox.setSize(sf::Vector2f(5 * (slotSize + slotPadding) - slotPadding, 4 * (slotSize + slotPadding) - slotPadding));
    bagItemInformationBox.setFillColor(sf::Color::Transparent);
    bagItemInformationBox.setOutlineThickness(1.f);
    bagItemInformationBox.setOutlineColor(sf::Color::White);
    bagItemInformationBox.setPosition(startPos + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    selectedBagItemInfomation.setFont(Font::font);
    selectedBagItemInfomation.setCharacterSize(textSize);
    selectedBagItemInfomation.setFillColor(sf::Color::White);
    selectedBagItemInfomation.setPosition(startPos + sf::Vector2f(slotPadding, 4 * (slotSize + slotPadding) + groupPadding));

    equipItemInformationBox.setSize(sf::Vector2f(5 * (slotSize + slotPadding) - slotPadding, 4 * (slotSize + slotPadding) - slotPadding));
    equipItemInformationBox.setFillColor(sf::Color::Transparent);
    equipItemInformationBox.setOutlineThickness(1.f);
    equipItemInformationBox.setOutlineColor(sf::Color::White);
    equipItemInformationBox.setPosition(startPos + sf::Vector2f(5 * (slotSize + slotPadding), 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    selectedEquipItemInfomation.setFont(Font::font);
    selectedEquipItemInfomation.setCharacterSize(textSize);
    selectedEquipItemInfomation.setFillColor(sf::Color::White);
    selectedEquipItemInfomation.setPosition(startPos + sf::Vector2f(5 * (slotSize + slotPadding) + slotPadding, 4 * (slotSize + slotPadding) + groupPadding));

    totalStatsText.setFont(Font::font);
    totalStatsText.setCharacterSize(textSize);
    totalStatsText.setFillColor(sf::Color::White);
    totalStatsText.setString("Total stats");
    totalStatsText.setOrigin(totalStatsText.getLocalBounds().left, totalStatsText.getLocalBounds().top + totalStatsText.getLocalBounds().height);
    totalStatsText.setPosition(equipStart + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - 2 * slotPadding + groupPadding));

    equipmentStatsBox.setSize(sf::Vector2f(5 * (slotSize + slotPadding) - slotPadding, 4 * (slotSize + slotPadding) - slotPadding));
    equipmentStatsBox.setFillColor(sf::Color::Transparent);
    equipmentStatsBox.setOutlineThickness(1.f);
    equipmentStatsBox.setOutlineColor(sf::Color::White);
    equipmentStatsBox.setPosition(equipStart + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    equipmentStats.setFont(Font::font);
    equipmentStats.setCharacterSize(textSize);
    equipmentStats.setFillColor(sf::Color::White);
    equipmentStats.setPosition(equipStart + sf::Vector2f(slotPadding, 4 * (slotSize + slotPadding) + groupPadding));
    updateStats();
}

void InventoryUI::updatePosition(const sf::Vector2f& windowSize) {
    containBox.setPosition(windowSize / 2.0f); // Vị trí ban đầu
    
    sf::Vector2f startPos = containBox.getGlobalBounds().getPosition() + sf::Vector2f(groupPadding, groupPadding);
    inventoryText.setPosition(startPos - sf::Vector2f(0.0f, slotPadding));
    for (int y = 0, index = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x, ++index) {
            bagSlots[index].slotBox.setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), y * (slotSize + slotPadding)));
        }
    }

    // ===== TẠO EQUIP SLOTS (8 ô dọc) =====
    sf::Vector2f equipStart = startPos + sf::Vector2f((cols - 1) * (slotSize + slotPadding) + slotSize + groupPadding, 0.0f);
    equipmentText.setPosition(equipStart - sf::Vector2f(0.0f, slotPadding));
    for (int i = 0; i < 4; ++i) {
        equipSlots[i].slotBox.setPosition(equipStart + sf::Vector2f(0.0f, i * (slotSize + slotPadding)));
        
        equipLabels[i].setPosition(equipStart + sf::Vector2f((slotSize + slotPadding), i * (slotSize + slotPadding)));
    }

    for (int i = 4; i < 8; ++i) {
        equipSlots[i].slotBox.setPosition(equipStart + sf::Vector2f(4 * (slotSize + slotPadding), (i - 4) * (slotSize + slotPadding)));

        equipLabels[i].setPosition(equipStart + sf::Vector2f(4 * (slotSize + slotPadding) - slotPadding, (i - 4) * (slotSize + slotPadding) + slotSize));
    }

    itemInformationText.setPosition(startPos + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - 2 * slotPadding + groupPadding));

    bagItemInformationBox.setPosition(startPos + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    selectedBagItemInfomation.setPosition(startPos + sf::Vector2f(slotPadding, 4 * (slotSize + slotPadding) + groupPadding));

    equipItemInformationBox.setPosition(startPos + sf::Vector2f(5 * (slotSize + slotPadding), 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    selectedEquipItemInfomation.setPosition(startPos + sf::Vector2f(5 * (slotSize + slotPadding) + slotPadding, 4 * (slotSize + slotPadding) + groupPadding));

    totalStatsText.setPosition(equipStart + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - 2 * slotPadding + groupPadding));

    equipmentStatsBox.setPosition(equipStart + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    equipmentStats.setPosition(equipStart + sf::Vector2f(slotPadding, 4 * (slotSize + slotPadding) + groupPadding));
    updateStats();
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

void InventoryUI::updateDrag(sf::Vector2f mousePos) {
    if (!visible) return;

    if (draggedItem) {
        dreggedPos = mousePos;
    }
}

void InventoryUI::updateStats() {
    totalHealth = 0.0f;
    totalDamage = 0.0f;
    for (const auto& slot : equipSlots) if (slot.item) {
        totalHealth += slot.item->getHealth();
        totalDamage += slot.item->getDamage();
    }

    std::string healthString = "Health: " + std::to_string(totalHealth);
    healthString = healthString.substr(0, healthString.find('.') + 3);
    std::string damageString = "Damage: " + std::to_string(totalDamage);
    damageString = damageString.substr(0, damageString.find('.') + 3);
    equipmentStats.setString(healthString + "\n" + damageString);
}

void InventoryUI::handleClick(const sf::Vector2f& mousePos) {
    previousDraggedItemID = -1;

    for (auto& slot : bagSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if (!draggedItem && slot.item) {
                draggedItem = slot.item;
                slot.item   = nullptr;

                selectedBagItemInfomation.setString(draggedItem->getInformation());

                return;
            }
        }
    }

    for (auto& slot : equipSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if (!draggedItem && slot.item) {
                draggedItem = slot.item;
                slot.item   = nullptr;
                
                selectedEquipItemInfomation.setString(draggedItem->getInformation());

                return;
            }
        }
    }
}

void InventoryUI::handleRelease(const sf::Vector2f& mousePos) {
    for (auto& slot : bagSlots) {
        if (slot.contains(mousePos)) {
            if (draggedItem && !slot.item) {
                if (previousDraggedItemID >= static_cast<int>(bagSlots.size())) {
                    selectedBagItemInfomation.setString(draggedItem->getInformation());
                    selectedEquipItemInfomation.setString(std::string());
                }
                
                slot.item   = draggedItem;
                draggedItem = nullptr;

                if (previousDraggedItemID >= static_cast<int>(bagSlots.size())) { 
                    updateStats();
                }

                return;
            }
        }
    }

    for (auto& slot : equipSlots) {
        if (slot.contains(mousePos)) {
            if (draggedItem && !slot.item && slot.type == draggedItem->type) {
                if (previousDraggedItemID < static_cast<int>(bagSlots.size())) {
                    selectedBagItemInfomation.setString(std::string());
                    selectedEquipItemInfomation.setString(draggedItem->getInformation());
                }

                slot.item   = draggedItem;
                draggedItem = nullptr;

                if (previousDraggedItemID < static_cast<int>(bagSlots.size())) { 
                    updateStats();
                }

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

    target.draw(inventoryText);
    for (const auto& slot : bagSlots) { slot.draw(target); }

    target.draw(equipmentText);
    for (const auto& slot : equipSlots) { slot.draw(target); }
    for (const auto& label : equipLabels) { target.draw(label); }

    target.draw(itemInformationText);
    target.draw(bagItemInformationBox);
    target.draw(selectedBagItemInfomation);
    target.draw(equipItemInformationBox);
    target.draw(selectedEquipItemInfomation);
    
    target.draw(totalStatsText);
    target.draw(equipmentStatsBox);
    target.draw(equipmentStats);

    if (draggedItem) {
        sf::Sprite sprite = draggedItem->sprite;
        sprite.setPosition(dreggedPos);
        target.draw(sprite);
    }
}

bool InventoryUI::isVisible() const {
    return visible;
}