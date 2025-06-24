#pragma once

#include "Npc.hpp"

class MerchantNpc : public Npc {
private:
    bool shopVisible = false;

public:
    MerchantNpc(const sf::FloatRect& newHitbox, const std::string& name, const std::string& spriteName);

    void update(const float& dt) override;

    void interactWithPlayer(Player& player) override;

    void interruptedInteract();

    bool isInteractWithPlayer() const;
};
