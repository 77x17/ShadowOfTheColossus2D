#include "MerchantUI.hpp"

#include <iostream>

#include "Font.hpp"
#include "Player.hpp"

MerchantUI::MerchantUI(const sf::Vector2f& windowSize, Player& player) {
    containBox.setSize(sf::Vector2f(400.0f, 348.0f));   // (slotSize + slotPadding) * 10 + 2 * 20 | (slotSize + slotPadding) * 8 + 3 * 20
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
    inventoryText.setOrigin(inventoryText.getLocalBounds().left, 
                            inventoryText.getLocalBounds().top + inventoryText.getLocalBounds().height);
    inventoryText.setPosition(startPos - sf::Vector2f(0.0f, slotPadding));
    
    std::vector<std::shared_ptr<ItemData>>* inventory = player.getInventory();
    if (inventory == nullptr || static_cast<int>((*inventory).size()) != rows * cols) {
        std::cerr << "[Bug] - InventoryUI.cpp - Constructor\n";
    }
    BagSlot bagSlot;
    bagSlot.init(slotSize);
    for (int y = 0, index = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x, ++index) {
            bagSlot.item = &(*inventory)[index];
            bagSlot.slotBox.setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), y * (slotSize + slotPadding)));
            
            playerBagSlots.push_back(bagSlot);
        }
    }

    merchantInventoryText.setFont(Font::font);
    merchantInventoryText.setCharacterSize(textSize);
    merchantInventoryText.setFillColor(sf::Color::White);
    merchantInventoryText.setString("Merchant Inventory");
    merchantInventoryText.setOrigin(merchantInventoryText.getLocalBounds().left, 
                                    merchantInventoryText.getLocalBounds().top + merchantInventoryText.getLocalBounds().height);
    merchantInventoryText.setPosition(startPos + sf::Vector2f(0.0f, 4 * (slotSize + slotPadding) - 2 * slotPadding + groupPadding));

    merchantInventory.assign(rows * cols, nullptr);
    for (int y = 0, index = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x, ++index) {
            bagSlot.item = &merchantInventory[index];
            bagSlot.slotBox.setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), (y + 4) * (slotSize + slotPadding)) + sf::Vector2f(0.0f, -slotPadding + groupPadding));
            
            merchantBagSlots.push_back(bagSlot);
        }
    }

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
}

void MerchantUI::updatePosition(const sf::Vector2f& windowSize) {
    containBox.setPosition(windowSize / 2.0f); // Vị trí ban đầu
    
    sf::Vector2f startPos = containBox.getGlobalBounds().getPosition() + sf::Vector2f(groupPadding, groupPadding);
    inventoryText.setPosition(startPos - sf::Vector2f(0.0f, slotPadding));
    for (int y = 0, index = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x, ++index) {
            playerBagSlots[index].slotBox.setPosition(startPos + sf::Vector2f(x * (slotSize + slotPadding), y * (slotSize + slotPadding)));
        }
    }
}

void MerchantUI::updateHover(const sf::Vector2f& mousePos) {
    hoveredItemInfo.setString(std::string());
    hoveredItemName.setString(std::string());
    hoveredItemRarity.setString(std::string());
    for (auto& slot : playerBagSlots) {
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
    for (auto& slot : merchantBagSlots) {
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

void MerchantUI::updateDrag(const sf::Vector2f& mousePos) {
    if (!visible) return;

    hoveredItemInfo.setString(std::string());
    hoveredItemName.setString(std::string());
    hoveredItemRarity.setString(std::string());
    if (draggedItem) {
        dreggedPos = mousePos;
    }
}

void MerchantUI::handleClick(const sf::Vector2f& mousePos) {
    previousDraggedItemID = -1;

    for (auto& slot : playerBagSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if (!draggedItem && (*slot.item)) {
                draggedItem  = (*slot.item);
                (*slot.item) = nullptr;

                return;
            }
        }
    }

    for (auto& slot : merchantBagSlots) {
        ++previousDraggedItemID;

        if (slot.contains(mousePos)) {
            if (!draggedItem && (*slot.item)) {
                draggedItem  = (*slot.item);
                (*slot.item) = nullptr;

                return;
            }
        }
    }
}

void MerchantUI::handleRelease(const sf::Vector2f& mousePos, Player& player, std::vector<Item>& items) {
    for (auto& slot : playerBagSlots) {
        if (slot.contains(mousePos)) {
            if (draggedItem && !(*slot.item)) {
                (*slot.item) = draggedItem;
                draggedItem  = nullptr;

                return;
            }
        }
    }

    for (auto& slot : merchantBagSlots) {
        if (slot.contains(mousePos)) {
            if (draggedItem && !(*slot.item)) {
                (*slot.item) = draggedItem;
                draggedItem  = nullptr;

                return;
            }
        }
    }

    if (containBox.getGlobalBounds().contains(mousePos)) {
        if (previousDraggedItemID >= static_cast<int>((playerBagSlots).size())) {
            previousDraggedItemID -= static_cast<int>((playerBagSlots).size());

            for (auto& slot : merchantBagSlots) {
                if (previousDraggedItemID == 0) {
                    if (draggedItem && !(*slot.item)) {
                        (*slot.item) = draggedItem;
                        draggedItem  = nullptr;

                        return;
                    }
                    else if (draggedItem) {
                        std::cerr << "[Bug] - MerchantUI.cpp - handleRelease()\n";
                    }
                }
                --previousDraggedItemID;
            }
            // std::cerr << "[Bug] - MerchantUI.cpp - handleRelease()\n";
        } 
        else {
            for (auto& slot : playerBagSlots) {
                if (previousDraggedItemID == 0) {
                    if (draggedItem && !(*slot.item)) {
                        (*slot.item) = draggedItem;
                        draggedItem  = nullptr;
                        return;
                    }
                    else if (draggedItem) {
                        for (auto& o_slot : playerBagSlots) {
                            if (draggedItem && !(*o_slot.item)) {
                                (*o_slot.item) = draggedItem;
                                draggedItem    = nullptr;
                                return;
                            }
                        }
                        
                        if (player.dropItem(draggedItem, items)) {
                            draggedItem = nullptr;
                        }
                        else {
                            std::cerr << "[Bug] - MerchantUI.cpp - handleRelease()\n";
                        }
                    }
                }
                --previousDraggedItemID;
            }
            // std::cerr << "[Bug] - MerchantUI.cpp - handleRelease()\n";
        }
    }
    else if (draggedItem) {
        if (player.dropItem(draggedItem, items)) {
            draggedItem = nullptr;
        }
        else {
            if (previousDraggedItemID >= static_cast<int>((playerBagSlots).size())) {
                previousDraggedItemID -= static_cast<int>((playerBagSlots).size());

                for (auto& slot : merchantBagSlots) {
                    if (previousDraggedItemID == 0) {
                        if (draggedItem && !(*slot.item)) {
                            (*slot.item) = draggedItem;
                            draggedItem  = nullptr;
                            return;
                        } else if (draggedItem) {
                            for (auto& o_slot : merchantBagSlots) {
                                if (draggedItem && !(*o_slot.item)) {
                                    (*o_slot.item) = draggedItem;
                                    draggedItem    = nullptr;
                                    return;
                                }
                            }
                            
                            if (player.dropItem(draggedItem, items)) {
                                draggedItem = nullptr;
                            }
                            else {
                                std::cerr << "[Bug] - MerchantUI.cpp - handleRelease()\n";
                            }
                        }
                    }
                    --previousDraggedItemID;
                }
                previousDraggedItemID += static_cast<int>((playerBagSlots).size());

                // std::cerr << "[Bug] - MerchantUI.cpp - handleRelease()\n";
            } 
            else {
                for (auto& slot : playerBagSlots) {
                    if (previousDraggedItemID == 0) {
                        if (draggedItem && !(*slot.item)) {
                            (*slot.item) = draggedItem;
                            draggedItem  = nullptr;
                            return;
                        }
                        else if (draggedItem) {
                            for (auto& o_slot : playerBagSlots) {
                                if (draggedItem && !(*o_slot.item)) {
                                    (*o_slot.item) = draggedItem;
                                    draggedItem    = nullptr;
                                    return;
                                }
                            }
                            
                            if (player.dropItem(draggedItem, items)) {
                                draggedItem = nullptr;
                            }
                            else {
                                std::cerr << "[Bug] - MerchantUI.cpp - handleRelease()\n";
                            }
                        }
                    }
                    --previousDraggedItemID;
                }
                // std::cerr << "[Bug] - MerchantUI.cpp - handleRelease()\n";
            }
        }
    }
}

void MerchantUI::draw(sf::RenderTarget& target) {
    if (!visible) return;

    target.draw(containBox);

    target.draw(inventoryText);
    for (const auto& slot : playerBagSlots) { slot.draw(target); }

    target.draw(merchantInventoryText);
    for (const auto& slot : merchantBagSlots) { slot.draw(target); }

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
    }
}

bool MerchantUI::isVisible() const {
    return visible;
}

bool MerchantUI::isDrag() const {
    return draggedItem != nullptr;
}

void MerchantUI::isPayment(Player& player) {
    for (std::shared_ptr<ItemData>& item : merchantInventory) {
        if (item != nullptr) {
            item          = nullptr;
            player.golds += 10.0f;

            SoundManager::playSound("payment");
        }
    }
}