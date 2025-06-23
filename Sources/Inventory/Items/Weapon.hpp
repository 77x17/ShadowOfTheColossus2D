#pragma once

#include "EquipItem.hpp"

class Bow : public EquipItem {
public:
    float damage;

    Bow(const std::string& _name, const std::string& textureName, int _levelRequired, ItemRarity _rarity, float _damage);

    std::string getDetails() const override;

    float getDamage() const override;

    float getHealth() const override;
};