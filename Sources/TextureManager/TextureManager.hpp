#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <iostream>
#include <memory>

class TextureManager {
private:
    // Dùng map để lưu trữ texture, với key là một ID (string) và value là đối tượng texture
    static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;

public:
    // Tải một texture từ file và lưu bằng một ID
    static void load(const std::string& id, const std::string& path);

    // Lấy một tham chiếu hằng (const reference) đến texture đã được tải
    static const sf::Texture& get(const std::string& id);

    static void loadSprites();
};