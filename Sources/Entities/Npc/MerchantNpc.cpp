#include "MerchantNpc.hpp"

MerchantNpc::MerchantNpc(const sf::FloatRect& newHitbox, const std::string& name, const std::string& spriteName)
: Npc(newHitbox, name, spriteName) {
    interactText.setString("Press [F] to interact");
    interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width / 2, 
                           interactText.getLocalBounds().top  + interactText.getLocalBounds().height / 2);
}

void MerchantNpc::update(const float& dt) {
    Npc::update(dt);

    if (collisionWithPlayer) {
        collisionWithPlayer = false;
        interactTextOpacity += (255 - interactTextOpacity) * FADE_SPEED * dt;
    }
    else {
        shopVisible = false;
        interactTextOpacity += (0   - interactTextOpacity) * FADE_SPEED * dt;
    }

    if (std::abs(previousInteractTextOpacity - interactTextOpacity) > ZERO_EPSILON) {
        interactText.setFillColor(sf::Color(255, 255, 255, interactTextOpacity));
        interactText.setOutlineColor(sf::Color(0, 0, 0, interactTextOpacity));
        previousInteractTextOpacity = interactTextOpacity;
    }
}

void MerchantNpc::interactWithPlayer(Player& player) {
    if (interactCooldownTimer > 0) {
        return;
    }

    shopVisible = !shopVisible;

    SoundManager::playSound("talk");

    interactCooldownTimer = INTERACT_COOLDOWN;
}

bool MerchantNpc::isInteractWithPlayer() const {
    return shopVisible;
}