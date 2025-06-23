#include "InventorySlot.hpp"

#include "Font.hpp"

InventorySlot::InventorySlot(const float& slotSize) {
    slotBox.setSize(sf::Vector2f(slotSize, slotSize));
    slotBox.setOutlineThickness(1.0f);
    slotBox.setOutlineColor(sf::Color::White);

    amountText.setFont(Font::font);
    amountText.setStyle(sf::Text::Bold);
    amountText.setCharacterSize(10.0f);
    amountText.setFillColor(sf::Color::White);
}

void InventorySlot::setPosition(const sf::Vector2f position) {
    slotBox.setPosition(position);
    amountText.setPosition(position + slotBox.getSize() - sf::Vector2f(2.0f, 2.0f));
    if (item && *item) {
        amountText.setString(std::to_string((*item)->amount));
        amountText.setOrigin(amountText.getLocalBounds().left + amountText.getLocalBounds().width, amountText.getLocalBounds().top + amountText.getLocalBounds().height);
    }
    else {
        amountText.setString(std::string());
    }
}

bool InventorySlot::contains(const sf::Vector2f& point) const {
    return slotBox.getGlobalBounds().contains(point);
}

void InventorySlot::draw(sf::RenderTarget& target) const {
    target.draw(slotBox);
    if (item && *item) {
        (*item)->sprite.setPosition(slotBox.getPosition());
        target.draw((*item)->sprite);
    }
    target.draw(amountText);
}

void InventorySlot::updateAmount() {
    if (item && *item) {
        amountText.setString(std::to_string((*item)->amount));
        amountText.setOrigin(amountText.getLocalBounds().left + amountText.getLocalBounds().width, amountText.getLocalBounds().top + amountText.getLocalBounds().height);
    }
    else {
        amountText.setString(std::string());
    }
}

BagSlot::BagSlot(const float& slotSize) : InventorySlot(slotSize) {
    slotBox.setFillColor(sf::Color(100, 100, 100));
}

EquipSlot::EquipSlot(const float& slotSize) : InventorySlot(slotSize) {
    slotBox.setFillColor(sf::Color(80, 80, 80));
}
