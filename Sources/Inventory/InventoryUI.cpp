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
    
    // --- [Begin] - Inventory ---
    sf::Vector2f startPos = containBox.getGlobalBounds().getPosition() + sf::Vector2f(groupPadding, groupPadding);
    inventoryText.setFont(Font::font);
    inventoryText.setCharacterSize(textSize);
    inventoryText.setFillColor(sf::Color::White);
    inventoryText.setString("Inventory");
    inventoryText.setOrigin(inventoryText.getLocalBounds().left, inventoryText.getLocalBounds().top + inventoryText.getLocalBounds().height);
    inventoryText.setPosition(startPos - sf::Vector2f(0.0f, slotPadding));
    
    std::vector<std::shared_ptr<ItemData>>* inventory = player.getInventory();
    if (inventory == nullptr || static_cast<int>((*inventory).size()) != rows * cols) {
        std::cerr << "[Bug] - InventoryUI.cpp - Constructor\n";
    }
    BagSlot bagSlot(slotSize);
    for (int y = 0, index = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x, ++index) {
            bagSlot.item = &(*inventory)[index];
            bagSlot.setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), y * (slotSize + slotPadding)));
            
            bagSlots.push_back(bagSlot);
        }
    }
    // --- [End] ---

    // --- [Begin] - Equipment ---
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
    
    std::vector<std::shared_ptr<ItemData>>* equipment = player.getEquipment();
    if (equipment == nullptr || (*equipment).size() != 8) {
        std::cerr << "[Bug] - InventoryUI.cpp - Constructor\n";
    }
    EquipSlot equipSlot(slotSize);
    for (int i = 0; i < 4; ++i) {
        equipSlot.item = &(*equipment)[i];
        equipSlot.setPosition(equipStart + sf::Vector2f(0.0f, i * (slotSize + slotPadding)));
        equipSlot.type = equipSlotTypes[i];
        
        equipSlots.push_back(equipSlot);


        equipLabels[i].setOrigin(0.0f, equipLabels[i].getLocalBounds().top);
        equipLabels[i].setPosition(equipStart + sf::Vector2f((slotSize + slotPadding), i * (slotSize + slotPadding)));
    }

    for (int i = 4; i < 8; ++i) {
        equipSlot.item = &(*equipment)[i];
        equipSlot.setPosition(equipStart + sf::Vector2f(4 * (slotSize + slotPadding), (i - 4) * (slotSize + slotPadding)));
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

    // selectedBagItemInfomation.setFont(Font::font);
    // selectedBagItemInfomation.setCharacterSize(textSize);
    // selectedBagItemInfomation.setFillColor(sf::Color::White);
    // selectedBagItemInfomation.setPosition(startPos + sf::Vector2f(slotPadding, 4 * (slotSize + slotPadding) + groupPadding));

    equipItemInformationBox.setSize(sf::Vector2f(5 * (slotSize + slotPadding) - slotPadding, 4 * (slotSize + slotPadding) - slotPadding));
    equipItemInformationBox.setFillColor(sf::Color::Transparent);
    equipItemInformationBox.setOutlineThickness(1.f);
    equipItemInformationBox.setOutlineColor(sf::Color::White);
    equipItemInformationBox.setPosition(startPos + sf::Vector2f(5 * (slotSize + slotPadding), 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    // selectedEquipItemInfomation.setFont(Font::font);
    // selectedEquipItemInfomation.setCharacterSize(textSize);
    // selectedEquipItemInfomation.setFillColor(sf::Color::White);
    // selectedEquipItemInfomation.setPosition(startPos + sf::Vector2f(5 * (slotSize + slotPadding) + slotPadding, 4 * (slotSize + slotPadding) + groupPadding));

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

    hoveredItemInfoBox.setFillColor(sf::Color(0, 0, 0, 160));
    hoveredItemInfoBox.setOutlineThickness(1.f);
    hoveredItemInfoBox.setOutlineColor(sf::Color::White);
    
    hoveredItemInfo.setFont(Font::font);
    hoveredItemInfo.setCharacterSize(textSize);
    hoveredItemInfo.setFillColor(sf::Color::White);
    hoveredItemInfo.setOutlineThickness(1.0f);
    hoveredItemInfo.setOutlineColor(sf::Color::Black);

    hoveredItemName.setFont(Font::font);
    hoveredItemName.setCharacterSize(textSize);
    hoveredItemName.setOutlineThickness(1.0f);
    hoveredItemName.setOutlineColor(sf::Color::Black);

    hoveredItemRarity.setFont(Font::font);
    hoveredItemRarity.setCharacterSize(textSize);
    hoveredItemRarity.setOutlineThickness(1.0f);
    hoveredItemRarity.setOutlineColor(sf::Color::Black);

    amountText.setFont(Font::font);
    amountText.setStyle(sf::Text::Bold);
    amountText.setCharacterSize(10.0f);
    amountText.setFillColor(sf::Color::White);
}

void InventoryUI::updatePosition(const sf::Vector2f& windowSize) {
    containBox.setPosition(windowSize / 2.0f); // Vị trí ban đầu
    
    sf::Vector2f startPos = containBox.getGlobalBounds().getPosition() + sf::Vector2f(groupPadding, groupPadding);
    inventoryText.setPosition(startPos - sf::Vector2f(0.0f, slotPadding));
    for (int y = 0, index = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x, ++index) {
            bagSlots[index].setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), y * (slotSize + slotPadding)));
        }
    }

    // ===== TẠO EQUIP SLOTS (8 ô dọc) =====
    sf::Vector2f equipStart = startPos + sf::Vector2f((cols - 1) * (slotSize + slotPadding) + slotSize + groupPadding, 0.0f);
    equipmentText.setPosition(equipStart - sf::Vector2f(0.0f, slotPadding));
    for (int i = 0; i < 4; ++i) {
        equipSlots[i].setPosition(equipStart + sf::Vector2f(0.0f, i * (slotSize + slotPadding)));
        
        equipLabels[i].setPosition(equipStart + sf::Vector2f((slotSize + slotPadding), i * (slotSize + slotPadding)));
    }

    for (int i = 4; i < 8; ++i) {
        equipSlots[i].setPosition(equipStart + sf::Vector2f(4 * (slotSize + slotPadding), (i - 4) * (slotSize + slotPadding)));

        equipLabels[i].setPosition(equipStart + sf::Vector2f(4 * (slotSize + slotPadding) - slotPadding, (i - 4) * (slotSize + slotPadding) + slotSize));
    }

    itemInformationText.setPosition(startPos + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - 2 * slotPadding + groupPadding));

    bagItemInformationBox.setPosition(startPos + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    // selectedBagItemInfomation.setPosition(startPos + sf::Vector2f(slotPadding, 4 * (slotSize + slotPadding) + groupPadding));

    equipItemInformationBox.setPosition(startPos + sf::Vector2f(5 * (slotSize + slotPadding), 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    // selectedEquipItemInfomation.setPosition(startPos + sf::Vector2f(5 * (slotSize + slotPadding) + slotPadding, 4 * (slotSize + slotPadding) + groupPadding));

    totalStatsText.setPosition(equipStart + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - 2 * slotPadding + groupPadding));

    equipmentStatsBox.setPosition(equipStart + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - slotPadding + groupPadding));

    equipmentStats.setPosition(equipStart + sf::Vector2f(slotPadding, 4 * (slotSize + slotPadding) + groupPadding));
}

void InventoryUI::updateHover(const sf::Vector2f& mousePos) {
    hoveredItemInfo.setString(std::string());
    hoveredItemName.setString(std::string());
    hoveredItemRarity.setString(std::string());
    for (auto& slot : bagSlots) {
        if (slot.contains(mousePos) && (*slot.item)) {
            hoveredItemInfo.setString((*slot.item)->getInformation());
            hoveredItemInfo.setPosition(mousePos);
            hoveredItemInfo.setOrigin(hoveredItemInfo.getLocalBounds().left + hoveredItemInfo.getLocalBounds().width, hoveredItemInfo.getLocalBounds().top + hoveredItemInfo.getLocalBounds().height);
            
            hoveredItemName.setString((*slot.item)->name);
            hoveredItemName.setPosition(hoveredItemInfo.getPosition() + sf::Vector2f(hoveredItemInfo.getLocalBounds().getSize().x / 2.0f, 0.0f));
            hoveredItemName.setOrigin(hoveredItemInfo.getOrigin() + sf::Vector2f(hoveredItemName.getLocalBounds().left + hoveredItemName.getLocalBounds().width / 2.0f, 0.0f));
            hoveredItemName.setFillColor((*slot.item)->getItemRarityColor());
            
            hoveredItemRarity.setString((*slot.item)->getItemRarityString());
            hoveredItemRarity.setPosition(hoveredItemInfo.getPosition() + sf::Vector2f(hoveredItemInfo.getLocalBounds().getSize().x, hoveredItemInfo.getLocalBounds().getSize().y));
            hoveredItemRarity.setOrigin(hoveredItemInfo.getOrigin() + sf::Vector2f(hoveredItemRarity.getLocalBounds().left + hoveredItemRarity.getLocalBounds().width, hoveredItemRarity.getLocalBounds().top + hoveredItemRarity.getLocalBounds().height));
            hoveredItemRarity.setFillColor((*slot.item)->getItemRarityColor());

            hoveredItemInfoBox.setSize(hoveredItemInfo.getLocalBounds().getSize() + sf::Vector2f(slotPadding, slotPadding) * 2.0f);
            hoveredItemInfoBox.setPosition(hoveredItemInfo.getPosition() - sf::Vector2f(slotPadding, slotPadding));           
            hoveredItemInfoBox.setOrigin(hoveredItemInfo.getOrigin());
            return; 
        }
    }
    for (auto& slot : equipSlots) {
        if (slot.contains(mousePos) && (*slot.item)) {
            hoveredItemInfo.setString((*slot.item)->getInformation());
            hoveredItemInfo.setPosition(mousePos);
            hoveredItemInfo.setOrigin(hoveredItemInfo.getLocalBounds().left + hoveredItemInfo.getLocalBounds().width, hoveredItemInfo.getLocalBounds().top + hoveredItemInfo.getLocalBounds().height);
            
            hoveredItemName.setString((*slot.item)->name);
            hoveredItemName.setPosition(hoveredItemInfo.getPosition() + sf::Vector2f(hoveredItemInfo.getLocalBounds().getSize().x / 2.0f, 0.0f));
            hoveredItemName.setOrigin(hoveredItemInfo.getOrigin() + sf::Vector2f(hoveredItemName.getLocalBounds().left + hoveredItemName.getLocalBounds().width / 2.0f, 0.0f));
            hoveredItemName.setFillColor((*slot.item)->getItemRarityColor());
            
            hoveredItemRarity.setString((*slot.item)->getItemRarityString());
            hoveredItemRarity.setPosition(hoveredItemInfo.getPosition() + sf::Vector2f(hoveredItemInfo.getLocalBounds().getSize().x, hoveredItemInfo.getLocalBounds().getSize().y));
            hoveredItemRarity.setOrigin(hoveredItemInfo.getOrigin() + sf::Vector2f(hoveredItemRarity.getLocalBounds().left + hoveredItemRarity.getLocalBounds().width, hoveredItemRarity.getLocalBounds().top + hoveredItemRarity.getLocalBounds().height));
            hoveredItemRarity.setFillColor((*slot.item)->getItemRarityColor());

            hoveredItemInfoBox.setSize(hoveredItemInfo.getLocalBounds().getSize() + sf::Vector2f(slotPadding, slotPadding) * 2.0f);
            hoveredItemInfoBox.setPosition(hoveredItemInfo.getPosition() - sf::Vector2f(slotPadding, slotPadding));           
            hoveredItemInfoBox.setOrigin(hoveredItemInfo.getOrigin());

            return;
        }
    }
}

void InventoryUI::updateDrag(const sf::Vector2f& mousePos) {
    if (!visible) return;

    hoveredItemName.setString(std::string());
    hoveredItemRarity.setString(std::string());
    hoveredItemInfo.setString(std::string());
    if (draggedItem) {
        dreggedPos = mousePos;

        amountText.setPosition(mousePos + sf::Vector2f(slotSize - 2.0f, slotSize - 2.0f));
        amountText.setString(std::to_string(draggedItem->amount));
        amountText.setOrigin(amountText.getLocalBounds().left + amountText.getLocalBounds().width, amountText.getLocalBounds().top + amountText.getLocalBounds().height);
    }
    else {
        amountText.setString(std::string());
    }
}

void InventoryUI::handleClick(const sf::Vector2f& mousePos) {
    if (draggedItem) {
        // std::cerr << "[Bug] - InventoryUI.cpp - handleClick()\n";
        return;
    }

    previousDraggedItemID = -1;

    for (auto& slot : bagSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if ((*slot.item)) {
                draggedItem  = (*slot.item);
                (*slot.item) = nullptr;

                return;
            }
        }
    }

    for (auto& slot : equipSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if ((*slot.item)) {
                draggedItem  = (*slot.item);
                (*slot.item) = nullptr;
                
                return;
            }
        }
    }
}

void InventoryUI::handleRightClick(const sf::Vector2f& mousePos) {
    if (draggedItem) {
        // std::cerr << "[Bug] - InventoryUI.cpp - handleClick()\n";
        return;
    }

    previousDraggedItemID = -1;

    for (auto& slot : bagSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if ((*slot.item)) {
                draggedItem         = (*slot.item)->clone();
                draggedItem->amount = (draggedItem->amount + 1) / 2;
                
                (*slot.item)->amount -= draggedItem->amount;
                if ((*slot.item)->amount == 0) {
                    (*slot.item) = nullptr;
                }

                return;
            }
        }
    }

    for (auto& slot : equipSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if ((*slot.item)) {
                draggedItem         = (*slot.item)->clone();
                draggedItem->amount = (draggedItem->amount + 1) / 2;
                
                (*slot.item)->amount -= draggedItem->amount;
                if ((*slot.item)->amount == 0) {
                    (*slot.item) = nullptr;
                }
                
                return;
            }
        }
    }
}

void InventoryUI::handleRelease(const sf::Vector2f& mousePos, Player& player, std::vector<Item>& items) {
    if (draggedItem == nullptr) {
        // std::cerr << "[Bug] - InventoryUI.cpp - handleRelease()\n";
        return;
    }

    for (auto& slot : bagSlots) {
        if (slot.contains(mousePos)) {
            if (!(*slot.item)) {    // Empty thì đặt vào
                (*slot.item) = draggedItem;
                draggedItem  = nullptr;

                if (previousDraggedItemID >= static_cast<int>((bagSlots).size())) { 
                    player.updateEquipmentStats();
                }

                return;
            }
            else if ((*slot.item)->name == draggedItem->name) { // Cùng loại item thì gộp vào
                (*slot.item)->amount += draggedItem->amount;
                draggedItem = nullptr;

                if (previousDraggedItemID >= static_cast<int>((bagSlots).size())) { 
                    player.updateEquipmentStats();
                }

                return;
            }
        }
    }

    for (auto& slot : equipSlots) {
        if (slot.contains(mousePos)) {
            if (!(*slot.item)) {    // Empty thì đặt vào
                if (slot.type != draggedItem->type) {
                    SoundManager::playSound("blockEquipItem");
                    continue;
                }
                if (const auto& equipItem = dynamic_cast<EquipItem*>(draggedItem.get())) {
                    if (equipItem->levelRequired > player.getLevel()) {
                        SoundManager::playSound("blockEquipItem");
                        continue;
                    }
                }

                (*slot.item) = draggedItem;
                draggedItem  = nullptr;

                if (previousDraggedItemID < static_cast<int>((bagSlots).size())) { 
                    player.updateEquipmentStats();
                }

                SoundManager::playSound("equipItem");

                return;
            }
            else if ((*slot.item)->name == draggedItem->name) { // Cùng loại item thì gộp vào
                (*slot.item)->amount += draggedItem->amount;
                draggedItem = nullptr;

                if (previousDraggedItemID < static_cast<int>((bagSlots).size())) { 
                    player.updateEquipmentStats();
                }

                SoundManager::playSound("equipItem");

                return;
            }
        }
    }

    if (containBox.getGlobalBounds().contains(mousePos)) {
        if (previousDraggedItemID >= static_cast<int>((bagSlots).size())) {
            previousDraggedItemID -= static_cast<int>((bagSlots).size());

            for (auto& slot : equipSlots) {
                if (previousDraggedItemID == 0) {
                    if (!(*slot.item) && slot.type == draggedItem->type) {
                        if (const auto& equipItem = dynamic_cast<EquipItem*>(draggedItem.get())) {
                            if (equipItem->levelRequired > player.getLevel()) continue;
                        }

                        (*slot.item) = draggedItem;
                        draggedItem  = nullptr;
                        return;
                    }
                    else {
                        std::cerr << "[Bug] - InventoryUI.cpp - handleRelease()\n";
                    }
                }
                --previousDraggedItemID;
            }
            // std::cerr << "[Bug] - Inventory.cpp - handleRelease()\n";
        } 
        else {
            for (auto& slot : bagSlots) {
                if (previousDraggedItemID == 0) {
                    if (!(*slot.item)) {    // Empty thì đặt vào
                        (*slot.item) = draggedItem;
                        draggedItem  = nullptr;

                        return;
                    }
                    else if ((*slot.item)->name == draggedItem->name) { // Cùng loại item thì gộp vào
                        (*slot.item)->amount += draggedItem->amount;
                        draggedItem = nullptr;

                        return;
                    }
                    else {  // Duyệt xem có ô nào chung loại item hoặc trống thì đặt vào
                        for (auto& o_slot : bagSlots) {
                            if (!(*o_slot.item)) {  // Trống
                                (*o_slot.item) = draggedItem;
                                draggedItem    = nullptr;

                                return;
                            }
                            else if ((*o_slot.item)->name == draggedItem->name) {   // Chung loại
                                (*o_slot.item)->amount += draggedItem->amount;
                                draggedItem = nullptr;

                                return;
                            }
                        }
                        
                        if (player.dropItem(draggedItem, items)) {  // Nếu full thì drop
                            draggedItem = nullptr;
                        }
                        else {
                            std::cerr << "[Bug] - InventoryUI.cpp - handleRelease()\n";
                        }
                    }
                }
                --previousDraggedItemID;
            }
            // std::cerr << "[Bug] - Inventory.cpp - handleRelease()\n";
        }
    }
    else if (player.dropItem(draggedItem, items)) {
        draggedItem = nullptr;
    }
    else {
        std::cerr << "[Bug] - InventoryUI.cpp - handleRelease()\n";
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
    // target.draw(selectedBagItemInfomation);
    target.draw(equipItemInformationBox);
    // target.draw(selectedEquipItemInfomation);
    
    target.draw(totalStatsText);
    target.draw(equipmentStatsBox);
    target.draw(equipmentStats);

    if (hoveredItemInfo.getString() != std::string()) {
        target.draw(hoveredItemInfoBox);
        target.draw(hoveredItemInfo);
        target.draw(hoveredItemName);
        target.draw(hoveredItemRarity);
    }

    if (draggedItem) {
        sf::Sprite sprite = draggedItem->sprite;
        sprite.setPosition(dreggedPos);
        target.draw(sprite);

        if (draggedItem->amount >= 2) {
            target.draw(amountText);
        }
    }
}

bool InventoryUI::isVisible() const {
    return visible;
}

bool InventoryUI::isDrag() const {
    return draggedItem != nullptr;
}

void InventoryUI::updateStats(Player &player) {
    equipmentStats.setString(player.getStats());
}

void InventoryUI::updateAmount() {
    for (auto& slot : bagSlots) {
        slot.updateAmount();
    }
    
    for (auto& slot : equipSlots) {
        slot.updateAmount();
    }
}