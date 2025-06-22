#pragma once

#include "ItemData.hpp"

class Bow : public ItemData {
public:
    float damage;
    int   levelRequired;

    Bow(const std::string& _name, const std::string& textureName, float _damage, int _levelRequired, ItemRarity _rarity);

    std::string getDetails() const override;

    float getDamage() const override;

    float getHealth() const override;
};