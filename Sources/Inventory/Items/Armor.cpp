#include "Armor.hpp"

Armor::Armor(const std::string& _name, ItemType armorType, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
: ItemData(_name, armorType, textureName, _rarity), health(_health), levelRequired(_levelRequired) {}

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

Helmet::Helmet(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
: Armor(_name, ItemType::Helmet, textureName, _health, _levelRequired, _rarity) {}

Chestplate::Chestplate(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
: Armor(_name, ItemType::Chestplate, textureName, _health, _levelRequired, _rarity) {}

Leggings::Leggings(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
: Armor(_name, ItemType::Leggings, textureName, _health, _levelRequired, _rarity) {}

Boots::Boots(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
: Armor(_name, ItemType::Boots, textureName, _health, _levelRequired, _rarity) {}
