#include "InventorySlot.hpp"

bool InventorySlot::contains(const sf::Vector2f& point) const {
    return slotBox.getGlobalBounds().contains(point);
}

void InventorySlot::draw(sf::RenderTarget& target) const {
    target.draw(slotBox);
    if (item && *item) {
        (*item)->sprite.setPosition(slotBox.getPosition());
        target.draw((*item)->sprite);
    }
}
