#include "SoundManager.hpp"

#include <iostream>

void SoundManager::loadSound(const std::string& name, const std::string& filepath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        std::cerr << "Failed to load " << filepath << '\n';
        return;
    }
    buffers[name] = buffer;
    sounds[name].setBuffer(buffers[name]);
}

void SoundManager::playSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second.play();
    } else {
        std::cerr << "Sound not found: " << name << "\n";
    }
}

void SoundManager::stopAll() {
    for (auto& pair : sounds) {
        pair.second.stop();
    }
}