#pragma once

#include <SFML/Audio.hpp>

#include <unordered_map>
#include <string>

class SoundManager {
private:
    static std::unordered_map<std::string, sf::SoundBuffer> buffers;
    static std::unordered_map<std::string, sf::Sound> sounds;
    
public:
    // Load 1 âm thanh vào bộ nhớ
    static void loadSound(const std::string& name, const std::string& filepath);

    // Phát âm thanh theo tên
    static void playSound(const std::string& name);

    // Dừng toàn bộ
    static void stopAll();

};