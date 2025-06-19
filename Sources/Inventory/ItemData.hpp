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

class ItemData {
public:
    std::string name;
    ItemType    type;
    sf::Sprite  sprite;

    ItemData(const std::string& _name, ItemType _type, const std::string& _textureName) 
    : name(_name), type(_type) {
        sprite.setTexture(TextureManager::get(_textureName));
        sprite.setScale(2.0f, 2.0f);
    }

    virtual std::string getInformation() const {
        return name;
    }

    virtual float getDamage() const = 0;
    virtual float getHealth() const = 0;
};

class Helmet : public ItemData {
public:
    float health;
    int   levelRequired;

    Helmet(const std::string& _name, const std::string& textureName, float _health, int _levelRequired) 
    : ItemData(_name, ItemType::Helmet, textureName), health(_health), levelRequired(_levelRequired) {

    }

    std::string getInformation() const override {
        std::string healthString = "Health: " + std::to_string(health);
        healthString = healthString.substr(0, healthString.find('.') + 3);

        std::string levelRequiredString = "Lv.Minimum: " + std::to_string(levelRequired);

        return ItemData::getInformation() + "\n\n" 
             + healthString + "\n\n" 
             + levelRequiredString;
    }

    float getDamage() const override {
        return 0.0f;
    }

    float getHealth() const override {
        return health;
    }
};

class Bow : public ItemData {
public:
    float damage;
    int   levelRequired;

    Bow(const std::string& _name, const std::string& textureName, float _damage, int _levelRequired) 
    : ItemData(_name, ItemType::Weapon, textureName), damage(_damage), levelRequired(_levelRequired) {

    }

    std::string getInformation() const override {
        std::string damageString = "Damage: " + std::to_string(damage);
        damageString = damageString.substr(0, damageString.find('.') + 3);

        std::string levelRequiredString = "Lv.Minimum: " + std::to_string(levelRequired);

        return ItemData::getInformation() + "\n\n" 
             + damageString + "\n\n" 
             + levelRequiredString;
    }

    float getDamage() const override {
        return damage;
    }

    float getHealth() const override {
        return 0.0f;
    }
};