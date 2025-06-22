#include "Weapon.hpp"

Bow::Bow(const std::string& _name, const std::string& textureName, float _damage, int _levelRequired, ItemRarity _rarity) 
: ItemData(_name, ItemType::Weapon, textureName, _rarity), damage(_damage), levelRequired(_levelRequired) {}

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
