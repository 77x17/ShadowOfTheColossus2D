#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Animation.hpp"
#include "Player.hpp"
#include "Bat.hpp"
#include "Eye.hpp"
#include "SoundManager.hpp"
#include "TileMap.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shadow Of The Colossus 2D");
    window.setFramerateLimit(60);
    
    TextureManager::load("playerSprite", "Sprites/player.png");
    TextureManager::load("playerShadow", "Sprites/playerShadow.png");
    TextureManager::load("arrow"       , "Sprites/arrow.png");

    TextureManager::load("alert"    , "Sprites/alert.png");
    TextureManager::load("batSprite", "Sprites/bat.png");
    TextureManager::load("batShadow", "Sprites/batShadow.png");
    TextureManager::load("batDead"  , "Sprites/batDead.png");
    
    TextureManager::load("eyeSprite", "Sprites/eye.png");
    TextureManager::load("eyeShadow", "Sprites/eyeShadow.png");
    TextureManager::load("eyeDead"  , "Sprites/eyeDead.png");
    TextureManager::load("fireball" , "Sprites/fireball.png");

    SoundManager::loadSound("arrow"     , "Sounds/arrow.wav");
    SoundManager::loadSound("roll"      , "Sounds/roll.wav");
    SoundManager::loadSound("playerHurt", "Sounds/playerHurt.wav");
    SoundManager::loadSound("enemyHurt" , "Sounds/enemyHurt.wav");

    bool isMinimized = false;
    sf::View view = window.getView();
    sf::Clock clock;

    TileMap map;
    if (!map.load("Maps/test.tmx", "Maps/overworld.png")) {
        return -1;
    }
    map.scale(2.f, 2.f);
    map.updateCollisionRects();

    Player player(300, 300);

    std::vector<Bat> bats;
    bats.push_back(Bat(950, 200));
    bats.push_back(Bat(1250, 200));
    bats.push_back(Bat(1550, 200));
    bats.push_back(Bat(1850, 200));
    
    std::vector<Eye> eyes;
    eyes.push_back(Eye(1400, 500));

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::LostFocus) {
                isMinimized = true;
            }
            else if (event.type == sf::Event::GainedFocus) {
                isMinimized = false;
            }
            else if (event.type == sf::Event::Resized) {
                float aspectRatio = static_cast<float>(event.size.width) / event.size.height;
                
                view = window.getView();
                view.setSize(WINDOW_HEIGHT * aspectRatio, WINDOW_HEIGHT);  // giữ chiều cao cố định
                view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
        
        if (isMinimized) {
            // pause rendering or music here if needed
            continue;
        }

        if (player.isAlive()) {
            player.update(dt, window, map.getCollisionRects());
        }
        else {
            player.respawn();
            for (Bat& bat : bats) {
                bat.respawn();
            }
            for (Eye& eye : eyes) {
                eye.respawn();
            }
        }
        for (Bat& bat : bats) if (bat.calculateDistance(player) <= LOADING_DISTANCE) {
            bat.update(dt, player, map.getCollisionRects());
        }
        for (Eye& eye : eyes) if (eye.calculateDistance(player) <= LOADING_DISTANCE) {
            eye.update(dt, player, map.getCollisionRects());
        }

        // Điều chỉnh camera theo player
        sf::Vector2f currentCenter = view.getCenter();
        sf::Vector2f targetCenter  = player.getPosition();
        sf::Vector2f lerped        = currentCenter + 0.1f * (targetCenter - currentCenter);
        view.setCenter(lerped);
        window.setView(view);

        window.clear(sf::Color::White);
        window.draw(map);
        
        player.draw(window);
        for (Bat& bat : bats) if (bat.calculateDistance(player) <= LOADING_DISTANCE) {
            if (bat.isAlive()) {
                bat.draw(window);
            }
            else {
                bat.respawn();
            }
        }
        for (Eye& eye : eyes) if (eye.calculateDistance(player) <= LOADING_DISTANCE) {
            if (eye.isAlive()) {
                eye.draw(window);
            }
            else {
                eye.respawn();
            }
        }

        window.display();
    }

    return 0;
}