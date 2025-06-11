#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Animation.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bat.hpp"
#include "Eye.hpp"
#include "SoundManager.hpp"
#include "TileMap.hpp"

void loadSprite() {
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
}

void loadSound() {
    SoundManager::loadSound("arrow"     , "Sounds/arrow.wav");
    SoundManager::loadSound("roll"      , "Sounds/roll.wav");
    SoundManager::loadSound("playerHurt", "Sounds/playerHurt.wav");
    SoundManager::loadSound("enemyHurt" , "Sounds/enemyHurt.wav");
}

void loadMap(TileMap& map) {
    map.load("Maps/test.tmx", "Maps/overworld.png");

    map.scale(2.f, 2.f);
    map.updateCollisionRects();
}

void loadEnemy(std::vector<Enemy*>& enemys) {
    enemys.push_back(new Bat(950, 200));
    enemys.push_back(new Bat(1250, 200));
    enemys.push_back(new Bat(1550, 200));
    enemys.push_back(new Bat(1850, 200));
    
    enemys.push_back(new Eye(1400, 500));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shadow Of The Colossus 2D", sf::Style::Close);
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    loadSprite();
    loadSound();

    TileMap map;
    loadMap(map);

    Player player(300, 300);

    std::vector<Enemy*> enemys;
    loadEnemy(enemys);

    sf::Clock clock;
    bool      isMinimized  = false;
    bool      isFullscreen = false;
    sf::View  view         = window.getView();
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
                else if (event.key.code == sf::Keyboard::F11) {
                    isFullscreen = !isFullscreen;

                    window.close();
                    if (isFullscreen) {
                        window.create(sf::VideoMode::getDesktopMode(), "Shadow Of The Colossus 2D", sf::Style::Fullscreen);
                    } else {
                        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shadow Of The Colossus 2D", sf::Style::Close);
                    }
                    // window.setFramerateLimit(60);
                    window.setVerticalSyncEnabled(true);
                    
                    sf::Vector2u windowSize = window.getSize();
                    float aspectRatio = static_cast<float>(windowSize.x) / windowSize.y;

                    view = window.getView();
                    view.setSize(WINDOW_HEIGHT * aspectRatio, WINDOW_HEIGHT);  
                    view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
                }
            }
        }
        
        if (isMinimized) {
            // pause rendering or music here if needed
            continue;
        }

        player.update(dt, window, map.getCollisionRects());

        // Điều chỉnh camera theo player
        player.updateView(dt, view);
        
        for (Enemy* enemy : enemys) if (enemy->calculateDistance(player) <= LOADING_DISTANCE) {
            enemy->update(dt, player, map.getCollisionRects());
        }

        if (!player.isAlive()) {
            for (Enemy* enemy : enemys) {
                enemy->respawn();
            }
        }

        window.clear(sf::Color::White);
        window.setView(view);

        window.draw(map);
        
        player.draw(window);
        for (Enemy* enemy : enemys) if (enemy->calculateDistance(player) <= LOADING_DISTANCE) {
            enemy->draw(window);
        }

        window.display();
    }

    return 0;
}