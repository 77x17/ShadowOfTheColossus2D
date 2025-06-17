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