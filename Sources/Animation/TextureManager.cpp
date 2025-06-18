#include "TextureManager.hpp"

// Tải một texture từ file và lưu bằng một ID
void TextureManager::load(const std::string& id, const std::string& path) {
    auto texture = std::make_unique<sf::Texture>();

    if (texture->loadFromFile(path)) {
        texture->setSmooth(false);
        textures[id] = std::move(texture);
    } 
    else {
        std::cerr << "Failed to load texture: " << path << '\n';
    }
}

// Lấy một tham chiếu hằng (const reference) đến texture đã được tải
const sf::Texture& TextureManager::get(const std::string& id) {
    return *textures.at(id); 
}

void TextureManager::loadSprite() {
    load("playerSprite", "Sprites/player.png");
    load("playerShadow", "Sprites/playerShadow.png");
    load("arrow"       , "Sprites/arrow.png");

    load("alert"    , "Sprites/alert.png");
    load("batSprite", "Sprites/bat.png");
    load("batShadow", "Sprites/batShadow.png");
    load("batDead"  , "Sprites/batDead.png");
    
    load("eyeSprite", "Sprites/eye.png");
    load("eyeShadow", "Sprites/eyeShadow.png");
    load("eyeDead"  , "Sprites/eyeDead.png");
    load("fireball" , "Sprites/fireball.png");

    load("npc_00", "Sprites/npc_00.png");
    load("npc_01", "Sprites/npc_01.png");
    load("npc_02", "Sprites/npc_02.png");
    load("npc_03", "Sprites/npc_03.png");
}