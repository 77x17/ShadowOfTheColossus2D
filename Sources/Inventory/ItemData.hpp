#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "TextureManager.hpp"

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

    ItemData(const std::string& _name, ItemType _type, const std::string& _textureName, ItemRarity _rarity) 
    : name(_name), type(_type), rarity(_rarity) {
        sprite.setTexture(TextureManager::get(_textureName));
        sprite.setScale(2.0f, 2.0f);
    }

    std::string getItemRarityString() const {
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

    sf::Color getItemRarityColor() const {
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

    std::string getInformation() const {
        // return name + "\n\n"
        //      + getDetails() + "\n\n"
        //      + getItemRarityString();
        std::string name_padding(static_cast<int>(name.size()), ' ');
        return name_padding + "\n\n"
             + getDetails() + "\n\n"
             + "";
    }

    virtual std::string getDetails() const = 0;

    virtual float getDamage() const = 0;
    virtual float getHealth() const = 0;
};

class Armor : public ItemData {
public:
    float health;
    int   levelRequired;

    Armor(const std::string& _name, ItemType armorType, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
    : ItemData(_name, armorType, textureName, _rarity), health(_health), levelRequired(_levelRequired) {}

    std::string getDetails() const override {
        std::string healthString = "Health: " + std::to_string(health);
        healthString = healthString.substr(0, healthString.find('.') + 3);

        std::string levelRequiredString = "Lv.Minimum: " + std::to_string(levelRequired);

        return healthString + "\n\n" 
             + levelRequiredString;
    }

    float getDamage() const override {
        return 0.0f;
    }

    float getHealth() const override {
        return health;
    }
};

class Helmet : public Armor {
public:
    Helmet(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
    : Armor(_name, ItemType::Helmet, textureName, _health, _levelRequired, _rarity) {}
};

class Chestplate : public Armor {
public:
    Chestplate(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
    : Armor(_name, ItemType::Chestplate, textureName, _health, _levelRequired, _rarity) {}
};

class Leggings : public Armor {
public:
    Leggings(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
    : Armor(_name, ItemType::Leggings, textureName, _health, _levelRequired, _rarity) {}
};

class Boots : public Armor {
public:
    Boots(const std::string& _name, const std::string& textureName, float _health, int _levelRequired, ItemRarity _rarity) 
    : Armor(_name, ItemType::Boots, textureName, _health, _levelRequired, _rarity) {}
};

class Bow : public ItemData {
public:
    float damage;
    int   levelRequired;

    Bow(const std::string& _name, const std::string& textureName, float _damage, int _levelRequired, ItemRarity _rarity) 
    : ItemData(_name, ItemType::Weapon, textureName, _rarity), damage(_damage), levelRequired(_levelRequired) {}

    std::string getDetails() const override {
        std::string damageString = "Damage: " + std::to_string(damage);
        damageString = damageString.substr(0, damageString.find('.') + 3);

        std::string levelRequiredString = "Lv.Minimum: " + std::to_string(levelRequired);

        return damageString + "\n\n" 
             + levelRequiredString;
    }

    float getDamage() const override {
        return damage;
    }

    float getHealth() const override {
        return 0.0f;
    }
};