#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

enum class ItemType {
    Armor,
    Helmet,
    Chestplate,
    Leggings,
    Boots,
    Ring,
    Amulet,
    Shield, 
    Weapon,
    Material,
    Consumable,
    Quest
};

enum class ItemRarity {
    Normal,
    Unique,
    Rare,
    Legendary,
    Mythic
};

class ItemData {
public:
    std::string name;
    ItemType    type;
    sf::Sprite  sprite;
    ItemRarity  rarity;
    int         amount;

    ItemData(const std::string& m_name, ItemType m_type, const std::string& m_textureName, ItemRarity m_rarity, const int& m_amount = 1);
    virtual ~ItemData() = default;

    std::string getItemRarityString() const;
    sf::Color getItemRarityColor() const;
    std::string getInformation() const;
    virtual std::string getDetails() const = 0;
    virtual std::shared_ptr<ItemData> clone() const = 0;
};
