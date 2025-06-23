#pragma once

#include "ItemData.hpp"

class EquipItem : public ItemData {
public:
    int levelRequired;
    
    EquipItem(const std::string& _name, ItemType type, const std::string& textureName, int _levelRequired, ItemRarity _rarity);
    virtual ~EquipItem() = default;

    virtual std::string getDetails() const override = 0;

    virtual float getDamage() const;

    virtual float getHealth() const;

    virtual std::shared_ptr<ItemData> clone() const override = 0;
};