#include "TextureManager.hpp"

// Tải một texture từ file và lưu bằng một ID
void TextureManager::load(const std::string& id, const std::string& path) {
    sf::Texture texture;
    if (texture.loadFromFile(path)) {
        textures[id] = texture;
    } 
    else {
        std::cerr << "Failed to load texture: " << path << '\n';
    }
}

// Lấy một tham chiếu hằng (const reference) đến texture đã được tải
const sf::Texture& TextureManager::get(const std::string& id) {
    // .at() sẽ báo lỗi nếu không tìm thấy, an toàn hơn []
    return textures.at(id); 
}