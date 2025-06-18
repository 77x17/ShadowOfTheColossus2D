#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "TextureManager.hpp"

enum class ItemType {
    Helmet,
    Chestplate,
    Leggings,
    Boots,
    Ring,
    Amulet,
    Shield, 
    Weapon, 
    Consumable,
    Quest
};

class Item {
public:
    std::string name;
    ItemType    type;
    sf::Sprite  sprite;

    Item(const std::string& _name, ItemType _type, const std::string& _textureName) 
    : name(_name), type(_type) {
        sprite.setTexture(TextureManager::get(_textureName));
        sprite.setScale(2.0f, 2.0f);
    }
};

class Bow : public Item {
public:
    float damage;
    Bow(const std::string& _name, float _damage) 
    : Item(_name, ItemType::Weapon, "bow"), damage(_damage) {

    }
};