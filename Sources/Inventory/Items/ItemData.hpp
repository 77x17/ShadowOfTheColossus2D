#pragma once

#include <SFML/Graphics.hpp>
#include <string>

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

    ItemData(const std::string& _name, ItemType _type, const std::string& _textureName, ItemRarity _rarity);

    std::string getItemRarityString() const;
    sf::Color getItemRarityColor() const;
    std::string getInformation() const;
    virtual std::string getDetails() const = 0;

    virtual float getDamage() const;
    virtual float getHealth() const;
};
