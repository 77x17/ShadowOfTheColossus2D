#include "Weapon.hpp"

Bow::Bow(const std::string& m_name, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_damage) 
: EquipItem(m_name, ItemType::Weapon, textureName, m_levelRequired, m_rarity), damage(m_damage) {}

std::string Bow::getDetails() const {
    std::string damageString = "Damage: " + std::to_string(damage);
    damageString = damageString.substr(0, damageString.find('.') + 3);

    std::string levelRequiredString = "Lv.Minimum: " + std::to_string(levelRequired);

    return damageString + "\n\n" 
            + levelRequiredString;
}

float Bow::getDamage() const {
    return damage;
}

float Bow::getHealth() const {
    return 0.0f;
}
