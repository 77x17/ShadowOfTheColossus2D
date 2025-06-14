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

#include "Quest.hpp"
#include "Npc.hpp"

#include "ShaderManager.hpp"

sf::Font Font::font;

std::unordered_map<std::string, sf::Texture> TextureManager::textures;

std::unordered_map<std::string, sf::SoundBuffer> SoundManager::buffers;
std::unordered_map<std::string, sf::Sound> SoundManager::sounds;

std::unordered_map<std::string, std::unique_ptr<sf::Shader>> ShaderManager::shaders;

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

void loadShader() {
    ShaderManager::load("invincible", "Shaders/effect.frag", {
        {"invincibleAmount", 0.2f},
        {"flashAmount", 0.5f}
    });

    ShaderManager::load("flash", "Shaders/effect.frag", {
        {"invincibleAmount", 0.2f},
        {"flashAmount", 1.0f}
    });
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
    // map.load("Maps/test.tmx", "Maps/overworld.png");
    map.load("Maps/test.tmx", {
        {"overworld", "Maps/overworld.png"},
        {"CastleWalls", "Maps/CastleWalls.png"},
        {"medium_oak_tree_static", "Maps/medium_oak_tree_static.png"},
        {"big_oak_tree_static", "Maps/big_oak_tree_static.png"}
    });


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

void loadNpc(std::vector<Npc>& npcs, const std::unordered_map<int, sf::FloatRect>& npcRects) {
    npcs.emplace_back(
        0,
        std::vector<std::string>{
            "A",
            "B",
            "C"
        },
        1,
        npcRects.at(0)
    );
    npcs.emplace_back(
        1,
        std::vector<std::string>{
            "D",
            "E",
            "F"
        },
        3,
        npcRects.at(1)
    );
}

void loadQuests(std::vector<Quest>& quests) {
    quests.push_back(Quest(0, "Bat Hunt", "Slaying Bats", 40));
    quests.back().addObjective(std::make_shared<KillMonsterObjective>("Bat Lv.1", 1));
    quests.back().addObjective(std::make_shared<KillMonsterObjective>("Bat Lv.1", 2));

    quests.push_back(Quest(1, "Eye Hunt", "Help the villagers slaying Eyes", 200));
    quests.back().addObjective(std::make_shared<KillMonsterObjective>("Eye Lv.5", 1));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shadow Of The Colossus 2D", sf::Style::Close);
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    Font::font.loadFromFile("Fonts/Roboto_Mono.ttf");
    loadSprite();
    loadShader();
    loadSound();

    TileMap map;
    loadMap(map);

    std::vector<Enemy*> enemys;
    loadEnemy(enemys, map.getEnemyRects());

    std::vector<Npc> npcs;
    loadNpc(npcs, map.getNpcRects());
    
    std::vector<Quest> quests;
    loadQuests(quests);

    sf::Vector2f PlayerTiles = sf::Vector2f(
        78, 
        110
    );
    Player player(PlayerTiles.x * TILE_SIZE, PlayerTiles.y * TILE_SIZE, 50.0f, std::move(quests));
    UI ui;

    ui.generateMinimapTexture(map);

    sf::Clock clock;
    bool      isMinimized  = false;
    bool      isFullscreen = false;
    sf::View  view         = window.getView();
    view.setCenter(PlayerTiles.x * TILE_SIZE, PlayerTiles.y * TILE_SIZE);
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
                else if (event.key.code == sf::Keyboard::Q) {
                    ui.updateQuestsBox();
                }
                else if (event.key.code == sf::Keyboard::Hyphen) {
                    ui.updateMinimapBoxSize(false);
                }
                else if (event.key.code == sf::Keyboard::Equal) {
                    ui.updateMinimapBoxSize(true);
                }
                else if (event.key.code == sf::Keyboard::M) {
                    ui.openMap(uiView.getSize());
                }
            }
        }
        
        if (isMinimized) {
            // pause rendering or music here if needed
            continue;
        }

        player.update(dt, window, map.getCollisionRects(), npcs);
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