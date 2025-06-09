#pragma once

#include <SFML/Audio.hpp>

#include <unordered_map>
#include <string>

class SoundManager {
private:
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    
public:
    // Load 1 âm thanh vào bộ nhớ
    bool loadSound(const std::string& name, const std::string& filepath);

    // Phát âm thanh theo tên
    void playSound(const std::string& name);

    // Dừng toàn bộ
    void stopAll();

};