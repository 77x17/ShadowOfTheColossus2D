#pragma once

#include "ItemData.hpp"

class Orb : public ItemData {
public:
    Orb(const std::string& _name, const std::string& textureName, ItemRarity _rarity);

    std::string getDetails() const override;

    std::shared_ptr<ItemData> clone() const override;
};