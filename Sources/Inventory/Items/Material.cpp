#include "Material.hpp"

Orb::Orb(const std::string& _name, const std::string& textureName, ItemRarity _rarity) 
: ItemData(_name, ItemType::Material, textureName, _rarity) {}

std::string Orb::getDetails() const {
    return std::string();
}
