// #include "SoundManager.hpp"

// #include <iostream>

// bool SoundManager::loadSound(const std::string& name, const std::string& filepath) {
//     sf::SoundBuffer& buffer = soundBuffers[name];

//     if (!buffer.loadFromFile(filepath)) {
//         std::cerr << "Failed to load sound: " << filepath << "\n";
//         soundBuffers.erase(name); 
//         return false;
//     }

//     sounds[name].setBuffer(buffer);
//     return true;
// }

// void SoundManager::playSound(const std::string& name) {
//     auto it = sounds.find(name);
//     if (it != sounds.end()) {
//         it->second.play();
//     } else {
//         std::cerr << "Sound not found: " << name << "\n";
//     }
// }

// void SoundManager::stopAll() {
//     for (auto& pair : sounds) {
//         pair.second.stop();
//     }
// }