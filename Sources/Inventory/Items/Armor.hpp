#pragma once

#include "ItemData.hpp"

class Armor : public ItemData {
public:
    float health;
    int   levelRequired;

    Armor(const std::string& _name, ItemType armorType, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity);

    std::string getDetails() const override;

    float getDamage() const override;

    float getHealth() const override;
};

class Helmet : public Armor {
public:
    Helmet(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity);
};

class Chestplate : public Armor {
public:
    Chestplate(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity);
};

class Leggings : public Armor {
public:
    Leggings(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity);
};

class Boots : public Armor {
public:
    Boots(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity);
};