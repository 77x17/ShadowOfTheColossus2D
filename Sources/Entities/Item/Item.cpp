#include "Item.hpp"

#include "Constants.hpp"
#include "Font.hpp"
#include "EntityEffects.hpp"

Item::Item(const sf::Vector2f& position, const std::shared_ptr<ItemData>& _item) 
: hitbox(position, sf::Vector2f(TILE_SIZE, TILE_SIZE) / 2.0f), item(_item) {
    // --- [Begin] - Nametag --- 
    TEXT_SIZE          = 10;
    BACKGROUND_PADDING = 5.0f;
    label.setFont(Font::font);
    label.setString(item->name);
    label.setCharacterSize(TEXT_SIZE);
    label.setFillColor(sf::Color::White);
    label.setOutlineThickness(1.0f);
    label.setOutlineColor(sf::Color::Black);
    label.setPosition(hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x / 2, 0));

    labelBackground.setFillColor(sf::Color(60, 60, 60, 139));
    labelBackground.setPosition(label.getPosition());
    
    sf::FloatRect labelBounds = label.getLocalBounds();
    labelBackground.setSize(labelBounds.getSize() + sf::Vector2f(BACKGROUND_PADDING, BACKGROUND_PADDING));
    label.setOrigin(labelBounds.left + labelBounds.width / 2, labelBounds.top + labelBounds.height / 2 + 10.0f);
    labelBackground.setOrigin(label.getOrigin() + sf::Vector2f(BACKGROUND_PADDING / 2, -2.0f + BACKGROUND_PADDING / 2));
    // --- [End] - Nametag --- 

    // --- [Begin] - Time ---
    TIMEOUT_TIME = 30.0f;
    timeoutTimer = TIMEOUT_TIME;

    PICKUP_DELAY_TIME = 2.0f;
    pickupDelayTimer  = PICKUP_DELAY_TIME; 
    // --- [End] - Time ---

    // --- [Begin] - Sprites
    sprite.setTexture(*(item->sprite).getTexture());
    sprite.setPosition(hitbox.getPosition());
    // item->sprite.setPosition(hitbox.getPosition());
    // --- [End] - Sprites
}

void Item::update(const float& dt) {
    // if (timeoutTimer > 0) {
    //     timeoutTimer -= dt;
    // }

    if (pickupDelayTimer > 0) {
        pickupDelayTimer -= dt;
    }
}

void Item::draw(sf::RenderTarget& target) const {
    target.draw(labelBackground);
    target.draw(label);

    sf::RectangleShape hitboxShape;
    hitboxShape.setPosition(hitbox.getPosition());
    hitboxShape.setSize(hitbox.getSize());
    hitboxShape.setOutlineColor(sf::Color::Yellow);
    hitboxShape.setOutlineThickness(1.f);
    hitboxShape.setFillColor(sf::Color::Transparent);
    target.draw(hitboxShape);

    if (canPickup()) {
        target.draw(sprite);
    }
    else {
        target.draw(sprite, EntityEffects::get("invincible"));
    }
    // target.draw(item->sprite);
}

sf::FloatRect Item::getHitbox() const {
    return hitbox;
}

bool Item::canPickup() const {
    return pickupDelayTimer <= 0.0f;
}

const std::shared_ptr<ItemData>& Item::getItem() const {
    return item;
}