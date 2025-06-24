#pragma once

#include <SFML/Graphics.hpp>

#include "ItemData.hpp"

class Item {
private:
    sf::FloatRect hitbox;
    sf::Sprite    sprite;

    std::shared_ptr<ItemData> item;

    int                TEXT_SIZE;
    float              BACKGROUND_PADDING;
    sf::Text           label;
    sf::RectangleShape labelBackground;

    float TIMEOUT_TIME;
    float timeoutTimer;

    float PICKUP_DELAY_TIME;
    float pickupDelayTimer;
public:
    Item(const sf::Vector2f& position, const std::shared_ptr<ItemData>& _item);

    void update(const float& dt);

    void draw(sf::RenderTarget& target) const;

    sf::FloatRect getHitbox() const;

    bool canPickup() const;

    const std::shared_ptr<ItemData>& getItem() const;

};