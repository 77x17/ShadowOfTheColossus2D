#include "EquipItem.hpp"

EquipItem::EquipItem(const std::string& _name, ItemType type, const std::string& textureName, int _levelRequired, ItemRarity _rarity)
: ItemData(_name, type, textureName, _rarity), levelRequired(_levelRequired) {}

float EquipItem::getDamage() const {
    return 0.0f;
}

float EquipItem::getHealth() const {
    return 0.0f;
}

