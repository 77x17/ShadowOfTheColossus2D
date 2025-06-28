#include "SoundManager.hpp"

#include <iostream>

std::unordered_map<std::string, sf::SoundBuffer> SoundManager::buffers;
std::unordered_map<std::string, sf::Sound> SoundManager::sounds;
std::unordered_map<std::string, std::unique_ptr<sf::Music>> SoundManager::regionMusic;
std::string SoundManager::oldRegionMusic = std::string();
float SoundManager::normalVolume = 100.0f;
float SoundManager::fadeVolume   = 100.0f;

void SoundManager::loadSounds() {
    loadSound("arrow"     , "Assets/Sounds/arrow.wav");
    loadSound("roll"      , "Assets/Sounds/roll.wav");
    loadSound("playerHurt", "Assets/Sounds/playerHurt.wav");
    loadSound("playerDie" , "Assets/Sounds/playerDie.wav");
    loadSound("enemyHurt" , "Assets/Sounds/enemyHurt.wav");
    loadSound("enemyDie"  , "Assets/Sounds/enemyDie.wav");
    loadSound("levelUp"   , "Assets/Sounds/levelUp.wav");
    loadSound("talk"      , "Assets/Sounds/talk.wav");
    loadSound("updateQuest", "Assets/Sounds/updateQuest.wav");
    loadSound("finishedQuest", "Assets/Sounds/finishedQuest.wav");
    loadSound("menuOpen"  , "Assets/Sounds/menu-open.wav");
    loadSound("menuClose" , "Assets/Sounds/menu-close.wav");
    loadSound("pickupItem", "Assets/Sounds/pickupItem.wav");
    loadSound("payment"   , "Assets/Sounds/payment.wav");
    loadSound("equipItem", "Assets/Sounds/equipItem.wav");
    loadSound("blockEquipItem", "Assets/Sounds/blockEquipItem.wav");
    loadSound("dash", "Assets/Sounds/roll.wav");
    loadSound("fireball", "Assets/Sounds/fireball.wav");
    loadSound("menutap" , "Assets/Sounds/menutap.wav");
    // loadMusic("region0"   , "Assets/Sounds/Salted - Wynn OST - 05 Detlas Suburb.ogg");
    // loadMusic("region1"   , "Assets/Sounds/Salted - Wynn OST - 04 Gavel Journey.ogg");
    // setMusicVolume("region0", 50.0f);
    // setMusicVolume("region1", 50.0f);
}

void SoundManager::loadSound(const std::string& name, const std::string& filepath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        std::cerr << "Failed to load " << filepath << '\n';
        return;
    }
    buffers[name] = buffer;
    sounds[name].setBuffer(buffers[name]);
}

void SoundManager::loadMusic(const std::string& name, const std::string& filepath) {
    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(filepath)) {
        std::cerr << "Failed to load music: " << filepath << '\n';
        return;
    }

    regionMusic[name] = std::move(music);  // OK vì unique_ptr có thể move
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

bool SoundManager::playMusic(const std::string& name, bool loop) {
    auto it = regionMusic.find(name);
    if (it != regionMusic.end()) {
        it->second->setLoop(loop);
        it->second->play();
        // it->second->setVolume(100); 
    } else {
        std::cerr << "Music not found: " << name << "\n";
    }
    
    return true;
}

void SoundManager::stopMusic(const std::string& name) {
    auto it = regionMusic.find(name);
    if (it != regionMusic.end()) {
        it->second->stop();
    }
}

void SoundManager::pauseMusic(const float& dt, const std::string& name) {
    auto it = regionMusic.find(name);
    if (it != regionMusic.end()) {
        it->second->pause();
    }
}

void SoundManager::resumeMusic(const float& dt, const std::string& name) {
    if (oldRegionMusic == name) {
        if (getMusicVolume(oldRegionMusic) < normalVolume) {
            fadeVolume += (normalVolume - fadeVolume) * FADE_SPEED * dt;
            if (std::abs(normalVolume - fadeVolume) < 0.01f) {
                fadeVolume = normalVolume;
            }
            setMusicVolume(oldRegionMusic, fadeVolume);
        }

        return;
    }
    
    if (getMusicVolume(oldRegionMusic) > 0) {
        fadeVolume += (0 - fadeVolume) * FADE_SPEED * dt;
        if (std::abs(0 - fadeVolume) < 0.01f) {
            fadeVolume = 0;
        }
        setMusicVolume(oldRegionMusic, fadeVolume);
        return;
    }

    pauseMusic(dt, oldRegionMusic);

    oldRegionMusic = name;
    auto it = regionMusic.find(name);
    if (it != regionMusic.end()) {
        sf::Music::Status status = it->second->getStatus();
        if (status != sf::Music::Playing) {
            it->second->setLoop(true);
            it->second->play();
        }
    }
    
}

float SoundManager::getMusicVolume(const std::string& name) {
    auto it = regionMusic.find(name);
    if (it != regionMusic.end()) {
        return it->second->getVolume();
    }
    return 0;
}

void SoundManager::setMusicVolume(const std::string& name, const float& volume) {
    auto it = regionMusic.find(name);
    if (it != regionMusic.end()) {
        it->second->setVolume(volume);
    }
}