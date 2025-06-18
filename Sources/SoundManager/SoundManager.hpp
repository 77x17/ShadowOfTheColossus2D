#pragma once

#include <SFML/Audio.hpp>

#include <unordered_map>
#include <string>
#include <memory>

class SoundManager {
private:
    static std::unordered_map<std::string, sf::SoundBuffer> buffers;
    static std::unordered_map<std::string, sf::Sound> sounds;

    static std::unordered_map<std::string, std::unique_ptr<sf::Music>> regionMusic;
    
    static std::string oldRegionMusic;
    static constexpr float FADE_SPEED = 2.0f;
    static float normalVolume;
    static float fadeVolume;
    
public:
    // Load 1 âm thanh vào bộ nhớ
    static void loadSound(const std::string& name, const std::string& filepath);
    static void loadMusic(const std::string& name, const std::string& filepath);

    // Phát âm thanh theo tên
    static void playSound(const std::string& name);
    static bool playMusic(const std::string& name, bool loop);

    // Dừng toàn bộ
    static void stopAll();

    static void stopMusic(const std::string& name);
    static void pauseMusic(const float& dt, const std::string& name);
    static void resumeMusic(const float& dt, const std::string& name);
    static float getMusicVolume(const std::string& name);
    static void setMusicVolume(const std::string& name, const float& volume);

    static void loadSound();
};