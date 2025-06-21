#pragma once

#include "Npc.hpp"

class MerchantNpc : public Npc {
private:
    bool shopVisible = false;

    std::vector<BagSlot>* bagSlots = nullptr;

public:
    MerchantNpc(const sf::FloatRect& newHitbox, const std::string& name, const std::string& spriteName);

    void update(const float& dt) override;

    void interactWithPlayer(Player& player) override;

    bool isInteractWithPlayer() const;
};
