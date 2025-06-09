#include <SFML/Audio.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("Sounds/arrow.wav")) {
        std::cerr << "Failed to load arrow.wav\n";
        return 1;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    std::cout << "Sound playing...\n";

    while (sound.getStatus() == sf::Sound::Playing) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Sound finished.\n";
    return 0;
}
