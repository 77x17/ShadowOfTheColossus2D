#include "MerchantNpc.hpp"

MerchantNpc::MerchantNpc(const sf::FloatRect& newHitbox, const std::string& name, const std::string& spriteName)
: Npc(newHitbox, name, spriteName) {}

void MerchantNpc::update(const float& dt) {
    Npc::update(dt);

    if (collisionWithPlayer) {
        collisionWithPlayer = false;
    }
    else {
        shopVisible = false;
    }
}

void MerchantNpc::interactWithPlayer(Player& player) {
    if (interactCooldownTimer > 0) {
        return;
    }

    shopVisible = !shopVisible;

    if (shopVisible) {
        // bagSlots = player.getBagSlots();
    }
    else {
        bagSlots = nullptr;
    }

    interactCooldownTimer = INTERACT_COOLDOWN;
}

bool MerchantNpc::isInteractWithPlayer() const {
    return shopVisible;
}