#include "Armor.hpp"

Armor::Armor(const std::string& m_name, ItemType armorType, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_health) 
: EquipItem(m_name, armorType, textureName, m_levelRequired, m_rarity), health(m_health) {}

std::string Armor::getDetails() const {
    std::string healthString = "Health: " + std::to_string(health);
    healthString = healthString.substr(0, healthString.find('.') + 3);

    std::string levelRequiredString = "Lv.Minimum: " + std::to_string(levelRequired);

    return healthString + "\n\n" 
         + levelRequiredString;
}

float Armor::getDamage() const {
    return 0.0f;
}

float Armor::getHealth() const {
    return health;
}

Helmet::Helmet(const std::string& m_name, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_health) 
: Armor(m_name, ItemType::Helmet, textureName, m_levelRequired, m_rarity, m_health) {}

Chestplate::Chestplate(const std::string& m_name, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_health) 
: Armor(m_name, ItemType::Chestplate, textureName, m_levelRequired, m_rarity, m_health) {}

Leggings::Leggings(const std::string& m_name, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_health) 
: Armor(m_name, ItemType::Leggings, textureName, m_levelRequired, m_rarity, m_health) {}

Boots::Boots(const std::string& m_name, const std::string& textureName, int m_levelRequired, ItemRarity m_rarity, float m_health) 
: Armor(m_name, ItemType::Boots, textureName, m_levelRequired, m_rarity, m_health) {}
