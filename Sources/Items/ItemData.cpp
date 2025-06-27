#include "ItemData.hpp"

#include "TextureManager.hpp"

ItemData::ItemData(const std::string& m_name, ItemType m_type, const std::string& m_textureName, ItemRarity m_rarity, const int& m_amount) 
: name(m_name), type(m_type), rarity(m_rarity), amount(m_amount) {
    sprite.setTexture(TextureManager::get(m_textureName));
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
