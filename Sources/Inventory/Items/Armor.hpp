#pragma once

#include "EquipItem.hpp"

class Armor : public EquipItem {
public:
    float health;

    Armor(const std::string& _name, ItemType armorType, const std::string& textureName, int _levelRequired, ItemRarity _rarity, float _health);
    virtual ~Armor() = default;

    std::string getDetails() const override;

    float getDamage() const override;

    float getHealth() const override;
};

class Helmet : public Armor {
public:
    Helmet(const std::string& m_name, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_health);
};

class Chestplate : public Armor {
public:
    Chestplate(const std::string& m_name, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_health);
};

class Leggings : public Armor {
public:
    Leggings(const std::string& m_name, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_health);
};

class Boots : public Armor {
public:
    Boots(const std::string& m_name, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_health);
};