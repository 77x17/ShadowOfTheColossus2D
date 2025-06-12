#include <SFML/Graphics.hpp>
#include <memory>
#include <windows.h>

#include "Constants.hpp"

#include "Font.hpp"
#include "SoundManager.hpp"
#include "TileMap.hpp"

#include "Player.hpp"
#include "UI.hpp"

#include "Enemy.hpp"
#include "Bat.hpp"
#include "Eye.hpp"

sf::Font Font::font;

std::unordered_map<std::string, sf::Texture> TextureManager::textures;

std::unordered_map<std::string, sf::SoundBuffer> SoundManager::buffers;
std::unordered_map<std::string, sf::Sound> SoundManager::sounds;

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
    SoundManager::loadSound("playerDie" , "Sounds/playerDie.wav");
    SoundManager::loadSound("enemyHurt" , "Sounds/enemyHurt.wav");
    SoundManager::loadSound("enemyDie"  , "Sounds/enemyDie.wav");
}

void loadMap(TileMap& map) {
    map.load("Maps/test.tmx", "Maps/overworld.png");

    map.scale(2.f, 2.f);
    map.updateObjects();
}

void loadEnemy(std::vector<Enemy*>& enemys, const std::unordered_map<std::string, std::vector<sf::FloatRect>>& enemyRects) {
    for (const auto& pair : enemyRects) {
        for (const sf::FloatRect& rect : pair.second) {
            if (pair.first == "Bat Lv.1") {
                enemys.push_back(new Bat(rect.getPosition().x, rect.getPosition().y));
            }
            else if (pair.first == "Eye Lv.5") {
                enemys.push_back(new Eye(rect.getPosition().x, rect.getPosition().y));
            }
            else {
                std::cerr << "[Bug] - Main.cpp - loadEnemy()\n";
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shadow Of The Colossus 2D", sf::Style::Close);
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    Font::font.loadFromFile("Fonts/Roboto_Mono.ttf");
    loadSprite();
    loadSound();

    TileMap map;
    loadMap(map);

    Player player(300, 300, 5.0f);
    UI ui;

    std::vector<Enemy*> enemys;
    loadEnemy(enemys, map.getEnemyRects());

    sf::Clock clock;
    bool      isMinimized  = false;
    bool      isFullscreen = false;
    sf::View  view         = window.getView();
    sf::View  uiView       = window.getDefaultView();
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::LostFocus) {
                isMinimized = true;

                HWND hwnd = window.getSystemHandle(); // lấy HWND của cửa sổ
                ShowWindow(hwnd, SW_MINIMIZE);        // minimize cửa sổ

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

                    uiView = window.getDefaultView();
                }
            }
        }
        
        if (isMinimized) {
            // pause rendering or music here if needed
            continue;
        }

        player.update(dt, window, map.getCollisionRects(), map.getNPCRects());
        ui.update(dt, player, uiView.getSize());

        // Điều chỉnh camera theo player
        player.updateView(dt, view);
        
        for (Enemy* enemy : enemys) {
            enemy->update(dt, player, map.getCollisionRects());
        }

        window.clear(sf::Color::White);
        window.setView(view);

        window.draw(map);
        
        for (Enemy* enemy : enemys) if (enemy->calculateDistance(player) <= LOADING_DISTANCE) {
            enemy->draw(window);
        }
        player.draw(window);

        window.setView(uiView);
        ui.draw(window);

        window.display();
    }

    return 0;
}