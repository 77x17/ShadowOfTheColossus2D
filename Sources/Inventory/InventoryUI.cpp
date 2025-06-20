#include "InventoryUI.hpp"

#include <iostream>

#include "Font.hpp"
#include "Player.hpp"

InventoryUI::InventoryUI(const sf::Vector2f& windowSize, Player& player) {
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
    
    bagSlots = player.getBagSlots();
    if (bagSlots == nullptr || static_cast<int>((*bagSlots).size()) != rows * cols) {
        std::cerr << "[Bug] - InventoryUI.cpp - Constructor\n";
    }
    for (int y = 0, index = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x, ++index) {
            (*bagSlots)[index].init(slotSize);
            (*bagSlots)[index].slotBox.setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), y * (slotSize + slotPadding)));
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
    
    equipSlots = player.getEquipSlots();
    if (equipSlots == nullptr || (*equipSlots).size() != 8) {
        std::cerr << "[Bug] - InventoryUI.cpp - Constructor\n";
    }
    for (int i = 0; i < 4; ++i) {
        (*equipSlots)[i].init(slotSize);
        (*equipSlots)[i].slotBox.setPosition(equipStart + sf::Vector2f(0.0f, i * (slotSize + slotPadding)));
        (*equipSlots)[i].type = equipSlotTypes[i];

        equipLabels[i].setOrigin(0.0f, equipLabels[i].getLocalBounds().top);
        equipLabels[i].setPosition(equipStart + sf::Vector2f((slotSize + slotPadding), i * (slotSize + slotPadding)));
    }

    for (int i = 4; i < 8; ++i) {
        (*equipSlots)[i].init(slotSize);
        (*equipSlots)[i].slotBox.setPosition(equipStart + sf::Vector2f(4 * (slotSize + slotPadding), (i - 4) * (slotSize + slotPadding)));
        (*equipSlots)[i].type = equipSlotTypes[i];

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
    player.updateEquipmentStats();

    hoveredItemInfo.setFont(Font::font);
    hoveredItemInfo.setCharacterSize(textSize);
    hoveredItemInfo.setFillColor(sf::Color::White);
    hoveredItemInfo.setOutlineThickness(1.0f);
    hoveredItemInfo.setOutlineColor(sf::Color::Black);
}

void InventoryUI::updatePosition(const sf::Vector2f& windowSize) {
    containBox.setPosition(windowSize / 2.0f); // Vị trí ban đầu
    
    sf::Vector2f startPos = containBox.getGlobalBounds().getPosition() + sf::Vector2f(groupPadding, groupPadding);
    inventoryText.setPosition(startPos - sf::Vector2f(0.0f, slotPadding));
    for (int y = 0, index = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x, ++index) {
            (*bagSlots)[index].slotBox.setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), y * (slotSize + slotPadding)));
        }
    }

    // ===== TẠO EQUIP SLOTS (8 ô dọc) =====
    sf::Vector2f equipStart = startPos + sf::Vector2f((cols - 1) * (slotSize + slotPadding) + slotSize + groupPadding, 0.0f);
    equipmentText.setPosition(equipStart - sf::Vector2f(0.0f, slotPadding));
    for (int i = 0; i < 4; ++i) {
        (*equipSlots)[i].slotBox.setPosition(equipStart + sf::Vector2f(0.0f, i * (slotSize + slotPadding)));
        
        equipLabels[i].setPosition(equipStart + sf::Vector2f((slotSize + slotPadding), i * (slotSize + slotPadding)));
    }

    for (int i = 4; i < 8; ++i) {
        (*equipSlots)[i].slotBox.setPosition(equipStart + sf::Vector2f(4 * (slotSize + slotPadding), (i - 4) * (slotSize + slotPadding)));

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
}

void InventoryUI::updateHover(const sf::Vector2f& mousePos) {
    hoveredItemInfo.setString(std::string());
    for (auto& slot : *bagSlots) {
        if (slot.contains(mousePos) && slot.item) {
            hoveredItemInfo.setString(slot.item->name);
            hoveredItemInfo.setPosition(mousePos);
            hoveredItemInfo.setOrigin(hoveredItemInfo.getLocalBounds().left + hoveredItemInfo.getLocalBounds().width, hoveredItemInfo.getLocalBounds().top + hoveredItemInfo.getLocalBounds().height);
            
            return;
        }
    }
    for (auto& slot : *equipSlots) {
        if (slot.contains(mousePos) && slot.item) {
            hoveredItemInfo.setString(slot.item->name);
            hoveredItemInfo.setPosition(mousePos);
            hoveredItemInfo.setOrigin(hoveredItemInfo.getLocalBounds().left + hoveredItemInfo.getLocalBounds().width, hoveredItemInfo.getLocalBounds().top + hoveredItemInfo.getLocalBounds().height);
            
            return;
        }
    }
}

void InventoryUI::updateDrag(const sf::Vector2f& mousePos) {
    if (!visible) return;

    hoveredItemInfo.setString(std::string());
    if (draggedItem) {
        dreggedPos = mousePos;
    }
}

void InventoryUI::handleClick(const sf::Vector2f& mousePos) {
    previousDraggedItemID = -1;

    for (auto& slot : *bagSlots) {
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

    for (auto& slot : *equipSlots) {
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

void InventoryUI::handleRelease(const sf::Vector2f& mousePos, Player& player, std::vector<Item>& items) {
    for (auto& slot : *bagSlots) {
        if (slot.contains(mousePos)) {
            if (draggedItem && !slot.item) {
                if (previousDraggedItemID >= static_cast<int>((*bagSlots).size())) {
                    selectedBagItemInfomation.setString(draggedItem->getInformation());
                    selectedEquipItemInfomation.setString(std::string());
                }
                
                slot.item   = draggedItem;
                draggedItem = nullptr;

                if (previousDraggedItemID >= static_cast<int>((*bagSlots).size())) { 
                    player.updateEquipmentStats();
                }

                return;
            }
        }
    }

    for (auto& slot : *equipSlots) {
        if (slot.contains(mousePos)) {
            if (draggedItem && !slot.item && slot.type == draggedItem->type) {
                if (previousDraggedItemID < static_cast<int>((*bagSlots).size())) {
                    selectedBagItemInfomation.setString(std::string());
                    selectedEquipItemInfomation.setString(draggedItem->getInformation());
                }

                slot.item   = draggedItem;
                draggedItem = nullptr;

                if (previousDraggedItemID < static_cast<int>((*bagSlots).size())) { 
                    player.updateEquipmentStats();
                }

                return;
            }
        }
    }

    if (containBox.getGlobalBounds().contains(mousePos)) {
        if (previousDraggedItemID >= static_cast<int>((*bagSlots).size())) {
            previousDraggedItemID -= static_cast<int>((*bagSlots).size());

            for (auto& slot : *equipSlots) {
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
            for (auto& slot : *bagSlots) {
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
    else if (draggedItem) {
        if (player.dropItem(draggedItem, items)) {
            draggedItem = nullptr;
            if (previousDraggedItemID < static_cast<int>((*bagSlots).size())) {
                selectedBagItemInfomation.setString(std::string());
            }
            else {
                selectedEquipItemInfomation.setString(std::string());
            }
        }
        else {
            if (previousDraggedItemID >= static_cast<int>((*bagSlots).size())) {
                previousDraggedItemID -= static_cast<int>((*bagSlots).size());

                for (auto& slot : *equipSlots) {
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
                for (auto& slot : *bagSlots) {
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
    }
}

void InventoryUI::draw(sf::RenderTarget& target) {
    if (!visible) return;

    target.draw(containBox);

    target.draw(inventoryText);
    for (const auto& slot : *bagSlots) { slot.draw(target); }

    target.draw(equipmentText);
    for (const auto& slot : *equipSlots) { slot.draw(target); }
    for (const auto& label : equipLabels) { target.draw(label); }

    target.draw(itemInformationText);
    target.draw(bagItemInformationBox);
    target.draw(selectedBagItemInfomation);
    target.draw(equipItemInformationBox);
    target.draw(selectedEquipItemInfomation);
    
    target.draw(totalStatsText);
    target.draw(equipmentStatsBox);
    target.draw(equipmentStats);

    target.draw(hoveredItemInfo);

    if (draggedItem) {
        sf::Sprite sprite = draggedItem->sprite;
        sprite.setPosition(dreggedPos);
        target.draw(sprite);
    }
}

bool InventoryUI::isVisible() const {
    return visible;
}

void InventoryUI::updateStats(Player &player) {
    equipmentStats.setString(player.getStats());
}