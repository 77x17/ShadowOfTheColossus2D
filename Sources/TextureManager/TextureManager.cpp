#include "TextureManager.hpp"

std::unordered_map<std::string, std::unique_ptr<sf::Texture>> TextureManager::textures;

void TextureManager::loadSprites() {
    // --- [Begin] - Player ---
    load("playerSprite", "Assets/Sprites/player.png");
    load("playerShadow", "Assets/Sprites/playerShadow.png");
    load("arrow"       , "Assets/Sprites/arrow.png");
    // --- [End] ---

    // --- [Begin] - Enemy ---
    load("alert"    , "Assets/Sprites/alert.png");

    load("batSprite", "Assets/Sprites/bat.png");
    load("batShadow", "Assets/Sprites/batShadow.png");
    load("batDead"  , "Assets/Sprites/batDead.png");

    load("vampireBat", "Assets/Sprites/vampireBat.png");

    load("eyeSprite", "Assets/Sprites/eye.png");
    load("eyeShadow", "Assets/Sprites/eyeShadow.png");
    load("eyeDead"  , "Assets/Sprites/eyeDead.png");

    load("slime"    , "Assets/Sprites/slime.png");
    load("slimeDead", "Assets/Sprites/slimeDead.png");

    load("fireball" , "Assets/Sprites/fireball.png");

    load("batBoss", "Assets/Sprites/batBoss.png");
    load("batBossDead", "Assets/Sprites/batBossDead.png");
    // --- [End] ---
    
    // --- [Begin] - Npc ---
    load("npc_00", "Assets/Sprites/npc_00.png");
    load("npc_01", "Assets/Sprites/npc_01.png");
    load("npc_02", "Assets/Sprites/npc_02.png");
    load("npc_03", "Assets/Sprites/npc_03.png");
    // --- [End] ---

    // --- [Begin] - Item ---
    load("bow_00"       , "Assets/Items/bow_00.png");
    load("helmet_00"    , "Assets/Items/helmet_00.png");
    load("chestplate_00", "Assets/Items/chestplate_00.png");
    load("leggings_00"  , "Assets/Items/leggings_00.png");
    load("boots_00"     , "Assets/Items/boots_00.png");
    load("orb"          , "Assets/Items/orb.png");
    // --- [End] ---
}

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