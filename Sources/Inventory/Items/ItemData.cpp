#include "ItemData.hpp"

#include "TextureManager.hpp"

ItemData::ItemData(const std::string& _name, ItemType _type, const std::string& _textureName, ItemRarity _rarity) 
: name(_name), type(_type), rarity(_rarity) {
    sprite.setTexture(TextureManager::get(_textureName));
    sprite.setScale(2.0f, 2.0f);
}

std::string ItemData::getItemRarityString() const {
    switch (rarity) {
        case ItemRarity::Normal:
            return "Normal";
            break;
        case ItemRarity::Unique:
            return "Unique";
            break;
        case ItemRarity::Rare:
            return "Rare";
            break;
        case ItemRarity::Legendary:
            return "Legendary";
            break;
        case ItemRarity::Mythic:
            return "Mythic";
            break;
        default:
            std::cerr << "[Bug] - ItemData.hpp - getInformation()\n";
            break;
    }
    return std::string();
}

sf::Color ItemData::getItemRarityColor() const {
    switch (rarity) {
        case ItemRarity::Normal:
            return sf::Color::White;
            break;
        case ItemRarity::Unique:
            return sf::Color::Yellow;
            break;
        case ItemRarity::Rare:
            return sf::Color::Magenta;
            break;
        case ItemRarity::Legendary:
            return sf::Color::Cyan;
            break;
        case ItemRarity::Mythic:
            return sf::Color::Red;
            break;
        default:
            std::cerr << "[Bug] - ItemData.hpp - getInformation()\n";
            break;
    }
    return sf::Color::Black;
}

std::string ItemData::getInformation() const {
    // return name + "\n\n"
    //      + getDetails() + "\n\n"
    //      + getItemRarityString();
    std::string name_padding(static_cast<int>(name.size()), ' ');
    std::string detaisString = getDetails();

    if (detaisString == std::string()) {
        return name_padding + "\n"
             + "\n";
    }
    return name_padding + "\n\n"
            + getDetails() + "\n\n"
            + "\n";
}

float ItemData::getDamage() const {
    return 0.0f;
}

float ItemData::getHealth() const {
    return 0.0f;
}

